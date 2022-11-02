import numpy as np
from PIL import Image,ImageDraw
import tensorflow as tf
import sys
import matplotlib.pyplot as plt
import os
import pandas as pd
import re
import numpy as np
import xml.etree.ElementTree as ET
from sklearn.metrics import average_precision_score
import glob
import copy
import cv2
LABELS=['Bottle','Tin Can']
def select (img_paths,temporal_delay):
	selected=[]
	starting=0
	for i in img_paths:
		splits=i.split('/')
		name=splits[-1]
		name=name.split('.')[0]
		if(int(name)-starting>temporal_delay):
			selected.append(i)
			starting=int(name)
	return selected

def prediction(intepreter,image):
	input_details = interpreter.get_input_details()
	output_details = interpreter.get_output_details()
	nn_height = input_details[0]['shape'][1]
	nn_width = input_details[0]['shape'][2]
	img = Image.open(image)
	width, height = img.size
	
	
	img=img.resize((nn_width,nn_height))
	img=np.array(img)
	# add N dim
	if(len(img.shape)==2):
		input_data =img[None,:,:,None]
		input_data=np.repeat(input_data,3,axis=-1)
		
	else:
		input_data =img[None,:,:,:3]
	input_data=input_data.astype(int)
	input_data=input_data.astype(np.uint8)
		
	interpreter.set_tensor(input_details[0]['index'], input_data)
	interpreter.invoke()
	
	output_data = np.array(interpreter.get_tensor(output_details[0]['index']))
	output_data_1 = np.array(interpreter.get_tensor(output_details[1]['index'])).astype(int)
	output_data_2 = np.array(interpreter.get_tensor(output_details[2]['index']))
	output_data_3 = np.array(interpreter.get_tensor(output_details[3]['index']))
	
	
	boolena=output_data_2>0.3
	output_data=output_data[boolena]
	output_data=output_data*np.array([height,width, height,width])[None,:]
	output_data_1=output_data_1[boolena]
	
	return output_data,output_data_1



if __name__ == "__main__":
	parser = argparse.ArgumentParser(description='Connect to AI-deck JPEG streamer example')
	parser.add_argument("-m",  default="/home/bomps/Scrivania/gap_8/date_paper/true_symmetric/graph.tflite", metavar="model save", help="path for saved model in tflite format")
	parser.add_argument("-p", default='/home/bomps/Scrivania/gap_8/Codice/testset', metavar="save images", help="directory of saved images")
	parser.add_argument("-s",default="",help="directory of saved images with detections")
	parser.add_argument("-t",type=int,default='0',help="time delay for undersampling")

	args = parser.parse_args()
	model_path=args.m
	delay=args.t
	image_directory=args.p
	img_paths=sorted(glob.glob(image_directory+'/*.jpg'))
	INTERPRETER = tf.lite.Interpreter(
		  model_path=model_path,
		  experimental_delegates=None)
	INTERPRETER.allocate_tensors()
	img_paths=select(img_paths,delay)
	for i in img_paths:
		detections,classes=prediction(INTERPRETER,i)
		im=Image.open(img_decoded)
        draw=ImageDraw.Draw(im)
		for det,cla in zip(detections,classes):
			y_min,x_min,y_max,x_max=det #cordinates of bounding box and class
			draw.rectangle((x_min,y_min,x_max,y_max), outline = "yellow")
			if cla<=2 and cla>0 :
				draw.text((x_min,y_max-10),LABELS[cla-1],fill='white')
		if(args.s==""):
			name=i.split('.')[0]
		else:
			name=i.split('.')[0]
			name=args.s+'/'+i.split('/')[-1]
		
		im.save(name+'_bbdraw.jpg')
		
		
















