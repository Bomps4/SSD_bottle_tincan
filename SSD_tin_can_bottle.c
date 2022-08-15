/*
 * Copyright (C) 2020 GreenWaves Technologies
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 * 


  dump in nntool per inferenza senza passare da gvsoc. 
 */




#include "SSD_tin_can_bottle.h"
#include "SSD_tin_can_bottleKernels.h"
#include "SSD_tin_can_bottleInfo.h"



#include "/home/bomps/Scrivania/gap_8/gap_sdk/frame_streamer/include/tools/frame_streamer.h"
#include "/home/bomps/Scrivania/gap_8/gap_sdk/frame_streamer/frame_streamer/frame_streamer.c"
#include "SSD_tin_can_bottle.h"
#include "SSD_tin_can_bottleKernels.h"
#include "SSD_tin_can_bottleInfo.h"
#include "pmsis.h"
#include "bsp/transport.h"
#include "bsp/flash/hyperflash.h"
#include "bsp/bsp.h"
#include "bsp/ram.h"
#include "bsp/buffer.h"
#include "bsp/transport/nina_w10.h"
#include "bsp/camera/himax.h"
#include "bsp/ram/hyperram.h"
#include "gaplib/ImgIO.h"
#include "stdio.h"

#define __XSTR(__s) __STR(__s)
#define __STR(__s) #__s

#ifdef SILENT
  #define PRINTF(...) ((void) 0)
#else
  #define PRINTF PRINTF
#endif

#define FIX2FP(Val, Precision)    ((float) (Val) / (float) (1<<(Precision)))

#define AT_INPUT_SIZE (AT_INPUT_WIDTH_SSD*AT_INPUT_HEIGHT_SSD*AT_INPUT_COLORS_SSD)
#define MAX_BB          (300)
#define CAMERA_WIDTH    (324)
#define CAMERA_HEIGHT   (244)
#define NUMBER_OF_DETECTION (10)
#define BYTES_DETECTION (10)
#define EXTRA_RECOGNITION (2)
#define TEXT_SIZE 		(NUMBER_OF_DETECTION*BYTES_DETECTION +EXTRA_RECOGNITION)
#define CAMERA_COLORS   (1)
#define CAMERA_SIZE     (CAMERA_WIDTH*CAMERA_HEIGHT*CAMERA_COLORS)
#define SCORE_THR       0
#define HAVE_HIMAX 		  1


AT_HYPERFLASH_FS_EXT_ADDR_TYPE __PREFIX(_L3_Flash) = 0;

#ifdef __EMUL__
  char *ImageName;
  uint8_t Input_1[AT_INPUT_SIZE];
  
#else
  L2_MEM static struct pi_device gpio_device;
  #define LED_ON pi_gpio_pin_write(&gpio_device, 2, 1)

  #define LED_OFF pi_gpio_pin_write(&gpio_device, 2, 0)
  struct simple_streamer{
  int channel;
  struct pi_transport_header header;
  unsigned int size;
	};
  struct simple_streamer text_streamer;
  struct pi_device wifi;
  struct pi_device camera;
  struct pi_device cluster_dev;
  static pi_task_t cam_task;
  static pi_task_t streamer_task;
  static frame_streamer_t *streamer;
  //PI_FC_L1 static frame_streamer_t *txt_streamer;
  static pi_task_t detection_task;

  struct pi_device HyperRam;
  static pi_buffer_t buffer;
  static uint32_t l3_buff;
  L2_MEM static uint8_t Input_1[CAMERA_SIZE];
  struct pi_cluster_task *task;

  
#endif
static void detection_handler();
static void camera_handler();
static void main_handler();


L2_MEM signed char outputs[TEXT_SIZE];
// controllare indirizzi malloc perchè importante (strano) 

