# Copyright 2018 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================


import argparse
import time

import numpy as np
from PIL import Image,ImageDraw,ImageFont
import tensorflow as tf


def load_labels(filename):
	with open(filename, 'r') as f:
		return [line.strip() for line in f.readlines()]


if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument(
      '-i',
      '--image',
      default='/tmp/grace_hopper.bmp',
      help='../tflite_model/test_1_out.ppm')
	parser.add_argument(
      '-m',
      '--model_file',
      default='../tflite_model/SSD_tin_can_bottle.tflite',
      help='.tflite model to be executed')
	parser.add_argument(
      '--input_mean',
      default=127.5, type=float,
      help='input_mean')
	parser.add_argument(
      '--input_std',
      default=127.5, type=float,
      help='input standard deviation')
	parser.add_argument(
      '--num_threads', default=None, type=int, help='number of threads')
	

	args = parser.parse_args()


	labels=["Bottle","Tincan"]


	interpreter = tf.lite.Interpreter(
      model_path=args.model_file,
      experimental_delegates=None,
      num_threads=args.num_threads)
	interpreter.allocate_tensors()

	input_details = interpreter.get_input_details()
	output_details = interpreter.get_output_details()

  # check the type of the input tensor
	floating_model = input_details[0]['dtype'] == np.float32
	int8_model= input_details[0]['dtype'] == np.int8

  # NxHxWxC, H:1, W:2
	height = input_details[0]['shape'][1]
	width = input_details[0]['shape'][2]
	img = Image.open(args.image).resize((width, height))

  # add N dim
	
	if(len(np.array(img).shape)==2):
		input_data = np.expand_dims(img, axis=[0,3])
	else:
		input_data = np.expand_dims(img, axis=0)
	if floating_model:
		input_data = (np.float32(input_data) - args.input_mean) / args.input_std
	if int8_model:	
		input_data -=128


	if(len(input_data .shape)==3):
		input_data=np.repeat(input_data,3,axis=-1)
	

	interpreter.set_tensor(input_details[0]['index'], input_data)
	
	start_time = time.time()
	interpreter.invoke()
	stop_time = time.time()

	output_data = np.array(interpreter.get_tensor(output_details[0]['index']))
	output_data_1 = np.array(interpreter.get_tensor(output_details[1]['index']))
	output_data_2 = np.array(interpreter.get_tensor(output_details[2]['index']))
	output_data_3 = np.array(interpreter.get_tensor(output_details[3]['index']))

	boolena=output_data_2>=0.3 if output_details[2]['dtype']==np.float32 else output_data_2>=78
	output_data=output_data[boolena]
	

	output_data_1=output_data_1[boolena]
	output_data_2=output_data_2[boolena]
	zero_point=0 if output_details[0]['dtype']==np.float32 else output_details[0]['quantization_parameters']['zero_points']
	scale=1 if output_details[0]['dtype']==np.float32 else output_details[0]['quantization_parameters']['scales']
	print(output_details[0].keys())
	for i,j in zip(output_data,output_data_1):
		ymin, xmin, ymax, xmax = i
		img1 = ImageDraw.Draw(img) 
		shape=[(xmin -zero_point)*scale*width,(ymin-zero_point)*scale*height,(xmax-zero_point)*scale*width,(ymax-zero_point)*scale*height] 

		img1.rectangle(shape, outline ="black")
		img1.text((shape[0],shape[1]), labels[j])
		img.show()
	

'''
  top_k = results.argsort()[-5:][::-1]
  labels = load_labels(args.label_file)
  for i in top_k:
    if floating_model:
      print('{:08.6f}: {}'.format(float(results[i]), labels[i]))
    else:
      print('{:08.6f}: {}'.format(float(results[i] / 255.0), labels[i]))

  print('time: {:.3f}ms'.format((stop_time - start_time) * 1000))
'''
