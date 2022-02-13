# Tincan and bottle Detection

The project implements an image-based Deep Learning model to detect and classify tincans and bottles. 
The algorithm is based on Mobilenet SSD-Lite with post training quantization. 

## Getting Started

the whole model can be run with the command:

	make clean all run

by default it will take the image test_1_output located in the tflite_model folder 

## AT Optimization

The utilis/memory_test.py script launches the Autotiler engine with different L2 memory configurations and plots the trend of several metrics to explore the optimal configuration:

	python3 utils/memory_test.py

To visualize the perfomences of the different layers there is also the utilis/different_memory.py script  which runs an inference with different L2 memory configurations and saves a series of plots with the number of operations for each layer , the operation/cycle performance and the execution time for memory configuration:

	python3 utils/different_memory

## Tflite int8 Quantization

the original model with the quantization script are included in the tflite_converter folder the script requires as parameters the full path to the frozen graph file and a name for the output. The script can apply full int8 quantization (default) or only quantization of the weights (with --no-full_8_bit). In the full int8 it requires also the path to a folder containing some images to calibrate the outputs. 

	python3 tensorflow_converter.py -m=./tflite_graph.pb -o=./fully_quantized -i=../images/


## Test tflite models

The tflite_test folder contains two Python scripts (test_tflite.py and comparison.py) the fisrt one can be used to test the tflite models: 

	python3 test_tflite.py -i=path/to/image

The compare script draws on the image the bounding boxes produced by the model with int8 activations(yellow) and float32 activations(black): 

	python3 comparison.py -m=./non_quantized.tflite  -mq=../tflite_model/SSD_tin_can_bottle.tflite -i=path/to/image

if not specified the test image will be test_1_output.ppm located in the tflite_model folder. 


	

	

	


