import tensorflow as tf
import argparse
import glob
import numpy as np 
import matplotlib.pyplot as plt
import tensorflow.compat.v1 as tf1
from PIL import Image
def create_parser():
	parser = argparse.ArgumentParser(description='tflite converter for mobilenet')
	parser.add_argument('-m', '--model_file', help='path to the pb model you wish to convert to tflite',
		                default='./tflite_graph.pb')

	parser.add_argument('-o', '--output_file', help='name and path of the converted model',
		                default='./output')

	parser.add_argument('-i','--images_path', help='path to a representative dataset of images for full 8 bit quantization',
		                default='../images')
	parser.add_argument('--full_8_bit',dest='full_8_bit',action='store_true')
	parser.add_argument('--no-full_8_bit',dest='full_8_bit',action='store_false')
	parser.set_defaults(full_8_bit=True)                   
	return parser






	
if __name__ == '__main__':
	
	parser = create_parser()
	args = parser.parse_args()

	def create_data_generator():
		a = []
		nomi_files=glob.glob(args.images_path+'/*jpg')
		for i in nomi_files:
			image=Image.open(i).resize((320,240))
			
			image = np.array(image).astype(np.float32)
			image=(image-127)/128
			if(len(image.shape)==2):
				image = np.expand_dims(image,axis=2)
				image = np.repeat(image,3,axis=-1)
			a.append(image)
		a=np.array(a)
		img = tf.data.Dataset.from_tensor_slices(a).batch(1)
		for i in img.take(len(nomi_files)):	
			yield [i]

		
	converter=0
	
	if(args.full_8_bit):
		if tf.__version__[0] == '1':
			tf_lite=tf.lite
		else:
			tf_lite=tf.compat.v1.lite
		
		converter = tf_lite.TFLiteConverter.from_frozen_graph(
		graph_def_file=args.model_file,
		input_arrays=['normalized_input_image_tensor'],
		input_shapes={'normalized_input_image_tensor' : [1, 240,320,3]},
		output_arrays=["TFLite_Detection_PostProcess","TFLite_Detection_PostProcess:1","TFLite_Detection_PostProcess:2","TFLite_Detection_PostProcess:3"]) 

		converter.allow_custom_ops = True
		converter.optimizations = [tf1.lite.Optimize.DEFAULT]

		#converter.post_training_quantization=True

		converter.target_spec.supported_ops = [tf1.lite.OpsSet.TFLITE_BUILTINS_INT8,tf1.lite.OpsSet.TFLITE_BUILTINS]
		converter.inference_input_type = tf.int8
		converter.inference_output_type = tf.float32
		converter.inference_type = tf.int8
		converter.quantized_input_stats = {'normalized_input_image_tensor' : (127, 128)}  # mean, std_dev (input range is [-1, 1])
		converter.representative_dataset = create_data_generator
	else:
		if tf.__version__[0] == '1':
			tf_lite=tf.lite
		else:
			tf_lite=tf.compat.v1.lite
		converter = tf_lite.TFLiteConverter.from_frozen_graph(
		graph_def_file=args.model_file,
		input_arrays=['normalized_input_image_tensor'],
		input_shapes={'normalized_input_image_tensor' : [1, 240,320,3]},
		output_arrays=["TFLite_Detection_PostProcess","TFLite_Detection_PostProcess:1","TFLite_Detection_PostProcess:2","TFLite_Detection_PostProcess:3"])
		converter.allow_custom_ops = True
		converter.optimizations = [tf1.lite.Optimize.DEFAULT]
		converter.target_spec.supported_ops = [tf1.lite.OpsSet.TFLITE_BUILTINS]
		#converter.quantized_input_stats = {'normalized_input_image_tensor' : (128, 127)}  # mean, std_dev (input range is [-1, 1])


	tflite_quant_model = converter.convert()
	with open(args.output_file+'.tflite', 'wb') as f:
  		f.write(tflite_quant_model)

