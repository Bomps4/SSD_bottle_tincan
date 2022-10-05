#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
#     ||          ____  _ __
#  +------+      / __ )(_) /_______________ _____  ___
#  | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
#  +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
#   ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
#
#  Copyright (C) 2020 Bitcraze AB
#
# Modified by: Lorenzo Lamberti <lorenzo.lamberti@unibo.it>
# Mobified by: Luca Bompani <luca.bompani4@studio.unibo.it>
#
#  AI-deck demo
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  You should have received a copy of the GNU General Public License along with
#  this program; if not, write to the Free Software Foundation, Inc., 51
#  Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
#  Demo for showing streamed JPEG images from the AI-deck example.
#
#  By default this demo connects to the IP of the AI-deck example when in
#  Access point mode.
#
#  The demo works by opening a socket to the AI-deck, downloads a stream of
#  JPEG images and looks for start/end-of-frame for the streamed JPEG images.
#  Once an image has been fully downloaded it's rendered in the UI.
#
#  Note that the demo firmware is continously streaming JPEG files so a single
#  JPEG image is taken from the stream using the JPEG start-of-frame (0xFF 0xD8)
#  and the end-of-frame (0xFF 0xD9).

# notes:
# QVGA  format 320x240 px = 76800
# QQVGA format 160x120 px = 19200
# imgdata average size = 11k or 16k

from os.path import join
import argparse
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk, GdkPixbuf, GLib
import threading
import time
import socket,os,struct
from PIL import Image,ImageDraw

#save image and visualize
import numpy as np
import cv2 
import binascii
import io 

VERBOSE=False
RECEIVE_TIMESTAMP=False #only used for the dataset framework collector

deck_ip = None
deck_port = None
labels=["Bottle","Tincan"]
def decode_bytes(byte_arr):
    cordinates=[struct.unpack('h',byte_arr[i*2:(i+1)*2])[0] for i in range(0,40)]
    # print(cordinates)
    scores=[i for i in struct.iter_unpack('b', byte_arr[80:90])]
    # print(scores)
    classes=[i for i in struct.iter_unpack('b', byte_arr[90:100])]
    seen_boxes=[tuple(cordinates[idx*4:(idx+1)*4])+classes[idx] for idx,i in enumerate(scores) if (scores[idx][0]>=25)]
    
    return seen_boxes,classes

def save_image_bytearray(imgdata, number_of_images):
    decoded = cv2.imdecode(np.frombuffer(imgdata, np.uint8), -1)
    image_name = str(number_of_images)+".jpg"
    try: cv2.imwrite(join(images_folder_path,image_name), decoded)
    except: print('couldnt decode image, data lenght was', len(imgdata))

def save_image_pil(img, name):
    image_name = str(name)+'.png'
    try: img.save(join(images_folder_path,image_name))
    except: print('couldnt decode image')

class ImgThread(threading.Thread):
    def __init__(self, callback):
        threading.Thread.__init__(self, daemon=True)
        self._callback = callback
    def run(self):
        print("Connecting to socket on {}:{}...".format(deck_ip, deck_port))
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((deck_ip, deck_port))
        print("Socket connected")
        RECIEVE_IMAGE=True
        imgdata = None
        imgtext = None
        imgdata_complete = None
        number_of_images = 0
        left_out=0
        starting_point=None

        if SAVE_IMAGES: # create directory to save images
            os.makedirs(images_folder_path, exist_ok=True)
	
        while(1):
            strng = client_socket.recv(512)
            if VERBOSE: print("\nsize of packet received:", len(strng), "\n")
            if VERBOSE: print (binascii.hexlify(strng))
            # Look for start-of-frame and end-of-frame
            start_idx = strng.find(b"\xff\xd8")
            end_idx = strng.find(b"\xff\xd9")

            # Concatenate image data, once finished send it to the UI
            if start_idx >= 0:
                # print("preparo l'immagine")
                imgdata += strng[:start_idx]
                starting_point=imgdata.rfind(b"\x81\r")
                if(starting_point>=0):
                       imgtext=imgdata[starting_point:starting_point+102]
                       imgdata=imgdata[:starting_point]+imgdata[starting_point+102:]
                       
                #put in another variable the complete image
                imgdata_complete = imgdata
				
                #start the acquisition of the new image
                imgdata = strng[start_idx:]

                # search for the footer in the complete_image and ignore it (Temporal fix: the footer is transmitted not at the end of each image so we just discard it to not break the image)
                end_idx = imgdata_complete.find(b"\xff\xd9")
                if end_idx >= 0 and imgdata_complete:
                    imgdata_complete = imgdata_complete[0:end_idx] + imgdata_complete[end_idx+2:]
                
                if RECEIVE_TIMESTAMP: # remove last 8 bytes, which are just timestamp
                    timestamp = imgdata_complete[-8:]
                    imgdata_complete = imgdata_complete[:-8]

                # Now append the jpeg footer at the end of the complete image. We do this before saving or visualizing the image, so it can be dec,y,w,h) oded correctly
                imgdata_complete = imgdata_complete + (b"\xff\xd9") 
                if VERBOSE: print('len strng %d  \t Bytes imgdata  %d\t \n\n' % (len(strng), len(imgdata_complete)-308 )) #308 = len(header)+len(footer)
                
                # if SAVE_IMAGES==True and (number_of_images % 1 ==0 ): #saves just one every 5 images to not overload
                #     save_image_bytearray(imgdata_complete, number_of_images)
                number_of_images+=1
                try: #show frame
                    if(imgtext!=None):
                       boxes,classes=decode_bytes(imgtext[2:])
                    else:
                       boxes=[]
                       classes=[]
                    self._callback(imgdata_complete, number_of_images, boxes,classes)
                    
                except gi.repository.GLib.Error:
                    print ("image not shown")
                    pass 
 
            else: # Continue receiving the image
                if imgdata==None:
                    imgdata=strng
                else:
                    imgdata += strng
            

          