L2_MEM short int out_boxes[40];
L2_MEM signed char out_scores[10];
L2_MEM signed char out_classes[10];
/*
#ifdef HAVE_LCD
static int open_display(struct pi_device *device)
{
  struct pi_ili9341_conf ili_conf;
  pi_ili9341_conf_init(&ili_conf);
  pi_open_from_conf(device, &ili_conf);
  if (pi_display_open(device))
    return -1;
  if (pi_display_ioctl(device, PI_ILI_IOCTL_ORIENTATION, (void *)PI_ILI_ORIENTATION_90))
    return -1;
  return 0;
}
#endif*/
static void init_wifi() {

	int32_t errors = 0;
	struct pi_nina_w10_conf nina_conf;

	pi_nina_w10_conf_init(&nina_conf);

	nina_conf.ssid = "";
	nina_conf.passwd = "";
	nina_conf.ip_addr = "0.0.0.0";
	nina_conf.port = 5555;

	pi_open_from_conf(&wifi, &nina_conf);

	errors = pi_transport_open(&wifi);

#ifdef VERBOSE	
	//PRINTF("NINA WiFi init:\t\t\t\t%s\n", errors?"Failed":"Ok");
#endif	

	if(errors) pmsis_exit(errors);
}

static void init_streamer() {

	struct frame_streamer_conf streamer_conf;

	frame_streamer_conf_init(&streamer_conf);

	streamer_conf.transport = &wifi;
	streamer_conf.format = FRAME_STREAMER_FORMAT_JPEG;
	streamer_conf.width = AT_INPUT_WIDTH_SSD;
	streamer_conf.height = AT_INPUT_HEIGHT_SSD;
	streamer_conf.depth = 1;
	streamer_conf.name = "image_Stream";

	streamer = frame_streamer_open(&streamer_conf);

	pi_buffer_init(&buffer, PI_BUFFER_TYPE_L2, Input_1);
	pi_buffer_set_format(&buffer, AT_INPUT_WIDTH_SSD, AT_INPUT_HEIGHT_SSD, 1, PI_BUFFER_FORMAT_GRAY);

#ifdef VERBOSE	
	//PRINTF("Streamer init:\t\t\t\t%s\n", streamer?"Ok":"Failed");
#endif	

	if(streamer == NULL) pmsis_exit(-1);
}
/*
static void init_txt_streamer() {

	struct frame_streamer_conf streamer_conf;

	frame_streamer_conf_init(&streamer_conf);

	streamer_conf.transport = &wifi;
	streamer_conf.format = FRAME_STREAMER_FORMAT_RAW;
	streamer_conf.width = NUMBER_OF_DETECTION*BYTES_DETECTION+2;
	streamer_conf.height = 1;
	streamer_conf.depth = 1;
	streamer_conf.name = "detection_Stream";
	

	txt_streamer = frame_streamer_open(&streamer_conf);

	pi_buffer_init(&txt_buffer, PI_BUFFER_TYPE_L2, (char*)outputs);
	pi_buffer_set_format(&txt_buffer, NUMBER_OF_DETECTION*BYTES_DETECTION+2, 1, 1, PI_BUFFER_FORMAT_GRAY);
	
	//PRINTF("Streamer init:\t\t\t\t%s\n", txt_streamer?"Ok":"Failed");
#ifdef VERBOSE	
	
#endif	

	if(txt_streamer == NULL) pmsis_exit(-1);
}
*/

#ifdef HAVE_HIMAX
static int open_camera_himax(struct pi_device *device)
{ //PRINTF("Opening camera\n");
  struct pi_himax_conf cam_conf;

  pi_himax_conf_init(&cam_conf);

  cam_conf.format = PI_CAMERA_QVGA;

  pi_open_from_conf(device, &cam_conf);
  if (pi_camera_open(device))return -1;
	//{for(int i=0;i<100;++i)//PRINTF("error occurred\n");
    //return -1;}
  //for(int i=0;i<100;++i)//PRINTF("Finishing\n");
  uint8_t reg_value, set_value;

	// set_value = ANA;
	// pi_camera_reg_set(&camera, HIMAX_ANA_Register_17, &set_value);
	// pi_camera_reg_get(&camera, HIMAX_ANA_Register_17, &reg_value);
	// //PRINTF("ANA %d\n", reg_value);

	// set_value = CLK_DIV;
	// pi_camera_reg_set(&camera, HIMAX_OSC_CLK_DIV, &set_value);
	// pi_camera_reg_get(&camera, HIMAX_OSC_CLK_DIV, &reg_value);
	// //PRINTF("CLK_DIV %d\n", reg_value);

	
    set_value=0;
	
    pi_camera_reg_set(device, IMG_ORIENTATION, &set_value);
	pi_camera_reg_get(device, IMG_ORIENTATION, &reg_value);
  	if (set_value!=reg_value)
  	{
    	//PRINTF("Failed to rotate camera image\n");
    	return -1;
  	}

    pi_camera_control(device, PI_CAMERA_CMD_AEG_INIT, 0);

    return 0;
}
#endif



