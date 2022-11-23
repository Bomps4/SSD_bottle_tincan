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

#save image and visualize
import numpy as np
import cv2 
import binascii
import io 
# vicon needs
from datetime import datetime

# obj dete
from PIL import Image,ImageDraw


VERBOSE=False
RECEIVE_TIMESTAMP=False #only used for the dataset framework collector
SCORE_THR=128*0.3
TEXTBYTES=104
deck_ip = None
deck_port = None
labels=["Bottle","Tincan"]


def decode_bytes(byte_arr):
    """byte decoding

    Args:
        byte_arr (bytearray):detection inputs as data coordinates

    Returns:
        tuple[lists]: [0] list of y_min,x_min,y_max,x_max,class of bounding boxes, list of scores associated with each bounding boxes
    """ 
	   
    cordinates=[struct.unpack('h',byte_arr[i*2:(i+1)*2])[0] for i in range(0,40)]
    scores=[i for i in struct.iter_unpack('b', byte_arr[80:90])]
    classes=[i for i in struct.iter_unpack('b', byte_arr[90:100])] 
    
  
    seen_boxes=[tuple(cordinates[idx*4:(idx+1)*4])+classes[idx] for idx,i in enumerate(scores) if (scores[idx][0]>=SCORE_THR)]
    return seen_boxes,scores


def save_image_bytearray(imgdata, path, name):
    """saving bytearray data as images

    Args:
        imgdata (bytearray): bytearay of images data 
    """    
    subfolder = 'original'
    path_and_subfolder = join(path, subfolder)
    # create folder tree
    if not os.path.exists(path_and_subfolder):
        os.makedirs(path_and_subfolder)

    decoded = cv2.imdecode(np.frombuffer(imgdata, np.uint8), -1)
    image_name = str(name)+".jpg"
    try: cv2.imwrite(join(path_and_subfolder, image_name), decoded)
    except: print('[ai-deck] --- couldnt decode image, data lenght was', len(imgdata))

def save_image_pil(img, path, name):
    """saving images

    Args:
        img (Image):PIL type for images
        name (string): name to give the image
    """    
    subfolder = 'bbox'
    path_and_subfolder = join(path, subfolder)
    # create folder tree
    if not os.path.exists(path_and_subfolder):
        os.makedirs(path_and_subfolder)

    image_name = str(name)+'.png'
    try: img.save(join(path_and_subfolder, image_name))
    except: print('[ai-deck] --- couldnt decode image')

class ImgThread(threading.Thread):
    def __init__(self, callback, ip, port, time0, save_images, save_images_path,object_detection=True):
        threading.Thread.__init__(self, daemon=True)
        self._callback = callback
        self.time0 = time0
        self.save_images = save_images
        self.save_images_path = save_images_path
        self.deck_ip = ip
        self.deck_port=port
        self.object_detection=object_detection

    def run(self):
        print("[ai-deck] --- Connecting to socket on {}:{}...".format(self.deck_ip, self.deck_port))
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((self.deck_ip, self.deck_port))
        print("[ai-deck] --- Socket connected")

        imgdata = None
        imgdata_complete = None
        number_of_images = 0
        img_buffer=[0,0] #one frame buffer for simple 
        current=True #simple flag for first image in the stream 
        # obj detection
        imgtext = None
        left_out=0
        starting_point=None

        if self.save_images: # create directory to save images
            os.makedirs(self.save_images_path, exist_ok=True)

        while(1):
            strng = client_socket.recv(512)
            if VERBOSE: print("\nsize of packet received:", len(strng), "\n")
            if VERBOSE: print (binascii.hexlify(strng))

            # Look for start-of-frame and end-of-frame
            start_idx = strng.find(b"\xff\xd8")
            end_idx = strng.find(b"\xff\xd9")

            # Concatenate image data, once finished send it to the UI
            if start_idx >= 0:
                #append end of packet
                imgdata += strng[:start_idx]

                if self.object_detection:
                    starting_point=imgdata.rfind(b"\x81\x81\x81\x81")
                    if(starting_point>=0):
                        imgtext=imgdata[starting_point:starting_point+TEXTBYTES]
                        imgdata=imgdata[:starting_point]+imgdata[starting_point+TEXTBYTES:]
                    if(current):
                        img_buffer[0]=imgdata
                        current=False
                    else:
                        img_buffer[1]=img_buffer[0] #image buffer for strange ordering of jpeg header
                        img_buffer[0]=imgdata
                #put in another variable the complete image
                imgdata_complete = imgdata

                #start the acquisition of the new image
                imgdata = strng[start_idx:]
                if self.object_detection:
                    if(img_buffer[1]==0):
                        continue
                    imgdata_complete=img_buffer[1]

                # search for the footer in the complete_image and ignore it (Temporal fix: the footer is transmitted not at the end of each image so we just discard it to not break the image)
                end_idx = imgdata_complete.find(b"\xff\xd9")
                if end_idx >= 0 and imgdata_complete:
                    imgdata_complete = imgdata_complete[0:end_idx] + imgdata_complete[end_idx+2:]
                
                if RECEIVE_TIMESTAMP: # remove last 8 bytes, which are just timestamp
                    timestamp = imgdata_complete[-8:]
                    imgdata_complete = imgdata_complete[:-8]

                # Now append the jpeg footer at the end of the complete image. We do this before saving or visualizing the image, so it can be decoded correctly
                imgdata_complete = imgdata_complete + (b"\xff\xd9")

                if VERBOSE: print('len strng %d  \t Bytes imgdata  %d\t \n\n' % (len(strng), len(imgdata_complete)-308 )) #308 = len(header)+len(footer)

                timestamp = round(1000 * (datetime.now() - self.time0).total_seconds(), 3) # ms?
                if self.save_images==True and (number_of_images % 1 ==0 ): #saves just one every 5 images to not overload
                    save_image_bytearray(imgdata_complete, self.save_images_path, timestamp)
                number_of_images+=1

                try: #show frame
                    if self.object_detection:
                        if(imgtext!=None):
                            boxes, scores=decode_bytes(imgtext[4:])
                        else:
                            boxes=[]     
                            scores=[]
                        self._callback(imgdata_complete, timestamp, boxes, scores)
                    else:
                        self._callback(imgdata_complete)
                except gi.repository.GLib.Error:
                    print ("[ai-deck] --- image not shown")
                    pass

            else: # Continue receiving the image
                if imgdata==None:
                    imgdata=strng
                else:
                    imgdata += strng


          