# UI for showing frames from AI-deck example
class FrameViewer(Gtk.Window):

    def __init__(self):
        super(FrameViewer, self).__init__()
        self.frame = None
        self.init_ui()
        self._start = None
        self.set_default_size(374, 294)

    def init_ui(self):            
        self.override_background_color(Gtk.StateType.NORMAL, Gdk.RGBA(0, 0, 0, 1))
        self.set_border_width(20)
        self.set_title("Connecting...")
        self.frame = Gtk.Image()
        f = Gtk.Fixed()
        f.put(self.frame, 10, 10)
        self.add(f)
        self.connect("destroy", Gtk.main_quit)
        self._thread = ImgThread(self._showframe)
        self._thread.start()

    def _update_image(self, pix):

        #print(touple)
        self.frame.set_from_pixbuf(pix)

			

    def _showframe(self, imgdata_complete, im_name, seen_boxes,classes):
        # Add FPS/img size to window title
        if (self._start != None):
            fps = 1 / (time.time() - self._start)
            GLib.idle_add(self.set_title, "{:.1f} fps / {:.1f} kb".format(fps, len(imgdata_complete)/1000))
        self._start = time.time()
        img_loader = GdkPixbuf.PixbufLoader()

        # Try to decode JPEG from the data sent from the stream
        try:
            if(imgdata_complete.find(b"\xff\xd8")!=-1):
                img_decoded= np.array( cv2.imdecode(np.frombuffer(imgdata_complete, np.uint8), -1),dtype=np.uint8)
                im=Image.fromarray(img_decoded)
                draw=ImageDraw.Draw(im)
                for i,j in zip(seen_boxes,classes): 
                    draw.rectangle((i[1],i[0],i[3],i[2]), outline = "yellow")
                    if i[4]<=2 and i[4]>=0 :
                       print(labels[i[4]-1])
                if SAVE_IMAGES:
                    save_image_pil(im, im_name)
                img_byte_arr = io.BytesIO()
                im.save(img_byte_arr, format='PNG')
                img_byte_arr = img_byte_arr.getvalue()
                imgdata_complete=img_byte_arr

            out=img_loader.write(bytearray(imgdata_complete))
            pix = img_loader.get_pixbuf()
            # print(pix)
            GLib.idle_add(self._update_image,pix)
        except gi.repository.GLib.Error:
            print("Could not set image!")
        img_loader.close()

# Args for setting IP/port of AI-deck. Default settings are for when
# AI-deck is in AP mode.
parser = argparse.ArgumentParser(description='Connect to AI-deck JPEG streamer example')
parser.add_argument("-n",  default="192.168.4.1", metavar="ip", help="AI-deck IP")
parser.add_argument("-p", type=int, default='5000', metavar="port", help="AI-deck port")
parser.add_argument('--save_images', help='save images on your pc', action='store_true')
parser.add_argument('--save_images_path', help='folder where images are saved', default='./dataset')
args = parser.parse_args()

SAVE_IMAGES = args.save_images
images_folder_path = args.save_images_path

deck_port = args.p
deck_ip = args.n

fw = FrameViewer()
fw.show_all()
Gtk.main()
