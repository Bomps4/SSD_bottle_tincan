from PIL import Image, ImageDraw, ImageFont
import re
import time
import psutil
import os
import numpy as np
from subprocess import Popen, PIPE
import glob
import numpy as np
import xml.etree.ElementTree as ET
from object_detection.metrics import coco_evaluation
from object_detection.core import standard_fields
from object_detection.utils.label_map_util import create_categories_from_labelmap, get_label_map_dict
TESTSET=sorted(glob.glob('/home/bomps/Scrivania/gap_8/Codice/testset/*.jpg'))
XMLSET=sorted(glob.glob('/home/bomps/Scrivania/gap_8/Codice/testset/*.xml'))

RECTANGLE=re.compile("Draw_boxes\(-*[0-9]+\,-*[0-9]+\,-*[0-9]+\,-*[0-9]+\)")
SCORE=re.compile("SCORES:-*[0-9]+")
CLASS=re.compile("CLASS:-*[0-9]+")
IMG=re.compile("FINISHED READING ")
INTPUT_FILE ="/home/bomps/Scrivania/gap_8/conversion_tflite/SSD_bottle_tincan/tflite_model/output_camera.ppm"
FOUND=False
THRESHOLD=0.3*128
def voc_parser(path_to_xml_file, label_map_dict):
	"""Parser for Pascal VOC format annotation to TF OD API format
	args:
		path_to_xml_file : path to annotation in Pascal VOC format
		label_map_dict : dictionary of class name to index
	returns
		boxes: array of boundary boxes (m, 4) where each row is [ymin, xmin, ymax, xmax]
		classes: list of class index (m, 1)
		where m is the number of objects
	"""
	boxes = []
	classes = []

	xml = open(path_to_xml_file, "r")
	tree = ET.parse(xml)
	root = tree.getroot()
	
	xml_size = root.find("size")

	objects = root.findall("object")

	if len(objects) == 0:
		print("No objects for {}")
		return [boxes], classes

	obj_index = 0
	for obj in objects:
		class_id = label_map_dict[obj.find("name").text]
		xml_bndbox = obj.find("bndbox")
		xmin = float(xml_bndbox.find("xmin").text)
		ymin = float(xml_bndbox.find("ymin").text)
		xmax = float(xml_bndbox.find("xmax").text)
		ymax = float(xml_bndbox.find("ymax").text)
		boxes.append([ymin, xmin, ymax, xmax])
		classes.append(class_id)
	return boxes, classes

def evaluate_single_image(image_path, annotation_path,label_map_dict,dt,coco_evaluator):
    """ Evaluate mAP on image
    args:
        image_path: path to image
        annotation_path: path to groundtruth in Pascal VOC format .xml
        label_file: dictionary processed of labels
    """

    
    image_name = os.path.basename(image_path).split('.')[0]

    # Read groundtruth (here, an XML file in Pascal VOC format)
    gt_boxes, gt_classes = voc_parser(annotation_path, label_map_dict)
    print('gt boxes',gt_boxes)
    print('gt classes',gt_classes)
    #print(gt_boxes)
    # Get the detection after post processing
    #dt_boxes, dt_classes, dt_scores, num_det = postprocess_output(image_path)
	
    coco_evaluator.add_single_ground_truth_image_info(
        image_id=image_name,
        groundtruth_dict={
            standard_fields.InputDataFields.groundtruth_boxes:
            np.array(gt_boxes),
            standard_fields.InputDataFields.groundtruth_classes:
            np.array(gt_classes)
    })
    coco_evaluator.add_single_detected_image_info(
        image_id=image_name,
        detections_dict={
            standard_fields.DetectionResultFields.detection_boxes:
            dt['boxes'],
            standard_fields.DetectionResultFields.detection_scores:
            dt['score'],
            standard_fields.DetectionResultFields.detection_classes:
            dt['label']
        })
    
   



FOUND=False
categories = create_categories_from_labelmap('/home/bomps/Scrivania/gap_8/Codice/Code/object_detection/detection/configs/oid_v4_label_map_bottle_and_tin_can.pbtxt')
label_map_dict = {'Bottle':1,'Tin can':2,'Tin Can':2}#get_label_map_dict('/home/bomps/Scrivania/gap_8/Codice/Code/object_detection/detection/configs/oid_v4_label_map_bottle_and_tin_can.pbtxt')
coco_evaluator = coco_evaluation.CocoDetectionEvaluator(categories)
print(len(TESTSET),len(XMLSET))
for img,xml in zip(TESTSET,XMLSET):
	im=Image.open(img).convert('L')
	im=im.resize((320,240))
	im.save('/home/bomps/Scrivania/gap_8/conversion_tflite/SSD_bottle_tincan/tflite_model/output_camera.ppm')
	with Popen(["make","run"], stdout=PIPE, bufsize=1,universal_newlines=True) as p:
		rettangle=[]
		scores=[]
		classes=[]

		for line in p.stdout:
			print(line)
			rect=re.match(RECTANGLE,line)
			score=re.match(SCORE,line)
			clas=re.match(CLASS,line)
			next_img=re.match(IMG,line)
			
			if rect:
				preso=rect.group(0).replace(')','')
				numeri=preso.split('(')[-1]
				numeri=numeri.split(',')
				rettangle.append([int(i) for i in numeri])
			elif score:
				preso=score.group(0)
				score=int(preso.split(':')[-1])
				scores.append(score)
				
			elif clas:
				preso=clas.group(0)
				cla=int(preso.split(':')[-1])
				
				classes.append(cla)
	sel=np.array(scores)>=THRESHOLD			
	rettangle=np.array(rettangle)[sel]
	print('predicted rettangle',rettangle)
	
	classes=np.array(classes)[sel]
	print('predicted classes',classes)
	scores=np.array(scores)[sel]
	
	evaluate_single_image(img,xml,label_map_dict,{'boxes':rettangle,'score':scores,'label':classes},coco_evaluator)
print(coco_evaluator.evaluate())
					
					
        




                    

