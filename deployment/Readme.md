#ON DRONED DEPLOYMENT 
This section shows the commands necessary for the deployment of a quantized neural network on Gap8.

##FROZEN GRAPH EXPORT 
The checkpoints produced by the training of the neural network (see section #TODO link a directory specifica) requires to be exported in a format that must be accepted by the tflite converter. First of all you'll need to use the export_tflite_ssd_graph.py python script present in the object detector directory. An example
	python export_tflite_ssd_graph.py --trained_checkpoint_prefix /path/to/checkpoint/model.ckpt-#### --output_directory ./tflite_friendly/ --pipeline_config_path /path/to/configs/ssd_mobilenet_v2_oid_v4_copy.config

## TFLITE CONVERSION
the conversion of the frozen graph can be done using the tflite_convert command. usage example:
	tflite_convert --graph_def_file=tflite_graph.pb --output_file=graph.tflite --inference_type=QUANTIZED_UINT8 --input_arrays=normalized_input_image_tensor --output_arrays=TFLite_Detection_PostProcess,TFLite_Detection_PostProcess:1,TFLite_Detection_PostProcess:2,TFLite_Detection_PostProcess:3 --mean_values=128 --std_dev_values=127.5 --input_shapes=1,240,320,3 --allow_custom_ops --inference_input_type=QUANTIZED_UINT8

## TO DEPLOY  ON DRONE 
You need to source the correct configuration this can be done with the 
	source path/to/gap_sdk/configs/ai_deck.sh
once this is done you need to export the GAPY_OPENOCD_CABLE which can be done with the command
	export GAPY_OPENOCD_CABLE=path/to/gap_sdk/utils/gap8-openocd/tcl/interface/ftdi/olimex-arm-usb-ocd-h.cfg

the last step for the deployment is to use the :
	make clean all run

## VISUALIZATION
To  visualize the detections being performed by the drone you need first to connect to the Bitcraze AI-deck example wi-fi network. Then you need to run the viewer_custom.py with :
	python viewer_custom 


 


	 





