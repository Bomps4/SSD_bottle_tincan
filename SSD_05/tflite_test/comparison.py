import argparse
import time

import numpy as np
from PIL import Image,ImageDraw
import tensorflow as tf
import glob 

def iou(true,predicted):
	ground=np.zeros((320,240))
	pred=np.zeros((320,240))
	ground[true[1]:true[3],true[0]:true[2]]=1
	pred[predicted[1]:predicted[3],predicted[0]:predicted[2]]=1
	intersection=np.sum(np.logical_and(ground,pred))
	union = np.sum(np.logical_or(ground,pred))
	
	return intersection/union
		
if __name__ == '__main__':



	parser = argparse.ArgumentParser(description='tflite converter for mobilenet')
	parser.add_argument('-m', '--model_file', help='path to the tflite model non fully quantized',
		                default='./non_quantized.tflite')
	parser.add_argument('-mq','--model_quantized',help='path to the non quantized model',default='../tflite_model/SSD_tin_can_bottle.tflite')
	parser.add_argument('-i','--image_path',help='path to an image on which to apply inference',defualt='../tflite_model/test_1_out.ppm')

	interpreter1 = tf.lite.Interpreter(
		  model_path=parser.model_file,
		  experimental_delegates=None,
		  num_threads=4)
	interpreter1.allocate_tensors()

	interpreter2 = tf.lite.Interpreter(
		  model_path=parser.model_quantized,
		  experimental_delegates=None,
		  num_threads=4)
	interpreter2.allocate_tensors()
	input_details1 = interpreter1.get_input_details()
	output_details1 = interpreter1.get_output_details()
	input_details2 = interpreter2.get_input_details()
	output_details2 = interpreter2.get_output_details()
	iou_difference={0:[],1:[]}
	score_difference={0:[],1:[]}

	images_paths=[parser.image_path]
	for i in images_paths:
	

		height = input_details1[0]['shape'][1]
		width = input_details1[0]['shape'][2]
		orig=Image.open(i).resize((width, height)).convert('RGB')
		img = np.array(orig)
		input_data=0
		if(len(img.shape)==2):
			input_data = np.expand_dims(img, axis=[0,3])
			input_data = np.repeat(input_data,3,axis=-1)
		else:
			input_data = np.expand_dims(img, axis=0)
			
		input_data_float = np.float32(input_data/255)
		if input_details2[0]['dtype']==np.int8:
			input_data-=128
		interpreter1.set_tensor(input_details1[0]['index'], input_data_float)
		interpreter2.set_tensor(input_details2[0]['index'], input_data)
		interpreter1.invoke()
		interpreter2.invoke()

		output_data_float = np.array(interpreter1.get_tensor(output_details1[0]['index']))
		output_data_1_float = np.array(interpreter1.get_tensor(output_details1[1]['index']))
		output_data_2_float = np.array(interpreter1.get_tensor(output_details1[2]['index']))
		output_data_3_float = np.array(interpreter1.get_tensor(output_details1[3]['index']))
		
		output_data = np.array(interpreter2.get_tensor(output_details2[0]['index']))
		output_data_1 = np.array(interpreter2.get_tensor(output_details2[1]['index']))
		output_data_2 = np.array(interpreter2.get_tensor(output_details2[2]['index']))
		output_data_3 = np.array(interpreter2.get_tensor(output_details2[3]['index']))
		
		boolena_float=output_data_2_float>=0.3
		boolena=output_data_2>=62
		
		for i in range(len(boolena_float)):
			
			if(output_data_2_float[0,i]>=0.3  and output_data_1[0,i]==output_data_1_float[0,i]):
				print(output_data_2[0,i])
				print(output_data_2_float[0,i])
				score_difference[output_data_1_float[0,i]].append(output_data_2_float[0,i]-output_data_2[0,i]/255)

			if(output_data_2_float[0,i]>=0.3 and output_data_2[0,i]>=50 and output_data_1[0,i]!=output_data_1_float[0,i]):
				
				score_difference[output_data_1_float[0,i]].append(output_data_2_float[0,i]+output_data_2[0,i]/255)


		output_data=output_data[boolena]
		output_data_1=output_data_1[boolena]
		output_data_2=output_data_2[boolena]
		
		output_data_float=output_data_float[boolena_float]
		output_data_1_float=output_data_1_float[boolena_float]
		output_data_2_float=output_data_2_float[boolena_float]

		zero_point= output_details2[0]['quantization_parameters']['zero_points']
		scale= output_details2[0]['quantization_parameters']['scales']
		
		for i in range(len(output_data_float)):
			if(i<len(output_data) and output_data_1_float[i]==output_data_1[i]):
				ymin, xmin, ymax, xmax = output_data[i]
				xmin, ymin, xmax, ymax=(xmin -zero_point)*scale,(ymin-zero_point)*scale,(xmax-zero_point)*scale,(ymax-zero_point)*scale
				ymin_float, xmin_float, ymax_float, xmax_float=output_data_float[i]
				iou_difference[output_data_1_float[i]].append(iou([ymin_float, xmin_float, ymax_float, xmax_float],[ymin, xmin, ymax, xmax]))
				img1 = ImageDraw.Draw(orig) 
				img1.rectangle([xmin*width, ymin*height, xmax*width, ymax*height], outline ="yellow")
				img1.rectangle([xmin_float*width, ymin_float*height, xmax_float*width, ymax_float*height], outline ="black")
				orig.show()
				
			else:
				if(output_data_2_float[i]>=0.3):
					iou_difference[output_data_1_float[i]].append(0)
	print(iou_difference)
	print(score_difference)
		
	
		
