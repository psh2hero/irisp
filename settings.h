#define _SIMULATION_

#define IRWID       320
#define IRHGT       240

#define NO_DETECTOR 0
#define ULIS_QVGA   1
#define DRS_IR      2
#define I3_IR       3
#define UE_IR       4

#ifdef _SIMULATION_
#define NUC_FRAME_NUM   1
#else
#define NUC_FRAME_NUM   100
#endif

#define SHUTTER_TIME    1000 * 60 * 60 /*1 hour*/ 
#define SHUTTER_TEMP    200 /* 2 degree */

#ifdef 	_SIMULATION_
#define MANUFACTURING   0   // 0 - no manufacture, 1 - manufacture, 2 - user select
//#define SAVE_RAW_IMAGE
//#define SAVE_NUC_IMAGE
//#define SAVE_DPC_IMAGE
//#define SAVE_2DNR_IMAGE
#endif

// Simple DPC
#define DPC_LOW_TH  0.7
#define DPC_HIGH_TH 1.5

// Auto DPC
#define K_MIN_TH		100
#define K_MAX_TH		500
#define DIR_TH_RATIO	20 /* 20 % */
#define DIR_TH_OFFSET	20