static void RunNetwork()
{ 
  ////PRINTF("Running on cluster\n");
//#ifdef PERF
//  //PRINTF("Start timer\n");
//  gap_cl_starttimer();
//  gap_cl_resethwtimer();
//#endif
//#ifndef __EMUL__
  __PREFIX(CNN)(l3_buff,out_boxes,out_classes, out_scores); //(signed short*)(outputs+2),outputs+82,outputs+92);
    //out_boxes, out_classes, out_scores);
/*
#else
  __PREFIX(CNN)(Input_1, out_boxes, out_classes, out_scores);
#endif
	*/
}
static void send_text(){
	/**/
    pi_transport_send_header(&wifi, &(text_streamer.header), text_streamer.channel, text_streamer.size);
	pi_transport_send_async(&wifi,outputs,text_streamer.size,pi_task_callback(&cam_task, camera_handler, NULL));	
}

static void init_simple_streamer(){
	text_streamer.channel=pi_transport_connect(&wifi, NULL, NULL);
	text_streamer.size=TEXT_SIZE;
}
static void detection_handler(){
	  pi_camera_control(&camera, PI_CAMERA_CMD_STOP, 0);
	  
	  task = pmsis_l2_malloc(sizeof(struct pi_cluster_task));
	  if(task==NULL) {
		//PRINTF("pi_cluster_task alloc Error!\n");
		pmsis_exit(-1);
	  }
	  //PRINTF("Stack size is %d and %d\n",STACK_SIZE,SLAVE_STACK_SIZE );
	  memset(task, 0, sizeof(struct pi_cluster_task));
	  task->entry = &RunNetwork;
	  task->stack_size = STACK_SIZE;
	  task->slave_stack_size = SLAVE_STACK_SIZE;
	  task->arg = NULL;



	  

	  
	  int error_cnn=__PREFIX(CNN_Construct)();
	  //PRINTF("error cnn constructor %d \n",error_cnn);
	  if (error_cnn)
	  {
		//PRINTF("Graph constructor exited with an error\n");
		pmsis_exit(-1);
	  }
	  //PRINTF("Graph constructor was OK\n");
	
	int idx=0;
		    for(int i =0;i<CAMERA_HEIGHT;i++){
		      for(int j=0;j<CAMERA_WIDTH;j++){
		        if (i<AT_INPUT_HEIGHT_SSD && j<AT_INPUT_WIDTH_SSD){
		          Input_1[idx] = Input_1[i*CAMERA_WIDTH+j];
		        idx+=1;
		        }
		    };}
	//workaround rotation of images
	for(int i=AT_INPUT_WIDTH_SSD;i>0;--i){
		for (int j=0;j<AT_INPUT_HEIGHT_SSD/2;++j){
		
		unsigned char pixel=Input_1[i+j*AT_INPUT_WIDTH_SSD];
		Input_1[i+j*AT_INPUT_WIDTH_SSD]=Input_1[-i+(AT_INPUT_HEIGHT_SSD-j)*AT_INPUT_WIDTH_SSD];
		Input_1[-i+(AT_INPUT_HEIGHT_SSD-j)*AT_INPUT_WIDTH_SSD]=pixel;
		};}
	/*  
      rimettere assieme codice al contrario;
	  mettere checksum cumulativo somma dei pixel check in tutti i punti (farlo anche su python) idem con patate sui pesi;
	  controllare singoli layers 
	  ricordarsi per print da l3 a l2 e poi printare .
	*/
	  int8_t* Input_2 = (int8_t*) Input_1 ;
	  

	  for(int i=0; i<AT_INPUT_WIDTH_SSD*AT_INPUT_HEIGHT_SSD ; ++i){Input_2[i] = Input_1[i]-128; }
	  
	  //int8_t *attimo=(int8_t*)pi_l2_malloc(AT_INPUT_WIDTH_SSD); good indicare il size of in dimensione*sizeof
	
	  pi_ram_write(&HyperRam, l3_buff , Input_2, (uint32_t)AT_INPUT_WIDTH_SSD*AT_INPUT_HEIGHT_SSD);

	  pi_ram_write(&HyperRam, l3_buff+AT_INPUT_WIDTH_SSD*AT_INPUT_HEIGHT_SSD , Input_2, (uint32_t) AT_INPUT_WIDTH_SSD*AT_INPUT_HEIGHT_SSD);

	  pi_ram_write(&HyperRam, l3_buff+2*AT_INPUT_WIDTH_SSD*AT_INPUT_HEIGHT_SSD , Input_2, (uint32_t)AT_INPUT_WIDTH_SSD*AT_INPUT_HEIGHT_SSD);
	/*
	pi_ram_read(&HyperRam, l3_buff,attimo,AT_INPUT_WIDTH_SSD);
	//PRINTF("posizione attimo %x",attimo);
	for (int i=0;i<AT_INPUT_WIDTH_SSD;++i){
		//PRINTF("sono i dati pescati da hyperam %d cosa ho in pancia %d \n",i,attimo[i]);}*/
	  
	  

	  


	  //PRINTF("ci arrivo qua?\n");
	  uint32_t time_begin=rt_time_get_us(); 
	  LED_ON;
	  pi_cluster_send_task_to_cl(&cluster_dev, task);
	  //PRINTF("TOTAL TIME IN MICROSECONDS: %d \n",rt_time_get_us()-time_begin);
	  LED_OFF;
	  __PREFIX(CNN_Destruct)();	
	  pi_l2_free(task,sizeof(struct pi_cluster_task));
	  for(char i=0;i<10;i+=1){
	  	out_boxes[i*4] = (short int)(FIX2FP(((int)out_boxes[i*4])*SSD_tin_can_bottle_Output_1_OUT_QSCALE,SSD_tin_can_bottle_Output_1_OUT_QNORM)*240);

		  out_boxes[i*4+1 ] = (short int)(FIX2FP(((int)out_boxes[1+i*4])*SSD_tin_can_bottle_Output_1_OUT_QSCALE,SSD_tin_can_bottle_Output_1_OUT_QNORM)*320);

		  out_boxes[i*4 +2] = (short int)(FIX2FP(((int)out_boxes[2+i*4])*SSD_tin_can_bottle_Output_1_OUT_QSCALE,SSD_tin_can_bottle_Output_1_OUT_QNORM)*240);

		  out_boxes[i*4 +3] = (short int)(FIX2FP(((int)out_boxes[3+i*4])*SSD_tin_can_bottle_Output_1_OUT_QSCALE,SSD_tin_can_bottle_Output_1_OUT_QNORM)*320);	

	} 
	
	  for (char i=0;i<80;++i){
		outputs[i+2]=((signed char*)out_boxes)[i];
		}
	
	  for (char i=80;i<90;++i)outputs[i+2]=out_scores[i-80];
	  for (char i=90;i<100;++i)outputs[i+2]=out_classes[i-90];
		
	
	  for(int i=0; i<CAMERA_SIZE ; i++){Input_1[i] = Input_2[i]+128; }
	  frame_streamer_send_async(streamer, &buffer,pi_task_callback(&streamer_task, send_text, NULL));
	  //frame_streamer_send_async(txt_streamer, &txt_buffer, pi_task_callback(&cam_task, camera_handler, NULL));
	  




}

