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
    CNN_GenControl_T gen_ctrl_S3_Conv2d_16x3x3x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S3_Conv2d_16x3x3x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S3_Conv2d_16x3x3x3_Relu6, "PADTYPE", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S3_Conv2d_16x3x3x3_Relu6", &gen_ctrl_S3_Conv2d_16x3x3x3_Relu6, 4, 1, 3, 16, 320, 240,
        KOP_CONV, 3, 3, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_1_fusion
    CNN_ConvolutionPoolAct_SQ8("S6_Conv2d_16x1x3x3_Relu6", 0, 4, 1, 16, 16, 160, 120,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_2
    CNN_GenControl_T gen_ctrl_S9_Conv2d_16x16x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S9_Conv2d_16x16x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S9_Conv2d_16x16x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S9_Conv2d_16x16x1x1", &gen_ctrl_S9_Conv2d_16x16x1x1, 4, 1, 16, 16, 160, 120,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_3
    CNN_MatAddAct_SQ8("S10_MatAdd_16x120x160", 0, 16, 120, 160, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_4_fusion
    CNN_GenControl_T gen_ctrl_S13_Conv2d_96x16x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S13_Conv2d_96x16x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S13_Conv2d_96x16x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S13_Conv2d_96x16x1x1_Relu6", &gen_ctrl_S13_Conv2d_96x16x1x1_Relu6, 4, 1, 16, 96, 160, 120,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_5_fusion
    CNN_GenControl_T gen_ctrl_S16_Conv2d_96x1x3x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S16_Conv2d_96x1x3x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S16_Conv2d_96x1x3x3_Relu6, "PADTYPE", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S16_Conv2d_96x1x3x3_Relu6", &gen_ctrl_S16_Conv2d_96x1x3x3_Relu6, 4, 1, 96, 96, 160, 120,
        KOP_CONV_DW, 3, 3, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_6
    CNN_GenControl_T gen_ctrl_S19_Conv2d_16x96x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S19_Conv2d_16x96x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S19_Conv2d_16x96x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S19_Conv2d_16x96x1x1", &gen_ctrl_S19_Conv2d_16x96x1x1, 4, 1, 96, 16, 80, 60,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_7_fusion
    CNN_GenControl_T gen_ctrl_S22_Conv2d_96x16x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S22_Conv2d_96x16x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S22_Conv2d_96x16x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S22_Conv2d_96x16x1x1_Relu6", &gen_ctrl_S22_Conv2d_96x16x1x1_Relu6, 4, 1, 16, 96, 80, 60,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_8_fusion
    CNN_ConvolutionPoolAct_SQ8("S25_Conv2d_96x1x3x3_Relu6", 0, 4, 1, 96, 96, 80, 60,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_9
    CNN_GenControl_T gen_ctrl_S28_Conv2d_16x96x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S28_Conv2d_16x96x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S28_Conv2d_16x96x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S28_Conv2d_16x96x1x1", &gen_ctrl_S28_Conv2d_16x96x1x1, 4, 1, 96, 16, 80, 60,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_10
    CNN_MatAddAct_SQ8("S29_MatAdd_16x60x80", 0, 16, 60, 80, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_11_fusion
    CNN_GenControl_T gen_ctrl_S32_Conv2d_96x16x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S32_Conv2d_96x16x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S32_Conv2d_96x16x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S32_Conv2d_96x16x1x1_Relu6", &gen_ctrl_S32_Conv2d_96x16x1x1_Relu6, 4, 1, 16, 96, 80, 60,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_12_fusion
    CNN_GenControl_T gen_ctrl_S35_Conv2d_96x1x3x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S35_Conv2d_96x1x3x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S35_Conv2d_96x1x3x3_Relu6, "PADTYPE", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S35_Conv2d_96x1x3x3_Relu6", &gen_ctrl_S35_Conv2d_96x1x3x3_Relu6, 4, 1, 96, 96, 80, 60,
        KOP_CONV_DW, 3, 3, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_13
    CNN_GenControl_T gen_ctrl_S38_Conv2d_16x96x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S38_Conv2d_16x96x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S38_Conv2d_16x96x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S38_Conv2d_16x96x1x1", &gen_ctrl_S38_Conv2d_16x96x1x1, 4, 1, 96, 16, 40, 30,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_14_fusion
    CNN_GenControl_T gen_ctrl_S41_Conv2d_96x16x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S41_Conv2d_96x16x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S41_Conv2d_96x16x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S41_Conv2d_96x16x1x1_Relu6", &gen_ctrl_S41_Conv2d_96x16x1x1_Relu6, 4, 1, 16, 96, 40, 30,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_15_fusion
    CNN_ConvolutionPoolAct_SQ8("S44_Conv2d_96x1x3x3_Relu6", 0, 4, 1, 96, 96, 40, 30,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_16
    CNN_GenControl_T gen_ctrl_S47_Conv2d_16x96x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S47_Conv2d_16x96x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S47_Conv2d_16x96x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S47_Conv2d_16x96x1x1", &gen_ctrl_S47_Conv2d_16x96x1x1, 4, 1, 96, 16, 40, 30,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_17
    CNN_MatAddAct_SQ8("S48_MatAdd_16x30x40", 0, 16, 30, 40, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_18_fusion
    CNN_GenControl_T gen_ctrl_S51_Conv2d_96x16x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S51_Conv2d_96x16x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S51_Conv2d_96x16x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S51_Conv2d_96x16x1x1_Relu6", &gen_ctrl_S51_Conv2d_96x16x1x1_Relu6, 4, 1, 16, 96, 40, 30,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_19_fusion
    CNN_ConvolutionPoolAct_SQ8("S54_Conv2d_96x1x3x3_Relu6", 0, 4, 1, 96, 96, 40, 30,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_20
    CNN_GenControl_T gen_ctrl_S57_Conv2d_16x96x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S57_Conv2d_16x96x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S57_Conv2d_16x96x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S57_Conv2d_16x96x1x1", &gen_ctrl_S57_Conv2d_16x96x1x1, 4, 1, 96, 16, 40, 30,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_21
    CNN_MatAddAct_SQ8("S58_MatAdd_16x30x40", 0, 16, 30, 40, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_22_fusion
    CNN_GenControl_T gen_ctrl_S61_Conv2d_96x16x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S61_Conv2d_96x16x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S61_Conv2d_96x16x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S61_Conv2d_96x16x1x1_Relu6", &gen_ctrl_S61_Conv2d_96x16x1x1_Relu6, 4, 1, 16, 96, 40, 30,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_23_fusion
    CNN_GenControl_T gen_ctrl_S64_Conv2d_96x1x3x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S64_Conv2d_96x1x3x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S64_Conv2d_96x1x3x3_Relu6, "PADTYPE", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S64_Conv2d_96x1x3x3_Relu6", &gen_ctrl_S64_Conv2d_96x1x3x3_Relu6, 4, 1, 96, 96, 40, 30,
        KOP_CONV_DW, 3, 3, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_24
    CNN_GenControl_T gen_ctrl_S67_Conv2d_32x96x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S67_Conv2d_32x96x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S67_Conv2d_32x96x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S67_Conv2d_32x96x1x1", &gen_ctrl_S67_Conv2d_32x96x1x1, 4, 1, 96, 32, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_25_fusion
    CNN_GenControl_T gen_ctrl_S70_Conv2d_192x32x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S70_Conv2d_192x32x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S70_Conv2d_192x32x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S70_Conv2d_192x32x1x1_Relu6", &gen_ctrl_S70_Conv2d_192x32x1x1_Relu6, 4, 1, 32, 192, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_26_fusion
    CNN_ConvolutionPoolAct_SQ8("S73_Conv2d_192x1x3x3_Relu6", 0, 4, 1, 192, 192, 20, 15,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_27
    CNN_GenControl_T gen_ctrl_S76_Conv2d_32x192x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S76_Conv2d_32x192x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S76_Conv2d_32x192x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S76_Conv2d_32x192x1x1", &gen_ctrl_S76_Conv2d_32x192x1x1, 4, 1, 192, 32, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_28
    CNN_MatAddAct_SQ8("S77_MatAdd_32x15x20", 0, 32, 15, 20, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_29_fusion
    CNN_GenControl_T gen_ctrl_S80_Conv2d_192x32x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S80_Conv2d_192x32x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S80_Conv2d_192x32x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S80_Conv2d_192x32x1x1_Relu6", &gen_ctrl_S80_Conv2d_192x32x1x1_Relu6, 4, 1, 32, 192, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_30_fusion
    CNN_ConvolutionPoolAct_SQ8("S83_Conv2d_192x1x3x3_Relu6", 0, 4, 1, 192, 192, 20, 15,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_31
    CNN_GenControl_T gen_ctrl_S86_Conv2d_32x192x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S86_Conv2d_32x192x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S86_Conv2d_32x192x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S86_Conv2d_32x192x1x1", &gen_ctrl_S86_Conv2d_32x192x1x1, 4, 1, 192, 32, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_32
    CNN_MatAddAct_SQ8("S87_MatAdd_32x15x20", 0, 32, 15, 20, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_33_fusion
    CNN_GenControl_T gen_ctrl_S90_Conv2d_192x32x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S90_Conv2d_192x32x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S90_Conv2d_192x32x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S90_Conv2d_192x32x1x1_Relu6", &gen_ctrl_S90_Conv2d_192x32x1x1_Relu6, 4, 1, 32, 192, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_34_fusion
    CNN_ConvolutionPoolAct_SQ8("S93_Conv2d_192x1x3x3_Relu6", 0, 4, 1, 192, 192, 20, 15,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_35
    CNN_GenControl_T gen_ctrl_S96_Conv2d_32x192x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S96_Conv2d_32x192x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S96_Conv2d_32x192x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S96_Conv2d_32x192x1x1", &gen_ctrl_S96_Conv2d_32x192x1x1, 4, 1, 192, 32, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_36
    CNN_MatAddAct_SQ8("S97_MatAdd_32x15x20", 0, 32, 15, 20, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_37_fusion
    CNN_GenControl_T gen_ctrl_S100_Conv2d_192x32x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S100_Conv2d_192x32x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S100_Conv2d_192x32x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S100_Conv2d_192x32x1x1_Relu6", &gen_ctrl_S100_Conv2d_192x32x1x1_Relu6, 4, 1, 32, 192, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_38_fusion
    CNN_ConvolutionPoolAct_SQ8("S103_Conv2d_192x1x3x3_Relu6", 0, 4, 1, 192, 192, 20, 15,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_39
    CNN_GenControl_T gen_ctrl_S106_Conv2d_48x192x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S106_Conv2d_48x192x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S106_Conv2d_48x192x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S106_Conv2d_48x192x1x1", &gen_ctrl_S106_Conv2d_48x192x1x1, 4, 1, 192, 48, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_40_fusion
    CNN_GenControl_T gen_ctrl_S109_Conv2d_288x48x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S109_Conv2d_288x48x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S109_Conv2d_288x48x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S109_Conv2d_288x48x1x1_Relu6", &gen_ctrl_S109_Conv2d_288x48x1x1_Relu6, 4, 1, 48, 288, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_41_fusion
    CNN_ConvolutionPoolAct_SQ8("S112_Conv2d_288x1x3x3_Relu6", 0, 4, 1, 288, 288, 20, 15,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_42
    CNN_GenControl_T gen_ctrl_S115_Conv2d_48x288x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S115_Conv2d_48x288x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S115_Conv2d_48x288x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S115_Conv2d_48x288x1x1", &gen_ctrl_S115_Conv2d_48x288x1x1, 4, 1, 288, 48, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_43
    CNN_MatAddAct_SQ8("S116_MatAdd_48x15x20", 0, 48, 15, 20, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_44_fusion
    CNN_GenControl_T gen_ctrl_S119_Conv2d_288x48x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S119_Conv2d_288x48x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S119_Conv2d_288x48x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S119_Conv2d_288x48x1x1_Relu6", &gen_ctrl_S119_Conv2d_288x48x1x1_Relu6, 4, 1, 48, 288, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_45_fusion
    CNN_ConvolutionPoolAct_SQ8("S122_Conv2d_288x1x3x3_Relu6", 0, 4, 1, 288, 288, 20, 15,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_46
    CNN_GenControl_T gen_ctrl_S125_Conv2d_48x288x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S125_Conv2d_48x288x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S125_Conv2d_48x288x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S125_Conv2d_48x288x1x1", &gen_ctrl_S125_Conv2d_48x288x1x1, 4, 1, 288, 48, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_47
    CNN_MatAddAct_SQ8("S126_MatAdd_48x15x20", 0, 48, 15, 20, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_48_fusion
    CNN_GenControl_T gen_ctrl_S129_Conv2d_288x48x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S129_Conv2d_288x48x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S129_Conv2d_288x48x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S129_Conv2d_288x48x1x1_Relu6", &gen_ctrl_S129_Conv2d_288x48x1x1_Relu6, 4, 1, 48, 288, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_49_fusion
    CNN_GenControl_T gen_ctrl_S132_Conv2d_288x1x3x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S132_Conv2d_288x1x3x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S132_Conv2d_288x1x3x3_Relu6, "PADTYPE", AT_OPT_VAL(3));
    CNN_ConvolutionPoolAct_SQ8("S132_Conv2d_288x1x3x3_Relu6", &gen_ctrl_S132_Conv2d_288x1x3x3_Relu6, 4, 1, 288, 288, 20, 15,
        KOP_CONV_DW, 3, 3, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_50
    CNN_GenControl_T gen_ctrl_S135_Conv2d_80x288x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S135_Conv2d_80x288x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S135_Conv2d_80x288x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S135_Conv2d_80x288x1x1", &gen_ctrl_S135_Conv2d_80x288x1x1, 4, 1, 288, 80, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_51_fusion
    CNN_GenControl_T gen_ctrl_S138_Conv2d_480x80x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S138_Conv2d_480x80x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S138_Conv2d_480x80x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S138_Conv2d_480x80x1x1_Relu6", &gen_ctrl_S138_Conv2d_480x80x1x1_Relu6, 4, 1, 80, 480, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_52_fusion
    CNN_ConvolutionPoolAct_SQ8("S141_Conv2d_480x1x3x3_Relu6", 0, 4, 1, 480, 480, 10, 8,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_53
    CNN_GenControl_T gen_ctrl_S144_Conv2d_80x480x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S144_Conv2d_80x480x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S144_Conv2d_80x480x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S144_Conv2d_80x480x1x1", &gen_ctrl_S144_Conv2d_80x480x1x1, 4, 1, 480, 80, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_54
    CNN_MatAddAct_SQ8("S145_MatAdd_80x8x10", 0, 80, 8, 10, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_55_fusion
    CNN_GenControl_T gen_ctrl_S148_Conv2d_480x80x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S148_Conv2d_480x80x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S148_Conv2d_480x80x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S148_Conv2d_480x80x1x1_Relu6", &gen_ctrl_S148_Conv2d_480x80x1x1_Relu6, 4, 1, 80, 480, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_56_fusion
    CNN_ConvolutionPoolAct_SQ8("S151_Conv2d_480x1x3x3_Relu6", 0, 4, 1, 480, 480, 10, 8,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_57
    CNN_GenControl_T gen_ctrl_S154_Conv2d_80x480x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S154_Conv2d_80x480x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S154_Conv2d_80x480x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S154_Conv2d_80x480x1x1", &gen_ctrl_S154_Conv2d_80x480x1x1, 4, 1, 480, 80, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    
    // generator for ADD_0_58
    CNN_MatAddAct_SQ8("S155_MatAdd_80x8x10", 0, 80, 8, 10, KOP_MATADD, KOP_NONE);
    
    // generator for CONV_2D_0_59_fusion
    CNN_GenControl_T gen_ctrl_S158_Conv2d_480x80x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S158_Conv2d_480x80x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S158_Conv2d_480x80x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S158_Conv2d_480x80x1x1_Relu6", &gen_ctrl_S158_Conv2d_480x80x1x1_Relu6, 4, 1, 80, 480, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for DEPTHWISE_CONV_2D_0_60_fusion
    CNN_ConvolutionPoolAct_SQ8("S161_Conv2d_480x1x3x3_Relu6", 0, 4, 1, 480, 480, 10, 8,
        KOP_CONV_DW, 3, 3, 1, 1, 1, 1, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_61
    CNN_GenControl_T gen_ctrl_S164_Conv2d_160x480x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S164_Conv2d_160x480x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S164_Conv2d_160x480x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S164_Conv2d_160x480x1x1", &gen_ctrl_S164_Conv2d_160x480x1x1, 4, 1, 480, 160, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_62_fusion
    CNN_GenControl_T gen_ctrl_S167_Conv2d_640x160x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S167_Conv2d_640x160x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S167_Conv2d_640x160x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S167_Conv2d_640x160x1x1_Relu6", &gen_ctrl_S167_Conv2d_640x160x1x1_Relu6, 4, 1, 160, 640, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_63_fusion
    CNN_GenControl_T gen_ctrl_S170_Conv2d_128x640x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S170_Conv2d_128x640x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S170_Conv2d_128x640x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S170_Conv2d_128x640x1x1_Relu6", &gen_ctrl_S170_Conv2d_128x640x1x1_Relu6, 4, 1, 640, 128, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_64_fusion
    CNN_GenControl_T gen_ctrl_S173_Conv2d_256x128x3x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S173_Conv2d_256x128x3x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S173_Conv2d_256x128x3x3_Relu6, "PADTYPE", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S173_Conv2d_256x128x3x3_Relu6", &gen_ctrl_S173_Conv2d_256x128x3x3_Relu6, 4, 1, 128, 256, 10, 8,
        KOP_CONV, 3, 3, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_65_fusion
    CNN_GenControl_T gen_ctrl_S176_Conv2d_64x256x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S176_Conv2d_64x256x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S176_Conv2d_64x256x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S176_Conv2d_64x256x1x1_Relu6", &gen_ctrl_S176_Conv2d_64x256x1x1_Relu6, 4, 1, 256, 64, 5, 4,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_66_fusion
    CNN_GenControl_T gen_ctrl_S179_Conv2d_128x64x3x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S179_Conv2d_128x64x3x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S179_Conv2d_128x64x3x3_Relu6, "PADTYPE", AT_OPT_VAL(3));
    CNN_ConvolutionPoolAct_SQ8("S179_Conv2d_128x64x3x3_Relu6", &gen_ctrl_S179_Conv2d_128x64x3x3_Relu6, 4, 1, 64, 128, 5, 4,
        KOP_CONV, 3, 3, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_67_fusion
    CNN_GenControl_T gen_ctrl_S182_Conv2d_64x128x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S182_Conv2d_64x128x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S182_Conv2d_64x128x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S182_Conv2d_64x128x1x1_Relu6", &gen_ctrl_S182_Conv2d_64x128x1x1_Relu6, 4, 1, 128, 64, 3, 2,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_68_fusion
    CNN_GenControl_T gen_ctrl_S185_Conv2d_128x64x3x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S185_Conv2d_128x64x3x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S185_Conv2d_128x64x3x3_Relu6, "PADTYPE", AT_OPT_VAL(3));
    CNN_ConvolutionPoolAct_SQ8("S185_Conv2d_128x64x3x3_Relu6", &gen_ctrl_S185_Conv2d_128x64x3x3_Relu6, 4, 1, 64, 128, 3, 2,
        KOP_CONV, 3, 3, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_69_fusion
    CNN_GenControl_T gen_ctrl_S188_Conv2d_32x128x1x1_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S188_Conv2d_32x128x1x1_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S188_Conv2d_32x128x1x1_Relu6, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S188_Conv2d_32x128x1x1_Relu6", &gen_ctrl_S188_Conv2d_32x128x1x1_Relu6, 4, 1, 128, 32, 2, 1,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_70_fusion
    CNN_GenControl_T gen_ctrl_S191_Conv2d_64x32x1x3_Relu6;
    CNN_InitGenCtrl(&gen_ctrl_S191_Conv2d_64x32x1x3_Relu6);
    CNN_SetGenCtrl(&gen_ctrl_S191_Conv2d_64x32x1x3_Relu6, "PADTYPE", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S191_Conv2d_64x32x1x3_Relu6", &gen_ctrl_S191_Conv2d_64x32x1x3_Relu6, 4, 1, 32, 64, 2, 1,
        KOP_CONV, 3, 1, 1, 1, 2, 2, 1,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_RELU);
    // generator for CONV_2D_0_71
    CNN_GenControl_T gen_ctrl_S194_Conv2d_12x288x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S194_Conv2d_12x288x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S194_Conv2d_12x288x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S194_Conv2d_12x288x1x1", &gen_ctrl_S194_Conv2d_12x288x1x1, 4, 1, 288, 12, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_71_trans_out0
    // transpose from 12x15x20 to 15x20x12 ((1, 2, 0))
    CNN_3DTensorPermute("S195_Op_CONV_2D_0_71_trans_out0", 0, 1, 12, 20, 15, KOP_MATPERM_CHW2HWC);
    // generator for CONV_2D_0_73_fusion
    CNN_GenControl_T gen_ctrl_S199_Conv2d_9x288x1x1_Hsigmoid;
    CNN_InitGenCtrl(&gen_ctrl_S199_Conv2d_9x288x1x1_Hsigmoid);
    CNN_SetGenCtrl(&gen_ctrl_S199_Conv2d_9x288x1x1_Hsigmoid, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S199_Conv2d_9x288x1x1_Hsigmoid", &gen_ctrl_S199_Conv2d_9x288x1x1_Hsigmoid, 4, 1, 288, 9, 20, 15,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_HSIGMOID);
    // generator for CONV_2D_0_73_trans_out0
    // transpose from 9x15x20 to 15x20x9 ((1, 2, 0))
    CNN_3DTensorPermute("S200_Op_CONV_2D_0_73_trans_out0", 0, 1, 9, 20, 15, KOP_MATPERM_CHW2HWC);
    // generator for CONV_2D_0_75
    CNN_GenControl_T gen_ctrl_S204_Conv2d_24x640x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S204_Conv2d_24x640x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S204_Conv2d_24x640x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S204_Conv2d_24x640x1x1", &gen_ctrl_S204_Conv2d_24x640x1x1, 4, 1, 640, 24, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_75_trans_out0
    // transpose from 24x8x10 to 8x10x24 ((1, 2, 0))
    CNN_3DTensorPermute("S205_Op_CONV_2D_0_75_trans_out0", 0, 1, 24, 10, 8, KOP_MATPERM_CHW2HWC);
    // generator for CONV_2D_0_77_fusion
    CNN_GenControl_T gen_ctrl_S209_Conv2d_18x640x1x1_Hsigmoid;
    CNN_InitGenCtrl(&gen_ctrl_S209_Conv2d_18x640x1x1_Hsigmoid);
    CNN_SetGenCtrl(&gen_ctrl_S209_Conv2d_18x640x1x1_Hsigmoid, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S209_Conv2d_18x640x1x1_Hsigmoid", &gen_ctrl_S209_Conv2d_18x640x1x1_Hsigmoid, 4, 1, 640, 18, 10, 8,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_HSIGMOID);
    // generator for CONV_2D_0_77_trans_out0
    // transpose from 18x8x10 to 8x10x18 ((1, 2, 0))
    CNN_3DTensorPermute("S210_Op_CONV_2D_0_77_trans_out0", 0, 1, 18, 10, 8, KOP_MATPERM_CHW2HWC);
    // generator for CONV_2D_0_79
    CNN_GenControl_T gen_ctrl_S214_Conv2d_24x256x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S214_Conv2d_24x256x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S214_Conv2d_24x256x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S214_Conv2d_24x256x1x1", &gen_ctrl_S214_Conv2d_24x256x1x1, 4, 1, 256, 24, 5, 4,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_79_trans_out0
    // transpose from 24x4x5 to 4x5x24 ((1, 2, 0))
    CNN_3DTensorPermute("S215_Op_CONV_2D_0_79_trans_out0", 0, 1, 24, 5, 4, KOP_MATPERM_CHW2HWC);
    // generator for CONV_2D_0_81_fusion
    CNN_GenControl_T gen_ctrl_S219_Conv2d_18x256x1x1_Hsigmoid;
    CNN_InitGenCtrl(&gen_ctrl_S219_Conv2d_18x256x1x1_Hsigmoid);
    CNN_SetGenCtrl(&gen_ctrl_S219_Conv2d_18x256x1x1_Hsigmoid, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S219_Conv2d_18x256x1x1_Hsigmoid", &gen_ctrl_S219_Conv2d_18x256x1x1_Hsigmoid, 4, 1, 256, 18, 5, 4,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_HSIGMOID);
    // generator for CONV_2D_0_81_trans_out0
    // transpose from 18x4x5 to 4x5x18 ((1, 2, 0))
    CNN_3DTensorPermute("S220_Op_CONV_2D_0_81_trans_out0", 0, 1, 18, 5, 4, KOP_MATPERM_CHW2HWC);
    // generator for CONV_2D_0_83
    CNN_GenControl_T gen_ctrl_S224_Conv2d_24x128x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S224_Conv2d_24x128x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S224_Conv2d_24x128x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S224_Conv2d_24x128x1x1", &gen_ctrl_S224_Conv2d_24x128x1x1, 4, 1, 128, 24, 3, 2,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_83_trans_out0
    // transpose from 24x2x3 to 2x3x24 ((1, 2, 0))
    CNN_3DTensorPermute("S225_Op_CONV_2D_0_83_trans_out0", 0, 1, 24, 3, 2, KOP_MATPERM_CHW2HWC);
    // generator for CONV_2D_0_85_fusion
    CNN_GenControl_T gen_ctrl_S229_Conv2d_18x128x1x1_Hsigmoid;
    CNN_InitGenCtrl(&gen_ctrl_S229_Conv2d_18x128x1x1_Hsigmoid);
    CNN_SetGenCtrl(&gen_ctrl_S229_Conv2d_18x128x1x1_Hsigmoid, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S229_Conv2d_18x128x1x1_Hsigmoid", &gen_ctrl_S229_Conv2d_18x128x1x1_Hsigmoid, 4, 1, 128, 18, 3, 2,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_HSIGMOID);
    // generator for CONV_2D_0_85_trans_out0
    // transpose from 18x2x3 to 2x3x18 ((1, 2, 0))
    CNN_3DTensorPermute("S230_Op_CONV_2D_0_85_trans_out0", 0, 1, 18, 3, 2, KOP_MATPERM_CHW2HWC);
    // generator for CONV_2D_0_87
    CNN_GenControl_T gen_ctrl_S234_Conv2d_24x128x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S234_Conv2d_24x128x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S234_Conv2d_24x128x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S234_Conv2d_24x128x1x1", &gen_ctrl_S234_Conv2d_24x128x1x1, 4, 1, 128, 24, 2, 1,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_87_trans_out0
    // transpose from 24x1x2 to 1x2x24 ((1, 0))
    CNN_MatTranspose("S235_Op_CONV_2D_0_87_trans_out0", 0, 1, 1, 2, 24, KOP_MATTRANSP);
    // generator for CONV_2D_0_89_fusion
    CNN_GenControl_T gen_ctrl_S239_Conv2d_18x128x1x1_Hsigmoid;
    CNN_InitGenCtrl(&gen_ctrl_S239_Conv2d_18x128x1x1_Hsigmoid);
    CNN_SetGenCtrl(&gen_ctrl_S239_Conv2d_18x128x1x1_Hsigmoid, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S239_Conv2d_18x128x1x1_Hsigmoid", &gen_ctrl_S239_Conv2d_18x128x1x1_Hsigmoid, 4, 1, 128, 18, 2, 1,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_HSIGMOID);
    // generator for CONV_2D_0_89_trans_out0
    // transpose from 18x1x2 to 1x2x18 ((1, 0))
    CNN_MatTranspose("S240_Op_CONV_2D_0_89_trans_out0", 0, 1, 1, 2, 18, KOP_MATTRANSP);
    // generator for CONV_2D_0_91
    CNN_GenControl_T gen_ctrl_S244_Conv2d_24x64x1x1;
    CNN_InitGenCtrl(&gen_ctrl_S244_Conv2d_24x64x1x1);
    CNN_SetGenCtrl(&gen_ctrl_S244_Conv2d_24x64x1x1, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S244_Conv2d_24x64x1x1", &gen_ctrl_S244_Conv2d_24x64x1x1, 4, 1, 64, 24, 1, 1,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_NONE);
    // generator for CONV_2D_0_95_fusion
    CNN_GenControl_T gen_ctrl_S250_Conv2d_18x64x1x1_Hsigmoid;
    CNN_InitGenCtrl(&gen_ctrl_S250_Conv2d_18x64x1x1_Hsigmoid);
    CNN_SetGenCtrl(&gen_ctrl_S250_Conv2d_18x64x1x1_Hsigmoid, "ENABLEIM2COL", AT_OPT_VAL(1));
    CNN_ConvolutionPoolAct_SQ8("S250_Conv2d_18x64x1x1_Hsigmoid", &gen_ctrl_S250_Conv2d_18x64x1x1_Hsigmoid, 4, 1, 64, 18, 1, 1,
        KOP_CONV, 1, 1, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0,
        KOP_HSIGMOID);
    
    // generator for CUSTOM_0_99
    CNN_SSD_PostProcess_SQ8("S254_Op_CUSTOM_0_99", 0, 1554, 3, 10, 300, 0.000, 0.500);
    

#define GRAPH
#ifdef GRAPH
    CreateGraph("SSD_tin_can_bottleCNN",
        /* Arguments either passed or globals */
            CArgs(378,
                TCArgInfo("signed char * __restrict__", "Input_1", ARG_SCOPE_ARG, ARG_DIR_IN, AT_MEM_L3_HRAM, AT_MEM_L3_HRAM, 0),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2con_8db1191d", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2con_8db1191d.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2con", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2con.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S3_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S3_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S3_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S3_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S3_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S3_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_64df44de", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_64df44de.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_8ca654a8", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_8ca654a8.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S6_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S6_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S6_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S6_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S6_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S6_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_885a3bd0", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_885a3bd0.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_7e38b462", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_7e38b462.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S9_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S9_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S9_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S9_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S9_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S9_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 19 In1ScaleN: 2 OutScale: 13 OutScaleN: 6
                TCArgInfo("signed char * __restrict__", "S10_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S10_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_8539b665", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_8539b665.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_ab63e9b9", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_ab63e9b9.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S13_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S13_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S13_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S13_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S13_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S13_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_a2b059aa", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_a2b059aa.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_6a98a40c", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_6a98a40c.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S16_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S16_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S16_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S16_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S16_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S16_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_3263cbf4", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_3263cbf4.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_798c9299", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_798c9299.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S19_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S19_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S19_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S19_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S19_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S19_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_c154601d", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_c154601d.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_9bde526a", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_9bde526a.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S22_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S22_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S22_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S22_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S22_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S22_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_cbbad378", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_cbbad378.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_91d5cf3c", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_91d5cf3c.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S25_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S25_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S25_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S25_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S25_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S25_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_b306895a", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_b306895a.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_11dac546", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_11dac546.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S28_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S28_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S28_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S28_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S28_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S28_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 35 In1ScaleN: 5 OutScale: 197 OutScaleN: 8
                TCArgInfo("signed char * __restrict__", "S29_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S29_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_1fe80eb6", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_1fe80eb6.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_dbf573c5", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_dbf573c5.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S32_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S32_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S32_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S32_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S32_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S32_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_6aa4b1e3", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_6aa4b1e3.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_efe56c7d", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_efe56c7d.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S35_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S35_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S35_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S35_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S35_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S35_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_2a83c320", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_2a83c320.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_e51902fe", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_e51902fe.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S38_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S38_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S38_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S38_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S38_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S38_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_e87a7605", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_e87a7605.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_36501435", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_36501435.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S41_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S41_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S41_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S41_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S41_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S41_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_ac66cd91", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_ac66cd91.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_cc95eaf6", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_cc95eaf6.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S44_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S44_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S44_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S44_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S44_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S44_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_042cd376", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_042cd376.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_3ce152a9", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_3ce152a9.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S47_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S47_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S47_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S47_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S47_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S47_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 73 In1ScaleN: 6 OutScale: 167 OutScaleN: 8
                TCArgInfo("signed char * __restrict__", "S48_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S48_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_b7f84a57", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_b7f84a57.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_f8c2ff15", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_f8c2ff15.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S51_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S51_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S51_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S51_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S51_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S51_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_a26d0376", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_a26d0376.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_61559135", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_61559135.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S54_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S54_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S54_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S54_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S54_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S54_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_14c9cb1f", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_14c9cb1f.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_8471409f", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_8471409f.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S57_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S57_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S57_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S57_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S57_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S57_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 167 In1ScaleN: 7 OutScale: 19 OutScaleN: 5
                TCArgInfo("signed char * __restrict__", "S58_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S58_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_03c5956b", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_03c5956b.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_8f0593ba", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_8f0593ba.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S61_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S61_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S61_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S61_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S61_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S61_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_98528ead", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_98528ead.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_357440b1", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_357440b1.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S64_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S64_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S64_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S64_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S64_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S64_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_9ff86288", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_9ff86288.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_51da2993", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_51da2993.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S67_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S67_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S67_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S67_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S67_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S67_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_f1c78ba1", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_f1c78ba1.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_799802b5", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_799802b5.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S70_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S70_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S70_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S70_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S70_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S70_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_3f45aca9", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_3f45aca9.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_93afe2ba", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_93afe2ba.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S73_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S73_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S73_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S73_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S73_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S73_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_9046602e", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_9046602e.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_712985b5", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_712985b5.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S76_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S76_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S76_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S76_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S76_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S76_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 65 In1ScaleN: 6 OutScale: 169 OutScaleN: 8
                TCArgInfo("signed char * __restrict__", "S77_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S77_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_aa5a1409", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_aa5a1409.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_c8bd61b2", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_c8bd61b2.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S80_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S80_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S80_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S80_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S80_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S80_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_504b4626", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_504b4626.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_f6b95d2f", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_f6b95d2f.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S83_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S83_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S83_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S83_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S83_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S83_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_87c48226", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_87c48226.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_0824e34e", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_0824e34e.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S86_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S86_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S86_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S86_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S86_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S86_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 93 In1ScaleN: 6 OutScale: 59 OutScaleN: 7
                TCArgInfo("signed char * __restrict__", "S87_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S87_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_e5d6b18e", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_e5d6b18e.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_72e5a603", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_72e5a603.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S90_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S90_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S90_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S90_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S90_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S90_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_48c92884", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_48c92884.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_84d4964c", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_84d4964c.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S93_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S93_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S93_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S93_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S93_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S93_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_a45cfb5d", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_a45cfb5d.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_32ac9c9c", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_32ac9c9c.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S96_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S96_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S96_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S96_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S96_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S96_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 27 In1ScaleN: 4 OutScale: 117 OutScaleN: 8
                TCArgInfo("signed char * __restrict__", "S97_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S97_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_172c2d0b", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_172c2d0b.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_190b2a61", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_190b2a61.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S100_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S100_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S100_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S100_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S100_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S100_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_d364dbb1", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_d364dbb1.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_9e08a626", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_9e08a626.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S103_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S103_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S103_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S103_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S103_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S103_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_bc6311f9", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_bc6311f9.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_ad8e0195", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_ad8e0195.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S106_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S106_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S106_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S106_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S106_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S106_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_edc9c55e", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_edc9c55e.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_aec326a5", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_aec326a5.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S109_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S109_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S109_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S109_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S109_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S109_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_100db087", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_100db087.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_a517cc75", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_a517cc75.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S112_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S112_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S112_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S112_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S112_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S112_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_80de23f2", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_80de23f2.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_f99239a3", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_f99239a3.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S115_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S115_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S115_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S115_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S115_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S115_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 71 In1ScaleN: 6 OutScale: 143 OutScaleN: 8
                TCArgInfo("signed char * __restrict__", "S116_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S116_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_17e9da8c", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_17e9da8c.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_a5e8bb11", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_a5e8bb11.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S119_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S119_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S119_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S119_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S119_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S119_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_6a1c5063", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_6a1c5063.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_13068f9c", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_13068f9c.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S122_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S122_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S122_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S122_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S122_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S122_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_90761782", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_90761782.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_f3f69494", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_f3f69494.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S125_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S125_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S125_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S125_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S125_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S125_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 171 In1ScaleN: 7 OutScale: 1 OutScaleN: 1
                TCArgInfo("signed char * __restrict__", "S126_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S126_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_36da854d", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_36da854d.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_14da572a", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_14da572a.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S129_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S129_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S129_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S129_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S129_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S129_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_db5d9d70", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_db5d9d70.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_1c666cb3", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_1c666cb3.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S132_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S132_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S132_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S132_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S132_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S132_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_34736308", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_34736308.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_d45a7bdb", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_d45a7bdb.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S135_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S135_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S135_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S135_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S135_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S135_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_9244bcb4", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_9244bcb4.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_261aeebe", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_261aeebe.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S138_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S138_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S138_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S138_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S138_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S138_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_16d57bcc", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_16d57bcc.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_7600ef9a", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_7600ef9a.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S141_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S141_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S141_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S141_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S141_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S141_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_48d7e2f4", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_48d7e2f4.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_cdb05c4c", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_cdb05c4c.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S144_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S144_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S144_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S144_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S144_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S144_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 9 In1ScaleN: 3 OutScale: 153 OutScaleN: 8
                TCArgInfo("signed char * __restrict__", "S145_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S145_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_e09b1350", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_e09b1350.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_6b7ac92d", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_6b7ac92d.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S148_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S148_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S148_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S148_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S148_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S148_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_b852e846", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_b852e846.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_2eae7951", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_2eae7951.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S151_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S151_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S151_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S151_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S151_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S151_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_808e88f2", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_808e88f2.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_d656d862", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_d656d862.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S154_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S154_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S154_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S154_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S154_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S154_Infos.tensor", 1, 1, 8, 0)),
                // In1Scale: 21 In1ScaleN: 4 OutScale: 141 OutScaleN: 8
                TCArgInfo("signed char * __restrict__", "S155_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S155_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_c26b18f7", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_c26b18f7.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_56728354", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_56728354.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S158_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S158_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S158_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S158_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S158_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S158_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_6d709c12", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_6d709c12.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_edeb4d0d", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_edeb4d0d.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S161_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S161_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S161_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S161_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S161_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S161_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2exp_72f16966", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_72f16966.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2exp_9d792281", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2exp_9d792281.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S164_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S164_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S164_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S164_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S164_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S164_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2con_b559ba6c", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2con_b559ba6c.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2con_ae878fea", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2con_ae878fea.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S167_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S167_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S167_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S167_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S167_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S167_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2lay_0e194e91", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_0e194e91.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2lay", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S170_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S170_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S170_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S170_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S170_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S170_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2lay_bc24e588", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_bc24e588.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2lay_b6d92256", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_b6d92256.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S173_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S173_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S173_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S173_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S173_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S173_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2lay_0a98f919", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_0a98f919.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2lay_45c41df4", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_45c41df4.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S176_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S176_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S176_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S176_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S176_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S176_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2lay_b3809151", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_b3809151.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2lay_47288463", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_47288463.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S179_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S179_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S179_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S179_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S179_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S179_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2lay_ee42bacd", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_ee42bacd.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2lay_950f395a", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_950f395a.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S182_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S182_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S182_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S182_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S182_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S182_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2lay_e1fc21f0", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_e1fc21f0.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2lay_493d8d4d", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_493d8d4d.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S185_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S185_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S185_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S185_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S185_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S185_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2lay_4eb0e6ed", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_4eb0e6ed.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2lay_09dff4e3", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_09dff4e3.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S188_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S188_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S188_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S188_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S188_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S188_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Featureextractormobilenetv2lay_2875ae35", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_2875ae35.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Featureextractormobilenetv2lay_66bcd31b", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Featureextractormobilenetv2lay_66bcd31b.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S191_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S191_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S191_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S191_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.047244 out: 0.047244 A0: 127 B0: 0 C0: 0
                TCArgInfo("signed char * __restrict__", "S191_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S191_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_0boxencodingpredi_696b62c5", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_0boxencodingpredi_696b62c5.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_0boxencodingpredi_b48ea6a4", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_0boxencodingpredi_b48ea6a4.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S194_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S194_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S194_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S194_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S194_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S194_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_0classpredictorwe", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_0classpredictorwe.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_0classpredictorco", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_0classpredictorco.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S199_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S199_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S199_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S199_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.117263 out: 0.007843 qbias: 159 qnorm: 6 A0: 51 B0: 26 C0: 1
                TCArgInfo("signed char * __restrict__", "S199_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S199_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_1boxencodingpredi_3dba6efc", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_1boxencodingpredi_3dba6efc.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_1boxencodingpredi_2a41b47f", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_1boxencodingpredi_2a41b47f.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S204_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S204_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S204_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S204_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S204_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S204_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_1classpredictorwe", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_1classpredictorwe.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_1classpredictorco", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_1classpredictorco.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S209_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S209_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S209_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S209_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.117263 out: 0.007843 qbias: 159 qnorm: 6 A0: 51 B0: 26 C0: 1
                TCArgInfo("signed char * __restrict__", "S209_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S209_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_2boxencodingpredi_aba037ce", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_2boxencodingpredi_aba037ce.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_2boxencodingpredi_25bcf53a", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_2boxencodingpredi_25bcf53a.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S214_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S214_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S214_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S214_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S214_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S214_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_2classpredictorwe", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_2classpredictorwe.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_2classpredictorco", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_2classpredictorco.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S219_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S219_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S219_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S219_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.117263 out: 0.007843 qbias: 159 qnorm: 6 A0: 51 B0: 26 C0: 1
                TCArgInfo("signed char * __restrict__", "S219_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S219_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_3boxencodingpredi_21bd6b57", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_3boxencodingpredi_21bd6b57.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_3boxencodingpredi_c12a614a", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_3boxencodingpredi_c12a614a.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S224_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S224_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S224_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S224_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S224_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S224_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_3classpredictorwe", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_3classpredictorwe.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_3classpredictorco", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_3classpredictorco.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S229_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S229_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S229_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S229_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.117263 out: 0.007843 qbias: 159 qnorm: 6 A0: 51 B0: 26 C0: 1
                TCArgInfo("signed char * __restrict__", "S229_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S229_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_4boxencodingpredi_80b21d17", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_4boxencodingpredi_80b21d17.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_4boxencodingpredi_be9e9917", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_4boxencodingpredi_be9e9917.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S234_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S234_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S234_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S234_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S234_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S234_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_4classpredictorwe", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_4classpredictorwe.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_4classpredictorco", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_4classpredictorco.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S239_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S239_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S239_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S239_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.117263 out: 0.007843 qbias: 159 qnorm: 6 A0: 51 B0: 26 C0: 1
                TCArgInfo("signed char * __restrict__", "S239_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S239_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_5boxencodingpredi_6beccbdb", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_5boxencodingpredi_6beccbdb.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_5boxencodingpredi_6990bed4", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_5boxencodingpredi_6990bed4.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S244_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S244_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S244_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S244_Mul_shift.tensor", 1, 1, 8, 0)),
                // BiasQ: 0
                TCArgInfo("signed char * __restrict__", "S244_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S244_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Boxpredictor_5classpredictorwe", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_5classpredictorwe.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed int * __restrict__", "Boxpredictor_5classpredictorco", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Boxpredictor_5classpredictorco.tensor", 1, 1, 32, 0)),
                TCArgInfo("unsigned char * __restrict__", "S250_Mul_scale", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S250_Mul_scale.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S250_Mul_shift", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S250_Mul_shift.tensor", 1, 1, 8, 0)),
                // in: 0.117263 out: 0.007843 qbias: 159 qnorm: 6 A0: 51 B0: 26 C0: 1
                TCArgInfo("signed char * __restrict__", "S250_Infos", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S250_Infos.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "Anchors", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/Anchors.tensor", 1, 1, 8, 0)),
                TCArgInfo("unsigned char * __restrict__", "S254_Ssd_scales", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S254_Ssd_scales.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed char * __restrict__", "S254_Ssd_norms", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_SSD/tensors/S254_Ssd_norms.tensor", 1, 1, 8, 0)),
                TCArgInfo("signed short * __restrict__", "Output_1", ARG_SCOPE_ARG, ARG_DIR_OUT, AT_MEM_L2, AT_MEM_L2, 0),
                TCArgInfo("signed char * __restrict__", "Output_2", ARG_SCOPE_ARG, ARG_DIR_OUT, AT_MEM_L2, AT_MEM_L2, 0),
                TCArgInfo("signed char * __restrict__", "Output_3", ARG_SCOPE_ARG, ARG_DIR_OUT, AT_MEM_L2, AT_MEM_L2, 0)
            ),
        /* Locals, allocated dynamically */
        CArgs(83,
            TCArgInfo("signed char * __restrict__", "S3_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S6_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S9_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S10_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S13_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S16_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S19_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S22_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S25_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S28_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S29_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S32_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S35_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S38_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S41_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S44_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S47_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S48_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S51_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S54_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S57_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S58_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S61_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S64_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S67_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S70_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S73_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S76_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S77_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S80_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S83_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S86_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S87_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S90_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S93_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S96_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S97_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S100_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S103_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S106_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S109_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S112_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S115_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S116_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S119_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S122_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S125_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S126_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S129_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S132_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S135_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S138_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S141_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S144_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S145_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S148_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S151_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S154_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S155_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S158_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S161_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S164_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S167_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S170_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S173_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S176_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S179_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S182_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S185_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S188_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S191_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S194_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S199_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S204_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S209_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S214_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S219_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S224_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S229_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S234_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S239_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S246_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char * __restrict__", "S252_Output", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0)
        )
    );

    /* Stacked tensors - Concats */
    AddStackedTensors("S246_Output", 6, "S195_Output", "S205_Output", "S215_Output", "S225_Output", "S235_Output", "S244_Output");
    AddStackedTensors("S252_Output", 6, "S200_Output", "S210_Output", "S220_Output", "S230_Output", "S240_Output", "S250_Output");

    // Node S3_Conv2d_16x3x3x3_Relu6 inq -1.00<(i8-0.00)*0.00784314<1.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S3_Conv2d_16x3x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "Input_1", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2con_8db1191d", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2con", 0), GNodeArg(GNA_OUT, "S3_Output", 0), GNodeArg(GNA_IN, "S3_Mul_scale", 0), GNodeArg(GNA_IN, "S3_Mul_shift", 0), GNodeArg(GNA_IN, "S3_Infos", 0)));
    // Node S6_Conv2d_16x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S6_Conv2d_16x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S3_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_64df44de", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_8ca654a8", 0), GNodeArg(GNA_OUT, "S6_Output", 0), GNodeArg(GNA_IN, "S6_Mul_scale", 0), GNodeArg(GNA_IN, "S6_Mul_shift", 0), GNodeArg(GNA_IN, "S6_Infos", 0)));
    // Node S9_Conv2d_16x16x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -28.65<(i8-0.00)*0.22386190<28.43 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S9_Conv2d_16x16x1x1", Bindings(7, GNodeArg(GNA_IN, "S6_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_885a3bd0", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_7e38b462", 0), GNodeArg(GNA_OUT, "S9_Output", 0), GNodeArg(GNA_IN, "S9_Mul_scale", 0), GNodeArg(GNA_IN, "S9_Mul_shift", 0), GNodeArg(GNA_IN, "S9_Infos", 0)));
    // Node S10_MatAdd_16x120x160 in1q -28.65<(i8-0.00)*0.22386190<28.43 forced in2q -6.05<(i8-0.00)*0.04724409<6.00 forced outq -29.77<(i8-0.00)*0.23254792<29.53 forced
    AddNode("S10_MatAdd_16x120x160", Bindings(4, GNodeArg(GNA_IN, "S9_Output", 0), GNodeArg(GNA_IN, "S3_Output", 0), GNodeArg(GNA_OUT, "S10_Output", 0), GNodeArg(GNA_IN, "S10_Infos", 0)));
    // Node S13_Conv2d_96x16x1x1_Relu6 inq -29.77<(i8-0.00)*0.23254792<29.53 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S13_Conv2d_96x16x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S10_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_8539b665", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_ab63e9b9", 0), GNodeArg(GNA_OUT, "S13_Output", 0), GNodeArg(GNA_IN, "S13_Mul_scale", 0), GNodeArg(GNA_IN, "S13_Mul_shift", 0), GNodeArg(GNA_IN, "S13_Infos", 0)));
    // Node S16_Conv2d_96x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S16_Conv2d_96x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S13_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_a2b059aa", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_6a98a40c", 0), GNodeArg(GNA_OUT, "S16_Output", 0), GNodeArg(GNA_IN, "S16_Mul_scale", 0), GNodeArg(GNA_IN, "S16_Mul_shift", 0), GNodeArg(GNA_IN, "S16_Infos", 0)));
    // Node S19_Conv2d_16x96x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -11.13<(i8-0.00)*0.08691512<11.04 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S19_Conv2d_16x96x1x1", Bindings(7, GNodeArg(GNA_IN, "S16_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_3263cbf4", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_798c9299", 0), GNodeArg(GNA_OUT, "S19_Output", 0), GNodeArg(GNA_IN, "S19_Mul_scale", 0), GNodeArg(GNA_IN, "S19_Mul_shift", 0), GNodeArg(GNA_IN, "S19_Infos", 0)));
    // Node S22_Conv2d_96x16x1x1_Relu6 inq -11.13<(i8-0.00)*0.08691512<11.04 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S22_Conv2d_96x16x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S19_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_c154601d", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_9bde526a", 0), GNodeArg(GNA_OUT, "S22_Output", 0), GNodeArg(GNA_IN, "S22_Mul_scale", 0), GNodeArg(GNA_IN, "S22_Mul_shift", 0), GNodeArg(GNA_IN, "S22_Infos", 0)));
    // Node S25_Conv2d_96x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S25_Conv2d_96x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S22_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_cbbad378", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_91d5cf3c", 0), GNodeArg(GNA_OUT, "S25_Output", 0), GNodeArg(GNA_IN, "S25_Mul_scale", 0), GNodeArg(GNA_IN, "S25_Mul_shift", 0), GNodeArg(GNA_IN, "S25_Infos", 0)));
    // Node S28_Conv2d_16x96x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -10.16<(i8-0.00)*0.07936735<10.08 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S28_Conv2d_16x96x1x1", Bindings(7, GNodeArg(GNA_IN, "S25_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_b306895a", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_11dac546", 0), GNodeArg(GNA_OUT, "S28_Output", 0), GNodeArg(GNA_IN, "S28_Mul_scale", 0), GNodeArg(GNA_IN, "S28_Mul_shift", 0), GNodeArg(GNA_IN, "S28_Infos", 0)));
    // Node S29_MatAdd_16x60x80 in1q -11.13<(i8-0.00)*0.08691512<11.04 forced in2q -10.16<(i8-0.00)*0.07936735<10.08 forced outq -13.19<(i8-0.00)*0.10305329<13.09 forced
    AddNode("S29_MatAdd_16x60x80", Bindings(4, GNodeArg(GNA_IN, "S19_Output", 0), GNodeArg(GNA_IN, "S28_Output", 0), GNodeArg(GNA_OUT, "S29_Output", 0), GNodeArg(GNA_IN, "S29_Infos", 0)));
    // Node S32_Conv2d_96x16x1x1_Relu6 inq -13.19<(i8-0.00)*0.10305329<13.09 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S32_Conv2d_96x16x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S29_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_1fe80eb6", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_dbf573c5", 0), GNodeArg(GNA_OUT, "S32_Output", 0), GNodeArg(GNA_IN, "S32_Mul_scale", 0), GNodeArg(GNA_IN, "S32_Mul_shift", 0), GNodeArg(GNA_IN, "S32_Infos", 0)));
    // Node S35_Conv2d_96x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S35_Conv2d_96x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S32_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_6aa4b1e3", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_efe56c7d", 0), GNodeArg(GNA_OUT, "S35_Output", 0), GNodeArg(GNA_IN, "S35_Mul_scale", 0), GNodeArg(GNA_IN, "S35_Mul_shift", 0), GNodeArg(GNA_IN, "S35_Infos", 0)));
    // Node S38_Conv2d_16x96x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -7.22<(i8-0.00)*0.05640914<7.16 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S38_Conv2d_16x96x1x1", Bindings(7, GNodeArg(GNA_IN, "S35_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_2a83c320", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_e51902fe", 0), GNodeArg(GNA_OUT, "S38_Output", 0), GNodeArg(GNA_IN, "S38_Mul_scale", 0), GNodeArg(GNA_IN, "S38_Mul_shift", 0), GNodeArg(GNA_IN, "S38_Infos", 0)));
    // Node S41_Conv2d_96x16x1x1_Relu6 inq -7.22<(i8-0.00)*0.05640914<7.16 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S41_Conv2d_96x16x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S38_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_e87a7605", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_36501435", 0), GNodeArg(GNA_OUT, "S41_Output", 0), GNodeArg(GNA_IN, "S41_Mul_scale", 0), GNodeArg(GNA_IN, "S41_Mul_shift", 0), GNodeArg(GNA_IN, "S41_Infos", 0)));
    // Node S44_Conv2d_96x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S44_Conv2d_96x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S41_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_ac66cd91", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_cc95eaf6", 0), GNodeArg(GNA_OUT, "S44_Output", 0), GNodeArg(GNA_IN, "S44_Mul_scale", 0), GNodeArg(GNA_IN, "S44_Mul_shift", 0), GNodeArg(GNA_IN, "S44_Infos", 0)));
    // Node S47_Conv2d_16x96x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -6.34<(i8-0.00)*0.04955105<6.29 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S47_Conv2d_16x96x1x1", Bindings(7, GNodeArg(GNA_IN, "S44_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_042cd376", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_3ce152a9", 0), GNodeArg(GNA_OUT, "S47_Output", 0), GNodeArg(GNA_IN, "S47_Mul_scale", 0), GNodeArg(GNA_IN, "S47_Mul_shift", 0), GNodeArg(GNA_IN, "S47_Infos", 0)));
    // Node S48_MatAdd_16x30x40 in1q -7.22<(i8-0.00)*0.05640914<7.16 forced in2q -6.34<(i8-0.00)*0.04955105<6.29 forced outq -9.70<(i8-0.00)*0.07579126<9.63 forced
    AddNode("S48_MatAdd_16x30x40", Bindings(4, GNodeArg(GNA_IN, "S38_Output", 0), GNodeArg(GNA_IN, "S47_Output", 0), GNodeArg(GNA_OUT, "S48_Output", 0), GNodeArg(GNA_IN, "S48_Infos", 0)));
    // Node S51_Conv2d_96x16x1x1_Relu6 inq -9.70<(i8-0.00)*0.07579126<9.63 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S51_Conv2d_96x16x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S48_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_b7f84a57", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_f8c2ff15", 0), GNodeArg(GNA_OUT, "S51_Output", 0), GNodeArg(GNA_IN, "S51_Mul_scale", 0), GNodeArg(GNA_IN, "S51_Mul_shift", 0), GNodeArg(GNA_IN, "S51_Infos", 0)));
    // Node S54_Conv2d_96x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S54_Conv2d_96x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S51_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_a26d0376", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_61559135", 0), GNodeArg(GNA_OUT, "S54_Output", 0), GNodeArg(GNA_IN, "S54_Mul_scale", 0), GNodeArg(GNA_IN, "S54_Mul_shift", 0), GNodeArg(GNA_IN, "S54_Infos", 0)));
    // Node S57_Conv2d_16x96x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -7.44<(i8-0.00)*0.05814352<7.38 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S57_Conv2d_16x96x1x1", Bindings(7, GNodeArg(GNA_IN, "S54_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_14c9cb1f", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_8471409f", 0), GNodeArg(GNA_OUT, "S57_Output", 0), GNodeArg(GNA_IN, "S57_Mul_scale", 0), GNodeArg(GNA_IN, "S57_Mul_shift", 0), GNodeArg(GNA_IN, "S57_Infos", 0)));
    // Node S58_MatAdd_16x30x40 in1q -9.70<(i8-0.00)*0.07579126<9.63 forced in2q -7.44<(i8-0.00)*0.05814352<7.38 forced outq -12.57<(i8-0.00)*0.09818900<12.47 forced
    AddNode("S58_MatAdd_16x30x40", Bindings(4, GNodeArg(GNA_IN, "S48_Output", 0), GNodeArg(GNA_IN, "S57_Output", 0), GNodeArg(GNA_OUT, "S58_Output", 0), GNodeArg(GNA_IN, "S58_Infos", 0)));
    // Node S61_Conv2d_96x16x1x1_Relu6 inq -12.57<(i8-0.00)*0.09818900<12.47 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S61_Conv2d_96x16x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S58_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_03c5956b", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_8f0593ba", 0), GNodeArg(GNA_OUT, "S61_Output", 0), GNodeArg(GNA_IN, "S61_Mul_scale", 0), GNodeArg(GNA_IN, "S61_Mul_shift", 0), GNodeArg(GNA_IN, "S61_Infos", 0)));
    // Node S64_Conv2d_96x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S64_Conv2d_96x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S61_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_98528ead", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_357440b1", 0), GNodeArg(GNA_OUT, "S64_Output", 0), GNodeArg(GNA_IN, "S64_Mul_scale", 0), GNodeArg(GNA_IN, "S64_Mul_shift", 0), GNodeArg(GNA_IN, "S64_Infos", 0)));
    // Node S67_Conv2d_32x96x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -5.74<(i8-0.00)*0.04483806<5.69 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S67_Conv2d_32x96x1x1", Bindings(7, GNodeArg(GNA_IN, "S64_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_9ff86288", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_51da2993", 0), GNodeArg(GNA_OUT, "S67_Output", 0), GNodeArg(GNA_IN, "S67_Mul_scale", 0), GNodeArg(GNA_IN, "S67_Mul_shift", 0), GNodeArg(GNA_IN, "S67_Infos", 0)));
    // Node S70_Conv2d_192x32x1x1_Relu6 inq -5.74<(i8-0.00)*0.04483806<5.69 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S70_Conv2d_192x32x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S67_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_f1c78ba1", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_799802b5", 0), GNodeArg(GNA_OUT, "S70_Output", 0), GNodeArg(GNA_IN, "S70_Mul_scale", 0), GNodeArg(GNA_IN, "S70_Mul_shift", 0), GNodeArg(GNA_IN, "S70_Infos", 0)));
    // Node S73_Conv2d_192x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S73_Conv2d_192x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S70_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_3f45aca9", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_93afe2ba", 0), GNodeArg(GNA_OUT, "S73_Output", 0), GNodeArg(GNA_IN, "S73_Mul_scale", 0), GNodeArg(GNA_IN, "S73_Mul_shift", 0), GNodeArg(GNA_IN, "S73_Infos", 0)));
    // Node S76_Conv2d_32x192x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -5.83<(i8-0.00)*0.04555831<5.79 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S76_Conv2d_32x192x1x1", Bindings(7, GNodeArg(GNA_IN, "S73_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_9046602e", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_712985b5", 0), GNodeArg(GNA_OUT, "S76_Output", 0), GNodeArg(GNA_IN, "S76_Mul_scale", 0), GNodeArg(GNA_IN, "S76_Mul_shift", 0), GNodeArg(GNA_IN, "S76_Infos", 0)));
    // Node S77_MatAdd_32x15x20 in1q -5.83<(i8-0.00)*0.04555831<5.79 forced in2q -5.74<(i8-0.00)*0.04483806<5.69 forced outq -8.71<(i8-0.00)*0.06806233<8.64 forced
    AddNode("S77_MatAdd_32x15x20", Bindings(4, GNodeArg(GNA_IN, "S76_Output", 0), GNodeArg(GNA_IN, "S67_Output", 0), GNodeArg(GNA_OUT, "S77_Output", 0), GNodeArg(GNA_IN, "S77_Infos", 0)));
    // Node S80_Conv2d_192x32x1x1_Relu6 inq -8.71<(i8-0.00)*0.06806233<8.64 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S80_Conv2d_192x32x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S77_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_aa5a1409", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_c8bd61b2", 0), GNodeArg(GNA_OUT, "S80_Output", 0), GNodeArg(GNA_IN, "S80_Mul_scale", 0), GNodeArg(GNA_IN, "S80_Mul_shift", 0), GNodeArg(GNA_IN, "S80_Infos", 0)));
    // Node S83_Conv2d_192x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S83_Conv2d_192x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S80_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_504b4626", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_f6b95d2f", 0), GNodeArg(GNA_OUT, "S83_Output", 0), GNodeArg(GNA_IN, "S83_Mul_scale", 0), GNodeArg(GNA_IN, "S83_Mul_shift", 0), GNodeArg(GNA_IN, "S83_Infos", 0)));
    // Node S86_Conv2d_32x192x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -6.00<(i8-0.00)*0.04685885<5.95 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S86_Conv2d_32x192x1x1", Bindings(7, GNodeArg(GNA_IN, "S83_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_87c48226", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_0824e34e", 0), GNodeArg(GNA_OUT, "S86_Output", 0), GNodeArg(GNA_IN, "S86_Mul_scale", 0), GNodeArg(GNA_IN, "S86_Mul_shift", 0), GNodeArg(GNA_IN, "S86_Infos", 0)));
    // Node S87_MatAdd_32x15x20 in1q -8.71<(i8-0.00)*0.06806233<8.64 forced in2q -6.00<(i8-0.00)*0.04685885<5.95 forced outq -12.99<(i8-0.00)*0.10150833<12.89 forced
    AddNode("S87_MatAdd_32x15x20", Bindings(4, GNodeArg(GNA_IN, "S77_Output", 0), GNodeArg(GNA_IN, "S86_Output", 0), GNodeArg(GNA_OUT, "S87_Output", 0), GNodeArg(GNA_IN, "S87_Infos", 0)));
    // Node S90_Conv2d_192x32x1x1_Relu6 inq -12.99<(i8-0.00)*0.10150833<12.89 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S90_Conv2d_192x32x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S87_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_e5d6b18e", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_72e5a603", 0), GNodeArg(GNA_OUT, "S90_Output", 0), GNodeArg(GNA_IN, "S90_Mul_scale", 0), GNodeArg(GNA_IN, "S90_Mul_shift", 0), GNodeArg(GNA_IN, "S90_Infos", 0)));
    // Node S93_Conv2d_192x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S93_Conv2d_192x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S90_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_48c92884", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_84d4964c", 0), GNodeArg(GNA_OUT, "S93_Output", 0), GNodeArg(GNA_IN, "S93_Mul_scale", 0), GNodeArg(GNA_IN, "S93_Mul_shift", 0), GNodeArg(GNA_IN, "S93_Infos", 0)));
    // Node S96_Conv2d_32x192x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -7.71<(i8-0.00)*0.06022034<7.65 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S96_Conv2d_32x192x1x1", Bindings(7, GNodeArg(GNA_IN, "S93_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_a45cfb5d", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_32ac9c9c", 0), GNodeArg(GNA_OUT, "S96_Output", 0), GNodeArg(GNA_IN, "S96_Mul_scale", 0), GNodeArg(GNA_IN, "S96_Mul_shift", 0), GNodeArg(GNA_IN, "S96_Infos", 0)));
    // Node S97_MatAdd_32x15x20 in1q -12.99<(i8-0.00)*0.10150833<12.89 forced in2q -7.71<(i8-0.00)*0.06022034<7.65 forced outq -16.86<(i8-0.00)*0.13173027<16.73 forced
    AddNode("S97_MatAdd_32x15x20", Bindings(4, GNodeArg(GNA_IN, "S87_Output", 0), GNodeArg(GNA_IN, "S96_Output", 0), GNodeArg(GNA_OUT, "S97_Output", 0), GNodeArg(GNA_IN, "S97_Infos", 0)));
    // Node S100_Conv2d_192x32x1x1_Relu6 inq -16.86<(i8-0.00)*0.13173027<16.73 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S100_Conv2d_192x32x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S97_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_172c2d0b", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_190b2a61", 0), GNodeArg(GNA_OUT, "S100_Output", 0), GNodeArg(GNA_IN, "S100_Mul_scale", 0), GNodeArg(GNA_IN, "S100_Mul_shift", 0), GNodeArg(GNA_IN, "S100_Infos", 0)));
    // Node S103_Conv2d_192x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S103_Conv2d_192x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S100_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_d364dbb1", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_9e08a626", 0), GNodeArg(GNA_OUT, "S103_Output", 0), GNodeArg(GNA_IN, "S103_Mul_scale", 0), GNodeArg(GNA_IN, "S103_Mul_shift", 0), GNodeArg(GNA_IN, "S103_Infos", 0)));
    // Node S106_Conv2d_48x192x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -6.57<(i8-0.00)*0.05129727<6.51 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S106_Conv2d_48x192x1x1", Bindings(7, GNodeArg(GNA_IN, "S103_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_bc6311f9", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_ad8e0195", 0), GNodeArg(GNA_OUT, "S106_Output", 0), GNodeArg(GNA_IN, "S106_Mul_scale", 0), GNodeArg(GNA_IN, "S106_Mul_shift", 0), GNodeArg(GNA_IN, "S106_Infos", 0)));
    // Node S109_Conv2d_288x48x1x1_Relu6 inq -6.57<(i8-0.00)*0.05129727<6.51 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S109_Conv2d_288x48x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S106_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_edc9c55e", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_aec326a5", 0), GNodeArg(GNA_OUT, "S109_Output", 0), GNodeArg(GNA_IN, "S109_Mul_scale", 0), GNodeArg(GNA_IN, "S109_Mul_shift", 0), GNodeArg(GNA_IN, "S109_Infos", 0)));
    // Node S112_Conv2d_288x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S112_Conv2d_288x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S109_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_100db087", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_a517cc75", 0), GNodeArg(GNA_OUT, "S112_Output", 0), GNodeArg(GNA_IN, "S112_Mul_scale", 0), GNodeArg(GNA_IN, "S112_Mul_shift", 0), GNodeArg(GNA_IN, "S112_Infos", 0)));
    // Node S115_Conv2d_48x288x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -7.26<(i8-0.00)*0.05675729<7.21 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S115_Conv2d_48x288x1x1", Bindings(7, GNodeArg(GNA_IN, "S112_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_80de23f2", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_f99239a3", 0), GNodeArg(GNA_OUT, "S115_Output", 0), GNodeArg(GNA_IN, "S115_Mul_scale", 0), GNodeArg(GNA_IN, "S115_Mul_shift", 0), GNodeArg(GNA_IN, "S115_Infos", 0)));
    // Node S116_MatAdd_48x15x20 in1q -7.26<(i8-0.00)*0.05675729<7.21 forced in2q -6.57<(i8-0.00)*0.05129727<6.51 forced outq -11.78<(i8-0.00)*0.09204145<11.69 forced
    AddNode("S116_MatAdd_48x15x20", Bindings(4, GNodeArg(GNA_IN, "S115_Output", 0), GNodeArg(GNA_IN, "S106_Output", 0), GNodeArg(GNA_OUT, "S116_Output", 0), GNodeArg(GNA_IN, "S116_Infos", 0)));
    // Node S119_Conv2d_288x48x1x1_Relu6 inq -11.78<(i8-0.00)*0.09204145<11.69 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S119_Conv2d_288x48x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S116_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_17e9da8c", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_a5e8bb11", 0), GNodeArg(GNA_OUT, "S119_Output", 0), GNodeArg(GNA_IN, "S119_Mul_scale", 0), GNodeArg(GNA_IN, "S119_Mul_shift", 0), GNodeArg(GNA_IN, "S119_Infos", 0)));
    // Node S122_Conv2d_288x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S122_Conv2d_288x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S119_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_6a1c5063", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_13068f9c", 0), GNodeArg(GNA_OUT, "S122_Output", 0), GNodeArg(GNA_IN, "S122_Mul_scale", 0), GNodeArg(GNA_IN, "S122_Mul_shift", 0), GNodeArg(GNA_IN, "S122_Infos", 0)));
    // Node S125_Conv2d_48x288x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -8.84<(i8-0.00)*0.06907581<8.77 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S125_Conv2d_48x288x1x1", Bindings(7, GNodeArg(GNA_IN, "S122_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_90761782", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_f3f69494", 0), GNodeArg(GNA_OUT, "S125_Output", 0), GNodeArg(GNA_IN, "S125_Mul_scale", 0), GNodeArg(GNA_IN, "S125_Mul_shift", 0), GNodeArg(GNA_IN, "S125_Infos", 0)));
    // Node S126_MatAdd_48x15x20 in1q -11.78<(i8-0.00)*0.09204145<11.69 forced in2q -8.84<(i8-0.00)*0.06907581<8.77 forced outq -17.67<(i8-0.00)*0.13804483<17.53 forced
    AddNode("S126_MatAdd_48x15x20", Bindings(4, GNodeArg(GNA_IN, "S116_Output", 0), GNodeArg(GNA_IN, "S125_Output", 0), GNodeArg(GNA_OUT, "S126_Output", 0), GNodeArg(GNA_IN, "S126_Infos", 0)));
    // Node S129_Conv2d_288x48x1x1_Relu6 inq -17.67<(i8-0.00)*0.13804483<17.53 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S129_Conv2d_288x48x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S126_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_36da854d", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_14da572a", 0), GNodeArg(GNA_OUT, "S129_Output", 0), GNodeArg(GNA_IN, "S129_Mul_scale", 0), GNodeArg(GNA_IN, "S129_Mul_shift", 0), GNodeArg(GNA_IN, "S129_Infos", 0)));
    // Node S132_Conv2d_288x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S132_Conv2d_288x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S129_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_db5d9d70", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_1c666cb3", 0), GNodeArg(GNA_OUT, "S132_Output", 0), GNodeArg(GNA_IN, "S132_Mul_scale", 0), GNodeArg(GNA_IN, "S132_Mul_shift", 0), GNodeArg(GNA_IN, "S132_Infos", 0)));
    // Node S135_Conv2d_80x288x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -5.51<(i8-0.00)*0.04305993<5.47 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S135_Conv2d_80x288x1x1", Bindings(7, GNodeArg(GNA_IN, "S132_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_34736308", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_d45a7bdb", 0), GNodeArg(GNA_OUT, "S135_Output", 0), GNodeArg(GNA_IN, "S135_Mul_scale", 0), GNodeArg(GNA_IN, "S135_Mul_shift", 0), GNodeArg(GNA_IN, "S135_Infos", 0)));
    // Node S138_Conv2d_480x80x1x1_Relu6 inq -5.51<(i8-0.00)*0.04305993<5.47 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S138_Conv2d_480x80x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S135_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_9244bcb4", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_261aeebe", 0), GNodeArg(GNA_OUT, "S138_Output", 0), GNodeArg(GNA_IN, "S138_Mul_scale", 0), GNodeArg(GNA_IN, "S138_Mul_shift", 0), GNodeArg(GNA_IN, "S138_Infos", 0)));
    // Node S141_Conv2d_480x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S141_Conv2d_480x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S138_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_16d57bcc", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_7600ef9a", 0), GNodeArg(GNA_OUT, "S141_Output", 0), GNodeArg(GNA_IN, "S141_Mul_scale", 0), GNodeArg(GNA_IN, "S141_Mul_shift", 0), GNodeArg(GNA_IN, "S141_Infos", 0)));
    // Node S144_Conv2d_80x480x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -6.21<(i8-0.00)*0.04853827<6.16 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S144_Conv2d_80x480x1x1", Bindings(7, GNodeArg(GNA_IN, "S141_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_48d7e2f4", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_cdb05c4c", 0), GNodeArg(GNA_OUT, "S144_Output", 0), GNodeArg(GNA_IN, "S144_Mul_scale", 0), GNodeArg(GNA_IN, "S144_Mul_shift", 0), GNodeArg(GNA_IN, "S144_Infos", 0)));
    // Node S145_MatAdd_80x8x10 in1q -6.21<(i8-0.00)*0.04853827<6.16 forced in2q -5.51<(i8-0.00)*0.04305993<5.47 forced outq -9.20<(i8-0.00)*0.07185803<9.13 forced
    AddNode("S145_MatAdd_80x8x10", Bindings(4, GNodeArg(GNA_IN, "S144_Output", 0), GNodeArg(GNA_IN, "S135_Output", 0), GNodeArg(GNA_OUT, "S145_Output", 0), GNodeArg(GNA_IN, "S145_Infos", 0)));
    // Node S148_Conv2d_480x80x1x1_Relu6 inq -9.20<(i8-0.00)*0.07185803<9.13 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S148_Conv2d_480x80x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S145_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_e09b1350", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_6b7ac92d", 0), GNodeArg(GNA_OUT, "S148_Output", 0), GNodeArg(GNA_IN, "S148_Mul_scale", 0), GNodeArg(GNA_IN, "S148_Mul_shift", 0), GNodeArg(GNA_IN, "S148_Infos", 0)));
    // Node S151_Conv2d_480x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S151_Conv2d_480x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S148_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_b852e846", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_2eae7951", 0), GNodeArg(GNA_OUT, "S151_Output", 0), GNodeArg(GNA_IN, "S151_Mul_scale", 0), GNodeArg(GNA_IN, "S151_Mul_shift", 0), GNodeArg(GNA_IN, "S151_Infos", 0)));
    // Node S154_Conv2d_80x480x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -7.03<(i8-0.00)*0.05489277<6.97 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S154_Conv2d_80x480x1x1", Bindings(7, GNodeArg(GNA_IN, "S151_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_808e88f2", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_d656d862", 0), GNodeArg(GNA_OUT, "S154_Output", 0), GNodeArg(GNA_IN, "S154_Mul_scale", 0), GNodeArg(GNA_IN, "S154_Mul_shift", 0), GNodeArg(GNA_IN, "S154_Infos", 0)));
    // Node S155_MatAdd_80x8x10 in1q -9.20<(i8-0.00)*0.07185803<9.13 forced in2q -7.03<(i8-0.00)*0.05489277<6.97 forced outq -12.74<(i8-0.00)*0.09949964<12.64 forced
    AddNode("S155_MatAdd_80x8x10", Bindings(4, GNodeArg(GNA_IN, "S145_Output", 0), GNodeArg(GNA_IN, "S154_Output", 0), GNodeArg(GNA_OUT, "S155_Output", 0), GNodeArg(GNA_IN, "S155_Infos", 0)));
    // Node S158_Conv2d_480x80x1x1_Relu6 inq -12.74<(i8-0.00)*0.09949964<12.64 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S158_Conv2d_480x80x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S155_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_c26b18f7", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_56728354", 0), GNodeArg(GNA_OUT, "S158_Output", 0), GNodeArg(GNA_IN, "S158_Mul_scale", 0), GNodeArg(GNA_IN, "S158_Mul_shift", 0), GNodeArg(GNA_IN, "S158_Infos", 0)));
    // Node S161_Conv2d_480x1x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S161_Conv2d_480x1x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S158_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_6d709c12", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_edeb4d0d", 0), GNodeArg(GNA_OUT, "S161_Output", 0), GNodeArg(GNA_IN, "S161_Mul_scale", 0), GNodeArg(GNA_IN, "S161_Mul_shift", 0), GNodeArg(GNA_IN, "S161_Infos", 0)));
    // Node S164_Conv2d_160x480x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -6.35<(i8-0.00)*0.04963627<6.30 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S164_Conv2d_160x480x1x1", Bindings(7, GNodeArg(GNA_IN, "S161_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_72f16966", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2exp_9d792281", 0), GNodeArg(GNA_OUT, "S164_Output", 0), GNodeArg(GNA_IN, "S164_Mul_scale", 0), GNodeArg(GNA_IN, "S164_Mul_shift", 0), GNodeArg(GNA_IN, "S164_Infos", 0)));
    // Node S167_Conv2d_640x160x1x1_Relu6 inq -6.35<(i8-0.00)*0.04963627<6.30 weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S167_Conv2d_640x160x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S164_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2con_b559ba6c", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2con_ae878fea", 0), GNodeArg(GNA_OUT, "S167_Output", 0), GNodeArg(GNA_IN, "S167_Mul_scale", 0), GNodeArg(GNA_IN, "S167_Mul_shift", 0), GNodeArg(GNA_IN, "S167_Infos", 0)));
    // Node S170_Conv2d_128x640x1x1_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S170_Conv2d_128x640x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S167_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_0e194e91", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay", 0), GNodeArg(GNA_OUT, "S170_Output", 0), GNodeArg(GNA_IN, "S170_Mul_scale", 0), GNodeArg(GNA_IN, "S170_Mul_shift", 0), GNodeArg(GNA_IN, "S170_Infos", 0)));
    // Node S173_Conv2d_256x128x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S173_Conv2d_256x128x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S170_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_bc24e588", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_b6d92256", 0), GNodeArg(GNA_OUT, "S173_Output", 0), GNodeArg(GNA_IN, "S173_Mul_scale", 0), GNodeArg(GNA_IN, "S173_Mul_shift", 0), GNodeArg(GNA_IN, "S173_Infos", 0)));
    // Node S176_Conv2d_64x256x1x1_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S176_Conv2d_64x256x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S173_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_0a98f919", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_45c41df4", 0), GNodeArg(GNA_OUT, "S176_Output", 0), GNodeArg(GNA_IN, "S176_Mul_scale", 0), GNodeArg(GNA_IN, "S176_Mul_shift", 0), GNodeArg(GNA_IN, "S176_Infos", 0)));
    // Node S179_Conv2d_128x64x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S179_Conv2d_128x64x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S176_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_b3809151", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_47288463", 0), GNodeArg(GNA_OUT, "S179_Output", 0), GNodeArg(GNA_IN, "S179_Mul_scale", 0), GNodeArg(GNA_IN, "S179_Mul_shift", 0), GNodeArg(GNA_IN, "S179_Infos", 0)));
    // Node S182_Conv2d_64x128x1x1_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S182_Conv2d_64x128x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S179_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_ee42bacd", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_950f395a", 0), GNodeArg(GNA_OUT, "S182_Output", 0), GNodeArg(GNA_IN, "S182_Mul_scale", 0), GNodeArg(GNA_IN, "S182_Mul_shift", 0), GNodeArg(GNA_IN, "S182_Infos", 0)));
    // Node S185_Conv2d_128x64x3x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S185_Conv2d_128x64x3x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S182_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_e1fc21f0", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_493d8d4d", 0), GNodeArg(GNA_OUT, "S185_Output", 0), GNodeArg(GNA_IN, "S185_Mul_scale", 0), GNodeArg(GNA_IN, "S185_Mul_shift", 0), GNodeArg(GNA_IN, "S185_Infos", 0)));
    // Node S188_Conv2d_32x128x1x1_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 forced biasesq chan<(i32-0.00)*chan<chan
    AddNode("S188_Conv2d_32x128x1x1_Relu6", Bindings(7, GNodeArg(GNA_IN, "S185_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_4eb0e6ed", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_09dff4e3", 0), GNodeArg(GNA_OUT, "S188_Output", 0), GNodeArg(GNA_IN, "S188_Mul_scale", 0), GNodeArg(GNA_IN, "S188_Mul_shift", 0), GNodeArg(GNA_IN, "S188_Infos", 0)));
    // Node S191_Conv2d_64x32x1x3_Relu6 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -6.05<(i8-0.00)*0.04724409<6.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S191_Conv2d_64x32x1x3_Relu6", Bindings(7, GNodeArg(GNA_IN, "S188_Output", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_2875ae35", 0), GNodeArg(GNA_IN, "Featureextractormobilenetv2lay_66bcd31b", 0), GNodeArg(GNA_OUT, "S191_Output", 0), GNodeArg(GNA_IN, "S191_Mul_scale", 0), GNodeArg(GNA_IN, "S191_Mul_shift", 0), GNodeArg(GNA_IN, "S191_Infos", 0)));
    // Node S194_Conv2d_12x288x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -10.05<(i8-0.00)*0.07850172<9.97 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S194_Conv2d_12x288x1x1", Bindings(7, GNodeArg(GNA_IN, "S129_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_0boxencodingpredi_696b62c5", 0), GNodeArg(GNA_IN, "Boxpredictor_0boxencodingpredi_b48ea6a4", 0), GNodeArg(GNA_OUT, "S194_Output", 0), GNodeArg(GNA_IN, "S194_Mul_scale", 0), GNodeArg(GNA_IN, "S194_Mul_shift", 0), GNodeArg(GNA_IN, "S194_Infos", 0)));
    // Node CONV_2D_0_71_trans_out0 inq -10.05<(i8-0.00)*0.07850172<9.97 outq -10.05<(i8-0.00)*0.07850172<9.97
    AddNode("S195_Op_CONV_2D_0_71_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S194_Output", 0), GNodeArg(GNA_OUT, "S195_Output", 0)));
    // Node S199_Conv2d_9x288x1x1_Hsigmoid inq -6.05<(i8-0.00)*0.04724409<6.00 forced weightsq chan<(i8-0.00)*chan<chan outq -1.00<(i8-0.00)*0.00784326<1.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S199_Conv2d_9x288x1x1_Hsigmoid", Bindings(7, GNodeArg(GNA_IN, "S129_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_0classpredictorwe", 0), GNodeArg(GNA_IN, "Boxpredictor_0classpredictorco", 0), GNodeArg(GNA_OUT, "S199_Output", 0), GNodeArg(GNA_IN, "S199_Mul_scale", 0), GNodeArg(GNA_IN, "S199_Mul_shift", 0), GNodeArg(GNA_IN, "S199_Infos", 0)));
    // Node CONV_2D_0_73_trans_out0 inq -1.00<(i8-0.00)*0.00784326<1.00 outq -1.00<(i8-0.00)*0.00784326<1.00
    AddNode("S200_Op_CONV_2D_0_73_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S199_Output", 0), GNodeArg(GNA_OUT, "S200_Output", 0)));
    // Node S204_Conv2d_24x640x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -10.05<(i8-0.00)*0.07850172<9.97 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S204_Conv2d_24x640x1x1", Bindings(7, GNodeArg(GNA_IN, "S167_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_1boxencodingpredi_3dba6efc", 0), GNodeArg(GNA_IN, "Boxpredictor_1boxencodingpredi_2a41b47f", 0), GNodeArg(GNA_OUT, "S204_Output", 0), GNodeArg(GNA_IN, "S204_Mul_scale", 0), GNodeArg(GNA_IN, "S204_Mul_shift", 0), GNodeArg(GNA_IN, "S204_Infos", 0)));
    // Node CONV_2D_0_75_trans_out0 inq -10.05<(i8-0.00)*0.07850172<9.97 outq -10.05<(i8-0.00)*0.07850172<9.97
    AddNode("S205_Op_CONV_2D_0_75_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S204_Output", 0), GNodeArg(GNA_OUT, "S205_Output", 0)));
    // Node S209_Conv2d_18x640x1x1_Hsigmoid inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -1.00<(i8-0.00)*0.00784326<1.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S209_Conv2d_18x640x1x1_Hsigmoid", Bindings(7, GNodeArg(GNA_IN, "S167_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_1classpredictorwe", 0), GNodeArg(GNA_IN, "Boxpredictor_1classpredictorco", 0), GNodeArg(GNA_OUT, "S209_Output", 0), GNodeArg(GNA_IN, "S209_Mul_scale", 0), GNodeArg(GNA_IN, "S209_Mul_shift", 0), GNodeArg(GNA_IN, "S209_Infos", 0)));
    // Node CONV_2D_0_77_trans_out0 inq -1.00<(i8-0.00)*0.00784326<1.00 outq -1.00<(i8-0.00)*0.00784326<1.00
    AddNode("S210_Op_CONV_2D_0_77_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S209_Output", 0), GNodeArg(GNA_OUT, "S210_Output", 0)));
    // Node S214_Conv2d_24x256x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -10.05<(i8-0.00)*0.07850172<9.97 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S214_Conv2d_24x256x1x1", Bindings(7, GNodeArg(GNA_IN, "S173_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_2boxencodingpredi_aba037ce", 0), GNodeArg(GNA_IN, "Boxpredictor_2boxencodingpredi_25bcf53a", 0), GNodeArg(GNA_OUT, "S214_Output", 0), GNodeArg(GNA_IN, "S214_Mul_scale", 0), GNodeArg(GNA_IN, "S214_Mul_shift", 0), GNodeArg(GNA_IN, "S214_Infos", 0)));
    // Node CONV_2D_0_79_trans_out0 inq -10.05<(i8-0.00)*0.07850172<9.97 outq -10.05<(i8-0.00)*0.07850172<9.97
    AddNode("S215_Op_CONV_2D_0_79_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S214_Output", 0), GNodeArg(GNA_OUT, "S215_Output", 0)));
    // Node S219_Conv2d_18x256x1x1_Hsigmoid inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -1.00<(i8-0.00)*0.00784326<1.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S219_Conv2d_18x256x1x1_Hsigmoid", Bindings(7, GNodeArg(GNA_IN, "S173_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_2classpredictorwe", 0), GNodeArg(GNA_IN, "Boxpredictor_2classpredictorco", 0), GNodeArg(GNA_OUT, "S219_Output", 0), GNodeArg(GNA_IN, "S219_Mul_scale", 0), GNodeArg(GNA_IN, "S219_Mul_shift", 0), GNodeArg(GNA_IN, "S219_Infos", 0)));
    // Node CONV_2D_0_81_trans_out0 inq -1.00<(i8-0.00)*0.00784326<1.00 outq -1.00<(i8-0.00)*0.00784326<1.00
    AddNode("S220_Op_CONV_2D_0_81_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S219_Output", 0), GNodeArg(GNA_OUT, "S220_Output", 0)));
    // Node S224_Conv2d_24x128x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -10.05<(i8-0.00)*0.07850172<9.97 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S224_Conv2d_24x128x1x1", Bindings(7, GNodeArg(GNA_IN, "S179_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_3boxencodingpredi_21bd6b57", 0), GNodeArg(GNA_IN, "Boxpredictor_3boxencodingpredi_c12a614a", 0), GNodeArg(GNA_OUT, "S224_Output", 0), GNodeArg(GNA_IN, "S224_Mul_scale", 0), GNodeArg(GNA_IN, "S224_Mul_shift", 0), GNodeArg(GNA_IN, "S224_Infos", 0)));
    // Node CONV_2D_0_83_trans_out0 inq -10.05<(i8-0.00)*0.07850172<9.97 outq -10.05<(i8-0.00)*0.07850172<9.97
    AddNode("S225_Op_CONV_2D_0_83_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S224_Output", 0), GNodeArg(GNA_OUT, "S225_Output", 0)));
    // Node S229_Conv2d_18x128x1x1_Hsigmoid inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -1.00<(i8-0.00)*0.00784326<1.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S229_Conv2d_18x128x1x1_Hsigmoid", Bindings(7, GNodeArg(GNA_IN, "S179_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_3classpredictorwe", 0), GNodeArg(GNA_IN, "Boxpredictor_3classpredictorco", 0), GNodeArg(GNA_OUT, "S229_Output", 0), GNodeArg(GNA_IN, "S229_Mul_scale", 0), GNodeArg(GNA_IN, "S229_Mul_shift", 0), GNodeArg(GNA_IN, "S229_Infos", 0)));
    // Node CONV_2D_0_85_trans_out0 inq -1.00<(i8-0.00)*0.00784326<1.00 outq -1.00<(i8-0.00)*0.00784326<1.00
    AddNode("S230_Op_CONV_2D_0_85_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S229_Output", 0), GNodeArg(GNA_OUT, "S230_Output", 0)));
    // Node S234_Conv2d_24x128x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -10.05<(i8-0.00)*0.07850172<9.97 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S234_Conv2d_24x128x1x1", Bindings(7, GNodeArg(GNA_IN, "S185_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_4boxencodingpredi_80b21d17", 0), GNodeArg(GNA_IN, "Boxpredictor_4boxencodingpredi_be9e9917", 0), GNodeArg(GNA_OUT, "S234_Output", 0), GNodeArg(GNA_IN, "S234_Mul_scale", 0), GNodeArg(GNA_IN, "S234_Mul_shift", 0), GNodeArg(GNA_IN, "S234_Infos", 0)));
    // Node CONV_2D_0_87_trans_out0 inq -10.05<(i8-0.00)*0.07850172<9.97 outq -10.05<(i8-0.00)*0.07850172<9.97
    AddNode("S235_Op_CONV_2D_0_87_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S234_Output", 0), GNodeArg(GNA_OUT, "S235_Output", 0)));
    // Node S239_Conv2d_18x128x1x1_Hsigmoid inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -1.00<(i8-0.00)*0.00784326<1.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S239_Conv2d_18x128x1x1_Hsigmoid", Bindings(7, GNodeArg(GNA_IN, "S185_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_4classpredictorwe", 0), GNodeArg(GNA_IN, "Boxpredictor_4classpredictorco", 0), GNodeArg(GNA_OUT, "S239_Output", 0), GNodeArg(GNA_IN, "S239_Mul_scale", 0), GNodeArg(GNA_IN, "S239_Mul_shift", 0), GNodeArg(GNA_IN, "S239_Infos", 0)));
    // Node CONV_2D_0_89_trans_out0 inq -1.00<(i8-0.00)*0.00784326<1.00 outq -1.00<(i8-0.00)*0.00784326<1.00
    AddNode("S240_Op_CONV_2D_0_89_trans_out0", Bindings(2, GNodeArg(GNA_IN, "S239_Output", 0), GNodeArg(GNA_OUT, "S240_Output", 0)));
    // Node S244_Conv2d_24x64x1x1 inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -10.05<(i8-0.00)*0.07850172<9.97 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S244_Conv2d_24x64x1x1", Bindings(7, GNodeArg(GNA_IN, "S191_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_5boxencodingpredi_6beccbdb", 0), GNodeArg(GNA_IN, "Boxpredictor_5boxencodingpredi_6990bed4", 0), GNodeArg(GNA_OUT, "S244_Output", 0), GNodeArg(GNA_IN, "S244_Mul_scale", 0), GNodeArg(GNA_IN, "S244_Mul_shift", 0), GNodeArg(GNA_IN, "S244_Infos", 0)));
    // Node S250_Conv2d_18x64x1x1_Hsigmoid inq -6.05<(i8-0.00)*0.04724409<6.00 weightsq chan<(i8-0.00)*chan<chan outq -1.00<(i8-0.00)*0.00784326<1.00 biasesq chan<(i32-0.00)*chan<chan
    AddNode("S250_Conv2d_18x64x1x1_Hsigmoid", Bindings(7, GNodeArg(GNA_IN, "S191_Output", 0), GNodeArg(GNA_IN, "Boxpredictor_5classpredictorwe", 0), GNodeArg(GNA_IN, "Boxpredictor_5classpredictorco", 0), GNodeArg(GNA_OUT, "S250_Output", 0), GNodeArg(GNA_IN, "S250_Mul_scale", 0), GNodeArg(GNA_IN, "S250_Mul_shift", 0), GNodeArg(GNA_IN, "S250_Infos", 0)));
    // Node CUSTOM_0_99 offsetsq -10.05<(i8-0.00)*0.07850172<9.97 scoresq -1.00<(i8-0.00)*0.00784326<1.00 anchorsq -1.66<(i8-0.00)*0.01295693<1.65 forced outboxesq -2.00<(i16-0.00)*0.00006104<2.00
    AddNode("S254_Op_CUSTOM_0_99", Bindings(8, GNodeArg(GNA_IN, "S246_Output", 0), GNodeArg(GNA_IN, "S252_Output", 0), GNodeArg(GNA_IN, "Anchors", 0), GNodeArg(GNA_OUT, "Output_1", 0), GNodeArg(GNA_OUT, "Output_2", 0), GNodeArg(GNA_OUT, "Output_3", 0), GNodeArg(GNA_IN, "S254_Ssd_scales", 0), GNodeArg(GNA_IN, "S254_Ssd_norms", 0)));
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
