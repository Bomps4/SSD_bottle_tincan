#include <stdint.h>
#include <stdio.h>
#include "AutoTilerLib.h"
#include "CNN_Generators_SQ8.h"

#include "SSD_Generators.h"
#include "CNN_Copy_Generators.h"





void SSD_tin_can_bottleModel(unsigned int L1Memory, unsigned int L2Memory, unsigned int L3Memory, unsigned int L3Flash)
{
    KernelOper_T Cop = KOP_CONV;

    // SetKernelOpts(KER_OPT_NONE, KER_OPT_BUFFER_PROMOTE);
    SetSymbolDynamics();

    SetUsedFilesNames(0, 3, "CNN_BasicKernels_SQ8.h", "SSD_tin_can_bottle.h", "SSD_BasicKernels.h");
    SetGeneratedFilesNames("SSD_tin_can_bottleKernels.c", "SSD_tin_can_bottleKernels.h");
    AT_SetGraphCtrl(AT_GRAPH_MONITOR_CYCLES, AT_OPT_ON);
    AT_SetGraphCtrl(AT_GRAPH_MONITOR_CVAR_NAME, AT_OPT_VAL("SSD_Monitor"));
    AT_SetGraphCtrl(AT_GRAPH_PRODUCE_NODE_NAMES, AT_OPT_ON);
    AT_SetGraphCtrl(AT_GRAPH_PRODUCE_NODE_CVAR_NAME, AT_OPT_VAL("SSD_Nodes"));
    AT_SetGraphCtrl(AT_GRAPH_PRODUCE_OPERINFOS, AT_OPT_ON);
    AT_SetGraphCtrl(AT_GRAPH_PRODUCE_OPERINFOS_CVAR_NAME, AT_OPT_VAL("SSD_Op"));
    AT_SetGraphCtrl(AT_GRAPH_REORDER_CONSTANT_IN, AT_OPT_OFF);
    AT_SetGraphCtrl(AT_GRAPH_TRACE_EXEC, AT_OPT_ON);
    AT_SetGraphCtrl(AT_GRAPH_CONST_EXEC_FROM_FLASH, AT_OPT_ON);
    AT_SetGraphCtrl(AT_GRAPH_SIZE_OPT, AT_OPT_VAL(2));

    SetMemoryDeviceInfos(4,
        AT_MEM_L1, L1Memory, "SSD_tin_can_bottle_L1_Memory", 0, 0,
        AT_MEM_L2, L2Memory, "SSD_tin_can_bottle_L2_Memory", 0, 1,
        AT_MEM_L3_HRAM, L3Memory, "SSD_tin_can_bottle_L3_Memory", 0, 1,
        AT_MEM_L3_HFLASH, L3Flash, "SSD_tin_can_bottle_L3_Flash", "SSD_tin_can_bottle_L3_Flash_Const.dat", 0
    );

    LoadCNN_SQ8_Library();
    LoadSSDLibrary();
    LoadCNN_Copy_Library();


    // generator for CONV_2D_0_0_fusion
    CNN_GenControl_T gen_ctrl_S3_Conv2d_32x3x3x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S3_Conv2d_32x3x3x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S3_Conv2d_32x3x3x3_Relu6, "PADTYPE", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S3_Conv2d_32x3x3x3_Relu6", &gen_ctrl_S3_Conv2d_32x3x3x3_Relu6, 4, 1, 3, 32, 320, 240,
        KOP_CONV, 3, 3, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_1_fusion
    CNN_ConvolutionPoolAct_SQ8("S6_Conv2d_32x1x3x3_Relu6", 0, 4, 1, 32, 32, 160, 120,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_2
    CNN_GenControl_T gen_ctrl_S9_Conv2d_16x32x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S9_Conv2d_16x32x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S9_Conv2d_16x32x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S9_Conv2d_16x32x1x1", &gen_ctrl_S9_Conv2d_16x32x1x1, 4, 1, 32, 16, 160, 120,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_3_fusion
    CNN_GenControl_T gen_ctrl_S12_Conv2d_96x16x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S12_Conv2d_96x16x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S12_Conv2d_96x16x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S12_Conv2d_96x16x1x1_Relu6", &gen_ctrl_S12_Conv2d_96x16x1x1_Relu6, 4, 1, 16, 96, 160, 120,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_4_fusion
    CNN_GenControl_T gen_ctrl_S15_Conv2d_96x1x3x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S15_Conv2d_96x1x3x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S15_Conv2d_96x1x3x3_Relu6, "PADTYPE", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S15_Conv2d_96x1x3x3_Relu6", &gen_ctrl_S15_Conv2d_96x1x3x3_Relu6, 4, 1, 96, 96, 160, 120,
        KOP_CONV_DW, 3, 3, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_5
    CNN_GenControl_T gen_ctrl_S18_Conv2d_24x96x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S18_Conv2d_24x96x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S18_Conv2d_24x96x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S18_Conv2d_24x96x1x1", &gen_ctrl_S18_Conv2d_24x96x1x1, 4, 1, 96, 24, 80, 60,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_6_fusion
    CNN_GenControl_T gen_ctrl_S21_Conv2d_144x24x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S21_Conv2d_144x24x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S21_Conv2d_144x24x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S21_Conv2d_144x24x1x1_Relu6", &gen_ctrl_S21_Conv2d_144x24x1x1_Relu6, 4, 1, 24, 144, 80, 60,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_7_fusion
    CNN_ConvolutionPoolAct_SQ8("S24_Conv2d_144x1x3x3_Relu6", 0, 4, 1, 144, 144, 80, 60,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_8
    CNN_GenControl_T gen_ctrl_S27_Conv2d_24x144x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S27_Conv2d_24x144x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S27_Conv2d_24x144x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S27_Conv2d_24x144x1x1", &gen_ctrl_S27_Conv2d_24x144x1x1, 4, 1, 144, 24, 80, 60,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_9
    CNN_MatAddAct_SQ8("S28_MatAdd_24x60x80", 0, 24, 60, 80, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_10_fusion
    CNN_GenControl_T gen_ctrl_S31_Conv2d_144x24x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S31_Conv2d_144x24x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S31_Conv2d_144x24x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S31_Conv2d_144x24x1x1_Relu6", &gen_ctrl_S31_Conv2d_144x24x1x1_Relu6, 4, 1, 24, 144, 80, 60,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_11_fusion
    CNN_GenControl_T gen_ctrl_S34_Conv2d_144x1x3x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S34_Conv2d_144x1x3x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S34_Conv2d_144x1x3x3_Relu6, "PADTYPE", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S34_Conv2d_144x1x3x3_Relu6", &gen_ctrl_S34_Conv2d_144x1x3x3_Relu6, 4, 1, 144, 144, 80, 60,
        KOP_CONV_DW, 3, 3, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_12
    CNN_GenControl_T gen_ctrl_S37_Conv2d_32x144x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S37_Conv2d_32x144x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S37_Conv2d_32x144x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S37_Conv2d_32x144x1x1", &gen_ctrl_S37_Conv2d_32x144x1x1, 4, 1, 144, 32, 40, 30,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_13_fusion
    CNN_GenControl_T gen_ctrl_S40_Conv2d_192x32x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S40_Conv2d_192x32x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S40_Conv2d_192x32x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S40_Conv2d_192x32x1x1_Relu6", &gen_ctrl_S40_Conv2d_192x32x1x1_Relu6, 4, 1, 32, 192, 40, 30,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_14_fusion
    CNN_ConvolutionPoolAct_SQ8("S43_Conv2d_192x1x3x3_Relu6", 0, 4, 1, 192, 192, 40, 30,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_15
    CNN_GenControl_T gen_ctrl_S46_Conv2d_32x192x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S46_Conv2d_32x192x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S46_Conv2d_32x192x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S46_Conv2d_32x192x1x1", &gen_ctrl_S46_Conv2d_32x192x1x1, 4, 1, 192, 32, 40, 30,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_16
    CNN_MatAddAct_SQ8("S47_MatAdd_32x30x40", 0, 32, 30, 40, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_17_fusion
    CNN_GenControl_T gen_ctrl_S50_Conv2d_192x32x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S50_Conv2d_192x32x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S50_Conv2d_192x32x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S50_Conv2d_192x32x1x1_Relu6", &gen_ctrl_S50_Conv2d_192x32x1x1_Relu6, 4, 1, 32, 192, 40, 30,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_18_fusion
    CNN_ConvolutionPoolAct_SQ8("S53_Conv2d_192x1x3x3_Relu6", 0, 4, 1, 192, 192, 40, 30,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_19
    CNN_GenControl_T gen_ctrl_S56_Conv2d_32x192x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S56_Conv2d_32x192x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S56_Conv2d_32x192x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S56_Conv2d_32x192x1x1", &gen_ctrl_S56_Conv2d_32x192x1x1, 4, 1, 192, 32, 40, 30,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_20
    CNN_MatAddAct_SQ8("S57_MatAdd_32x30x40", 0, 32, 30, 40, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_21_fusion
    CNN_GenControl_T gen_ctrl_S60_Conv2d_192x32x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S60_Conv2d_192x32x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S60_Conv2d_192x32x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S60_Conv2d_192x32x1x1_Relu6", &gen_ctrl_S60_Conv2d_192x32x1x1_Relu6, 4, 1, 32, 192, 40, 30,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_22_fusion
    CNN_GenControl_T gen_ctrl_S63_Conv2d_192x1x3x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S63_Conv2d_192x1x3x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S63_Conv2d_192x1x3x3_Relu6, "PADTYPE", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S63_Conv2d_192x1x3x3_Relu6", &gen_ctrl_S63_Conv2d_192x1x3x3_Relu6, 4, 1, 192, 192, 40, 30,
        KOP_CONV_DW, 3, 3, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_23
    CNN_GenControl_T gen_ctrl_S66_Conv2d_64x192x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S66_Conv2d_64x192x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S66_Conv2d_64x192x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S66_Conv2d_64x192x1x1", &gen_ctrl_S66_Conv2d_64x192x1x1, 4, 1, 192, 64, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_24_fusion
    CNN_GenControl_T gen_ctrl_S69_Conv2d_384x64x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S69_Conv2d_384x64x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S69_Conv2d_384x64x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S69_Conv2d_384x64x1x1_Relu6", &gen_ctrl_S69_Conv2d_384x64x1x1_Relu6, 4, 1, 64, 384, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_25_fusion
    CNN_ConvolutionPoolAct_SQ8("S72_Conv2d_384x1x3x3_Relu6", 0, 4, 1, 384, 384, 20, 15,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_26
    CNN_GenControl_T gen_ctrl_S75_Conv2d_64x384x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S75_Conv2d_64x384x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S75_Conv2d_64x384x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S75_Conv2d_64x384x1x1", &gen_ctrl_S75_Conv2d_64x384x1x1, 4, 1, 384, 64, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_27
    CNN_MatAddAct_SQ8("S76_MatAdd_64x15x20", 0, 64, 15, 20, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_28_fusion
    CNN_GenControl_T gen_ctrl_S79_Conv2d_384x64x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S79_Conv2d_384x64x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S79_Conv2d_384x64x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S79_Conv2d_384x64x1x1_Relu6", &gen_ctrl_S79_Conv2d_384x64x1x1_Relu6, 4, 1, 64, 384, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_29_fusion
    CNN_ConvolutionPoolAct_SQ8("S82_Conv2d_384x1x3x3_Relu6", 0, 4, 1, 384, 384, 20, 15,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_30
    CNN_GenControl_T gen_ctrl_S85_Conv2d_64x384x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S85_Conv2d_64x384x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S85_Conv2d_64x384x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S85_Conv2d_64x384x1x1", &gen_ctrl_S85_Conv2d_64x384x1x1, 4, 1, 384, 64, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_31
    CNN_MatAddAct_SQ8("S86_MatAdd_64x15x20", 0, 64, 15, 20, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_32_fusion
    CNN_GenControl_T gen_ctrl_S89_Conv2d_384x64x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S89_Conv2d_384x64x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S89_Conv2d_384x64x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S89_Conv2d_384x64x1x1_Relu6", &gen_ctrl_S89_Conv2d_384x64x1x1_Relu6, 4, 1, 64, 384, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_33_fusion
    CNN_ConvolutionPoolAct_SQ8("S92_Conv2d_384x1x3x3_Relu6", 0, 4, 1, 384, 384, 20, 15,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_34
    CNN_GenControl_T gen_ctrl_S95_Conv2d_64x384x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S95_Conv2d_64x384x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S95_Conv2d_64x384x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S95_Conv2d_64x384x1x1", &gen_ctrl_S95_Conv2d_64x384x1x1, 4, 1, 384, 64, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_35
    CNN_MatAddAct_SQ8("S96_MatAdd_64x15x20", 0, 64, 15, 20, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_36_fusion
    CNN_GenControl_T gen_ctrl_S99_Conv2d_384x64x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S99_Conv2d_384x64x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S99_Conv2d_384x64x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S99_Conv2d_384x64x1x1_Relu6", &gen_ctrl_S99_Conv2d_384x64x1x1_Relu6, 4, 1, 64, 384, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_37_fusion
    CNN_ConvolutionPoolAct_SQ8("S102_Conv2d_384x1x3x3_Relu6", 0, 4, 1, 384, 384, 20, 15,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_38
    CNN_GenControl_T gen_ctrl_S105_Conv2d_96x384x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S105_Conv2d_96x384x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S105_Conv2d_96x384x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S105_Conv2d_96x384x1x1", &gen_ctrl_S105_Conv2d_96x384x1x1, 4, 1, 384, 96, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_39_fusion
    CNN_GenControl_T gen_ctrl_S108_Conv2d_576x96x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S108_Conv2d_576x96x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S108_Conv2d_576x96x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S108_Conv2d_576x96x1x1_Relu6", &gen_ctrl_S108_Conv2d_576x96x1x1_Relu6, 4, 1, 96, 576, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_40_fusion
    CNN_ConvolutionPoolAct_SQ8("S111_Conv2d_576x1x3x3_Relu6", 0, 4, 1, 576, 576, 20, 15,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_41
    CNN_GenControl_T gen_ctrl_S114_Conv2d_96x576x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S114_Conv2d_96x576x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S114_Conv2d_96x576x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S114_Conv2d_96x576x1x1", &gen_ctrl_S114_Conv2d_96x576x1x1, 4, 1, 576, 96, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_42
    CNN_MatAddAct_SQ8("S115_MatAdd_96x15x20", 0, 96, 15, 20, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_43_fusion
    CNN_GenControl_T gen_ctrl_S118_Conv2d_576x96x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S118_Conv2d_576x96x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S118_Conv2d_576x96x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S118_Conv2d_576x96x1x1_Relu6", &gen_ctrl_S118_Conv2d_576x96x1x1_Relu6, 4, 1, 96, 576, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_44_fusion
    CNN_ConvolutionPoolAct_SQ8("S121_Conv2d_576x1x3x3_Relu6", 0, 4, 1, 576, 576, 20, 15,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_45
    CNN_GenControl_T gen_ctrl_S124_Conv2d_96x576x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S124_Conv2d_96x576x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S124_Conv2d_96x576x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S124_Conv2d_96x576x1x1", &gen_ctrl_S124_Conv2d_96x576x1x1, 4, 1, 576, 96, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_46
    CNN_MatAddAct_SQ8("S125_MatAdd_96x15x20", 0, 96, 15, 20, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_47_fusion
    CNN_GenControl_T gen_ctrl_S128_Conv2d_576x96x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S128_Conv2d_576x96x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S128_Conv2d_576x96x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S128_Conv2d_576x96x1x1_Relu6", &gen_ctrl_S128_Conv2d_576x96x1x1_Relu6, 4, 1, 96, 576, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_48
    CNN_GenControl_T gen_ctrl_S131_Conv2d_12x576x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S131_Conv2d_12x576x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S131_Conv2d_12x576x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S131_Conv2d_12x576x1x1", &gen_ctrl_S131_Conv2d_12x576x1x1, 4, 1, 576, 12, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_48_trans_out0
    // transpose from 12x15x20 to 15x20x12 ((1, 2, 0))
    CNN_3DTensorPermute("S132_Op_CONV_2D_0_48_trans_out0", 0, 1, 12, 20, 15, KOP_MATPERM_CHW2HWC);
    // generator for CONV_2D_0_50_fusion
    CNN_GenControl_T gen_ctrl_S136_Conv2d_9x576x1x1_Hsigmoid;
    CNN_InitGenCtrl(&gen_ctrl_S136_Conv2d_9x576x1x1_Hsigmoid);
    CNN_SetGenCtrl(&gen_ctrl_S136_Conv2d_9x576x1x1_Hsigmoid, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S136_Conv2d_9x576x1x1_Hsigmoid", &gen_ctrl_S136_Conv2d_9x576x1x1_Hsigmoid, 4, 1, 576, 9, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_HSIGMOID);
    // generator for CONV_2D_0_50_trans_out0
    // transpose from 9x15x20 to 15x20x9 ((1, 2, 0))
    CNN_3DTensorPermute("S137_Op_CONV_2D_0_50_trans_out0", 0, 1, 9, 20, 15, KOP_MATPERM_CHW2HWC);
    // generator for DEPTHWISE_CONV_2D_0_52_fusion
    CNN_GenControl_T gen_ctrl_S141_Conv2d_576x1x3x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S141_Conv2d_576x1x3x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S141_Conv2d_576x1x3x3_Relu6, "PADTYPE", AT_OPT_VAL(3));
    CNN_ConvolutionPoolAct_SQ8("S141_Conv2d_576x1x3x3_Relu6", &gen_ctrl_S141_Conv2d_576x1x3x3_Relu6, 4, 1, 576, 576, 20, 15,
        KOP_CONV_DW, 3, 3, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_53
    CNN_GenControl_T gen_ctrl_S144_Conv2d_160x576x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S144_Conv2d_160x576x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S144_Conv2d_160x576x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S144_Conv2d_160x576x1x1", &gen_ctrl_S144_Conv2d_160x576x1x1, 4, 1, 576, 160, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_54_fusion
    CNN_GenControl_T gen_ctrl_S147_Conv2d_960x160x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S147_Conv2d_960x160x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S147_Conv2d_960x160x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S147_Conv2d_960x160x1x1_Relu6", &gen_ctrl_S147_Conv2d_960x160x1x1_Relu6, 4, 1, 160, 960, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_55_fusion
    CNN_ConvolutionPoolAct_SQ8("S150_Conv2d_960x1x3x3_Relu6", 0, 4, 1, 960, 960, 10, 8,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_56
    CNN_GenControl_T gen_ctrl_S153_Conv2d_160x960x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S153_Conv2d_160x960x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S153_Conv2d_160x960x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S153_Conv2d_160x960x1x1", &gen_ctrl_S153_Conv2d_160x960x1x1, 4, 1, 960, 160, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_57
    CNN_MatAddAct_SQ8("S154_MatAdd_160x8x10", 0, 160, 8, 10, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_58_fusion
    CNN_GenControl_T gen_ctrl_S157_Conv2d_960x160x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S157_Conv2d_960x160x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S157_Conv2d_960x160x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S157_Conv2d_960x160x1x1_Relu6", &gen_ctrl_S157_Conv2d_960x160x1x1_Relu6, 4, 1, 160, 960, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_59_fusion
    CNN_ConvolutionPoolAct_SQ8("S160_Conv2d_960x1x3x3_Relu6", 0, 4, 1, 960, 960, 10, 8,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_60
    CNN_GenControl_T gen_ctrl_S163_Conv2d_160x960x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S163_Conv2d_160x960x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S163_Conv2d_160x960x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S163_Conv2d_160x960x1x1", &gen_ctrl_S163_Conv2d_160x960x1x1, 4, 1, 960, 160, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_61
    CNN_MatAddAct_SQ8("S164_MatAdd_160x8x10", 0, 160, 8, 10, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_62_fusion
    CNN_GenControl_T gen_ctrl_S167_Conv2d_960x160x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S167_Conv2d_960x160x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S167_Conv2d_960x160x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S167_Conv2d_960x160x1x1_Relu6", &gen_ctrl_S167_Conv2d_960x160x1x1_Relu6, 4, 1, 160, 960, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_63_fusion
    CNN_ConvolutionPoolAct_SQ8("S170_Conv2d_960x1x3x3_Relu6", 0, 4, 1, 960, 960, 10, 8,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_64
    CNN_GenControl_T gen_ctrl_S173_Conv2d_320x960x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S173_Conv2d_320x960x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S173_Conv2d_320x960x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S173_Conv2d_320x960x1x1", &gen_ctrl_S173_Conv2d_320x960x1x1, 4, 1, 960, 320, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_65_fusion
    CNN_GenControl_T gen_ctrl_S176_Conv2d_1280x320x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S176_Conv2d_1280x320x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S176_Conv2d_1280x320x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S176_Conv2d_1280x320x1x1_Relu6", &gen_ctrl_S176_Conv2d_1280x320x1x1_Relu6, 4, 1, 320, 1280, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_66
    CNN_GenControl_T gen_ctrl_S179_Conv2d_24x1280x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S179_Conv2d_24x1280x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S179_Conv2d_24x1280x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S179_Conv2d_24x1280x1x1", &gen_ctrl_S179_Conv2d_24x1280x1x1, 4, 1, 1280, 24, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_66_trans_out0
    // transpose from 24x8x10 to 8x10x24 ((1, 2, 0))
    CNN_3DTensorPermute("S180_Op_CONV_2D_0_66_trans_out0", 0, 1, 24, 10, 8, KOP_MATPERM_CHW2HWC);
    // generator for CONV_2D_0_69_fusion
    CNN_GenControl_T gen_ctrl_S184_Conv2d_18x1280x1x1_Hsigmoid;
    CNN_InitGenCtrl(&gen_ctrl_S184_Conv2d_18x1280x1x1_Hsigmoid);
    CNN_SetGenCtrl(&gen_ctrl_S184_Conv2d_18x1280x1x1_Hsigmoid, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S184_Conv2d_18x1280x1x1_Hsigmoid", &gen_ctrl_S184_Conv2d_18x1280x1x1_Hsigmoid, 4, 1, 1280, 18, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_HSIGMOID);
    // generator for CONV_2D_0_69_trans_out0
    // transpose from 18x8x10 to 8x10x18 ((1, 2, 0))
    CNN_3DTensorPermute("S185_Op_CONV_2D_0_69_trans_out0", 0, 1, 18, 10, 8, KOP_MATPERM_CHW2HWC);
    // generator for CONV_2D_0_72_fusion
    CNN_GenControl_T gen_ctrl_S189_Conv2d_256x1280x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S189_Conv2d_256x1280x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S189_Conv2d_256x1280x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S189_Conv2d_256x1280x1x1_Relu6", &gen_ctrl_S189_Conv2d_256x1280x1x1_Relu6, 4, 1, 1280, 256, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_73_fusion
    CNN_GenControl_T gen_ctrl_S192_Conv2d_512x256x3x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S192_Conv2d_512x256x3x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S192_Conv2d_512x256x3x3_Relu6, "PADTYPE", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S192_Conv2d_512x256x3x3_Relu6", &gen_ctrl_S192_Conv2d_512x256x3x3_Relu6, 4, 1, 256, 512, 10, 8,
        KOP_CONV, 3, 3, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_74
    CNN_GenControl_T gen_ctrl_S195_Conv2d_24x512x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S195_Conv2d_24x512x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S195_Conv2d_24x512x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S195_Conv2d_24x512x1x1", &gen_ctrl_S195_Conv2d_24x512x1x1, 4, 1, 512, 24, 5, 4,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_74_trans_out0
    // transpose from 24x4x5 to 4x5x24 ((1, 2, 0))
    CNN_3DTensorPermute("S196_Op_CONV_2D_0_74_trans_out0", 0, 1, 24, 5, 4, KOP_MATPERM_CHW2HWC);
    // generator for CONV_2D_0_77_fusion
    CNN_GenControl_T gen_ctrl_S200_Conv2d_18x512x1x1_Hsigmoid;
    CNN_InitGenCtrl(&gen_ctrl_S200_Conv2d_18x512x1x1_Hsigmoid);
    CNN_SetGenCtrl(&gen_ctrl_S200_Conv2d_18x512x1x1_Hsigmoid, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S200_Conv2d_18x512x1x1_Hsigmoid", &gen_ctrl_S200_Conv2d_18x512x1x1_Hsigmoid, 4, 1, 512, 18, 5, 4,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_HSIGMOID);
    // generator for CONV_2D_0_77_trans_out0
    // transpose from 18x4x5 to 4x5x18 ((1, 2, 0))
    CNN_3DTensorPermute("S201_Op_CONV_2D_0_77_trans_out0", 0, 1, 18, 5, 4, KOP_MATPERM_CHW2HWC);
    // generator for CONV_2D_0_80_fusion
    CNN_GenControl_T gen_ctrl_S205_Conv2d_128x512x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S205_Conv2d_128x512x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S205_Conv2d_128x512x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S205_Conv2d_128x512x1x1_Relu6", &gen_ctrl_S205_Conv2d_128x512x1x1_Relu6, 4, 1, 512, 128, 5, 4,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_81_fusion
    CNN_GenControl_T gen_ctrl_S208_Conv2d_256x128x3x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S208_Conv2d_256x128x3x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S208_Conv2d_256x128x3x3_Relu6, "PADTYPE", AT_OPT_VAL(3));
    CNN_ConvolutionPoolAct_SQ8("S208_Conv2d_256x128x3x3_Relu6", &gen_ctrl_S208_Conv2d_256x128x3x3_Relu6, 4, 1, 128, 256, 5, 4,
        KOP_CONV, 3, 3, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_82
    CNN_GenControl_T gen_ctrl_S211_Conv2d_24x256x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S211_Conv2d_24x256x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S211_Conv2d_24x256x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S211_Conv2d_24x256x1x1", &gen_ctrl_S211_Conv2d_24x256x1x1, 4, 1, 256, 24, 3, 2,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_82_trans_out0
    // transpose from 24x2x3 to 2x3x24 ((1, 2, 0))
    CNN_3DTensorPermute("S212_Op_CONV_2D_0_82_trans_out0", 0, 1, 24, 3, 2, KOP_MATPERM_CHW2HWC);
    // generator for CONV_2D_0_85_fusion
    CNN_GenControl_T gen_ctrl_S216_Conv2d_18x256x1x1_Hsigmoid;
    CNN_InitGenCtrl(&gen_ctrl_S216_Conv2d_18x256x1x1_Hsigmoid);
    CNN_SetGenCtrl(&gen_ctrl_S216_Conv2d_18x256x1x1_Hsigmoid, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S216_Conv2d_18x256x1x1_Hsigmoid", &gen_ctrl_S216_Conv2d_18x256x1x1_Hsigmoid, 4, 1, 256, 18, 3, 2,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_HSIGMOID);
    // generator for CONV_2D_0_85_trans_out0
    // transpose from 18x2x3 to 2x3x18 ((1, 2, 0))
    CNN_3DTensorPermute("S217_Op_CONV_2D_0_85_trans_out0", 0, 1, 18, 3, 2, KOP_MATPERM_CHW2HWC);
    // generator for CONV_2D_0_88_fusion
    CNN_GenControl_T gen_ctrl_S221_Conv2d_128x256x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S221_Conv2d_128x256x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S221_Conv2d_128x256x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S221_Conv2d_128x256x1x1_Relu6", &gen_ctrl_S221_Conv2d_128x256x1x1_Relu6, 4, 1, 256, 128, 3, 2,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_89_fusion
    CNN_GenControl_T gen_ctrl_S224_Conv2d_256x128x3x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S224_Conv2d_256x128x3x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S224_Conv2d_256x128x3x3_Relu6, "PADTYPE", AT_OPT_VAL(3));
    CNN_ConvolutionPoolAct_SQ8("S224_Conv2d_256x128x3x3_Relu6", &gen_ctrl_S224_Conv2d_256x128x3x3_Relu6, 4, 1, 128, 256, 3, 2,
        KOP_CONV, 3, 3, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_90
    CNN_GenControl_T gen_ctrl_S227_Conv2d_24x256x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S227_Conv2d_24x256x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S227_Conv2d_24x256x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S227_Conv2d_24x256x1x1", &gen_ctrl_S227_Conv2d_24x256x1x1, 4, 1, 256, 24, 2, 1,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_90_trans_out0
    // transpose from 24x1x2 to 1x2x24 ((1, 0))
    CNN_MatTranspose("S228_Op_CONV_2D_0_90_trans_out0", 0, 1, 1, 2, 24, KOP_MATTRANSP);
    // generator for CONV_2D_0_93_fusion
    CNN_GenControl_T gen_ctrl_S232_Conv2d_18x256x1x1_Hsigmoid;
    CNN_InitGenCtrl(&gen_ctrl_S232_Conv2d_18x256x1x1_Hsigmoid);
    CNN_SetGenCtrl(&gen_ctrl_S232_Conv2d_18x256x1x1_Hsigmoid, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S232_Conv2d_18x256x1x1_Hsigmoid", &gen_ctrl_S232_Conv2d_18x256x1x1_Hsigmoid, 4, 1, 256, 18, 2, 1,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_HSIGMOID);
    // generator for CONV_2D_0_93_trans_out0
    // transpose from 18x1x2 to 1x2x18 ((1, 0))
    CNN_MatTranspose("S233_Op_CONV_2D_0_93_trans_out0", 0, 1, 1, 2, 18, KOP_MATTRANSP);
    // generator for CONV_2D_0_96_fusion
    CNN_GenControl_T gen_ctrl_S237_Conv2d_64x256x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S237_Conv2d_64x256x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S237_Conv2d_64x256x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S237_Conv2d_64x256x1x1_Relu6", &gen_ctrl_S237_Conv2d_64x256x1x1_Relu6, 4, 1, 256, 64, 2, 1,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_97_fusion
    CNN_GenControl_T gen_ctrl_S240_Conv2d_128x64x1x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S240_Conv2d_128x64x1x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S240_Conv2d_128x64x1x3_Relu6, "PADTYPE", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S240_Conv2d_128x64x1x3_Relu6", &gen_ctrl_S240_Conv2d_128x64x1x3_Relu6, 4, 1, 64, 128, 2, 1,
        KOP_CONV, 3, 1, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_98
    CNN_GenControl_T gen_ctrl_S243_Conv2d_24x128x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S243_Conv2d_24x128x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S243_Conv2d_24x128x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S243_Conv2d_24x128x1x1", &gen_ctrl_S243_Conv2d_24x128x1x1, 4, 1, 128, 24, 1, 1,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_104_fusion
    CNN_GenControl_T gen_ctrl_S249_Conv2d_18x128x1x1_Hsigmoid;
    CNN_InitGenCtrl(&gen_ctrl_S249_Conv2d_18x128x1x1_Hsigmoid);
    CNN_SetGenCtrl(&gen_ctrl_S249_Conv2d_18x128x1x1_Hsigmoid, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S249_Conv2d_18x128x1x1_Hsigmoid", &gen_ctrl_S249_Conv2d_18x128x1x1_Hsigmoid, 4, 1, 128, 18, 1, 1,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_HSIGMOID);
    
    // generator for CUSTOM_0_110
    CNN_SSD_PostProcess_SQ8("S253_Op_CUSTOM_0_110", 0, 1554, 3, 10, 300, 0.000, 0.500);
    

#define GRAPH
#ifdef GRAPH
    CreateGraph("SSD_tin_can_bottleCNN",
        /* Arguments either passed or globals */
            CArgs(377,
                TCArgInfo("signed char * __restrict__", "Input_1", ARG_SCOPE_ARG, ARG_DIR_IN, AT_MEM_L3_HRAM, AT_MEM_L3_HRAM, 0),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2con_6e32d451", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2con_6e32d451.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2con_9058f71e", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2con_9058f71e.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S3_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S3_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S3_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S3_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S3_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S3_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_dac2de23", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_dac2de23.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_7630c70d", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_7630c70d.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S6_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S6_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S6_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S6_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S6_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S6_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_0e35af86", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_0e35af86.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_54323b1e", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_54323b1e.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S9_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S9_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S9_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S9_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S9_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S9_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_6868d4c8", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_6868d4c8.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_68f5cacf", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_68f5cacf.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S12_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S12_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S12_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S12_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S12_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S12_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_a924d691", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_a924d691.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_eab262e5", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_eab262e5.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S15_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S15_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S15_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S15_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S15_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S15_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_d016b58d", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_d016b58d.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_071bb161", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_071bb161.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S18_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S18_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S18_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S18_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S18_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S18_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_922ecd94", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_922ecd94.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_e694b757", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_e694b757.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S21_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S21_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S21_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S21_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S21_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S21_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_e4a41008", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_e4a41008.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_2a23b754", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_2a23b754.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S24_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S24_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S24_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S24_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S24_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S24_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_db0a9ada", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_db0a9ada.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_8cd1ecfc", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_8cd1ecfc.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S27_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S27_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S27_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S27_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S27_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S27_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 153 In1ScaleN: 7 OutScale: 185 OutScaleN: 8
                TCArgInfo("signed char * __restrict__", "S28_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S28_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_a61a4ff1", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_a61a4ff1.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_44e55ef1", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_44e55ef1.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S31_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S31_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S31_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S31_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S31_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S31_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_4c175508", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_4c175508.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_1a33d820", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_1a33d820.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S34_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S34_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S34_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S34_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S34_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S34_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_8ad79f5c", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_8ad79f5c.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_3ab24af1", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_3ab24af1.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S37_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S37_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S37_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S37_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S37_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S37_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_225de092", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_225de092.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_72f4a37a", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_72f4a37a.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S40_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S40_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S40_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S40_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S40_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S40_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_24e95c58", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_24e95c58.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_4538386e", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_4538386e.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S43_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S43_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S43_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S43_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S43_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S43_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_48b245c5", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_48b245c5.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_66f6d069", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_66f6d069.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S46_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S46_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S46_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S46_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S46_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S46_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 143 In1ScaleN: 7 OutScale: 27 OutScaleN: 5
                TCArgInfo("signed char * __restrict__", "S47_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S47_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_43ab4e7d", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_43ab4e7d.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_4a22f47b", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_4a22f47b.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S50_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S50_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S50_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S50_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S50_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S50_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_d1754f09", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_d1754f09.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_d4437b58", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_d4437b58.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S53_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S53_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S53_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S53_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S53_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S53_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_070c419d", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_070c419d.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_0cec3764", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_0cec3764.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S56_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S56_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S56_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S56_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S56_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S56_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 9 In1ScaleN: 3 OutScale: 25 OutScaleN: 5
                TCArgInfo("signed char * __restrict__", "S57_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S57_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_3693aa42", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_3693aa42.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_72b50c0a", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_72b50c0a.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S60_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S60_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S60_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S60_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S60_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S60_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_fb92201b", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_fb92201b.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_1d0acefd", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_1d0acefd.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S63_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S63_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S63_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S63_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S63_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S63_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_e9f95d46", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_e9f95d46.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_cdf3d143", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_cdf3d143.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S66_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S66_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S66_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S66_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S66_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S66_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_89048f6a", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_89048f6a.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_0ed4f8ee", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_0ed4f8ee.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S69_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S69_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S69_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S69_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S69_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S69_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_68d6fe31", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_68d6fe31.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_020fdb92", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_020fdb92.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S72_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S72_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S72_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S72_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S72_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S72_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_8022d855", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_8022d855.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_63ed3f08", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_63ed3f08.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S75_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S75_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S75_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S75_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S75_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S75_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 173 In1ScaleN: 7 OutScale: 11 OutScaleN: 4
                TCArgInfo("signed char * __restrict__", "S76_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S76_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_d7a27a34", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_d7a27a34.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_6b47e064", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_6b47e064.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S79_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S79_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S79_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S79_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S79_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S79_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_98c48eca", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_98c48eca.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_badef1d6", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_badef1d6.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S82_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S82_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S82_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S82_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S82_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S82_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_10a44943", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_10a44943.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_1c874c2d", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_1c874c2d.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S85_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S85_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S85_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S85_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S85_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S85_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 185 In1ScaleN: 7 OutScale: 153 OutScaleN: 8
                TCArgInfo("signed char * __restrict__", "S86_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S86_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_dd90b460", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_dd90b460.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_31bd8e31", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_31bd8e31.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S89_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S89_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S89_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S89_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S89_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S89_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_98660d0b", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_98660d0b.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_81193444", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_81193444.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S92_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S92_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S92_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S92_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S92_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S92_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_13c8766a", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_13c8766a.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_cb123231", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_cb123231.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S95_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S95_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S95_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S95_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S95_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S95_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 39 In1ScaleN: 5 OutScale: 163 OutScaleN: 8
                TCArgInfo("signed char * __restrict__", "S96_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S96_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_bc1e6bd0", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_bc1e6bd0.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_234f34d0", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_234f34d0.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S99_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S99_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S99_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S99_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S99_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S99_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_2a46f942", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_2a46f942.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_0786b546", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_0786b546.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S102_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S102_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S102_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S102_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S102_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S102_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_eeb8ab91", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_eeb8ab91.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_4fdaedaf", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_4fdaedaf.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S105_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S105_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S105_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S105_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S105_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S105_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_ce4b614b", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_ce4b614b.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_4e7bedd6", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_4e7bedd6.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S108_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S108_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S108_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S108_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S108_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S108_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_91e62c4b", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_91e62c4b.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_10c1fd87", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_10c1fd87.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S111_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S111_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S111_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S111_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S111_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S111_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_52133b70", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_52133b70.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_a5e99dd2", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_a5e99dd2.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S114_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S114_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S114_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S114_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S114_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S114_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 135 In1ScaleN: 7 OutScale: 3 OutScaleN: 2
                TCArgInfo("signed char * __restrict__", "S115_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S115_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_412321df", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_412321df.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_5de61ec5", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_5de61ec5.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S118_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S118_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S118_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S118_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S118_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S118_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_c58d241b", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_c58d241b.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_0438cc0f", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_0438cc0f.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S121_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S121_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S121_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S121_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S121_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S121_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_b7b986df", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_b7b986df.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_4a14691f", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_4a14691f.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S124_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S124_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S124_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S124_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S124_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S124_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 43 In1ScaleN: 5 OutScale: 153 OutScaleN: 8
                TCArgInfo("signed char * __restrict__", "S125_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S125_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_a9a81d58", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_a9a81d58.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_f8dde384", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_f8dde384.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S128_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S128_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S128_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S128_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S128_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S128_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_0boxencodingpredi_3ef25574", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_0boxencodingpredi_3ef25574.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_0boxencodingpredi", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_0boxencodingpredi.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S131_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S131_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S131_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S131_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S131_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S131_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_0classpredictorco", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_0classpredictorco.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_0classpredictorbi", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_0classpredictorbi.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S136_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S136_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S136_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S136_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.260622 out: 0.007843 qbias: 177 qnorm: 5 A0: 23 B0: 12 C0: 1
                TCArgInfo("signed char * __restrict__", "S136_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S136_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_c51751b2", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_c51751b2.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_661144ee", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_661144ee.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S141_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S141_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S141_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S141_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S141_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S141_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_a2274156", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_a2274156.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_c87ef171", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_c87ef171.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S144_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S144_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S144_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S144_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S144_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S144_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_84c563fe", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_84c563fe.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_b050988d", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_b050988d.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S147_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S147_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S147_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S147_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S147_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S147_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_92a94755", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_92a94755.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_47549dfd", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_47549dfd.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S150_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S150_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S150_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S150_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S150_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S150_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_abd28f6c", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_abd28f6c.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_4ae70677", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_4ae70677.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S153_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S153_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S153_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S153_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S153_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S153_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 39 In1ScaleN: 5 OutScale: 157 OutScaleN: 8
                TCArgInfo("signed char * __restrict__", "S154_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S154_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_4bfd1ce4", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_4bfd1ce4.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_2c2e1c56", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_2c2e1c56.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S157_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S157_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S157_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S157_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S157_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S157_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_4c239961", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_4c239961.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_d2dba10d", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_d2dba10d.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S160_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S160_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S160_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S160_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S160_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S160_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_5d81017c", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_5d81017c.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_18e7e1f9", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_18e7e1f9.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S163_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S163_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S163_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S163_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S163_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S163_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 167 In1ScaleN: 7 OutScale: 133 OutScaleN: 8
                TCArgInfo("signed char * __restrict__", "S164_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S164_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_6ce3ab45", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_6ce3ab45.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_f43d5057", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_f43d5057.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S167_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S167_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S167_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S167_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S167_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S167_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_fe48a6f4", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_fe48a6f4.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_5faea3cb", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_5faea3cb.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S170_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S170_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S170_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S170_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S170_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S170_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_db31bcec", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_db31bcec.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S173_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S173_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S173_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S173_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S173_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S173_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2con_04ed4f25", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2con_04ed4f25.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2con", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2con.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S176_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S176_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S176_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S176_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S176_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S176_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_1boxencodingpredi_185fd89e", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_1boxencodingpredi_185fd89e.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_1boxencodingpredi", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_1boxencodingpredi.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S179_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S179_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S179_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S179_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S179_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S179_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_1classpredictorco", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_1classpredictorco.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_1classpredictorbi", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_1classpredictorbi.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S184_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S184_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S184_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S184_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.260622 out: 0.007843 qbias: 177 qnorm: 5 A0: 23 B0: 12 C0: 1
                TCArgInfo("signed char * __restrict__", "S184_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S184_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2lay_7d739858", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_7d739858.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2lay_7b3686d5", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_7b3686d5.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S189_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S189_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S189_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S189_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S189_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S189_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2lay_93e7cbd6", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_93e7cbd6.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2lay_0fde7b1a", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_0fde7b1a.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S192_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S192_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S192_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S192_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S192_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S192_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_2boxencodingpredi_bde07c43", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_2boxencodingpredi_bde07c43.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_2boxencodingpredi", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_2boxencodingpredi.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S195_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S195_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S195_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S195_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S195_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S195_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_2classpredictorco", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_2classpredictorco.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_2classpredictorbi", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_2classpredictorbi.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S200_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S200_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S200_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S200_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.260622 out: 0.007843 qbias: 177 qnorm: 5 A0: 23 B0: 12 C0: 1
                TCArgInfo("signed char * __restrict__", "S200_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S200_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2lay_77ac8956", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_77ac8956.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2lay_bbbdea67", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_bbbdea67.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S205_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S205_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S205_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S205_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S205_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S205_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2lay_4f2e096c", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_4f2e096c.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2lay_20136f8b", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_20136f8b.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S208_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S208_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S208_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S208_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S208_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S208_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_3boxencodingpredi_34c879aa", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_3boxencodingpredi_34c879aa.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_3boxencodingpredi", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_3boxencodingpredi.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S211_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S211_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S211_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S211_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S211_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S211_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_3classpredictorco", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_3classpredictorco.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_3classpredictorbi", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_3classpredictorbi.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S216_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S216_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S216_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S216_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.260622 out: 0.007843 qbias: 177 qnorm: 5 A0: 23 B0: 12 C0: 1
                TCArgInfo("signed char * __restrict__", "S216_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S216_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2lay_c9953090", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_c9953090.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2lay_8b904905", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_8b904905.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S221_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S221_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S221_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S221_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S221_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S221_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2lay_1b54ffcb", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_1b54ffcb.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2lay_df75bdd5", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_df75bdd5.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S224_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S224_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S224_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S224_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S224_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S224_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_4boxencodingpredi_8df609c1", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_4boxencodingpredi_8df609c1.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_4boxencodingpredi", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_4boxencodingpredi.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S227_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S227_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S227_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S227_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S227_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S227_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_4classpredictorco", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_4classpredictorco.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_4classpredictorbi", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_4classpredictorbi.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S232_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S232_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S232_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S232_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.260622 out: 0.007843 qbias: 177 qnorm: 5 A0: 23 B0: 12 C0: 1
                TCArgInfo("signed char * __restrict__", "S232_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S232_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2lay_9f8f14fa", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_9f8f14fa.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2lay_f48e9fc3", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_f48e9fc3.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S237_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S237_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S237_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S237_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S237_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S237_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2lay_4c0e1d34", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_4c0e1d34.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2lay", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S240_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S240_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S240_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S240_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S240_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S240_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_5boxencodingpredi_f05378fe", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_5boxencodingpredi_f05378fe.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_5boxencodingpredi", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_5boxencodingpredi.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S243_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S243_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S243_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S243_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S243_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S243_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_5classpredictorco", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_5classpredictorco.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_5classpredictorbi", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_5classpredictorbi.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S249_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S249_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S249_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S249_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.260622 out: 0.007843 qbias: 177 qnorm: 5 A0: 23 B0: 12 C0: 1
                TCArgInfo("signed char * __restrict__", "S249_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S249_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Anchors", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Anchors.tensor", 1, 1, 8, 0)),
                TCArgInfo("unsigned char * __restrict__", "S253_Ssd_scales", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S253_Ssd_scales.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S253_Ssd_norms", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S253_Ssd_norms.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed short * __restrict__", "Output_1", ARG_SCOPE_ARG, ARG_DIR_OUT, AT_MEM_L2, AT_MEM_L2, 0),
                TCArgInfo("signed char * __restrict__", "Output_2", ARG_SCOPE_ARG, ARG_DIR_OUT, AT_MEM_L2, AT_MEM_L2, 0),
                TCArgInfo("signed char * __restrict__", "Output_3", ARG_SCOPE_ARG, ARG_DIR_OUT, AT_MEM_L2, AT_MEM_L2, 0)
            ),
        /* Locals, allocated dynamically */
        CArgs(82,
            TCArgInfo("signed char * __restrict__", "S3_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S6_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S9_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S12_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S15_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S18_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S21_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S24_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S27_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S28_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S31_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S34_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S37_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S40_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S43_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S46_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S47_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S50_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S53_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S56_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S57_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S60_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S63_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S66_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S69_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S72_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S75_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S76_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S79_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S82_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S85_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S86_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S89_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S92_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S95_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S96_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S99_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S102_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S105_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S108_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S111_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S114_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S115_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S118_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S121_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S124_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S125_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S128_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S131_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S136_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S141_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S144_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S147_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S150_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S153_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S154_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S157_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S160_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S163_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S164_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S167_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S170_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S173_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S176_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S179_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S184_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S189_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S192_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S195_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S200_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S205_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S208_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S211_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S216_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S221_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S224_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S227_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S232_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S237_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S240_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S245_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S251_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0)
        )
    );

    /* Stacked tensors - Concats */
    AddStackedTensors("S245_Output", 6, "S132_Output", "S180_Output", "S196_Output", "S212_Output", "S228_Output", "S243_Output");
    AddStackedTensors("S251_Output", 6, "S137_Output", "S185_Output", "S201_Output", "S217_Output", "S233_Output", "S249_Output");

    // Node S3_Conv2d_32x3x3x3_Relu6 inq -1.00<(i8-0.00)*0.00784314<1.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S3_Conv2d_32x3x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "Input_1", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2con_6e32d451", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2con_9058f71e", 0), GNodeArg(GNA_OUT, "S3_Output", 0), GNodeArg(GNA_IN, "S3_Mul_scale", 0), GNodeArg(GNA_IN, "S3_Mul_shift", 0), GNodeArg(GNA_IN, "S3_Infos", 0)));
    // Node S6_Conv2d_32x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S6_Conv2d_32x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S3_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_dac2de23", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_7630c70d", 0), GNodeArg(GNA_OUT, "S6_Output", 0), GNodeArg(GNA_IN, "S6_Mul_scale", 0), GNodeArg(GNA_IN, "S6_Mul_shift", 0), GNodeArg(GNA_IN, "S6_Infos", 0)));
    // Node S9_Conv2d_16x32x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -36.22<(i8-0.00)*0.28298804<35.94 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S9_Conv2d_16x32x1x1", Bindings(7, GNodeArg(GNA_IN, "S6_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_0e35af86", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_54323b1e", 0), GNodeArg(GNA_OUT, "S9_Output", 0), GNodeArg(GNA_IN, "S9_Mul_scale", 0), GNodeArg(GNA_IN, "S9_Mul_shift", 0), GNodeArg(GNA_IN, "S9_Infos", 0)));
    // Node S12_Conv2d_96x16x1x1_Relu6 inq -36.22<(i8-0.00)*0.28298804<35.94 weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S12_Conv2d_96x16x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S9_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_6868d4c8", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_68f5cacf", 0), GNodeArg(GNA_OUT, "S12_Output", 0), GNodeArg(GNA_IN, "S12_Mul_scale", 0), GNodeArg(GNA_IN, "S12_Mul_shift", 0), GNodeArg(GNA_IN, "S12_Infos", 0)));
    // Node S15_Conv2d_96x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S15_Conv2d_96x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S12_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_a924d691", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_eab262e5", 0), GNodeArg(GNA_OUT, "S15_Output", 0), GNodeArg(GNA_IN, "S15_Mul_scale", 0), GNodeArg(GNA_IN, "S15_Mul_shift", 0), GNodeArg(GNA_IN, "S15_Infos", 0)));
    // Node S18_Conv2d_24x96x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -28.00<(i8-0.00)*0.21877038<27.78 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S18_Conv2d_24x96x1x1", Bindings(7, GNodeArg(GNA_IN, "S15_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_d016b58d", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_071bb161", 0), GNodeArg(GNA_OUT, "S18_Output", 0), GNodeArg(GNA_IN, "S18_Mul_scale", 0), GNodeArg(GNA_IN, "S18_Mul_shift", 0), GNodeArg(GNA_IN, "S18_Infos", 0)));
    // Node S21_Conv2d_144x24x1x1_Relu6 inq -28.00<(i8-0.00)*0.21877038<27.78 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S21_Conv2d_144x24x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S18_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_922ecd94", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_e694b757", 0), GNodeArg(GNA_OUT, "S21_Output", 0), GNodeArg(GNA_IN, "S21_Mul_scale", 0), GNodeArg(GNA_IN, "S21_Mul_shift", 0), GNodeArg(GNA_IN, "S21_Infos", 0)));
    // Node S24_Conv2d_144x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S24_Conv2d_144x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S21_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_e4a41008", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_2a23b754", 0), GNodeArg(GNA_OUT, "S24_Output", 0), GNodeArg(GNA_IN, "S24_Mul_scale", 0), GNodeArg(GNA_IN, "S24_Mul_shift", 0), GNodeArg(GNA_IN, "S24_Infos", 0)));
    // Node S27_Conv2d_24x144x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -33.37<(i8-0.00)*0.26070723<33.11 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S27_Conv2d_24x144x1x1", Bindings(7, GNodeArg(GNA_IN, "S24_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_db0a9ada", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_8cd1ecfc", 0), GNodeArg(GNA_OUT, "S27_Output", 0), GNodeArg(GNA_IN, "S27_Mul_scale", 0), GNodeArg(GNA_IN, "S27_Mul_shift", 0), GNodeArg(GNA_IN, "S27_Infos", 0)));
    // Node S28_MatAdd_24x60x80 in1q -33.37<(i8-0.00)*0.26070723<33.11 forced in2q -28.00<(i8-0.00)*0.21877038<27.78 forced outq -38.65<(i8-0.00)*0.30192825<38.34 forced
    AddNode("S28_MatAdd_24x60x80", Bindings(4, GNodeArg(GNA_IN, "S27_Output", 0), GNodeArg(GNA_IN, "S18_Output", 0), GNodeArg(GNA_OUT, "S28_Output", 0), GNodeArg(GNA_IN, "S28_Infos", 0)));
    // Node S31_Conv2d_144x24x1x1_Relu6 inq -38.65<(i8-0.00)*0.30192825<38.34 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S31_Conv2d_144x24x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S28_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_a61a4ff1", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_44e55ef1", 0), GNodeArg(GNA_OUT, "S31_Output", 0), GNodeArg(GNA_IN, "S31_Mul_scale", 0), GNodeArg(GNA_IN, "S31_Mul_shift", 0), GNodeArg(GNA_IN, "S31_Infos", 0)));
    // Node S34_Conv2d_144x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S34_Conv2d_144x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S31_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_4c175508", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_1a33d820", 0), GNodeArg(GNA_OUT, "S34_Output", 0), GNodeArg(GNA_IN, "S34_Mul_scale", 0), GNodeArg(GNA_IN, "S34_Mul_shift", 0), GNodeArg(GNA_IN, "S34_Infos", 0)));
    // Node S37_Conv2d_32x144x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -18.83<(i8-0.00)*0.14710091<18.68 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S37_Conv2d_32x144x1x1", Bindings(7, GNodeArg(GNA_IN, "S34_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_8ad79f5c", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_3ab24af1", 0), GNodeArg(GNA_OUT, "S37_Output", 0), GNodeArg(GNA_IN, "S37_Mul_scale", 0), GNodeArg(GNA_IN, "S37_Mul_shift", 0), GNodeArg(GNA_IN, "S37_Infos", 0)));
    // Node S40_Conv2d_192x32x1x1_Relu6 inq -18.83<(i8-0.00)*0.14710091<18.68 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S40_Conv2d_192x32x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S37_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_225de092", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_72f4a37a", 0), GNodeArg(GNA_OUT, "S40_Output", 0), GNodeArg(GNA_IN, "S40_Mul_scale", 0), GNodeArg(GNA_IN, "S40_Mul_shift", 0), GNodeArg(GNA_IN, "S40_Infos", 0)));
    // Node S43_Conv2d_192x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S43_Conv2d_192x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S40_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_24e95c58", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_4538386e", 0), GNodeArg(GNA_OUT, "S43_Output", 0), GNodeArg(GNA_IN, "S43_Mul_scale", 0), GNodeArg(GNA_IN, "S43_Mul_shift", 0), GNodeArg(GNA_IN, "S43_Infos", 0)));
    // Node S46_Conv2d_32x192x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -21.00<(i8-0.00)*0.16408615<20.84 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S46_Conv2d_32x192x1x1", Bindings(7, GNodeArg(GNA_IN, "S43_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_48b245c5", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_66f6d069", 0), GNodeArg(GNA_OUT, "S46_Output", 0), GNodeArg(GNA_IN, "S46_Mul_scale", 0), GNodeArg(GNA_IN, "S46_Mul_shift", 0), GNodeArg(GNA_IN, "S46_Infos", 0)));
    // Node S47_MatAdd_32x30x40 in1q -21.00<(i8-0.00)*0.16408615<20.84 forced in2q -18.83<(i8-0.00)*0.14710091<18.68 forced outq -22.31<(i8-0.00)*0.17426409<22.13 forced
    AddNode("S47_MatAdd_32x30x40", Bindings(4, GNodeArg(GNA_IN, "S46_Output", 0), GNodeArg(GNA_IN, "S37_Output", 0), GNodeArg(GNA_OUT, "S47_Output", 0), GNodeArg(GNA_IN, "S47_Infos", 0)));
    // Node S50_Conv2d_192x32x1x1_Relu6 inq -22.31<(i8-0.00)*0.17426409<22.13 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S50_Conv2d_192x32x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S47_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_43ab4e7d", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_4a22f47b", 0), GNodeArg(GNA_OUT, "S50_Output", 0), GNodeArg(GNA_IN, "S50_Mul_scale", 0), GNodeArg(GNA_IN, "S50_Mul_shift", 0), GNodeArg(GNA_IN, "S50_Infos", 0)));
    // Node S53_Conv2d_192x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S53_Conv2d_192x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S50_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_d1754f09", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_d4437b58", 0), GNodeArg(GNA_OUT, "S53_Output", 0), GNodeArg(GNA_IN, "S53_Mul_scale", 0), GNodeArg(GNA_IN, "S53_Mul_shift", 0), GNodeArg(GNA_IN, "S53_Infos", 0)));
    // Node S56_Conv2d_32x192x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -19.83<(i8-0.00)*0.15494861<19.68 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S56_Conv2d_32x192x1x1", Bindings(7, GNodeArg(GNA_IN, "S53_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_070c419d", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_0cec3764", 0), GNodeArg(GNA_OUT, "S56_Output", 0), GNodeArg(GNA_IN, "S56_Mul_scale", 0), GNodeArg(GNA_IN, "S56_Mul_shift", 0), GNodeArg(GNA_IN, "S56_Infos", 0)));
    // Node S57_MatAdd_32x30x40 in1q -22.31<(i8-0.00)*0.17426409<22.13 forced in2q -19.83<(i8-0.00)*0.15494861<19.68 forced outq -25.36<(i8-0.00)*0.19811144<25.16 forced
    AddNode("S57_MatAdd_32x30x40", Bindings(4, GNodeArg(GNA_IN, "S47_Output", 0), GNodeArg(GNA_IN, "S56_Output", 0), GNodeArg(GNA_OUT, "S57_Output", 0), GNodeArg(GNA_IN, "S57_Infos", 0)));
    // Node S60_Conv2d_192x32x1x1_Relu6 inq -25.36<(i8-0.00)*0.19811144<25.16 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S60_Conv2d_192x32x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S57_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_3693aa42", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_72b50c0a", 0), GNodeArg(GNA_OUT, "S60_Output", 0), GNodeArg(GNA_IN, "S60_Mul_scale", 0), GNodeArg(GNA_IN, "S60_Mul_shift", 0), GNodeArg(GNA_IN, "S60_Infos", 0)));
    // Node S63_Conv2d_192x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S63_Conv2d_192x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S60_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_fb92201b", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_1d0acefd", 0), GNodeArg(GNA_OUT, "S63_Output", 0), GNodeArg(GNA_IN, "S63_Mul_scale", 0), GNodeArg(GNA_IN, "S63_Mul_shift", 0), GNodeArg(GNA_IN, "S63_Infos", 0)));
    // Node S66_Conv2d_64x192x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -11.98<(i8-0.00)*0.09358190<11.88 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S66_Conv2d_64x192x1x1", Bindings(7, GNodeArg(GNA_IN, "S63_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_e9f95d46", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_cdf3d143", 0), GNodeArg(GNA_OUT, "S66_Output", 0), GNodeArg(GNA_IN, "S66_Mul_scale", 0), GNodeArg(GNA_IN, "S66_Mul_shift", 0), GNodeArg(GNA_IN, "S66_Infos", 0)));
    // Node S69_Conv2d_384x64x1x1_Relu6 inq -11.98<(i8-0.00)*0.09358190<11.88 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S69_Conv2d_384x64x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S66_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_89048f6a", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_0ed4f8ee", 0), GNodeArg(GNA_OUT, "S69_Output", 0), GNodeArg(GNA_IN, "S69_Mul_scale", 0), GNodeArg(GNA_IN, "S69_Mul_shift", 0), GNodeArg(GNA_IN, "S69_Infos", 0)));
    // Node S72_Conv2d_384x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S72_Conv2d_384x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S69_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_68d6fe31", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_020fdb92", 0), GNodeArg(GNA_OUT, "S72_Output", 0), GNodeArg(GNA_IN, "S72_Mul_scale", 0), GNodeArg(GNA_IN, "S72_Mul_shift", 0), GNodeArg(GNA_IN, "S72_Infos", 0)));
    // Node S75_Conv2d_64x384x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -16.18<(i8-0.00)*0.12640181<16.05 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S75_Conv2d_64x384x1x1", Bindings(7, GNodeArg(GNA_IN, "S72_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_8022d855", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_63ed3f08", 0), GNodeArg(GNA_OUT, "S75_Output", 0), GNodeArg(GNA_IN, "S75_Mul_scale", 0), GNodeArg(GNA_IN, "S75_Mul_shift", 0), GNodeArg(GNA_IN, "S75_Infos", 0)));
    // Node S76_MatAdd_64x15x20 in1q -16.18<(i8-0.00)*0.12640181<16.05 forced in2q -11.98<(i8-0.00)*0.09358190<11.88 forced outq -17.46<(i8-0.00)*0.13640295<17.32 forced
    AddNode("S76_MatAdd_64x15x20", Bindings(4, GNodeArg(GNA_IN, "S75_Output", 0), GNodeArg(GNA_IN, "S66_Output", 0), GNodeArg(GNA_OUT, "S76_Output", 0), GNodeArg(GNA_IN, "S76_Infos", 0)));
    // Node S79_Conv2d_384x64x1x1_Relu6 inq -17.46<(i8-0.00)*0.13640295<17.32 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S79_Conv2d_384x64x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S76_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_d7a27a34", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_6b47e064", 0), GNodeArg(GNA_OUT, "S79_Output", 0), GNodeArg(GNA_IN, "S79_Mul_scale", 0), GNodeArg(GNA_IN, "S79_Mul_shift", 0), GNodeArg(GNA_IN, "S79_Infos", 0)));
    // Node S82_Conv2d_384x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S82_Conv2d_384x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S79_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_98c48eca", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_badef1d6", 0), GNodeArg(GNA_OUT, "S82_Output", 0), GNodeArg(GNA_IN, "S82_Mul_scale", 0), GNodeArg(GNA_IN, "S82_Mul_shift", 0), GNodeArg(GNA_IN, "S82_Infos", 0)));
    // Node S85_Conv2d_64x384x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -12.08<(i8-0.00)*0.09440938<11.99 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S85_Conv2d_64x384x1x1", Bindings(7, GNodeArg(GNA_IN, "S82_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_10a44943", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_1c874c2d", 0), GNodeArg(GNA_OUT, "S85_Output", 0), GNodeArg(GNA_IN, "S85_Mul_scale", 0), GNodeArg(GNA_IN, "S85_Mul_shift", 0), GNodeArg(GNA_IN, "S85_Infos", 0)));
    // Node S86_MatAdd_64x15x20 in1q -17.46<(i8-0.00)*0.13640295<17.32 forced in2q -12.08<(i8-0.00)*0.09440938<11.99 forced outq -20.28<(i8-0.00)*0.15845883<20.12 forced
    AddNode("S86_MatAdd_64x15x20", Bindings(4, GNodeArg(GNA_IN, "S76_Output", 0), GNodeArg(GNA_IN, "S85_Output", 0), GNodeArg(GNA_OUT, "S86_Output", 0), GNodeArg(GNA_IN, "S86_Infos", 0)));
    // Node S89_Conv2d_384x64x1x1_Relu6 inq -20.28<(i8-0.00)*0.15845883<20.12 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S89_Conv2d_384x64x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S86_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_dd90b460", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_31bd8e31", 0), GNodeArg(GNA_OUT, "S89_Output", 0), GNodeArg(GNA_IN, "S89_Mul_scale", 0), GNodeArg(GNA_IN, "S89_Mul_shift", 0), GNodeArg(GNA_IN, "S89_Infos", 0)));
    // Node S92_Conv2d_384x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S92_Conv2d_384x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S89_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_98660d0b", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_81193444", 0), GNodeArg(GNA_OUT, "S92_Output", 0), GNodeArg(GNA_IN, "S92_Mul_scale", 0), GNodeArg(GNA_IN, "S92_Mul_shift", 0), GNodeArg(GNA_IN, "S92_Infos", 0)));
    // Node S95_Conv2d_64x384x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -24.69<(i8-0.00)*0.19292440<24.50 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S95_Conv2d_64x384x1x1", Bindings(7, GNodeArg(GNA_IN, "S92_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_13c8766a", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_cb123231", 0), GNodeArg(GNA_OUT, "S95_Output", 0), GNodeArg(GNA_IN, "S95_Mul_scale", 0), GNodeArg(GNA_IN, "S95_Mul_shift", 0), GNodeArg(GNA_IN, "S95_Infos", 0)));
    // Node S96_MatAdd_64x15x20 in1q -24.69<(i8-0.00)*0.19292440<24.50 forced in2q -20.28<(i8-0.00)*0.15845883<20.12 forced outq -31.94<(i8-0.00)*0.24953298<31.69 forced
    AddNode("S96_MatAdd_64x15x20", Bindings(4, GNodeArg(GNA_IN, "S95_Output", 0), GNodeArg(GNA_IN, "S86_Output", 0), GNodeArg(GNA_OUT, "S96_Output", 0), GNodeArg(GNA_IN, "S96_Infos", 0)));
    // Node S99_Conv2d_384x64x1x1_Relu6 inq -31.94<(i8-0.00)*0.24953298<31.69 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S99_Conv2d_384x64x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S96_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_bc1e6bd0", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_234f34d0", 0), GNodeArg(GNA_OUT, "S99_Output", 0), GNodeArg(GNA_IN, "S99_Mul_scale", 0), GNodeArg(GNA_IN, "S99_Mul_shift", 0), GNodeArg(GNA_IN, "S99_Infos", 0)));
    // Node S102_Conv2d_384x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S102_Conv2d_384x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S99_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_2a46f942", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_0786b546", 0), GNodeArg(GNA_OUT, "S102_Output", 0), GNodeArg(GNA_IN, "S102_Mul_scale", 0), GNodeArg(GNA_IN, "S102_Mul_shift", 0), GNodeArg(GNA_IN, "S102_Infos", 0)));
    // Node S105_Conv2d_96x384x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -17.34<(i8-0.00)*0.13546981<17.20 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S105_Conv2d_96x384x1x1", Bindings(7, GNodeArg(GNA_IN, "S102_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_eeb8ab91", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_4fdaedaf", 0), GNodeArg(GNA_OUT, "S105_Output", 0), GNodeArg(GNA_IN, "S105_Mul_scale", 0), GNodeArg(GNA_IN, "S105_Mul_shift", 0), GNodeArg(GNA_IN, "S105_Infos", 0)));
    // Node S108_Conv2d_576x96x1x1_Relu6 inq -17.34<(i8-0.00)*0.13546981<17.20 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S108_Conv2d_576x96x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S105_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_ce4b614b", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_4e7bedd6", 0), GNodeArg(GNA_OUT, "S108_Output", 0), GNodeArg(GNA_IN, "S108_Mul_scale", 0), GNodeArg(GNA_IN, "S108_Mul_shift", 0), GNodeArg(GNA_IN, "S108_Infos", 0)));
    // Node S111_Conv2d_576x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S111_Conv2d_576x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S108_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_91e62c4b", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_10c1fd87", 0), GNodeArg(GNA_OUT, "S111_Output", 0), GNodeArg(GNA_IN, "S111_Mul_scale", 0), GNodeArg(GNA_IN, "S111_Mul_shift", 0), GNodeArg(GNA_IN, "S111_Infos", 0)));
    // Node S114_Conv2d_96x576x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -16.45<(i8-0.00)*0.12853868<16.32 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S114_Conv2d_96x576x1x1", Bindings(7, GNodeArg(GNA_IN, "S111_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_52133b70", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_a5e99dd2", 0), GNodeArg(GNA_OUT, "S114_Output", 0), GNodeArg(GNA_IN, "S114_Mul_scale", 0), GNodeArg(GNA_IN, "S114_Mul_shift", 0), GNodeArg(GNA_IN, "S114_Infos", 0)));
    // Node S115_MatAdd_96x15x20 in1q -17.34<(i8-0.00)*0.13546981<17.20 forced in2q -16.45<(i8-0.00)*0.12853868<16.32 forced outq -21.93<(i8-0.00)*0.17128935<21.75 forced
    AddNode("S115_MatAdd_96x15x20", Bindings(4, GNodeArg(GNA_IN, "S105_Output", 0), GNodeArg(GNA_IN, "S114_Output", 0), GNodeArg(GNA_OUT, "S115_Output", 0), GNodeArg(GNA_IN, "S115_Infos", 0)));
    // Node S118_Conv2d_576x96x1x1_Relu6 inq -21.93<(i8-0.00)*0.17128935<21.75 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S118_Conv2d_576x96x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S115_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_412321df", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_5de61ec5", 0), GNodeArg(GNA_OUT, "S118_Output", 0), GNodeArg(GNA_IN, "S118_Mul_scale", 0), GNodeArg(GNA_IN, "S118_Mul_shift", 0), GNodeArg(GNA_IN, "S118_Infos", 0)));
    // Node S121_Conv2d_576x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S121_Conv2d_576x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S118_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_c58d241b", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_0438cc0f", 0), GNodeArg(GNA_OUT, "S121_Output", 0), GNodeArg(GNA_IN, "S121_Mul_scale", 0), GNodeArg(GNA_IN, "S121_Mul_shift", 0), GNodeArg(GNA_IN, "S121_Infos", 0)));
    // Node S124_Conv2d_96x576x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -29.39<(i8-0.00)*0.22963655<29.16 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S124_Conv2d_96x576x1x1", Bindings(7, GNodeArg(GNA_IN, "S121_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_b7b986df", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_4a14691f", 0), GNodeArg(GNA_OUT, "S124_Output", 0), GNodeArg(GNA_IN, "S124_Mul_scale", 0), GNodeArg(GNA_IN, "S124_Mul_shift", 0), GNodeArg(GNA_IN, "S124_Infos", 0)));
    // Node S125_MatAdd_96x15x20 in1q -29.39<(i8-0.00)*0.22963655<29.16 forced in2q -21.93<(i8-0.00)*0.17128935<21.75 forced outq -36.60<(i8-0.00)*0.28596756<36.32 forced
    AddNode("S125_MatAdd_96x15x20", Bindings(4, GNodeArg(GNA_IN, "S124_Output", 0), GNodeArg(GNA_IN, "S115_Output", 0), GNodeArg(GNA_OUT, "S125_Output", 0), GNodeArg(GNA_IN, "S125_Infos", 0)));
    // Node S128_Conv2d_576x96x1x1_Relu6 inq -36.60<(i8-0.00)*0.28596756<36.32 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S128_Conv2d_576x96x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S125_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_a9a81d58", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_f8dde384", 0), GNodeArg(GNA_OUT, "S128_Output", 0), GNodeArg(GNA_IN, "S128_Mul_scale", 0), GNodeArg(GNA_IN, "S128_Mul_shift", 0), GNodeArg(GNA_IN, "S128_Infos", 0)));
    // Node S131_Conv2d_12x576x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -18.15<(i8-0.00)*0.14183107<18.01 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S131_Conv2d_12x576x1x1", Bindings(7, GNodeArg(GNA_IN, "S128_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_0boxencodingpredi_3ef25574", 0), GNodeArg(GNA_IN, "Boxpredictor_0boxencodingpredi", 0), GNodeArg(GNA_OUT, "S131_Output", 0), GNodeArg(GNA_IN, "S131_Mul_scale", 0), GNodeArg(GNA_IN, "S131_Mul_shift", 0), GNodeArg(GNA_IN, "S131_Infos", 0)));
    // Node CONV_2D_0_48_trans_out0 inq -18.15<(i8-0.00)*0.14183107<18.01 outq -18.15<(i8-0.00)*0.14183107<18.01
    AddNode("S132_Op_CONV_2D_0_48_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S131_Output", 0), GNodeArg(GNA_OUT, "S132_Output", 0)));
    // Node S136_Conv2d_9x576x1x1_Hsigmoid inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -1.00<(i8-0.00)*0.00784326<1.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S136_Conv2d_9x576x1x1_Hsigmoid", Bindings(7, GNodeArg(GNA_IN, "S128_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_0classpredictorco", 0), GNodeArg(GNA_IN, "Boxpredictor_0classpredictorbi", 0), GNodeArg(GNA_OUT, "S136_Output", 0), GNodeArg(GNA_IN, "S136_Mul_scale", 0), GNodeArg(GNA_IN, "S136_Mul_shift", 0), GNodeArg(GNA_IN, "S136_Infos", 0)));
    // Node CONV_2D_0_50_trans_out0 inq -1.00<(i8-0.00)*0.00784326<1.00 outq -1.00<(i8-0.00)*0.00784326<1.00
    AddNode("S137_Op_CONV_2D_0_50_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S136_Output", 0), GNodeArg(GNA_OUT, "S137_Output", 0)));
    // Node S141_Conv2d_576x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S141_Conv2d_576x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S128_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_c51751b2", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_661144ee", 0), GNodeArg(GNA_OUT, "S141_Output", 0), GNodeArg(GNA_IN, "S141_Mul_scale", 0), GNodeArg(GNA_IN, "S141_Mul_shift", 0), GNodeArg(GNA_IN, "S141_Infos", 0)));
    // Node S144_Conv2d_160x576x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -15.39<(i8-0.00)*0.12021013<15.27 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S144_Conv2d_160x576x1x1", Bindings(7, GNodeArg(GNA_IN, "S141_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_a2274156", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_c87ef171", 0), GNodeArg(GNA_OUT, "S144_Output", 0), GNodeArg(GNA_IN, "S144_Mul_scale", 0), GNodeArg(GNA_IN, "S144_Mul_shift", 0), GNodeArg(GNA_IN, "S144_Infos", 0)));
    // Node S147_Conv2d_960x160x1x1_Relu6 inq -15.39<(i8-0.00)*0.12021013<15.27 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S147_Conv2d_960x160x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S144_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_84c563fe", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_b050988d", 0), GNodeArg(GNA_OUT, "S147_Output", 0), GNodeArg(GNA_IN, "S147_Mul_scale", 0), GNodeArg(GNA_IN, "S147_Mul_shift", 0), GNodeArg(GNA_IN, "S147_Infos", 0)));
    // Node S150_Conv2d_960x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S150_Conv2d_960x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S147_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_92a94755", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_47549dfd", 0), GNodeArg(GNA_OUT, "S150_Output", 0), GNodeArg(GNA_IN, "S150_Mul_scale", 0), GNodeArg(GNA_IN, "S150_Mul_shift", 0), GNodeArg(GNA_IN, "S150_Infos", 0)));
    // Node S153_Conv2d_160x960x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -12.60<(i8-0.00)*0.09844183<12.50 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S153_Conv2d_160x960x1x1", Bindings(7, GNodeArg(GNA_IN, "S150_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_abd28f6c", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_4ae70677", 0), GNodeArg(GNA_OUT, "S153_Output", 0), GNodeArg(GNA_IN, "S153_Mul_scale", 0), GNodeArg(GNA_IN, "S153_Mul_shift", 0), GNodeArg(GNA_IN, "S153_Infos", 0)));
    // Node S154_MatAdd_160x8x10 in1q -15.39<(i8-0.00)*0.12021013<15.27 forced in2q -12.60<(i8-0.00)*0.09844183<12.50 forced outq -20.55<(i8-0.00)*0.16054745<20.39 forced
    AddNode("S154_MatAdd_160x8x10", Bindings(4, GNodeArg(GNA_IN, "S144_Output", 0), GNodeArg(GNA_IN, "S153_Output", 0), GNodeArg(GNA_OUT, "S154_Output", 0), GNodeArg(GNA_IN, "S154_Infos", 0)));
    // Node S157_Conv2d_960x160x1x1_Relu6 inq -20.55<(i8-0.00)*0.16054745<20.39 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S157_Conv2d_960x160x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S154_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_4bfd1ce4", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_2c2e1c56", 0), GNodeArg(GNA_OUT, "S157_Output", 0), GNodeArg(GNA_IN, "S157_Mul_scale", 0), GNodeArg(GNA_IN, "S157_Mul_shift", 0), GNodeArg(GNA_IN, "S157_Infos", 0)));
    // Node S160_Conv2d_960x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S160_Conv2d_960x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S157_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_4c239961", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_d2dba10d", 0), GNodeArg(GNA_OUT, "S160_Output", 0), GNodeArg(GNA_IN, "S160_Mul_scale", 0), GNodeArg(GNA_IN, "S160_Mul_shift", 0), GNodeArg(GNA_IN, "S160_Infos", 0)));
    // Node S163_Conv2d_160x960x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -26.85<(i8-0.00)*0.20973364<26.64 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S163_Conv2d_160x960x1x1", Bindings(7, GNodeArg(GNA_IN, "S160_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_5d81017c", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_18e7e1f9", 0), GNodeArg(GNA_OUT, "S163_Output", 0), GNodeArg(GNA_IN, "S163_Mul_scale", 0), GNodeArg(GNA_IN, "S163_Mul_shift", 0), GNodeArg(GNA_IN, "S163_Infos", 0)));
    // Node S164_MatAdd_160x8x10 in1q -26.85<(i8-0.00)*0.20973364<26.64 forced in2q -20.55<(i8-0.00)*0.16054745<20.39 forced outq -39.57<(i8-0.00)*0.30915052<39.26 forced
    AddNode("S164_MatAdd_160x8x10", Bindings(4, GNodeArg(GNA_IN, "S163_Output", 0), GNodeArg(GNA_IN, "S154_Output", 0), GNodeArg(GNA_OUT, "S164_Output", 0), GNodeArg(GNA_IN, "S164_Infos", 0)));
    // Node S167_Conv2d_960x160x1x1_Relu6 inq -39.57<(i8-0.00)*0.30915052<39.26 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S167_Conv2d_960x160x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S164_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_6ce3ab45", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_f43d5057", 0), GNodeArg(GNA_OUT, "S167_Output", 0), GNodeArg(GNA_IN, "S167_Mul_scale", 0), GNodeArg(GNA_IN, "S167_Mul_shift", 0), GNodeArg(GNA_IN, "S167_Infos", 0)));
    // Node S170_Conv2d_960x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S170_Conv2d_960x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S167_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_fe48a6f4", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_5faea3cb", 0), GNodeArg(GNA_OUT, "S170_Output", 0), GNodeArg(GNA_IN, "S170_Mul_scale", 0), GNodeArg(GNA_IN, "S170_Mul_shift", 0), GNodeArg(GNA_IN, "S170_Infos", 0)));
    // Node S173_Conv2d_320x960x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -19.09<(i8-0.00)*0.14915805<18.94 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S173_Conv2d_320x960x1x1", Bindings(7, GNodeArg(GNA_IN, "S170_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_db31bcec", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp", 0), GNodeArg(GNA_OUT, "S173_Output", 0), GNodeArg(GNA_IN, "S173_Mul_scale", 0), GNodeArg(GNA_IN, "S173_Mul_shift", 0), GNodeArg(GNA_IN, "S173_Infos", 0)));
    // Node S176_Conv2d_1280x320x1x1_Relu6 inq -19.09<(i8-0.00)*0.14915805<18.94 weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S176_Conv2d_1280x320x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S173_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2con_04ed4f25", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2con", 0), GNodeArg(GNA_OUT, "S176_Output", 0), GNodeArg(GNA_IN, "S176_Mul_scale", 0), GNodeArg(GNA_IN, "S176_Mul_shift", 0), GNodeArg(GNA_IN, "S176_Infos", 0)));
    // Node S179_Conv2d_24x1280x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -18.15<(i8-0.00)*0.14183107<18.01 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S179_Conv2d_24x1280x1x1", Bindings(7, GNodeArg(GNA_IN, "S176_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_1boxencodingpredi_185fd89e", 0), GNodeArg(GNA_IN, "Boxpredictor_1boxencodingpredi", 0), GNodeArg(GNA_OUT, "S179_Output", 0), GNodeArg(GNA_IN, "S179_Mul_scale", 0), GNodeArg(GNA_IN, "S179_Mul_shift", 0), GNodeArg(GNA_IN, "S179_Infos", 0)));
    // Node CONV_2D_0_66_trans_out0 inq -18.15<(i8-0.00)*0.14183107<18.01 outq -18.15<(i8-0.00)*0.14183107<18.01
    AddNode("S180_Op_CONV_2D_0_66_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S179_Output", 0), GNodeArg(GNA_OUT, "S180_Output", 0)));
    // Node S184_Conv2d_18x1280x1x1_Hsigmoid inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -1.00<(i8-0.00)*0.00784326<1.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S184_Conv2d_18x1280x1x1_Hsigmoid", Bindings(7, GNodeArg(GNA_IN, "S176_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_1classpredictorco", 0), GNodeArg(GNA_IN, "Boxpredictor_1classpredictorbi", 0), GNodeArg(GNA_OUT, "S184_Output", 0), GNodeArg(GNA_IN, "S184_Mul_scale", 0), GNodeArg(GNA_IN, "S184_Mul_shift", 0), GNodeArg(GNA_IN, "S184_Infos", 0)));
    // Node CONV_2D_0_69_trans_out0 inq -1.00<(i8-0.00)*0.00784326<1.00 outq -1.00<(i8-0.00)*0.00784326<1.00
    AddNode("S185_Op_CONV_2D_0_69_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S184_Output", 0), GNodeArg(GNA_OUT, "S185_Output", 0)));
    // Node S189_Conv2d_256x1280x1x1_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S189_Conv2d_256x1280x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S176_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_7d739858", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_7b3686d5", 0), GNodeArg(GNA_OUT, "S189_Output", 0), GNodeArg(GNA_IN, "S189_Mul_scale", 0), GNodeArg(GNA_IN, "S189_Mul_shift", 0), GNodeArg(GNA_IN, "S189_Infos", 0)));
    // Node S192_Conv2d_512x256x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S192_Conv2d_512x256x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S189_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_93e7cbd6", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_0fde7b1a", 0), GNodeArg(GNA_OUT, "S192_Output", 0), GNodeArg(GNA_IN, "S192_Mul_scale", 0), GNodeArg(GNA_IN, "S192_Mul_shift", 0), GNodeArg(GNA_IN, "S192_Infos", 0)));
    // Node S195_Conv2d_24x512x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -18.15<(i8-0.00)*0.14183107<18.01 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S195_Conv2d_24x512x1x1", Bindings(7, GNodeArg(GNA_IN, "S192_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_2boxencodingpredi_bde07c43", 0), GNodeArg(GNA_IN, "Boxpredictor_2boxencodingpredi", 0), GNodeArg(GNA_OUT, "S195_Output", 0), GNodeArg(GNA_IN, "S195_Mul_scale", 0), GNodeArg(GNA_IN, "S195_Mul_shift", 0), GNodeArg(GNA_IN, "S195_Infos", 0)));
    // Node CONV_2D_0_74_trans_out0 inq -18.15<(i8-0.00)*0.14183107<18.01 outq -18.15<(i8-0.00)*0.14183107<18.01
    AddNode("S196_Op_CONV_2D_0_74_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S195_Output", 0), GNodeArg(GNA_OUT, "S196_Output", 0)));
    // Node S200_Conv2d_18x512x1x1_Hsigmoid inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -1.00<(i8-0.00)*0.00784326<1.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S200_Conv2d_18x512x1x1_Hsigmoid", Bindings(7, GNodeArg(GNA_IN, "S192_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_2classpredictorco", 0), GNodeArg(GNA_IN, "Boxpredictor_2classpredictorbi", 0), GNodeArg(GNA_OUT, "S200_Output", 0), GNodeArg(GNA_IN, "S200_Mul_scale", 0), GNodeArg(GNA_IN, "S200_Mul_shift", 0), GNodeArg(GNA_IN, "S200_Infos", 0)));
    // Node CONV_2D_0_77_trans_out0 inq -1.00<(i8-0.00)*0.00784326<1.00 outq -1.00<(i8-0.00)*0.00784326<1.00
    AddNode("S201_Op_CONV_2D_0_77_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S200_Output", 0), GNodeArg(GNA_OUT, "S201_Output", 0)));
    // Node S205_Conv2d_128x512x1x1_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S205_Conv2d_128x512x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S192_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_77ac8956", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_bbbdea67", 0), GNodeArg(GNA_OUT, "S205_Output", 0), GNodeArg(GNA_IN, "S205_Mul_scale", 0), GNodeArg(GNA_IN, "S205_Mul_shift", 0), GNodeArg(GNA_IN, "S205_Infos", 0)));
    // Node S208_Conv2d_256x128x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S208_Conv2d_256x128x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S205_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_4f2e096c", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_20136f8b", 0), GNodeArg(GNA_OUT, "S208_Output", 0), GNodeArg(GNA_IN, "S208_Mul_scale", 0), GNodeArg(GNA_IN, "S208_Mul_shift", 0), GNodeArg(GNA_IN, "S208_Infos", 0)));
    // Node S211_Conv2d_24x256x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -18.15<(i8-0.00)*0.14183107<18.01 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S211_Conv2d_24x256x1x1", Bindings(7, GNodeArg(GNA_IN, "S208_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_3boxencodingpredi_34c879aa", 0), GNodeArg(GNA_IN, "Boxpredictor_3boxencodingpredi", 0), GNodeArg(GNA_OUT, "S211_Output", 0), GNodeArg(GNA_IN, "S211_Mul_scale", 0), GNodeArg(GNA_IN, "S211_Mul_shift", 0), GNodeArg(GNA_IN, "S211_Infos", 0)));
    // Node CONV_2D_0_82_trans_out0 inq -18.15<(i8-0.00)*0.14183107<18.01 outq -18.15<(i8-0.00)*0.14183107<18.01
    AddNode("S212_Op_CONV_2D_0_82_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S211_Output", 0), GNodeArg(GNA_OUT, "S212_Output", 0)));
    // Node S216_Conv2d_18x256x1x1_Hsigmoid inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -1.00<(i8-0.00)*0.00784326<1.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S216_Conv2d_18x256x1x1_Hsigmoid", Bindings(7, GNodeArg(GNA_IN, "S208_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_3classpredictorco", 0), GNodeArg(GNA_IN, "Boxpredictor_3classpredictorbi", 0), GNodeArg(GNA_OUT, "S216_Output", 0), GNodeArg(GNA_IN, "S216_Mul_scale", 0), GNodeArg(GNA_IN, "S216_Mul_shift", 0), GNodeArg(GNA_IN, "S216_Infos", 0)));
    // Node CONV_2D_0_85_trans_out0 inq -1.00<(i8-0.00)*0.00784326<1.00 outq -1.00<(i8-0.00)*0.00784326<1.00
    AddNode("S217_Op_CONV_2D_0_85_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S216_Output", 0), GNodeArg(GNA_OUT, "S217_Output", 0)));
    // Node S221_Conv2d_128x256x1x1_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S221_Conv2d_128x256x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S208_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_c9953090", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_8b904905", 0), GNodeArg(GNA_OUT, "S221_Output", 0), GNodeArg(GNA_IN, "S221_Mul_scale", 0), GNodeArg(GNA_IN, "S221_Mul_shift", 0), GNodeArg(GNA_IN, "S221_Infos", 0)));
    // Node S224_Conv2d_256x128x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S224_Conv2d_256x128x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S221_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_1b54ffcb", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_df75bdd5", 0), GNodeArg(GNA_OUT, "S224_Output", 0), GNodeArg(GNA_IN, "S224_Mul_scale", 0), GNodeArg(GNA_IN, "S224_Mul_shift", 0), GNodeArg(GNA_IN, "S224_Infos", 0)));
    // Node S227_Conv2d_24x256x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -18.15<(i8-0.00)*0.14183107<18.01 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S227_Conv2d_24x256x1x1", Bindings(7, GNodeArg(GNA_IN, "S224_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_4boxencodingpredi_8df609c1", 0), GNodeArg(GNA_IN, "Boxpredictor_4boxencodingpredi", 0), GNodeArg(GNA_OUT, "S227_Output", 0), GNodeArg(GNA_IN, "S227_Mul_scale", 0), GNodeArg(GNA_IN, "S227_Mul_shift", 0), GNodeArg(GNA_IN, "S227_Infos", 0)));
    // Node CONV_2D_0_90_trans_out0 inq -18.15<(i8-0.00)*0.14183107<18.01 outq -18.15<(i8-0.00)*0.14183107<18.01
    AddNode("S228_Op_CONV_2D_0_90_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S227_Output", 0), GNodeArg(GNA_OUT, "S228_Output", 0)));
    // Node S232_Conv2d_18x256x1x1_Hsigmoid inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -1.00<(i8-0.00)*0.00784326<1.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S232_Conv2d_18x256x1x1_Hsigmoid", Bindings(7, GNodeArg(GNA_IN, "S224_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_4classpredictorco", 0), GNodeArg(GNA_IN, "Boxpredictor_4classpredictorbi", 0), GNodeArg(GNA_OUT, "S232_Output", 0), GNodeArg(GNA_IN, "S232_Mul_scale", 0), GNodeArg(GNA_IN, "S232_Mul_shift", 0), GNodeArg(GNA_IN, "S232_Infos", 0)));
    // Node CONV_2D_0_93_trans_out0 inq -1.00<(i8-0.00)*0.00784326<1.00 outq -1.00<(i8-0.00)*0.00784326<1.00
    AddNode("S233_Op_CONV_2D_0_93_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S232_Output", 0), GNodeArg(GNA_OUT, "S233_Output", 0)));
    // Node S237_Conv2d_64x256x1x1_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S237_Conv2d_64x256x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S224_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_9f8f14fa", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_f48e9fc3", 0), GNodeArg(GNA_OUT, "S237_Output", 0), GNodeArg(GNA_IN, "S237_Mul_scale", 0), GNodeArg(GNA_IN, "S237_Mul_shift", 0), GNodeArg(GNA_IN, "S237_Infos", 0)));
    // Node S240_Conv2d_128x64x1x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S240_Conv2d_128x64x1x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S237_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_4c0e1d34", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay", 0), GNodeArg(GNA_OUT, "S240_Output", 0), GNodeArg(GNA_IN, "S240_Mul_scale", 0), GNodeArg(GNA_IN, "S240_Mul_shift", 0), GNodeArg(GNA_IN, "S240_Infos", 0)));
    // Node S243_Conv2d_24x128x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -18.15<(i8-0.00)*0.14183107<18.01 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S243_Conv2d_24x128x1x1", Bindings(7, GNodeArg(GNA_IN, "S240_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_5boxencodingpredi_f05378fe", 0), GNodeArg(GNA_IN, "Boxpredictor_5boxencodingpredi", 0), GNodeArg(GNA_OUT, "S243_Output", 0), GNodeArg(GNA_IN, "S243_Mul_scale", 0), GNodeArg(GNA_IN, "S243_Mul_shift", 0), GNodeArg(GNA_IN, "S243_Infos", 0)));
    // Node S249_Conv2d_18x128x1x1_Hsigmoid inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -1.00<(i8-0.00)*0.00784326<1.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S249_Conv2d_18x128x1x1_Hsigmoid", Bindings(7, GNodeArg(GNA_IN, "S240_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_5classpredictorco", 0), GNodeArg(GNA_IN, "Boxpredictor_5classpredictorbi", 0), GNodeArg(GNA_OUT, "S249_Output", 0), GNodeArg(GNA_IN, "S249_Mul_scale", 0), GNodeArg(GNA_IN, "S249_Mul_shift", 0), GNodeArg(GNA_IN, "S249_Infos", 0)));
    // Node CUSTOM_0_110 offsetsq -18.15<(i8-0.00)*0.14183107<18.01 scoresq -1.00<(i8-0.00)*0.00784326<1.00 anchorsq -1.66<(i8-0.00)*0.01295693<1.65 forced outboxesq -2.00<(i16-0.00)*0.00006104<2.00
    AddNode("S253_Op_CUSTOM_0_110", Bindings(8, GNodeArg(GNA_IN, "S245_Output", 0), GNodeArg(GNA_IN, "S251_Output", 0), GNodeArg(GNA_IN, "Anchors", 0), GNodeArg(GNA_OUT, "Output_1", 0), GNodeArg(GNA_OUT, "Output_2", 0), GNodeArg(GNA_OUT, "Output_3", 0), GNodeArg(GNA_IN, "S253_Ssd_scales", 0), GNodeArg(GNA_IN, "S253_Ssd_norms", 0)));
    CloseGraph();
#endif
}

int main(int argc, char **argv)

{
    if (TilerParseOptions(argc, argv)) {
            printf("Failed to initialize or incorrect output arguments directory.\n"); return 1;
    }
    SSD_tin_can_bottleModel(64000, 300000, 8000000, 20*1024*1024);
    GenerateTilingCode();
    return 0;
}