static void camera_handler() {
	 
	pi_camera_control(&camera, PI_CAMERA_CMD_ON, 0);
	pi_camera_capture_async(&camera,  Input_1, CAMERA_WIDTH*CAMERA_HEIGHT,pi_task_callback(&detection_task, detection_handler, NULL) );
	//ReadImageFromFile("/home/bomps/Scrivania/gap_8/conversion_tflite/SSD_bottle_tincan/tflite_model/test_1_out.ppm", AT_INPUT_WIDTH_SSD, AT_INPUT_HEIGHT_SSD, 1, Input_1, AT_INPUT_WIDTH_SSD*AT_INPUT_HEIGHT_SSD*sizeof(char), IMGIO_OUTPUT_CHAR, 0);
	//pi_task_push(pi_task_callback(&detection_task, detection_handler, NULL));
	pi_camera_control(&camera, PI_CAMERA_CMD_START, 0);
}







int start()
{
  //spostare qui le frequenze prova
  /*
  #ifdef MEASUREMENTS
  pi_gpio_pin_configure(NULL, PI_GPIO_A0_PAD_8_A4, PI_GPIO_OUTPUT);
  pi_gpio_pin_write(NULL, PI_GPIO_A0_PAD_8_A4, 0);
  #endif   */
  #ifdef HAVE_HIMAX
  pi_gpio_pin_configure(&gpio_device, 2, PI_GPIO_OUTPUT); 
  
	outputs[0]=-127;
	outputs[1]=13;
    int err = open_camera_himax(&camera);
    if (err) {
      //PRINTF("Failed to open camera\n");
      pmsis_exit(-2);
    }
    
  #endif
  /* Init & open ram. */
  struct pi_hyperram_conf hyper_conf;
  pi_hyperram_conf_init(&hyper_conf);
  pi_open_from_conf(&HyperRam, &hyper_conf);
  if (pi_ram_open(&HyperRam))
  {
    //PRINTF("Error ram open !\n");
    pmsis_exit(-3);
  }

  if (pi_ram_alloc(&HyperRam, &l3_buff, (uint32_t) AT_INPUT_SIZE))
  {
    //PRINTF("Ram malloc failed !\n");
    pmsis_exit(-4);
  }
  //PRINTF("l3 origin %d \n",l3_buff);

  

  /*-----------------------OPEN THE CLUSTER--------------------------*/
	  PMU_set_voltage(1200, 0);
	  pi_time_wait_us(100000);
	  pi_freq_set(PI_FREQ_DOMAIN_FC, FREQ_FC*1000*1000);
	  pi_time_wait_us(100000);
      struct pi_cluster_conf conf;
	  pi_cluster_conf_init(&conf);
	  pi_open_from_conf(&cluster_dev, (void *)&conf);
	  int error=pi_cluster_open(&cluster_dev);
	  //PRINTF("is cluster giving an error? %d\n",error);
	  if(error) pmsis_exit(error);

      pi_freq_set(PI_FREQ_DOMAIN_CL, FREQ_CL*1000*1000);
      pi_time_wait_us(100000);
  

  
  if(Input_1==NULL){
      //PRINTF("Error allocating image buffer\n");
      pmsis_exit(-1);}
  init_wifi();
  //PRINTF("OPENED_WIFI \n");
  init_streamer();
  //PRINTF("OPENED_STREAMER_IMAGES\n");
  init_simple_streamer();
  //PRINTF("OPENED_STREAMER_TEXT\n");
  pi_task_push(pi_task_callback(&cam_task, camera_handler, NULL));
  while(true){
	
	pi_yield();
	
}
  
  pmsis_exit(0);

}

//#ifndef __EMUL__
int main(void)
{ 
  //PRINTF("\n\n\t *** OCR SSD ***\n\n");

  return pmsis_kickoff((void *) start);
}
/*
#else
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        //PRINTF("Usage: ./exe [image_file]\n");
        exit(-1);
    }
    ImageName = argv[1];
    //PRINTF("\n\n\t *** OCR SSD Emul ***\n\n");
    start();
    return 0;
}
#endif
*/
                