# UI for showing frames from AI-deck example
class FrameViewer(Gtk.Window, threading.Thread):

    def __init__(self, deck_ip, deck_port, time0, save_images, save_images_path,object_detection=False):
        super(FrameViewer, self).__init__()
        self.time0 = time0
        self.save_images = save_images
        self.save_images_path = save_images_path     
        self.deck_ip = deck_ip
        self.deck_port = deck_port 
        self.frame = None
        self._start = None
        self.set_default_size(374, 374)
        self.object_detection = object_detection
        self.init_ui()
        

    def init_ui(self):            
        self.override_background_color(Gtk.StateType.NORMAL, Gdk.RGBA(0, 0, 0, 1))
        self.set_border_width(20)
        self.set_title("Connecting...")
        self.frame = Gtk.Image()
        f = Gtk.Fixed()
        f.put(self.frame, 10, 10)
        self.add(f)
        self.connect("destroy", Gtk.main_quit)
        print(vars(self))
        self._thread = ImgThread(self._showframe, self.deck_ip, self.deck_port, self.time0, self.save_images, self.save_images_path,self.object_detection)
        self._thread.start()

    def _update_image(self, pix):
        self.frame.set_from_pixbuf(pix)

    def _showframe(self, imgdata_complete, im_name, seen_boxes, scores):
        # Add FPS/img size to window title
        if (self._start != None):
            fps = 1 / (time.time() - self._start)
            GLib.idle_add(self.set_title, "{:.1f} fps / {:.1f} kb".format(fps, len(imgdata_complete)/1000))
        self._start = time.time()

        # Try to decode JPEG from the data sent from the stream
        try:
            img_loader = GdkPixbuf.PixbufLoader()
            if self.object_detection:
                if(imgdata_complete.find(b"\xff\xd8")!=-1):
                    buffer=np.frombuffer(imgdata_complete, np.uint8)
                    decoded=cv2.imdecode(buffer, -1)
                    img_decoded= np.array(decoded,dtype=np.uint8)
                    im=Image.fromarray(img_decoded)
                    draw=ImageDraw.Draw(im)

                    for i,score in zip(seen_boxes,scores):
                        y_min,x_min,y_max,x_max,cla=i #cordinates of bounding box and class
                        draw.rectangle((x_min,y_min,x_max,y_max), outline = "yellow")
                        if cla<=2 and cla>0 :
                            draw.text((x_min,y_max-10),labels[i[4]-1]+' '+str(score),fill='white')
                            if VERBOSE: print(labels[i[4]-1],score)

                    if VERBOSE: print('-------------------------------------------------')

                    if self.save_images==True: #saves just one every 5 images to not overload
                        # timestamp = round(1000 * (datetime.now() - self.time0).total_seconds(), 3) # ms?
                        save_image_pil(im, self.save_images_path, im_name)
                    img_byte_arr = io.BytesIO()
                    im.save(img_byte_arr, format='PNG')
                    img_byte_arr = img_byte_arr.getvalue()
                    imgdata_complete=img_byte_arr

                out=img_loader.write(bytearray(imgdata_complete))
                pix = img_loader.get_pixbuf()
                # print(pix)
                GLib.idle_add(self._update_image,pix)                
            else:
                img_loader.write(imgdata_complete)
                img_loader.close()
                pix = img_loader.get_pixbuf()
                GLib.idle_add(self._update_image, pix)
        except gi.repository.GLib.Error:
            print("[ai-deck] --- Could not set image!")
    
        if self.object_detection:
            img_loader.close()


###################################################
#                     Main                        #
###################################################

###########
# Parser #
###########

def create_parser():
    # Args for setting IP/port of AI-deck. Default settings are for when
    # AI-deck is in AP mode.    
    parser = argparse.ArgumentParser(description='PULP-Dronet in-field testing')
    parser.add_argument('--ip', default="192.168.4.1", metavar="ip", help="AI-deck IP")
    parser.add_argument('--port', type=int, default='5000', metavar="port", help="AI-deck port")
    parser.add_argument('--dont_save_images', help='save images on your pc', action='store_false')
    parser.add_argument('--save_bboxes',help='save images with objects bounding boxes',action='store_true')
    parser.add_argument('--save_images_path', help='folder where images are saved', default='./dataset_save_folder/')

    return parser

def main():

    parser = create_parser()
    args = parser.parse_args()
    save_images = args.dont_save_images
    deck_port = args.port
    deck_ip = args.ip
    save_images_path = args.save_images_path
    save_bboxes=args.save_bboxes
    time0 = datetime.now()

    fw = FrameViewer(deck_ip, deck_port, time0, save_images, save_images_path,True)

    fw.show_all()
    Gtk.main()


if __name__ == "__main__":
    main()
