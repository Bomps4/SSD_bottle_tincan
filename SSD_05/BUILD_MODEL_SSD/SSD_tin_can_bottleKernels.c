#include "SSD_tin_can_bottleKernels.h"
L1_CL_MEM AT_L1_POINTER SSD_tin_can_bottle_L1_Memory;
L2_MEM AT_L2_POINTER SSD_tin_can_bottle_L2_Memory;
AT_HYPERRAM_POINTER SSD_tin_can_bottle_L3_Memory;
extern AT_HYPERRAM_T HyperRam;
static AT_HYPERFLASH_FS_T HyperFlash;
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S3_Conv2d_16x3x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 40860 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERRAM_CL_EVENT UchanHR2;
	KerSetBias_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerConvLinReduct_SQ8_T S_KerArg2, *KerArg2 = &S_KerArg2;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Last;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast, T0Ind_NextNextLast;
	int D0Ind, D0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	unsigned int _NN_In;
	unsigned int _SN_In, _SNN_In;
	unsigned int _LN_In, _LNN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: Init: 16, Tiled: 1][Tile0 Dim: 60][D0 Dim: Init: 3, Tiled: 1]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 60 logical tiles, 60 physical tiles
			Total Size: 307200 [D1, [0 x 307200, 307200]][Tile0, 60:[160x2, 58:160x2, 160x2], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 307200, 307200]][Tile0, 60:[160x2, 58:160x2, 160x2], 1]
		Tile0: [0, 5120, 320], Tile1: [320, 5120, 320], Tile2; [640, 5120, 320]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [D1, [0 x 64, 64]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 64, 64]]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 16 [D1, [0 x 16, 16]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 16, 16]]
		Tile0: [0, 16, 16], Tile1: [0, 16, 16], Tile2; [0, 16, 16]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 16 [D1, [0 x 16, 16]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 16, 16]]
		Tile0: [0, 16, 16], Tile1: [0, 16, 16], Tile2; [0, 16, 16]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 432 [D1, [0 x 432, 432]][D0, [0 x 432, 432]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 432, 432]][D0, [0 x 432, 432]]
		Tile0: [0, 432, 432], Tile1: [0, 432, 432], Tile2; [0, 432, 432]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 60 logical tiles, 60 physical tiles
			Total Size: 230400 [D0, [0 x 230400, 230400]][Tile0, 60:[320x5, 58:320x5, 320x4], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 60:[320x4], 1][D0, [0 x 230400, 230400]]
		Tile0: [0, 4800, 1600], Tile1: [1280, 4800, 1600], Tile2; [2560, 4800, 1600]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 60 logical tiles, 1 physical tiles
			Total Size: 1228800 [D1, [0 x 1228800, 1228800]][Tile0, 60:[160x2, 58:160x2, 160x2], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 1228800, 1228800]][Tile0, 60:[160x2, 58:160x2, 160x2], 4]
		Tile0: [0, 20480, 1280], Tile1: [0, 20480, 1280], Tile2; [0, 20480, 1280]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 60 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 60:[9x1, 58:9x1, 9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 60:[9x1, 58:9x1, 9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+20368);
	KerArg0->W = (unsigned short int) (160);
	KerArg0->H = (unsigned short int) (2);
	KerArg0->Feat = (unsigned short int) (16);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+9600);
	KerArg1->W = (unsigned short int) (320);
	KerArg1->UsedW = (unsigned short int) (320);
	KerArg1->InFeatures = (unsigned short int) (3);
	KerArg1->OutFeatures = (unsigned short int) (16);
	KerArg1->TotalInFeatures = (unsigned short int) (3);
	KerArg1->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+9696);
	KerArg1->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+20368);
	KerArg2->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+20368);
	KerArg2->Feat = (unsigned short int) (16);
	KerArg2->W = (unsigned short int) (160);
	KerArg2->H = (unsigned short int) (2);
	KerArg2->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+9664);
	KerArg2->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+9680);
	KerArg2->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+40848);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=5120; _LC_Out=320;
	_SPP_Out=0; _SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+9600), 64, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+9664), 16, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+9680), 16, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Filter+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+43232+0), 432, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Filter */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+43232+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+9696), 432, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33632+0), 4800, 76800, 1600, 0, &UchanHR2);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait previous uDMA read In */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+1280), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33632+4800), 4800, 76800, 1600, 0, &UchanHR2);
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33632+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 4800, 0, &DmaR_Evt5);
	_NN_In=1280; _SN_In=4800;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+40848), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D1 */
		int D1Ind_Last = 1;
		for (T0Ind=0; T0Ind<60; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==59), T0Ind_NextLast = ((T0Ind+1)==59), T0Ind_NextNextLast = ((T0Ind+2)==59);
			/*================================= Prepare Tiles ===================================*/
			_SNN_In = 0;
			if (!(T0Ind_Last)) {
				if (!(T0Ind_NextLast)) {
					_NN_In = _NN_In + (1280); _LNN_In = ((T0Ind_NextNextLast)?1280:1600); _SNN_In = (3*_LNN_In); 
				} else if (!(1)) {
					_NN_In = _NN_In + (-75520); _LNN_In = (1600); _SNN_In = (3*_LNN_In); 
				}
			} else if (!((1))) {
				_NN_In = _NN_In + (1280); _LNN_In = (1600); _SNN_In = (3*_LNN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait previous uDMA read In */
			if (_SNN_In) {
				AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+_NN_In), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33632+4800*((T0Ind_Total)%2)),
						_SNN_In, 76800, _LNN_In, 0, &UchanHR2);
			}
			AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33632+4800*((T0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+4800*((T0Ind_Total+1)%2)),
						_SN_In, 0, &DmaR_Evt5);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+40848))[5]);
			AT_FORK(gap_ncore(), (void *) KerParSetBiasB32_SQ8, (void *) KerArg0);
			__CALL(KerParSetBiasB32_SQ8, KerArg0);
			{ /* Single iteration on D0 */
				int D0Ind_Last = 1;
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+4800*((T0Ind_Total)%2));
				KerArg1->H = (unsigned short int) (5-0*(T0Ind==0)-1*(T0Ind_Last));
				KerArg1->UsedH = (unsigned short int) (5-0*(T0Ind==0)-1*(T0Ind_Last));
				KerArg1->Pad = (v4s) ((v4s){0,1,0*(T0Ind==0),1*(T0Ind_Last)});
				AT_FORK(gap_ncore(), (void *) KerParConv3x3Stride2_SQ8, (void *) KerArg1);
				__CALL(KerParConv3x3Stride2_SQ8, KerArg1);
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			KerArg2->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+10128+5120*((T0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg2);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg2);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR3); /* Wait previous uDMA write Out */
			if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+5120*((T0Ind_Total+-1)%2)),
						_SP_Out, 19200, _LP_Out, 1, &UchanHR3);
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+5120*((T0Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10128+5120*((T0Ind_Total)%2)),
					_SC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
			_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			_SN_In = _SNN_In;_LN_In = _LNN_In;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (320); _LC_Out = (320); _SC_Out = (16*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR3); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+5120*((T0Ind_Total+-1)%2)), _SP_Out, 19200, _LP_Out, 1, &UchanHR3);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR3); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S6_Conv2d_16x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 46332 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	AT_HYPERRAM_CL_EVENT UchanHR2;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last, D0Ind_NextLast, D0Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast, T0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _NN_In;
	unsigned int _SN_In, _SNN_In;
	unsigned int _LN_In, _LNN_In;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 16, Tiled: 2][Tile0 Dim: 30]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 60 logical tiles, 60 physical tiles
			Total Size: 307200 [D0, [1 x 153600, 153600]][Tile0, 30:[160x5, 28:160x6, 160x5], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [1 x 153600, 153600]][Tile0, 30:[160x5, 28:160x6, 160x5], 1]
		Tile0: [0, 6400, 800], Tile1: [480, 7680, 960], Tile2; [1120, 7680, 960]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 64 [D0, [1 x 32, 32]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [1 x 32, 32]]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 16 [D0, [1 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [1 x 8, 8]]
		Tile0: [0, 16, 16], Tile1: [0, 16, 16], Tile2; [0, 16, 16]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 16 [D0, [1 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [1 x 8, 8]]
		Tile0: [0, 16, 16], Tile1: [0, 16, 16], Tile2; [0, 16, 16]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 144 [D0, [1 x 72, 72]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [1 x 72, 72]]
		Tile0: [0, 144, 144], Tile1: [0, 144, 144], Tile2; [0, 144, 144]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 60 logical tiles, 60 physical tiles
			Total Size: 307200 [D0, [1 x 153600, 153600]][Tile0, 30:[160x4, 28:160x4, 160x4], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [1 x 153600, 153600]][Tile0, 30:[160x4, 28:160x4, 160x4], 1]
		Tile0: [0, 5120, 640], Tile1: [640, 5120, 640], Tile2; [1280, 5120, 640]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 60 logical tiles, 1 physical tiles
			Total Size: 1228800 [D0, [1 x 614400, 614400]][Tile0, 30:[160x4, 28:160x4, 160x4], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [1 x 614400, 614400]][Tile0, 30:[160x4, 28:160x4, 160x4], 4]
		Tile0: [0, 20480, 2560], Tile1: [0, 20480, 2560], Tile2; [0, 20480, 2560]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 30 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 30:[9x1, 28:9x1, 9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 30:[9x1, 28:9x1, 9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (160);
	KerArg0->UsedW = (unsigned short int) (160);
	KerArg0->InFeatures = (unsigned short int) (8);
	KerArg0->OutFeatures = (unsigned short int) (8);
	KerArg0->TotalInFeatures = (unsigned short int) (16);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+25840);
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+25840);
	KerArg1->Feat = (unsigned short int) (8);
	KerArg1->W = (unsigned short int) (160);
	KerArg1->H = (unsigned short int) (4);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+46320);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33632+0), 6400, 19200, 800, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+480), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33632+7680), 7680, 19200, 960, 0, &UchanHR1);
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33632+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 6400, 0, &DmaR_Evt1);
	_NN_In=480; _SN_In=7680;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+15360), 64, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+15424), 16, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+15440), 16, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+15456), 144, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
	_C_Out=0; _SC_Out=5120; _LC_Out=640;
	_SPP_Out=0; _SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+46320), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<2; D0Ind++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==1), D0Ind_NextLast = ((D0Ind+1)==1), D0Ind_NextNextLast = ((D0Ind+2)==1);
		for (T0Ind=0; T0Ind<30; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==29), T0Ind_NextLast = ((T0Ind+1)==29), T0Ind_NextNextLast = ((T0Ind+2)==29);
			/*================================= Prepare Tiles ===================================*/
			_SNN_In = 0;
			if (!(T0Ind_Last)) {
				if (!(T0Ind_NextLast)) {
					_NN_In = _NN_In + (640); _LNN_In = ((T0Ind_NextNextLast)?800:960); _SNN_In = (8*_LNN_In); 
				} else if (!(D0Ind_Last)) {
					_NN_In = _NN_In + (153600)+(-18400); _LNN_In = (800); _SNN_In = (8*_LNN_In); 
				}
			} else if (!(D0Ind_Last)) {
				_NN_In = _NN_In + (640-160); _LNN_In = (960); _SNN_In = (8*_LNN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In */
			if (_SNN_In) {
				AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+_NN_In), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33632+7680*((T0Ind_Total)%2)),
						_SNN_In, 19200, _LNN_In, 0, &UchanHR1);
			}
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33632+7680*((T0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+7680*((T0Ind_Total+1)%2)),
						_SN_In, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+7680*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (6-1*(T0Ind==0)-1*(T0Ind_Last));
			KerArg0->UsedH = (unsigned short int) (6-1*(T0Ind==0)-1*(T0Ind_Last));
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+15456+((D0Ind)*72));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+15360+((D0Ind)*32));
			KerArg0->Pad = (v4s) ((v4s){1,1,1*(T0Ind==0),1*(T0Ind_Last)});
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+46320))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride1B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride1B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+15600+5120*((T0Ind_Total)%2));
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+15424+((D0Ind)*8));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+15440+((D0Ind)*8));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait previous uDMA write Out */
			if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+5120*((T0Ind_Total+-1)%2)),
						_SP_Out, 19200, _LP_Out, 1, &UchanHR2);
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+5120*((T0Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+15600+5120*((T0Ind_Total)%2)),
					_SC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SN_In = _SNN_In;_LN_In = _LNN_In;
			_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
			_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (640); _LC_Out = (640); _SC_Out = (8*_LC_Out); 
			} else if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (153600)+(-18560); _LC_Out = (640); _SC_Out = (8*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+5120*((T0Ind_Total+-1)%2)), _SP_Out, 19200, _LP_Out, 1, &UchanHR2);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S9_Conv2d_16x16x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 52204 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	AT_HYPERRAM_CL_EVENT UchanHR2;
	KerMatTranspose_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerMatMul_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast, T0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _NN_In2;
	unsigned int _SN_In2, _SNN_In2;
	unsigned int _LN_In2, _LNN_In2;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 30]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 30 logical tiles, 30 physical tiles
			Total Size: 307200 [Tile0, 30:[16x648, 28:16x648, 16x408], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 30:[16x648, 28:16x648, 16x408], 1]
		Tile0: [0, 10368, 648], Tile1: [648, 10368, 648], Tile2; [1296, 10368, 648]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 30 logical tiles, 1 physical tiles
			Total Size: 256 [Tile0, 30:[16x16, 28:16x16, 16x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 30:[16x16, 28:16x16, 16x16], 1]
		Tile0: [0, 256, 256], Tile1: [0, 256, 256], Tile2; [0, 256, 256]
	Ker Arg: TransIn2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 30 logical tiles, 1 physical tiles
			Total Size: 307200 [Tile0, 30:[16x648, 28:16x648, 16x408], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 30:[16x648, 28:16x648, 16x408], 1]
		Tile0: [0, 10368, 648], Tile1: [0, 10368, 648], Tile2; [0, 10368, 648]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 30 logical tiles, 1 physical tiles
			Total Size: 64 [Tile0, 30:[16x1, 28:16x1, 16x1], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 30:[16x1, 28:16x1, 16x1], 4]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 30 logical tiles, 30 physical tiles
			Total Size: 307200 [Tile0, 30:[16x648, 28:16x648, 16x408], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 30:[16x648, 28:16x648, 16x408], 1]
		Tile0: [0, 10368, 648], Tile1: [648, 10368, 648], Tile2; [1296, 10368, 648]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 30 logical tiles, 1 physical tiles
			Total Size: 16 [Tile0, 30:[16x1, 28:16x1, 16x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 30:[16x1, 28:16x1, 16x1], 1]
		Tile0: [0, 16, 16], Tile1: [0, 16, 16], Tile2; [0, 16, 16]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 30 logical tiles, 1 physical tiles
			Total Size: 16 [Tile0, 30:[16x1, 28:16x1, 16x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 30:[16x1, 28:16x1, 16x1], 1]
		Tile0: [0, 16, 16], Tile1: [0, 16, 16], Tile2; [0, 16, 16]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 30 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 30:[1x1, 28:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 30:[1x1, 28:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+20992);
	KerArg0->Feat = (unsigned short int) (1);
	KerArg0->H = (unsigned short int) (16);
	KerArg1->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg1->W_In1 = (unsigned short int) (16);
	KerArg1->H_In1 = (unsigned short int) (16);
	KerArg1->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+20992);
	KerArg1->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+31360);
	KerArg1->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+52160);
	KerArg1->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+52176);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+52192);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+0), 10368, 19200, 648, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+648), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+10368), 10368, 19200, 648, 0, &UchanHR1);
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+256+0), 10368, 0, &DmaR_Evt1);
	_NN_In2=648; _SN_In2=10368;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 256, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+31360), 64, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=10368; _LC_Out=648;
	_SPP_Out=0; _SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+52160), 16, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+52176), 16, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+52192), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T0Ind=0; T0Ind<30; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
		int T0Ind_Last = (T0Ind==29), T0Ind_NextLast = ((T0Ind+1)==29), T0Ind_NextNextLast = ((T0Ind+2)==29);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In2 = 0;
		if (!(T0Ind_Last)) {
			if (!(T0Ind_NextLast)) {
				_NN_In2 = _NN_In2 + (648); _LNN_In2 = ((T0Ind_NextNextLast)?408:648); _SNN_In2 = (16*_LNN_In2); 
			}
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
		if (_SNN_In2) {
			AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+_NN_In2), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+10368*((T0Ind_Total)%2)),
					_SNN_In2, 19200, _LNN_In2, 0, &UchanHR1);
		}
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+10368*((T0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+256+10368*((T0Ind_Total+1)%2)),
					_SN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		/*====================== Call Kernel LOC_LOOP =========================*/
		KerArg0->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+256+10368*((T0Ind_Total)%2));
		KerArg0->W = (unsigned short int) ((T0Ind_Last)?408:648);
		AT_FORK(gap_ncore(), (void *) CNN_Transpose_fps, (void *) KerArg0);
		__CALL(CNN_Transpose_fps, KerArg0);
		KerArg1->W_In2 = (unsigned short int) ((T0Ind_Last)?408:648);
		KerArg1->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+31424+10368*((T0Ind_Total)%2));
		KerArg1->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+52192))[5]);
		AT_FORK(gap_ncore(), (void *) KerParMatMulB32_SF_SQ8, (void *) KerArg1);
		__CALL(KerParMatMulB32_SF_SQ8, KerArg1);
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait previous uDMA write Out */
		if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+44128+10368*((T0Ind_Total+-1)%2)),
					_SP_Out, 19200, _LP_Out, 1, &UchanHR2);
		AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+44128+10368*((T0Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+31424+10368*((T0Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SN_In2 = _SNN_In2;_LN_In2 = _LNN_In2;
		_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
		_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T0Ind_Last)) {
			_C_Out = _C_Out + (648); _LC_Out = ((T0Ind_NextLast)?408:648); _SC_Out = (16*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+44128+10368*((T0Ind_Total+-1)%2)), _SP_Out, 19200, _LP_Out, 1, &UchanHR2);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S10_MatAdd_16x120x160(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 52716 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	AT_HYPERRAM_CL_EVENT UchanHR2;
	AT_HYPERRAM_CL_EVENT UchanHR3;
	KerMat3_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast, T0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _NN_In1;
	unsigned int _SN_In1, _SNN_In1;
	unsigned int _NN_In2;
	unsigned int _SN_In2, _SNN_In2;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 35]
	Ker Arg: In1, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 35 logical tiles, 35 physical tiles
			Total Size: 307200 [Tile0, 35:[1x8784, 33:1x8784, 1x8544], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 35:[1x8784, 33:1x8784, 1x8544], 1]
		Tile0: [0, 8784, 8784], Tile1: [8784, 8784, 8784], Tile2; [17568, 8784, 8784]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 35 logical tiles, 35 physical tiles
			Total Size: 307200 [Tile0, 35:[1x8784, 33:1x8784, 1x8544], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 35:[1x8784, 33:1x8784, 1x8544], 1]
		Tile0: [0, 8784, 8784], Tile1: [8784, 8784, 8784], Tile2; [17568, 8784, 8784]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 35 logical tiles, 35 physical tiles
			Total Size: 307200 [Tile0, 35:[1x8784, 33:1x8784, 1x8544], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 35:[1x8784, 33:1x8784, 1x8544], 1]
		Tile0: [0, 8784, 8784], Tile1: [8784, 8784, 8784], Tile2; [17568, 8784, 8784]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 35 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 35:[1x1, 33:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 35:[1x1, 33:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->DoScale = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+52704);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+40960+0), 8784, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+8784), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+40960+8784), 8784, 0, &UchanHR1);
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+40960+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 8784, 0, &DmaR_Evt1);
	_NN_In1=8784; _SN_In1=8784;
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+58528+0), 8784, 0, &UchanHR2);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait previous uDMA read In2 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+8784), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+58528+8784), 8784, 0, &UchanHR2);
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+58528+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+17568+0), 8784, 0, &DmaR_Evt2);
	_NN_In2=8784; _SN_In2=8784;
	_C_Out=0; _SC_Out=8784;
	_SPP_Out=0; _SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+52704), 9, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T0Ind=0; T0Ind<35; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
		int T0Ind_Last = (T0Ind==34), T0Ind_NextLast = ((T0Ind+1)==34), T0Ind_NextNextLast = ((T0Ind+2)==34);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In1 = 0;
		if (!(T0Ind_Last)) {
			if (!(T0Ind_NextLast)) {
				_NN_In1 = _NN_In1 + (8784); _SNN_In1 = ((T0Ind_NextNextLast)?8544:8784); 
			}
		}
		_SNN_In2 = 0;
		if (!(T0Ind_Last)) {
			if (!(T0Ind_NextLast)) {
				_NN_In2 = _NN_In2 + (8784); _SNN_In2 = ((T0Ind_NextNextLast)?8544:8784); 
			}
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In1 */
		if (_SNN_In1) {
			AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+_NN_In1), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+40960+8784*((T0Ind_Total)%2)),
					_SNN_In1, 0, &UchanHR1);
		}
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+40960+8784*((T0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+8784*((T0Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt1);
		}
		AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait previous uDMA read In2 */
		if (_SNN_In2) {
			AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+_NN_In2), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+58528+8784*((T0Ind_Total)%2)),
					_SNN_In2, 0, &UchanHR2);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+58528+8784*((T0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+17568+8784*((T0Ind_Total+1)%2)),
					_SN_In2, 0, &DmaR_Evt2);
		}
		/*============================= End Read Tiles ======================================*/
		/*====================== Call Kernel LOC_LOOP =========================*/
		KerArg0->In1 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0+8784*((T0Ind_Total)%2));
		KerArg0->In2 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+17568+8784*((T0Ind_Total)%2));
		KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+35136+8784*((T0Ind_Total)%2));
		KerArg0->Feat = (unsigned short int) (T0Ind_Last?8544:8784);
		AT_FORK(gap_ncore(), (void *) KerMatAdd_SQ8, (void *) KerArg0);
		__CALL(KerMatAdd_SQ8, KerArg0);
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR3); /* Wait previous uDMA write Out */
		if (_SP_Out) AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+8784*((T0Ind_Total+-1)%2)),
					_SP_Out, 1, &UchanHR3);
		AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+8784*((T0Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+35136+8784*((T0Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SN_In1 = _SNN_In1;
		_SN_In2 = _SNN_In2;
		_SPP_Out = _SP_Out;
		_P_Out = _C_Out;_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T0Ind_Last)) {
			_C_Out = _C_Out + (8784); _SC_Out = ((T0Ind_NextLast)?8544:8784); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR3); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+8784*((T0Ind_Total+-1)%2)), _SP_Out, 1, &UchanHR3);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR3); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S13_Conv2d_96x16x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50572 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	AT_HYPERRAM_CL_EVENT UchanHR2;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _NN_In2;
	unsigned int _SN_In2, _SNN_In2;
	unsigned int _LN_In2, _LNN_In2;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 89][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 89 logical tiles, 89 physical tiles
			Total Size: 307200 [Tile1, 89:[16x216, 87:16x216, 16x192], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 89:[16x216, 87:16x216, 16x192], 1]
		Tile0: [0, 3456, 216], Tile1: [216, 3456, 216], Tile2; [432, 3456, 216]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1536 [Tile0, 1:[16x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[16x96], 1]
		Tile0: [0, 1536, 1536], Tile1: [0, 1536, 1536], Tile2; [0, 1536, 1536]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 89 logical tiles, 1 physical tiles
			Total Size: 64 [Tile1, 89:[64x1, 87:64x1, 64x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 89:[64x1, 87:64x1, 64x1], 1]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 384 [Tile0, 1:[1x96], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 4]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 89 logical tiles, 89 physical tiles
			Total Size: 1843200 [Tile1, 89:[96x216, 87:96x216, 96x192], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 89:[96x216, 87:96x216, 96x192], 1]
		Tile0: [0, 20736, 216], Tile1: [216, 20736, 216], Tile2; [432, 20736, 216]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile0, 1:[1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 1]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile0, 1:[1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 1]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 89 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 89:[1x1, 87:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 89:[1x1, 87:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+48460);
	KerArg0->W_In1 = (unsigned short int) (16);
	KerArg0->H_In1 = (unsigned short int) (96);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+49996);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50380);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50476);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+48448);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+64864+0), 3456, 19200, 216, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+216), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+64864+3456), 3456, 19200, 216, 0, &UchanHR1);
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+64864+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+64+0), 3456, 0, &DmaR_Evt1);
	_NN_In2=216; _SN_In2=3456;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48460), 1536, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+49996), 384, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=20736; _LC_Out=216;
	_SPP_Out=0; _SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50380), 96, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50476), 96, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48448), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<89; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==88), T1Ind_NextLast = ((T1Ind+1)==88), T1Ind_NextNextLast = ((T1Ind+2)==88);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In2 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In2 = _NN_In2 + (216); _LNN_In2 = ((T1Ind_NextNextLast)?192:216); _SNN_In2 = (16*_LNN_In2); 
			}
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
		if (_SNN_In2) {
			AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+_NN_In2), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+64864+3456*((T1Ind_Total)%2)),
					_SNN_In2, 19200, _LNN_In2, 0, &UchanHR1);
		}
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+64864+3456*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+64+3456*((T1Ind_Total+1)%2)),
					_SN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+64+3456*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?192:216);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+6976+20736*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?192:216);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*96);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+48448))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait previous uDMA write Out */
		if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+20736*((T1Ind_Total+-1)%2)),
					_SP_Out, 19200, _LP_Out, 1, &UchanHR2);
		AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+20736*((T1Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6976+20736*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SN_In2 = _SNN_In2;_LN_In2 = _LNN_In2;
		_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
		_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (216); _LC_Out = ((T1Ind_NextLast)?192:216); _SC_Out = (96*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+20736*((T1Ind_Total+-1)%2)), _SP_Out, 19200, _LP_Out, 1, &UchanHR2);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S16_Conv2d_96x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 48812 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR2;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last, D0Ind_NextLast, D0Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast, T0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	unsigned int _NN_In;
	unsigned int _SN_In, _SNN_In;
	unsigned int _LN_In, _LNN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 96, Tiled: 12][Tile0 Dim: 12]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 144 logical tiles, 144 physical tiles
			Total Size: 460800 [D0, [11 x 38400, 38400]][Tile0, 12:[80x5, 10:80x5, 80x5], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 38400, 38400]][Tile0, 12:[80x5, 10:80x5, 80x5], 1]
		Tile0: [0, 3200, 400], Tile1: [400, 3200, 400], Tile2; [800, 3200, 400]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 384 [D0, [11 x 32, 32]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 32, 32]]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 96 [D0, [11 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 8, 8]]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 96 [D0, [11 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 8, 8]]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 864 [D0, [11 x 72, 72]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 72, 72]]
		Tile0: [0, 864, 864], Tile1: [0, 864, 864], Tile2; [0, 864, 864]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 144 logical tiles, 144 physical tiles
			Total Size: 1843200 [D0, [11 x 153600, 153600]][Tile0, 12:[160x11, 10:160x11, 160x10], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 153600, 153600]][Tile0, 12:[160x11, 10:160x11, 160x10], 1]
		Tile0: [0, 14080, 1760], Tile1: [1600, 14080, 1760], Tile2; [3200, 14080, 1760]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 144 logical tiles, 1 physical tiles
			Total Size: 1843200 [D0, [11 x 153600, 153600]][Tile0, 12:[80x5, 10:80x5, 80x5], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 153600, 153600]][Tile0, 12:[80x5, 10:80x5, 80x5], 4]
		Tile0: [0, 12800, 1600], Tile1: [0, 12800, 1600], Tile2; [0, 12800, 1600]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 12:[9x1, 10:9x1, 9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 12:[9x1, 10:9x1, 9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (160);
	KerArg0->UsedW = (unsigned short int) (160);
	KerArg0->InFeatures = (unsigned short int) (8);
	KerArg0->OutFeatures = (unsigned short int) (8);
	KerArg0->TotalInFeatures = (unsigned short int) (96);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+36000);
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+36000);
	KerArg1->Feat = (unsigned short int) (8);
	KerArg1->W = (unsigned short int) (80);
	KerArg1->H = (unsigned short int) (5);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+48800);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=3200; _LC_Out=400;
	_SPP_Out=0; _SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+28160), 384, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+28544), 96, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+28640), 96, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+28736), 864, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+29792+0), 14080, 19200, 1760, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+1600), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+29792+14080), 14080, 19200, 1760, 0, &UchanHR1);
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+29792+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 14080, 0, &DmaR_Evt5);
	_NN_In=1600; _SN_In=14080;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48800), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<12; D0Ind++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==11), D0Ind_NextLast = ((D0Ind+1)==11), D0Ind_NextNextLast = ((D0Ind+2)==11);
		for (T0Ind=0; T0Ind<12; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==11), T0Ind_NextLast = ((T0Ind+1)==11), T0Ind_NextNextLast = ((T0Ind+2)==11);
			/*================================= Prepare Tiles ===================================*/
			_SNN_In = 0;
			if (!(T0Ind_Last)) {
				if (!(T0Ind_NextLast)) {
					_NN_In = _NN_In + (1600); _LNN_In = ((T0Ind_NextNextLast)?1600:1760); _SNN_In = (8*_LNN_In); 
				} else if (!(D0Ind_Last)) {
					_NN_In = _NN_In + (153600)+(-17600); _LNN_In = (1760); _SNN_In = (8*_LNN_In); 
				}
			} else if (!(D0Ind_Last)) {
				_NN_In = _NN_In + (1600); _LNN_In = (1760); _SNN_In = (8*_LNN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In */
			if (_SNN_In) {
				AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+_NN_In), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+29792+14080*((T0Ind_Total)%2)),
						_SNN_In, 19200, _LNN_In, 0, &UchanHR1);
			}
			AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+29792+14080*((T0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+14080*((T0Ind_Total+1)%2)),
						_SN_In, 0, &DmaR_Evt5);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+14080*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (11-0*(T0Ind==0)-1*(T0Ind_Last));
			KerArg0->UsedH = (unsigned short int) (11-0*(T0Ind==0)-1*(T0Ind_Last));
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+28736+((D0Ind)*72));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+28160+((D0Ind)*32));
			KerArg0->Pad = (v4s) ((v4s){0,1,0*(T0Ind==0),1*(T0Ind_Last)});
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+48800))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride2B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride2B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+29600+3200*((T0Ind_Total)%2));
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+28544+((D0Ind)*8));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+28640+((D0Ind)*8));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait previous uDMA write Out */
			if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+3200*((T0Ind_Total+-1)%2)),
						_SP_Out, 4800, _LP_Out, 1, &UchanHR2);
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+3200*((T0Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+29600+3200*((T0Ind_Total)%2)),
					_SC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
			_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			_SN_In = _SNN_In;_LN_In = _LNN_In;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (400); _LC_Out = (400); _SC_Out = (8*_LC_Out); 
			} else if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (38400)+(-4400); _LC_Out = (400); _SC_Out = (8*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+3200*((T0Ind_Total+-1)%2)), _SP_Out, 4800, _LP_Out, 1, &UchanHR2);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S19_Conv2d_16x96x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50412 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _NN_In2;
	unsigned int _SN_In2, _SNN_In2;
	unsigned int _LN_In2, _LNN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 23][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 23 logical tiles, 23 physical tiles
			Total Size: 460800 [Tile1, 23:[96x216, 21:96x216, 96x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 23:[96x216, 21:96x216, 96x48], 1]
		Tile0: [0, 20736, 216], Tile1: [216, 20736, 216], Tile2; [432, 20736, 216]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1536 [Tile0, 1:[96x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[96x16], 1]
		Tile0: [0, 1536, 1536], Tile1: [0, 1536, 1536], Tile2; [0, 1536, 1536]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 23 logical tiles, 1 physical tiles
			Total Size: 384 [Tile1, 23:[384x1, 21:384x1, 384x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 23:[384x1, 21:384x1, 384x1], 1]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile0, 1:[1x16], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x16], 4]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 23 logical tiles, 23 physical tiles
			Total Size: 76800 [Tile1, 23:[16x216, 21:16x216, 16x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 23:[16x216, 21:16x216, 16x48], 1]
		Tile0: [0, 3456, 216], Tile1: [216, 3456, 216], Tile2; [432, 3456, 216]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 16 [Tile0, 1:[1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x16], 1]
		Tile0: [0, 16, 16], Tile1: [0, 16, 16], Tile2; [0, 16, 16]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 16 [Tile0, 1:[1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x16], 1]
		Tile0: [0, 16, 16], Tile1: [0, 16, 16], Tile2; [0, 16, 16]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 23 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 23:[1x1, 21:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 23:[1x1, 21:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+48780);
	KerArg0->W_In1 = (unsigned short int) (96);
	KerArg0->H_In1 = (unsigned short int) (16);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+50316);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50380);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50396);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+48768);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+0), 20736, 4800, 216, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+216), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+20736), 20736, 4800, 216, 0, &UchanHR1);
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+384+0), 20736, 0, &DmaR_Evt1);
	_NN_In2=216; _SN_In2=20736;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48780), 1536, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50316), 64, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=3456; _LC_Out=216;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50380), 16, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50396), 16, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48768), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<23; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==22), T1Ind_NextLast = ((T1Ind+1)==22), T1Ind_NextNextLast = ((T1Ind+2)==22);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In2 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In2 = _NN_In2 + (216); _LNN_In2 = ((T1Ind_NextNextLast)?48:216); _SNN_In2 = (96*_LNN_In2); 
			}
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
		if (_SNN_In2) {
			AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+_NN_In2), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+20736*((T1Ind_Total)%2)),
					_SNN_In2, 4800, _LNN_In2, 0, &UchanHR1);
		}
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+20736*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+384+20736*((T1Ind_Total+1)%2)),
					_SN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+384+20736*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?48:216);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+41856+3456*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?48:216);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*16);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+48768))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+41856+3456*((T1Ind_Total)%2)),
				_SC_Out, 4800, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SN_In2 = _SNN_In2;_LN_In2 = _LNN_In2;
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (216); _LC_Out = ((T1Ind_NextLast)?48:216); _SC_Out = (16*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S22_Conv2d_96x16x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50572 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 23][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 23 logical tiles, 23 physical tiles
			Total Size: 76800 [Tile1, 23:[16x216, 21:16x216, 16x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 23:[16x216, 21:16x216, 16x48], 1]
		Tile0: [0, 3456, 216], Tile1: [216, 3456, 216], Tile2; [432, 3456, 216]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1536 [Tile0, 1:[16x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[16x96], 1]
		Tile0: [0, 1536, 1536], Tile1: [0, 1536, 1536], Tile2; [0, 1536, 1536]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 23 logical tiles, 1 physical tiles
			Total Size: 64 [Tile1, 23:[64x1, 21:64x1, 64x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 23:[64x1, 21:64x1, 64x1], 1]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 384 [Tile0, 1:[1x96], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 4]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 23 logical tiles, 23 physical tiles
			Total Size: 460800 [Tile1, 23:[96x216, 21:96x216, 96x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 23:[96x216, 21:96x216, 96x48], 1]
		Tile0: [0, 20736, 216], Tile1: [216, 20736, 216], Tile2; [432, 20736, 216]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile0, 1:[1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 1]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile0, 1:[1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 1]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 23 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 23:[1x1, 21:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 23:[1x1, 21:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+48460);
	KerArg0->W_In1 = (unsigned short int) (16);
	KerArg0->H_In1 = (unsigned short int) (96);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+49996);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50380);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50476);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+48448);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+64+0), 3456, 4800, 216, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48460), 1536, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+49996), 384, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=20736; _LC_Out=216;
	_SPP_Out=0; _SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50380), 96, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50476), 96, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48448), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<23; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==22), T1Ind_NextLast = ((T1Ind+1)==22);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (216); _LN_In2 = ((T1Ind_NextLast)?48:216); _SN_In2 = (16*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+64+3456*((T1Ind_Total+1)%2)),
					_SN_In2, 4800, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+64+3456*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?48:216);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+6976+20736*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?48:216);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*96);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+48448))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA write Out */
		if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+20736*((T1Ind_Total+-1)%2)),
					_SP_Out, 4800, _LP_Out, 1, &UchanHR1);
		AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+20736*((T1Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6976+20736*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
		_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (216); _LC_Out = ((T1Ind_NextLast)?48:216); _SC_Out = (96*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+20736*((T1Ind_Total+-1)%2)), _SP_Out, 4800, _LP_Out, 1, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S25_Conv2d_96x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50092 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	AT_HYPERRAM_CL_EVENT UchanHR2;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last, D0Ind_NextLast, D0Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast, T0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _NN_In;
	unsigned int _SN_In, _SNN_In;
	unsigned int _LN_In, _LNN_In;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 96, Tiled: 12][Tile0 Dim: 7]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 84 logical tiles, 84 physical tiles
			Total Size: 460800 [D0, [11 x 38400, 38400]][Tile0, 7:[80x10, 5:80x11, 80x7], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 38400, 38400]][Tile0, 7:[80x10, 5:80x11, 80x7], 1]
		Tile0: [0, 6400, 800], Tile1: [640, 7040, 880], Tile2; [1360, 7040, 880]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 384 [D0, [11 x 32, 32]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 32, 32]]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 96 [D0, [11 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 8, 8]]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 96 [D0, [11 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 8, 8]]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 864 [D0, [11 x 72, 72]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 72, 72]]
		Tile0: [0, 864, 864], Tile1: [0, 864, 864], Tile2; [0, 864, 864]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 84 logical tiles, 84 physical tiles
			Total Size: 460800 [D0, [11 x 38400, 38400]][Tile0, 7:[80x9, 5:80x9, 80x6], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 38400, 38400]][Tile0, 7:[80x9, 5:80x9, 80x6], 1]
		Tile0: [0, 5760, 720], Tile1: [720, 5760, 720], Tile2; [1440, 5760, 720]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 84 logical tiles, 1 physical tiles
			Total Size: 1843200 [D0, [11 x 153600, 153600]][Tile0, 7:[80x9, 5:80x9, 80x6], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 153600, 153600]][Tile0, 7:[80x9, 5:80x9, 80x6], 4]
		Tile0: [0, 23040, 2880], Tile1: [0, 23040, 2880], Tile2; [0, 23040, 2880]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 7:[9x1, 5:9x1, 9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 7:[9x1, 5:9x1, 9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (80);
	KerArg0->UsedW = (unsigned short int) (80);
	KerArg0->InFeatures = (unsigned short int) (8);
	KerArg0->OutFeatures = (unsigned short int) (8);
	KerArg0->TotalInFeatures = (unsigned short int) (96);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+27040);
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+27040);
	KerArg1->Feat = (unsigned short int) (8);
	KerArg1->W = (unsigned short int) (80);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+50080);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+0), 6400, 4800, 800, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+640), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+7040), 7040, 4800, 880, 0, &UchanHR1);
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 6400, 0, &DmaR_Evt1);
	_NN_In=640; _SN_In=7040;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+14080), 384, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+14464), 96, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+14560), 96, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+14656), 864, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
	_C_Out=0; _SC_Out=5760; _LC_Out=720;
	_SPP_Out=0; _SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50080), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<12; D0Ind++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==11), D0Ind_NextLast = ((D0Ind+1)==11), D0Ind_NextNextLast = ((D0Ind+2)==11);
		for (T0Ind=0; T0Ind<7; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==6), T0Ind_NextLast = ((T0Ind+1)==6), T0Ind_NextNextLast = ((T0Ind+2)==6);
			/*================================= Prepare Tiles ===================================*/
			_SNN_In = 0;
			if (!(T0Ind_Last)) {
				if (!(T0Ind_NextLast)) {
					_NN_In = _NN_In + (720); _LNN_In = ((T0Ind_NextNextLast)?560:880); _SNN_In = (8*_LNN_In); 
				} else if (!(D0Ind_Last)) {
					_NN_In = _NN_In + (38400)+(-4240); _LNN_In = (800); _SNN_In = (8*_LNN_In); 
				}
			} else if (!(D0Ind_Last)) {
				_NN_In = _NN_In + (720-80); _LNN_In = (880); _SNN_In = (8*_LNN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In */
			if (_SNN_In) {
				AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+_NN_In), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+7040*((T0Ind_Total)%2)),
						_SNN_In, 4800, _LNN_In, 0, &UchanHR1);
			}
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+7040*((T0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+7040*((T0Ind_Total+1)%2)),
						_SN_In, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+7040*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (((T0Ind_Last)?7:11)-1*(T0Ind==0));
			KerArg0->UsedH = (unsigned short int) (((T0Ind_Last)?7:11)-1*(T0Ind==0));
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+14656+((D0Ind)*72));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+14080+((D0Ind)*32));
			KerArg0->Pad = (v4s) ((v4s){1,1,1*(T0Ind==0),1*(T0Ind_Last)});
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+50080))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride1B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride1B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+15520+5760*((T0Ind_Total)%2));
			KerArg1->H = (unsigned short int) (T0Ind_Last?6:9);
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+14464+((D0Ind)*8));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+14560+((D0Ind)*8));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait previous uDMA write Out */
			if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37472+5760*((T0Ind_Total+-1)%2)),
						_SP_Out, 4800, _LP_Out, 1, &UchanHR2);
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37472+5760*((T0Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+15520+5760*((T0Ind_Total)%2)),
					_SC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SN_In = _SNN_In;_LN_In = _LNN_In;
			_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
			_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (720); _LC_Out = ((T0Ind_NextLast)?480:720); _SC_Out = (8*_LC_Out); 
			} else if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (38400)+(-4320); _LC_Out = (720); _SC_Out = (8*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37472+5760*((T0Ind_Total+-1)%2)), _SP_Out, 4800, _LP_Out, 1, &UchanHR2);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S28_Conv2d_16x96x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50412 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _NN_In2;
	unsigned int _SN_In2, _SNN_In2;
	unsigned int _LN_In2, _LNN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 23][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 23 logical tiles, 23 physical tiles
			Total Size: 460800 [Tile1, 23:[96x216, 21:96x216, 96x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 23:[96x216, 21:96x216, 96x48], 1]
		Tile0: [0, 20736, 216], Tile1: [216, 20736, 216], Tile2; [432, 20736, 216]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1536 [Tile0, 1:[96x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[96x16], 1]
		Tile0: [0, 1536, 1536], Tile1: [0, 1536, 1536], Tile2; [0, 1536, 1536]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 23 logical tiles, 1 physical tiles
			Total Size: 384 [Tile1, 23:[384x1, 21:384x1, 384x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 23:[384x1, 21:384x1, 384x1], 1]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile0, 1:[1x16], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x16], 4]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 23 logical tiles, 23 physical tiles
			Total Size: 76800 [Tile1, 23:[16x216, 21:16x216, 16x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 23:[16x216, 21:16x216, 16x48], 1]
		Tile0: [0, 3456, 216], Tile1: [216, 3456, 216], Tile2; [432, 3456, 216]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 16 [Tile0, 1:[1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x16], 1]
		Tile0: [0, 16, 16], Tile1: [0, 16, 16], Tile2; [0, 16, 16]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 16 [Tile0, 1:[1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x16], 1]
		Tile0: [0, 16, 16], Tile1: [0, 16, 16], Tile2; [0, 16, 16]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 23 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 23:[1x1, 21:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 23:[1x1, 21:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+48780);
	KerArg0->W_In1 = (unsigned short int) (96);
	KerArg0->H_In1 = (unsigned short int) (16);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+50316);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50380);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50396);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+48768);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+176992+0), 20736, 4800, 216, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+216), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+176992+20736), 20736, 4800, 216, 0, &UchanHR1);
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+176992+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+384+0), 20736, 0, &DmaR_Evt1);
	_NN_In2=216; _SN_In2=20736;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48780), 1536, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50316), 64, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=3456; _LC_Out=216;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50380), 16, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50396), 16, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48768), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<23; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==22), T1Ind_NextLast = ((T1Ind+1)==22), T1Ind_NextNextLast = ((T1Ind+2)==22);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In2 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In2 = _NN_In2 + (216); _LNN_In2 = ((T1Ind_NextNextLast)?48:216); _SNN_In2 = (96*_LNN_In2); 
			}
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
		if (_SNN_In2) {
			AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+_NN_In2), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+176992+20736*((T1Ind_Total)%2)),
					_SNN_In2, 4800, _LNN_In2, 0, &UchanHR1);
		}
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+176992+20736*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+384+20736*((T1Ind_Total+1)%2)),
					_SN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+384+20736*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?48:216);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+41856+3456*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?48:216);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*16);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+48768))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+41856+3456*((T1Ind_Total)%2)),
				_SC_Out, 4800, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SN_In2 = _SNN_In2;_LN_In2 = _LNN_In2;
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (216); _LC_Out = ((T1Ind_NextLast)?48:216); _SC_Out = (16*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S29_MatAdd_16x60x80(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 52716 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerMat3_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In1;
	unsigned int _SN_In1;
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 9]
	Ker Arg: In1, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 9 logical tiles, 9 physical tiles
			Total Size: 76800 [Tile0, 9:[1x8784, 7:1x8784, 1x6528], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 9:[1x8784, 7:1x8784, 1x6528], 1]
		Tile0: [0, 8784, 8784], Tile1: [8784, 8784, 8784], Tile2; [17568, 8784, 8784]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 9 logical tiles, 9 physical tiles
			Total Size: 76800 [Tile0, 9:[1x8784, 7:1x8784, 1x6528], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 9:[1x8784, 7:1x8784, 1x6528], 1]
		Tile0: [0, 8784, 8784], Tile1: [8784, 8784, 8784], Tile2; [17568, 8784, 8784]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 9 logical tiles, 9 physical tiles
			Total Size: 76800 [Tile0, 9:[1x8784, 7:1x8784, 1x6528], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 9:[1x8784, 7:1x8784, 1x6528], 1]
		Tile0: [0, 8784, 8784], Tile1: [8784, 8784, 8784], Tile2; [17568, 8784, 8784]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 9 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 9:[1x1, 7:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 9:[1x1, 7:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->DoScale = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+52704);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 8784, 0, &DmaR_Evt1);
	_N_In1=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+17568+0), 8784, 0, &DmaR_Evt2);
	_N_In2=0;
	_C_Out=0; _SC_Out=8784;
	_SPP_Out=0; _SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+196096+0), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+196096+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+52704), 9, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T0Ind=0; T0Ind<9; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
		int T0Ind_Last = (T0Ind==8), T0Ind_NextLast = ((T0Ind+1)==8);
		/*================================= Prepare Tiles ===================================*/
		_SN_In1 = 0;
		if (!(T0Ind_Last)) {
			_N_In1 = _N_In1 + (8784); _SN_In1 = ((T0Ind_NextLast)?6528:8784); 
		}
		_SN_In2 = 0;
		if (!(T0Ind_Last)) {
			_N_In2 = _N_In2 + (8784); _SN_In2 = ((T0Ind_NextLast)?6528:8784); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+_N_In1), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+8784*((T0Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+17568+8784*((T0Ind_Total+1)%2)),
					_SN_In2, 0, &DmaR_Evt2);
		}
		/*============================= End Read Tiles ======================================*/
		/*====================== Call Kernel LOC_LOOP =========================*/
		KerArg0->In1 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0+8784*((T0Ind_Total)%2));
		KerArg0->In2 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+17568+8784*((T0Ind_Total)%2));
		KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+35136+8784*((T0Ind_Total)%2));
		KerArg0->Feat = (unsigned short int) (T0Ind_Last?6528:8784);
		AT_FORK(gap_ncore(), (void *) KerMatAdd_SQ8, (void *) KerArg0);
		__CALL(KerMatAdd_SQ8, KerArg0);
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait previous uDMA write Out */
		if (_SP_Out) AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+176992+8784*((T0Ind_Total+-1)%2)),
					_SP_Out, 1, &UchanHR2);
		AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+176992+8784*((T0Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+35136+8784*((T0Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SPP_Out = _SP_Out;
		_P_Out = _C_Out;_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T0Ind_Last)) {
			_C_Out = _C_Out + (8784); _SC_Out = ((T0Ind_NextLast)?6528:8784); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+176992+8784*((T0Ind_Total+-1)%2)), _SP_Out, 1, &UchanHR2);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S32_Conv2d_96x16x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50572 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERRAM_CL_EVENT UchanHR6;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF5;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _NN_In2;
	unsigned int _SN_In2, _SNN_In2;
	unsigned int _LN_In2, _LNN_In2;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 23][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 23 logical tiles, 23 physical tiles
			Total Size: 76800 [Tile1, 23:[16x216, 21:16x216, 16x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 23:[16x216, 21:16x216, 16x48], 1]
		Tile0: [0, 3456, 216], Tile1: [216, 3456, 216], Tile2; [432, 3456, 216]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1536 [Tile0, 1:[16x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[16x96], 1]
		Tile0: [0, 1536, 1536], Tile1: [0, 1536, 1536], Tile2; [0, 1536, 1536]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 23 logical tiles, 1 physical tiles
			Total Size: 64 [Tile1, 23:[64x1, 21:64x1, 64x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 23:[64x1, 21:64x1, 64x1], 1]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 384 [Tile0, 1:[1x96], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 4]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 23 logical tiles, 23 physical tiles
			Total Size: 460800 [Tile1, 23:[96x216, 21:96x216, 96x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 23:[96x216, 21:96x216, 96x48], 1]
		Tile0: [0, 20736, 216], Tile1: [216, 20736, 216], Tile2; [432, 20736, 216]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile0, 1:[1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 1]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile0, 1:[1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 1]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 23 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 23:[1x1, 21:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 23:[1x1, 21:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+48460);
	KerArg0->W_In1 = (unsigned short int) (16);
	KerArg0->H_In1 = (unsigned short int) (96);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+49996);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50380);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50476);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+48448);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+64864+0), 3456, 4800, 216, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+216), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+64864+3456), 3456, 4800, 216, 0, &UchanHR1);
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+64864+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+64+0), 3456, 0, &DmaR_Evt1);
	_NN_In2=216; _SN_In2=3456;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48460), 1536, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+71776+0), 384, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+71776+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+49996), 384, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=20736; _LC_Out=216;
	_SPP_Out=0; _SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+72160+0), 96, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+72160+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50380), 96, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+72256+0), 96, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+72256+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50476), 96, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+72352+0), 9, 0, &UchanHF5);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+72352+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48448), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<23; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==22), T1Ind_NextLast = ((T1Ind+1)==22), T1Ind_NextNextLast = ((T1Ind+2)==22);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In2 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In2 = _NN_In2 + (216); _LNN_In2 = ((T1Ind_NextNextLast)?48:216); _SNN_In2 = (16*_LNN_In2); 
			}
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
		if (_SNN_In2) {
			AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+_NN_In2), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+64864+3456*((T1Ind_Total)%2)),
					_SNN_In2, 4800, _LNN_In2, 0, &UchanHR1);
		}
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+64864+3456*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+64+3456*((T1Ind_Total+1)%2)),
					_SN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+64+3456*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?48:216);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+6976+20736*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?48:216);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*96);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+48448))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR6); /* Wait previous uDMA write Out */
		if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+20736*((T1Ind_Total+-1)%2)),
					_SP_Out, 4800, _LP_Out, 1, &UchanHR6);
		AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+20736*((T1Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6976+20736*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SN_In2 = _SNN_In2;_LN_In2 = _LNN_In2;
		_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
		_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (216); _LC_Out = ((T1Ind_NextLast)?48:216); _SC_Out = (96*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR6); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+20736*((T1Ind_Total+-1)%2)), _SP_Out, 4800, _LP_Out, 1, &UchanHR6);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR6); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S35_Conv2d_96x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 52012 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERRAM_CL_EVENT UchanHR4;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF5;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last, D0Ind_NextLast, D0Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast, T0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	unsigned int _NN_In;
	unsigned int _SN_In, _SNN_In;
	unsigned int _LN_In, _LNN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 96, Tiled: 12][Tile0 Dim: 3]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 36 logical tiles, 36 physical tiles
			Total Size: 115200 [D0, [11 x 9600, 9600]][Tile0, 3:[40x11, 1:40x11, 40x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 9600, 9600]][Tile0, 3:[40x11, 1:40x11, 40x8], 1]
		Tile0: [0, 3520, 440], Tile1: [440, 3520, 440], Tile2; [880, 2560, 320]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 384 [D0, [11 x 32, 32]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 32, 32]]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 96 [D0, [11 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 8, 8]]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 96 [D0, [11 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 8, 8]]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 864 [D0, [11 x 72, 72]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 72, 72]]
		Tile0: [0, 864, 864], Tile1: [0, 864, 864], Tile2; [0, 864, 864]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 36 logical tiles, 36 physical tiles
			Total Size: 460800 [D0, [11 x 38400, 38400]][Tile0, 3:[80x23, 1:80x23, 80x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 38400, 38400]][Tile0, 3:[80x23, 1:80x23, 80x16], 1]
		Tile0: [0, 14720, 1840], Tile1: [1760, 14720, 1840], Tile2; [3520, 10240, 1280]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 36 logical tiles, 1 physical tiles
			Total Size: 460800 [D0, [11 x 38400, 38400]][Tile0, 3:[40x11, 1:40x11, 40x8], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 38400, 38400]][Tile0, 3:[40x11, 1:40x11, 40x8], 4]
		Tile0: [0, 14080, 1760], Tile1: [0, 14080, 1760], Tile2; [0, 14080, 1760]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 3 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 3:[9x1, 1:9x1, 9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[9x1, 1:9x1, 9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (80);
	KerArg0->UsedW = (unsigned short int) (80);
	KerArg0->InFeatures = (unsigned short int) (8);
	KerArg0->OutFeatures = (unsigned short int) (8);
	KerArg0->TotalInFeatures = (unsigned short int) (96);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+37920);
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+37920);
	KerArg1->Feat = (unsigned short int) (8);
	KerArg1->W = (unsigned short int) (40);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+52000);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=3520; _LC_Out=440;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+170432+0), 384, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+170432+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+29440), 384, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+170816+0), 96, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+170816+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+29824), 96, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+170912+0), 96, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+170912+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+29920), 96, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+30016), 864, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+0), 14720, 4800, 1840, 0, &UchanHR4);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR4); /* Wait previous uDMA read In */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+1760), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+14720), 14720, 4800, 1840, 0, &UchanHR4);
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 14720, 0, &DmaR_Evt5);
	_NN_In=1760; _SN_In=14720;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+171008+0), 9, 0, &UchanHF5);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+171008+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+52000), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<12; D0Ind++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==11), D0Ind_NextLast = ((D0Ind+1)==11), D0Ind_NextNextLast = ((D0Ind+2)==11);
		for (T0Ind=0; T0Ind<3; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==2), T0Ind_NextLast = ((T0Ind+1)==2), T0Ind_NextNextLast = ((T0Ind+2)==2);
			/*================================= Prepare Tiles ===================================*/
			_SNN_In = 0;
			if (!(T0Ind_Last)) {
				if (!(T0Ind_NextLast)) {
					_NN_In = _NN_In + (1760); _LNN_In = ((1)?1280:1840); _SNN_In = (8*_LNN_In); 
				} else if (!(D0Ind_Last)) {
					_NN_In = _NN_In + (38400)+(-3520); _LNN_In = (1840); _SNN_In = (8*_LNN_In); 
				}
			} else if (!(D0Ind_Last)) {
				_NN_In = _NN_In + (1760); _LNN_In = (1840); _SNN_In = (8*_LNN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR4); /* Wait previous uDMA read In */
			if (_SNN_In) {
				AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+_NN_In), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+14720*((T0Ind_Total)%2)),
						_SNN_In, 4800, _LNN_In, 0, &UchanHR4);
			}
			AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+14720*((T0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+14720*((T0Ind_Total+1)%2)),
						_SN_In, 0, &DmaR_Evt5);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+14720*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (((T0Ind_Last)?16:23)-0*(T0Ind==0));
			KerArg0->UsedH = (unsigned short int) (((T0Ind_Last)?16:23)-0*(T0Ind==0));
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+30016+((D0Ind)*72));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+29440+((D0Ind)*32));
			KerArg0->Pad = (v4s) ((v4s){0,1,0*(T0Ind==0),1*(T0Ind_Last)});
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+52000))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride2B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride2B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+30880+3520*((T0Ind_Total)%2));
			KerArg1->H = (unsigned short int) (T0Ind_Last?8:11);
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+29824+((D0Ind)*8));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+29920+((D0Ind)*8));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+30880+3520*((T0Ind_Total)%2)),
					_SC_Out, 1200, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			_SN_In = _SNN_In;_LN_In = _LNN_In;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (440); _LC_Out = ((T0Ind_NextLast)?320:440); _SC_Out = (8*_LC_Out); 
			} else if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (9600)+(-880); _LC_Out = (440); _SC_Out = (8*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S38_Conv2d_16x96x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50412 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 6][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 6 logical tiles, 6 physical tiles
			Total Size: 115200 [Tile1, 6:[96x216, 4:96x216, 96x120], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[96x216, 4:96x216, 96x120], 1]
		Tile0: [0, 20736, 216], Tile1: [216, 20736, 216], Tile2; [432, 20736, 216]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1536 [Tile0, 1:[96x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[96x16], 1]
		Tile0: [0, 1536, 1536], Tile1: [0, 1536, 1536], Tile2; [0, 1536, 1536]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 1 physical tiles
			Total Size: 384 [Tile1, 6:[384x1, 4:384x1, 384x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[384x1, 4:384x1, 384x1], 1]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile0, 1:[1x16], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x16], 4]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 6 physical tiles
			Total Size: 19200 [Tile1, 6:[16x216, 4:16x216, 16x120], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[16x216, 4:16x216, 16x120], 1]
		Tile0: [0, 3456, 216], Tile1: [216, 3456, 216], Tile2; [432, 3456, 216]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 16 [Tile0, 1:[1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x16], 1]
		Tile0: [0, 16, 16], Tile1: [0, 16, 16], Tile2; [0, 16, 16]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 16 [Tile0, 1:[1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x16], 1]
		Tile0: [0, 16, 16], Tile1: [0, 16, 16], Tile2; [0, 16, 16]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 6:[1x1, 4:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[1x1, 4:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+48780);
	KerArg0->W_In1 = (unsigned short int) (96);
	KerArg0->H_In1 = (unsigned short int) (16);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+50316);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50380);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50396);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+48768);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+384+0), 20736, 1200, 216, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48780), 1536, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+42592+0), 64, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+42592+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50316), 64, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=3456; _LC_Out=216;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+42656+0), 16, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+42656+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50380), 16, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+42672+0), 16, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+42672+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50396), 16, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+42688+0), 9, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+42688+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48768), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<6; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==5), T1Ind_NextLast = ((T1Ind+1)==5);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (216); _LN_In2 = ((T1Ind_NextLast)?120:216); _SN_In2 = (96*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+384+20736*((T1Ind_Total+1)%2)),
					_SN_In2, 1200, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+384+20736*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?120:216);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+41856+3456*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?120:216);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*16);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+48768))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+41856+3456*((T1Ind_Total)%2)),
				_SC_Out, 1200, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (216); _LC_Out = ((T1Ind_NextLast)?120:216); _SC_Out = (16*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S41_Conv2d_96x16x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50572 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 6][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 6 logical tiles, 6 physical tiles
			Total Size: 19200 [Tile1, 6:[16x216, 4:16x216, 16x120], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[16x216, 4:16x216, 16x120], 1]
		Tile0: [0, 3456, 216], Tile1: [216, 3456, 216], Tile2; [432, 3456, 216]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1536 [Tile0, 1:[16x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[16x96], 1]
		Tile0: [0, 1536, 1536], Tile1: [0, 1536, 1536], Tile2; [0, 1536, 1536]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 1 physical tiles
			Total Size: 64 [Tile1, 6:[64x1, 4:64x1, 64x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[64x1, 4:64x1, 64x1], 1]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 384 [Tile0, 1:[1x96], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 4]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 6 physical tiles
			Total Size: 115200 [Tile1, 6:[96x216, 4:96x216, 96x120], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[96x216, 4:96x216, 96x120], 1]
		Tile0: [0, 20736, 216], Tile1: [216, 20736, 216], Tile2; [432, 20736, 216]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile0, 1:[1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 1]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile0, 1:[1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 1]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 6:[1x1, 4:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[1x1, 4:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+48460);
	KerArg0->W_In1 = (unsigned short int) (16);
	KerArg0->H_In1 = (unsigned short int) (96);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+49996);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50380);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50476);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+48448);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+64+0), 3456, 1200, 216, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48460), 1536, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+157792+0), 384, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+157792+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+49996), 384, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=20736; _LC_Out=216;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+158176+0), 96, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+158176+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50380), 96, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+158272+0), 96, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+158272+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50476), 96, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+158368+0), 9, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+158368+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48448), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<6; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==5), T1Ind_NextLast = ((T1Ind+1)==5);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (216); _LN_In2 = ((T1Ind_NextLast)?120:216); _SN_In2 = (16*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+64+3456*((T1Ind_Total+1)%2)),
					_SN_In2, 1200, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+64+3456*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?120:216);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+6976+20736*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?120:216);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*96);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+48448))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6976+20736*((T1Ind_Total)%2)),
				_SC_Out, 1200, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (216); _LC_Out = ((T1Ind_NextLast)?120:216); _SC_Out = (96*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S44_Conv2d_96x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 51372 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERRAM_CL_EVENT UchanHR5;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 96, Tiled: 12][Tile0 Dim: 2]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 24 logical tiles, 24 physical tiles
			Total Size: 115200 [D0, [11 x 9600, 9600]][Tile0, 2:[40x20, 40x12], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 9600, 9600]][Tile0, 2:[40x20, 40x12], 1]
		Tile0: [0, 6400, 800], Tile1: [720, 3840, 480], Tile2; [9600, 6400, 800]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 384 [D0, [11 x 32, 32]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 32, 32]]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 96 [D0, [11 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 8, 8]]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 96 [D0, [11 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 8, 8]]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 864 [D0, [11 x 72, 72]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 72, 72]]
		Tile0: [0, 864, 864], Tile1: [0, 864, 864], Tile2; [0, 864, 864]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 24 logical tiles, 24 physical tiles
			Total Size: 115200 [D0, [11 x 9600, 9600]][Tile0, 2:[40x19, 40x11], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 9600, 9600]][Tile0, 2:[40x19, 40x11], 1]
		Tile0: [0, 6080, 760], Tile1: [760, 3520, 440], Tile2; [9600, 6080, 760]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 24 logical tiles, 1 physical tiles
			Total Size: 460800 [D0, [11 x 38400, 38400]][Tile0, 2:[40x19, 40x11], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 38400, 38400]][Tile0, 2:[40x19, 40x11], 4]
		Tile0: [0, 24320, 3040], Tile1: [0, 24320, 3040], Tile2; [0, 24320, 3040]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 2:[9x1, 9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 2:[9x1, 9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (40);
	KerArg0->UsedW = (unsigned short int) (40);
	KerArg0->InFeatures = (unsigned short int) (8);
	KerArg0->OutFeatures = (unsigned short int) (8);
	KerArg0->TotalInFeatures = (unsigned short int) (96);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+27040);
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+27040);
	KerArg1->Feat = (unsigned short int) (8);
	KerArg1->W = (unsigned short int) (40);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+51360);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 6400, 1200, 800, 0, &DmaR_Evt1);
	_N_In=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+172352+0), 384, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+172352+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+13440), 384, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+172736+0), 96, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+172736+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+13824), 96, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+172832+0), 96, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+172832+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+13920), 96, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+14016), 864, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
	_C_Out=0; _SC_Out=6080; _LC_Out=760;
	_SPP_Out=0; _SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+172928+0), 9, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+172928+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+51360), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<12; D0Ind++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==11), D0Ind_NextLast = ((D0Ind+1)==11);
		for (T0Ind=0; T0Ind<2; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==1), T0Ind_NextLast = ((T0Ind+1)==1);
			/*================================= Prepare Tiles ===================================*/
			_SN_In = 0;
			if (!(T0Ind_Last)) {
				_N_In = _N_In + (760-(40*(T0Ind==0))); _LN_In = ((1)?480:840); _SN_In = (8*_LN_In); 
			} else if (!(D0Ind_Last)) {
				_N_In = _N_In + (9600)+(-720); _LN_In = (800); _SN_In = (8*_LN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+6720*((T0Ind_Total+1)%2)),
						_SN_In, 1200, _LN_In, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+6720*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (((T0Ind_Last)?12:21)-1*(T0Ind==0));
			KerArg0->UsedH = (unsigned short int) (((T0Ind_Last)?12:21)-1*(T0Ind==0));
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+14016+((D0Ind)*72));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+13440+((D0Ind)*32));
			KerArg0->Pad = (v4s) ((v4s){1,1,1*(T0Ind==0),1*(T0Ind_Last)});
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+51360))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride1B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride1B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+14880+6080*((T0Ind_Total)%2));
			KerArg1->H = (unsigned short int) (T0Ind_Last?11:19);
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+13824+((D0Ind)*8));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+13920+((D0Ind)*8));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR5); /* Wait previous uDMA write Out */
			if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+157792+6080*((T0Ind_Total+-1)%2)),
						_SP_Out, 1200, _LP_Out, 1, &UchanHR5);
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+157792+6080*((T0Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+14880+6080*((T0Ind_Total)%2)),
					_SC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
			_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (760); _LC_Out = ((1)?440:760); _SC_Out = (8*_LC_Out); 
			} else if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (9600)+(-760); _LC_Out = (760); _SC_Out = (8*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR5); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+157792+6080*((T0Ind_Total+-1)%2)), _SP_Out, 1200, _LP_Out, 1, &UchanHR5);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR5); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S47_Conv2d_16x96x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50412 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF5;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _NN_In2;
	unsigned int _SN_In2, _SNN_In2;
	unsigned int _LN_In2, _LNN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 6][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 6 logical tiles, 6 physical tiles
			Total Size: 115200 [Tile1, 6:[96x216, 4:96x216, 96x120], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[96x216, 4:96x216, 96x120], 1]
		Tile0: [0, 20736, 216], Tile1: [216, 20736, 216], Tile2; [432, 20736, 216]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1536 [Tile0, 1:[96x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[96x16], 1]
		Tile0: [0, 1536, 1536], Tile1: [0, 1536, 1536], Tile2; [0, 1536, 1536]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 1 physical tiles
			Total Size: 384 [Tile1, 6:[384x1, 4:384x1, 384x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[384x1, 4:384x1, 384x1], 1]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile0, 1:[1x16], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x16], 4]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 6 physical tiles
			Total Size: 19200 [Tile1, 6:[16x216, 4:16x216, 16x120], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[16x216, 4:16x216, 16x120], 1]
		Tile0: [0, 3456, 216], Tile1: [216, 3456, 216], Tile2; [432, 3456, 216]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 16 [Tile0, 1:[1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x16], 1]
		Tile0: [0, 16, 16], Tile1: [0, 16, 16], Tile2; [0, 16, 16]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 16 [Tile0, 1:[1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x16], 1]
		Tile0: [0, 16, 16], Tile1: [0, 16, 16], Tile2; [0, 16, 16]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 6:[1x1, 4:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[1x1, 4:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+48780);
	KerArg0->W_In1 = (unsigned short int) (96);
	KerArg0->H_In1 = (unsigned short int) (16);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+50316);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50380);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50396);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+48768);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+61792+0), 20736, 1200, 216, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+216), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+61792+20736), 20736, 1200, 216, 0, &UchanHR1);
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+61792+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+384+0), 20736, 0, &DmaR_Evt1);
	_NN_In2=216; _SN_In2=20736;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48780), 1536, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+103264+0), 64, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+103264+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50316), 64, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=3456; _LC_Out=216;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+103328+0), 16, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+103328+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50380), 16, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+103344+0), 16, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+103344+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50396), 16, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+103360+0), 9, 0, &UchanHF5);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+103360+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48768), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<6; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==5), T1Ind_NextLast = ((T1Ind+1)==5), T1Ind_NextNextLast = ((T1Ind+2)==5);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In2 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In2 = _NN_In2 + (216); _LNN_In2 = ((T1Ind_NextNextLast)?120:216); _SNN_In2 = (96*_LNN_In2); 
			}
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
		if (_SNN_In2) {
			AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+_NN_In2), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+61792+20736*((T1Ind_Total)%2)),
					_SNN_In2, 1200, _LNN_In2, 0, &UchanHR1);
		}
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+61792+20736*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+384+20736*((T1Ind_Total+1)%2)),
					_SN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+384+20736*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?120:216);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+41856+3456*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?120:216);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*16);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+48768))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+41856+3456*((T1Ind_Total)%2)),
				_SC_Out, 1200, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SN_In2 = _SNN_In2;_LN_In2 = _LNN_In2;
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (216); _LC_Out = ((T1Ind_NextLast)?120:216); _SC_Out = (16*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S48_MatAdd_16x30x40(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 52716 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerMat3_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In1;
	unsigned int _SN_In1;
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 3]
	Ker Arg: In1, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 3 logical tiles, 3 physical tiles
			Total Size: 19200 [Tile0, 3:[1x8784, 1:1x8784, 1x1632], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[1x8784, 1:1x8784, 1x1632], 1]
		Tile0: [0, 8784, 8784], Tile1: [8784, 8784, 8784], Tile2; [17568, 1632, 1632]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 3 logical tiles, 3 physical tiles
			Total Size: 19200 [Tile0, 3:[1x8784, 1:1x8784, 1x1632], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[1x8784, 1:1x8784, 1x1632], 1]
		Tile0: [0, 8784, 8784], Tile1: [8784, 8784, 8784], Tile2; [17568, 1632, 1632]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 3 logical tiles, 3 physical tiles
			Total Size: 19200 [Tile0, 3:[1x8784, 1:1x8784, 1x1632], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[1x8784, 1:1x8784, 1x1632], 1]
		Tile0: [0, 8784, 8784], Tile1: [8784, 8784, 8784], Tile2; [17568, 1632, 1632]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 3 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 3:[1x1, 1:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[1x1, 1:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->DoScale = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+52704);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 8784, 0, &DmaR_Evt1);
	_N_In1=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+17568+0), 8784, 0, &DmaR_Evt2);
	_N_In2=0;
	_C_Out=0; _SC_Out=8784;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+61792+0), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+61792+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+52704), 9, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T0Ind=0; T0Ind<3; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
		int T0Ind_Last = (T0Ind==2), T0Ind_NextLast = ((T0Ind+1)==2);
		/*================================= Prepare Tiles ===================================*/
		_SN_In1 = 0;
		if (!(T0Ind_Last)) {
			_N_In1 = _N_In1 + (8784); _SN_In1 = ((T0Ind_NextLast)?1632:8784); 
		}
		_SN_In2 = 0;
		if (!(T0Ind_Last)) {
			_N_In2 = _N_In2 + (8784); _SN_In2 = ((T0Ind_NextLast)?1632:8784); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+_N_In1), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+8784*((T0Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+17568+8784*((T0Ind_Total+1)%2)),
					_SN_In2, 0, &DmaR_Evt2);
		}
		/*============================= End Read Tiles ======================================*/
		/*====================== Call Kernel LOC_LOOP =========================*/
		KerArg0->In1 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0+8784*((T0Ind_Total)%2));
		KerArg0->In2 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+17568+8784*((T0Ind_Total)%2));
		KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+35136+8784*((T0Ind_Total)%2));
		KerArg0->Feat = (unsigned short int) (T0Ind_Last?1632:8784);
		AT_FORK(gap_ncore(), (void *) KerMatAdd_SQ8, (void *) KerArg0);
		__CALL(KerMatAdd_SQ8, KerArg0);
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+35136+8784*((T0Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T0Ind_Last)) {
			_C_Out = _C_Out + (8784); _SC_Out = ((T0Ind_NextLast)?1632:8784); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S51_Conv2d_96x16x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50572 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 6][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 6 logical tiles, 6 physical tiles
			Total Size: 19200 [Tile1, 6:[16x216, 4:16x216, 16x120], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[16x216, 4:16x216, 16x120], 1]
		Tile0: [0, 3456, 216], Tile1: [216, 3456, 216], Tile2; [432, 3456, 216]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1536 [Tile0, 1:[16x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[16x96], 1]
		Tile0: [0, 1536, 1536], Tile1: [0, 1536, 1536], Tile2; [0, 1536, 1536]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 1 physical tiles
			Total Size: 64 [Tile1, 6:[64x1, 4:64x1, 64x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[64x1, 4:64x1, 64x1], 1]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 384 [Tile0, 1:[1x96], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 4]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 6 physical tiles
			Total Size: 115200 [Tile1, 6:[96x216, 4:96x216, 96x120], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[96x216, 4:96x216, 96x120], 1]
		Tile0: [0, 20736, 216], Tile1: [216, 20736, 216], Tile2; [432, 20736, 216]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile0, 1:[1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 1]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile0, 1:[1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 1]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 6:[1x1, 4:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[1x1, 4:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+48460);
	KerArg0->W_In1 = (unsigned short int) (16);
	KerArg0->H_In1 = (unsigned short int) (96);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+49996);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50380);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50476);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+48448);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+64+0), 3456, 1200, 216, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48460), 1536, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+140128+0), 384, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+140128+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+49996), 384, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=20736; _LC_Out=216;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+140512+0), 96, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+140512+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50380), 96, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+140608+0), 96, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+140608+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50476), 96, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+140704+0), 9, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+140704+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48448), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<6; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==5), T1Ind_NextLast = ((T1Ind+1)==5);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (216); _LN_In2 = ((T1Ind_NextLast)?120:216); _SN_In2 = (16*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+64+3456*((T1Ind_Total+1)%2)),
					_SN_In2, 1200, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+64+3456*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?120:216);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+6976+20736*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?120:216);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*96);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+48448))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6976+20736*((T1Ind_Total)%2)),
				_SC_Out, 1200, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (216); _LC_Out = ((T1Ind_NextLast)?120:216); _SC_Out = (96*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S54_Conv2d_96x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 51372 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERRAM_CL_EVENT UchanHR5;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 96, Tiled: 12][Tile0 Dim: 2]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 24 logical tiles, 24 physical tiles
			Total Size: 115200 [D0, [11 x 9600, 9600]][Tile0, 2:[40x20, 40x12], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 9600, 9600]][Tile0, 2:[40x20, 40x12], 1]
		Tile0: [0, 6400, 800], Tile1: [720, 3840, 480], Tile2; [9600, 6400, 800]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 384 [D0, [11 x 32, 32]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 32, 32]]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 96 [D0, [11 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 8, 8]]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 96 [D0, [11 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 8, 8]]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 864 [D0, [11 x 72, 72]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 72, 72]]
		Tile0: [0, 864, 864], Tile1: [0, 864, 864], Tile2; [0, 864, 864]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 24 logical tiles, 24 physical tiles
			Total Size: 115200 [D0, [11 x 9600, 9600]][Tile0, 2:[40x19, 40x11], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 9600, 9600]][Tile0, 2:[40x19, 40x11], 1]
		Tile0: [0, 6080, 760], Tile1: [760, 3520, 440], Tile2; [9600, 6080, 760]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 24 logical tiles, 1 physical tiles
			Total Size: 460800 [D0, [11 x 38400, 38400]][Tile0, 2:[40x19, 40x11], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 38400, 38400]][Tile0, 2:[40x19, 40x11], 4]
		Tile0: [0, 24320, 3040], Tile1: [0, 24320, 3040], Tile2; [0, 24320, 3040]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 2:[9x1, 9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 2:[9x1, 9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (40);
	KerArg0->UsedW = (unsigned short int) (40);
	KerArg0->InFeatures = (unsigned short int) (8);
	KerArg0->OutFeatures = (unsigned short int) (8);
	KerArg0->TotalInFeatures = (unsigned short int) (96);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+27040);
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+27040);
	KerArg1->Feat = (unsigned short int) (8);
	KerArg1->W = (unsigned short int) (40);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+51360);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 6400, 1200, 800, 0, &DmaR_Evt1);
	_N_In=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+172352+0), 384, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+172352+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+13440), 384, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+172736+0), 96, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+172736+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+13824), 96, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+172832+0), 96, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+172832+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+13920), 96, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+14016), 864, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
	_C_Out=0; _SC_Out=6080; _LC_Out=760;
	_SPP_Out=0; _SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+172928+0), 9, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+172928+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+51360), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<12; D0Ind++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==11), D0Ind_NextLast = ((D0Ind+1)==11);
		for (T0Ind=0; T0Ind<2; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==1), T0Ind_NextLast = ((T0Ind+1)==1);
			/*================================= Prepare Tiles ===================================*/
			_SN_In = 0;
			if (!(T0Ind_Last)) {
				_N_In = _N_In + (760-(40*(T0Ind==0))); _LN_In = ((1)?480:840); _SN_In = (8*_LN_In); 
			} else if (!(D0Ind_Last)) {
				_N_In = _N_In + (9600)+(-720); _LN_In = (800); _SN_In = (8*_LN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+6720*((T0Ind_Total+1)%2)),
						_SN_In, 1200, _LN_In, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+6720*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (((T0Ind_Last)?12:21)-1*(T0Ind==0));
			KerArg0->UsedH = (unsigned short int) (((T0Ind_Last)?12:21)-1*(T0Ind==0));
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+14016+((D0Ind)*72));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+13440+((D0Ind)*32));
			KerArg0->Pad = (v4s) ((v4s){1,1,1*(T0Ind==0),1*(T0Ind_Last)});
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+51360))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride1B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride1B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+14880+6080*((T0Ind_Total)%2));
			KerArg1->H = (unsigned short int) (T0Ind_Last?11:19);
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+13824+((D0Ind)*8));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+13920+((D0Ind)*8));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR5); /* Wait previous uDMA write Out */
			if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+138592+6080*((T0Ind_Total+-1)%2)),
						_SP_Out, 1200, _LP_Out, 1, &UchanHR5);
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+138592+6080*((T0Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+14880+6080*((T0Ind_Total)%2)),
					_SC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
			_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (760); _LC_Out = ((1)?440:760); _SC_Out = (8*_LC_Out); 
			} else if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (9600)+(-760); _LC_Out = (760); _SC_Out = (8*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR5); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+138592+6080*((T0Ind_Total+-1)%2)), _SP_Out, 1200, _LP_Out, 1, &UchanHR5);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR5); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S57_Conv2d_16x96x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50412 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF5;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _NN_In2;
	unsigned int _SN_In2, _SNN_In2;
	unsigned int _LN_In2, _LNN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 6][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 6 logical tiles, 6 physical tiles
			Total Size: 115200 [Tile1, 6:[96x216, 4:96x216, 96x120], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[96x216, 4:96x216, 96x120], 1]
		Tile0: [0, 20736, 216], Tile1: [216, 20736, 216], Tile2; [432, 20736, 216]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1536 [Tile0, 1:[96x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[96x16], 1]
		Tile0: [0, 1536, 1536], Tile1: [0, 1536, 1536], Tile2; [0, 1536, 1536]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 1 physical tiles
			Total Size: 384 [Tile1, 6:[384x1, 4:384x1, 384x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[384x1, 4:384x1, 384x1], 1]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile0, 1:[1x16], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x16], 4]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 6 physical tiles
			Total Size: 19200 [Tile1, 6:[16x216, 4:16x216, 16x120], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[16x216, 4:16x216, 16x120], 1]
		Tile0: [0, 3456, 216], Tile1: [216, 3456, 216], Tile2; [432, 3456, 216]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 16 [Tile0, 1:[1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x16], 1]
		Tile0: [0, 16, 16], Tile1: [0, 16, 16], Tile2; [0, 16, 16]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 16 [Tile0, 1:[1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x16], 1]
		Tile0: [0, 16, 16], Tile1: [0, 16, 16], Tile2; [0, 16, 16]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 6:[1x1, 4:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[1x1, 4:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+48780);
	KerArg0->W_In1 = (unsigned short int) (96);
	KerArg0->H_In1 = (unsigned short int) (16);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+50316);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50380);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50396);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+48768);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+0), 20736, 1200, 216, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+216), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+20736), 20736, 1200, 216, 0, &UchanHR1);
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+384+0), 20736, 0, &DmaR_Evt1);
	_NN_In2=216; _SN_In2=20736;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48780), 1536, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+84064+0), 64, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+84064+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50316), 64, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=3456; _LC_Out=216;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+84128+0), 16, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+84128+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50380), 16, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+84144+0), 16, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+84144+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50396), 16, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+84160+0), 9, 0, &UchanHF5);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+84160+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48768), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<6; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==5), T1Ind_NextLast = ((T1Ind+1)==5), T1Ind_NextNextLast = ((T1Ind+2)==5);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In2 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In2 = _NN_In2 + (216); _LNN_In2 = ((T1Ind_NextNextLast)?120:216); _SNN_In2 = (96*_LNN_In2); 
			}
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
		if (_SNN_In2) {
			AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+_NN_In2), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+20736*((T1Ind_Total)%2)),
					_SNN_In2, 1200, _LNN_In2, 0, &UchanHR1);
		}
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+20736*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+384+20736*((T1Ind_Total+1)%2)),
					_SN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+384+20736*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?120:216);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+41856+3456*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?120:216);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*16);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+48768))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+41856+3456*((T1Ind_Total)%2)),
				_SC_Out, 1200, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SN_In2 = _SNN_In2;_LN_In2 = _LNN_In2;
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (216); _LC_Out = ((T1Ind_NextLast)?120:216); _SC_Out = (16*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S58_MatAdd_16x30x40(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 52716 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerMat3_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In1;
	unsigned int _SN_In1;
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 3]
	Ker Arg: In1, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 3 logical tiles, 3 physical tiles
			Total Size: 19200 [Tile0, 3:[1x8784, 1:1x8784, 1x1632], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[1x8784, 1:1x8784, 1x1632], 1]
		Tile0: [0, 8784, 8784], Tile1: [8784, 8784, 8784], Tile2; [17568, 1632, 1632]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 3 logical tiles, 3 physical tiles
			Total Size: 19200 [Tile0, 3:[1x8784, 1:1x8784, 1x1632], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[1x8784, 1:1x8784, 1x1632], 1]
		Tile0: [0, 8784, 8784], Tile1: [8784, 8784, 8784], Tile2; [17568, 1632, 1632]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 3 logical tiles, 3 physical tiles
			Total Size: 19200 [Tile0, 3:[1x8784, 1:1x8784, 1x1632], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[1x8784, 1:1x8784, 1x1632], 1]
		Tile0: [0, 8784, 8784], Tile1: [8784, 8784, 8784], Tile2; [17568, 1632, 1632]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 3 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 3:[1x1, 1:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[1x1, 1:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->DoScale = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+52704);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 8784, 0, &DmaR_Evt1);
	_N_In1=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+17568+0), 8784, 0, &DmaR_Evt2);
	_N_In2=0;
	_C_Out=0; _SC_Out=8784;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+52704), 9, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T0Ind=0; T0Ind<3; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
		int T0Ind_Last = (T0Ind==2), T0Ind_NextLast = ((T0Ind+1)==2);
		/*================================= Prepare Tiles ===================================*/
		_SN_In1 = 0;
		if (!(T0Ind_Last)) {
			_N_In1 = _N_In1 + (8784); _SN_In1 = ((T0Ind_NextLast)?1632:8784); 
		}
		_SN_In2 = 0;
		if (!(T0Ind_Last)) {
			_N_In2 = _N_In2 + (8784); _SN_In2 = ((T0Ind_NextLast)?1632:8784); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+_N_In1), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+8784*((T0Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+17568+8784*((T0Ind_Total+1)%2)),
					_SN_In2, 0, &DmaR_Evt2);
		}
		/*============================= End Read Tiles ======================================*/
		/*====================== Call Kernel LOC_LOOP =========================*/
		KerArg0->In1 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0+8784*((T0Ind_Total)%2));
		KerArg0->In2 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+17568+8784*((T0Ind_Total)%2));
		KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+35136+8784*((T0Ind_Total)%2));
		KerArg0->Feat = (unsigned short int) (T0Ind_Last?1632:8784);
		AT_FORK(gap_ncore(), (void *) KerMatAdd_SQ8, (void *) KerArg0);
		__CALL(KerMatAdd_SQ8, KerArg0);
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+35136+8784*((T0Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T0Ind_Last)) {
			_C_Out = _C_Out + (8784); _SC_Out = ((T0Ind_NextLast)?1632:8784); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S61_Conv2d_96x16x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50572 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 6][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 6 logical tiles, 6 physical tiles
			Total Size: 19200 [Tile1, 6:[16x216, 4:16x216, 16x120], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[16x216, 4:16x216, 16x120], 1]
		Tile0: [0, 3456, 216], Tile1: [216, 3456, 216], Tile2; [432, 3456, 216]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1536 [Tile0, 1:[16x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[16x96], 1]
		Tile0: [0, 1536, 1536], Tile1: [0, 1536, 1536], Tile2; [0, 1536, 1536]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 1 physical tiles
			Total Size: 64 [Tile1, 6:[64x1, 4:64x1, 64x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[64x1, 4:64x1, 64x1], 1]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 384 [Tile0, 1:[1x96], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 4]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 6 physical tiles
			Total Size: 115200 [Tile1, 6:[96x216, 4:96x216, 96x120], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[96x216, 4:96x216, 96x120], 1]
		Tile0: [0, 20736, 216], Tile1: [216, 20736, 216], Tile2; [432, 20736, 216]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile0, 1:[1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 1]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile0, 1:[1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x96], 1]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 6:[1x1, 4:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 6:[1x1, 4:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+48460);
	KerArg0->W_In1 = (unsigned short int) (16);
	KerArg0->H_In1 = (unsigned short int) (96);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+49996);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50380);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+50476);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+48448);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+64+0), 3456, 1200, 216, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48460), 1536, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+140128+0), 384, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+140128+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+49996), 384, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=20736; _LC_Out=216;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+140512+0), 96, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+140512+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50380), 96, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+140608+0), 96, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+140608+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50476), 96, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+140704+0), 9, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+140704+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48448), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<6; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==5), T1Ind_NextLast = ((T1Ind+1)==5);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (216); _LN_In2 = ((T1Ind_NextLast)?120:216); _SN_In2 = (16*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+64+3456*((T1Ind_Total+1)%2)),
					_SN_In2, 1200, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+64+3456*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?120:216);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+6976+20736*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?120:216);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*96);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+48448))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6976+20736*((T1Ind_Total)%2)),
				_SC_Out, 1200, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (216); _LC_Out = ((T1Ind_NextLast)?120:216); _SC_Out = (96*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S64_Conv2d_96x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 35052 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 96, Tiled: 12][Tile0 Dim: 1]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 12 physical tiles
			Total Size: 28800 [D0, [11 x 2400, 2400]][Tile0, 1:[20x15], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 2400, 2400]][Tile0, 1:[20x15], 1]
		Tile0: [0, 2400, 300], Tile1: [2400, 2400, 300], Tile2; [4800, 2400, 300]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 384 [D0, [11 x 32, 32]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 32, 32]]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 96 [D0, [11 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 8, 8]]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 96 [D0, [11 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 8, 8]]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 864 [D0, [11 x 72, 72]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 72, 72]]
		Tile0: [0, 864, 864], Tile1: [0, 864, 864], Tile2; [0, 864, 864]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 12 physical tiles
			Total Size: 115200 [D0, [11 x 9600, 9600]][Tile0, 1:[40x30], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 9600, 9600]][Tile0, 1:[40x30], 1]
		Tile0: [0, 9600, 1200], Tile1: [9600, 9600, 1200], Tile2; [19200, 9600, 1200]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 115200 [D0, [11 x 9600, 9600]][Tile0, 1:[20x15], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 9600, 9600]][Tile0, 1:[20x15], 4]
		Tile0: [0, 9600, 1200], Tile1: [0, 9600, 1200], Tile2; [0, 9600, 1200]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (40);
	KerArg0->UsedW = (unsigned short int) (40);
	KerArg0->InFeatures = (unsigned short int) (8);
	KerArg0->OutFeatures = (unsigned short int) (8);
	KerArg0->TotalInFeatures = (unsigned short int) (96);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+25440);
	KerArg0->Pad = (v4s) ((v4s){0,1,0,1});
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+25440);
	KerArg1->Feat = (unsigned short int) (8);
	KerArg1->W = (unsigned short int) (20);
	KerArg1->H = (unsigned short int) (15);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+35040);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=2400; _LC_Out=300;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+170464+0), 384, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+170464+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+19200), 384, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+170848+0), 96, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+170848+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+19584), 96, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+170944+0), 96, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+170944+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+19680), 96, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+19776), 864, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 9600, 1200, 1200, 0, &DmaR_Evt5);
	_N_In=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+171040+0), 9, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+171040+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+35040), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<12; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==11), D0Ind_NextLast = ((D0Ind+1)==11);
		/*================================= Prepare Tiles ===================================*/
		_SN_In = 0;
		if (!(D0Ind_Last)) {
			_N_In = _N_In + (9600); _LN_In = (1200); _SN_In = (8*_LN_In); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read In */
		if (_SN_In) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+9600*((D0Ind_Total+1)%2)),
					_SN_In, 1200, _LN_In, 0, &DmaR_Evt5);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+9600*((D0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (31-0*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (31-0*(1)-1*(1));
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+19776+((D0Ind)*72));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+19200+((D0Ind)*32));
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+35040))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride2B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride2B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+20640+2400*((D0Ind_Total)%2));
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+19584+((D0Ind)*8));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+19680+((D0Ind)*8));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+20640+2400*((D0Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(D0Ind_Last)) {
			_C_Out = _C_Out + (2400); _LC_Out = (300); _SC_Out = (8*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S67_Conv2d_32x96x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 42060 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 1][Tile0 Dim: 1]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 384 [Tile1, 1:[384x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[384x1], 1]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 3072 [Tile0, 1:[96x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[96x32], 1]
		Tile0: [0, 3072, 3072], Tile1: [0, 3072, 3072], Tile2; [0, 3072, 3072]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 28800 [Tile1, 1:[96x300], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[96x300], 1]
		Tile0: [0, 28800, 28800], Tile1: [0, 28800, 28800], Tile2; [0, 28800, 28800]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [Tile0, 1:[1x32], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x32], 4]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9600 [Tile1, 1:[32x300], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[32x300], 1]
		Tile0: [0, 9600, 9600], Tile1: [0, 9600, 9600], Tile2; [0, 9600, 9600]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [Tile0, 1:[1x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x32], 1]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [Tile0, 1:[1x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x32], 1]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+38796);
	KerArg0->W_In1 = (unsigned short int) (96);
	KerArg0->H_In1 = (unsigned short int) (32);
	KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+384);
	KerArg0->W_In2 = (unsigned short int) (300);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+41868);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+41996);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+42028);
	KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+29184);
	KerArg0->W_Out = (unsigned short int) (300);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+38784);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+38796), 3072, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+384), 28800, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+32992+0), 128, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+32992+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+41868), 128, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33120+0), 32, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33120+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+41996), 32, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33152+0), 32, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33152+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+42028), 32, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33184+0), 9, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33184+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+38784), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile1 */
		int T1Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->OutFirstCol = (unsigned short int) ((0)*32);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+38784))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+29184), 9600, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S70_Conv2d_192x32x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 45068 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 4][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 9600 [Tile1, 4:[32x84, 2:32x84, 32x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[32x84, 2:32x84, 32x48], 1]
		Tile0: [0, 2688, 84], Tile1: [84, 2688, 84], Tile2; [168, 2688, 84]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6144 [Tile0, 1:[32x192], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[32x192], 1]
		Tile0: [0, 6144, 6144], Tile1: [0, 6144, 6144], Tile2; [0, 6144, 6144]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 128 [Tile1, 4:[128x1, 2:128x1, 128x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[128x1, 2:128x1, 128x1], 1]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 768 [Tile0, 1:[1x192], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x192], 4]
		Tile0: [0, 768, 768], Tile1: [0, 768, 768], Tile2; [0, 768, 768]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 57600 [Tile1, 4:[192x84, 2:192x84, 192x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[192x84, 2:192x84, 192x48], 1]
		Tile0: [0, 16128, 84], Tile1: [84, 16128, 84], Tile2; [168, 16128, 84]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 192 [Tile0, 1:[1x192], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x192], 1]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 192 [Tile0, 1:[1x192], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x192], 1]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+37772);
	KerArg0->W_In1 = (unsigned short int) (32);
	KerArg0->H_In1 = (unsigned short int) (192);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+43916);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44684);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44876);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+37760);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+128+0), 2688, 300, 84, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+37772), 6144, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+43916), 768, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=16128; _LC_Out=84;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44684), 192, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+97696+0), 192, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+97696+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44876), 192, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+97888+0), 9, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+97888+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+37760), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<4; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==3), T1Ind_NextLast = ((T1Ind+1)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (84); _LN_In2 = ((T1Ind_NextLast)?48:84); _SN_In2 = (32*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+128+2688*((T1Ind_Total+1)%2)),
					_SN_In2, 300, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+128+2688*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?48:84);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+5504+16128*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?48:84);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*192);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+37760))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+5504+16128*((T1Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (84); _LC_Out = ((T1Ind_NextLast)?48:84); _SC_Out = (192*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S73_Conv2d_192x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 41292 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 192, Tiled: 12][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 12 physical tiles
			Total Size: 57600 [D0, [11 x 4800, 4800]][Tile0, 1:[20x15], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 4800, 4800]][Tile0, 1:[20x15], 1]
		Tile0: [0, 4800, 300], Tile1: [4800, 4800, 300], Tile2; [9600, 4800, 300]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 768 [D0, [11 x 64, 64]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 64, 64]]
		Tile0: [0, 768, 768], Tile1: [0, 768, 768], Tile2; [0, 768, 768]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 192 [D0, [11 x 16, 16]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 16, 16]]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 192 [D0, [11 x 16, 16]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 16, 16]]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 1728 [D0, [11 x 144, 144]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 144, 144]]
		Tile0: [0, 1728, 1728], Tile1: [0, 1728, 1728], Tile2; [0, 1728, 1728]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 12 physical tiles
			Total Size: 57600 [D0, [11 x 4800, 4800]][Tile0, 1:[20x15], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 4800, 4800]][Tile0, 1:[20x15], 1]
		Tile0: [0, 4800, 300], Tile1: [4800, 4800, 300], Tile2; [9600, 4800, 300]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 230400 [D0, [11 x 19200, 19200]][Tile0, 1:[20x15], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 19200, 19200]][Tile0, 1:[20x15], 4]
		Tile0: [0, 19200, 1200], Tile1: [0, 19200, 1200], Tile2; [0, 19200, 1200]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (20);
	KerArg0->UsedW = (unsigned short int) (20);
	KerArg0->InFeatures = (unsigned short int) (16);
	KerArg0->OutFeatures = (unsigned short int) (16);
	KerArg0->TotalInFeatures = (unsigned short int) (192);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+22080);
	KerArg0->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+22080);
	KerArg1->Feat = (unsigned short int) (16);
	KerArg1->W = (unsigned short int) (20);
	KerArg1->H = (unsigned short int) (15);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+41280);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 4800, 300, 300, 0, &DmaR_Evt1);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+9600), 768, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10368), 192, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10560), 192, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10752), 1728, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
	_C_Out=0; _SC_Out=4800; _LC_Out=300;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+41280), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<12; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==11), D0Ind_NextLast = ((D0Ind+1)==11);
		/*================================= Prepare Tiles ===================================*/
		_SN_In = 0;
		if (!(D0Ind_Last)) {
			_N_In = _N_In + (4800); _LN_In = (300); _SN_In = (16*_LN_In); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
		if (_SN_In) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+4800*((D0Ind_Total+1)%2)),
					_SN_In, 300, _LN_In, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+4800*((D0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (17-1*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (17-1*(1)-1*(1));
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+10752+((D0Ind)*144));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+9600+((D0Ind)*64));
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+41280))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride1B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride1B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+12480+4800*((D0Ind_Total)%2));
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+10368+((D0Ind)*16));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+10560+((D0Ind)*16));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+12480+4800*((D0Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(D0Ind_Last)) {
			_C_Out = _C_Out + (4800); _LC_Out = (300); _SC_Out = (16*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S76_Conv2d_32x192x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 44748 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 4][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 57600 [Tile1, 4:[192x84, 2:192x84, 192x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[192x84, 2:192x84, 192x48], 1]
		Tile0: [0, 16128, 84], Tile1: [84, 16128, 84], Tile2; [168, 16128, 84]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6144 [Tile0, 1:[192x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[192x32], 1]
		Tile0: [0, 6144, 6144], Tile1: [0, 6144, 6144], Tile2; [0, 6144, 6144]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 768 [Tile1, 4:[768x1, 2:768x1, 768x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[768x1, 2:768x1, 768x1], 1]
		Tile0: [0, 768, 768], Tile1: [0, 768, 768], Tile2; [0, 768, 768]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [Tile0, 1:[1x32], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x32], 4]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 9600 [Tile1, 4:[32x84, 2:32x84, 32x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[32x84, 2:32x84, 32x48], 1]
		Tile0: [0, 2688, 84], Tile1: [84, 2688, 84], Tile2; [168, 2688, 84]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [Tile0, 1:[1x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x32], 1]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [Tile0, 1:[1x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x32], 1]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+38412);
	KerArg0->W_In1 = (unsigned short int) (192);
	KerArg0->H_In1 = (unsigned short int) (32);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+44556);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44684);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44716);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+38400);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+768+0), 16128, 300, 84, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+38412), 6144, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44556), 128, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=2688; _LC_Out=84;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44684), 32, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44716), 32, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+38400), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<4; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==3), T1Ind_NextLast = ((T1Ind+1)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (84); _LN_In2 = ((T1Ind_NextLast)?48:84); _SN_In2 = (192*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+768+16128*((T1Ind_Total+1)%2)),
					_SN_In2, 300, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+768+16128*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?48:84);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+33024+2688*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?48:84);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*32);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+38400))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+33024+2688*((T1Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (84); _LC_Out = ((T1Ind_NextLast)?48:84); _SC_Out = (32*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S77_MatAdd_32x15x20(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 28812 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerMat3_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9600 [Tile0, 1:[1x9600], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x9600], 1]
		Tile0: [0, 9600, 9600], Tile1: [0, 9600, 9600], Tile2; [0, 9600, 9600]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9600 [Tile0, 1:[1x9600], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x9600], 1]
		Tile0: [0, 9600, 9600], Tile1: [0, 9600, 9600], Tile2; [0, 9600, 9600]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9600 [Tile0, 1:[1x9600], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x9600], 1]
		Tile0: [0, 9600, 9600], Tile1: [0, 9600, 9600], Tile2; [0, 9600, 9600]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->In2 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+9600);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+19200);
	KerArg0->Feat = (unsigned short int) (9600);
	KerArg0->DoScale = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+28800);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 9600, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+9600), 9600, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+28800), 9, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		AT_FORK(gap_ncore(), (void *) KerMatAdd_SQ8, (void *) KerArg0);
		__CALL(KerMatAdd_SQ8, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+19200), 9600, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S80_Conv2d_192x32x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 45068 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 4][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 9600 [Tile1, 4:[32x84, 2:32x84, 32x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[32x84, 2:32x84, 32x48], 1]
		Tile0: [0, 2688, 84], Tile1: [84, 2688, 84], Tile2; [168, 2688, 84]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6144 [Tile0, 1:[32x192], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[32x192], 1]
		Tile0: [0, 6144, 6144], Tile1: [0, 6144, 6144], Tile2; [0, 6144, 6144]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 128 [Tile1, 4:[128x1, 2:128x1, 128x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[128x1, 2:128x1, 128x1], 1]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 768 [Tile0, 1:[1x192], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x192], 4]
		Tile0: [0, 768, 768], Tile1: [0, 768, 768], Tile2; [0, 768, 768]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 57600 [Tile1, 4:[192x84, 2:192x84, 192x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[192x84, 2:192x84, 192x48], 1]
		Tile0: [0, 16128, 84], Tile1: [84, 16128, 84], Tile2; [168, 16128, 84]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 192 [Tile0, 1:[1x192], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x192], 1]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 192 [Tile0, 1:[1x192], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x192], 1]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+37772);
	KerArg0->W_In1 = (unsigned short int) (32);
	KerArg0->H_In1 = (unsigned short int) (192);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+43916);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44684);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44876);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+37760);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+128+0), 2688, 300, 84, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+37772), 6144, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+43916), 768, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=16128; _LC_Out=84;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44684), 192, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44876), 192, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+37760), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<4; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==3), T1Ind_NextLast = ((T1Ind+1)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (84); _LN_In2 = ((T1Ind_NextLast)?48:84); _SN_In2 = (32*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+128+2688*((T1Ind_Total+1)%2)),
					_SN_In2, 300, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+128+2688*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?48:84);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+5504+16128*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?48:84);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*192);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+37760))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+5504+16128*((T1Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (84); _LC_Out = ((T1Ind_NextLast)?48:84); _SC_Out = (192*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S83_Conv2d_192x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 41292 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 192, Tiled: 12][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 12 physical tiles
			Total Size: 57600 [D0, [11 x 4800, 4800]][Tile0, 1:[20x15], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 4800, 4800]][Tile0, 1:[20x15], 1]
		Tile0: [0, 4800, 300], Tile1: [4800, 4800, 300], Tile2; [9600, 4800, 300]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 768 [D0, [11 x 64, 64]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 64, 64]]
		Tile0: [0, 768, 768], Tile1: [0, 768, 768], Tile2; [0, 768, 768]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 192 [D0, [11 x 16, 16]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 16, 16]]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 192 [D0, [11 x 16, 16]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 16, 16]]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 1728 [D0, [11 x 144, 144]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 144, 144]]
		Tile0: [0, 1728, 1728], Tile1: [0, 1728, 1728], Tile2; [0, 1728, 1728]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 12 physical tiles
			Total Size: 57600 [D0, [11 x 4800, 4800]][Tile0, 1:[20x15], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 4800, 4800]][Tile0, 1:[20x15], 1]
		Tile0: [0, 4800, 300], Tile1: [4800, 4800, 300], Tile2; [9600, 4800, 300]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 230400 [D0, [11 x 19200, 19200]][Tile0, 1:[20x15], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 19200, 19200]][Tile0, 1:[20x15], 4]
		Tile0: [0, 19200, 1200], Tile1: [0, 19200, 1200], Tile2; [0, 19200, 1200]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (20);
	KerArg0->UsedW = (unsigned short int) (20);
	KerArg0->InFeatures = (unsigned short int) (16);
	KerArg0->OutFeatures = (unsigned short int) (16);
	KerArg0->TotalInFeatures = (unsigned short int) (192);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+22080);
	KerArg0->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+22080);
	KerArg1->Feat = (unsigned short int) (16);
	KerArg1->W = (unsigned short int) (20);
	KerArg1->H = (unsigned short int) (15);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+41280);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 4800, 300, 300, 0, &DmaR_Evt1);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+9600), 768, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10368), 192, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10560), 192, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10752), 1728, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
	_C_Out=0; _SC_Out=4800; _LC_Out=300;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+41280), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<12; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==11), D0Ind_NextLast = ((D0Ind+1)==11);
		/*================================= Prepare Tiles ===================================*/
		_SN_In = 0;
		if (!(D0Ind_Last)) {
			_N_In = _N_In + (4800); _LN_In = (300); _SN_In = (16*_LN_In); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
		if (_SN_In) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+4800*((D0Ind_Total+1)%2)),
					_SN_In, 300, _LN_In, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+4800*((D0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (17-1*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (17-1*(1)-1*(1));
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+10752+((D0Ind)*144));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+9600+((D0Ind)*64));
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+41280))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride1B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride1B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+12480+4800*((D0Ind_Total)%2));
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+10368+((D0Ind)*16));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+10560+((D0Ind)*16));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+12480+4800*((D0Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(D0Ind_Last)) {
			_C_Out = _C_Out + (4800); _LC_Out = (300); _SC_Out = (16*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S86_Conv2d_32x192x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 44748 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 4][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 57600 [Tile1, 4:[192x84, 2:192x84, 192x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[192x84, 2:192x84, 192x48], 1]
		Tile0: [0, 16128, 84], Tile1: [84, 16128, 84], Tile2; [168, 16128, 84]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6144 [Tile0, 1:[192x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[192x32], 1]
		Tile0: [0, 6144, 6144], Tile1: [0, 6144, 6144], Tile2; [0, 6144, 6144]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 768 [Tile1, 4:[768x1, 2:768x1, 768x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[768x1, 2:768x1, 768x1], 1]
		Tile0: [0, 768, 768], Tile1: [0, 768, 768], Tile2; [0, 768, 768]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [Tile0, 1:[1x32], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x32], 4]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 9600 [Tile1, 4:[32x84, 2:32x84, 32x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[32x84, 2:32x84, 32x48], 1]
		Tile0: [0, 2688, 84], Tile1: [84, 2688, 84], Tile2; [168, 2688, 84]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [Tile0, 1:[1x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x32], 1]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [Tile0, 1:[1x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x32], 1]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+38412);
	KerArg0->W_In1 = (unsigned short int) (192);
	KerArg0->H_In1 = (unsigned short int) (32);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+44556);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44684);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44716);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+38400);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+768+0), 16128, 300, 84, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+38412), 6144, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44556), 128, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=2688; _LC_Out=84;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44684), 32, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44716), 32, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+38400), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<4; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==3), T1Ind_NextLast = ((T1Ind+1)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (84); _LN_In2 = ((T1Ind_NextLast)?48:84); _SN_In2 = (192*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+768+16128*((T1Ind_Total+1)%2)),
					_SN_In2, 300, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+768+16128*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?48:84);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+33024+2688*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?48:84);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*32);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+38400))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+33024+2688*((T1Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (84); _LC_Out = ((T1Ind_NextLast)?48:84); _SC_Out = (32*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S87_MatAdd_32x15x20(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 28812 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerMat3_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9600 [Tile0, 1:[1x9600], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x9600], 1]
		Tile0: [0, 9600, 9600], Tile1: [0, 9600, 9600], Tile2; [0, 9600, 9600]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9600 [Tile0, 1:[1x9600], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x9600], 1]
		Tile0: [0, 9600, 9600], Tile1: [0, 9600, 9600], Tile2; [0, 9600, 9600]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9600 [Tile0, 1:[1x9600], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x9600], 1]
		Tile0: [0, 9600, 9600], Tile1: [0, 9600, 9600], Tile2; [0, 9600, 9600]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->In2 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+9600);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+19200);
	KerArg0->Feat = (unsigned short int) (9600);
	KerArg0->DoScale = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+28800);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 9600, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+9600), 9600, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+28800), 9, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		AT_FORK(gap_ncore(), (void *) KerMatAdd_SQ8, (void *) KerArg0);
		__CALL(KerMatAdd_SQ8, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+19200), 9600, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S90_Conv2d_192x32x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 45068 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 4][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 9600 [Tile1, 4:[32x84, 2:32x84, 32x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[32x84, 2:32x84, 32x48], 1]
		Tile0: [0, 2688, 84], Tile1: [84, 2688, 84], Tile2; [168, 2688, 84]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6144 [Tile0, 1:[32x192], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[32x192], 1]
		Tile0: [0, 6144, 6144], Tile1: [0, 6144, 6144], Tile2; [0, 6144, 6144]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 128 [Tile1, 4:[128x1, 2:128x1, 128x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[128x1, 2:128x1, 128x1], 1]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 768 [Tile0, 1:[1x192], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x192], 4]
		Tile0: [0, 768, 768], Tile1: [0, 768, 768], Tile2; [0, 768, 768]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 57600 [Tile1, 4:[192x84, 2:192x84, 192x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[192x84, 2:192x84, 192x48], 1]
		Tile0: [0, 16128, 84], Tile1: [84, 16128, 84], Tile2; [168, 16128, 84]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 192 [Tile0, 1:[1x192], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x192], 1]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 192 [Tile0, 1:[1x192], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x192], 1]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+37772);
	KerArg0->W_In1 = (unsigned short int) (32);
	KerArg0->H_In1 = (unsigned short int) (192);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+43916);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44684);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44876);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+37760);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+128+0), 2688, 300, 84, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+37772), 6144, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+43916), 768, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=16128; _LC_Out=84;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44684), 192, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44876), 192, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+37760), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<4; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==3), T1Ind_NextLast = ((T1Ind+1)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (84); _LN_In2 = ((T1Ind_NextLast)?48:84); _SN_In2 = (32*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+128+2688*((T1Ind_Total+1)%2)),
					_SN_In2, 300, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+128+2688*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?48:84);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+5504+16128*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?48:84);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*192);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+37760))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+5504+16128*((T1Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (84); _LC_Out = ((T1Ind_NextLast)?48:84); _SC_Out = (192*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S93_Conv2d_192x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 41292 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 192, Tiled: 12][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 12 physical tiles
			Total Size: 57600 [D0, [11 x 4800, 4800]][Tile0, 1:[20x15], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 4800, 4800]][Tile0, 1:[20x15], 1]
		Tile0: [0, 4800, 300], Tile1: [4800, 4800, 300], Tile2; [9600, 4800, 300]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 768 [D0, [11 x 64, 64]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 64, 64]]
		Tile0: [0, 768, 768], Tile1: [0, 768, 768], Tile2; [0, 768, 768]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 192 [D0, [11 x 16, 16]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 16, 16]]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 192 [D0, [11 x 16, 16]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 16, 16]]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 1728 [D0, [11 x 144, 144]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 144, 144]]
		Tile0: [0, 1728, 1728], Tile1: [0, 1728, 1728], Tile2; [0, 1728, 1728]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 12 physical tiles
			Total Size: 57600 [D0, [11 x 4800, 4800]][Tile0, 1:[20x15], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 4800, 4800]][Tile0, 1:[20x15], 1]
		Tile0: [0, 4800, 300], Tile1: [4800, 4800, 300], Tile2; [9600, 4800, 300]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 230400 [D0, [11 x 19200, 19200]][Tile0, 1:[20x15], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 19200, 19200]][Tile0, 1:[20x15], 4]
		Tile0: [0, 19200, 1200], Tile1: [0, 19200, 1200], Tile2; [0, 19200, 1200]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (20);
	KerArg0->UsedW = (unsigned short int) (20);
	KerArg0->InFeatures = (unsigned short int) (16);
	KerArg0->OutFeatures = (unsigned short int) (16);
	KerArg0->TotalInFeatures = (unsigned short int) (192);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+22080);
	KerArg0->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+22080);
	KerArg1->Feat = (unsigned short int) (16);
	KerArg1->W = (unsigned short int) (20);
	KerArg1->H = (unsigned short int) (15);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+41280);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 4800, 300, 300, 0, &DmaR_Evt1);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+9600), 768, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10368), 192, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10560), 192, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10752), 1728, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
	_C_Out=0; _SC_Out=4800; _LC_Out=300;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+41280), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<12; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==11), D0Ind_NextLast = ((D0Ind+1)==11);
		/*================================= Prepare Tiles ===================================*/
		_SN_In = 0;
		if (!(D0Ind_Last)) {
			_N_In = _N_In + (4800); _LN_In = (300); _SN_In = (16*_LN_In); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
		if (_SN_In) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+4800*((D0Ind_Total+1)%2)),
					_SN_In, 300, _LN_In, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+4800*((D0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (17-1*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (17-1*(1)-1*(1));
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+10752+((D0Ind)*144));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+9600+((D0Ind)*64));
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+41280))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride1B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride1B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+12480+4800*((D0Ind_Total)%2));
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+10368+((D0Ind)*16));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+10560+((D0Ind)*16));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+12480+4800*((D0Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(D0Ind_Last)) {
			_C_Out = _C_Out + (4800); _LC_Out = (300); _SC_Out = (16*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S96_Conv2d_32x192x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 44748 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 4][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 57600 [Tile1, 4:[192x84, 2:192x84, 192x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[192x84, 2:192x84, 192x48], 1]
		Tile0: [0, 16128, 84], Tile1: [84, 16128, 84], Tile2; [168, 16128, 84]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6144 [Tile0, 1:[192x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[192x32], 1]
		Tile0: [0, 6144, 6144], Tile1: [0, 6144, 6144], Tile2; [0, 6144, 6144]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 768 [Tile1, 4:[768x1, 2:768x1, 768x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[768x1, 2:768x1, 768x1], 1]
		Tile0: [0, 768, 768], Tile1: [0, 768, 768], Tile2; [0, 768, 768]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [Tile0, 1:[1x32], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x32], 4]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 9600 [Tile1, 4:[32x84, 2:32x84, 32x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[32x84, 2:32x84, 32x48], 1]
		Tile0: [0, 2688, 84], Tile1: [84, 2688, 84], Tile2; [168, 2688, 84]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [Tile0, 1:[1x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x32], 1]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [Tile0, 1:[1x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x32], 1]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+38412);
	KerArg0->W_In1 = (unsigned short int) (192);
	KerArg0->H_In1 = (unsigned short int) (32);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+44556);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44684);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44716);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+38400);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+768+0), 16128, 300, 84, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+38412), 6144, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44556), 128, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=2688; _LC_Out=84;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44684), 32, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44716), 32, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+38400), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<4; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==3), T1Ind_NextLast = ((T1Ind+1)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (84); _LN_In2 = ((T1Ind_NextLast)?48:84); _SN_In2 = (192*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+768+16128*((T1Ind_Total+1)%2)),
					_SN_In2, 300, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+768+16128*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?48:84);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+33024+2688*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?48:84);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*32);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+38400))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+33024+2688*((T1Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (84); _LC_Out = ((T1Ind_NextLast)?48:84); _SC_Out = (32*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S97_MatAdd_32x15x20(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 28812 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerMat3_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9600 [Tile0, 1:[1x9600], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x9600], 1]
		Tile0: [0, 9600, 9600], Tile1: [0, 9600, 9600], Tile2; [0, 9600, 9600]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9600 [Tile0, 1:[1x9600], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x9600], 1]
		Tile0: [0, 9600, 9600], Tile1: [0, 9600, 9600], Tile2; [0, 9600, 9600]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9600 [Tile0, 1:[1x9600], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x9600], 1]
		Tile0: [0, 9600, 9600], Tile1: [0, 9600, 9600], Tile2; [0, 9600, 9600]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->In2 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+9600);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+19200);
	KerArg0->Feat = (unsigned short int) (9600);
	KerArg0->DoScale = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+28800);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 9600, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+9600), 9600, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+28800), 9, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		AT_FORK(gap_ncore(), (void *) KerMatAdd_SQ8, (void *) KerArg0);
		__CALL(KerMatAdd_SQ8, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+19200), 9600, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S100_Conv2d_192x32x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 45068 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 4][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 9600 [Tile1, 4:[32x84, 2:32x84, 32x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[32x84, 2:32x84, 32x48], 1]
		Tile0: [0, 2688, 84], Tile1: [84, 2688, 84], Tile2; [168, 2688, 84]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6144 [Tile0, 1:[32x192], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[32x192], 1]
		Tile0: [0, 6144, 6144], Tile1: [0, 6144, 6144], Tile2; [0, 6144, 6144]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 128 [Tile1, 4:[128x1, 2:128x1, 128x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[128x1, 2:128x1, 128x1], 1]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 768 [Tile0, 1:[1x192], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x192], 4]
		Tile0: [0, 768, 768], Tile1: [0, 768, 768], Tile2; [0, 768, 768]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 57600 [Tile1, 4:[192x84, 2:192x84, 192x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[192x84, 2:192x84, 192x48], 1]
		Tile0: [0, 16128, 84], Tile1: [84, 16128, 84], Tile2; [168, 16128, 84]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 192 [Tile0, 1:[1x192], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x192], 1]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 192 [Tile0, 1:[1x192], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x192], 1]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+37772);
	KerArg0->W_In1 = (unsigned short int) (32);
	KerArg0->H_In1 = (unsigned short int) (192);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+43916);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44684);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44876);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+37760);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+128+0), 2688, 300, 84, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+37772), 6144, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+43916), 768, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=16128; _LC_Out=84;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44684), 192, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44876), 192, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+37760), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<4; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==3), T1Ind_NextLast = ((T1Ind+1)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (84); _LN_In2 = ((T1Ind_NextLast)?48:84); _SN_In2 = (32*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+128+2688*((T1Ind_Total+1)%2)),
					_SN_In2, 300, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+128+2688*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?48:84);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+5504+16128*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?48:84);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*192);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+37760))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+5504+16128*((T1Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (84); _LC_Out = ((T1Ind_NextLast)?48:84); _SC_Out = (192*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S103_Conv2d_192x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 41292 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 192, Tiled: 12][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 12 physical tiles
			Total Size: 57600 [D0, [11 x 4800, 4800]][Tile0, 1:[20x15], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 4800, 4800]][Tile0, 1:[20x15], 1]
		Tile0: [0, 4800, 300], Tile1: [4800, 4800, 300], Tile2; [9600, 4800, 300]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 768 [D0, [11 x 64, 64]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 64, 64]]
		Tile0: [0, 768, 768], Tile1: [0, 768, 768], Tile2; [0, 768, 768]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 192 [D0, [11 x 16, 16]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 16, 16]]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 192 [D0, [11 x 16, 16]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 16, 16]]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 1728 [D0, [11 x 144, 144]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 144, 144]]
		Tile0: [0, 1728, 1728], Tile1: [0, 1728, 1728], Tile2; [0, 1728, 1728]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 12 physical tiles
			Total Size: 57600 [D0, [11 x 4800, 4800]][Tile0, 1:[20x15], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 4800, 4800]][Tile0, 1:[20x15], 1]
		Tile0: [0, 4800, 300], Tile1: [4800, 4800, 300], Tile2; [9600, 4800, 300]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 12 logical tiles, 1 physical tiles
			Total Size: 230400 [D0, [11 x 19200, 19200]][Tile0, 1:[20x15], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [11 x 19200, 19200]][Tile0, 1:[20x15], 4]
		Tile0: [0, 19200, 1200], Tile1: [0, 19200, 1200], Tile2; [0, 19200, 1200]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (20);
	KerArg0->UsedW = (unsigned short int) (20);
	KerArg0->InFeatures = (unsigned short int) (16);
	KerArg0->OutFeatures = (unsigned short int) (16);
	KerArg0->TotalInFeatures = (unsigned short int) (192);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+22080);
	KerArg0->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+22080);
	KerArg1->Feat = (unsigned short int) (16);
	KerArg1->W = (unsigned short int) (20);
	KerArg1->H = (unsigned short int) (15);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+41280);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 4800, 300, 300, 0, &DmaR_Evt1);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+9600), 768, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10368), 192, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10560), 192, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10752), 1728, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
	_C_Out=0; _SC_Out=4800; _LC_Out=300;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+41280), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<12; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==11), D0Ind_NextLast = ((D0Ind+1)==11);
		/*================================= Prepare Tiles ===================================*/
		_SN_In = 0;
		if (!(D0Ind_Last)) {
			_N_In = _N_In + (4800); _LN_In = (300); _SN_In = (16*_LN_In); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
		if (_SN_In) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+4800*((D0Ind_Total+1)%2)),
					_SN_In, 300, _LN_In, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+4800*((D0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (17-1*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (17-1*(1)-1*(1));
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+10752+((D0Ind)*144));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+9600+((D0Ind)*64));
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+41280))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride1B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride1B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+12480+4800*((D0Ind_Total)%2));
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+10368+((D0Ind)*16));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+10560+((D0Ind)*16));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+12480+4800*((D0Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(D0Ind_Last)) {
			_C_Out = _C_Out + (4800); _LC_Out = (300); _SC_Out = (16*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S106_Conv2d_48x192x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 42924 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 5][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 5 logical tiles, 5 physical tiles
			Total Size: 57600 [Tile1, 5:[192x68, 3:192x68, 192x28], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 5:[192x68, 3:192x68, 192x28], 1]
		Tile0: [0, 13056, 68], Tile1: [68, 13056, 68], Tile2; [136, 13056, 68]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9216 [Tile0, 1:[192x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[192x48], 1]
		Tile0: [0, 9216, 9216], Tile1: [0, 9216, 9216], Tile2; [0, 9216, 9216]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 1 physical tiles
			Total Size: 768 [Tile1, 5:[768x1, 3:768x1, 768x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 5:[768x1, 3:768x1, 768x1], 1]
		Tile0: [0, 768, 768], Tile1: [0, 768, 768], Tile2; [0, 768, 768]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 192 [Tile0, 1:[1x48], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x48], 4]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 5 physical tiles
			Total Size: 14400 [Tile1, 5:[48x68, 3:48x68, 48x28], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 5:[48x68, 3:48x68, 48x28], 1]
		Tile0: [0, 3264, 68], Tile1: [68, 3264, 68], Tile2; [136, 3264, 68]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 48 [Tile0, 1:[1x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x48], 1]
		Tile0: [0, 48, 48], Tile1: [0, 48, 48], Tile2; [0, 48, 48]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 48 [Tile0, 1:[1x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x48], 1]
		Tile0: [0, 48, 48], Tile1: [0, 48, 48], Tile2; [0, 48, 48]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 5:[1x1, 3:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 5:[1x1, 3:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+33420);
	KerArg0->W_In1 = (unsigned short int) (192);
	KerArg0->H_In1 = (unsigned short int) (48);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+42636);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+42828);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+42876);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+33408);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+768+0), 13056, 300, 68, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+33420), 9216, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+95392+0), 192, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+95392+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+42636), 192, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=3264; _LC_Out=68;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+42828), 48, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+95632+0), 48, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+95632+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+42876), 48, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+95680+0), 9, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+95680+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+33408), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<5; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==4), T1Ind_NextLast = ((T1Ind+1)==4);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (68); _LN_In2 = ((T1Ind_NextLast)?28:68); _SN_In2 = (192*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+768+13056*((T1Ind_Total+1)%2)),
					_SN_In2, 300, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+768+13056*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?28:68);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+26880+3264*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?28:68);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*48);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+33408))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+26880+3264*((T1Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (68); _LC_Out = ((T1Ind_NextLast)?28:68); _SC_Out = (48*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S109_Conv2d_288x48x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 42636 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 8][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 14400 [Tile1, 8:[48x40, 6:48x40, 48x20], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[48x40, 6:48x40, 48x20], 1]
		Tile0: [0, 1920, 40], Tile1: [40, 1920, 40], Tile2; [80, 1920, 40]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 13824 [Tile0, 1:[48x288], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[48x288], 1]
		Tile0: [0, 13824, 13824], Tile1: [0, 13824, 13824], Tile2; [0, 13824, 13824]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 1 physical tiles
			Total Size: 192 [Tile1, 8:[192x1, 6:192x1, 192x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[192x1, 6:192x1, 192x1], 1]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1152 [Tile0, 1:[1x288], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x288], 4]
		Tile0: [0, 1152, 1152], Tile1: [0, 1152, 1152], Tile2; [0, 1152, 1152]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 86400 [Tile1, 8:[288x40, 6:288x40, 288x20], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[288x40, 6:288x40, 288x20], 1]
		Tile0: [0, 11520, 40], Tile1: [40, 11520, 40], Tile2; [80, 11520, 40]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 288 [Tile0, 1:[1x288], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x288], 1]
		Tile0: [0, 288, 288], Tile1: [0, 288, 288], Tile2; [0, 288, 288]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 288 [Tile0, 1:[1x288], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x288], 1]
		Tile0: [0, 288, 288], Tile1: [0, 288, 288], Tile2; [0, 288, 288]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 8:[1x1, 6:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[1x1, 6:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+27084);
	KerArg0->W_In1 = (unsigned short int) (48);
	KerArg0->H_In1 = (unsigned short int) (288);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+40908);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+42060);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+42348);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+27072);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+192+0), 1920, 300, 40, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+0), 13824, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read In1 */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+27084), 13824, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+40908), 1152, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=11520; _LC_Out=40;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+40960+0), 288, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+40960+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+42060), 288, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+41248+0), 288, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+41248+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+42348), 288, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+41536+0), 9, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+41536+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+27072), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<8; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==7), T1Ind_NextLast = ((T1Ind+1)==7);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (40); _LN_In2 = ((T1Ind_NextLast)?20:40); _SN_In2 = (48*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+192+1920*((T1Ind_Total+1)%2)),
					_SN_In2, 300, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+192+1920*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?20:40);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+4032+11520*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?20:40);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*288);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+27072))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+4032+11520*((T1Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (40); _LC_Out = ((T1Ind_NextLast)?20:40); _SC_Out = (288*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S112_Conv2d_288x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 42732 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERRAM_CL_EVENT UchanHR4;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 288, Tiled: 18][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 18 logical tiles, 18 physical tiles
			Total Size: 86400 [D0, [17 x 4800, 4800]][Tile0, 1:[20x15], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [17 x 4800, 4800]][Tile0, 1:[20x15], 1]
		Tile0: [0, 4800, 300], Tile1: [4800, 4800, 300], Tile2; [9600, 4800, 300]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 18 logical tiles, 1 physical tiles
			Total Size: 1152 [D0, [17 x 64, 64]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [17 x 64, 64]]
		Tile0: [0, 1152, 1152], Tile1: [0, 1152, 1152], Tile2; [0, 1152, 1152]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 18 logical tiles, 1 physical tiles
			Total Size: 288 [D0, [17 x 16, 16]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [17 x 16, 16]]
		Tile0: [0, 288, 288], Tile1: [0, 288, 288], Tile2; [0, 288, 288]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 18 logical tiles, 1 physical tiles
			Total Size: 288 [D0, [17 x 16, 16]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [17 x 16, 16]]
		Tile0: [0, 288, 288], Tile1: [0, 288, 288], Tile2; [0, 288, 288]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 18 logical tiles, 1 physical tiles
			Total Size: 2592 [D0, [17 x 144, 144]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [17 x 144, 144]]
		Tile0: [0, 2592, 2592], Tile1: [0, 2592, 2592], Tile2; [0, 2592, 2592]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 18 logical tiles, 18 physical tiles
			Total Size: 86400 [D0, [17 x 4800, 4800]][Tile0, 1:[20x15], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [17 x 4800, 4800]][Tile0, 1:[20x15], 1]
		Tile0: [0, 4800, 300], Tile1: [4800, 4800, 300], Tile2; [9600, 4800, 300]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 18 logical tiles, 1 physical tiles
			Total Size: 345600 [D0, [17 x 19200, 19200]][Tile0, 1:[20x15], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [17 x 19200, 19200]][Tile0, 1:[20x15], 4]
		Tile0: [0, 19200, 1200], Tile1: [0, 19200, 1200], Tile2; [0, 19200, 1200]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (20);
	KerArg0->UsedW = (unsigned short int) (20);
	KerArg0->InFeatures = (unsigned short int) (16);
	KerArg0->OutFeatures = (unsigned short int) (16);
	KerArg0->TotalInFeatures = (unsigned short int) (288);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+23520);
	KerArg0->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+23520);
	KerArg1->Feat = (unsigned short int) (16);
	KerArg1->W = (unsigned short int) (20);
	KerArg1->H = (unsigned short int) (15);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+42720);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 4800, 300, 300, 0, &DmaR_Evt1);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+9600), 1152, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+32992+0), 288, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+32992+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10752), 288, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33280+0), 288, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33280+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+11040), 288, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+11328), 2592, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
	_C_Out=0; _SC_Out=4800; _LC_Out=300;
	_SPP_Out=0; _SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33568+0), 9, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+33568+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+42720), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<18; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==17), D0Ind_NextLast = ((D0Ind+1)==17);
		/*================================= Prepare Tiles ===================================*/
		_SN_In = 0;
		if (!(D0Ind_Last)) {
			_N_In = _N_In + (4800); _LN_In = (300); _SN_In = (16*_LN_In); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
		if (_SN_In) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+4800*((D0Ind_Total+1)%2)),
					_SN_In, 300, _LN_In, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+4800*((D0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (17-1*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (17-1*(1)-1*(1));
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+11328+((D0Ind)*144));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+9600+((D0Ind)*64));
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+42720))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride1B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride1B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+13920+4800*((D0Ind_Total)%2));
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+10752+((D0Ind)*16));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+11040+((D0Ind)*16));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR4); /* Wait previous uDMA write Out */
		if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+4800*((D0Ind_Total+-1)%2)),
					_SP_Out, 300, _LP_Out, 1, &UchanHR4);
		AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+4800*((D0Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+13920+4800*((D0Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
		_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(D0Ind_Last)) {
			_C_Out = _C_Out + (4800); _LC_Out = (300); _SC_Out = (16*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR4); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+4800*((D0Ind_Total+-1)%2)), _SP_Out, 300, _LP_Out, 1, &UchanHR4);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR4); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S115_Conv2d_48x288x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 36780 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF5;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _NN_In2;
	unsigned int _SN_In2, _SNN_In2;
	unsigned int _LN_In2, _LNN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 10][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 10 logical tiles, 10 physical tiles
			Total Size: 86400 [Tile1, 10:[288x32, 8:288x32, 288x12], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 10:[288x32, 8:288x32, 288x12], 1]
		Tile0: [0, 9216, 32], Tile1: [32, 9216, 32], Tile2; [64, 9216, 32]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 13824 [Tile0, 1:[288x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[288x48], 1]
		Tile0: [0, 13824, 13824], Tile1: [0, 13824, 13824], Tile2; [0, 13824, 13824]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 10 logical tiles, 1 physical tiles
			Total Size: 1152 [Tile1, 10:[1152x1, 8:1152x1, 1152x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 10:[1152x1, 8:1152x1, 1152x1], 1]
		Tile0: [0, 1152, 1152], Tile1: [0, 1152, 1152], Tile2; [0, 1152, 1152]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 192 [Tile0, 1:[1x48], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x48], 4]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 10 logical tiles, 10 physical tiles
			Total Size: 14400 [Tile1, 10:[48x32, 8:48x32, 48x12], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 10:[48x32, 8:48x32, 48x12], 1]
		Tile0: [0, 1536, 32], Tile1: [32, 1536, 32], Tile2; [64, 1536, 32]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 48 [Tile0, 1:[1x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x48], 1]
		Tile0: [0, 48, 48], Tile1: [0, 48, 48], Tile2; [0, 48, 48]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 48 [Tile0, 1:[1x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x48], 1]
		Tile0: [0, 48, 48], Tile1: [0, 48, 48], Tile2; [0, 48, 48]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 10 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 10:[1x1, 8:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 10:[1x1, 8:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+22668);
	KerArg0->W_In1 = (unsigned short int) (288);
	KerArg0->H_In1 = (unsigned short int) (48);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+36492);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+36684);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+36732);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+22656);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37792+0), 9216, 300, 32, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+32), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37792+9216), 9216, 300, 32, 0, &UchanHR1);
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37792+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1152+0), 9216, 0, &DmaR_Evt1);
	_NN_In2=32; _SN_In2=9216;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+22668), 13824, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+70048+0), 192, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+70048+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+36492), 192, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=1536; _LC_Out=32;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+70240+0), 48, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+70240+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+36684), 48, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+70288+0), 48, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+70288+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+36732), 48, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+70336+0), 9, 0, &UchanHF5);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+70336+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+22656), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<10; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==9), T1Ind_NextLast = ((T1Ind+1)==9), T1Ind_NextNextLast = ((T1Ind+2)==9);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In2 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In2 = _NN_In2 + (32); _LNN_In2 = ((T1Ind_NextNextLast)?12:32); _SNN_In2 = (288*_LNN_In2); 
			}
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
		if (_SNN_In2) {
			AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+_NN_In2), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37792+9216*((T1Ind_Total)%2)),
					_SNN_In2, 300, _LNN_In2, 0, &UchanHR1);
		}
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37792+9216*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1152+9216*((T1Ind_Total+1)%2)),
					_SN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+1152+9216*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?12:32);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+19584+1536*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?12:32);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*48);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+22656))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+19584+1536*((T1Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SN_In2 = _SNN_In2;_LN_In2 = _LNN_In2;
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (32); _LC_Out = ((T1Ind_NextLast)?12:32); _SC_Out = (48*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S116_MatAdd_48x15x20(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 43212 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	KerMat3_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 14400 [Tile0, 1:[1x14400], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x14400], 1]
		Tile0: [0, 14400, 14400], Tile1: [0, 14400, 14400], Tile2; [0, 14400, 14400]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 14400 [Tile0, 1:[1x14400], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x14400], 1]
		Tile0: [0, 14400, 14400], Tile1: [0, 14400, 14400], Tile2; [0, 14400, 14400]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 14400 [Tile0, 1:[1x14400], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x14400], 1]
		Tile0: [0, 14400, 14400], Tile1: [0, 14400, 14400], Tile2; [0, 14400, 14400]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->In2 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+14400);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+28800);
	KerArg0->Feat = (unsigned short int) (14400);
	KerArg0->DoScale = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+43200);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 14400, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+14400), 14400, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+43200), 9, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		AT_FORK(gap_ncore(), (void *) KerMatAdd_SQ8, (void *) KerArg0);
		__CALL(KerMatAdd_SQ8, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37792+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+28800), 14400, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37792+0), 14400, 1, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S119_Conv2d_288x48x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 42636 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _NN_In2;
	unsigned int _SN_In2, _SNN_In2;
	unsigned int _LN_In2, _LNN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 8][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 14400 [Tile1, 8:[48x40, 6:48x40, 48x20], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[48x40, 6:48x40, 48x20], 1]
		Tile0: [0, 1920, 40], Tile1: [40, 1920, 40], Tile2; [80, 1920, 40]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 13824 [Tile0, 1:[48x288], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[48x288], 1]
		Tile0: [0, 13824, 13824], Tile1: [0, 13824, 13824], Tile2; [0, 13824, 13824]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 1 physical tiles
			Total Size: 192 [Tile1, 8:[192x1, 6:192x1, 192x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[192x1, 6:192x1, 192x1], 1]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1152 [Tile0, 1:[1x288], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x288], 4]
		Tile0: [0, 1152, 1152], Tile1: [0, 1152, 1152], Tile2; [0, 1152, 1152]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 86400 [Tile1, 8:[288x40, 6:288x40, 288x20], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[288x40, 6:288x40, 288x20], 1]
		Tile0: [0, 11520, 40], Tile1: [40, 11520, 40], Tile2; [80, 11520, 40]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 288 [Tile0, 1:[1x288], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x288], 1]
		Tile0: [0, 288, 288], Tile1: [0, 288, 288], Tile2; [0, 288, 288]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 288 [Tile0, 1:[1x288], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x288], 1]
		Tile0: [0, 288, 288], Tile1: [0, 288, 288], Tile2; [0, 288, 288]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 8:[1x1, 6:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[1x1, 6:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+27084);
	KerArg0->W_In1 = (unsigned short int) (48);
	KerArg0->H_In1 = (unsigned short int) (288);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+40908);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+42060);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+42348);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+27072);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+109792+0), 1920, 300, 40, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+40), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+109792+1920), 1920, 300, 40, 0, &UchanHR1);
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+109792+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+192+0), 1920, 0, &DmaR_Evt1);
	_NN_In2=40; _SN_In2=1920;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+27084), 13824, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+40908), 1152, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=11520; _LC_Out=40;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+42060), 288, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+42348), 288, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+27072), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<8; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==7), T1Ind_NextLast = ((T1Ind+1)==7), T1Ind_NextNextLast = ((T1Ind+2)==7);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In2 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In2 = _NN_In2 + (40); _LNN_In2 = ((T1Ind_NextNextLast)?20:40); _SNN_In2 = (48*_LNN_In2); 
			}
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In2 */
		if (_SNN_In2) {
			AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+_NN_In2), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+109792+1920*((T1Ind_Total)%2)),
					_SNN_In2, 300, _LNN_In2, 0, &UchanHR1);
		}
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+109792+1920*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+192+1920*((T1Ind_Total+1)%2)),
					_SN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+192+1920*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?20:40);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+4032+11520*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?20:40);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*288);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+27072))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+4032+11520*((T1Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SN_In2 = _SNN_In2;_LN_In2 = _LNN_In2;
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (40); _LC_Out = ((T1Ind_NextLast)?20:40); _SC_Out = (288*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S122_Conv2d_288x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 42732 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF5;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 288, Tiled: 18][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 18 logical tiles, 18 physical tiles
			Total Size: 86400 [D0, [17 x 4800, 4800]][Tile0, 1:[20x15], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [17 x 4800, 4800]][Tile0, 1:[20x15], 1]
		Tile0: [0, 4800, 300], Tile1: [4800, 4800, 300], Tile2; [9600, 4800, 300]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 18 logical tiles, 1 physical tiles
			Total Size: 1152 [D0, [17 x 64, 64]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [17 x 64, 64]]
		Tile0: [0, 1152, 1152], Tile1: [0, 1152, 1152], Tile2; [0, 1152, 1152]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 18 logical tiles, 1 physical tiles
			Total Size: 288 [D0, [17 x 16, 16]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [17 x 16, 16]]
		Tile0: [0, 288, 288], Tile1: [0, 288, 288], Tile2; [0, 288, 288]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 18 logical tiles, 1 physical tiles
			Total Size: 288 [D0, [17 x 16, 16]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [17 x 16, 16]]
		Tile0: [0, 288, 288], Tile1: [0, 288, 288], Tile2; [0, 288, 288]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 18 logical tiles, 1 physical tiles
			Total Size: 2592 [D0, [17 x 144, 144]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [17 x 144, 144]]
		Tile0: [0, 2592, 2592], Tile1: [0, 2592, 2592], Tile2; [0, 2592, 2592]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 18 logical tiles, 18 physical tiles
			Total Size: 86400 [D0, [17 x 4800, 4800]][Tile0, 1:[20x15], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [17 x 4800, 4800]][Tile0, 1:[20x15], 1]
		Tile0: [0, 4800, 300], Tile1: [4800, 4800, 300], Tile2; [9600, 4800, 300]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 18 logical tiles, 1 physical tiles
			Total Size: 345600 [D0, [17 x 19200, 19200]][Tile0, 1:[20x15], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [17 x 19200, 19200]][Tile0, 1:[20x15], 4]
		Tile0: [0, 19200, 1200], Tile1: [0, 19200, 1200], Tile2; [0, 19200, 1200]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (20);
	KerArg0->UsedW = (unsigned short int) (20);
	KerArg0->InFeatures = (unsigned short int) (16);
	KerArg0->OutFeatures = (unsigned short int) (16);
	KerArg0->TotalInFeatures = (unsigned short int) (288);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+23520);
	KerArg0->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+23520);
	KerArg1->Feat = (unsigned short int) (16);
	KerArg1->W = (unsigned short int) (20);
	KerArg1->H = (unsigned short int) (15);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+42720);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 4800, 300, 300, 0, &DmaR_Evt1);
	_N_In=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+198784+0), 1152, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+198784+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+9600), 1152, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+199936+0), 288, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+199936+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10752), 288, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+200224+0), 288, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+200224+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+11040), 288, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Filter+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+196192+0), 2592, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read Filter */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+196192+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+11328), 2592, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
	_C_Out=0; _SC_Out=4800; _LC_Out=300;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+200512+0), 9, 0, &UchanHF5);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+200512+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+42720), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<18; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==17), D0Ind_NextLast = ((D0Ind+1)==17);
		/*================================= Prepare Tiles ===================================*/
		_SN_In = 0;
		if (!(D0Ind_Last)) {
			_N_In = _N_In + (4800); _LN_In = (300); _SN_In = (16*_LN_In); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
		if (_SN_In) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+4800*((D0Ind_Total+1)%2)),
					_SN_In, 300, _LN_In, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+4800*((D0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (17-1*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (17-1*(1)-1*(1));
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+11328+((D0Ind)*144));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+9600+((D0Ind)*64));
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+42720))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride1B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride1B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+13920+4800*((D0Ind_Total)%2));
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+10752+((D0Ind)*16));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+11040+((D0Ind)*16));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+13920+4800*((D0Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(D0Ind_Last)) {
			_C_Out = _C_Out + (4800); _LC_Out = (300); _SC_Out = (16*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S125_Conv2d_48x288x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 36780 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF5;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 10][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 10 logical tiles, 10 physical tiles
			Total Size: 86400 [Tile1, 10:[288x32, 8:288x32, 288x12], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 10:[288x32, 8:288x32, 288x12], 1]
		Tile0: [0, 9216, 32], Tile1: [32, 9216, 32], Tile2; [64, 9216, 32]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 13824 [Tile0, 1:[288x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[288x48], 1]
		Tile0: [0, 13824, 13824], Tile1: [0, 13824, 13824], Tile2; [0, 13824, 13824]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 10 logical tiles, 1 physical tiles
			Total Size: 1152 [Tile1, 10:[1152x1, 8:1152x1, 1152x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 10:[1152x1, 8:1152x1, 1152x1], 1]
		Tile0: [0, 1152, 1152], Tile1: [0, 1152, 1152], Tile2; [0, 1152, 1152]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 192 [Tile0, 1:[1x48], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x48], 4]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 10 logical tiles, 10 physical tiles
			Total Size: 14400 [Tile1, 10:[48x32, 8:48x32, 48x12], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 10:[48x32, 8:48x32, 48x12], 1]
		Tile0: [0, 1536, 32], Tile1: [32, 1536, 32], Tile2; [64, 1536, 32]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 48 [Tile0, 1:[1x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x48], 1]
		Tile0: [0, 48, 48], Tile1: [0, 48, 48], Tile2; [0, 48, 48]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 48 [Tile0, 1:[1x48], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x48], 1]
		Tile0: [0, 48, 48], Tile1: [0, 48, 48], Tile2; [0, 48, 48]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 10 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 10:[1x1, 8:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 10:[1x1, 8:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+22668);
	KerArg0->W_In1 = (unsigned short int) (288);
	KerArg0->H_In1 = (unsigned short int) (48);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+36492);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+36684);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+36732);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+22656);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1152+0), 9216, 300, 32, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+0), 13824, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read In1 */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+22668), 13824, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37216+0), 192, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37216+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+36492), 192, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=1536; _LC_Out=32;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37408+0), 48, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37408+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+36684), 48, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37456+0), 48, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37456+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+36732), 48, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37504+0), 9, 0, &UchanHF5);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+37504+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+22656), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<10; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==9), T1Ind_NextLast = ((T1Ind+1)==9);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (32); _LN_In2 = ((T1Ind_NextLast)?12:32); _SN_In2 = (288*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1152+9216*((T1Ind_Total+1)%2)),
					_SN_In2, 300, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+1152+9216*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?12:32);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+19584+1536*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?12:32);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*48);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+22656))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+19584+1536*((T1Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (32); _LC_Out = ((T1Ind_NextLast)?12:32); _SC_Out = (48*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S126_MatAdd_48x15x20(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 43212 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	KerMat3_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 14400 [Tile0, 1:[1x14400], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x14400], 1]
		Tile0: [0, 14400, 14400], Tile1: [0, 14400, 14400], Tile2; [0, 14400, 14400]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 14400 [Tile0, 1:[1x14400], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x14400], 1]
		Tile0: [0, 14400, 14400], Tile1: [0, 14400, 14400], Tile2; [0, 14400, 14400]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 14400 [Tile0, 1:[1x14400], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x14400], 1]
		Tile0: [0, 14400, 14400], Tile1: [0, 14400, 14400], Tile2; [0, 14400, 14400]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->In2 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+14400);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+28800);
	KerArg0->Feat = (unsigned short int) (14400);
	KerArg0->DoScale = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+43200);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+0), ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+0), 14400, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In1 */
	AT_L2_COPY(0, ((AT_HYPERRAM_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23392+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 14400, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+14400), 14400, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+43200), 9, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		AT_FORK(gap_ncore(), (void *) KerMatAdd_SQ8, (void *) KerArg0);
		__CALL(KerMatAdd_SQ8, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+28800), 14400, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S129_Conv2d_288x48x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 42636 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 8][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 14400 [Tile1, 8:[48x40, 6:48x40, 48x20], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[48x40, 6:48x40, 48x20], 1]
		Tile0: [0, 1920, 40], Tile1: [40, 1920, 40], Tile2; [80, 1920, 40]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 13824 [Tile0, 1:[48x288], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[48x288], 1]
		Tile0: [0, 13824, 13824], Tile1: [0, 13824, 13824], Tile2; [0, 13824, 13824]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 1 physical tiles
			Total Size: 192 [Tile1, 8:[192x1, 6:192x1, 192x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[192x1, 6:192x1, 192x1], 1]
		Tile0: [0, 192, 192], Tile1: [0, 192, 192], Tile2; [0, 192, 192]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1152 [Tile0, 1:[1x288], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x288], 4]
		Tile0: [0, 1152, 1152], Tile1: [0, 1152, 1152], Tile2; [0, 1152, 1152]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 86400 [Tile1, 8:[288x40, 6:288x40, 288x20], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[288x40, 6:288x40, 288x20], 1]
		Tile0: [0, 11520, 40], Tile1: [40, 11520, 40], Tile2; [80, 11520, 40]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 288 [Tile0, 1:[1x288], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x288], 1]
		Tile0: [0, 288, 288], Tile1: [0, 288, 288], Tile2; [0, 288, 288]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 288 [Tile0, 1:[1x288], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x288], 1]
		Tile0: [0, 288, 288], Tile1: [0, 288, 288], Tile2; [0, 288, 288]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 8:[1x1, 6:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[1x1, 6:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+27084);
	KerArg0->W_In1 = (unsigned short int) (48);
	KerArg0->H_In1 = (unsigned short int) (288);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+40908);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+42060);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+42348);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+27072);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+192+0), 1920, 300, 40, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+27084), 13824, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+40908), 1152, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=11520; _LC_Out=40;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+42060), 288, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+42348), 288, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+27072), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<8; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==7), T1Ind_NextLast = ((T1Ind+1)==7);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (40); _LN_In2 = ((T1Ind_NextLast)?20:40); _SN_In2 = (48*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+192+1920*((T1Ind_Total+1)%2)),
					_SN_In2, 300, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+192+1920*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?20:40);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+4032+11520*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?20:40);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*288);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+27072))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+4032+11520*((T1Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (40); _LC_Out = ((T1Ind_NextLast)?20:40); _SC_Out = (288*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S132_Conv2d_288x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 47532 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 288, Tiled: 8][Tile0 Dim: 1]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 23040 [D0, [7 x 3200, 640]][Tile0, 1:[10x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [7 x 3200, 640]][Tile0, 1:[10x8], 1]
		Tile0: [0, 3200, 80], Tile1: [3200, 3200, 80], Tile2; [6400, 3200, 80]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 1 physical tiles
			Total Size: 1152 [D0, [7 x 160, 32]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [7 x 160, 32]]
		Tile0: [0, 1152, 1152], Tile1: [0, 1152, 1152], Tile2; [0, 1152, 1152]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 1 physical tiles
			Total Size: 288 [D0, [7 x 40, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [7 x 40, 8]]
		Tile0: [0, 288, 288], Tile1: [0, 288, 288], Tile2; [0, 288, 288]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 1 physical tiles
			Total Size: 288 [D0, [7 x 40, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [7 x 40, 8]]
		Tile0: [0, 288, 288], Tile1: [0, 288, 288], Tile2; [0, 288, 288]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 1 physical tiles
			Total Size: 2592 [D0, [7 x 360, 72]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [7 x 360, 72]]
		Tile0: [0, 2592, 2592], Tile1: [0, 2592, 2592], Tile2; [0, 2592, 2592]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 86400 [D0, [7 x 12000, 2400]][Tile0, 1:[20x15], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [7 x 12000, 2400]][Tile0, 1:[20x15], 1]
		Tile0: [0, 12000, 300], Tile1: [12000, 12000, 300], Tile2; [24000, 12000, 300]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 1 physical tiles
			Total Size: 92160 [D0, [7 x 12800, 2560]][Tile0, 1:[10x8], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [7 x 12800, 2560]][Tile0, 1:[10x8], 4]
		Tile0: [0, 12800, 320], Tile1: [0, 12800, 320], Tile2; [0, 12800, 320]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (20);
	KerArg0->UsedW = (unsigned short int) (20);
	KerArg0->TotalInFeatures = (unsigned short int) (288);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+34720);
	KerArg0->Pad = (v4s) ((v4s){0,1,1,1});
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+34720);
	KerArg1->W = (unsigned short int) (10);
	KerArg1->H = (unsigned short int) (8);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+47520);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=3200; _LC_Out=80;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+24000), 1152, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+170496+0), 288, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+170496+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+25152), 288, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+170784+0), 288, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+170784+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+25440), 288, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+25728), 2592, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 12000, 300, 300, 0, &DmaR_Evt5);
	_N_In=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+171072+0), 9, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+171072+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+47520), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<8; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==7), D0Ind_NextLast = ((D0Ind+1)==7);
		/*================================= Prepare Tiles ===================================*/
		_SN_In = 0;
		if (!(D0Ind_Last)) {
			_N_In = _N_In + (12000); _LN_In = (300); _SN_In = (((D0Ind_NextLast)?8:40)*_LN_In); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read In */
		if (_SN_In) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+12000*((D0Ind_Total+1)%2)),
					_SN_In, 300, _LN_In, 0, &DmaR_Evt5);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+12000*((D0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (17-1*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (17-1*(1)-1*(1));
			KerArg0->InFeatures = (unsigned short int) ((D0Ind_Last)?8:40);
			KerArg0->OutFeatures = (unsigned short int) ((D0Ind_Last)?8:40);
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+25728+((D0Ind)*360));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+24000+((D0Ind)*160));
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+47520))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride2B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride2B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+28320+3200*((D0Ind_Total)%2));
			KerArg1->Feat = (unsigned short int) ((D0Ind_Last)?8:40);
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+25152+((D0Ind)*40));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+25440+((D0Ind)*40));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+28320+3200*((D0Ind_Total)%2)),
				_SC_Out, 80, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(D0Ind_Last)) {
			_C_Out = _C_Out + (3200); _LC_Out = (80); _SC_Out = (((D0Ind_NextLast)?8:40)*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S135_Conv2d_80x288x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 47212 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Last;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 1][Tile0 Dim: 3]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 3 logical tiles, 3 physical tiles
			Total Size: 23040 [Tile0, 3:[288x28, 1:288x28, 288x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[288x28, 1:288x28, 288x24], 1]
		Tile0: [0, 8064, 28], Tile1: [28, 8064, 28], Tile2; [56, 6912, 24]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 23040 [Tile1, 1:[288x80], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[288x80], 1]
		Tile0: [0, 23040, 23040], Tile1: [0, 23040, 23040], Tile2; [0, 23040, 23040]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 3 logical tiles, 1 physical tiles
			Total Size: 1152 [Tile0, 3:[1152x1, 1:1152x1, 1152x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[1152x1, 1:1152x1, 1152x1], 1]
		Tile0: [0, 1152, 1152], Tile1: [0, 1152, 1152], Tile2; [0, 1152, 1152]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 320 [Tile1, 1:[1x80], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x80], 4]
		Tile0: [0, 320, 320], Tile1: [0, 320, 320], Tile2; [0, 320, 320]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6400 [Tile1, 1:[80x80], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[80x80], 1]
		Tile0: [0, 6400, 6400], Tile1: [0, 6400, 6400], Tile2; [0, 6400, 6400]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 80 [Tile1, 1:[1x80], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x80], 1]
		Tile0: [0, 80, 80], Tile1: [0, 80, 80], Tile2; [0, 80, 80]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 80 [Tile1, 1:[1x80], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x80], 1]
		Tile0: [0, 80, 80], Tile1: [0, 80, 80], Tile2; [0, 80, 80]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 3 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 3:[1x1, 1:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[1x1, 1:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->W_In1 = (unsigned short int) (288);
	KerArg0->H_In1 = (unsigned short int) (80);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+23040);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+29760);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+29840);
	KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+23360);
	KerArg0->W_Out = (unsigned short int) (80);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+29920);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+47200);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+31072+0), 8064, 80, 28, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 23040, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+29792+0), 320, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+29792+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+23040), 320, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+30112+0), 80, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+30112+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+29760), 80, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+30192+0), 80, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+30192+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+29840), 80, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+30272+0), 9, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+30272+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+47200), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile1 */
		int T1Ind_Last = 1;
		for (T0Ind=0; T0Ind<3; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==2), T0Ind_NextLast = ((T0Ind+1)==2);
			/*================================= Prepare Tiles ===================================*/
			_SN_In2 = 0;
			if (!(T0Ind_Last)) {
				_N_In2 = _N_In2 + (28); _LN_In2 = ((T0Ind_NextLast)?24:28); _SN_In2 = (288*_LN_In2); 
			} else if (!(1)) {
				_N_In2 = _N_In2 + (-56); _LN_In2 = (28); _SN_In2 = (288*_LN_In2); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
			if (_SN_In2) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+31072+8064*((T0Ind_Total+1)%2)),
						_SN_In2, 80, _LN_In2, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+31072+8064*((T0Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T0Ind_Last)?24:28);
			KerArg0->OutFirstCol = (unsigned short int) ((T0Ind)*28);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+47200))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
			/*================================= Update Arg Pipeline =============================*/
			/*============================= End Update Arg Pipeline =============================*/
		} /* End iteration on Tile0 */
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+23360), 6400, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S138_Conv2d_480x80x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50572 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In1;
	unsigned int _SN_In1;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 4][Tile0 Dim: 1]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 320 [Tile0, 1:[320x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[320x1], 1]
		Tile0: [0, 320, 320], Tile1: [0, 320, 320], Tile2; [0, 320, 320]
	Ker Arg: In1, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 38400 [Tile1, 4:[80x128, 2:80x128, 80x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[80x128, 2:80x128, 80x96], 1]
		Tile0: [0, 10240, 10240], Tile1: [10240, 10240, 10240], Tile2; [20480, 10240, 10240]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6400 [Tile0, 1:[80x80], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[80x80], 1]
		Tile0: [0, 6400, 6400], Tile1: [0, 6400, 6400], Tile2; [0, 6400, 6400]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 1920 [Tile1, 4:[1x128, 2:1x128, 1x96], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x128, 2:1x128, 1x96], 4]
		Tile0: [0, 1920, 1920], Tile1: [0, 1920, 1920], Tile2; [0, 1920, 1920]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 38400 [Tile1, 4:[80x128, 2:80x128, 80x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[80x128, 2:80x128, 80x96], 1]
		Tile0: [0, 10240, 10240], Tile1: [10240, 10240, 10240], Tile2; [20480, 10240, 10240]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 480 [Tile1, 4:[1x128, 2:1x128, 1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x128, 2:1x128, 1x96], 1]
		Tile0: [0, 480, 480], Tile1: [0, 480, 480], Tile2; [0, 480, 480]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 480 [Tile1, 4:[1x128, 2:1x128, 1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x128, 2:1x128, 1x96], 1]
		Tile0: [0, 480, 480], Tile1: [0, 480, 480], Tile2; [0, 480, 480]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (80);
	KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44160);
	KerArg0->W_In2 = (unsigned short int) (80);
	KerArg0->W_Out = (unsigned short int) (80);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+43840);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+50560);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 10240, 0, &DmaR_Evt1);
	_N_In1=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44160), 6400, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+20480), 1920, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=10240;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+42880), 480, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+43360), 480, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+113792+0), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+113792+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50560), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<4; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==3), T1Ind_NextLast = ((T1Ind+1)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_In1 = 0;
		if (!(T1Ind_Last)) {
			_N_In1 = _N_In1 + (10240); _SN_In1 = ((T1Ind_NextLast)?7680:10240); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+_N_In1), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+10240*((T1Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+10240*((T1Ind_Total)%2));
			KerArg0->H_In1 = (unsigned short int) (T1Ind_Last?96:128);
			KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+20480+(512*(T1Ind)));
			KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+42880+(128*(T1Ind)));
			KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+43360+(128*(T1Ind)));
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+22400+10240*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((0)*80);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+50560))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+22400+10240*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (10240); _SC_Out = ((T1Ind_NextLast)?7680:10240); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S141_Conv2d_480x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50268 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 480, Tiled: 7][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 38400 [D0, [6 x 5760, 3840]][Tile0, 1:[10x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 5760, 3840]][Tile0, 1:[10x8], 1]
		Tile0: [0, 5760, 80], Tile1: [5760, 5760, 80], Tile2; [11520, 5760, 80]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 1920 [D0, [6 x 288, 192]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 288, 192]]
		Tile0: [0, 1920, 1920], Tile1: [0, 1920, 1920], Tile2; [0, 1920, 1920]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 480 [D0, [6 x 72, 48]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 72, 48]]
		Tile0: [0, 480, 480], Tile1: [0, 480, 480], Tile2; [0, 480, 480]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 480 [D0, [6 x 72, 48]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 72, 48]]
		Tile0: [0, 480, 480], Tile1: [0, 480, 480], Tile2; [0, 480, 480]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 4320 [D0, [6 x 648, 432]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 648, 432]]
		Tile0: [0, 648, 648], Tile1: [648, 648, 648], Tile2; [1296, 648, 648]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 38400 [D0, [6 x 5760, 3840]][Tile0, 1:[10x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 5760, 3840]][Tile0, 1:[10x8], 1]
		Tile0: [0, 5760, 80], Tile1: [5760, 5760, 80], Tile2; [11520, 5760, 80]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 153600 [D0, [6 x 23040, 15360]][Tile0, 1:[10x8], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 23040, 15360]][Tile0, 1:[10x8], 4]
		Tile0: [0, 23040, 320], Tile1: [0, 23040, 320], Tile2; [0, 23040, 320]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (10);
	KerArg0->UsedW = (unsigned short int) (10);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+27216);
	KerArg0->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+27216);
	KerArg1->W = (unsigned short int) (10);
	KerArg1->H = (unsigned short int) (8);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+50256);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 5760, 80, 80, 0, &DmaR_Evt1);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+11520), 1920, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+13440), 480, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+13920), 480, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+14400+0), 648, 0, &DmaR_Evt5);
	_N_Filter=0;
	_C_Out=0; _SC_Out=5760; _LC_Out=80;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+113792+0), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+113792+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50256), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<7; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==6), D0Ind_NextLast = ((D0Ind+1)==6);
		/*================================= Prepare Tiles ===================================*/
		_SN_In = 0;
		if (!(D0Ind_Last)) {
			_N_In = _N_In + (5760); _LN_In = (80); _SN_In = (((D0Ind_NextLast)?48:72)*_LN_In); 
		}
		_SN_Filter = 0;
		if (!(D0Ind_Last)) {
			_N_Filter = _N_Filter + (648); _SN_Filter = ((D0Ind_NextLast)?432:648); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
		if (_SN_In) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+5760*((D0Ind_Total+1)%2)),
					_SN_In, 80, _LN_In, 0, &DmaR_Evt1);
		}
		AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+14400+648*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt5);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+5760*((D0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (10-1*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (10-1*(1)-1*(1));
			KerArg0->InFeatures = (unsigned short int) ((D0Ind_Last)?48:72);
			KerArg0->OutFeatures = (unsigned short int) ((D0Ind_Last)?48:72);
			KerArg0->TotalInFeatures = (unsigned short int) ((D0Ind_Last)?48:72);
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+14400+648*((D0Ind_Total)%2));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+11520+((D0Ind)*288));
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+50256))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride1B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride1B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+15696+5760*((D0Ind_Total)%2));
			KerArg1->Feat = (unsigned short int) ((D0Ind_Last)?48:72);
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+13440+((D0Ind)*72));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+13920+((D0Ind)*72));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+15696+5760*((D0Ind_Total)%2)),
				_SC_Out, 80, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(D0Ind_Last)) {
			_C_Out = _C_Out + (5760); _LC_Out = (80); _SC_Out = (((D0Ind_NextLast)?48:72)*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S144_Conv2d_80x480x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 44652 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In1;
	unsigned int _SN_In1;
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 4][Tile0 Dim: 5]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 1 physical tiles
			Total Size: 1920 [Tile0, 5:[1920x1, 3:1920x1, 1920x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 5:[1920x1, 3:1920x1, 1920x1], 1]
		Tile0: [0, 1920, 1920], Tile1: [0, 1920, 1920], Tile2; [0, 1920, 1920]
	Ker Arg: In1, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 38400 [Tile1, 4:[480x24, 2:480x24, 480x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[480x24, 2:480x24, 480x8], 1]
		Tile0: [0, 11520, 11520], Tile1: [11520, 11520, 11520], Tile2; [23040, 11520, 11520]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 5 logical tiles, 5 physical tiles
			Total Size: 38400 [Tile0, 5:[480x16, 3:480x16, 480x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 5:[480x16, 3:480x16, 480x16], 1]
		Tile0: [0, 7680, 16], Tile1: [16, 7680, 16], Tile2; [32, 7680, 16]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 320 [Tile1, 4:[1x24, 2:1x24, 1x8], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x24, 2:1x24, 1x8], 4]
		Tile0: [0, 320, 320], Tile1: [0, 320, 320], Tile2; [0, 320, 320]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 6400 [Tile1, 4:[80x24, 2:80x24, 80x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[80x24, 2:80x24, 80x8], 1]
		Tile0: [0, 1920, 1920], Tile1: [1920, 1920, 1920], Tile2; [3840, 1920, 1920]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 80 [Tile1, 4:[1x24, 2:1x24, 1x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x24, 2:1x24, 1x8], 1]
		Tile0: [0, 80, 80], Tile1: [0, 80, 80], Tile2; [0, 80, 80]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 80 [Tile1, 4:[1x24, 2:1x24, 1x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x24, 2:1x24, 1x8], 1]
		Tile0: [0, 80, 80], Tile1: [0, 80, 80], Tile2; [0, 80, 80]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 5:[1x1, 3:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 5:[1x1, 3:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (480);
	KerArg0->W_In2 = (unsigned short int) (16);
	KerArg0->W_Out = (unsigned short int) (80);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+27360);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+44640);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 11520, 0, &DmaR_Evt1);
	_N_In1=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+29280+0), 7680, 80, 16, 0, &DmaR_Evt2);
	_N_In2=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+36192+0), 320, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+36192+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+23040), 320, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=1920;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+36512+0), 80, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+36512+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+27200), 80, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+36592+0), 80, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+36592+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+27280), 80, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+36672+0), 9, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+36672+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44640), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<4; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==3), T1Ind_NextLast = ((T1Ind+1)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_In1 = 0;
		if (!(T1Ind_Last)) {
			_N_In1 = _N_In1 + (11520); _SN_In1 = ((T1Ind_NextLast)?3840:11520); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+_N_In1), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+11520*((T1Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<5; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==4), T0Ind_NextLast = ((T0Ind+1)==4);
			/*================================= Prepare Tiles ===================================*/
			_SN_In2 = 0;
			if (!(T0Ind_Last)) {
				_N_In2 = _N_In2 + (16); _LN_In2 = (16); _SN_In2 = (480*_LN_In2); 
			} else if (!(T1Ind_Last)) {
				_N_In2 = _N_In2 + (-64); _LN_In2 = (16); _SN_In2 = (480*_LN_In2); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
			if (_SN_In2) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+29280+7680*((T0Ind_Total+1)%2)),
						_SN_In2, 80, _LN_In2, 0, &DmaR_Evt2);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+11520*((T1Ind_Total)%2));
			KerArg0->H_In1 = (unsigned short int) (T1Ind_Last?8:24);
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+29280+7680*((T0Ind_Total)%2));
			KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+23040+(96*(T1Ind)));
			KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+27200+(24*(T1Ind)));
			KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+27280+(24*(T1Ind)));
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+23360+1920*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((T0Ind)*16);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+44640))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
			/*================================= Update Arg Pipeline =============================*/
			/*============================= End Update Arg Pipeline =============================*/
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+23360+1920*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (1920); _SC_Out = ((T1Ind_NextLast)?640:1920); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S145_MatAdd_80x8x10(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 19212 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerMat3_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6400 [Tile0, 1:[1x6400], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x6400], 1]
		Tile0: [0, 6400, 6400], Tile1: [0, 6400, 6400], Tile2; [0, 6400, 6400]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6400 [Tile0, 1:[1x6400], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x6400], 1]
		Tile0: [0, 6400, 6400], Tile1: [0, 6400, 6400], Tile2; [0, 6400, 6400]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6400 [Tile0, 1:[1x6400], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x6400], 1]
		Tile0: [0, 6400, 6400], Tile1: [0, 6400, 6400], Tile2; [0, 6400, 6400]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->In2 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+6400);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+12800);
	KerArg0->Feat = (unsigned short int) (6400);
	KerArg0->DoScale = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+19200);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 6400, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6400), 6400, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+36192+0), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+36192+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+19200), 9, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		AT_FORK(gap_ncore(), (void *) KerMatAdd_SQ8, (void *) KerArg0);
		__CALL(KerMatAdd_SQ8, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+12800), 6400, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S148_Conv2d_480x80x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50572 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In1;
	unsigned int _SN_In1;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 4][Tile0 Dim: 1]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 320 [Tile0, 1:[320x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[320x1], 1]
		Tile0: [0, 320, 320], Tile1: [0, 320, 320], Tile2; [0, 320, 320]
	Ker Arg: In1, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 38400 [Tile1, 4:[80x128, 2:80x128, 80x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[80x128, 2:80x128, 80x96], 1]
		Tile0: [0, 10240, 10240], Tile1: [10240, 10240, 10240], Tile2; [20480, 10240, 10240]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6400 [Tile0, 1:[80x80], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[80x80], 1]
		Tile0: [0, 6400, 6400], Tile1: [0, 6400, 6400], Tile2; [0, 6400, 6400]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 1920 [Tile1, 4:[1x128, 2:1x128, 1x96], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x128, 2:1x128, 1x96], 4]
		Tile0: [0, 1920, 1920], Tile1: [0, 1920, 1920], Tile2; [0, 1920, 1920]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 38400 [Tile1, 4:[80x128, 2:80x128, 80x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[80x128, 2:80x128, 80x96], 1]
		Tile0: [0, 10240, 10240], Tile1: [10240, 10240, 10240], Tile2; [20480, 10240, 10240]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 480 [Tile1, 4:[1x128, 2:1x128, 1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x128, 2:1x128, 1x96], 1]
		Tile0: [0, 480, 480], Tile1: [0, 480, 480], Tile2; [0, 480, 480]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 480 [Tile1, 4:[1x128, 2:1x128, 1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x128, 2:1x128, 1x96], 1]
		Tile0: [0, 480, 480], Tile1: [0, 480, 480], Tile2; [0, 480, 480]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (80);
	KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44160);
	KerArg0->W_In2 = (unsigned short int) (80);
	KerArg0->W_Out = (unsigned short int) (80);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+43840);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+50560);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 10240, 0, &DmaR_Evt1);
	_N_In1=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44160), 6400, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+20480), 1920, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=10240;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+42880), 480, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+43360), 480, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50560), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<4; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==3), T1Ind_NextLast = ((T1Ind+1)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_In1 = 0;
		if (!(T1Ind_Last)) {
			_N_In1 = _N_In1 + (10240); _SN_In1 = ((T1Ind_NextLast)?7680:10240); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+_N_In1), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+10240*((T1Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+10240*((T1Ind_Total)%2));
			KerArg0->H_In1 = (unsigned short int) (T1Ind_Last?96:128);
			KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+20480+(512*(T1Ind)));
			KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+42880+(128*(T1Ind)));
			KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+43360+(128*(T1Ind)));
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+22400+10240*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((0)*80);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+50560))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+22400+10240*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (10240); _SC_Out = ((T1Ind_NextLast)?7680:10240); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S151_Conv2d_480x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50268 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 480, Tiled: 7][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 38400 [D0, [6 x 5760, 3840]][Tile0, 1:[10x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 5760, 3840]][Tile0, 1:[10x8], 1]
		Tile0: [0, 5760, 80], Tile1: [5760, 5760, 80], Tile2; [11520, 5760, 80]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 1920 [D0, [6 x 288, 192]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 288, 192]]
		Tile0: [0, 1920, 1920], Tile1: [0, 1920, 1920], Tile2; [0, 1920, 1920]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 480 [D0, [6 x 72, 48]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 72, 48]]
		Tile0: [0, 480, 480], Tile1: [0, 480, 480], Tile2; [0, 480, 480]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 480 [D0, [6 x 72, 48]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 72, 48]]
		Tile0: [0, 480, 480], Tile1: [0, 480, 480], Tile2; [0, 480, 480]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 4320 [D0, [6 x 648, 432]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 648, 432]]
		Tile0: [0, 648, 648], Tile1: [648, 648, 648], Tile2; [1296, 648, 648]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 38400 [D0, [6 x 5760, 3840]][Tile0, 1:[10x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 5760, 3840]][Tile0, 1:[10x8], 1]
		Tile0: [0, 5760, 80], Tile1: [5760, 5760, 80], Tile2; [11520, 5760, 80]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 153600 [D0, [6 x 23040, 15360]][Tile0, 1:[10x8], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 23040, 15360]][Tile0, 1:[10x8], 4]
		Tile0: [0, 23040, 320], Tile1: [0, 23040, 320], Tile2; [0, 23040, 320]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (10);
	KerArg0->UsedW = (unsigned short int) (10);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+27216);
	KerArg0->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+27216);
	KerArg1->W = (unsigned short int) (10);
	KerArg1->H = (unsigned short int) (8);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+50256);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 5760, 80, 80, 0, &DmaR_Evt1);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+11520), 1920, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+13440), 480, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+13920), 480, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+14400+0), 648, 0, &DmaR_Evt5);
	_N_Filter=0;
	_C_Out=0; _SC_Out=5760; _LC_Out=80;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+110912+0), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+110912+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50256), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<7; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==6), D0Ind_NextLast = ((D0Ind+1)==6);
		/*================================= Prepare Tiles ===================================*/
		_SN_In = 0;
		if (!(D0Ind_Last)) {
			_N_In = _N_In + (5760); _LN_In = (80); _SN_In = (((D0Ind_NextLast)?48:72)*_LN_In); 
		}
		_SN_Filter = 0;
		if (!(D0Ind_Last)) {
			_N_Filter = _N_Filter + (648); _SN_Filter = ((D0Ind_NextLast)?432:648); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
		if (_SN_In) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+5760*((D0Ind_Total+1)%2)),
					_SN_In, 80, _LN_In, 0, &DmaR_Evt1);
		}
		AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+14400+648*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt5);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+5760*((D0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (10-1*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (10-1*(1)-1*(1));
			KerArg0->InFeatures = (unsigned short int) ((D0Ind_Last)?48:72);
			KerArg0->OutFeatures = (unsigned short int) ((D0Ind_Last)?48:72);
			KerArg0->TotalInFeatures = (unsigned short int) ((D0Ind_Last)?48:72);
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+14400+648*((D0Ind_Total)%2));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+11520+((D0Ind)*288));
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+50256))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride1B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride1B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+15696+5760*((D0Ind_Total)%2));
			KerArg1->Feat = (unsigned short int) ((D0Ind_Last)?48:72);
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+13440+((D0Ind)*72));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+13920+((D0Ind)*72));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+15696+5760*((D0Ind_Total)%2)),
				_SC_Out, 80, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(D0Ind_Last)) {
			_C_Out = _C_Out + (5760); _LC_Out = (80); _SC_Out = (((D0Ind_NextLast)?48:72)*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S154_Conv2d_80x480x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 44652 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In1;
	unsigned int _SN_In1;
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 4][Tile0 Dim: 5]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 1 physical tiles
			Total Size: 1920 [Tile0, 5:[1920x1, 3:1920x1, 1920x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 5:[1920x1, 3:1920x1, 1920x1], 1]
		Tile0: [0, 1920, 1920], Tile1: [0, 1920, 1920], Tile2; [0, 1920, 1920]
	Ker Arg: In1, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 38400 [Tile1, 4:[480x24, 2:480x24, 480x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[480x24, 2:480x24, 480x8], 1]
		Tile0: [0, 11520, 11520], Tile1: [11520, 11520, 11520], Tile2; [23040, 11520, 11520]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 5 logical tiles, 5 physical tiles
			Total Size: 38400 [Tile0, 5:[480x16, 3:480x16, 480x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 5:[480x16, 3:480x16, 480x16], 1]
		Tile0: [0, 7680, 16], Tile1: [16, 7680, 16], Tile2; [32, 7680, 16]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 320 [Tile1, 4:[1x24, 2:1x24, 1x8], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x24, 2:1x24, 1x8], 4]
		Tile0: [0, 320, 320], Tile1: [0, 320, 320], Tile2; [0, 320, 320]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 6400 [Tile1, 4:[80x24, 2:80x24, 80x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[80x24, 2:80x24, 80x8], 1]
		Tile0: [0, 1920, 1920], Tile1: [1920, 1920, 1920], Tile2; [3840, 1920, 1920]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 80 [Tile1, 4:[1x24, 2:1x24, 1x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x24, 2:1x24, 1x8], 1]
		Tile0: [0, 80, 80], Tile1: [0, 80, 80], Tile2; [0, 80, 80]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 80 [Tile1, 4:[1x24, 2:1x24, 1x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x24, 2:1x24, 1x8], 1]
		Tile0: [0, 80, 80], Tile1: [0, 80, 80], Tile2; [0, 80, 80]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 5:[1x1, 3:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 5:[1x1, 3:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (480);
	KerArg0->W_In2 = (unsigned short int) (16);
	KerArg0->W_Out = (unsigned short int) (80);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+27360);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+44640);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 11520, 0, &DmaR_Evt1);
	_N_In1=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+29280+0), 7680, 80, 16, 0, &DmaR_Evt2);
	_N_In2=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Bias+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+29792+0), 320, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Bias */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+29792+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+23040), 320, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=1920;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Scale+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+30112+0), 80, 0, &UchanHF2);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2); /* Wait previous uDMA read Scale */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+30112+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+27200), 80, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) ScaleN+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+30192+0), 80, 0, &UchanHF3);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3); /* Wait previous uDMA read ScaleN */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+30192+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+27280), 80, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+30272+0), 9, 0, &UchanHF4);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+30272+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44640), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<4; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==3), T1Ind_NextLast = ((T1Ind+1)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_In1 = 0;
		if (!(T1Ind_Last)) {
			_N_In1 = _N_In1 + (11520); _SN_In1 = ((T1Ind_NextLast)?3840:11520); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+_N_In1), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+11520*((T1Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<5; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==4), T0Ind_NextLast = ((T0Ind+1)==4);
			/*================================= Prepare Tiles ===================================*/
			_SN_In2 = 0;
			if (!(T0Ind_Last)) {
				_N_In2 = _N_In2 + (16); _LN_In2 = (16); _SN_In2 = (480*_LN_In2); 
			} else if (!(T1Ind_Last)) {
				_N_In2 = _N_In2 + (-64); _LN_In2 = (16); _SN_In2 = (480*_LN_In2); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
			if (_SN_In2) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+29280+7680*((T0Ind_Total+1)%2)),
						_SN_In2, 80, _LN_In2, 0, &DmaR_Evt2);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+11520*((T1Ind_Total)%2));
			KerArg0->H_In1 = (unsigned short int) (T1Ind_Last?8:24);
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+29280+7680*((T0Ind_Total)%2));
			KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+23040+(96*(T1Ind)));
			KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+27200+(24*(T1Ind)));
			KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+27280+(24*(T1Ind)));
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+23360+1920*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((T0Ind)*16);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+44640))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
			/*================================= Update Arg Pipeline =============================*/
			/*============================= End Update Arg Pipeline =============================*/
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+23360+1920*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (1920); _SC_Out = ((T1Ind_NextLast)?640:1920); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S155_MatAdd_80x8x10(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 19212 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerMat3_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6400 [Tile0, 1:[1x6400], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x6400], 1]
		Tile0: [0, 6400, 6400], Tile1: [0, 6400, 6400], Tile2; [0, 6400, 6400]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6400 [Tile0, 1:[1x6400], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x6400], 1]
		Tile0: [0, 6400, 6400], Tile1: [0, 6400, 6400], Tile2; [0, 6400, 6400]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6400 [Tile0, 1:[1x6400], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x6400], 1]
		Tile0: [0, 6400, 6400], Tile1: [0, 6400, 6400], Tile2; [0, 6400, 6400]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->In2 = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+6400);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+12800);
	KerArg0->Feat = (unsigned short int) (6400);
	KerArg0->DoScale = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+19200);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 6400, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6400), 6400, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+29792+0), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+29792+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+19200), 9, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		AT_FORK(gap_ncore(), (void *) KerMatAdd_SQ8, (void *) KerArg0);
		__CALL(KerMatAdd_SQ8, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+12800), 6400, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S158_Conv2d_480x80x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50572 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In1;
	unsigned int _SN_In1;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 4][Tile0 Dim: 1]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 320 [Tile0, 1:[320x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[320x1], 1]
		Tile0: [0, 320, 320], Tile1: [0, 320, 320], Tile2; [0, 320, 320]
	Ker Arg: In1, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 38400 [Tile1, 4:[80x128, 2:80x128, 80x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[80x128, 2:80x128, 80x96], 1]
		Tile0: [0, 10240, 10240], Tile1: [10240, 10240, 10240], Tile2; [20480, 10240, 10240]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6400 [Tile0, 1:[80x80], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[80x80], 1]
		Tile0: [0, 6400, 6400], Tile1: [0, 6400, 6400], Tile2; [0, 6400, 6400]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 1920 [Tile1, 4:[1x128, 2:1x128, 1x96], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x128, 2:1x128, 1x96], 4]
		Tile0: [0, 1920, 1920], Tile1: [0, 1920, 1920], Tile2; [0, 1920, 1920]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 38400 [Tile1, 4:[80x128, 2:80x128, 80x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[80x128, 2:80x128, 80x96], 1]
		Tile0: [0, 10240, 10240], Tile1: [10240, 10240, 10240], Tile2; [20480, 10240, 10240]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 480 [Tile1, 4:[1x128, 2:1x128, 1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x128, 2:1x128, 1x96], 1]
		Tile0: [0, 480, 480], Tile1: [0, 480, 480], Tile2; [0, 480, 480]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 480 [Tile1, 4:[1x128, 2:1x128, 1x96], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x128, 2:1x128, 1x96], 1]
		Tile0: [0, 480, 480], Tile1: [0, 480, 480], Tile2; [0, 480, 480]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (80);
	KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44160);
	KerArg0->W_In2 = (unsigned short int) (80);
	KerArg0->W_Out = (unsigned short int) (80);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+43840);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+50560);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 10240, 0, &DmaR_Evt1);
	_N_In1=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44160), 6400, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+20480), 1920, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=10240;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+42880), 480, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+43360), 480, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+71072+0), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+71072+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50560), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<4; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==3), T1Ind_NextLast = ((T1Ind+1)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_In1 = 0;
		if (!(T1Ind_Last)) {
			_N_In1 = _N_In1 + (10240); _SN_In1 = ((T1Ind_NextLast)?7680:10240); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+_N_In1), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+10240*((T1Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+10240*((T1Ind_Total)%2));
			KerArg0->H_In1 = (unsigned short int) (T1Ind_Last?96:128);
			KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+20480+(512*(T1Ind)));
			KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+42880+(128*(T1Ind)));
			KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+43360+(128*(T1Ind)));
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+22400+10240*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((0)*80);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+50560))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+22400+10240*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (10240); _SC_Out = ((T1Ind_NextLast)?7680:10240); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S161_Conv2d_480x1x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 50268 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerConv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConvLinReduct_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 480, Tiled: 7][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 38400 [D0, [6 x 5760, 3840]][Tile0, 1:[10x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 5760, 3840]][Tile0, 1:[10x8], 1]
		Tile0: [0, 5760, 80], Tile1: [5760, 5760, 80], Tile2; [11520, 5760, 80]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 1920 [D0, [6 x 288, 192]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 288, 192]]
		Tile0: [0, 1920, 1920], Tile1: [0, 1920, 1920], Tile2; [0, 1920, 1920]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 480 [D0, [6 x 72, 48]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 72, 48]]
		Tile0: [0, 480, 480], Tile1: [0, 480, 480], Tile2; [0, 480, 480]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 480 [D0, [6 x 72, 48]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 72, 48]]
		Tile0: [0, 480, 480], Tile1: [0, 480, 480], Tile2; [0, 480, 480]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 4320 [D0, [6 x 648, 432]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 648, 432]]
		Tile0: [0, 648, 648], Tile1: [648, 648, 648], Tile2; [1296, 648, 648]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 38400 [D0, [6 x 5760, 3840]][Tile0, 1:[10x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 5760, 3840]][Tile0, 1:[10x8], 1]
		Tile0: [0, 5760, 80], Tile1: [5760, 5760, 80], Tile2; [11520, 5760, 80]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 153600 [D0, [6 x 23040, 15360]][Tile0, 1:[10x8], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [6 x 23040, 15360]][Tile0, 1:[10x8], 4]
		Tile0: [0, 23040, 320], Tile1: [0, 23040, 320], Tile2; [0, 23040, 320]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (10);
	KerArg0->UsedW = (unsigned short int) (10);
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+27216);
	KerArg0->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg1->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+27216);
	KerArg1->W = (unsigned short int) (10);
	KerArg1->H = (unsigned short int) (8);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+50256);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 5760, 80, 80, 0, &DmaR_Evt1);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+11520), 1920, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+13440), 480, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+13920), 480, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+14400+0), 648, 0, &DmaR_Evt5);
	_N_Filter=0;
	_C_Out=0; _SC_Out=5760; _LC_Out=80;
	_SP_Out=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+108352+0), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+108352+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+50256), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<7; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==6), D0Ind_NextLast = ((D0Ind+1)==6);
		/*================================= Prepare Tiles ===================================*/
		_SN_In = 0;
		if (!(D0Ind_Last)) {
			_N_In = _N_In + (5760); _LN_In = (80); _SN_In = (((D0Ind_NextLast)?48:72)*_LN_In); 
		}
		_SN_Filter = 0;
		if (!(D0Ind_Last)) {
			_N_Filter = _N_Filter + (648); _SN_Filter = ((D0Ind_NextLast)?432:648); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
		if (_SN_In) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+5760*((D0Ind_Total+1)%2)),
					_SN_In, 80, _LN_In, 0, &DmaR_Evt1);
		}
		AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+14400+648*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt5);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+5760*((D0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (10-1*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (10-1*(1)-1*(1));
			KerArg0->InFeatures = (unsigned short int) ((D0Ind_Last)?48:72);
			KerArg0->OutFeatures = (unsigned short int) ((D0Ind_Last)?48:72);
			KerArg0->TotalInFeatures = (unsigned short int) ((D0Ind_Last)?48:72);
			KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+14400+648*((D0Ind_Total)%2));
			KerArg0->Bias = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+11520+((D0Ind)*288));
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+50256))[5]);
			AT_FORK(gap_ncore(), (void *) KerParConvDW3x3Stride1B32_SQ8, (void *) KerArg0);
			__CALL(KerParConvDW3x3Stride1B32_SQ8, KerArg0);
			KerArg1->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+15696+5760*((D0Ind_Total)%2));
			KerArg1->Feat = (unsigned short int) ((D0Ind_Last)?48:72);
			KerArg1->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+13440+((D0Ind)*72));
			KerArg1->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+13920+((D0Ind)*72));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg1);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg1);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+15696+5760*((D0Ind_Total)%2)),
				_SC_Out, 80, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(D0Ind_Last)) {
			_C_Out = _C_Out + (5760); _LC_Out = (80); _SC_Out = (((D0Ind_NextLast)?48:72)*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S164_Conv2d_160x480x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 45132 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _NN_In1;
	unsigned int _SN_In1, _SNN_In1;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 7][Tile0 Dim: 5]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 5 logical tiles, 5 physical tiles
			Total Size: 38400 [Tile0, 5:[480x16, 3:480x16, 480x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 5:[480x16, 3:480x16, 480x16], 1]
		Tile0: [0, 7680, 16], Tile1: [16, 7680, 16], Tile2; [32, 7680, 16]
	Ker Arg: In1, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 76800 [Tile1, 7:[480x24, 5:480x24, 480x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 7:[480x24, 5:480x24, 480x16], 1]
		Tile0: [0, 11520, 11520], Tile1: [11520, 11520, 11520], Tile2; [23040, 11520, 11520]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 1 physical tiles
			Total Size: 1920 [Tile0, 5:[1920x1, 3:1920x1, 1920x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 5:[1920x1, 3:1920x1, 1920x1], 1]
		Tile0: [0, 1920, 1920], Tile1: [0, 1920, 1920], Tile2; [0, 1920, 1920]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 640 [Tile1, 7:[1x24, 5:1x24, 1x16], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 7:[1x24, 5:1x24, 1x16], 4]
		Tile0: [0, 640, 640], Tile1: [0, 640, 640], Tile2; [0, 640, 640]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 12800 [Tile1, 7:[80x24, 5:80x24, 80x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 7:[80x24, 5:80x24, 80x16], 1]
		Tile0: [0, 1920, 1920], Tile1: [1920, 1920, 1920], Tile2; [3840, 1920, 1920]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 160 [Tile1, 7:[1x24, 5:1x24, 1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 7:[1x24, 5:1x24, 1x16], 1]
		Tile0: [0, 160, 160], Tile1: [0, 160, 160], Tile2; [0, 160, 160]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 160 [Tile1, 7:[1x24, 5:1x24, 1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 7:[1x24, 5:1x24, 1x16], 1]
		Tile0: [0, 160, 160], Tile1: [0, 160, 160], Tile2; [0, 160, 160]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 5:[1x1, 3:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 5:[1x1, 3:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (480);
	KerArg0->W_In2 = (unsigned short int) (16);
	KerArg0->W_Out = (unsigned short int) (80);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+27840);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+45120);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+29760+0), 7680, 80, 16, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+36192+0), 11520, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read In1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+11520), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+36192+11520), 11520, 0, &UchanHF1);
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+36192+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 11520, 0, &DmaR_Evt2);
	_NN_In1=11520; _SN_In1=11520;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+23040), 640, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=1920;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+27520), 160, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+27680), 160, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+45120), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<7; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==6), T1Ind_NextLast = ((T1Ind+1)==6), T1Ind_NextNextLast = ((T1Ind+2)==6);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In1 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In1 = _NN_In1 + (11520); _SNN_In1 = ((T1Ind_NextNextLast)?7680:11520); 
			}
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read In1 */
		if (_SNN_In1) {
			AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+_NN_In1), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+36192+11520*((T1Ind_Total)%2)),
					_SNN_In1, 0, &UchanHF1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+36192+11520*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+11520*((T1Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt2);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<5; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==4), T0Ind_NextLast = ((T0Ind+1)==4);
			/*================================= Prepare Tiles ===================================*/
			_SN_In2 = 0;
			if (!(T0Ind_Last)) {
				_N_In2 = _N_In2 + (16); _LN_In2 = (16); _SN_In2 = (480*_LN_In2); 
			} else if (!(T1Ind_Last)) {
				_N_In2 = _N_In2 + (-64); _LN_In2 = (16); _SN_In2 = (480*_LN_In2); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
			if (_SN_In2) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+29760+7680*((T0Ind_Total+1)%2)),
						_SN_In2, 80, _LN_In2, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+11520*((T1Ind_Total)%2));
			KerArg0->H_In1 = (unsigned short int) (T1Ind_Last?16:24);
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+29760+7680*((T0Ind_Total)%2));
			KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+23040+(96*(T1Ind)));
			KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+27520+(24*(T1Ind)));
			KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+27680+(24*(T1Ind)));
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+23680+1920*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((T0Ind)*16);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+45120))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
			/*================================= Update Arg Pipeline =============================*/
			/*============================= End Update Arg Pipeline =============================*/
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+23680+1920*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SN_In1 = _SNN_In1;
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (1920); _SC_Out = ((T1Ind_NextLast)?1280:1920); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S167_Conv2d_640x160x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 48012 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _NN_In1;
	unsigned int _SN_In1, _SNN_In1;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 9][Tile0 Dim: 3]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 3 logical tiles, 3 physical tiles
			Total Size: 12800 [Tile0, 3:[160x28, 1:160x28, 160x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[160x28, 1:160x28, 160x24], 1]
		Tile0: [0, 4480, 28], Tile1: [28, 4480, 28], Tile2; [56, 3840, 24]
	Ker Arg: In1, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 9 logical tiles, 9 physical tiles
			Total Size: 102400 [Tile1, 9:[160x72, 7:160x72, 160x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 9:[160x72, 7:160x72, 160x64], 1]
		Tile0: [0, 11520, 11520], Tile1: [11520, 11520, 11520], Tile2; [23040, 11520, 11520]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 3 logical tiles, 1 physical tiles
			Total Size: 640 [Tile0, 3:[640x1, 1:640x1, 640x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[640x1, 1:640x1, 640x1], 1]
		Tile0: [0, 640, 640], Tile1: [0, 640, 640], Tile2; [0, 640, 640]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 9 logical tiles, 1 physical tiles
			Total Size: 2560 [Tile1, 9:[1x72, 7:1x72, 1x64], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 9:[1x72, 7:1x72, 1x64], 4]
		Tile0: [0, 2560, 2560], Tile1: [0, 2560, 2560], Tile2; [0, 2560, 2560]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 9 logical tiles, 9 physical tiles
			Total Size: 51200 [Tile1, 9:[80x72, 7:80x72, 80x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 9:[80x72, 7:80x72, 80x64], 1]
		Tile0: [0, 5760, 5760], Tile1: [5760, 5760, 5760], Tile2; [11520, 5760, 5760]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 9 logical tiles, 1 physical tiles
			Total Size: 640 [Tile1, 9:[1x72, 7:1x72, 1x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 9:[1x72, 7:1x72, 1x64], 1]
		Tile0: [0, 640, 640], Tile1: [0, 640, 640], Tile2; [0, 640, 640]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 9 logical tiles, 1 physical tiles
			Total Size: 640 [Tile1, 9:[1x72, 7:1x72, 1x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 9:[1x72, 7:1x72, 1x64], 1]
		Tile0: [0, 640, 640], Tile1: [0, 640, 640], Tile2; [0, 640, 640]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 3 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 3:[1x1, 1:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[1x1, 1:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (160);
	KerArg0->W_Out = (unsigned short int) (80);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+38400);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+48000);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+39040+0), 4480, 80, 28, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+87392+0), 11520, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read In1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+11520), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+87392+11520), 11520, 0, &UchanHF1);
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+87392+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 11520, 0, &DmaR_Evt2);
	_NN_In1=11520; _SN_In1=11520;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+23040), 2560, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=5760;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+37120), 640, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+37760), 640, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48000), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<9; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==8), T1Ind_NextLast = ((T1Ind+1)==8), T1Ind_NextNextLast = ((T1Ind+2)==8);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In1 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In1 = _NN_In1 + (11520); _SNN_In1 = ((T1Ind_NextNextLast)?10240:11520); 
			}
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read In1 */
		if (_SNN_In1) {
			AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+_NN_In1), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+87392+11520*((T1Ind_Total)%2)),
					_SNN_In1, 0, &UchanHF1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+87392+11520*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+11520*((T1Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt2);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<3; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==2), T0Ind_NextLast = ((T0Ind+1)==2);
			/*================================= Prepare Tiles ===================================*/
			_SN_In2 = 0;
			if (!(T0Ind_Last)) {
				_N_In2 = _N_In2 + (28); _LN_In2 = ((T0Ind_NextLast)?24:28); _SN_In2 = (160*_LN_In2); 
			} else if (!(T1Ind_Last)) {
				_N_In2 = _N_In2 + (-56); _LN_In2 = (28); _SN_In2 = (160*_LN_In2); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
			if (_SN_In2) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+39040+4480*((T0Ind_Total+1)%2)),
						_SN_In2, 80, _LN_In2, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+11520*((T1Ind_Total)%2));
			KerArg0->H_In1 = (unsigned short int) (T1Ind_Last?64:72);
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+39040+4480*((T0Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T0Ind_Last)?24:28);
			KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+23040+(288*(T1Ind)));
			KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+37120+(72*(T1Ind)));
			KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+37760+(72*(T1Ind)));
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+25600+5760*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((T0Ind)*28);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+48000))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
			/*================================= Update Arg Pipeline =============================*/
			/*============================= End Update Arg Pipeline =============================*/
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+25600+5760*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SN_In1 = _SNN_In1;
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (5760); _SC_Out = ((T1Ind_NextLast)?5120:5760); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S170_Conv2d_128x640x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 41740 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _NN_In1;
	unsigned int _SN_In1, _SNN_In1;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 8][Tile0 Dim: 7]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 51200 [Tile0, 7:[640x12, 5:640x12, 640x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 7:[640x12, 5:640x12, 640x8], 1]
		Tile0: [0, 7680, 12], Tile1: [12, 7680, 12], Tile2; [24, 7680, 12]
	Ker Arg: In1, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 81920 [Tile1, 8:[640x16, 6:640x16, 640x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[640x16, 6:640x16, 640x16], 1]
		Tile0: [0, 10240, 10240], Tile1: [10240, 10240, 10240], Tile2; [20480, 10240, 10240]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 2560 [Tile0, 7:[2560x1, 5:2560x1, 2560x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 7:[2560x1, 5:2560x1, 2560x1], 1]
		Tile0: [0, 2560, 2560], Tile1: [0, 2560, 2560], Tile2; [0, 2560, 2560]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 1 physical tiles
			Total Size: 512 [Tile1, 8:[1x16, 6:1x16, 1x16], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[1x16, 6:1x16, 1x16], 4]
		Tile0: [0, 512, 512], Tile1: [0, 512, 512], Tile2; [0, 512, 512]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 10240 [Tile1, 8:[80x16, 6:80x16, 80x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[80x16, 6:80x16, 80x16], 1]
		Tile0: [0, 1280, 1280], Tile1: [1280, 1280, 1280], Tile2; [2560, 1280, 1280]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 1 physical tiles
			Total Size: 128 [Tile1, 8:[1x16, 6:1x16, 1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[1x16, 6:1x16, 1x16], 1]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 1 physical tiles
			Total Size: 128 [Tile1, 8:[1x16, 6:1x16, 1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 8:[1x16, 6:1x16, 1x16], 1]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 7:[1x1, 5:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 7:[1x1, 5:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (640);
	KerArg0->H_In1 = (unsigned short int) (16);
	KerArg0->W_Out = (unsigned short int) (80);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+23808);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+41728);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+26368+0), 7680, 80, 12, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+87392+0), 10240, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read In1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+10240), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+87392+10240), 10240, 0, &UchanHF1);
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+87392+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 10240, 0, &DmaR_Evt2);
	_NN_In1=10240; _SN_In1=10240;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+20480), 512, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=1280;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+23552), 128, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+23680), 128, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+41728), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<8; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==7), T1Ind_NextLast = ((T1Ind+1)==7), T1Ind_NextNextLast = ((T1Ind+2)==7);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In1 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In1 = _NN_In1 + (10240); _SNN_In1 = (10240); 
			}
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read In1 */
		if (_SNN_In1) {
			AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+_NN_In1), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+87392+10240*((T1Ind_Total)%2)),
					_SNN_In1, 0, &UchanHF1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+87392+10240*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+10240*((T1Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt2);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<7; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==6), T0Ind_NextLast = ((T0Ind+1)==6);
			/*================================= Prepare Tiles ===================================*/
			_SN_In2 = 0;
			if (!(T0Ind_Last)) {
				_N_In2 = _N_In2 + (12); _LN_In2 = ((T0Ind_NextLast)?8:12); _SN_In2 = (640*_LN_In2); 
			} else if (!(T1Ind_Last)) {
				_N_In2 = _N_In2 + (-72); _LN_In2 = (12); _SN_In2 = (640*_LN_In2); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
			if (_SN_In2) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+26368+7680*((T0Ind_Total+1)%2)),
						_SN_In2, 80, _LN_In2, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+10240*((T1Ind_Total)%2));
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+26368+7680*((T0Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T0Ind_Last)?8:12);
			KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+20480+(64*(T1Ind)));
			KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+23552+(16*(T1Ind)));
			KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+23680+(16*(T1Ind)));
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+20992+1280*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((T0Ind)*12);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+41728))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
			/*================================= Update Arg Pipeline =============================*/
			/*============================= End Update Arg Pipeline =============================*/
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+20992+1280*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SN_In1 = _SNN_In1;
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (1280); _SC_Out = (1280); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S173_Conv2d_256x128x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 46220 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerSetBias_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerConvLinReduct_SQ8_T S_KerArg2, *KerArg2 = &S_KerArg2;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Last, D1Ind_NextLast, D1Ind_NextNextLast;
	int T0Ind, T0Ind_Last, T0Ind_NextLast;
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast, D0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _NN_Filter;
	unsigned int _SN_Filter, _SNN_Filter;
	unsigned int _LN_Filter, _LNN_Filter;
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: Init: 256, Tiled: 1][Tile0 Dim: 1][D0 Dim: Init: 128, Tiled: 32]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 20480 [D1, [0 x 20480, 20480]][Tile0, 1:[5x4], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 20480, 20480]][Tile0, 1:[5x4], 4]
		Tile0: [0, 20480, 80], Tile1: [0, 20480, 80], Tile2; [0, 20480, 80]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1024 [D1, [0 x 1024, 1024]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 1024, 1024]]
		Tile0: [0, 1024, 1024], Tile1: [0, 1024, 1024], Tile2; [0, 1024, 1024]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 256 [D1, [0 x 256, 256]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 256, 256]]
		Tile0: [0, 256, 256], Tile1: [0, 256, 256], Tile2; [0, 256, 256]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 256 [D1, [0 x 256, 256]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 256, 256]]
		Tile0: [0, 256, 256], Tile1: [0, 256, 256], Tile2; [0, 256, 256]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 32 logical tiles, 32 physical tiles
			Total Size: 294912 [D1, [0 x 294912, 294912]][D0, [31 x 9216, 9216]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 294912, 294912]][D0, [31 x 9216, 9216]]
		Tile0: [0, 9216, 36], Tile1: [36, 9216, 36], Tile2; [72, 9216, 36]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 5120 [D1, [0 x 5120, 5120]][Tile0, 1:[5x4], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 5120, 5120]][Tile0, 1:[5x4], 1]
		Tile0: [0, 5120, 5120], Tile1: [0, 5120, 5120], Tile2; [0, 5120, 5120]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 32 logical tiles, 32 physical tiles
			Total Size: 10240 [D0, [31 x 320, 320]][Tile0, 1:[10x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[10x9, 30:10x9, 10x8], 1][D0, [31 x 320, 320]]
		Tile0: [0, 320, 80], Tile1: [320, 320, 80], Tile2; [640, 320, 80]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+25728);
	KerArg0->W = (unsigned short int) (5);
	KerArg0->H = (unsigned short int) (4);
	KerArg0->Feat = (unsigned short int) (256);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+640);
	KerArg1->W = (unsigned short int) (10);
	KerArg1->UsedW = (unsigned short int) (10);
	KerArg1->InFeatures = (unsigned short int) (4);
	KerArg1->OutFeatures = (unsigned short int) (256);
	KerArg1->TotalInFeatures = (unsigned short int) (4);
	KerArg1->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+25728);
	KerArg1->Pad = (v4s) ((v4s){0,1,0,1});
	KerArg2->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+25728);
	KerArg2->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+20608);
	KerArg2->Feat = (unsigned short int) (256);
	KerArg2->W = (unsigned short int) (5);
	KerArg2->H = (unsigned short int) (4);
	KerArg2->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+1664);
	KerArg2->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+1920);
	KerArg2->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+46208);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+640), 1024, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1664), 256, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1920), 256, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY2D(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Filter+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+38752+0), 9216, 1152, 36, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Filter */
	AT_HYPERFLASH_FS_CL_COPY2D(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Filter+36), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+38752+9216), 9216, 1152, 36, 0, &UchanHF1);
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+38752+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+2176+0), 9216, 0, &DmaR_Evt4);
	_NN_Filter=36; _SN_Filter=9216;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 320, 80, 80, 0, &DmaR_Evt5);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+46208), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D1 */
		int D1Ind_Last = 1, D1Ind_NextLast = 1, D1Ind_NextNextLast = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+46208))[5]);
			AT_FORK(gap_ncore(), (void *) KerParSetBiasB32_SQ8, (void *) KerArg0);
			__CALL(KerParSetBiasB32_SQ8, KerArg0);
			for (D0Ind=0; D0Ind<32; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
				int D0Ind_Last = (D0Ind==31), D0Ind_NextLast = ((D0Ind+1)==31), D0Ind_NextNextLast = ((D0Ind+2)==31);
				/*================================= Prepare Tiles ===================================*/
				_SNN_Filter = 0;
				if (!(D0Ind_Last)) {
					if (!(D0Ind_NextLast)) {
						_NN_Filter = _NN_Filter + (36); _LNN_Filter = (36); _SNN_Filter = (256*_LNN_Filter); 
					} else if (!((1))) {
						_NN_Filter = _NN_Filter + (-1116); _LNN_Filter = (36); _SNN_Filter = (256*_LNN_Filter); 
					}
				} else if (!((1))) {
					_NN_Filter = _NN_Filter + (36); _LNN_Filter = (36); _SNN_Filter = (256*_LNN_Filter); 
				}
				_SN_In = 0;
				if (!(D0Ind_Last)) {
					_N_In = _N_In + (320); _LN_In = (80); _SN_In = (4*_LN_In); 
				} else if (!(1)) {
					_N_In = _N_In + (-9920); _LN_In = (80); _SN_In = (4*_LN_In); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Filter */
				if (_SNN_Filter) {
					AT_HYPERFLASH_FS_CL_COPY2D(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Filter+_NN_Filter), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+38752+9216*((D0Ind_Total)%2)),
							_SNN_Filter, 1152, _LNN_Filter, 0, &UchanHF1);
				}
				AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
				if (_SN_Filter) {
					AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+38752+9216*((D0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+2176+9216*((D0Ind_Total+1)%2)),
							_SN_Filter, 0, &DmaR_Evt4);
				}
				AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+320*((D0Ind_Total+1)%2)),
							_SN_In, 80, _LN_In, 0, &DmaR_Evt5);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+320*((D0Ind_Total)%2));
				KerArg1->H = (unsigned short int) (9-0*(1)-1*(1));
				KerArg1->UsedH = (unsigned short int) (9-0*(1)-1*(1));
				KerArg1->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+2176+9216*((D0Ind_Total)%2));
				AT_FORK(gap_ncore(), (void *) KerParConv3x3Stride2_SQ8, (void *) KerArg1);
				__CALL(KerParConv3x3Stride2_SQ8, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				_SN_Filter = _SNN_Filter;_LN_Filter = _LNN_Filter;
				/*============================= End Update Arg Pipeline =============================*/
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg2);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg2);
		} /* End iteration on Tile0 */
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+20608), 5120, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S176_Conv2d_64x256x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 24204 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 1][Tile0 Dim: 1]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1024 [Tile0, 1:[1024x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1024x1], 1]
		Tile0: [0, 1024, 1024], Tile1: [0, 1024, 1024], Tile2; [0, 1024, 1024]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 16384 [Tile1, 1:[256x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[256x64], 1]
		Tile0: [0, 16384, 16384], Tile1: [0, 16384, 16384], Tile2; [0, 16384, 16384]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 5120 [Tile0, 1:[256x20], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[256x20], 1]
		Tile0: [0, 5120, 5120], Tile1: [0, 5120, 5120], Tile2; [0, 5120, 5120]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 256 [Tile1, 1:[1x64], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x64], 4]
		Tile0: [0, 256, 256], Tile1: [0, 256, 256], Tile2; [0, 256, 256]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1280 [Tile1, 1:[20x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[20x64], 1]
		Tile0: [0, 1280, 1280], Tile1: [0, 1280, 1280], Tile2; [0, 1280, 1280]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile1, 1:[1x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x64], 1]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile1, 1:[1x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x64], 1]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->W_In1 = (unsigned short int) (256);
	KerArg0->H_In1 = (unsigned short int) (64);
	KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+19072);
	KerArg0->W_In2 = (unsigned short int) (20);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+16384);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+17920);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+17984);
	KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+16640);
	KerArg0->W_Out = (unsigned short int) (20);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+18048);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+24192);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 16384, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+19072), 5120, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+16384), 256, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+17920), 64, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+17984), 64, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+24192), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile1 */
		int T1Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->OutFirstCol = (unsigned short int) ((0)*20);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+24192))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+16640), 1280, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S179_Conv2d_128x64x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 51500 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerSetBias_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerConvLinReduct_SQ8_T S_KerArg2, *KerArg2 = &S_KerArg2;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Last, D1Ind_NextLast, D1Ind_NextNextLast;
	int T0Ind, T0Ind_Last, T0Ind_NextLast;
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast, D0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _NN_Filter;
	unsigned int _SN_Filter, _SNN_Filter;
	unsigned int _LN_Filter, _LNN_Filter;
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: Init: 128, Tiled: 1][Tile0 Dim: 1][D0 Dim: Init: 64, Tiled: 4]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 3072 [D1, [0 x 3072, 3072]][Tile0, 1:[3x2], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 3072, 3072]][Tile0, 1:[3x2], 4]
		Tile0: [0, 3072, 24], Tile1: [0, 3072, 24], Tile2; [0, 3072, 24]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 512 [D1, [0 x 512, 512]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 512, 512]]
		Tile0: [0, 512, 512], Tile1: [0, 512, 512], Tile2; [0, 512, 512]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [D1, [0 x 128, 128]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 128, 128]]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [D1, [0 x 128, 128]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 128, 128]]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 73728 [D1, [0 x 73728, 73728]][D0, [3 x 23040, 4608]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 73728, 73728]][D0, [3 x 23040, 4608]]
		Tile0: [0, 23040, 180], Tile1: [180, 23040, 180], Tile2; [360, 23040, 180]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 768 [D1, [0 x 768, 768]][Tile0, 1:[3x2], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 768, 768]][Tile0, 1:[3x2], 1]
		Tile0: [0, 768, 768], Tile1: [0, 768, 768], Tile2; [0, 768, 768]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 1280 [D0, [3 x 400, 80]][Tile0, 1:[5x4], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[5x5, 2:5x5, 5x4], 1][D0, [3 x 400, 80]]
		Tile0: [0, 400, 20], Tile1: [400, 400, 20], Tile2; [800, 400, 20]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+48416);
	KerArg0->W = (unsigned short int) (3);
	KerArg0->H = (unsigned short int) (2);
	KerArg0->Feat = (unsigned short int) (128);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+800);
	KerArg1->W = (unsigned short int) (5);
	KerArg1->UsedW = (unsigned short int) (5);
	KerArg1->OutFeatures = (unsigned short int) (128);
	KerArg1->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+48416);
	KerArg1->Pad = (v4s) ((v4s){1,1,0,1});
	KerArg2->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+48416);
	KerArg2->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+47648);
	KerArg2->Feat = (unsigned short int) (128);
	KerArg2->W = (unsigned short int) (3);
	KerArg2->H = (unsigned short int) (2);
	KerArg2->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+1312);
	KerArg2->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+1440);
	KerArg2->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+51488);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+800), 512, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1312), 128, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1440), 128, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY2D(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Filter+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+25440+0), 23040, 576, 180, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Filter */
	AT_HYPERFLASH_FS_CL_COPY2D(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Filter+180), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+25440+23040), 23040, 576, 180, 0, &UchanHF1);
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+25440+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1568+0), 23040, 0, &DmaR_Evt4);
	_NN_Filter=180; _SN_Filter=23040;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 400, 20, 20, 0, &DmaR_Evt5);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+51488), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D1 */
		int D1Ind_Last = 1, D1Ind_NextLast = 1, D1Ind_NextNextLast = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+51488))[5]);
			AT_FORK(gap_ncore(), (void *) KerParSetBiasB32_SQ8, (void *) KerArg0);
			__CALL(KerParSetBiasB32_SQ8, KerArg0);
			for (D0Ind=0; D0Ind<4; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
				int D0Ind_Last = (D0Ind==3), D0Ind_NextLast = ((D0Ind+1)==3), D0Ind_NextNextLast = ((D0Ind+2)==3);
				/*================================= Prepare Tiles ===================================*/
				_SNN_Filter = 0;
				if (!(D0Ind_Last)) {
					if (!(D0Ind_NextLast)) {
						_NN_Filter = _NN_Filter + (180); _LNN_Filter = ((D0Ind_NextNextLast)?36:180); _SNN_Filter = (128*_LNN_Filter); 
					} else if (!((1))) {
						_NN_Filter = _NN_Filter + (-540); _LNN_Filter = (180); _SNN_Filter = (128*_LNN_Filter); 
					}
				} else if (!((1))) {
					_NN_Filter = _NN_Filter + (180); _LNN_Filter = (180); _SNN_Filter = (128*_LNN_Filter); 
				}
				_SN_In = 0;
				if (!(D0Ind_Last)) {
					_N_In = _N_In + (400); _LN_In = (20); _SN_In = (((D0Ind_NextLast)?4:20)*_LN_In); 
				} else if (!(1)) {
					_N_In = _N_In + (-1200); _LN_In = (20); _SN_In = (20*_LN_In); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Filter */
				if (_SNN_Filter) {
					AT_HYPERFLASH_FS_CL_COPY2D(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Filter+_NN_Filter), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+25440+23040*((D0Ind_Total)%2)),
							_SNN_Filter, 576, _LNN_Filter, 0, &UchanHF1);
				}
				AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
				if (_SN_Filter) {
					AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+25440+23040*((D0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1568+23040*((D0Ind_Total+1)%2)),
							_SN_Filter, 0, &DmaR_Evt4);
				}
				AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+400*((D0Ind_Total+1)%2)),
							_SN_In, 20, _LN_In, 0, &DmaR_Evt5);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+400*((D0Ind_Total)%2));
				KerArg1->H = (unsigned short int) (5-0*(1)-1*(1));
				KerArg1->UsedH = (unsigned short int) (5-0*(1)-1*(1));
				KerArg1->InFeatures = (unsigned short int) ((D0Ind_Last)?4:20);
				KerArg1->TotalInFeatures = (unsigned short int) ((D0Ind_Last)?4:20);
				KerArg1->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+1568+23040*((D0Ind_Total)%2));
				AT_FORK(gap_ncore(), (void *) KerParConv3x3Stride2_SQ8, (void *) KerArg1);
				__CALL(KerParConv3x3Stride2_SQ8, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				_SN_Filter = _SNN_Filter;_LN_Filter = _LNN_Filter;
				/*============================= End Update Arg Pipeline =============================*/
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg2);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg2);
		} /* End iteration on Tile0 */
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+47648), 768, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S182_Conv2d_64x128x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 10252 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 1][Tile0 Dim: 1]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 512 [Tile0, 1:[512x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[512x1], 1]
		Tile0: [0, 512, 512], Tile1: [0, 512, 512], Tile2; [0, 512, 512]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 8192 [Tile1, 1:[128x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[128x64], 1]
		Tile0: [0, 8192, 8192], Tile1: [0, 8192, 8192], Tile2; [0, 8192, 8192]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 768 [Tile0, 1:[128x6], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[128x6], 1]
		Tile0: [0, 768, 768], Tile1: [0, 768, 768], Tile2; [0, 768, 768]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 256 [Tile1, 1:[1x64], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x64], 4]
		Tile0: [0, 256, 256], Tile1: [0, 256, 256], Tile2; [0, 256, 256]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 384 [Tile1, 1:[6x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[6x64], 1]
		Tile0: [0, 384, 384], Tile1: [0, 384, 384], Tile2; [0, 384, 384]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile1, 1:[1x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x64], 1]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile1, 1:[1x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x64], 1]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->W_In1 = (unsigned short int) (128);
	KerArg0->H_In1 = (unsigned short int) (64);
	KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+9472);
	KerArg0->W_In2 = (unsigned short int) (6);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+8192);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+8832);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+8896);
	KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+8448);
	KerArg0->W_Out = (unsigned short int) (6);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+8960);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+10240);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+25440+0), 8192, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read In1 */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+25440+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 8192, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+9472), 768, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+8192), 256, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+8832), 64, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+8896), 64, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10240), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile1 */
		int T1Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->OutFirstCol = (unsigned short int) ((0)*6);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+10240))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+8448), 384, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S185_Conv2d_128x64x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 48380 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerSetBias_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerConvLinReduct_SQ8_T S_KerArg2, *KerArg2 = &S_KerArg2;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Last, D1Ind_NextLast, D1Ind_NextNextLast;
	int T0Ind, T0Ind_Last, T0Ind_NextLast;
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast, D0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _NN_Filter;
	unsigned int _SN_Filter, _SNN_Filter;
	unsigned int _LN_Filter, _LNN_Filter;
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: Init: 128, Tiled: 1][Tile0 Dim: 1][D0 Dim: Init: 64, Tiled: 4]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1024 [D1, [0 x 1024, 1024]][Tile0, 1:[2x1], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 1024, 1024]][Tile0, 1:[2x1], 4]
		Tile0: [0, 1024, 8], Tile1: [0, 1024, 8], Tile2; [0, 1024, 8]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 512 [D1, [0 x 512, 512]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 512, 512]]
		Tile0: [0, 512, 512], Tile1: [0, 512, 512], Tile2; [0, 512, 512]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [D1, [0 x 128, 128]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 128, 128]]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [D1, [0 x 128, 128]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 128, 128]]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 73728 [D1, [0 x 73728, 73728]][D0, [3 x 23040, 4608]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 73728, 73728]][D0, [3 x 23040, 4608]]
		Tile0: [0, 23040, 180], Tile1: [180, 23040, 180], Tile2; [360, 23040, 180]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 256 [D1, [0 x 256, 256]][Tile0, 1:[2x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 256, 256]][Tile0, 1:[2x1], 1]
		Tile0: [0, 256, 256], Tile1: [0, 256, 256], Tile2; [0, 256, 256]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 384 [D0, [3 x 120, 24]][Tile0, 1:[3x2], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[3x3, 2:3x3, 3x2], 1][D0, [3 x 120, 24]]
		Tile0: [0, 120, 6], Tile1: [120, 120, 6], Tile2; [240, 120, 6]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+47344);
	KerArg0->W = (unsigned short int) (2);
	KerArg0->H = (unsigned short int) (1);
	KerArg0->Feat = (unsigned short int) (128);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+240);
	KerArg1->W = (unsigned short int) (3);
	KerArg1->UsedW = (unsigned short int) (3);
	KerArg1->OutFeatures = (unsigned short int) (128);
	KerArg1->Out = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+47344);
	KerArg1->Pad = (v4s) ((v4s){1,1,0,1});
	KerArg2->In = (int *__restrict__) (SSD_tin_can_bottle_L1_Memory+47344);
	KerArg2->Out = (void *__restrict__) (SSD_tin_can_bottle_L1_Memory+47088);
	KerArg2->Feat = (unsigned short int) (128);
	KerArg2->W = (unsigned short int) (2);
	KerArg2->H = (unsigned short int) (1);
	KerArg2->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+752);
	KerArg2->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+880);
	KerArg2->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+48368);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+240), 512, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+752), 128, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+880), 128, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY2D(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Filter+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+24032+0), 23040, 576, 180, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Filter */
	AT_HYPERFLASH_FS_CL_COPY2D(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Filter+180), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+24032+23040), 23040, 576, 180, 0, &UchanHF1);
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+24032+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1008+0), 23040, 0, &DmaR_Evt4);
	_NN_Filter=180; _SN_Filter=23040;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+0), 120, 6, 6, 0, &DmaR_Evt5);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48368), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D1 */
		int D1Ind_Last = 1, D1Ind_NextLast = 1, D1Ind_NextNextLast = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+48368))[5]);
			AT_FORK(gap_ncore(), (void *) KerParSetBiasB32_SQ8, (void *) KerArg0);
			__CALL(KerParSetBiasB32_SQ8, KerArg0);
			for (D0Ind=0; D0Ind<4; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
				int D0Ind_Last = (D0Ind==3), D0Ind_NextLast = ((D0Ind+1)==3), D0Ind_NextNextLast = ((D0Ind+2)==3);
				/*================================= Prepare Tiles ===================================*/
				_SNN_Filter = 0;
				if (!(D0Ind_Last)) {
					if (!(D0Ind_NextLast)) {
						_NN_Filter = _NN_Filter + (180); _LNN_Filter = ((D0Ind_NextNextLast)?36:180); _SNN_Filter = (128*_LNN_Filter); 
					} else if (!((1))) {
						_NN_Filter = _NN_Filter + (-540); _LNN_Filter = (180); _SNN_Filter = (128*_LNN_Filter); 
					}
				} else if (!((1))) {
					_NN_Filter = _NN_Filter + (180); _LNN_Filter = (180); _SNN_Filter = (128*_LNN_Filter); 
				}
				_SN_In = 0;
				if (!(D0Ind_Last)) {
					_N_In = _N_In + (120); _LN_In = (6); _SN_In = (((D0Ind_NextLast)?4:20)*_LN_In); 
				} else if (!(1)) {
					_N_In = _N_In + (-360); _LN_In = (6); _SN_In = (20*_LN_In); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Filter */
				if (_SNN_Filter) {
					AT_HYPERFLASH_FS_CL_COPY2D(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Filter+_NN_Filter), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+24032+23040*((D0Ind_Total)%2)),
							_SNN_Filter, 576, _LNN_Filter, 0, &UchanHF1);
				}
				AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
				if (_SN_Filter) {
					AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+24032+23040*((D0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1008+23040*((D0Ind_Total+1)%2)),
							_SN_Filter, 0, &DmaR_Evt4);
				}
				AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0+120*((D0Ind_Total+1)%2)),
							_SN_In, 6, _LN_In, 0, &DmaR_Evt5);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0+120*((D0Ind_Total)%2));
				KerArg1->H = (unsigned short int) (3-0*(1)-1*(1));
				KerArg1->UsedH = (unsigned short int) (3-0*(1)-1*(1));
				KerArg1->InFeatures = (unsigned short int) ((D0Ind_Last)?4:20);
				KerArg1->TotalInFeatures = (unsigned short int) ((D0Ind_Last)?4:20);
				KerArg1->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+1008+23040*((D0Ind_Total)%2));
				AT_FORK(gap_ncore(), (void *) KerParConv3x3Stride2_SQ8, (void *) KerArg1);
				__CALL(KerParConv3x3Stride2_SQ8, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				_SN_Filter = _SNN_Filter;_LN_Filter = _LNN_Filter;
				/*============================= End Update Arg Pipeline =============================*/
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg2);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg2);
		} /* End iteration on Tile0 */
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+47088), 256, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S188_Conv2d_32x128x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 5132 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 1][Tile0 Dim: 1]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 512 [Tile0, 1:[512x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[512x1], 1]
		Tile0: [0, 512, 512], Tile1: [0, 512, 512], Tile2; [0, 512, 512]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 4096 [Tile1, 1:[128x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[128x32], 1]
		Tile0: [0, 4096, 4096], Tile1: [0, 4096, 4096], Tile2; [0, 4096, 4096]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 256 [Tile0, 1:[128x2], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[128x2], 1]
		Tile0: [0, 256, 256], Tile1: [0, 256, 256], Tile2; [0, 256, 256]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [Tile1, 1:[1x32], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x32], 4]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile1, 1:[2x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[2x32], 1]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [Tile1, 1:[1x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x32], 1]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [Tile1, 1:[1x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x32], 1]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->W_In1 = (unsigned short int) (128);
	KerArg0->H_In1 = (unsigned short int) (32);
	KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+4864);
	KerArg0->W_In2 = (unsigned short int) (2);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+4096);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+4288);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+4320);
	KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+4224);
	KerArg0->W_Out = (unsigned short int) (2);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+4352);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+5120);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23712+0), 4096, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read In1 */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23712+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 4096, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+4864), 256, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+4096), 128, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+4288), 32, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+4320), 32, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+5120), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile1 */
		int T1Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->OutFirstCol = (unsigned short int) ((0)*2);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+5120))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+4224), 64, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S191_Conv2d_64x32x1x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 6764 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	Ker_MM_Conv_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Last;
	int T0Ind, T0Ind_Last;
	int D0Ind, D0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: Init: 64, Tiled: 1][Tile0 Dim: 1][D0 Dim: Init: 32, Tiled: 1]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile0, 1:[96x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[96x1], 1]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [D0, [0 x 64, 64]][Tile0, 1:[2x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[2x1], 1][D0, [0 x 64, 64]]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 256 [D1, [0 x 256, 256]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 256, 256]]
		Tile0: [0, 256, 256], Tile1: [0, 256, 256], Tile2; [0, 256, 256]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [D1, [0 x 64, 64]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 64, 64]]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [D1, [0 x 64, 64]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 64, 64]]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6144 [D1, [0 x 6144, 6144]][D0, [0 x 96, 96]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 6144, 6144]][D0, [0 x 96, 96]]
		Tile0: [0, 6144, 6144], Tile1: [0, 6144, 6144], Tile2; [0, 6144, 6144]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [D1, [0 x 64, 64]][Tile0, 1:[1x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 64, 64]][Tile0, 1:[1x1], 1]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+96);
	KerArg0->W = (unsigned short int) (2);
	KerArg0->H = (unsigned short int) (1);
	KerArg0->Fx = (unsigned char) (3);
	KerArg0->Sx = (unsigned char) (2);
	KerArg0->Sy = (unsigned char) (2);
	KerArg0->FirstTile = (unsigned char) ((1));
	KerArg0->Pad = (v4s) ((v4s){0,1,0,0});
	KerArg0->Filter = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+544);
	KerArg0->Bias = (int * __restrict__) (SSD_tin_can_bottle_L1_Memory+160);
	KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+6688);
	KerArg0->InFeat = (unsigned short int) (32);
	KerArg0->OutFeat = (unsigned short int) (64);
	KerArg0->Wo = (unsigned short int) (1);
	KerArg0->Ho = (unsigned short int) (1);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+416);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+480);
	KerArg0->ColBuff = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->Infos = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+6752);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+96), 64, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+160), 256, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+416), 64, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+480), 64, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Filter+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23712+0), 6144, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Filter */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23712+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+544), 6144, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6752), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D1 */
		int D1Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			{ /* Single iteration on D0 */
				int D0Ind_Last = 1;
				/*====================== Call Kernel LOC_D0 =========================*/
				AT_FORK(gap_ncore(), (void *) KerPar_MM_Conv1D_ReLU_SQ8, (void *) KerArg0);
				__CALL(KerPar_MM_Conv1D_ReLU_SQ8, KerArg0);
			} /* End iteration on D0 */
		} /* End iteration on Tile0 */
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6688), 64, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S194_Conv2d_12x288x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 47892 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 5][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 5 logical tiles, 5 physical tiles
			Total Size: 86400 [Tile1, 5:[288x72, 3:288x72, 288x12], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 5:[288x72, 3:288x72, 288x12], 1]
		Tile0: [0, 20736, 72], Tile1: [72, 20736, 72], Tile2; [144, 20736, 72]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 3456 [Tile0, 1:[288x12], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[288x12], 1]
		Tile0: [0, 3456, 3456], Tile1: [0, 3456, 3456], Tile2; [0, 3456, 3456]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 1 physical tiles
			Total Size: 1152 [Tile1, 5:[1152x1, 3:1152x1, 1152x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 5:[1152x1, 3:1152x1, 1152x1], 1]
		Tile0: [0, 1152, 1152], Tile1: [0, 1152, 1152], Tile2; [0, 1152, 1152]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 48 [Tile0, 1:[1x12], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x12], 4]
		Tile0: [0, 48, 48], Tile1: [0, 48, 48], Tile2; [0, 48, 48]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 5 physical tiles
			Total Size: 3600 [Tile1, 5:[12x72, 3:12x72, 12x12], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 5:[12x72, 3:12x72, 12x12], 1]
		Tile0: [0, 864, 72], Tile1: [72, 864, 72], Tile2; [144, 864, 72]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 12 [Tile0, 1:[1x12], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x12], 1]
		Tile0: [0, 12, 12], Tile1: [0, 12, 12], Tile2; [0, 12, 12]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 12 [Tile0, 1:[1x12], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x12], 1]
		Tile0: [0, 12, 12], Tile1: [0, 12, 12], Tile2; [0, 12, 12]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 5:[1x1, 3:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 5:[1x1, 3:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44364);
	KerArg0->W_In1 = (unsigned short int) (288);
	KerArg0->H_In1 = (unsigned short int) (12);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+47820);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+47868);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+47880);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+44352);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1152+0), 20736, 300, 72, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44364), 3456, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+47820), 48, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=864; _LC_Out=72;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+47868), 12, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+47880), 12, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44352), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<5; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==4), T1Ind_NextLast = ((T1Ind+1)==4);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (72); _LN_In2 = ((T1Ind_NextLast)?12:72); _SN_In2 = (288*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1152+20736*((T1Ind_Total+1)%2)),
					_SN_In2, 300, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+1152+20736*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?12:72);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+42624+864*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?12:72);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*12);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+44352))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+42624+864*((T1Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (72); _LC_Out = ((T1Ind_NextLast)?12:72); _SC_Out = (12*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S195_Op_CONV_2D_0_71_trans_out0(
		signed char * __restrict__ In,
		signed char * __restrict__ Out)

{
	/* Shared L1: 7200 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerMatTranspose_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 3600 [Tile0, 1:[180x20], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[180x20], 1]
		Tile0: [0, 3600, 3600], Tile1: [0, 3600, 3600], Tile2; [0, 3600, 3600]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 3600 [Tile0, 1:[15x240], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[15x240], 1]
		Tile0: [0, 3600, 3600], Tile1: [0, 3600, 3600], Tile2; [0, 3600, 3600]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+3600);
	KerArg0->Feat = (unsigned short int) (12);
	KerArg0->W = (unsigned short int) (20);
	KerArg0->H = (unsigned short int) (15);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 3600, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		AT_FORK(gap_ncore(), (void *) CNN_MatPermCHW2HWC_fps, (void *) KerArg0);
		__CALL(CNN_MatPermCHW2HWC_fps, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+3600), 3600, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S199_Conv2d_9x288x1x1_Hsigmoid(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 48960 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerActivation_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 4][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 86400 [Tile1, 4:[288x76, 2:288x76, 288x72], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[288x76, 2:288x76, 288x72], 1]
		Tile0: [0, 21888, 76], Tile1: [76, 21888, 76], Tile2; [152, 21888, 76]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 2592 [Tile0, 1:[288x9], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[288x9], 1]
		Tile0: [0, 2592, 2592], Tile1: [0, 2592, 2592], Tile2; [0, 2592, 2592]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 1152 [Tile1, 4:[1152x1, 2:1152x1, 1152x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1152x1, 2:1152x1, 1152x1], 1]
		Tile0: [0, 1152, 1152], Tile1: [0, 1152, 1152], Tile2; [0, 1152, 1152]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 36 [Tile0, 1:[1x9], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x9], 4]
		Tile0: [0, 36, 36], Tile1: [0, 36, 36], Tile2; [0, 36, 36]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 2700 [Tile1, 4:[9x76, 2:9x76, 9x72], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[9x76, 2:9x76, 9x72], 1]
		Tile0: [0, 684, 76], Tile1: [76, 684, 76], Tile2; [152, 684, 76]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x9], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x9], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x9], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x9], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+46308);
	KerArg0->W_In1 = (unsigned short int) (288);
	KerArg0->H_In1 = (unsigned short int) (9);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+48900);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+48936);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+48948);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+46296);
	KerArg1->Feat = (unsigned short int) (1);
	KerArg1->H = (unsigned short int) (9);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+46296);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1152+0), 21888, 300, 76, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+46308), 2592, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48900), 36, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=684; _LC_Out=76;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48936), 9, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48948), 9, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+46296), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<4; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==3), T1Ind_NextLast = ((T1Ind+1)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (76); _LN_In2 = ((T1Ind_NextLast)?72:76); _SN_In2 = (288*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1152+21888*((T1Ind_Total+1)%2)),
					_SN_In2, 300, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+1152+21888*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?72:76);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+44928+684*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?72:76);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*9);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+46296))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*====================== Call Kernel LOC_LOOP_EPILOG =========================*/
		KerArg1->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+44928+684*((T1Ind_Total)%2));
		KerArg1->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+44928+684*((T1Ind_Total)%2));
		KerArg1->W = (unsigned short int) ((T1Ind_Last)?72:76);
		AT_FORK(gap_ncore(), (void *) Ker_HSigmoid_SQ8, (void *) KerArg1);
		__CALL(Ker_HSigmoid_SQ8, KerArg1);
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+44928+684*((T1Ind_Total)%2)),
				_SC_Out, 300, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (76); _LC_Out = ((T1Ind_NextLast)?72:76); _SC_Out = (9*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S200_Op_CONV_2D_0_73_trans_out0(
		signed char * __restrict__ In,
		signed char * __restrict__ Out)

{
	/* Shared L1: 5400 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerMatTranspose_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 2700 [Tile0, 1:[135x20], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[135x20], 1]
		Tile0: [0, 2700, 2700], Tile1: [0, 2700, 2700], Tile2; [0, 2700, 2700]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 2700 [Tile0, 1:[15x180], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[15x180], 1]
		Tile0: [0, 2700, 2700], Tile1: [0, 2700, 2700], Tile2; [0, 2700, 2700]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+2700);
	KerArg0->Feat = (unsigned short int) (9);
	KerArg0->W = (unsigned short int) (20);
	KerArg0->H = (unsigned short int) (15);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 2700, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		AT_FORK(gap_ncore(), (void *) CNN_MatPermCHW2HWC_fps, (void *) KerArg0);
		__CALL(CNN_MatPermCHW2HWC_fps, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+2700), 2700, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S204_Conv2d_24x640x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 34012 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 7][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 51200 [Tile1, 7:[640x12, 5:640x12, 640x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 7:[640x12, 5:640x12, 640x8], 1]
		Tile0: [0, 7680, 12], Tile1: [12, 7680, 12], Tile2; [24, 7680, 12]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 15360 [Tile0, 1:[640x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[640x24], 1]
		Tile0: [0, 15360, 15360], Tile1: [0, 15360, 15360], Tile2; [0, 15360, 15360]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 2560 [Tile1, 7:[2560x1, 5:2560x1, 2560x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 7:[2560x1, 5:2560x1, 2560x1], 1]
		Tile0: [0, 2560, 2560], Tile1: [0, 2560, 2560], Tile2; [0, 2560, 2560]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile0, 1:[1x24], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x24], 4]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 1920 [Tile1, 7:[24x12, 5:24x12, 24x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 7:[24x12, 5:24x12, 24x8], 1]
		Tile0: [0, 288, 12], Tile1: [12, 288, 12], Tile2; [24, 288, 12]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 24 [Tile0, 1:[1x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x24], 1]
		Tile0: [0, 24, 24], Tile1: [0, 24, 24], Tile2; [0, 24, 24]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 24 [Tile0, 1:[1x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x24], 1]
		Tile0: [0, 24, 24], Tile1: [0, 24, 24], Tile2; [0, 24, 24]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 7:[1x1, 5:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 7:[1x1, 5:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+18508);
	KerArg0->W_In1 = (unsigned short int) (640);
	KerArg0->H_In1 = (unsigned short int) (24);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+33868);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+33964);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+33988);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+18496);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+2560+0), 7680, 80, 12, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+18508), 15360, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+33868), 96, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=288; _LC_Out=12;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+33964), 24, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+33988), 24, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+18496), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<7; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==6), T1Ind_NextLast = ((T1Ind+1)==6);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (12); _LN_In2 = ((T1Ind_NextLast)?8:12); _SN_In2 = (640*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+2560+7680*((T1Ind_Total+1)%2)),
					_SN_In2, 80, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+2560+7680*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?8:12);
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+17920+288*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?8:12);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*24);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+18496))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+17920+288*((T1Ind_Total)%2)),
				_SC_Out, 80, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (12); _LC_Out = ((T1Ind_NextLast)?8:12); _SC_Out = (24*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S205_Op_CONV_2D_0_75_trans_out0(
		signed char * __restrict__ In,
		signed char * __restrict__ Out)

{
	/* Shared L1: 3840 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerMatTranspose_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1920 [Tile0, 1:[192x10], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[192x10], 1]
		Tile0: [0, 1920, 1920], Tile1: [0, 1920, 1920], Tile2; [0, 1920, 1920]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1920 [Tile0, 1:[8x240], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[8x240], 1]
		Tile0: [0, 1920, 1920], Tile1: [0, 1920, 1920], Tile2; [0, 1920, 1920]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+1920);
	KerArg0->Feat = (unsigned short int) (24);
	KerArg0->W = (unsigned short int) (10);
	KerArg0->H = (unsigned short int) (8);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 1920, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		AT_FORK(gap_ncore(), (void *) CNN_MatPermCHW2HWC_fps, (void *) KerArg0);
		__CALL(CNN_MatPermCHW2HWC_fps, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1920), 1920, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S209_Conv2d_18x640x1x1_Hsigmoid(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 40524 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerActivation_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 4][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 51200 [Tile1, 4:[640x20, 2:640x20, 640x20], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[640x20, 2:640x20, 640x20], 1]
		Tile0: [0, 12800, 20], Tile1: [20, 12800, 20], Tile2; [40, 12800, 20]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 11520 [Tile0, 1:[640x18], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[640x18], 1]
		Tile0: [0, 11520, 11520], Tile1: [0, 11520, 11520], Tile2; [0, 11520, 11520]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 2560 [Tile1, 4:[2560x1, 2:2560x1, 2560x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[2560x1, 2:2560x1, 2560x1], 1]
		Tile0: [0, 2560, 2560], Tile1: [0, 2560, 2560], Tile2; [0, 2560, 2560]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 72 [Tile0, 1:[1x18], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x18], 4]
		Tile0: [0, 72, 72], Tile1: [0, 72, 72], Tile2; [0, 72, 72]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 1440 [Tile1, 4:[18x20, 2:18x20, 18x20], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[18x20, 2:18x20, 18x20], 1]
		Tile0: [0, 360, 20], Tile1: [20, 360, 20], Tile2; [40, 360, 20]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 18 [Tile0, 1:[1x18], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x18], 1]
		Tile0: [0, 18, 18], Tile1: [0, 18, 18], Tile2; [0, 18, 18]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 18 [Tile0, 1:[1x18], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x18], 1]
		Tile0: [0, 18, 18], Tile1: [0, 18, 18], Tile2; [0, 18, 18]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 4:[1x1, 2:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+28892);
	KerArg0->W_In1 = (unsigned short int) (640);
	KerArg0->H_In1 = (unsigned short int) (18);
	KerArg0->W_In2 = (unsigned short int) (20);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+40412);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+40484);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+40504);
	KerArg0->W_Out = (unsigned short int) (20);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+28880);
	KerArg1->Feat = (unsigned short int) (1);
	KerArg1->W = (unsigned short int) (20);
	KerArg1->H = (unsigned short int) (18);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+28880);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+2560+0), 12800, 80, 20, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+28892), 11520, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+40412), 72, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=360; _LC_Out=20;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+40484), 18, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+40504), 18, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+28880), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<4; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==3), T1Ind_NextLast = ((T1Ind+1)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (20); _LN_In2 = (20); _SN_In2 = (640*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+2560+12800*((T1Ind_Total+1)%2)),
					_SN_In2, 80, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+2560+12800*((T1Ind_Total)%2));
			KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+28160+360*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((0)*18);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+28880))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*====================== Call Kernel LOC_LOOP_EPILOG =========================*/
		KerArg1->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+28160+360*((T1Ind_Total)%2));
		KerArg1->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+28160+360*((T1Ind_Total)%2));
		AT_FORK(gap_ncore(), (void *) Ker_HSigmoid_SQ8, (void *) KerArg1);
		__CALL(Ker_HSigmoid_SQ8, KerArg1);
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+28160+360*((T1Ind_Total)%2)),
				_SC_Out, 80, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (20); _LC_Out = (20); _SC_Out = (18*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S210_Op_CONV_2D_0_77_trans_out0(
		signed char * __restrict__ In,
		signed char * __restrict__ Out)

{
	/* Shared L1: 2880 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerMatTranspose_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1440 [Tile0, 1:[144x10], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[144x10], 1]
		Tile0: [0, 1440, 1440], Tile1: [0, 1440, 1440], Tile2; [0, 1440, 1440]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1440 [Tile0, 1:[8x180], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[8x180], 1]
		Tile0: [0, 1440, 1440], Tile1: [0, 1440, 1440], Tile2; [0, 1440, 1440]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+1440);
	KerArg0->Feat = (unsigned short int) (18);
	KerArg0->W = (unsigned short int) (10);
	KerArg0->H = (unsigned short int) (8);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 1440, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		AT_FORK(gap_ncore(), (void *) CNN_MatPermCHW2HWC_fps, (void *) KerArg0);
		__CALL(CNN_MatPermCHW2HWC_fps, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1440), 1440, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S214_Conv2d_24x256x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 12924 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 1][Tile0 Dim: 1]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1024 [Tile0, 1:[1024x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1024x1], 1]
		Tile0: [0, 1024, 1024], Tile1: [0, 1024, 1024], Tile2; [0, 1024, 1024]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6144 [Tile1, 1:[256x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[256x24], 1]
		Tile0: [0, 6144, 6144], Tile1: [0, 6144, 6144], Tile2; [0, 6144, 6144]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 5120 [Tile0, 1:[256x20], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[256x20], 1]
		Tile0: [0, 5120, 5120], Tile1: [0, 5120, 5120], Tile2; [0, 5120, 5120]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile1, 1:[1x24], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x24], 4]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 480 [Tile1, 1:[20x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[20x24], 1]
		Tile0: [0, 480, 480], Tile1: [0, 480, 480], Tile2; [0, 480, 480]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 24 [Tile1, 1:[1x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x24], 1]
		Tile0: [0, 24, 24], Tile1: [0, 24, 24], Tile2; [0, 24, 24]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 24 [Tile1, 1:[1x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x24], 1]
		Tile0: [0, 24, 24], Tile1: [0, 24, 24], Tile2; [0, 24, 24]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->W_In1 = (unsigned short int) (256);
	KerArg0->H_In1 = (unsigned short int) (24);
	KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+7792);
	KerArg0->W_In2 = (unsigned short int) (20);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+6144);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+6720);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+6744);
	KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+6240);
	KerArg0->W_Out = (unsigned short int) (20);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+6768);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+12912);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 6144, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+7792), 5120, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6144), 96, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6720), 24, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6744), 24, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+12912), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile1 */
		int T1Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->OutFirstCol = (unsigned short int) ((0)*20);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+12912))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6240), 480, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S215_Op_CONV_2D_0_79_trans_out0(
		signed char * __restrict__ In,
		signed char * __restrict__ Out)

{
	/* Shared L1: 960 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerMatTranspose_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 480 [Tile0, 1:[96x5], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[96x5], 1]
		Tile0: [0, 480, 480], Tile1: [0, 480, 480], Tile2; [0, 480, 480]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 480 [Tile0, 1:[4x120], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[4x120], 1]
		Tile0: [0, 480, 480], Tile1: [0, 480, 480], Tile2; [0, 480, 480]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+480);
	KerArg0->Feat = (unsigned short int) (24);
	KerArg0->W = (unsigned short int) (5);
	KerArg0->H = (unsigned short int) (4);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 480, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		AT_FORK(gap_ncore(), (void *) CNN_MatPermCHW2HWC_fps, (void *) KerArg0);
		__CALL(CNN_MatPermCHW2HWC_fps, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+480), 480, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S219_Conv2d_18x256x1x1_Hsigmoid(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 11236 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerActivation_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 1][Tile0 Dim: 1]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1024 [Tile1, 1:[1024x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1024x1], 1]
		Tile0: [0, 1024, 1024], Tile1: [0, 1024, 1024], Tile2; [0, 1024, 1024]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 4608 [Tile0, 1:[256x18], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[256x18], 1]
		Tile0: [0, 4608, 4608], Tile1: [0, 4608, 4608], Tile2; [0, 4608, 4608]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 5120 [Tile1, 1:[256x20], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[256x20], 1]
		Tile0: [0, 5120, 5120], Tile1: [0, 5120, 5120], Tile2; [0, 5120, 5120]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 72 [Tile0, 1:[1x18], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x18], 4]
		Tile0: [0, 72, 72], Tile1: [0, 72, 72], Tile2; [0, 72, 72]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 360 [Tile1, 1:[18x20], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[18x20], 1]
		Tile0: [0, 360, 360], Tile1: [0, 360, 360], Tile2; [0, 360, 360]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 18 [Tile0, 1:[1x18], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x18], 1]
		Tile0: [0, 18, 18], Tile1: [0, 18, 18], Tile2; [0, 18, 18]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 18 [Tile0, 1:[1x18], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x18], 1]
		Tile0: [0, 18, 18], Tile1: [0, 18, 18], Tile2; [0, 18, 18]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+6516);
	KerArg0->W_In1 = (unsigned short int) (256);
	KerArg0->H_In1 = (unsigned short int) (18);
	KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+1024);
	KerArg0->W_In2 = (unsigned short int) (20);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+11124);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+11196);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+11216);
	KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+6144);
	KerArg0->W_Out = (unsigned short int) (20);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+6504);
	KerArg1->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+6144);
	KerArg1->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+6144);
	KerArg1->Feat = (unsigned short int) (1);
	KerArg1->W = (unsigned short int) (20);
	KerArg1->H = (unsigned short int) (18);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+6504);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6516), 4608, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1024), 5120, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+11124), 72, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+11196), 18, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+11216), 18, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6504), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile1 */
		int T1Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->OutFirstCol = (unsigned short int) ((0)*18);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+6504))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*====================== Call Kernel LOC_LOOP_EPILOG =========================*/
		AT_FORK(gap_ncore(), (void *) Ker_HSigmoid_SQ8, (void *) KerArg1);
		__CALL(Ker_HSigmoid_SQ8, KerArg1);
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6144), 360, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S220_Op_CONV_2D_0_81_trans_out0(
		signed char * __restrict__ In,
		signed char * __restrict__ Out)

{
	/* Shared L1: 720 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerMatTranspose_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 360 [Tile0, 1:[72x5], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[72x5], 1]
		Tile0: [0, 360, 360], Tile1: [0, 360, 360], Tile2; [0, 360, 360]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 360 [Tile0, 1:[4x90], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[4x90], 1]
		Tile0: [0, 360, 360], Tile1: [0, 360, 360], Tile2; [0, 360, 360]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+360);
	KerArg0->Feat = (unsigned short int) (18);
	KerArg0->W = (unsigned short int) (5);
	KerArg0->H = (unsigned short int) (4);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 360, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		AT_FORK(gap_ncore(), (void *) CNN_MatPermCHW2HWC_fps, (void *) KerArg0);
		__CALL(CNN_MatPermCHW2HWC_fps, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+360), 360, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S224_Conv2d_24x128x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 4652 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 1][Tile0 Dim: 1]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 512 [Tile0, 1:[512x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[512x1], 1]
		Tile0: [0, 512, 512], Tile1: [0, 512, 512], Tile2; [0, 512, 512]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 3072 [Tile1, 1:[128x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[128x24], 1]
		Tile0: [0, 3072, 3072], Tile1: [0, 3072, 3072], Tile2; [0, 3072, 3072]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 768 [Tile0, 1:[128x6], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[128x6], 1]
		Tile0: [0, 768, 768], Tile1: [0, 768, 768], Tile2; [0, 768, 768]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile1, 1:[1x24], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x24], 4]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 144 [Tile1, 1:[6x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[6x24], 1]
		Tile0: [0, 144, 144], Tile1: [0, 144, 144], Tile2; [0, 144, 144]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 24 [Tile1, 1:[1x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x24], 1]
		Tile0: [0, 24, 24], Tile1: [0, 24, 24], Tile2; [0, 24, 24]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 24 [Tile1, 1:[1x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x24], 1]
		Tile0: [0, 24, 24], Tile1: [0, 24, 24], Tile2; [0, 24, 24]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->W_In1 = (unsigned short int) (128);
	KerArg0->H_In1 = (unsigned short int) (24);
	KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+3872);
	KerArg0->W_In2 = (unsigned short int) (6);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+3072);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+3312);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+3336);
	KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+3168);
	KerArg0->W_Out = (unsigned short int) (6);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+3360);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+4640);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+25440+0), 3072, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read In1 */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+25440+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 3072, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+3872), 768, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+3072), 96, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+3312), 24, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+3336), 24, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+4640), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile1 */
		int T1Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->OutFirstCol = (unsigned short int) ((0)*6);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+4640))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+3168), 144, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S225_Op_CONV_2D_0_83_trans_out0(
		signed char * __restrict__ In,
		signed char * __restrict__ Out)

{
	/* Shared L1: 288 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerMatTranspose_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 144 [Tile0, 1:[48x3], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[48x3], 1]
		Tile0: [0, 144, 144], Tile1: [0, 144, 144], Tile2; [0, 144, 144]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 144 [Tile0, 1:[2x72], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[2x72], 1]
		Tile0: [0, 144, 144], Tile1: [0, 144, 144], Tile2; [0, 144, 144]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+144);
	KerArg0->Feat = (unsigned short int) (24);
	KerArg0->W = (unsigned short int) (3);
	KerArg0->H = (unsigned short int) (2);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 144, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		AT_FORK(gap_ncore(), (void *) CNN_MatPermCHW2HWC_fps, (void *) KerArg0);
		__CALL(CNN_MatPermCHW2HWC_fps, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+144), 144, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S229_Conv2d_18x128x1x1_Hsigmoid(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 3816 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerActivation_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 1][Tile0 Dim: 1]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 512 [Tile0, 1:[512x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[512x1], 1]
		Tile0: [0, 512, 512], Tile1: [0, 512, 512], Tile2; [0, 512, 512]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 2304 [Tile1, 1:[128x18], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[128x18], 1]
		Tile0: [0, 2304, 2304], Tile1: [0, 2304, 2304], Tile2; [0, 2304, 2304]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 768 [Tile0, 1:[128x6], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[128x6], 1]
		Tile0: [0, 768, 768], Tile1: [0, 768, 768], Tile2; [0, 768, 768]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 72 [Tile1, 1:[1x18], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x18], 4]
		Tile0: [0, 72, 72], Tile1: [0, 72, 72], Tile2; [0, 72, 72]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 108 [Tile1, 1:[6x18], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[6x18], 1]
		Tile0: [0, 108, 108], Tile1: [0, 108, 108], Tile2; [0, 108, 108]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 18 [Tile1, 1:[1x18], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x18], 1]
		Tile0: [0, 18, 18], Tile1: [0, 18, 18], Tile2; [0, 18, 18]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 18 [Tile1, 1:[1x18], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x18], 1]
		Tile0: [0, 18, 18], Tile1: [0, 18, 18], Tile2; [0, 18, 18]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->W_In1 = (unsigned short int) (128);
	KerArg0->H_In1 = (unsigned short int) (18);
	KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+3036);
	KerArg0->W_In2 = (unsigned short int) (6);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+2304);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+2484);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+2504);
	KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+2376);
	KerArg0->W_Out = (unsigned short int) (6);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+2524);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+3804);
	KerArg1->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+2376);
	KerArg1->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+2376);
	KerArg1->Feat = (unsigned short int) (1);
	KerArg1->W = (unsigned short int) (6);
	KerArg1->H = (unsigned short int) (18);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+3804);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+25440+0), 2304, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read In1 */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+25440+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 2304, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+3036), 768, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+2304), 72, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+2484), 18, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+2504), 18, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+3804), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile1 */
		int T1Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->OutFirstCol = (unsigned short int) ((0)*6);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+3804))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*====================== Call Kernel LOC_LOOP_EPILOG =========================*/
		AT_FORK(gap_ncore(), (void *) Ker_HSigmoid_SQ8, (void *) KerArg1);
		__CALL(Ker_HSigmoid_SQ8, KerArg1);
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+2376), 108, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S230_Op_CONV_2D_0_85_trans_out0(
		signed char * __restrict__ In,
		signed char * __restrict__ Out)

{
	/* Shared L1: 216 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerMatTranspose_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 108 [Tile0, 1:[36x3], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[36x3], 1]
		Tile0: [0, 108, 108], Tile1: [0, 108, 108], Tile2; [0, 108, 108]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 108 [Tile0, 1:[2x54], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[2x54], 1]
		Tile0: [0, 108, 108], Tile1: [0, 108, 108], Tile2; [0, 108, 108]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+108);
	KerArg0->Feat = (unsigned short int) (18);
	KerArg0->W = (unsigned short int) (3);
	KerArg0->H = (unsigned short int) (2);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 108, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		AT_FORK(gap_ncore(), (void *) CNN_MatPermCHW2HWC_fps, (void *) KerArg0);
		__CALL(CNN_MatPermCHW2HWC_fps, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+108), 108, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S234_Conv2d_24x128x1x1(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 4044 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 1][Tile0 Dim: 1]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 512 [Tile0, 1:[512x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[512x1], 1]
		Tile0: [0, 512, 512], Tile1: [0, 512, 512], Tile2; [0, 512, 512]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 3072 [Tile1, 1:[128x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[128x24], 1]
		Tile0: [0, 3072, 3072], Tile1: [0, 3072, 3072], Tile2; [0, 3072, 3072]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 256 [Tile0, 1:[128x2], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[128x2], 1]
		Tile0: [0, 256, 256], Tile1: [0, 256, 256], Tile2; [0, 256, 256]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [Tile1, 1:[1x24], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x24], 4]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 48 [Tile1, 1:[2x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[2x24], 1]
		Tile0: [0, 48, 48], Tile1: [0, 48, 48], Tile2; [0, 48, 48]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 24 [Tile1, 1:[1x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x24], 1]
		Tile0: [0, 24, 24], Tile1: [0, 24, 24], Tile2; [0, 24, 24]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 24 [Tile1, 1:[1x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x24], 1]
		Tile0: [0, 24, 24], Tile1: [0, 24, 24], Tile2; [0, 24, 24]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->W_In1 = (unsigned short int) (128);
	KerArg0->H_In1 = (unsigned short int) (24);
	KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+3776);
	KerArg0->W_In2 = (unsigned short int) (2);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+3072);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+3216);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+3240);
	KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+3168);
	KerArg0->W_Out = (unsigned short int) (2);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+3264);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+4032);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23696+0), 3072, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read In1 */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23696+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 3072, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+3776), 256, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+3072), 96, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+3216), 24, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+3240), 24, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+4032), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile1 */
		int T1Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->OutFirstCol = (unsigned short int) ((0)*2);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+4032))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+3168), 48, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S235_Op_CONV_2D_0_87_trans_out0(
		signed char * __restrict__ In,
		signed char * __restrict__ Out)

{
	/* Shared L1: 96 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerMatTranspose_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 1, Tiled: 1][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 48 [D0, [0 x 48, 48]][Tile0, 1:[2x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 48, 48]][Tile0, 1:[2x24], 1]
		Tile0: [0, 48, 48], Tile1: [0, 48, 48], Tile2; [0, 48, 48]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 48 [D0, [0 x 48, 48]][Tile0, 1:[2x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 48, 48]][Tile0, 1:[2x24], 1]
		Tile0: [0, 48, 48], Tile1: [0, 48, 48], Tile2; [0, 48, 48]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+48);
	KerArg0->Feat = (unsigned short int) (1);
	KerArg0->W = (unsigned short int) (2);
	KerArg0->H = (unsigned short int) (24);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 48, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D0 */
		int D0Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			AT_FORK(gap_ncore(), (void *) CNN_ParTranspose_fps, (void *) KerArg0);
			__CALL(CNN_ParTranspose_fps, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+48), 48, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S239_Conv2d_18x128x1x1_Hsigmoid(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 3232 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerActivation_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 1][Tile0 Dim: 1]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 512 [Tile0, 1:[512x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[512x1], 1]
		Tile0: [0, 512, 512], Tile1: [0, 512, 512], Tile2; [0, 512, 512]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 2304 [Tile1, 1:[128x18], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[128x18], 1]
		Tile0: [0, 2304, 2304], Tile1: [0, 2304, 2304], Tile2; [0, 2304, 2304]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 256 [Tile0, 1:[128x2], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[128x2], 1]
		Tile0: [0, 256, 256], Tile1: [0, 256, 256], Tile2; [0, 256, 256]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 72 [Tile1, 1:[1x18], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x18], 4]
		Tile0: [0, 72, 72], Tile1: [0, 72, 72], Tile2; [0, 72, 72]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 36 [Tile1, 1:[2x18], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[2x18], 1]
		Tile0: [0, 36, 36], Tile1: [0, 36, 36], Tile2; [0, 36, 36]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 18 [Tile1, 1:[1x18], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x18], 1]
		Tile0: [0, 18, 18], Tile1: [0, 18, 18], Tile2; [0, 18, 18]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 18 [Tile1, 1:[1x18], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x18], 1]
		Tile0: [0, 18, 18], Tile1: [0, 18, 18], Tile2; [0, 18, 18]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->W_In1 = (unsigned short int) (128);
	KerArg0->H_In1 = (unsigned short int) (18);
	KerArg0->In2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+2964);
	KerArg0->W_In2 = (unsigned short int) (2);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+2304);
	KerArg0->Scale = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+2412);
	KerArg0->ScaleN = (unsigned char * __restrict__) (SSD_tin_can_bottle_L1_Memory+2432);
	KerArg0->Out = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+2376);
	KerArg0->W_Out = (unsigned short int) (2);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+2452);
	KerArg0->ColFirst = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+3220);
	KerArg1->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+2376);
	KerArg1->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+2376);
	KerArg1->Feat = (unsigned short int) (1);
	KerArg1->W = (unsigned short int) (2);
	KerArg1->H = (unsigned short int) (18);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+3220);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) In1+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23684+0), 2304, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read In1 */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23684+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 2304, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+2964), 256, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+2304), 72, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+2412), 18, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+2432), 18, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+3220), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile1 */
		int T1Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->OutFirstCol = (unsigned short int) ((0)*2);
			KerArg0->NormBias = (unsigned char) (((char *)(SSD_tin_can_bottle_L1_Memory+3220))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulB32_2x4_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulB32_2x4_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*====================== Call Kernel LOC_LOOP_EPILOG =========================*/
		AT_FORK(gap_ncore(), (void *) Ker_HSigmoid_SQ8, (void *) KerArg1);
		__CALL(Ker_HSigmoid_SQ8, KerArg1);
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+2376), 36, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S240_Op_CONV_2D_0_89_trans_out0(
		signed char * __restrict__ In,
		signed char * __restrict__ Out)

{
	/* Shared L1: 72 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerMatTranspose_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 1, Tiled: 1][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 36 [D0, [0 x 36, 36]][Tile0, 1:[2x18], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 36, 36]][Tile0, 1:[2x18], 1]
		Tile0: [0, 36, 36], Tile1: [0, 36, 36], Tile2; [0, 36, 36]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 36 [D0, [0 x 36, 36]][Tile0, 1:[2x18], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 36, 36]][Tile0, 1:[2x18], 1]
		Tile0: [0, 36, 36], Tile1: [0, 36, 36], Tile2; [0, 36, 36]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+36);
	KerArg0->Feat = (unsigned short int) (1);
	KerArg0->W = (unsigned short int) (2);
	KerArg0->H = (unsigned short int) (18);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 36, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D0 */
		int D0Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			AT_FORK(gap_ncore(), (void *) CNN_ParTranspose_fps, (void *) KerArg0);
			__CALL(CNN_ParTranspose_fps, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+36), 36, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S244_Conv2d_24x64x1x1(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 1780 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerLinear_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 24, Tiled: 1][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile0, 1:[1x1], 64]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 64]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1536 [D0, [0 x 1536, 1536]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 1536, 1536]]
		Tile0: [0, 1536, 1536], Tile1: [0, 1536, 1536], Tile2; [0, 1536, 1536]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 96 [D0, [0 x 96, 96]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 96, 96]]
		Tile0: [0, 96, 96], Tile1: [0, 96, 96], Tile2; [0, 96, 96]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 24 [D0, [0 x 24, 24]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 24, 24]]
		Tile0: [0, 24, 24], Tile1: [0, 24, 24], Tile2; [0, 24, 24]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 24 [D0, [0 x 24, 24]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 24, 24]]
		Tile0: [0, 24, 24], Tile1: [0, 24, 24], Tile2; [0, 24, 24]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 24 [D0, [0 x 24, 24]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 24, 24]]
		Tile0: [0, 24, 24], Tile1: [0, 24, 24], Tile2; [0, 24, 24]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->Weights = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+64);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+1600);
	KerArg0->Out = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+1696);
	KerArg0->InDim = (unsigned short int) (64);
	KerArg0->TotalInDim = (unsigned short int) (64);
	KerArg0->OutDim = (unsigned short int) (24);
	KerArg0->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+1720);
	KerArg0->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+1744);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+1768);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 64, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Filter+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+29672+0), 1536, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Filter */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+29672+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+64), 1536, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Filter */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1600), 96, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1720), 24, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1744), 24, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1768), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D0 */
		int D0Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			AT_FORK(gap_ncore(), (void *) KerParLinearLayerFullFeatB32_SQ8, (void *) KerArg0);
			__CALL(KerParLinearLayerFullFeatB32_SQ8, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1696), 24, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S250_Conv2d_18x64x1x1_Hsigmoid(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 1360 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	KerLinear_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerActivation_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 18, Tiled: 1][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile0, 1:[1x1], 64]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 64]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1152 [D0, [0 x 1152, 1152]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 1152, 1152]]
		Tile0: [0, 1152, 1152], Tile1: [0, 1152, 1152], Tile2; [0, 1152, 1152]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 72 [D0, [0 x 72, 72]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 72, 72]]
		Tile0: [0, 72, 72], Tile1: [0, 72, 72], Tile2; [0, 72, 72]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 18 [D0, [0 x 18, 18]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 18, 18]]
		Tile0: [0, 18, 18], Tile1: [0, 18, 18], Tile2; [0, 18, 18]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 18 [D0, [0 x 18, 18]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 18, 18]]
		Tile0: [0, 18, 18], Tile1: [0, 18, 18], Tile2; [0, 18, 18]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 18 [D0, [0 x 18, 18]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 18, 18]]
		Tile0: [0, 18, 18], Tile1: [0, 18, 18], Tile2; [0, 18, 18]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg0->Weights = (signed char * __restrict__) (SSD_tin_can_bottle_L1_Memory+64);
	KerArg0->Bias = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+1216);
	KerArg0->Out = (void * __restrict__) (SSD_tin_can_bottle_L1_Memory+1288);
	KerArg0->InDim = (unsigned short int) (64);
	KerArg0->TotalInDim = (unsigned short int) (64);
	KerArg0->OutDim = (unsigned short int) (18);
	KerArg0->Scale = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+1308);
	KerArg0->ScaleN = (unsigned char *__restrict__) (SSD_tin_can_bottle_L1_Memory+1328);
	KerArg0->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+1348);
	KerArg1->In = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+1288);
	KerArg1->Out = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+1288);
	KerArg1->Feat = (unsigned short int) (18);
	KerArg1->W = (unsigned short int) (1);
	KerArg1->H = (unsigned short int) (1);
	KerArg1->Infos = (signed char *__restrict__) (SSD_tin_can_bottle_L1_Memory+1348);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 64, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Filter+0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23456+0), 1152, 0, &UchanHF1);
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1); /* Wait previous uDMA read Filter */
	AT_L2_COPY(0, ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory+23456+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+64), 1152, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Filter */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1216), 72, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1308), 18, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1328), 18, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1348), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D0 */
		int D0Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			AT_FORK(gap_ncore(), (void *) KerParLinearLayerFullFeatB32_SQ8, (void *) KerArg0);
			__CALL(KerParLinearLayerFullFeatB32_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*====================== Call Kernel LOC_LOOP_EPILOG =========================*/
		AT_FORK(gap_ncore(), (void *) Ker_HSigmoid_SQ8, (void *) KerArg1);
		__CALL(Ker_HSigmoid_SQ8, KerArg1);
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+1288), 18, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
void S254_Op_CUSTOM_0_99(
		int8_t * __restrict__ boxes_in,
		int8_t * __restrict__ classes_in,
		int8_t * __restrict__ anchors_in,
		int16_t * bbox_out,
		int8_t * class_out,
		int8_t * scores_out,
		uint8_t *  in_scales,
		uint8_t *  in_norms)

{
	/* Shared L1: 20820 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaW_Evt1;
	AT_L2_EVENT DmaW_Evt2;
	AT_L2_EVENT DmaW_Evt3;
	Ker_SSD_Init_ArgT S_KerArg0, *KerArg0 = &S_KerArg0;
	Ker_SSD_Decoder_ArgT S_KerArg1, *KerArg1 = &S_KerArg1;
	Ker_SSD_NMS_ArgT S_KerArg2, *KerArg2 = &S_KerArg2;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 1][Tile0 Dim: 1]
	Ker Arg: boxes_in, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6216 [Tile0, 1:[4x1554], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[4x1554], 1]
		Tile0: [0, 6216, 6216], Tile1: [0, 6216, 6216], Tile2; [0, 6216, 6216]
	Ker Arg: classes_in, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 4662 [Tile0, 1:[3x1554], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[3x1554], 1]
		Tile0: [0, 4662, 4662], Tile1: [0, 4662, 4662], Tile2; [0, 4662, 4662]
	Ker Arg: anchors_in, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6216 [Tile0, 1:[4x1554], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[4x1554], 1]
		Tile0: [0, 6216, 6216], Tile1: [0, 6216, 6216], Tile2; [0, 6216, 6216]
	Ker Arg: bbox_buf, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 3600 [D0, [0 x 3600, 3600]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 3600, 3600]]
		Tile0: [0, 3600, 3600], Tile1: [0, 3600, 3600], Tile2; [0, 3600, 3600]
	Ker Arg: bbox_out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 80 [D0, [0 x 80, 80]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 80, 80]]
		Tile0: [0, 80, 80], Tile1: [0, 80, 80], Tile2; [0, 80, 80]
	Ker Arg: scores_out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 10 [D0, [0 x 10, 10]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 10, 10]]
		Tile0: [0, 10, 10], Tile1: [0, 10, 10], Tile2; [0, 10, 10]
	Ker Arg: class_out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 10 [D0, [0 x 10, 10]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 10, 10]]
		Tile0: [0, 10, 10], Tile1: [0, 10, 10], Tile2; [0, 10, 10]
	Ker Arg: bbox_idx, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 2 [D0, [0 x 2, 2]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 2, 2]]
		Tile0: [0, 2, 2], Tile1: [0, 2, 2], Tile2; [0, 2, 2]
	Ker Arg: in_scales, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 8 [D0, [0 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 8, 8]]
		Tile0: [0, 8, 8], Tile1: [0, 8, 8], Tile2; [0, 8, 8]
	Ker Arg: in_norms, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 8 [D0, [0 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 8, 8]]
		Tile0: [0, 8, 8], Tile1: [0, 8, 8], Tile2; [0, 8, 8]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->n_max_bb = (int16_t ) (300);
	KerArg1->boxes_in = (int8_t * __restrict__) (SSD_tin_can_bottle_L1_Memory+0);
	KerArg1->classes_in = (int8_t * __restrict__) (SSD_tin_can_bottle_L1_Memory+6216);
	KerArg1->anchors_in = (int8_t * __restrict__) (SSD_tin_can_bottle_L1_Memory+10880);
	KerArg1->Box_W = (int16_t ) (4);
	KerArg1->Class_W = (int16_t ) (3);
	KerArg1->H = (int16_t ) (1554);
	KerArg1->ScoreThr = (int) (0);
	KerArg1->n_max_bb = (int16_t ) (300);
	KerArg2->n_out_box = (int16_t) (10);
	KerArg2->NMSThr = (int) (63);
	KerArg2->n_max_bb = (int16_t ) (300);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) boxes_in+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+0), 6216, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read boxes_in */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) classes_in+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+6216), 4662, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read classes_in */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) anchors_in+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+10880), 6216, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read anchors_in */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) in_scales+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+20804), 8, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read in_scales */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) in_norms+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+20812), 8, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read in_norms */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D0 */
		int D0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP_PROLOG =========================*/
		KerArg0->bbox_idx = (int16_t *) (SSD_tin_can_bottle_L1_Memory+20800);
		KerArg0->bbox_buf = (bbox_t *) (SSD_tin_can_bottle_L1_Memory+17096);
		Ker_SSD_Init(KerArg0);
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg1->bbox_buf = (bbox_t *) (SSD_tin_can_bottle_L1_Memory+17096);
			KerArg1->in_scales = (uint8_t *) (SSD_tin_can_bottle_L1_Memory+20804);
			KerArg1->in_norms = (uint8_t *) (SSD_tin_can_bottle_L1_Memory+20812);
			KerArg1->bbox_idx = (int16_t *) (SSD_tin_can_bottle_L1_Memory+20800);
			AT_FORK(gap_ncore(), (void *) Ker_SSD_Decoder, (void *) KerArg1);
			__CALL(Ker_SSD_Decoder, KerArg1);
		} /* End iteration on Tile0 */
		/*====================== Call Kernel LOC_LOOP_EPILOG =========================*/
		KerArg2->bbox_buf = (bbox_t *) (SSD_tin_can_bottle_L1_Memory+17096);
		KerArg2->bbox_out = (int16_t *) (SSD_tin_can_bottle_L1_Memory+20696);
		KerArg2->scores_out = (int8_t *) (SSD_tin_can_bottle_L1_Memory+20776);
		KerArg2->class_out = (int8_t *) (SSD_tin_can_bottle_L1_Memory+20788);
		KerArg2->bbox_idx = (int16_t *) (SSD_tin_can_bottle_L1_Memory+20800);
		Ker_SSD_NMS(KerArg2);
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) bbox_out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+20696), 80, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write bbox_out */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) scores_out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+20776), 10, 1, &DmaW_Evt2);
	AT_L2_WAIT(0, &DmaW_Evt2); /* Wait DMA write scores_out */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) class_out+0), ((AT_L2_INT_ADDR_TYPE) SSD_tin_can_bottle_L1_Memory+20788), 10, 1, &DmaW_Evt3);
	AT_L2_WAIT(0, &DmaW_Evt3); /* Wait DMA write class_out */
	/*============================ End Write Tiles Epilog ===============================*/
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
int SSD_tin_can_bottleCNN_Construct()

{
	AT_HYPERFLASH_FS_FC_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CONF_T HyperFlashConf;
	int Error;
	AT_HYPERFLASH_FS_CONF_INIT(&HyperFlashConf, AT_MEM_L3_HFLASH, 0);
	AT_HYPERFLASH_FS_OPEN(&HyperFlash, &HyperFlashConf, "SSD_tin_can_bottle_L3_Flash_Const.dat", &Error);
	if (Error) return 1;
	SSD_tin_can_bottle_L3_Memory = (AT_HYPERRAM_POINTER) AT_HYPERRAM_ALLOC(&HyperRam, 2304000);
	if (SSD_tin_can_bottle_L3_Memory == 0) return 2;
	SSD_tin_can_bottle_L2_Memory = (AT_L2_POINTER) AT_L2_ALLOC(0, 220000);
	if (SSD_tin_can_bottle_L2_Memory == 0) return 3;
	SSD_tin_can_bottle_L1_Memory = (AT_L1_POINTER) AT_L1_ALLOC(0, 52716);
	if (SSD_tin_can_bottle_L1_Memory == 0) return 4;
	/* Moving Featureextractormobilenetv2con, size 64 from HyperFlash at 1248336 to (size 64) L2 at 2704..2767 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1248336), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 2704), 64, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S3_Mul_scale, size 16 from HyperFlash at 1250364 to (size 16) L2 at 3024..3039 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250364), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3024), 16, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S3_Mul_shift, size 16 from HyperFlash at 1250380 to (size 16) L2 at 3040..3055 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250380), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3040), 16, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S3_Infos, size 9 from HyperFlash at 1250620 to (size 9) L2 at 3184..3192 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250620), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3184), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_64df44de, size 144 from HyperFlash at 1243160 to (size 144) L2 at 1792..1935 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1243160), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 1792), 144, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_8ca654a8, size 64 from HyperFlash at 1248400 to (size 64) L2 at 2768..2831 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1248400), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 2768), 64, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S6_Mul_scale, size 16 from HyperFlash at 1250396 to (size 16) L2 at 3056..3071 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250396), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3056), 16, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S6_Mul_shift, size 16 from HyperFlash at 1250412 to (size 16) L2 at 3072..3087 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250412), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3072), 16, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S6_Infos, size 9 from HyperFlash at 1250632 to (size 9) L2 at 3196..3204 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250632), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3196), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_885a3bd0, size 256 from HyperFlash at 1237656 to (size 256) L2 at 1536..1791 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1237656), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 1536), 256, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_7e38b462, size 64 from HyperFlash at 1248464 to (size 64) L2 at 2832..2895 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1248464), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 2832), 64, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S9_Mul_scale, size 16 from HyperFlash at 1250428 to (size 16) L2 at 3088..3103 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250428), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3088), 16, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S9_Mul_shift, size 16 from HyperFlash at 1250444 to (size 16) L2 at 3104..3119 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250444), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3104), 16, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S9_Infos, size 9 from HyperFlash at 1250644 to (size 9) L2 at 3208..3216 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250644), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3208), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S10_Infos, size 9 from HyperFlash at 1250656 to (size 9) L2 at 3220..3228 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250656), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3220), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_ab63e9b9, size 384 from HyperFlash at 1228632 to (size 384) L2 at 0..383 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1228632), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 0), 384, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S13_Mul_scale, size 96 from HyperFlash at 1244712 to (size 96) L2 at 1936..2031 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1244712), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 1936), 96, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S13_Mul_shift, size 96 from HyperFlash at 1244808 to (size 96) L2 at 2032..2127 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1244808), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 2032), 96, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S13_Infos, size 9 from HyperFlash at 1250668 to (size 9) L2 at 3232..3240 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250668), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3232), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_6a98a40c, size 384 from HyperFlash at 1229016 to (size 384) L2 at 384..767 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1229016), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 384), 384, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S16_Mul_scale, size 96 from HyperFlash at 1244904 to (size 96) L2 at 2128..2223 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1244904), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 2128), 96, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S16_Mul_shift, size 96 from HyperFlash at 1245000 to (size 96) L2 at 2224..2319 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1245000), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 2224), 96, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S16_Infos, size 9 from HyperFlash at 1250680 to (size 9) L2 at 3244..3252 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250680), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3244), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_798c9299, size 64 from HyperFlash at 1248528 to (size 64) L2 at 2896..2959 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1248528), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 2896), 64, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S19_Mul_scale, size 16 from HyperFlash at 1250460 to (size 16) L2 at 3120..3135 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250460), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3120), 16, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S19_Mul_shift, size 16 from HyperFlash at 1250476 to (size 16) L2 at 3136..3151 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250476), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3136), 16, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S19_Infos, size 9 from HyperFlash at 1250692 to (size 9) L2 at 3256..3264 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250692), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3256), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_9bde526a, size 384 from HyperFlash at 1229400 to (size 384) L2 at 768..1151 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1229400), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 768), 384, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S22_Mul_scale, size 96 from HyperFlash at 1245096 to (size 96) L2 at 2320..2415 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1245096), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 2320), 96, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S22_Mul_shift, size 96 from HyperFlash at 1245192 to (size 96) L2 at 2416..2511 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1245192), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 2416), 96, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S22_Infos, size 9 from HyperFlash at 1250704 to (size 9) L2 at 3268..3276 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250704), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3268), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_91d5cf3c, size 384 from HyperFlash at 1229784 to (size 384) L2 at 1152..1535 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1229784), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 1152), 384, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S25_Mul_scale, size 96 from HyperFlash at 1245288 to (size 96) L2 at 2512..2607 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1245288), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 2512), 96, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S25_Mul_shift, size 96 from HyperFlash at 1245384 to (size 96) L2 at 2608..2703 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1245384), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 2608), 96, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S25_Infos, size 9 from HyperFlash at 1250716 to (size 9) L2 at 3280..3288 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250716), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3280), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_11dac546, size 64 from HyperFlash at 1248592 to (size 64) L2 at 2960..3023 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1248592), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 2960), 64, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S28_Mul_scale, size 16 from HyperFlash at 1250492 to (size 16) L2 at 3152..3167 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250492), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3152), 16, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S28_Mul_shift, size 16 from HyperFlash at 1250508 to (size 16) L2 at 3168..3183 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250508), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3168), 16, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S28_Infos, size 9 from HyperFlash at 1250728 to (size 9) L2 at 3292..3300 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250728), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3292), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S70_Mul_scale, size 192 from HyperFlash at 1239192 to (size 192) L2 at 23144..23335 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1239192), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 23144), 192, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S106_Mul_scale, size 48 from HyperFlash at 1249232 to (size 48) L2 at 23336..23383 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249232), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 23336), 48, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving Featureextractormobilenetv2lay_0a98f919, size 16384 from HyperFlash at 918528 to (size 16384) L2 at 3304..19687 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 918528), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 3304), 16384, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving Boxpredictor_0boxencodingpredi_696b62c5, size 3456 from HyperFlash at 1131496 to (size 3456) L2 at 19688..23143 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1131496), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 19688), 3456, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S254_Ssd_scales, size 8 from HyperFlash at 1251664 to (size 8) L2 at 23384..23391 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251664), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 23384), 8, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	return 0;
}
#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC optimize ("-Os")
int SSD_tin_can_bottleCNN_Destruct()

{
	AT_HYPERRAM_FREE(&HyperRam, SSD_tin_can_bottle_L3_Memory, 2304000);
	AT_L2_FREE(0, SSD_tin_can_bottle_L2_Memory, 220000);
	AT_L1_FREE(0, SSD_tin_can_bottle_L1_Memory, 52716);
	AT_HYPERFLASH_FS_CLOSE(&HyperFlash);
	return 0;
}
#pragma GCC pop_options
unsigned int SSD_Monitor[94];
unsigned int SSD_Op[94] = {
	8601600,
	3072000,
	4915200,
	307200,
	29491200,
	4608000,
	7372800,
	7372800,
	4608000,
	7372800,
	76800,
	7372800,
	1152000,
	1843200,
	1843200,
	1152000,
	1843200,
	19200,
	1843200,
	1152000,
	1843200,
	19200,
	1843200,
	288000,
	921600,
	1843200,
	576000,
	1843200,
	9600,
	1843200,
	576000,
	1843200,
	9600,
	1843200,
	576000,
	1843200,
	9600,
	1843200,
	576000,
	2764800,
	4147200,
	864000,
	4147200,
	14400,
	4147200,
	864000,
	4147200,
	14400,
	4147200,
	777600,
	2700,
	1036800,
	3600,
	230400,
	1843200,
	3072000,
	384000,
	3072000,
	6400,
	3072000,
	384000,
	3072000,
	6400,
	3072000,
	384000,
	6144000,
	8192000,
	921600,
	1440,
	1228800,
	1920,
	6553600,
	5903360,
	92160,
	360,
	122880,
	480,
	327680,
	443136,
	13824,
	108,
	18432,
	144,
	49152,
	147712,
	4608,
	36,
	6144,
	48,
	8192,
	6208,
	1170,
	1536,
	92400,
};
char *SSD_Nodes[94] = {
	"S3_Conv2d_16x3x3x3_Relu6",
	"S6_Conv2d_16x1x3x3_Relu6",
	"S9_Conv2d_16x16x1x1",
	"S10_MatAdd_16x120x160",
	"S13_Conv2d_96x16x1x1_Relu6",
	"S16_Conv2d_96x1x3x3_Relu6",
	"S19_Conv2d_16x96x1x1",
	"S22_Conv2d_96x16x1x1_Relu6",
	"S25_Conv2d_96x1x3x3_Relu6",
	"S28_Conv2d_16x96x1x1",
	"S29_MatAdd_16x60x80",
	"S32_Conv2d_96x16x1x1_Relu6",
	"S35_Conv2d_96x1x3x3_Relu6",
	"S38_Conv2d_16x96x1x1",
	"S41_Conv2d_96x16x1x1_Relu6",
	"S44_Conv2d_96x1x3x3_Relu6",
	"S47_Conv2d_16x96x1x1",
	"S48_MatAdd_16x30x40",
	"S51_Conv2d_96x16x1x1_Relu6",
	"S54_Conv2d_96x1x3x3_Relu6",
	"S57_Conv2d_16x96x1x1",
	"S58_MatAdd_16x30x40",
	"S61_Conv2d_96x16x1x1_Relu6",
	"S64_Conv2d_96x1x3x3_Relu6",
	"S67_Conv2d_32x96x1x1",
	"S70_Conv2d_192x32x1x1_Relu6",
	"S73_Conv2d_192x1x3x3_Relu6",
	"S76_Conv2d_32x192x1x1",
	"S77_MatAdd_32x15x20",
	"S80_Conv2d_192x32x1x1_Relu6",
	"S83_Conv2d_192x1x3x3_Relu6",
	"S86_Conv2d_32x192x1x1",
	"S87_MatAdd_32x15x20",
	"S90_Conv2d_192x32x1x1_Relu6",
	"S93_Conv2d_192x1x3x3_Relu6",
	"S96_Conv2d_32x192x1x1",
	"S97_MatAdd_32x15x20",
	"S100_Conv2d_192x32x1x1_Relu6",
	"S103_Conv2d_192x1x3x3_Relu6",
	"S106_Conv2d_48x192x1x1",
	"S109_Conv2d_288x48x1x1_Relu6",
	"S112_Conv2d_288x1x3x3_Relu6",
	"S115_Conv2d_48x288x1x1",
	"S116_MatAdd_48x15x20",
	"S119_Conv2d_288x48x1x1_Relu6",
	"S122_Conv2d_288x1x3x3_Relu6",
	"S125_Conv2d_48x288x1x1",
	"S126_MatAdd_48x15x20",
	"S129_Conv2d_288x48x1x1_Relu6",
	"S199_Conv2d_9x288x1x1_Hsigmoid",
	"S200_Op_CONV_2D_0_73_trans_out0",
	"S194_Conv2d_12x288x1x1",
	"S195_Op_CONV_2D_0_71_trans_out0",
	"S132_Conv2d_288x1x3x3_Relu6",
	"S135_Conv2d_80x288x1x1",
	"S138_Conv2d_480x80x1x1_Relu6",
	"S141_Conv2d_480x1x3x3_Relu6",
	"S144_Conv2d_80x480x1x1",
	"S145_MatAdd_80x8x10",
	"S148_Conv2d_480x80x1x1_Relu6",
	"S151_Conv2d_480x1x3x3_Relu6",
	"S154_Conv2d_80x480x1x1",
	"S155_MatAdd_80x8x10",
	"S158_Conv2d_480x80x1x1_Relu6",
	"S161_Conv2d_480x1x3x3_Relu6",
	"S164_Conv2d_160x480x1x1",
	"S167_Conv2d_640x160x1x1_Relu6",
	"S209_Conv2d_18x640x1x1_Hsigmoid",
	"S210_Op_CONV_2D_0_77_trans_out0",
	"S204_Conv2d_24x640x1x1",
	"S205_Op_CONV_2D_0_75_trans_out0",
	"S170_Conv2d_128x640x1x1_Relu6",
	"S173_Conv2d_256x128x3x3_Relu6",
	"S219_Conv2d_18x256x1x1_Hsigmoid",
	"S220_Op_CONV_2D_0_81_trans_out0",
	"S214_Conv2d_24x256x1x1",
	"S215_Op_CONV_2D_0_79_trans_out0",
	"S176_Conv2d_64x256x1x1_Relu6",
	"S179_Conv2d_128x64x3x3_Relu6",
	"S229_Conv2d_18x128x1x1_Hsigmoid",
	"S230_Op_CONV_2D_0_85_trans_out0",
	"S224_Conv2d_24x128x1x1",
	"S225_Op_CONV_2D_0_83_trans_out0",
	"S182_Conv2d_64x128x1x1_Relu6",
	"S185_Conv2d_128x64x3x3_Relu6",
	"S239_Conv2d_18x128x1x1_Hsigmoid",
	"S240_Op_CONV_2D_0_89_trans_out0",
	"S234_Conv2d_24x128x1x1",
	"S235_Op_CONV_2D_0_87_trans_out0",
	"S188_Conv2d_32x128x1x1_Relu6",
	"S191_Conv2d_64x32x1x3_Relu6",
	"S250_Conv2d_18x64x1x1_Hsigmoid",
	"S244_Conv2d_24x64x1x1",
	"S254_Op_CUSTOM_0_99",
};
#pragma GCC push_options
#pragma GCC optimize ("-Os")
int SSD_tin_can_bottleCNN(
		signed char * __restrict__ Input_1,
		signed short * __restrict__ Output_1,
		signed char * __restrict__ Output_2,
		signed char * __restrict__ Output_3)

{
	AT_HYPERFLASH_FS_CL_EVENT UchanHF0;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF1;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF2;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF3;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF4;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF5;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF6;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF7;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF8;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF9;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF10;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF11;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF12;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF13;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF14;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF15;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF16;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF17;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF18;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF19;
	AT_HYPERFLASH_FS_CL_EVENT UchanHF20;
	/* Moving Featureextractormobilenetv2exp_8539b665, size 1536 from HyperFlash at 1180136 to (size 1536) L2 at 76096 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1180136), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 76096), 1536, 0, &UchanHF0);
	SSD_Monitor[0] = gap_cl_readhwtimer();
	S3_Conv2d_16x3x3x3_Relu6(
		((signed char * __restrict__) Input_1), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1228200)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+2704)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+614400)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3024)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3040)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3184)) /* Infos */
	);
	SSD_Monitor[0] = gap_cl_readhwtimer() - SSD_Monitor[0];
	SSD_Monitor[1] = gap_cl_readhwtimer();
	S6_Conv2d_16x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+614400)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+1792)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+2768)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3056)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3072)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3196)) /* Infos */
	);
	SSD_Monitor[1] = gap_cl_readhwtimer() - SSD_Monitor[1];
	SSD_Monitor[2] = gap_cl_readhwtimer();
	S9_Conv2d_16x16x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+1536)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+2832)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+307200)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3088)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3104)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3208)) /* Infos */
	);
	SSD_Monitor[2] = gap_cl_readhwtimer() - SSD_Monitor[2];
	SSD_Monitor[3] = gap_cl_readhwtimer();
	S10_MatAdd_16x120x160(
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+307200)), /* In1 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+614400)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* Out */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3220)) /* Infos */
	);
	SSD_Monitor[3] = gap_cl_readhwtimer() - SSD_Monitor[3];
	/* Moving Featureextractormobilenetv2exp_a2b059aa, size 864 from HyperFlash at 1207656 to (size 864) L2 at 71776 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1207656), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 71776), 864, 0, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_b306895a, size 1536 from HyperFlash at 1184744 to (size 1536) L2 at 218464 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1184744), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 218464), 1536, 0, &UchanHF2);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_8539b665 using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	SSD_Monitor[4] = gap_cl_readhwtimer();
	S13_Conv2d_96x16x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+76096)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+0)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+460800)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+1936)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+2032)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3232)) /* Infos */
	);
	SSD_Monitor[4] = gap_cl_readhwtimer() - SSD_Monitor[4];
	/* Moving Featureextractormobilenetv2exp_3263cbf4, size 1536 from HyperFlash at 1181672 to (size 1536) L2 at 64864 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1181672), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 64864), 1536, 0, &UchanHF0);
	/* Moving Featureextractormobilenetv2exp_c154601d, size 1536 from HyperFlash at 1183208 to (size 1536) L2 at 66400 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1183208), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 66400), 1536, 0, &UchanHF3);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_a2b059aa using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	SSD_Monitor[5] = gap_cl_readhwtimer();
	S16_Conv2d_96x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+460800)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+71776)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+384)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+2128)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+2224)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3244)) /* Infos */
	);
	SSD_Monitor[5] = gap_cl_readhwtimer() - SSD_Monitor[5];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_3263cbf4 using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	SSD_Monitor[6] = gap_cl_readhwtimer();
	S19_Conv2d_16x96x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+64864)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+2896)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+100192)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3120)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3136)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3256)) /* Infos */
	);
	SSD_Monitor[6] = gap_cl_readhwtimer() - SSD_Monitor[6];
	/* Moving Featureextractormobilenetv2exp_cbbad378, size 864 from HyperFlash at 1208520 to (size 864) L2 at 64864 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1208520), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 64864), 864, 0, &UchanHF0);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_c154601d using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	SSD_Monitor[7] = gap_cl_readhwtimer();
	S22_Conv2d_96x16x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+100192)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+66400)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+768)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+2320)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+2416)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3268)) /* Infos */
	);
	SSD_Monitor[7] = gap_cl_readhwtimer() - SSD_Monitor[7];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_cbbad378 using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	SSD_Monitor[8] = gap_cl_readhwtimer();
	S25_Conv2d_96x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+64864)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+1152)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+460800)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+2512)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+2608)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3280)) /* Infos */
	);
	SSD_Monitor[8] = gap_cl_readhwtimer() - SSD_Monitor[8];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_b306895a using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	SSD_Monitor[9] = gap_cl_readhwtimer();
	S28_Conv2d_16x96x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+460800)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+218464)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+2960)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3152)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3168)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3292)) /* Infos */
	);
	SSD_Monitor[9] = gap_cl_readhwtimer() - SSD_Monitor[9];
	/* Moving Featureextractormobilenetv2exp_1fe80eb6, size 1536 from HyperFlash at 1186280 to (size 1536) L2 at 194560 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1186280), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 194560), 1536, 0, &UchanHF0);
	SSD_Monitor[10] = gap_cl_readhwtimer();
	S29_MatAdd_16x60x80(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+100192)), /* In1 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* Out */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250740)) /* Infos */
	);
	SSD_Monitor[10] = gap_cl_readhwtimer() - SSD_Monitor[10];
	/* Moving Featureextractormobilenetv2exp_6aa4b1e3, size 864 from HyperFlash at 1209384 to (size 864) L2 at 169568 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1209384), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 169568), 864, 0, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_2a83c320, size 1536 from HyperFlash at 1187816 to (size 1536) L2 at 168032 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1187816), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 168032), 1536, 0, &UchanHF2);
	/* Moving Featureextractormobilenetv2exp_ac66cd91, size 864 from HyperFlash at 1210248 to (size 864) L2 at 171488 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1210248), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 171488), 864, 0, &UchanHF3);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_1fe80eb6 using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	SSD_Monitor[11] = gap_cl_readhwtimer();
	S32_Conv2d_96x16x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+194560)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1230168)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+76800)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1245480)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1245576)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250752)) /* Infos */
	);
	SSD_Monitor[11] = gap_cl_readhwtimer() - SSD_Monitor[11];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_6aa4b1e3 using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	SSD_Monitor[12] = gap_cl_readhwtimer();
	S35_Conv2d_96x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+76800)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+169568)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1230552)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+52832)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1245672)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1245768)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250764)) /* Infos */
	);
	SSD_Monitor[12] = gap_cl_readhwtimer() - SSD_Monitor[12];
	/* Moving Featureextractormobilenetv2exp_e87a7605, size 1536 from HyperFlash at 1189352 to (size 1536) L2 at 169568 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1189352), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 169568), 1536, 0, &UchanHF0);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_2a83c320 using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	SSD_Monitor[13] = gap_cl_readhwtimer();
	S38_Conv2d_16x96x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+52832)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+168032)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1248656)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250524)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250540)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250776)) /* Infos */
	);
	SSD_Monitor[13] = gap_cl_readhwtimer() - SSD_Monitor[13];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_e87a7605 using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	SSD_Monitor[14] = gap_cl_readhwtimer();
	S41_Conv2d_96x16x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+169568)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1230936)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+42592)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1245864)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1245960)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250788)) /* Infos */
	);
	SSD_Monitor[14] = gap_cl_readhwtimer() - SSD_Monitor[14];
	/* Moving Featureextractormobilenetv2exp_042cd376, size 1536 from HyperFlash at 1190888 to (size 1536) L2 at 169952 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1190888), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 169952), 1536, 0, &UchanHF0);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_ac66cd91 using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	SSD_Monitor[15] = gap_cl_readhwtimer();
	S44_Conv2d_96x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+42592)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+171488)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1231320)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1246056)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1246152)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250800)) /* Infos */
	);
	SSD_Monitor[15] = gap_cl_readhwtimer() - SSD_Monitor[15];
	/* Moving Featureextractormobilenetv2exp_b7f84a57, size 1536 from HyperFlash at 1192424 to (size 1536) L2 at 138592 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1192424), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 138592), 1536, 0, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_a26d0376, size 864 from HyperFlash at 1211112 to (size 864) L2 at 171488 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1211112), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 171488), 864, 0, &UchanHF2);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_042cd376 using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	SSD_Monitor[16] = gap_cl_readhwtimer();
	S47_Conv2d_16x96x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+169952)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1248720)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+42592)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250556)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250572)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250812)) /* Infos */
	);
	SSD_Monitor[16] = gap_cl_readhwtimer() - SSD_Monitor[16];
	/* Moving Featureextractormobilenetv2exp_14c9cb1f, size 1536 from HyperFlash at 1193960 to (size 1536) L2 at 169952 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1193960), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 169952), 1536, 0, &UchanHF0);
	SSD_Monitor[17] = gap_cl_readhwtimer();
	S48_MatAdd_16x30x40(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In1 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+42592)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+150752)), /* Out */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250824)) /* Infos */
	);
	SSD_Monitor[17] = gap_cl_readhwtimer() - SSD_Monitor[17];
	/* Moving Featureextractormobilenetv2exp_98528ead, size 864 from HyperFlash at 1211976 to (size 864) L2 at 189152 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1211976), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 189152), 864, 0, &UchanHF3);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_b7f84a57 using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	SSD_Monitor[18] = gap_cl_readhwtimer();
	S51_Conv2d_96x16x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+150752)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+138592)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1231704)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1246248)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1246344)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250836)) /* Infos */
	);
	SSD_Monitor[18] = gap_cl_readhwtimer() - SSD_Monitor[18];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_a26d0376 using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	SSD_Monitor[19] = gap_cl_readhwtimer();
	S54_Conv2d_96x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+171488)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1232088)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1246440)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1246536)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250848)) /* Infos */
	);
	SSD_Monitor[19] = gap_cl_readhwtimer() - SSD_Monitor[19];
	/* Moving S58_Infos, size 9 from HyperFlash at 1250872 to (size 9) L2 at 84172 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250872), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 84172), 9, 0, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_03c5956b, size 1536 from HyperFlash at 1195496 to (size 1536) L2 at 138592 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1195496), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 138592), 1536, 0, &UchanHF2);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_14c9cb1f using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	SSD_Monitor[20] = gap_cl_readhwtimer();
	S57_Conv2d_16x96x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+169952)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1248784)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+64864)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250588)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250604)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250860)) /* Infos */
	);
	SSD_Monitor[20] = gap_cl_readhwtimer() - SSD_Monitor[20];
	/* Waiting completion of transfer of S58_Infos using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	SSD_Monitor[21] = gap_cl_readhwtimer();
	S58_MatAdd_16x30x40(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+150752)), /* In1 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+64864)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+169952)), /* Out */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+84172)) /* Infos */
	);
	SSD_Monitor[21] = gap_cl_readhwtimer() - SSD_Monitor[21];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_03c5956b using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	SSD_Monitor[22] = gap_cl_readhwtimer();
	S61_Conv2d_96x16x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+169952)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+138592)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1232472)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1246632)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1246728)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250884)) /* Infos */
	);
	SSD_Monitor[22] = gap_cl_readhwtimer() - SSD_Monitor[22];
	/* Moving Featureextractormobilenetv2exp_9ff86288, size 3072 from HyperFlash at 1134952 to (size 3072) L2 at 167392 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1134952), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 167392), 3072, 0, &UchanHF0);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_98528ead using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	SSD_Monitor[23] = gap_cl_readhwtimer();
	S64_Conv2d_96x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+189152)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1232856)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+138592)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1246824)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1246920)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250896)) /* Infos */
	);
	SSD_Monitor[23] = gap_cl_readhwtimer() - SSD_Monitor[23];
	/* Moving Featureextractormobilenetv2exp_f1c78ba1, size 6144 from HyperFlash at 1054536 to (size 6144) L2 at 90592 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1054536), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 90592), 6144, 0, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_799802b5, size 768 from HyperFlash at 1212840 to (size 768) L2 at 96736 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1212840), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 96736), 768, 0, &UchanHF2);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_9ff86288 using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	SSD_Monitor[24] = gap_cl_readhwtimer();
	S67_Conv2d_32x96x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+138592)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+167392)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1243304)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1249604)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1249636)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250908)) /* Infos */
	);
	SSD_Monitor[24] = gap_cl_readhwtimer() - SSD_Monitor[24];
	/* Moving Featureextractormobilenetv2exp_3f45aca9, size 1728 from HyperFlash at 1173224 to (size 1728) L2 at 154336 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1173224), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 154336), 1728, 0, &UchanHF0);
	/* Moving Featureextractormobilenetv2exp_93afe2ba, size 768 from HyperFlash at 1213608 to (size 768) L2 at 156064 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1213608), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 156064), 768, 0, &UchanHF3);
	/* Moving S73_Mul_scale, size 192 from HyperFlash at 1239576 to (size 192) L2 at 156832 using event 4 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1239576), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 156832), 192, 0, &UchanHF4);
	/* Moving S73_Mul_shift, size 192 from HyperFlash at 1239768 to (size 192) L2 at 157024 using event 5 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1239768), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 157024), 192, 0, &UchanHF5);
	/* Moving S73_Infos, size 9 from HyperFlash at 1250932 to (size 9) L2 at 157408 using event 6 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250932), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 157408), 9, 0, &UchanHF6);
	/* Moving Featureextractormobilenetv2exp_9046602e, size 6144 from HyperFlash at 1060680 to (size 6144) L2 at 148192 using event 7 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1060680), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 148192), 6144, 0, &UchanHF7);
	/* Moving Featureextractormobilenetv2exp_712985b5, size 128 from HyperFlash at 1243432 to (size 128) L2 at 157216 using event 8 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1243432), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 157216), 128, 0, &UchanHF8);
	/* Moving S76_Mul_scale, size 32 from HyperFlash at 1249668 to (size 32) L2 at 157344 using event 9 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249668), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 157344), 32, 0, &UchanHF9);
	/* Moving S76_Mul_shift, size 32 from HyperFlash at 1249700 to (size 32) L2 at 157376 using event 10 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249700), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 157376), 32, 0, &UchanHF10);
	/* Moving S76_Infos, size 9 from HyperFlash at 1250944 to (size 9) L2 at 157420 using event 11 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250944), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 157420), 9, 0, &UchanHF11);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_f1c78ba1 using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_799802b5 using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	SSD_Monitor[25] = gap_cl_readhwtimer();
	S70_Conv2d_192x32x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+90592)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+96736)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+32992)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23144)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1239384)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1250920)) /* Infos */
	);
	SSD_Monitor[25] = gap_cl_readhwtimer() - SSD_Monitor[25];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_3f45aca9 using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_93afe2ba using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	/* Waiting completion of transfer of S73_Mul_scale using event 4 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4);
	/* Waiting completion of transfer of S73_Mul_shift using event 5 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5);
	/* Waiting completion of transfer of S73_Infos using event 6 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF6);
	SSD_Monitor[26] = gap_cl_readhwtimer();
	S73_Conv2d_192x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+32992)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+154336)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+156064)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+90592)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+156832)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+157024)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+157408)) /* Infos */
	);
	SSD_Monitor[26] = gap_cl_readhwtimer() - SSD_Monitor[26];
	/* Moving S77_Infos, size 9 from HyperFlash at 1250956 to (size 9) L2 at 42592 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250956), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 42592), 9, 0, &UchanHF0);
	/* Moving Featureextractormobilenetv2exp_aa5a1409, size 6144 from HyperFlash at 1066824 to (size 6144) L2 at 80992 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1066824), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 80992), 6144, 0, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_c8bd61b2, size 768 from HyperFlash at 1214376 to (size 768) L2 at 87136 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1214376), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 87136), 768, 0, &UchanHF2);
	/* Moving S80_Mul_scale, size 192 from HyperFlash at 1239960 to (size 192) L2 at 87904 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1239960), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 87904), 192, 0, &UchanHF3);
	/* Moving S80_Mul_shift, size 192 from HyperFlash at 1240152 to (size 192) L2 at 88096 using event 4 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1240152), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 88096), 192, 0, &UchanHF4);
	/* Moving S80_Infos, size 9 from HyperFlash at 1250968 to (size 9) L2 at 88288 using event 5 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250968), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 88288), 9, 0, &UchanHF5);
	/* Moving Featureextractormobilenetv2exp_504b4626, size 1728 from HyperFlash at 1174952 to (size 1728) L2 at 154336 using event 6 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1174952), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 154336), 1728, 0, &UchanHF6);
	/* Moving Featureextractormobilenetv2exp_f6b95d2f, size 768 from HyperFlash at 1215144 to (size 768) L2 at 156064 using event 12 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1215144), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 156064), 768, 0, &UchanHF12);
	/* Moving S83_Mul_scale, size 192 from HyperFlash at 1240344 to (size 192) L2 at 156832 using event 13 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1240344), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 156832), 192, 0, &UchanHF13);
	/* Moving S83_Mul_shift, size 192 from HyperFlash at 1240536 to (size 192) L2 at 157024 using event 14 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1240536), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 157024), 192, 0, &UchanHF14);
	/* Moving S83_Infos, size 9 from HyperFlash at 1250980 to (size 9) L2 at 157408 using event 15 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250980), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 157408), 9, 0, &UchanHF15);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_9046602e using event 7 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF7);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_712985b5 using event 8 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF8);
	/* Waiting completion of transfer of S76_Mul_scale using event 9 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF9);
	/* Waiting completion of transfer of S76_Mul_shift using event 10 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF10);
	/* Waiting completion of transfer of S76_Infos using event 11 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF11);
	SSD_Monitor[27] = gap_cl_readhwtimer();
	S76_Conv2d_32x192x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+90592)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+148192)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+157216)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+32992)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+157344)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+157376)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+157420)) /* Infos */
	);
	SSD_Monitor[27] = gap_cl_readhwtimer() - SSD_Monitor[27];
	/* Moving Featureextractormobilenetv2exp_87c48226, size 6144 from HyperFlash at 1072968 to (size 6144) L2 at 148192 using event 7 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1072968), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 148192), 6144, 0, &UchanHF7);
	/* Moving Featureextractormobilenetv2exp_0824e34e, size 128 from HyperFlash at 1243560 to (size 128) L2 at 157216 using event 8 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1243560), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 157216), 128, 0, &UchanHF8);
	/* Moving S86_Mul_scale, size 32 from HyperFlash at 1249732 to (size 32) L2 at 157344 using event 9 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249732), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 157344), 32, 0, &UchanHF9);
	/* Moving S86_Mul_shift, size 32 from HyperFlash at 1249764 to (size 32) L2 at 157376 using event 10 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249764), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 157376), 32, 0, &UchanHF10);
	/* Moving S86_Infos, size 9 from HyperFlash at 1250992 to (size 9) L2 at 157420 using event 11 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250992), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 157420), 9, 0, &UchanHF11);
	/* Waiting completion of transfer of S77_Infos using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	SSD_Monitor[28] = gap_cl_readhwtimer();
	S77_MatAdd_32x15x20(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+32992)), /* In1 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+138592)), /* Out */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+42592)) /* Infos */
	);
	SSD_Monitor[28] = gap_cl_readhwtimer() - SSD_Monitor[28];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_aa5a1409 using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_c8bd61b2 using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	/* Waiting completion of transfer of S80_Mul_scale using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	/* Waiting completion of transfer of S80_Mul_shift using event 4 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4);
	/* Waiting completion of transfer of S80_Infos using event 5 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5);
	SSD_Monitor[29] = gap_cl_readhwtimer();
	S80_Conv2d_192x32x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+138592)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+80992)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+87136)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+87904)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+88096)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+88288)) /* Infos */
	);
	SSD_Monitor[29] = gap_cl_readhwtimer() - SSD_Monitor[29];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_504b4626 using event 6 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF6);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_f6b95d2f using event 12 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF12);
	/* Waiting completion of transfer of S83_Mul_scale using event 13 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF13);
	/* Waiting completion of transfer of S83_Mul_shift using event 14 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF14);
	/* Waiting completion of transfer of S83_Infos using event 15 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF15);
	SSD_Monitor[30] = gap_cl_readhwtimer();
	S83_Conv2d_192x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+154336)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+156064)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+80992)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+156832)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+157024)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+157408)) /* Infos */
	);
	SSD_Monitor[30] = gap_cl_readhwtimer() - SSD_Monitor[30];
	/* Moving S87_Infos, size 9 from HyperFlash at 1251004 to (size 9) L2 at 32992 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251004), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 32992), 9, 0, &UchanHF0);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_87c48226 using event 7 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF7);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_0824e34e using event 8 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF8);
	/* Waiting completion of transfer of S86_Mul_scale using event 9 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF9);
	/* Waiting completion of transfer of S86_Mul_shift using event 10 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF10);
	/* Waiting completion of transfer of S86_Infos using event 11 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF11);
	SSD_Monitor[31] = gap_cl_readhwtimer();
	S86_Conv2d_32x192x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+80992)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+148192)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+157216)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+157344)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+157376)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+157420)) /* Infos */
	);
	SSD_Monitor[31] = gap_cl_readhwtimer() - SSD_Monitor[31];
	/* Moving Featureextractormobilenetv2exp_e5d6b18e, size 6144 from HyperFlash at 1079112 to (size 6144) L2 at 80992 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1079112), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 80992), 6144, 0, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_72e5a603, size 768 from HyperFlash at 1215912 to (size 768) L2 at 87136 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1215912), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 87136), 768, 0, &UchanHF2);
	/* Moving S90_Mul_scale, size 192 from HyperFlash at 1240728 to (size 192) L2 at 87904 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1240728), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 87904), 192, 0, &UchanHF3);
	/* Moving S90_Mul_shift, size 192 from HyperFlash at 1240920 to (size 192) L2 at 88096 using event 4 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1240920), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 88096), 192, 0, &UchanHF4);
	/* Moving S90_Infos, size 9 from HyperFlash at 1251016 to (size 9) L2 at 88288 using event 5 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251016), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 88288), 9, 0, &UchanHF5);
	/* Waiting completion of transfer of S87_Infos using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	SSD_Monitor[32] = gap_cl_readhwtimer();
	S87_MatAdd_32x15x20(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+138592)), /* In1 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+148192)), /* Out */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+32992)) /* Infos */
	);
	SSD_Monitor[32] = gap_cl_readhwtimer() - SSD_Monitor[32];
	/* Moving Featureextractormobilenetv2exp_48c92884, size 1728 from HyperFlash at 1176680 to (size 1728) L2 at 144736 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1176680), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 144736), 1728, 0, &UchanHF0);
	/* Moving Featureextractormobilenetv2exp_84d4964c, size 768 from HyperFlash at 1216680 to (size 768) L2 at 146464 using event 6 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1216680), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 146464), 768, 0, &UchanHF6);
	/* Moving S93_Mul_scale, size 192 from HyperFlash at 1241112 to (size 192) L2 at 147232 using event 7 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1241112), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 147232), 192, 0, &UchanHF7);
	/* Moving S93_Mul_shift, size 192 from HyperFlash at 1241304 to (size 192) L2 at 147424 using event 8 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1241304), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 147424), 192, 0, &UchanHF8);
	/* Moving S93_Infos, size 9 from HyperFlash at 1251028 to (size 9) L2 at 147808 using event 9 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251028), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 147808), 9, 0, &UchanHF9);
	/* Moving Featureextractormobilenetv2exp_a45cfb5d, size 6144 from HyperFlash at 1085256 to (size 6144) L2 at 138592 using event 10 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1085256), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 138592), 6144, 0, &UchanHF10);
	/* Moving Featureextractormobilenetv2exp_32ac9c9c, size 128 from HyperFlash at 1243688 to (size 128) L2 at 147616 using event 11 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1243688), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 147616), 128, 0, &UchanHF11);
	/* Moving S96_Mul_scale, size 32 from HyperFlash at 1249796 to (size 32) L2 at 147744 using event 12 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249796), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 147744), 32, 0, &UchanHF12);
	/* Moving S96_Mul_shift, size 32 from HyperFlash at 1249828 to (size 32) L2 at 147776 using event 13 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249828), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 147776), 32, 0, &UchanHF13);
	/* Moving S96_Infos, size 9 from HyperFlash at 1251040 to (size 9) L2 at 147820 using event 14 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251040), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 147820), 9, 0, &UchanHF14);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_e5d6b18e using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_72e5a603 using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	/* Waiting completion of transfer of S90_Mul_scale using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	/* Waiting completion of transfer of S90_Mul_shift using event 4 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4);
	/* Waiting completion of transfer of S90_Infos using event 5 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5);
	SSD_Monitor[33] = gap_cl_readhwtimer();
	S90_Conv2d_192x32x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+148192)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+80992)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+87136)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+87904)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+88096)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+88288)) /* Infos */
	);
	SSD_Monitor[33] = gap_cl_readhwtimer() - SSD_Monitor[33];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_48c92884 using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_84d4964c using event 6 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF6);
	/* Waiting completion of transfer of S93_Mul_scale using event 7 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF7);
	/* Waiting completion of transfer of S93_Mul_shift using event 8 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF8);
	/* Waiting completion of transfer of S93_Infos using event 9 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF9);
	SSD_Monitor[34] = gap_cl_readhwtimer();
	S93_Conv2d_192x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+144736)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+146464)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+80992)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+147232)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+147424)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+147808)) /* Infos */
	);
	SSD_Monitor[34] = gap_cl_readhwtimer() - SSD_Monitor[34];
	/* Moving S97_Infos, size 9 from HyperFlash at 1251052 to (size 9) L2 at 49888 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251052), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 49888), 9, 0, &UchanHF0);
	/* Moving Featureextractormobilenetv2exp_172c2d0b, size 6144 from HyperFlash at 1091400 to (size 6144) L2 at 42592 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1091400), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 42592), 6144, 0, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_190b2a61, size 768 from HyperFlash at 1217448 to (size 768) L2 at 48736 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1217448), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 48736), 768, 0, &UchanHF2);
	/* Moving S100_Mul_scale, size 192 from HyperFlash at 1241496 to (size 192) L2 at 49504 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1241496), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 49504), 192, 0, &UchanHF3);
	/* Moving S100_Mul_shift, size 192 from HyperFlash at 1241688 to (size 192) L2 at 49696 using event 4 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1241688), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 49696), 192, 0, &UchanHF4);
	/* Moving S100_Infos, size 9 from HyperFlash at 1251064 to (size 9) L2 at 49900 using event 5 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251064), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 49900), 9, 0, &UchanHF5);
	/* Moving Featureextractormobilenetv2exp_aec326a5, size 1152 from HyperFlash at 1198568 to (size 1152) L2 at 181792 using event 6 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1198568), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 181792), 1152, 0, &UchanHF6);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_a45cfb5d using event 10 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF10);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_32ac9c9c using event 11 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF11);
	/* Waiting completion of transfer of S96_Mul_scale using event 12 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF12);
	/* Waiting completion of transfer of S96_Mul_shift using event 13 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF13);
	/* Waiting completion of transfer of S96_Infos using event 14 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF14);
	SSD_Monitor[35] = gap_cl_readhwtimer();
	S96_Conv2d_32x192x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+80992)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+138592)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+147616)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+32992)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+147744)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+147776)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+147820)) /* Infos */
	);
	SSD_Monitor[35] = gap_cl_readhwtimer() - SSD_Monitor[35];
	/* Moving Featureextractormobilenetv2exp_bc6311f9, size 9216 from HyperFlash at 1030912 to (size 9216) L2 at 138592 using event 7 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1030912), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 138592), 9216, 0, &UchanHF7);
	/* Waiting completion of transfer of S97_Infos using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	SSD_Monitor[36] = gap_cl_readhwtimer();
	S97_MatAdd_32x15x20(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+148192)), /* In1 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+32992)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+49888)) /* Infos */
	);
	SSD_Monitor[36] = gap_cl_readhwtimer() - SSD_Monitor[36];
	/* Moving Featureextractormobilenetv2exp_d364dbb1, size 1728 from HyperFlash at 1178408 to (size 1728) L2 at 147808 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1178408), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 147808), 1728, 0, &UchanHF0);
	/* Moving Featureextractormobilenetv2exp_9e08a626, size 768 from HyperFlash at 1218216 to (size 768) L2 at 149536 using event 8 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1218216), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 149536), 768, 0, &UchanHF8);
	/* Moving S103_Mul_scale, size 192 from HyperFlash at 1241880 to (size 192) L2 at 150304 using event 9 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1241880), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 150304), 192, 0, &UchanHF9);
	/* Moving S103_Mul_shift, size 192 from HyperFlash at 1242072 to (size 192) L2 at 150496 using event 10 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1242072), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 150496), 192, 0, &UchanHF10);
	/* Moving S103_Infos, size 9 from HyperFlash at 1251076 to (size 9) L2 at 150688 using event 11 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251076), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 150688), 9, 0, &UchanHF11);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_172c2d0b using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_190b2a61 using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	/* Waiting completion of transfer of S100_Mul_scale using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	/* Waiting completion of transfer of S100_Mul_shift using event 4 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4);
	/* Waiting completion of transfer of S100_Infos using event 5 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5);
	SSD_Monitor[37] = gap_cl_readhwtimer();
	S100_Conv2d_192x32x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+42592)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+48736)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+80992)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+49504)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+49696)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+49900)) /* Infos */
	);
	SSD_Monitor[37] = gap_cl_readhwtimer() - SSD_Monitor[37];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_d364dbb1 using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_9e08a626 using event 8 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF8);
	/* Waiting completion of transfer of S103_Mul_scale using event 9 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF9);
	/* Waiting completion of transfer of S103_Mul_shift using event 10 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF10);
	/* Waiting completion of transfer of S103_Infos using event 11 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF11);
	SSD_Monitor[38] = gap_cl_readhwtimer();
	S103_Conv2d_192x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+80992)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+147808)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+149536)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+150304)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+150496)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+150688)) /* Infos */
	);
	SSD_Monitor[38] = gap_cl_readhwtimer() - SSD_Monitor[38];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_bc6311f9 using event 7 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF7);
	SSD_Monitor[39] = gap_cl_readhwtimer();
	S106_Conv2d_48x192x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+138592)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1242264)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+80992)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23336)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1249280)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251088)) /* Infos */
	);
	SSD_Monitor[39] = gap_cl_readhwtimer() - SSD_Monitor[39];
	/* Moving Featureextractormobilenetv2exp_100db087, size 2592 from HyperFlash at 1144168 to (size 2592) L2 at 37216 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1144168), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 37216), 2592, 0, &UchanHF0);
	/* Moving Featureextractormobilenetv2exp_a517cc75, size 1152 from HyperFlash at 1199720 to (size 1152) L2 at 39808 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1199720), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 39808), 1152, 0, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_80de23f2, size 13824 from HyperFlash at 964096 to (size 13824) L2 at 56224 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 964096), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 56224), 13824, 0, &UchanHF2);
	/* Moving S116_Infos, size 9 from HyperFlash at 1251136 to (size 9) L2 at 70348 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251136), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 70348), 9, 0, &UchanHF3);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_aec326a5 using event 6 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF6);
	SSD_Monitor[40] = gap_cl_readhwtimer();
	S109_Conv2d_288x48x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+80992)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+950272)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+181792)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+95392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1234200)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1234488)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251100)) /* Infos */
	);
	SSD_Monitor[40] = gap_cl_readhwtimer() - SSD_Monitor[40];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_100db087 using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_a517cc75 using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	SSD_Monitor[41] = gap_cl_readhwtimer();
	S112_Conv2d_288x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+95392)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+37216)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+39808)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1234776)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1235064)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251112)) /* Infos */
	);
	SSD_Monitor[41] = gap_cl_readhwtimer() - SSD_Monitor[41];
	/* Moving Featureextractormobilenetv2exp_17e9da8c, size 13824 from HyperFlash at 977920 to (size 13824) L2 at 113632 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 977920), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 113632), 13824, 0, &UchanHF0);
	/* Moving Featureextractormobilenetv2exp_a5e8bb11, size 1152 from HyperFlash at 1200872 to (size 1152) L2 at 127456 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1200872), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 127456), 1152, 0, &UchanHF1);
	/* Moving S119_Mul_scale, size 288 from HyperFlash at 1235352 to (size 288) L2 at 128608 using event 4 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1235352), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 128608), 288, 0, &UchanHF4);
	/* Moving S119_Mul_shift, size 288 from HyperFlash at 1235640 to (size 288) L2 at 128896 using event 5 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1235640), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 128896), 288, 0, &UchanHF5);
	/* Moving S119_Infos, size 9 from HyperFlash at 1251148 to (size 9) L2 at 129184 using event 6 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251148), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 129184), 9, 0, &UchanHF6);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_80de23f2 using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	SSD_Monitor[42] = gap_cl_readhwtimer();
	S115_Conv2d_48x288x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+56224)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1242456)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1249328)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1249376)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251124)) /* Infos */
	);
	SSD_Monitor[42] = gap_cl_readhwtimer() - SSD_Monitor[42];
	/* Waiting completion of transfer of S116_Infos using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	SSD_Monitor[43] = gap_cl_readhwtimer();
	S116_MatAdd_48x15x20(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In1 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+80992)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* Out */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+70348)) /* Infos */
	);
	SSD_Monitor[43] = gap_cl_readhwtimer() - SSD_Monitor[43];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_17e9da8c using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_a5e8bb11 using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	/* Waiting completion of transfer of S119_Mul_scale using event 4 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4);
	/* Waiting completion of transfer of S119_Mul_shift using event 5 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5);
	/* Waiting completion of transfer of S119_Infos using event 6 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF6);
	SSD_Monitor[44] = gap_cl_readhwtimer();
	S119_Conv2d_288x48x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+113632)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+127456)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+128608)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+128896)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+129184)) /* Infos */
	);
	SSD_Monitor[44] = gap_cl_readhwtimer() - SSD_Monitor[44];
	SSD_Monitor[45] = gap_cl_readhwtimer();
	S122_Conv2d_288x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1146760)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1202024)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+109792)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1235928)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1236216)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251160)) /* Infos */
	);
	SSD_Monitor[45] = gap_cl_readhwtimer() - SSD_Monitor[45];
	/* Moving S126_Infos, size 9 from HyperFlash at 1251184 to (size 9) L2 at 52192 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251184), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 52192), 9, 0, &UchanHF0);
	SSD_Monitor[46] = gap_cl_readhwtimer();
	S125_Conv2d_48x288x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+109792)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+991744)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1242648)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+37792)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1249424)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1249472)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251172)) /* Infos */
	);
	SSD_Monitor[46] = gap_cl_readhwtimer() - SSD_Monitor[46];
	/* Moving Featureextractormobilenetv2exp_36da854d, size 13824 from HyperFlash at 1005568 to (size 13824) L2 at 124192 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1005568), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 124192), 13824, 0, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_14da572a, size 1152 from HyperFlash at 1203176 to (size 1152) L2 at 140608 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1203176), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 140608), 1152, 0, &UchanHF2);
	/* Moving S129_Mul_scale, size 288 from HyperFlash at 1236504 to (size 288) L2 at 141760 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1236504), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 141760), 288, 0, &UchanHF3);
	/* Moving S129_Mul_shift, size 288 from HyperFlash at 1236792 to (size 288) L2 at 142048 using event 4 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1236792), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 142048), 288, 0, &UchanHF4);
	/* Moving S129_Infos, size 9 from HyperFlash at 1251196 to (size 9) L2 at 142372 using event 5 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251196), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 142372), 9, 0, &UchanHF5);
	/* Moving Boxpredictor_0classpredictorwe, size 2592 from HyperFlash at 1151944 to (size 2592) L2 at 138016 using event 6 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1151944), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 138016), 2592, 0, &UchanHF6);
	/* Moving Boxpredictor_0classpredictorco, size 36 from HyperFlash at 1249568 to (size 36) L2 at 142336 using event 7 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249568), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 142336), 36, 0, &UchanHF7);
	/* Moving S199_Mul_scale, size 9 from HyperFlash at 1251508 to (size 9) L2 at 142384 using event 8 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251508), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 142384), 9, 0, &UchanHF8);
	/* Moving S199_Mul_shift, size 9 from HyperFlash at 1251520 to (size 9) L2 at 142396 using event 9 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251520), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 142396), 9, 0, &UchanHF9);
	/* Moving S199_Infos, size 9 from HyperFlash at 1251532 to (size 9) L2 at 142408 using event 10 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251532), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 142408), 9, 0, &UchanHF10);
	/* Waiting completion of transfer of S126_Infos using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	SSD_Monitor[47] = gap_cl_readhwtimer();
	S126_MatAdd_48x15x20(
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Memory+0)), /* In1 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+37792)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+109792)), /* Out */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+52192)) /* Infos */
	);
	SSD_Monitor[47] = gap_cl_readhwtimer() - SSD_Monitor[47];
	/* Moving Featureextractormobilenetv2exp_db5d9d70, size 2592 from HyperFlash at 1149352 to (size 2592) L2 at 166752 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1149352), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 166752), 2592, 0, &UchanHF0);
	/* Moving Featureextractormobilenetv2exp_1c666cb3, size 1152 from HyperFlash at 1204328 to (size 1152) L2 at 169344 using event 11 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1204328), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 169344), 1152, 0, &UchanHF11);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_36da854d using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_14da572a using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	/* Waiting completion of transfer of S129_Mul_scale using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	/* Waiting completion of transfer of S129_Mul_shift using event 4 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4);
	/* Waiting completion of transfer of S129_Infos using event 5 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5);
	SSD_Monitor[48] = gap_cl_readhwtimer();
	S129_Conv2d_288x48x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+109792)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+124192)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+140608)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+141760)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+142048)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+142372)) /* Infos */
	);
	SSD_Monitor[48] = gap_cl_readhwtimer() - SSD_Monitor[48];
	/* Moving Featureextractormobilenetv2exp_34736308, size 23040 from HyperFlash at 895488 to (size 23040) L2 at 143712 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 895488), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 143712), 23040, 0, &UchanHF1);
	/* Moving Boxpredictor_0boxencodingpredi_b48ea6a4, size 48 from HyperFlash at 1249520 to (size 48) L2 at 116848 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249520), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 116848), 48, 0, &UchanHF2);
	/* Moving S194_Mul_scale, size 12 from HyperFlash at 1251472 to (size 12) L2 at 116896 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251472), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 116896), 12, 0, &UchanHF3);
	/* Moving S194_Mul_shift, size 12 from HyperFlash at 1251484 to (size 12) L2 at 116908 using event 4 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251484), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 116908), 12, 0, &UchanHF4);
	/* Moving S194_Infos, size 9 from HyperFlash at 1251496 to (size 9) L2 at 116920 using event 5 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251496), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 116920), 9, 0, &UchanHF5);
	/* Waiting completion of transfer of Boxpredictor_0classpredictorwe using event 6 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF6);
	/* Waiting completion of transfer of Boxpredictor_0classpredictorco using event 7 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF7);
	/* Waiting completion of transfer of S199_Mul_scale using event 8 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF8);
	/* Waiting completion of transfer of S199_Mul_shift using event 9 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF9);
	/* Waiting completion of transfer of S199_Infos using event 10 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF10);
	SSD_Monitor[49] = gap_cl_readhwtimer();
	S199_Conv2d_9x288x1x1_Hsigmoid(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+138016)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+142336)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+109792)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+142384)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+142396)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+142408)) /* Infos */
	);
	SSD_Monitor[49] = gap_cl_readhwtimer() - SSD_Monitor[49];
	SSD_Monitor[50] = gap_cl_readhwtimer();
	S200_Op_CONV_2D_0_73_trans_out0(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+109792)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+139048)) /* Out */
	);
	SSD_Monitor[50] = gap_cl_readhwtimer() - SSD_Monitor[50];
	/* Waiting completion of transfer of Boxpredictor_0boxencodingpredi_b48ea6a4 using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	/* Waiting completion of transfer of S194_Mul_scale using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	/* Waiting completion of transfer of S194_Mul_shift using event 4 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4);
	/* Waiting completion of transfer of S194_Infos using event 5 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5);
	SSD_Monitor[51] = gap_cl_readhwtimer();
	S194_Conv2d_12x288x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+19688)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+116848)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+109792)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+116896)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+116908)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+116920)) /* Infos */
	);
	SSD_Monitor[51] = gap_cl_readhwtimer() - SSD_Monitor[51];
	SSD_Monitor[52] = gap_cl_readhwtimer();
	S195_Op_CONV_2D_0_71_trans_out0(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+109792)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+132832)) /* Out */
	);
	SSD_Monitor[52] = gap_cl_readhwtimer() - SSD_Monitor[52];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_db5d9d70 using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_1c666cb3 using event 11 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF11);
	SSD_Monitor[53] = gap_cl_readhwtimer();
	S132_Conv2d_288x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+166752)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+169344)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+109792)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1237080)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1237368)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251208)) /* Infos */
	);
	SSD_Monitor[53] = gap_cl_readhwtimer() - SSD_Monitor[53];
	/* Moving Featureextractormobilenetv2exp_9244bcb4, size 38400 from HyperFlash at 703488 to (size 38400) L2 at 68192 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 703488), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 68192), 38400, 0, &UchanHF0);
	/* Moving Featureextractormobilenetv2exp_261aeebe, size 1920 from HyperFlash at 1161704 to (size 1920) L2 at 166752 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1161704), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 166752), 1920, 0, &UchanHF2);
	/* Moving S138_Mul_scale, size 480 from HyperFlash at 1222440 to (size 480) L2 at 168672 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1222440), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 168672), 480, 0, &UchanHF3);
	/* Moving S138_Mul_shift, size 480 from HyperFlash at 1222920 to (size 480) L2 at 169152 using event 4 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1222920), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 169152), 480, 0, &UchanHF4);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_34736308 using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	SSD_Monitor[54] = gap_cl_readhwtimer();
	S135_Conv2d_80x288x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+109792)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+143712)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1233240)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1247496)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1247576)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251220)) /* Infos */
	);
	SSD_Monitor[54] = gap_cl_readhwtimer() - SSD_Monitor[54];
	/* Moving Featureextractormobilenetv2exp_16d57bcc, size 4320 from HyperFlash at 1114440 to (size 4320) L2 at 106592 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1114440), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 106592), 4320, 0, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_7600ef9a, size 1920 from HyperFlash at 1163624 to (size 1920) L2 at 110912 using event 5 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1163624), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 110912), 1920, 0, &UchanHF5);
	/* Moving S141_Mul_scale, size 480 from HyperFlash at 1223400 to (size 480) L2 at 112832 using event 6 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1223400), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 112832), 480, 0, &UchanHF6);
	/* Moving S141_Mul_shift, size 480 from HyperFlash at 1223880 to (size 480) L2 at 113312 using event 7 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1223880), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 113312), 480, 0, &UchanHF7);
	/* Moving S148_Mul_scale, size 480 from HyperFlash at 1224360 to (size 480) L2 at 114752 using event 8 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1224360), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 114752), 480, 0, &UchanHF8);
	/* Moving S148_Mul_shift, size 480 from HyperFlash at 1224840 to (size 480) L2 at 115232 using event 9 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1224840), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 115232), 480, 0, &UchanHF9);
	/* Moving S148_Infos, size 9 from HyperFlash at 1251280 to (size 9) L2 at 116672 using event 10 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251280), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 116672), 9, 0, &UchanHF10);
	/* Moving S151_Mul_scale, size 480 from HyperFlash at 1225320 to (size 480) L2 at 115712 using event 11 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1225320), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 115712), 480, 0, &UchanHF11);
	/* Moving S151_Mul_shift, size 480 from HyperFlash at 1225800 to (size 480) L2 at 116192 using event 12 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1225800), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 116192), 480, 0, &UchanHF12);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_9244bcb4 using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_261aeebe using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	/* Waiting completion of transfer of S138_Mul_scale using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	/* Waiting completion of transfer of S138_Mul_shift using event 4 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4);
	SSD_Monitor[55] = gap_cl_readhwtimer();
	S138_Conv2d_480x80x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+68192)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+166752)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+29792)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+168672)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+169152)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251232)) /* Infos */
	);
	SSD_Monitor[55] = gap_cl_readhwtimer() - SSD_Monitor[55];
	/* Moving Featureextractormobilenetv2exp_48d7e2f4, size 38400 from HyperFlash at 741888 to (size 38400) L2 at 143712 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 741888), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 143712), 38400, 0, &UchanHF0);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_16d57bcc using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_7600ef9a using event 5 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5);
	/* Waiting completion of transfer of S141_Mul_scale using event 6 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF6);
	/* Waiting completion of transfer of S141_Mul_shift using event 7 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF7);
	SSD_Monitor[56] = gap_cl_readhwtimer();
	S141_Conv2d_480x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+29792)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+106592)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+110912)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+68192)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+112832)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+113312)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251244)) /* Infos */
	);
	SSD_Monitor[56] = gap_cl_readhwtimer() - SSD_Monitor[56];
	/* Moving Featureextractormobilenetv2exp_6b7ac92d, size 1920 from HyperFlash at 1165544 to (size 1920) L2 at 110912 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1165544), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 110912), 1920, 0, &UchanHF1);
	/* Moving Featureextractormobilenetv2exp_b852e846, size 4320 from HyperFlash at 1118760 to (size 4320) L2 at 106592 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1118760), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 106592), 4320, 0, &UchanHF2);
	/* Moving Featureextractormobilenetv2exp_2eae7951, size 1920 from HyperFlash at 1167464 to (size 1920) L2 at 112832 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1167464), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 112832), 1920, 0, &UchanHF3);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_48d7e2f4 using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	SSD_Monitor[57] = gap_cl_readhwtimer();
	S144_Conv2d_80x480x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+68192)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+143712)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1233560)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+29792)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1247656)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1247736)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251256)) /* Infos */
	);
	SSD_Monitor[57] = gap_cl_readhwtimer() - SSD_Monitor[57];
	/* Moving Featureextractormobilenetv2exp_e09b1350, size 38400 from HyperFlash at 780288 to (size 38400) L2 at 61792 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 780288), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 61792), 38400, 0, &UchanHF0);
	/* Moving Featureextractormobilenetv2exp_808e88f2, size 38400 from HyperFlash at 818688 to (size 38400) L2 at 143712 using event 4 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 818688), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 143712), 38400, 0, &UchanHF4);
	SSD_Monitor[58] = gap_cl_readhwtimer();
	S145_MatAdd_80x8x10(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+29792)), /* In1 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+100192)), /* Out */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251268)) /* Infos */
	);
	SSD_Monitor[58] = gap_cl_readhwtimer() - SSD_Monitor[58];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_e09b1350 using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_6b7ac92d using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	/* Waiting completion of transfer of S148_Mul_scale using event 8 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF8);
	/* Waiting completion of transfer of S148_Mul_shift using event 9 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF9);
	/* Waiting completion of transfer of S148_Infos using event 10 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF10);
	SSD_Monitor[59] = gap_cl_readhwtimer();
	S148_Conv2d_480x80x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+100192)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+61792)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+110912)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+114752)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+115232)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+116672)) /* Infos */
	);
	SSD_Monitor[59] = gap_cl_readhwtimer() - SSD_Monitor[59];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_b852e846 using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_2eae7951 using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	/* Waiting completion of transfer of S151_Mul_scale using event 11 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF11);
	/* Waiting completion of transfer of S151_Mul_shift using event 12 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF12);
	SSD_Monitor[60] = gap_cl_readhwtimer();
	S151_Conv2d_480x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+106592)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+112832)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+61792)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+115712)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+116192)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251292)) /* Infos */
	);
	SSD_Monitor[60] = gap_cl_readhwtimer() - SSD_Monitor[60];
	/* Moving S161_Mul_scale, size 480 from HyperFlash at 1227240 to (size 480) L2 at 107072 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1227240), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 107072), 480, 0, &UchanHF0);
	/* Moving S161_Mul_shift, size 480 from HyperFlash at 1227720 to (size 480) L2 at 107552 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1227720), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 107552), 480, 0, &UchanHF1);
	/* Moving S164_Mul_scale, size 160 from HyperFlash at 1242840 to (size 160) L2 at 108032 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1242840), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 108032), 160, 0, &UchanHF2);
	/* Moving S164_Mul_shift, size 160 from HyperFlash at 1243000 to (size 160) L2 at 108192 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1243000), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 108192), 160, 0, &UchanHF3);
	/* Moving S164_Infos, size 9 from HyperFlash at 1251352 to (size 9) L2 at 108364 using event 5 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251352), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 108364), 9, 0, &UchanHF5);
	/* Moving Featureextractormobilenetv2con_ae878fea, size 2560 from HyperFlash at 1154536 to (size 2560) L2 at 121952 using event 6 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1154536), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 121952), 2560, 0, &UchanHF6);
	/* Moving S167_Mul_scale, size 640 from HyperFlash at 1219624 to (size 640) L2 at 124512 using event 7 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1219624), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 124512), 640, 0, &UchanHF7);
	/* Moving S167_Mul_shift, size 640 from HyperFlash at 1220264 to (size 640) L2 at 125152 using event 8 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1220264), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 125152), 640, 0, &UchanHF8);
	/* Moving S167_Infos, size 9 from HyperFlash at 1251364 to (size 9) L2 at 125904 using event 9 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251364), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 125904), 9, 0, &UchanHF9);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_808e88f2 using event 4 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4);
	SSD_Monitor[61] = gap_cl_readhwtimer();
	S154_Conv2d_80x480x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+61792)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+143712)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L3_Flash+1233880)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1247816)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1247896)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251304)) /* Infos */
	);
	SSD_Monitor[61] = gap_cl_readhwtimer() - SSD_Monitor[61];
	/* Moving Featureextractormobilenetv2exp_c26b18f7, size 38400 from HyperFlash at 857088 to (size 38400) L2 at 143712 using event 4 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 857088), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 143712), 38400, 0, &UchanHF4);
	/* Moving Featureextractormobilenetv2exp_56728354, size 1920 from HyperFlash at 1169384 to (size 1920) L2 at 68192 using event 10 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1169384), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 68192), 1920, 0, &UchanHF10);
	/* Moving S158_Mul_scale, size 480 from HyperFlash at 1226280 to (size 480) L2 at 70112 using event 11 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1226280), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 70112), 480, 0, &UchanHF11);
	/* Moving S158_Mul_shift, size 480 from HyperFlash at 1226760 to (size 480) L2 at 70592 using event 12 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1226760), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 70592), 480, 0, &UchanHF12);
	/* Moving Boxpredictor_1classpredictorwe, size 11520 from HyperFlash at 1019392 to (size 11520) L2 at 110432 using event 13 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1019392), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 110432), 11520, 0, &UchanHF13);
	/* Moving Boxpredictor_1classpredictorco, size 72 from HyperFlash at 1247976 to (size 72) L2 at 125792 using event 14 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1247976), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 125792), 72, 0, &UchanHF14);
	/* Moving S209_Mul_scale, size 18 from HyperFlash at 1250164 to (size 18) L2 at 125864 using event 15 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250164), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 125864), 18, 0, &UchanHF15);
	/* Moving S209_Mul_shift, size 18 from HyperFlash at 1250184 to (size 18) L2 at 125884 using event 16 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250184), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 125884), 18, 0, &UchanHF16);
	/* Moving S209_Infos, size 9 from HyperFlash at 1251556 to (size 9) L2 at 125916 using event 17 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251556), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 125916), 9, 0, &UchanHF17);
	SSD_Monitor[62] = gap_cl_readhwtimer();
	S155_MatAdd_80x8x10(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+100192)), /* In1 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+61792)), /* Out */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251316)) /* Infos */
	);
	SSD_Monitor[62] = gap_cl_readhwtimer() - SSD_Monitor[62];
	/* Moving Featureextractormobilenetv2exp_6d709c12, size 4320 from HyperFlash at 1123080 to (size 4320) L2 at 100192 using event 18 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1123080), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 100192), 4320, 0, &UchanHF18);
	/* Moving Featureextractormobilenetv2exp_edeb4d0d, size 1920 from HyperFlash at 1171304 to (size 1920) L2 at 104512 using event 19 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1171304), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 104512), 1920, 0, &UchanHF19);
	/* Moving Featureextractormobilenetv2exp_9d792281, size 640 from HyperFlash at 1218984 to (size 640) L2 at 106432 using event 20 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1218984), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 106432), 640, 0, &UchanHF20);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_c26b18f7 using event 4 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_56728354 using event 10 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF10);
	/* Waiting completion of transfer of S158_Mul_scale using event 11 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF11);
	/* Waiting completion of transfer of S158_Mul_shift using event 12 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF12);
	SSD_Monitor[63] = gap_cl_readhwtimer();
	S158_Conv2d_480x80x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+61792)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+143712)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+68192)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+70112)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+70592)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251328)) /* Infos */
	);
	SSD_Monitor[63] = gap_cl_readhwtimer() - SSD_Monitor[63];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_6d709c12 using event 18 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF18);
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_edeb4d0d using event 19 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF19);
	/* Waiting completion of transfer of S161_Mul_scale using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	/* Waiting completion of transfer of S161_Mul_shift using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	SSD_Monitor[64] = gap_cl_readhwtimer();
	S161_Conv2d_480x1x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+100192)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+104512)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+61792)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+107072)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+107552)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1251340)) /* Infos */
	);
	SSD_Monitor[64] = gap_cl_readhwtimer() - SSD_Monitor[64];
	/* Waiting completion of transfer of Featureextractormobilenetv2exp_9d792281 using event 20 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF20);
	/* Waiting completion of transfer of S164_Mul_scale using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	/* Waiting completion of transfer of S164_Mul_shift using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	/* Waiting completion of transfer of S164_Infos using event 5 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5);
	SSD_Monitor[65] = gap_cl_readhwtimer();
	S164_Conv2d_160x480x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+61792)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+479232)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+106432)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+108032)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+108192)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+108364)) /* Infos */
	);
	SSD_Monitor[65] = gap_cl_readhwtimer() - SSD_Monitor[65];
	/* Waiting completion of transfer of Featureextractormobilenetv2con_ae878fea using event 6 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF6);
	/* Waiting completion of transfer of S167_Mul_scale using event 7 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF7);
	/* Waiting completion of transfer of S167_Mul_shift using event 8 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF8);
	/* Waiting completion of transfer of S167_Infos using event 9 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF9);
	SSD_Monitor[66] = gap_cl_readhwtimer();
	S167_Conv2d_640x160x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+0)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+121952)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+36192)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+124512)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+125152)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+125904)) /* Infos */
	);
	SSD_Monitor[66] = gap_cl_readhwtimer() - SSD_Monitor[66];
	/* Moving Featureextractormobilenetv2lay, size 512 from HyperFlash at 1220904 to (size 512) L2 at 33632 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1220904), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 33632), 512, 0, &UchanHF0);
	/* Moving S170_Mul_scale, size 128 from HyperFlash at 1243816 to (size 128) L2 at 34144 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1243816), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 34144), 128, 0, &UchanHF1);
	/* Moving S170_Mul_shift, size 128 from HyperFlash at 1243944 to (size 128) L2 at 34272 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1243944), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 34272), 128, 0, &UchanHF2);
	/* Moving S170_Infos, size 9 from HyperFlash at 1251376 to (size 9) L2 at 34400 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251376), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 34400), 9, 0, &UchanHF3);
	/* Moving Featureextractormobilenetv2lay_b6d92256, size 1024 from HyperFlash at 1206632 to (size 1024) L2 at 107872 using event 4 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1206632), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 107872), 1024, 0, &UchanHF4);
	/* Moving S173_Mul_scale, size 256 from HyperFlash at 1237912 to (size 256) L2 at 108896 using event 5 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1237912), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 108896), 256, 0, &UchanHF5);
	/* Moving S173_Mul_shift, size 256 from HyperFlash at 1238168 to (size 256) L2 at 109152 using event 6 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1238168), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 109152), 256, 0, &UchanHF6);
	/* Moving S173_Infos, size 9 from HyperFlash at 1251388 to (size 9) L2 at 109408 using event 7 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251388), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 109408), 9, 0, &UchanHF7);
	/* Moving Boxpredictor_1boxencodingpredi_3dba6efc, size 15360 from HyperFlash at 934912 to (size 15360) L2 at 87392 using event 8 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 934912), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 87392), 15360, 0, &UchanHF8);
	/* Moving Boxpredictor_1boxencodingpredi_2a41b47f, size 96 from HyperFlash at 1247016 to (size 96) L2 at 25312 using event 9 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1247016), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 25312), 96, 0, &UchanHF9);
	/* Moving S204_Mul_scale, size 24 from HyperFlash at 1249924 to (size 24) L2 at 25408 using event 10 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249924), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 25408), 24, 0, &UchanHF10);
	/* Moving S204_Mul_shift, size 24 from HyperFlash at 1249948 to (size 24) L2 at 25432 using event 11 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249948), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 25432), 24, 0, &UchanHF11);
	/* Moving S204_Infos, size 9 from HyperFlash at 1251544 to (size 9) L2 at 25456 using event 12 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251544), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 25456), 9, 0, &UchanHF12);
	/* Waiting completion of transfer of Boxpredictor_1classpredictorwe using event 13 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF13);
	/* Waiting completion of transfer of Boxpredictor_1classpredictorco using event 14 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF14);
	/* Waiting completion of transfer of S209_Mul_scale using event 15 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF15);
	/* Waiting completion of transfer of S209_Mul_shift using event 16 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF16);
	/* Waiting completion of transfer of S209_Infos using event 17 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF17);
	SSD_Monitor[67] = gap_cl_readhwtimer();
	S209_Conv2d_18x640x1x1_Hsigmoid(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+36192)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+110432)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+125792)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+125864)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+125884)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+125916)) /* Infos */
	);
	SSD_Monitor[67] = gap_cl_readhwtimer() - SSD_Monitor[67];
	SSD_Monitor[68] = gap_cl_readhwtimer();
	S210_Op_CONV_2D_0_77_trans_out0(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) ((SSD_tin_can_bottle_L2_Memory+139048) + 2700)) /* Out */
	);
	SSD_Monitor[68] = gap_cl_readhwtimer() - SSD_Monitor[68];
	/* Waiting completion of transfer of Boxpredictor_1boxencodingpredi_3dba6efc using event 8 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF8);
	/* Waiting completion of transfer of Boxpredictor_1boxencodingpredi_2a41b47f using event 9 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF9);
	/* Waiting completion of transfer of S204_Mul_scale using event 10 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF10);
	/* Waiting completion of transfer of S204_Mul_shift using event 11 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF11);
	/* Waiting completion of transfer of S204_Infos using event 12 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF12);
	SSD_Monitor[69] = gap_cl_readhwtimer();
	S204_Conv2d_24x640x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+36192)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+87392)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+25312)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+25408)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+25432)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+25456)) /* Infos */
	);
	SSD_Monitor[69] = gap_cl_readhwtimer() - SSD_Monitor[69];
	SSD_Monitor[70] = gap_cl_readhwtimer();
	S205_Op_CONV_2D_0_75_trans_out0(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) ((SSD_tin_can_bottle_L2_Memory+132832) + 3600)) /* Out */
	);
	SSD_Monitor[70] = gap_cl_readhwtimer() - SSD_Monitor[70];
	/* Waiting completion of transfer of Featureextractormobilenetv2lay using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	/* Waiting completion of transfer of S170_Mul_scale using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	/* Waiting completion of transfer of S170_Mul_shift using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	/* Waiting completion of transfer of S170_Infos using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	SSD_Monitor[71] = gap_cl_readhwtimer();
	S170_Conv2d_128x640x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+36192)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+397312)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+33632)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+34144)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+34272)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+34400)) /* Infos */
	);
	SSD_Monitor[71] = gap_cl_readhwtimer() - SSD_Monitor[71];
	/* Moving Boxpredictor_2classpredictorwe, size 4608 from HyperFlash at 1109832 to (size 4608) L2 at 57184 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1109832), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 57184), 4608, 0, &UchanHF0);
	/* Moving Boxpredictor_2classpredictorco, size 72 from HyperFlash at 1248048 to (size 72) L2 at 61792 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1248048), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 61792), 72, 0, &UchanHF1);
	/* Moving S219_Mul_scale, size 18 from HyperFlash at 1250204 to (size 18) L2 at 61864 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250204), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 61864), 18, 0, &UchanHF2);
	/* Moving S219_Mul_shift, size 18 from HyperFlash at 1250224 to (size 18) L2 at 61884 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250224), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 61884), 18, 0, &UchanHF3);
	/* Moving S219_Infos, size 9 from HyperFlash at 1251580 to (size 9) L2 at 61904 using event 8 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251580), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 61904), 9, 0, &UchanHF8);
	/* Waiting completion of transfer of Featureextractormobilenetv2lay_b6d92256 using event 4 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4);
	/* Waiting completion of transfer of S173_Mul_scale using event 5 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5);
	/* Waiting completion of transfer of S173_Mul_shift using event 6 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF6);
	/* Waiting completion of transfer of S173_Infos using event 7 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF7);
	SSD_Monitor[72] = gap_cl_readhwtimer();
	S173_Conv2d_256x128x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+102400)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+107872)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+33632)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+108896)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+109152)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+109408)) /* Infos */
	);
	SSD_Monitor[72] = gap_cl_readhwtimer() - SSD_Monitor[72];
	/* Moving Featureextractormobilenetv2lay_47288463, size 512 from HyperFlash at 1221416 to (size 512) L2 at 71520 using event 4 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1221416), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 71520), 512, 0, &UchanHF4);
	/* Moving S179_Mul_scale, size 128 from HyperFlash at 1244072 to (size 128) L2 at 72032 using event 5 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1244072), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 72032), 128, 0, &UchanHF5);
	/* Moving S179_Mul_shift, size 128 from HyperFlash at 1244200 to (size 128) L2 at 72160 using event 6 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1244200), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 72160), 128, 0, &UchanHF6);
	/* Moving S179_Infos, size 9 from HyperFlash at 1251412 to (size 9) L2 at 72400 using event 7 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251412), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 72400), 9, 0, &UchanHF7);
	/* Moving Boxpredictor_2boxencodingpredi_aba037ce, size 6144 from HyperFlash at 1103688 to (size 6144) L2 at 23872 using event 9 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1103688), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 23872), 6144, 0, &UchanHF9);
	/* Moving Boxpredictor_2boxencodingpredi_25bcf53a, size 96 from HyperFlash at 1247112 to (size 96) L2 at 30016 using event 10 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1247112), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 30016), 96, 0, &UchanHF10);
	/* Moving S214_Mul_scale, size 24 from HyperFlash at 1249972 to (size 24) L2 at 30112 using event 11 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249972), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 30112), 24, 0, &UchanHF11);
	/* Moving S214_Mul_shift, size 24 from HyperFlash at 1249996 to (size 24) L2 at 30136 using event 12 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249996), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 30136), 24, 0, &UchanHF12);
	/* Moving S214_Infos, size 9 from HyperFlash at 1251568 to (size 9) L2 at 30160 using event 13 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251568), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 30160), 9, 0, &UchanHF13);
	/* Waiting completion of transfer of Boxpredictor_2classpredictorwe using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	/* Waiting completion of transfer of Boxpredictor_2classpredictorco using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	/* Waiting completion of transfer of S219_Mul_scale using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	/* Waiting completion of transfer of S219_Mul_shift using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	/* Waiting completion of transfer of S219_Infos using event 8 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF8);
	SSD_Monitor[73] = gap_cl_readhwtimer();
	S219_Conv2d_18x256x1x1_Hsigmoid(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+33632)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+57184)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+61792)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+61864)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+61884)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+61904)) /* Infos */
	);
	SSD_Monitor[73] = gap_cl_readhwtimer() - SSD_Monitor[73];
	/* Moving Boxpredictor_3classpredictorco, size 72 from HyperFlash at 1248120 to (size 72) L2 at 72288 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1248120), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 72288), 72, 0, &UchanHF0);
	/* Moving S229_Mul_scale, size 18 from HyperFlash at 1250244 to (size 18) L2 at 72360 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250244), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 72360), 18, 0, &UchanHF1);
	/* Moving S229_Mul_shift, size 18 from HyperFlash at 1250264 to (size 18) L2 at 72380 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250264), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 72380), 18, 0, &UchanHF2);
	/* Moving S229_Infos, size 9 from HyperFlash at 1251604 to (size 9) L2 at 72412 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251604), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 72412), 9, 0, &UchanHF3);
	SSD_Monitor[74] = gap_cl_readhwtimer();
	S220_Op_CONV_2D_0_81_trans_out0(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) ((SSD_tin_can_bottle_L2_Memory+139048) + 4140)) /* Out */
	);
	SSD_Monitor[74] = gap_cl_readhwtimer() - SSD_Monitor[74];
	/* Waiting completion of transfer of Boxpredictor_2boxencodingpredi_aba037ce using event 9 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF9);
	/* Waiting completion of transfer of Boxpredictor_2boxencodingpredi_25bcf53a using event 10 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF10);
	/* Waiting completion of transfer of S214_Mul_scale using event 11 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF11);
	/* Waiting completion of transfer of S214_Mul_shift using event 12 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF12);
	/* Waiting completion of transfer of S214_Infos using event 13 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF13);
	SSD_Monitor[75] = gap_cl_readhwtimer();
	S214_Conv2d_24x256x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+33632)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23872)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+30016)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+30112)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+30136)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+30160)) /* Infos */
	);
	SSD_Monitor[75] = gap_cl_readhwtimer() - SSD_Monitor[75];
	/* Moving Featureextractormobilenetv2lay_45c41df4, size 256 from HyperFlash at 1238424 to (size 256) L2 at 24672 using event 8 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1238424), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 24672), 256, 0, &UchanHF8);
	/* Moving S176_Mul_scale, size 64 from HyperFlash at 1248848 to (size 64) L2 at 24928 using event 9 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1248848), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 24928), 64, 0, &UchanHF9);
	/* Moving S176_Mul_shift, size 64 from HyperFlash at 1248912 to (size 64) L2 at 24992 using event 10 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1248912), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 24992), 64, 0, &UchanHF10);
	/* Moving S176_Infos, size 9 from HyperFlash at 1251400 to (size 9) L2 at 25056 using event 11 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251400), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 25056), 9, 0, &UchanHF11);
	SSD_Monitor[76] = gap_cl_readhwtimer();
	S215_Op_CONV_2D_0_79_trans_out0(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) ((SSD_tin_can_bottle_L2_Memory+132832) + 5520)) /* Out */
	);
	SSD_Monitor[76] = gap_cl_readhwtimer() - SSD_Monitor[76];
	/* Waiting completion of transfer of Featureextractormobilenetv2lay_45c41df4 using event 8 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF8);
	/* Waiting completion of transfer of S176_Mul_scale using event 9 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF9);
	/* Waiting completion of transfer of S176_Mul_shift using event 10 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF10);
	/* Waiting completion of transfer of S176_Infos using event 11 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF11);
	SSD_Monitor[77] = gap_cl_readhwtimer();
	S176_Conv2d_64x256x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+33632)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+3304)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+24672)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+24928)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+24992)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+25056)) /* Infos */
	);
	SSD_Monitor[77] = gap_cl_readhwtimer() - SSD_Monitor[77];
	/* Waiting completion of transfer of Featureextractormobilenetv2lay_47288463 using event 4 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4);
	/* Waiting completion of transfer of S179_Mul_scale using event 5 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5);
	/* Waiting completion of transfer of S179_Mul_shift using event 6 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF6);
	/* Waiting completion of transfer of S179_Infos using event 7 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF7);
	SSD_Monitor[78] = gap_cl_readhwtimer();
	S179_Conv2d_128x64x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+556032)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+71520)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+24672)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+72032)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+72160)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+72400)) /* Infos */
	);
	SSD_Monitor[78] = gap_cl_readhwtimer() - SSD_Monitor[78];
	/* Moving Featureextractormobilenetv2lay_950f395a, size 256 from HyperFlash at 1238680 to (size 256) L2 at 24032 using event 4 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1238680), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 24032), 256, 0, &UchanHF4);
	/* Moving S182_Mul_shift, size 64 from HyperFlash at 1249040 to (size 64) L2 at 24288 using event 5 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249040), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 24288), 64, 0, &UchanHF5);
	/* Moving Featureextractormobilenetv2lay_493d8d4d, size 512 from HyperFlash at 1221928 to (size 512) L2 at 70112 using event 6 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1221928), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 70112), 512, 0, &UchanHF6);
	/* Moving S185_Mul_scale, size 128 from HyperFlash at 1244328 to (size 128) L2 at 70624 using event 7 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1244328), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 70624), 128, 0, &UchanHF7);
	/* Moving S185_Mul_shift, size 128 from HyperFlash at 1244456 to (size 128) L2 at 70752 using event 8 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1244456), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 70752), 128, 0, &UchanHF8);
	/* Moving S185_Infos, size 9 from HyperFlash at 1251436 to (size 9) L2 at 70992 using event 9 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251436), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 70992), 9, 0, &UchanHF9);
	/* Moving Boxpredictor_3boxencodingpredi_c12a614a, size 96 from HyperFlash at 1247208 to (size 96) L2 at 23536 using event 10 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1247208), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 23536), 96, 0, &UchanHF10);
	/* Moving S224_Mul_scale, size 24 from HyperFlash at 1250020 to (size 24) L2 at 23632 using event 11 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250020), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 23632), 24, 0, &UchanHF11);
	/* Moving S224_Mul_shift, size 24 from HyperFlash at 1250044 to (size 24) L2 at 23656 using event 12 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250044), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 23656), 24, 0, &UchanHF12);
	/* Moving S224_Infos, size 9 from HyperFlash at 1251592 to (size 9) L2 at 23680 using event 13 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251592), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 23680), 9, 0, &UchanHF13);
	/* Waiting completion of transfer of Boxpredictor_3classpredictorco using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	/* Waiting completion of transfer of S229_Mul_scale using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	/* Waiting completion of transfer of S229_Mul_shift using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	/* Waiting completion of transfer of S229_Infos using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	SSD_Monitor[79] = gap_cl_readhwtimer();
	S229_Conv2d_18x128x1x1_Hsigmoid(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+24672)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1157096)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+72288)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+72360)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+72380)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+72412)) /* Infos */
	);
	SSD_Monitor[79] = gap_cl_readhwtimer() - SSD_Monitor[79];
	/* Moving Boxpredictor_4classpredictorco, size 72 from HyperFlash at 1248192 to (size 72) L2 at 70880 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1248192), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 70880), 72, 0, &UchanHF0);
	/* Moving S239_Mul_scale, size 18 from HyperFlash at 1250284 to (size 18) L2 at 70952 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250284), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 70952), 18, 0, &UchanHF1);
	/* Moving S239_Mul_shift, size 18 from HyperFlash at 1250304 to (size 18) L2 at 70972 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250304), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 70972), 18, 0, &UchanHF2);
	/* Moving S239_Infos, size 9 from HyperFlash at 1251628 to (size 9) L2 at 71004 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251628), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 71004), 9, 0, &UchanHF3);
	SSD_Monitor[80] = gap_cl_readhwtimer();
	S230_Op_CONV_2D_0_85_trans_out0(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) ((SSD_tin_can_bottle_L2_Memory+139048) + 4500)) /* Out */
	);
	SSD_Monitor[80] = gap_cl_readhwtimer() - SSD_Monitor[80];
	/* Waiting completion of transfer of Boxpredictor_3boxencodingpredi_c12a614a using event 10 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF10);
	/* Waiting completion of transfer of S224_Mul_scale using event 11 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF11);
	/* Waiting completion of transfer of S224_Mul_shift using event 12 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF12);
	/* Waiting completion of transfer of S224_Infos using event 13 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF13);
	SSD_Monitor[81] = gap_cl_readhwtimer();
	S224_Conv2d_24x128x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+24672)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1138024)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+23536)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23632)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23656)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23680)) /* Infos */
	);
	SSD_Monitor[81] = gap_cl_readhwtimer() - SSD_Monitor[81];
	/* Moving S182_Mul_scale, size 64 from HyperFlash at 1248976 to (size 64) L2 at 23536 using event 10 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1248976), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 23536), 64, 0, &UchanHF10);
	/* Moving S182_Infos, size 9 from HyperFlash at 1251424 to (size 9) L2 at 23600 using event 11 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251424), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 23600), 9, 0, &UchanHF11);
	SSD_Monitor[82] = gap_cl_readhwtimer();
	S225_Op_CONV_2D_0_83_trans_out0(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) ((SSD_tin_can_bottle_L2_Memory+132832) + 6000)) /* Out */
	);
	SSD_Monitor[82] = gap_cl_readhwtimer() - SSD_Monitor[82];
	/* Waiting completion of transfer of Featureextractormobilenetv2lay_950f395a using event 4 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4);
	/* Waiting completion of transfer of S182_Mul_scale using event 10 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF10);
	/* Waiting completion of transfer of S182_Mul_shift using event 5 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5);
	/* Waiting completion of transfer of S182_Infos using event 11 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF11);
	SSD_Monitor[83] = gap_cl_readhwtimer();
	S182_Conv2d_64x128x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+24672)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1040128)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+24032)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23648)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23536)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+24288)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23600)) /* Infos */
	);
	SSD_Monitor[83] = gap_cl_readhwtimer() - SSD_Monitor[83];
	/* Waiting completion of transfer of Featureextractormobilenetv2lay_493d8d4d using event 6 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF6);
	/* Waiting completion of transfer of S185_Mul_scale using event 7 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF7);
	/* Waiting completion of transfer of S185_Mul_shift using event 8 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF8);
	/* Waiting completion of transfer of S185_Infos using event 9 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF9);
	SSD_Monitor[84] = gap_cl_readhwtimer();
	S185_Conv2d_128x64x3x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23648)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+629760)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+70112)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+70624)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+70752)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+70992)) /* Infos */
	);
	SSD_Monitor[84] = gap_cl_readhwtimer() - SSD_Monitor[84];
	/* Moving Featureextractormobilenetv2lay_09dff4e3, size 128 from HyperFlash at 1244584 to (size 128) L2 at 27808 using event 4 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1244584), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 27808), 128, 0, &UchanHF4);
	/* Moving S188_Mul_scale, size 32 from HyperFlash at 1249860 to (size 32) L2 at 27936 using event 5 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249860), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 27936), 32, 0, &UchanHF5);
	/* Moving S188_Mul_shift, size 32 from HyperFlash at 1249892 to (size 32) L2 at 27968 using event 6 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249892), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 27968), 32, 0, &UchanHF6);
	/* Moving S188_Infos, size 9 from HyperFlash at 1251448 to (size 9) L2 at 28000 using event 7 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251448), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 28000), 9, 0, &UchanHF7);
	/* Moving Featureextractormobilenetv2lay_66bcd31b, size 256 from HyperFlash at 1238936 to (size 256) L2 at 29856 using event 8 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1238936), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 29856), 256, 0, &UchanHF8);
	/* Moving S191_Mul_scale, size 64 from HyperFlash at 1249104 to (size 64) L2 at 30184 using event 9 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249104), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 30184), 64, 0, &UchanHF9);
	/* Moving S191_Mul_shift, size 64 from HyperFlash at 1249168 to (size 64) L2 at 30248 using event 10 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1249168), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 30248), 64, 0, &UchanHF10);
	/* Moving S191_Infos, size 9 from HyperFlash at 1251460 to (size 9) L2 at 30352 using event 11 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251460), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 30352), 9, 0, &UchanHF11);
	/* Moving Boxpredictor_4boxencodingpredi_be9e9917, size 96 from HyperFlash at 1247304 to (size 96) L2 at 26768 using event 12 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1247304), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 26768), 96, 0, &UchanHF12);
	/* Moving S234_Mul_scale, size 24 from HyperFlash at 1250068 to (size 24) L2 at 26864 using event 13 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250068), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 26864), 24, 0, &UchanHF13);
	/* Moving S234_Mul_shift, size 24 from HyperFlash at 1250092 to (size 24) L2 at 26888 using event 14 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250092), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 26888), 24, 0, &UchanHF14);
	/* Moving S234_Infos, size 9 from HyperFlash at 1251616 to (size 9) L2 at 26912 using event 15 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251616), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 26912), 9, 0, &UchanHF15);
	/* Waiting completion of transfer of Boxpredictor_4classpredictorco using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	/* Waiting completion of transfer of S239_Mul_scale using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	/* Waiting completion of transfer of S239_Mul_shift using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	/* Waiting completion of transfer of S239_Infos using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	SSD_Monitor[85] = gap_cl_readhwtimer();
	S239_Conv2d_18x128x1x1_Hsigmoid(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1159400)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+70880)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23648)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+70952)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+70972)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+71004)) /* Infos */
	);
	SSD_Monitor[85] = gap_cl_readhwtimer() - SSD_Monitor[85];
	/* Moving Boxpredictor_5classpredictorco, size 72 from HyperFlash at 1248264 to (size 72) L2 at 30112 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1248264), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 30112), 72, 0, &UchanHF0);
	/* Moving S250_Mul_scale, size 18 from HyperFlash at 1250324 to (size 18) L2 at 30312 using event 1 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250324), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 30312), 18, 0, &UchanHF1);
	/* Moving S250_Mul_shift, size 18 from HyperFlash at 1250344 to (size 18) L2 at 30332 using event 2 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250344), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 30332), 18, 0, &UchanHF2);
	/* Moving S250_Infos, size 9 from HyperFlash at 1251652 to (size 9) L2 at 30364 using event 3 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251652), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 30364), 9, 0, &UchanHF3);
	SSD_Monitor[86] = gap_cl_readhwtimer();
	S240_Op_CONV_2D_0_89_trans_out0(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23648)), /* In */
		((signed char * __restrict__) ((SSD_tin_can_bottle_L2_Memory+139048) + 4608)) /* Out */
	);
	SSD_Monitor[86] = gap_cl_readhwtimer() - SSD_Monitor[86];
	/* Moving Boxpredictor_5boxencodingpredi_6990bed4, size 96 from HyperFlash at 1247400 to (size 96) L2 at 31208 using event 16 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1247400), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 31208), 96, 0, &UchanHF16);
	/* Moving S244_Mul_scale, size 24 from HyperFlash at 1250116 to (size 24) L2 at 31304 using event 17 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250116), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 31304), 24, 0, &UchanHF17);
	/* Moving S244_Mul_shift, size 24 from HyperFlash at 1250140 to (size 24) L2 at 31328 using event 18 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1250140), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 31328), 24, 0, &UchanHF18);
	/* Moving S244_Infos, size 9 from HyperFlash at 1251640 to (size 9) L2 at 31352 using event 19 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251640), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 31352), 9, 0, &UchanHF19);
	/* Waiting completion of transfer of Boxpredictor_4boxencodingpredi_be9e9917 using event 12 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF12);
	/* Waiting completion of transfer of S234_Mul_scale using event 13 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF13);
	/* Waiting completion of transfer of S234_Mul_shift using event 14 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF14);
	/* Waiting completion of transfer of S234_Infos using event 15 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF15);
	SSD_Monitor[87] = gap_cl_readhwtimer();
	S234_Conv2d_24x128x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1141096)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+26768)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23648)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+26864)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+26888)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+26912)) /* Infos */
	);
	SSD_Monitor[87] = gap_cl_readhwtimer() - SSD_Monitor[87];
	/* Moving S254_Ssd_norms, size 8 from HyperFlash at 1251672 to (size 8) L2 at 31372 using event 12 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1251672), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 31372), 8, 0, &UchanHF12);
	SSD_Monitor[88] = gap_cl_readhwtimer();
	S235_Op_CONV_2D_0_87_trans_out0(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23648)), /* In */
		((signed char * __restrict__) ((SSD_tin_can_bottle_L2_Memory+132832) + 6144)) /* Out */
	);
	SSD_Monitor[88] = gap_cl_readhwtimer() - SSD_Monitor[88];
	/* Waiting completion of transfer of Featureextractormobilenetv2lay_09dff4e3 using event 4 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF4);
	/* Waiting completion of transfer of S188_Mul_scale using event 5 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF5);
	/* Waiting completion of transfer of S188_Mul_shift using event 6 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF6);
	/* Waiting completion of transfer of S188_Infos using event 7 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF7);
	SSD_Monitor[89] = gap_cl_readhwtimer();
	S188_Conv2d_32x128x1x1_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In2 */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1127400)), /* In1 */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+27808)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23648)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+27936)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+27968)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+28000)) /* Infos */
	);
	SSD_Monitor[89] = gap_cl_readhwtimer() - SSD_Monitor[89];
	/* Waiting completion of transfer of Featureextractormobilenetv2lay_66bcd31b using event 8 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF8);
	/* Waiting completion of transfer of S191_Mul_scale using event 9 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF9);
	/* Waiting completion of transfer of S191_Mul_shift using event 10 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF10);
	/* Waiting completion of transfer of S191_Infos using event 11 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF11);
	SSD_Monitor[90] = gap_cl_readhwtimer();
	S191_Conv2d_64x32x1x3_Relu6(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23648)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1097544)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+29856)), /* Bias */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+30184)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+30248)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+30352)) /* Infos */
	);
	SSD_Monitor[90] = gap_cl_readhwtimer() - SSD_Monitor[90];
	/* Waiting completion of transfer of Boxpredictor_5classpredictorco using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	/* Waiting completion of transfer of S250_Mul_scale using event 1 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF1);
	/* Waiting completion of transfer of S250_Mul_shift using event 2 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF2);
	/* Waiting completion of transfer of S250_Infos using event 3 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF3);
	SSD_Monitor[91] = gap_cl_readhwtimer();
	S250_Conv2d_18x64x1x1_Hsigmoid(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1205480)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+30112)), /* Bias */
		((signed char * __restrict__) ((SSD_tin_can_bottle_L2_Memory+139048) + 4644)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+30312)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+30332)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+30364)) /* Infos */
	);
	SSD_Monitor[91] = gap_cl_readhwtimer() - SSD_Monitor[91];
	/* Moving Anchors, size 6216 from HyperFlash at 1048320 to (size 6216) L2 at 23456 using event 0 */
	AT_HYPERFLASH_FS_CL_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) SSD_tin_can_bottle_L3_Flash + 1048320), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) SSD_tin_can_bottle_L2_Memory + 23456), 6216, 0, &UchanHF0);
	/* Waiting completion of transfer of Boxpredictor_5boxencodingpredi_6990bed4 using event 16 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF16);
	/* Waiting completion of transfer of S244_Mul_scale using event 17 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF17);
	/* Waiting completion of transfer of S244_Mul_shift using event 18 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF18);
	/* Waiting completion of transfer of S244_Infos using event 19 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF19);
	SSD_Monitor[92] = gap_cl_readhwtimer();
	S244_Conv2d_24x64x1x1(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23392)), /* In */
		((signed char * __restrict__) (SSD_tin_can_bottle_L3_Flash+1197032)), /* Filter */
		((signed int * __restrict__) (SSD_tin_can_bottle_L2_Memory+31208)), /* Bias */
		((signed char * __restrict__) ((SSD_tin_can_bottle_L2_Memory+132832) + 6192)), /* Out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+31304)), /* Scale */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+31328)), /* ScaleN */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+31352)) /* Infos */
	);
	SSD_Monitor[92] = gap_cl_readhwtimer() - SSD_Monitor[92];
	/* Waiting completion of transfer of Anchors using event 0 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF0);
	/* Waiting completion of transfer of S254_Ssd_norms using event 12 */
	AT_HYPERFLASH_FS_CL_WAIT(&HyperFlash, &UchanHF12);
	SSD_Monitor[93] = gap_cl_readhwtimer();
	S254_Op_CUSTOM_0_99(
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+132832)), /* boxes_in */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+139048)), /* classes_in */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23456)), /* anchors_in */
		((signed short * __restrict__) Output_1), /* bbox_out */
		((signed char * __restrict__) Output_2), /* class_out */
		((signed char * __restrict__) Output_3), /* scores_out */
		((unsigned char * __restrict__) (SSD_tin_can_bottle_L2_Memory+23384)), /* in_scales */
		((signed char * __restrict__) (SSD_tin_can_bottle_L2_Memory+31372)) /* in_norms */
	);
	SSD_Monitor[93] = gap_cl_readhwtimer() - SSD_Monitor[93];
	return 0;
}
#pragma GCC pop_options
