/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */

/* �O���֐���` */
#include "WinMain.h"

/* �O���ϐ���` */

/* �����֐���` */
#include "dx7ParamCtrl.h"

//#define DEBUG_DISP_STATIC_STRING

typedef struct
{
    HINSTANCE hInstance;
    PTSTR     szAppName;
    BOOL      bInitConfig;
} S_DX7_PARM_CTRL_DATA;

static S_DX7_PARM_CTRL_DATA dx7ParamCtrlData;

static BOOL dx7ParamCtrlCreate( HWND hwnd, DX7_PARAM_CTRL_GROUP_ID groupId, DX7_PARAM_CTRL_ID startId, DX7_PARAM_CTRL_ID endId );
/* �����ϐ���` */

static char *strTblOnOff[2] = {"OFF","ON"};
static char *strTblAlgorithm[32] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32"};
static char *strTblLfoWave[6]   = {"TRI","SAW DWN","SAW UP","SQU","SINE","S/H"};
static char *strTblTranspose[49]  = {"C 1","C#1","D 1","D#1","E 1","F 1","F#1","G 1","G#1","A 1","A#1","B 1","C 2","C#2","D 2","D#2","E 2","F 2","F#2","G 2","G#2","A 2","A#2","B 2","C 3","C#3","D 3","D#3","E 3","F 3","F#3","G 3","G#3","A 3","A#3","B 3","C 4","C#4","D 4","D#4","E 4","F 4","F#4","G 4","G#4","A 4","A#4","B 4","C 5"};
static char *strTblFreqMode[2] = {"FIXED","RATIO"};
static char *strTblBreakPoint[100]  = {"A -1","A#-1","B -1","C 0","C#0","D 0","D#0","E 0","F 0","F#0","G 0","G#0","A 0","A#0","B 0","C 1","C#1","D 1","D#1","E 1","F 1","F#1","G 1","G#1","A 1","A#1","B 1","C 2","C#2","D 2","D#2","E 2","F 2","F#2","G 2","G#2","A 2","A#2","B 2","C 3","C#3","D 3","D#3","E 3","F 3","F#3","G 3","G#3","A 3","A#3","B 3","C 4","C#4","D 4","D#4","E 4","F 4","F#4","G 4","G#4","A 4","A#4","B 4","C 5","C#5","D 5","D#5","E 5","F 5","F#5","G 5","G#5","A 5","A#5","B 5","C 6","C#6","D 6","D#6","E 6","F 6","F#6","G 6","G#6","A 6","A#6","B 6","C 7","C#7","D 7","D#7","E 7","F 7","F#7","G 7","G#7","A 7","A#7","B 7","C 8"};
static char *strTblCurve[4] = {"-LIN","-EXP","+EXP","+LIN"};

#if 0
static char *strTblPolyMono[2] = {"POLY","MONO"};
static char *strTblPortamentoMode[2] = {"Full","Fing"};
static char *strTblDetune[7] = {"-3","-2","-1","0","+1","+2","+3"};
static char *strTblFrequency[64] = {"0.50","0.71","0.78","0.87","1.00","1.41","1.57","1.73","2.00","2.82","3.00","3.14","3.46","4.00","4.24","4.71","5.00","5.19","5.65","6.00","6.28","6.92","7.00","7.07","7.85","8.00","8.48","8.65","9.00","9.42","9.89","10.00","10.38","10.99","11.00","11.30","12.00","12.11","12.56","12.72","13.00","13.84","14.00","14.10","14.13","15.00","15.55","15.57","15.70","16.96","17.27","17.30","18.37","18.84","19.03","19.78","20.41","20.76","21.20","21.98","22.49","23.55","24.22","25.95"};
#endif

/* X���� */
#define PARAM_STATIC_X_BASE_POS   5
#define PARAM_DATA_X_BASE_POS     5
#define PARAM_DATA_WIDTH         50
#define PARAM_STATIC_WIDTH       70
#define PARAM_WRAP_X_UNIT        70

/* Y���� */
#define PARAM_STATIC_Y_BASE_POS 125
#define PARAM_DATA_Y_BASE_POS   120
#define PARAM_DATA_Y_UNIT        10

#define PARAM_STATIC_HEIGHT      15
#define PARAM_DATA_HEIGHT       300
#define PARAM_BUTTON_HEIGHT      20
#define PARAM_EDIT_HEIGHT        20

#define ROW_POS_EDIT00   0 /* PATCHNAME                */
#define ROW_POS_STATIC0  1 /*           LFO            */
#define ROW_POS_STATIC1  2 /* ALGORITHM                */
#define ROW_POS_EDIT01   3 /* [      ]      [    ]     */
/*      ROW_POS_EDIT02                      [    ]     */
/*      ROW_POS_EDIT03                      [    ]     */
/*      ROW_POS_EDIT04                      [    ]     */
/*      ROW_POS_EDIT05                      [    ]     */
/*      ROW_POS_EDIT06                      [    ]     */
#define ROW_POS_STATIC2  4 /* MODULATION SENSITIVITY   */
#define ROW_POS_STATIC3  5 /* PITCH      AMPLITUDE     */
#define ROW_POS_EDIT07   6 /* OSCILLATOR1   [    ]     */
#define ROW_POS_EDIT08   7 /* OSCILLATOR2   [    ]     */
#define ROW_POS_EDIT09   8 /* OSCILLATOR3   [    ]     */
#define ROW_POS_EDIT10   9 /* OSCILLATOR4   [    ]     */
#define ROW_POS_EDIT11  10 /* OSCILLATOR5   [    ]     */
#define ROW_POS_EDIT12  11 /* OSCILLATOR6   [    ]     */
#define ROW_POS_STATIC4 12 /*                          */
#define ROW_POS_STATIC5 13 /*                          */
#define ROW_POS_EDIT13  14 /* OSCILLATOR1   [    ]     */
#define ROW_POS_EDIT14  15 /* OSCILLATOR2   [    ]     */
#define ROW_POS_EDIT15  16 /* OSCILLATOR3   [    ]     */
#define ROW_POS_EDIT16  17 /* OSCILLATOR4   [    ]     */
#define ROW_POS_EDIT17  18 /* OSCILLATOR5   [    ]     */
#define ROW_POS_EDIT18  19 /* OSCILLATOR6   [    ]     */
#define ROW_POS_STATIC6 20 /*                          */
#define ROW_POS_STATIC7 21 /*                          */
#define ROW_POS_EDIT19  22 /* [      ]                 */

#define COLPOS00  0
#define COLPOS01  1
#define COLPOS02  2
#define COLPOS03  3
#define COLPOS04  4
#define COLPOS05  5
#define COLPOS06  6
#define COLPOS07  7
#define COLPOS08  8
#define COLPOS09  9
#define COLPOS10 10
#define COLPOS11 11
#define COLPOS12 12
#define COLPOS13 13
#define COLPOS14 14
#define COLPOS15 15
#define COLPOS16 16
#define COLPOS17 17
#define COLPOS18 18
#define COLPOS19 19
#define COLPOS20 20
#define COLPOS21 21

enum
{
   PCT_COMBO ,
   PCT_EDIT  ,
   PCT_BUTTON,
   PCT_STATIC,
   PCT_MAX
};
typedef BYTE DX7_PARAM_CTRL_TYPE;

typedef struct
{
    DX7_PARAM_CTRL_TYPE type         ;
    PTSTR           strText      ; /* �E�B���h�E�e�L�X�g                         */
    INT             withTextLen  ; /* �����Ƀe�L�X�g��ǉ�����ꍇ�̒���(0�̎��ǉ����Ȃ�) */
    BOOL            bEnable      ;
    INT             minValue     ;
    INT             maxValue     ;
    char **         ptstrDataDisp;
    INT             addWidth     ;
    INT             rowPos       ;/* �s */
    INT             colPos       ;/* �� */
    INT             yPosAdj      ;
} S_DX7_PARAM_CTRL;

static S_DX7_PARAM_CTRL paramListTbl[DX7_PARAM_CTRL_MAX] =
{/*  type     ,strText                               ,wTLen,bEnable  ,minValue,maxValue,ptstrDataDisp   ,addWidth,rowPos          ,colPos  ,yPosAdj*/       /*strFullText                                    */
    {PCT_COMBO ,TEXT("RATE1"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS09,0   },      /* 0     OP6 EG RATE 1                            */
    {PCT_COMBO ,TEXT("RATE2"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS10,0   },      /* 1     OP6 EG RATE 2                            */
    {PCT_COMBO ,TEXT("RATE3"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS11,0   },      /* 2     OP6 EG RATE 3                            */
    {PCT_COMBO ,TEXT("RATE4"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS12,0   },      /* 3     OP6 EG RATE 4                            */
    {PCT_COMBO ,TEXT("LEVEL1"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS13,0   },      /* 4     OP6 EG LEVEL 1                           */
    {PCT_COMBO ,TEXT("LEVEL2"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS14,0   },      /* 5     OP6 EG LEVEL 2                           */
    {PCT_COMBO ,TEXT("LEVEL3"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS15,0   },      /* 6     OP6 EG LEVEL 3                           */
    {PCT_COMBO ,TEXT("LEVEL4"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS16,0   },      /* 7     OP6 EG LEVEL 4                           */
    {PCT_COMBO ,TEXT("BREAK POINT"                   ),0    ,TRUE    ,0       , 99     , strTblBreakPoint    ,  0, ROW_POS_EDIT18 ,COLPOS01,0   },      /* 8     OP6 BREAK POINT                          */
    {PCT_COMBO ,TEXT("L DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT18 ,COLPOS04,0   },      /* 9     OP6 LEFT DEPTH                           */
    {PCT_COMBO ,TEXT("R DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT18 ,COLPOS05,0   },      /*10     OP6 RIGHT DEPTH                          */
    {PCT_COMBO ,TEXT("L CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT18 ,COLPOS02,0   },      /*11     OP6 LEFT CURVE                           */
    {PCT_COMBO ,TEXT("R CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT18 ,COLPOS03,0   },      /*12     OP6 RIGHT CURVE                          */
    {PCT_COMBO ,TEXT("KEY BOARD RATE SCALING"        ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT18 ,COLPOS06,0   },      /*13     OP6 KEY BOARD RATE SCALING               */
    {PCT_COMBO ,TEXT("MOD AMP"                       ),0    ,TRUE    ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS02,0   },      /*14     OP6 MOD SENSITIVITY AMPLITUDE            */
    {PCT_COMBO ,TEXT("VELOCITY SENS"                 ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT18 ,COLPOS08,0   },      /*15     OP6 OPERATOR KEY VELOCITY SENSITIVITY    */
    {PCT_COMBO ,TEXT("OUTPUT LEVEL"                  ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT18 ,COLPOS07,0   },      /*16     OP6 OPERATOR OUTPUT LEVEL                */
    {PCT_COMBO ,TEXT("MODE"                          ),0    ,TRUE    ,0       ,  1     , strTblFreqMode      ,  0, ROW_POS_EDIT12 ,COLPOS04,0   },      /*17     OP6 OSILLATOR MODE                       */
    {PCT_COMBO ,TEXT("FREQ COARSE"                   ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS06,0   },      /*18     OP6 OSILLATOR FREQUENCY COARSE           */
    {PCT_COMBO ,TEXT("FREQ  FINE"                    ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS07,0   },      /*19     OP6 OSILLATOR FREQUENCY FINE             */
    {PCT_COMBO ,TEXT("DETUNE"                        ),0    ,TRUE    ,0       , 14     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS08,0   },      /*20     OP6 DETUNE                               */
    {PCT_COMBO ,TEXT("RATE1"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS09,0   },      /*21     OP5 EG RATE 1                            */
    {PCT_COMBO ,TEXT("RATE2"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS10,0   },      /*22     OP5 EG RATE 2                            */
    {PCT_COMBO ,TEXT("RATE3"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS11,0   },      /*23     OP5 EG RATE 3                            */
    {PCT_COMBO ,TEXT("RATE4"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS12,0   },      /*24     OP5 EG RATE 4                            */
    {PCT_COMBO ,TEXT("LEVEL1"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS13,0   },      /*25     OP5 EG LEVEL 1                           */
    {PCT_COMBO ,TEXT("LEVEL2"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS14,0   },      /*26     OP5 EG LEVEL 2                           */
    {PCT_COMBO ,TEXT("LEVEL3"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS15,0   },      /*27     OP5 EG LEVEL 3                           */
    {PCT_COMBO ,TEXT("LEVEL4"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS16,0   },      /*28     OP5 EG LEVEL 4                           */
    {PCT_COMBO ,TEXT("BREAK POINT"                   ),0    ,TRUE    ,0       , 99     , strTblBreakPoint    ,  0, ROW_POS_EDIT17 ,COLPOS01,0   },      /*29     OP5 BREAK POINT                          */
    {PCT_COMBO ,TEXT("L DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT17 ,COLPOS04,0   },      /*30     OP5 LEFT DEPTH                           */
    {PCT_COMBO ,TEXT("R DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT17 ,COLPOS05,0   },      /*31     OP5 RIGHT DEPTH                          */
    {PCT_COMBO ,TEXT("L CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT17 ,COLPOS02,0   },      /*32     OP5 LEFT CURVE                           */
    {PCT_COMBO ,TEXT("R CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT17 ,COLPOS03,0   },      /*33     OP5 RIGHT CURVE                          */
    {PCT_COMBO ,TEXT("KEY BOARD RATE SCALING"        ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT17 ,COLPOS06,0   },      /*34     OP5 KEY BOARD RATE SCALING               */
    {PCT_COMBO ,TEXT("MOD AMP"                       ),0    ,TRUE    ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS02,0   },      /*35     OP5 MOD SENSITIVITY AMPLITUDE            */
    {PCT_COMBO ,TEXT("VELOCITY SENS"                 ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT17 ,COLPOS08,0   },      /*36     OP5 OPERATOR KEY VELOCITY SENSITIVITY    */
    {PCT_COMBO ,TEXT("OUTPUT LEVEL"                  ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT17 ,COLPOS07,0   },      /*37     OP5 OPERATOR OUTPUT LEVEL                */
    {PCT_COMBO ,TEXT("MODE"                          ),0    ,TRUE    ,0       ,  1     , strTblFreqMode      ,  0, ROW_POS_EDIT11 ,COLPOS04,0   },      /*38     OP5 OSILLATOR MODE                       */
    {PCT_COMBO ,TEXT("FREQ COARSE"                   ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS06,0   },      /*39     OP5 OSILLATOR FREQUENCY COARSE           */
    {PCT_COMBO ,TEXT("FREQ  FINE"                    ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS07,0   },      /*40     OP5 OSILLATOR FREQUENCY FINE             */
    {PCT_COMBO ,TEXT("DETUNE"                        ),0    ,TRUE    ,0       , 14     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS08,0   },      /*41     OP5 DETUNE                               */
    {PCT_COMBO ,TEXT("RATE1"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS09,0   },      /*42     OP4 EG RATE 1                            */
    {PCT_COMBO ,TEXT("RATE2"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS10,0   },      /*43     OP4 EG RATE 2                            */
    {PCT_COMBO ,TEXT("RATE3"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS11,0   },      /*44     OP4 EG RATE 3                            */
    {PCT_COMBO ,TEXT("RATE4"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS12,0   },      /*45     OP4 EG RATE 4                            */
    {PCT_COMBO ,TEXT("LEVEL1"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS13,0   },      /*46     OP4 EG LEVEL 1                           */
    {PCT_COMBO ,TEXT("LEVEL2"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS14,0   },      /*47     OP4 EG LEVEL 2                           */
    {PCT_COMBO ,TEXT("LEVEL3"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS15,0   },      /*48     OP4 EG LEVEL 3                           */
    {PCT_COMBO ,TEXT("LEVEL4"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS16,0   },      /*49     OP4 EG LEVEL 4                           */
    {PCT_COMBO ,TEXT("BREAK POINT"                   ),0    ,TRUE    ,0       , 99     , strTblBreakPoint    ,  0, ROW_POS_EDIT16 ,COLPOS01,0   },      /*50     OP4 BREAK POINT                          */
    {PCT_COMBO ,TEXT("L DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT16 ,COLPOS04,0   },      /*51     OP4 LEFT DEPTH                           */
    {PCT_COMBO ,TEXT("R DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT16 ,COLPOS05,0   },      /*52     OP4 RIGHT DEPTH                          */
    {PCT_COMBO ,TEXT("L CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT16 ,COLPOS02,0   },      /*53     OP4 LEFT CURVE                           */
    {PCT_COMBO ,TEXT("R CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT16 ,COLPOS03,0   },      /*54     OP4 RIGHT CURVE                          */
    {PCT_COMBO ,TEXT("KEY BOARD RATE SCALING"        ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT16 ,COLPOS06,0   },      /*55     OP4 KEY BOARD RATE SCALING               */
    {PCT_COMBO ,TEXT("MOD AMP"                       ),0    ,TRUE    ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS02,0   },      /*56     OP4 MOD SENSITIVITY AMPLITUDE            */
    {PCT_COMBO ,TEXT("VELOCITY SENS"                 ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT16 ,COLPOS08,0   },      /*57     OP4 OPERATOR KEY VELOCITY SENSITIVITY    */
    {PCT_COMBO ,TEXT("OUTPUT LEVEL"                  ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT16 ,COLPOS07,0   },      /*58     OP4 OPERATOR OUTPUT LEVEL                */
    {PCT_COMBO ,TEXT("MODE"                          ),0    ,TRUE    ,0       ,  1     , strTblFreqMode      ,  0, ROW_POS_EDIT10 ,COLPOS04,0   },      /*59     OP4 OSILLATOR MODE                       */
    {PCT_COMBO ,TEXT("FREQ COARSE"                   ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS06,0   },      /*60     OP4 OSILLATOR FREQUENCY COARSE           */
    {PCT_COMBO ,TEXT("FREQ  FINE"                    ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS07,0   },      /*61     OP4 OSILLATOR FREQUENCY FINE             */
    {PCT_COMBO ,TEXT("DETUNE"                        ),0    ,TRUE    ,0       , 14     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS08,0   },      /*62     OP4 DETUNE                               */
    {PCT_COMBO ,TEXT("RATE1"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS09,0   },      /*63     OP3 EG RATE 1                            */
    {PCT_COMBO ,TEXT("RATE2"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS10,0   },      /*64     OP3 EG RATE 2                            */
    {PCT_COMBO ,TEXT("RATE3"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS11,0   },      /*65     OP3 EG RATE 3                            */
    {PCT_COMBO ,TEXT("RATE4"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS12,0   },      /*66     OP3 EG RATE 4                            */
    {PCT_COMBO ,TEXT("LEVEL1"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS13,0   },      /*67     OP3 EG LEVEL 1                           */
    {PCT_COMBO ,TEXT("LEVEL2"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS14,0   },      /*68     OP3 EG LEVEL 2                           */
    {PCT_COMBO ,TEXT("LEVEL3"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS15,0   },      /*69     OP3 EG LEVEL 3                           */
    {PCT_COMBO ,TEXT("LEVEL4"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS16,0   },      /*70     OP3 EG LEVEL 4                           */
    {PCT_COMBO ,TEXT("BREAK POINT"                   ),0    ,TRUE    ,0       , 99     , strTblBreakPoint    ,  0, ROW_POS_EDIT15 ,COLPOS01,0   },      /*71     OP3 BREAK POINT                          */
    {PCT_COMBO ,TEXT("L DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT15 ,COLPOS04,0   },      /*72     OP3 LEFT DEPTH                           */
    {PCT_COMBO ,TEXT("R DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT15 ,COLPOS05,0   },      /*73     OP3 RIGHT DEPTH                          */
    {PCT_COMBO ,TEXT("L CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT15 ,COLPOS02,0   },      /*74     OP3 LEFT CURVE                           */
    {PCT_COMBO ,TEXT("R CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT15 ,COLPOS03,0   },      /*75     OP3 RIGHT CURVE                          */
    {PCT_COMBO ,TEXT("KEY BOARD RATE SCALING"        ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT15 ,COLPOS06,0   },      /*76     OP3 KEY BOARD RATE SCALING               */
    {PCT_COMBO ,TEXT("MOD AMP"                       ),0    ,TRUE    ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS02,0   },      /*77     OP3 MOD SENSITIVITY AMPLITUDE            */
    {PCT_COMBO ,TEXT("VELOCITY SENS"                 ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT15 ,COLPOS08,0   },      /*78     OP3 OPERATOR KEY VELOCITY SENSITIVITY    */
    {PCT_COMBO ,TEXT("OUTPUT LEVEL"                  ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT15 ,COLPOS07,0   },      /*79     OP3 OPERATOR OUTPUT LEVEL                */
    {PCT_COMBO ,TEXT("MODE"                          ),0    ,TRUE    ,0       ,  1     , strTblFreqMode      ,  0, ROW_POS_EDIT09 ,COLPOS04,0   },      /*80     OP3 OSILLATOR MODE                       */
    {PCT_COMBO ,TEXT("FREQ COARSE"                   ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS06,0   },      /*81     OP3 OSILLATOR FREQUENCY COARSE           */
    {PCT_COMBO ,TEXT("FREQ  FINE"                    ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS07,0   },      /*82     OP3 OSILLATOR FREQUENCY FINE             */
    {PCT_COMBO ,TEXT("DETUNE"                        ),0    ,TRUE    ,0       , 14     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS08,0   },      /*83     OP3 DETUNE                               */
    {PCT_COMBO ,TEXT("RATE1"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS09,0   },      /*84     OP2 EG RATE 1                            */
    {PCT_COMBO ,TEXT("RATE2"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS10,0   },      /*85     OP2 EG RATE 2                            */
    {PCT_COMBO ,TEXT("RATE3"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS11,0   },      /*86     OP2 EG RATE 3                            */
    {PCT_COMBO ,TEXT("RATE4"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS12,0   },      /*87     OP2 EG RATE 4                            */
    {PCT_COMBO ,TEXT("LEVEL1"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS13,0   },      /*88     OP2 EG LEVEL 1                           */
    {PCT_COMBO ,TEXT("LEVEL2"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS14,0   },      /*89     OP2 EG LEVEL 2                           */
    {PCT_COMBO ,TEXT("LEVEL3"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS15,0   },      /*90     OP2 EG LEVEL 3                           */
    {PCT_COMBO ,TEXT("LEVEL4"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS16,0   },      /*91     OP2 EG LEVEL 4                           */
    {PCT_COMBO ,TEXT("BREAK POINT"                   ),0    ,TRUE    ,0       , 99     , strTblBreakPoint    ,  0, ROW_POS_EDIT14 ,COLPOS01,0   },      /*92     OP2 BREAK POINT                          */
    {PCT_COMBO ,TEXT("L DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT14 ,COLPOS04,0   },      /*93     OP2 LEFT DEPTH                           */
    {PCT_COMBO ,TEXT("R DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT14 ,COLPOS05,0   },      /*94     OP2 RIGHT DEPTH                          */
    {PCT_COMBO ,TEXT("L CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT14 ,COLPOS02,0   },      /*95     OP2 LEFT CURVE                           */
    {PCT_COMBO ,TEXT("R CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT14 ,COLPOS03,0   },      /*96     OP2 RIGHT CURVE                          */
    {PCT_COMBO ,TEXT("KEY BOARD RATE SCALING"        ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT14 ,COLPOS06,0   },      /*97     OP2 KEY BOARD RATE SCALING               */
    {PCT_COMBO ,TEXT("MOD AMP"                       ),0    ,TRUE    ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS02,0   },      /*98     OP2 MOD SENSITIVITY AMPLITUDE            */
    {PCT_COMBO ,TEXT("VELOCITY SENS"                 ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT14 ,COLPOS08,0   },      /*99     OP2 OPERATOR KEY VELOCITY SENSITIVITY    */
    {PCT_COMBO ,TEXT("OUTPUT LEVEL"                  ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT14 ,COLPOS07,0   },      /*100    OP2 OPERATOR OUTPUT LEVEL                */
    {PCT_COMBO ,TEXT("MODE"                          ),0    ,TRUE    ,0       ,  1     , strTblFreqMode      ,  0, ROW_POS_EDIT08 ,COLPOS04,0   },      /*101    OP2 OSILLATOR MODE                       */
    {PCT_COMBO ,TEXT("FREQ COARSE"                   ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS06,0   },      /*102    OP2 OSILLATOR FREQUENCY COARSE           */
    {PCT_COMBO ,TEXT("FREQ  FINE"                    ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS07,0   },      /*103    OP2 OSILLATOR FREQUENCY FINE             */
    {PCT_COMBO ,TEXT("DETUNE"                        ),0    ,TRUE    ,0       , 14     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS08,0   },      /*104    OP2 DETUNE                               */
    {PCT_COMBO ,TEXT("RATE1"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS09,0   },      /*105    OP1 EG RATE 1                            */
    {PCT_COMBO ,TEXT("RATE2"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS10,0   },      /*106    OP1 EG RATE 2                            */
    {PCT_COMBO ,TEXT("RATE3"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS11,0   },      /*107    OP1 EG RATE 3                            */
    {PCT_COMBO ,TEXT("RATE4"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS12,0   },      /*108    OP1 EG RATE 4                            */
    {PCT_COMBO ,TEXT("LEVEL1"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS13,0   },      /*109    OP1 EG LEVEL 1                           */
    {PCT_COMBO ,TEXT("LEVEL2"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS14,0   },      /*110    OP1 EG LEVEL 2                           */
    {PCT_COMBO ,TEXT("LEVEL3"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS15,0   },      /*111    OP1 EG LEVEL 3                           */
    {PCT_COMBO ,TEXT("LEVEL4"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS16,0   },      /*112    OP1 EG LEVEL 4                           */
    {PCT_COMBO ,TEXT("BREAK POINT"                   ),0    ,TRUE    ,0       , 99     , strTblBreakPoint    ,  0, ROW_POS_EDIT13 ,COLPOS01,0   },      /*113    OP1 BREAK POINT                          */
    {PCT_COMBO ,TEXT("L DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS04,0   },      /*114    OP1 LEFT DEPTH                           */
    {PCT_COMBO ,TEXT("R DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS05,0   },      /*115    OP1 RIGHT DEPTH                          */
    {PCT_COMBO ,TEXT("L CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT13 ,COLPOS02,0   },      /*116    OP1 LEFT CURVE                           */
    {PCT_COMBO ,TEXT("R CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT13 ,COLPOS03,0   },      /*117    OP1 RIGHT CURVE                          */
    {PCT_COMBO ,TEXT("KEY BOARD RATE SCALING"        ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS06,0   },      /*118    OP1 KEY BOARD RATE SCALING               */
    {PCT_COMBO ,TEXT("MOD AMP"                       ),0    ,TRUE    ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS02,0   },      /*119    OP1 MOD SENSITIVITY AMPLITUDE            */
    {PCT_COMBO ,TEXT("VELOCITY SENS"                 ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS08,0   },      /*120    OP1 OPERATOR KEY VELOCITY SENSITIVITY    */
    {PCT_COMBO ,TEXT("OUTPUT LEVEL"                  ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS07,0   },      /*121    OP1 OPERATOR OUTPUT LEVEL                */
    {PCT_COMBO ,TEXT("MODE"                          ),0    ,TRUE    ,0       ,  1     , strTblFreqMode      ,  0, ROW_POS_EDIT07 ,COLPOS04,0   },      /*122    OP1 OSILLATOR MODE                       */
    {PCT_COMBO ,TEXT("FREQ COARSE"                   ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS06,0   },      /*123    OP1 OSILLATOR FREQUENCY COARSE           */
    {PCT_COMBO ,TEXT("FREQ  FINE"                    ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS07,0   },      /*124    OP1 OSILLATOR FREQUENCY FINE             */
    {PCT_COMBO ,TEXT("DETUNE"                        ),0    ,TRUE    ,0       , 14     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS08,0   },      /*125    OP1 DETUNE                               */
    {PCT_COMBO ,TEXT("PEG R1"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS09,0   },      /*126        PITCH EG RATE 1                      */
    {PCT_COMBO ,TEXT("PEG R2"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS10,0   },      /*127        PITCH EG RATE 2                      */
    {PCT_COMBO ,TEXT("PEG R3"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS11,0   },      /*128        PITCH EG RATE 3                      */
    {PCT_COMBO ,TEXT("PEG R4"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS12,0   },      /*129        PITCH EG RATE 4                      */
    {PCT_COMBO ,TEXT("PEG L1"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS13,0   },      /*130        PITCH EG LEVEL 1                     */
    {PCT_COMBO ,TEXT("PEG L2"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS14,0   },      /*131        PITCH EG LEVEL 2                     */
    {PCT_COMBO ,TEXT("PEG L3"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS15,0   },      /*132        PITCH EG LEVEL 3                     */
    {PCT_COMBO ,TEXT("PEG L4"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS16,0   },      /*133        PITCH EG LEVEL 4                     */
    {PCT_COMBO ,TEXT("ALGORITHM SELECT"              ),0    ,TRUE    ,0       , 31     , strTblAlgorithm     ,  0, ROW_POS_EDIT01 ,COLPOS01,0   },      /*134        ALGORITHM SELECT                     */
    {PCT_COMBO ,TEXT("FEED BACK"                     ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT01 ,COLPOS02,0   },      /*135        FEED BACK                            */
    {PCT_COMBO ,TEXT("SYNC"                          ),0    ,TRUE    ,0       ,  1     , strTblOnOff         ,  0, ROW_POS_EDIT07 ,COLPOS05,0   },      /*136        OSILLATOR SYNC                       */
    {PCT_COMBO ,TEXT("SPEED"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT01 ,COLPOS04,0   },      /*137        LFO SPEED                            */
    {PCT_COMBO ,TEXT("DELAY"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT01 ,COLPOS05,0   },      /*138        LFO DELAY                            */
    {PCT_COMBO ,TEXT("PMD"                           ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT01 ,COLPOS06,0   },      /*139        LFO PMD                              */
    {PCT_COMBO ,TEXT("AMD"                           ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT01 ,COLPOS07,0   },      /*140        LFO AMD                              */
    {PCT_COMBO ,TEXT("SYNC"                          ),0    ,TRUE    ,0       ,  1     , strTblOnOff         ,  0, ROW_POS_EDIT01 ,COLPOS08,0   },      /*141        LFO SYNC                             */
    {PCT_COMBO ,TEXT("WAVE"                          ),0    ,TRUE    ,0       ,  5     , strTblLfoWave       ,  0, ROW_POS_EDIT01 ,COLPOS03,0   },      /*142        LFO WAVE                             */
    {PCT_COMBO ,TEXT("PITCH"                         ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS01,0   },      /*143        MOD SENSITIVITY PITCH                */
    {PCT_COMBO ,TEXT("TRANSPOSE"                     ),0    ,TRUE    ,0       , 48     , strTblTranspose     ,  0, ROW_POS_EDIT19 ,COLPOS01,0   },      /*144        TRANSPOSE                            */
    {PCT_EDIT  ,TEXT("PATCHNAME"                     ),0    ,TRUE    ,0       ,127     , NULL                , 50, ROW_POS_EDIT00 ,COLPOS01,0   },      /*145-154    VOICE NAME                           */
#if 0
    {PCT_COMBO ,TEXT("OPERATOR ON/OFF"               ),0    ,FALSE   ,0       ,127     , NULL                ,  0, ROW_POS_EDIT19 ,COLPOS02,0   },      /*155        OPERATOR ON/OFF                      */
#endif
    {PCT_STATIC,TEXT("PATCHNAME"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT00 ,COLPOS00,0   },      /**/
    {PCT_STATIC,TEXT("LFO"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS03,5   },      /**/
    {PCT_STATIC,TEXT("MODULATION SENSITVITY"         ),0    ,TRUE    ,0       ,0       , NULL                ,200, ROW_POS_STATIC2,COLPOS01,5   },      /**/
    {PCT_STATIC,TEXT("ALGORITHM"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS01,0   },      /**/
    {PCT_STATIC,TEXT("FEEDBACK"                      ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS02,0   },      /**/
    {PCT_STATIC,TEXT("WAVE"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS03,0   },      /**/
    {PCT_STATIC,TEXT("SPEED"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS04,0   },      /**/
    {PCT_STATIC,TEXT("DELAY"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS05,0   },      /**/
    {PCT_STATIC,TEXT("PMD"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS06,0   },      /**/
    {PCT_STATIC,TEXT("AMD"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS07,0   },      /**/
    {PCT_STATIC,TEXT("SYNC"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS08,0   },      /**/
    {PCT_STATIC,TEXT("PITCH"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS01,0   },      /**/
    {PCT_STATIC,TEXT("AMPLITUDE"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS02,0   },      /**/
    {PCT_STATIC,TEXT("OPERATOR1"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS00,0   },      /**/
    {PCT_STATIC,TEXT("OPERATOR2"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS00,0   },      /**/
    {PCT_STATIC,TEXT("OPERATOR3"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS00,0   },      /**/
    {PCT_STATIC,TEXT("OPERATOR4"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS00,0   },      /**/
    {PCT_STATIC,TEXT("OPERATOR5"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS00,0   },      /**/
    {PCT_STATIC,TEXT("OPERATOR6"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS00,0   },      /**/
    {PCT_STATIC,TEXT("OPERATOR1"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS00,0   },      /**/
    {PCT_STATIC,TEXT("OPERATOR2"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT14 ,COLPOS00,0   },      /**/
    {PCT_STATIC,TEXT("OPERATOR3"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT15 ,COLPOS00,0   },      /**/
    {PCT_STATIC,TEXT("OPERATOR4"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT16 ,COLPOS00,0   },      /**/
    {PCT_STATIC,TEXT("OPERATOR5"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT17 ,COLPOS00,0   },      /**/
    {PCT_STATIC,TEXT("OPERATOR6"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT18 ,COLPOS00,0   },      /**/
    {PCT_STATIC,TEXT("OSCILLATOR"                    ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS04,5   },      /**/
    {PCT_STATIC,TEXT("MODE"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS04,0   },      /**/
    {PCT_STATIC,TEXT("SYNC"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS05,0   },      /**/
    {PCT_STATIC,TEXT("FREQUENCY"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS06,5   },      /**/
    {PCT_STATIC,TEXT("COARSE"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS06,0   },      /**/
    {PCT_STATIC,TEXT("FINE"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS07,0   },      /**/
    {PCT_STATIC,TEXT("DETUNE"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS08,0   },      /**/
    {PCT_STATIC,TEXT("EG"                            ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS09,5   },      /**/
    {PCT_STATIC,TEXT("RATE1"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS09,0   },      /**/
    {PCT_STATIC,TEXT("RATE2"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS10,0   },      /**/
    {PCT_STATIC,TEXT("RATE3"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS11,0   },      /**/
    {PCT_STATIC,TEXT("RATE4"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS12,0   },      /**/
    {PCT_STATIC,TEXT("LEVEL1"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS13,0   },      /**/
    {PCT_STATIC,TEXT("LEVEL2"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS14,0   },      /**/
    {PCT_STATIC,TEXT("LEVEL3"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS15,0   },      /**/
    {PCT_STATIC,TEXT("LEVEL4"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS16,0   },      /**/
    {PCT_STATIC,TEXT("KEYBOARD LEVEL SCALING"        ),0    ,TRUE    ,0       ,0       , NULL                ,300, ROW_POS_STATIC4,COLPOS01,5   },      /**/
    {PCT_STATIC,TEXT("BREAK"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS01,0   },      /**/
    {PCT_STATIC,TEXT("CURVE L"                       ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS02,0   },      /**/
    {PCT_STATIC,TEXT("CURVE R"                       ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS03,0   },      /**/
    {PCT_STATIC,TEXT("DEPTH L"                       ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS04,0   },      /**/
    {PCT_STATIC,TEXT("DEPTH R"                       ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS05,0   },      /**/
    {PCT_STATIC,TEXT("RATE"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC4,COLPOS06,5   },      /**/
    {PCT_STATIC,TEXT("SCALING"                       ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS06,0   },      /**/
    {PCT_STATIC,TEXT("OPERATOR"                      ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC4,COLPOS07,5   },      /**/
    {PCT_STATIC,TEXT("OUT LEVEL"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS07,0   },      /**/
    {PCT_STATIC,TEXT("VELO SENS"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS08,0   },      /**/
    {PCT_STATIC,TEXT("PITCH EG"                      ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC4,COLPOS09,5   },      /**/
    {PCT_STATIC,TEXT("RATE1"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS09,0   },      /**/
    {PCT_STATIC,TEXT("RATE2"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS10,0   },      /**/
    {PCT_STATIC,TEXT("RATE3"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS11,0   },      /**/
    {PCT_STATIC,TEXT("RATE4"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS12,0   },      /**/
    {PCT_STATIC,TEXT("LEVEL1"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS13,0   },      /**/
    {PCT_STATIC,TEXT("LEVEL2"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS14,0   },      /**/
    {PCT_STATIC,TEXT("LEVEL3"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS15,0   },      /**/
    {PCT_STATIC,TEXT("LEVEL4"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS16,0   },      /**/
    {PCT_STATIC,TEXT("KEY TRANSPOSE"                 ),0    ,TRUE    ,0       ,0       , NULL                ,200, ROW_POS_STATIC7,COLPOS01,0   },      /**/
    {PCT_EDIT  ,TEXT("01"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 0              , 0      ,0   },    /*"VOICE_NAME_01"                                 */
    {PCT_EDIT  ,TEXT("02"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 1              , 0      ,0   },    /*"VOICE_NAME_02"                                 */
    {PCT_EDIT  ,TEXT("03"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 2              , 0      ,0   },    /*"VOICE_NAME_03"                                 */
    {PCT_EDIT  ,TEXT("04"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 3              , 0      ,0   },    /*"VOICE_NAME_04"                                 */
    {PCT_EDIT  ,TEXT("05"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 4              , 0      ,0   },    /*"VOICE_NAME_05"                                 */
    {PCT_EDIT  ,TEXT("06"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 5              , 0      ,0   },    /*"VOICE_NAME_06"                                 */
    {PCT_EDIT  ,TEXT("07"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 6              , 0      ,0   },    /*"VOICE_NAME_07"                                 */
    {PCT_EDIT  ,TEXT("08"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 7              , 0      ,0   },    /*"VOICE_NAME_08"                                 */
    {PCT_EDIT  ,TEXT("09"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 0              , 4      ,0   },    /*"VOICE_NAME_09"                                 */
    {PCT_EDIT  ,TEXT("10"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 1              , 4      ,0   },    /*"VOICE_NAME_10"                                 */
    {PCT_EDIT  ,TEXT("11"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 2              , 4      ,0   },    /*"VOICE_NAME_11"                                 */
    {PCT_EDIT  ,TEXT("12"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 3              , 4      ,0   },    /*"VOICE_NAME_12"                                 */
    {PCT_EDIT  ,TEXT("13"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 4              , 4      ,0   },    /*"VOICE_NAME_13"                                 */
    {PCT_EDIT  ,TEXT("14"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 5              , 4      ,0   },    /*"VOICE_NAME_14"                                 */
    {PCT_EDIT  ,TEXT("15"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 6              , 4      ,0   },    /*"VOICE_NAME_15"                                 */
    {PCT_EDIT  ,TEXT("16"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 7              , 4      ,0   },    /*"VOICE_NAME_16"                                 */
    {PCT_EDIT  ,TEXT("17"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 0              , 8      ,0   },    /*"VOICE_NAME_17"                                 */
    {PCT_EDIT  ,TEXT("18"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 1              , 8      ,0   },    /*"VOICE_NAME_18"                                 */
    {PCT_EDIT  ,TEXT("19"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 2              , 8      ,0   },    /*"VOICE_NAME_19"                                 */
    {PCT_EDIT  ,TEXT("20"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 3              , 8      ,0   },    /*"VOICE_NAME_20"                                 */
    {PCT_EDIT  ,TEXT("21"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 4              , 8      ,0   },    /*"VOICE_NAME_21"                                 */
    {PCT_EDIT  ,TEXT("22"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 5              , 8      ,0   },    /*"VOICE_NAME_22"                                 */
    {PCT_EDIT  ,TEXT("23"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 6              , 8      ,0   },    /*"VOICE_NAME_23"                                 */
    {PCT_EDIT  ,TEXT("24"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 7              , 8      ,0   },    /*"VOICE_NAME_24"                                 */
    {PCT_EDIT  ,TEXT("25"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 0              ,12      ,0   },    /*"VOICE_NAME_25"                                 */
    {PCT_EDIT  ,TEXT("26"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 1              ,12      ,0   },    /*"VOICE_NAME_26"                                 */
    {PCT_EDIT  ,TEXT("27"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 2              ,12      ,0   },    /*"VOICE_NAME_27"                                 */
    {PCT_EDIT  ,TEXT("28"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 3              ,12      ,0   },    /*"VOICE_NAME_28"                                 */
    {PCT_EDIT  ,TEXT("29"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 4              ,12      ,0   },    /*"VOICE_NAME_29"                                 */
    {PCT_EDIT  ,TEXT("30"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 5              ,12      ,0   },    /*"VOICE_NAME_30"                                 */
    {PCT_EDIT  ,TEXT("31"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 6              ,12      ,0   },    /*"VOICE_NAME_31"                                 */
    {PCT_EDIT  ,TEXT("32"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 7              ,12      ,0   },    /*"VOICE_NAME_32"                                 */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 0              , 2      ,0   },    /*"ALL_TO_ONEVOICE_01"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 1              , 2      ,0   },    /*"ALL_TO_ONEVOICE_02"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 2              , 2      ,0   },    /*"ALL_TO_ONEVOICE_03"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 3              , 2      ,0   },    /*"ALL_TO_ONEVOICE_04"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 4              , 2      ,0   },    /*"ALL_TO_ONEVOICE_05"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 5              , 2      ,0   },    /*"ALL_TO_ONEVOICE_06"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 6              , 2      ,0   },    /*"ALL_TO_ONEVOICE_07"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 7              , 2      ,0   },    /*"ALL_TO_ONEVOICE_08"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 0              , 6      ,0   },    /*"ALL_TO_ONEVOICE_09"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 1              , 6      ,0   },    /*"ALL_TO_ONEVOICE_10"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 2              , 6      ,0   },    /*"ALL_TO_ONEVOICE_11"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 3              , 6      ,0   },    /*"ALL_TO_ONEVOICE_12"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 4              , 6      ,0   },    /*"ALL_TO_ONEVOICE_13"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 5              , 6      ,0   },    /*"ALL_TO_ONEVOICE_14"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 6              , 6      ,0   },    /*"ALL_TO_ONEVOICE_15"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 7              , 6      ,0   },    /*"ALL_TO_ONEVOICE_16"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 0              ,10      ,0   },    /*"ALL_TO_ONEVOICE_17"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 1              ,10      ,0   },    /*"ALL_TO_ONEVOICE_18"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 2              ,10      ,0   },    /*"ALL_TO_ONEVOICE_19"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 3              ,10      ,0   },    /*"ALL_TO_ONEVOICE_20"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 4              ,10      ,0   },    /*"ALL_TO_ONEVOICE_21"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 5              ,10      ,0   },    /*"ALL_TO_ONEVOICE_22"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 6              ,10      ,0   },    /*"ALL_TO_ONEVOICE_23"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 7              ,10      ,0   },    /*"ALL_TO_ONEVOICE_24"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 0              ,14      ,0   },    /*"ALL_TO_ONEVOICE_25"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 1              ,14      ,0   },    /*"ALL_TO_ONEVOICE_26"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 2              ,14      ,0   },    /*"ALL_TO_ONEVOICE_27"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 3              ,14      ,0   },    /*"ALL_TO_ONEVOICE_28"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 4              ,14      ,0   },    /*"ALL_TO_ONEVOICE_29"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 5              ,14      ,0   },    /*"ALL_TO_ONEVOICE_30"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 6              ,14      ,0   },    /*"ALL_TO_ONEVOICE_31"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 7              ,14      ,0   },    /*"ALL_TO_ONEVOICE_32"                            */
};

typedef struct
{
    INT xPos  ;
    INT yPos  ;
    INT width ;
    INT height;
} S_DX7_PARAM_WINDOW_INFO;

typedef struct
{
    BOOL                exist                        ; /* �E�B���h�E����������Ă��邩�ǂ��� */
    HWND                hwnd_static_text             ; /* �E�B���h�E�̃n���h��               */
    HWND                hwnd_data                    ; /* �E�B���h�E�̃n���h��               */
    S_DX7_PARAM_WINDOW_INFO wtInfo                       ;
    DX7_PARAM_CTRL_GROUP_ID groupId                      ;
} S_DX7_PARAM_INFO;

static S_DX7_PARAM_INFO dx7ctrlParamInfo[DX7_PARAM_CTRL_MAX];

/********************************************************************************
 * ���e  : �o�^���ꂽ�S�Ẵp�����[�^�R���g���[���𐶐�����
 * ����  : HINSTANCE hInst
 * ����  : PTSTR szAppName
 * ����  : HWND hwnd �e�E�B���h�E�̃n���h��
 * �߂�l: BOOL
 ***************************************/
BOOL
Dx7ParamCtrlCreate( HINSTANCE hInst, PTSTR szAppName, HWND hwnd )
{
    dx7ParamCtrlData.bInitConfig = TRUE;
    dx7ParamCtrlData.hInstance   = hInst;
    dx7ParamCtrlData.szAppName   = szAppName;

    dx7ParamCtrlCreate(hwnd,DX7_PARAM_CTRL_GROUP_1VOICE       ,DX7_PARAM_CTRL_1VOICE_START      ,DX7_PARAM_CTRL_1VOICE_END        );
    dx7ParamCtrlCreate(hwnd,DX7_PARAM_CTRL_GROUP_ALL_VOICE    ,DX7_PARAM_CTRL_ALL_VOICE_START   ,DX7_PARAM_CTRL_ALL_VOICE_END     );

    return TRUE;
}

/********************************************************************************
 * ���e  : �o�^���ꂽ�S�Ẵp�����[�^�R���g���[���𐶐�����
 * ����  : HWND hwnd �e�E�B���h�E�̃n���h��
 * ����  : DX7_PARAM_CTRL_GROUP_ID groupId
 * ����  : DX7_PARAM_CTRL_ID       startId
 * ����  : DX7_PARAM_CTRL_ID       endId
 * �߂�l: BOOL
 ***************************************/
static BOOL
dx7ParamCtrlCreate( HWND hwnd, DX7_PARAM_CTRL_GROUP_ID groupId, DX7_PARAM_CTRL_ID startId, DX7_PARAM_CTRL_ID endId )
{
    int nowId;

    if( (startId <= endId) && (endId < DX7_PARAM_CTRL_MAX) )
    {
        for(nowId=startId; nowId<=endId; nowId++ )
        {
            PTSTR class;
            DWORD style;
            DWORD exStyle;
            PTSTR strText;
            DWORD height;
            INT   width;
            INT   xBasePos,yBasePos;
            INT xPos;
            S_DX7_PARAM_INFO *infoPtr = &dx7ctrlParamInfo[nowId];
            S_DX7_PARAM_CTRL *tblPtr = &paramListTbl[nowId];

            if( tblPtr->type == PCT_COMBO )
            {
                class = (PTSTR)TEXT("combobox");
                strText = (PTSTR)0;
                style = (WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
                exStyle = 0;
                height = PARAM_DATA_HEIGHT;
            }
            else if( tblPtr->type == PCT_BUTTON )
            {
                class = (PTSTR)TEXT("button");
                strText = (PTSTR)tblPtr->strText;
                style = (WS_CHILD);
                exStyle = 0;
                height = PARAM_BUTTON_HEIGHT;
            }
            else if( tblPtr->type == PCT_STATIC )
            {
                class = (PTSTR)TEXT("static");
                strText = (PTSTR)tblPtr->strText;
                style = (WS_CHILD|ES_LEFT);
                exStyle = 0;
                height = PARAM_STATIC_HEIGHT;
            }
            else
            {
                class = (PTSTR)TEXT("edit");
                strText = (PTSTR)0;
                style = (WS_CHILD|ES_LEFT|ES_AUTOHSCROLL);

                if( DX7_PARAM_CTRL_ALL_VOICE_START <= nowId && nowId <= DX7_PARAM_CTRL_ALL_VOICE_END )
                {
                    style |= ES_READONLY;
                }
                else
                {
                    nop();
                }

                exStyle = WS_EX_OVERLAPPEDWINDOW;
                height = PARAM_EDIT_HEIGHT;
            }

            xBasePos = (PARAM_WRAP_X_UNIT*tblPtr->colPos);
            yBasePos = (PARAM_DATA_Y_UNIT*2*tblPtr->rowPos) + tblPtr->yPosAdj;

            if( tblPtr->type == PCT_STATIC )
            {
                width = PARAM_STATIC_WIDTH;
                infoPtr->wtInfo.xPos   = PARAM_DATA_X_BASE_POS   + xBasePos;
                infoPtr->wtInfo.yPos   = PARAM_STATIC_Y_BASE_POS + yBasePos;
            }
            else
            {
                width = PARAM_DATA_WIDTH;
                infoPtr->wtInfo.xPos   = PARAM_DATA_X_BASE_POS + xBasePos;
                infoPtr->wtInfo.yPos   = PARAM_DATA_Y_BASE_POS + yBasePos;
            }
            infoPtr->wtInfo.width  = width + tblPtr->addWidth;
            infoPtr->wtInfo.height = height;

            if( tblPtr->withTextLen )
            {
                infoPtr->hwnd_static_text = CreateWindowEx( 0,                                          /* �g���X�^�C��       */
                                            TEXT("static"),                             /* �N���X��           */
                                            tblPtr->strText,                            /* �E�B���h�E�e�L�X�g */
                                            (WS_CHILD|ES_LEFT),                         /* �X�^�C��           */
                                            infoPtr->wtInfo.xPos,                  /* x���W              */
                                            PARAM_STATIC_Y_BASE_POS + yBasePos,   /* y���W              */
                                            tblPtr->withTextLen,    /* ��                 */
                                            infoPtr->wtInfo.height, /* ����               */
                                            hwnd,(HMENU)0,
                                            dx7ParamCtrlData.hInstance,NULL );               /* �C���X�^���X�n���h��,�⏕���� */

                if( infoPtr->hwnd_static_text != NULL )
                {
                    SendMessage(infoPtr->hwnd_static_text, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));
                }
                else
                {
                }

                infoPtr->wtInfo.xPos += tblPtr->withTextLen;
            }
            else
            {
                nop();
            }

#ifdef DEBUG_DISP_STATIC_STRING
            infoPtr->wtInfo.width  = width + tblPtr->addWidth;
            infoPtr->wtInfo.height = PARAM_STATIC_HEIGHT;

            infoPtr->hwnd_data = CreateWindowEx( WS_EX_OVERLAPPEDWINDOW,                    /* �g���X�^�C��       */
                                                (PTSTR)TEXT("edit")/*TEXT("static")*/,      /* �N���X��           */
                                                tblPtr->strText,                             /* �E�B���h�E�e�L�X�g */
                                                (WS_CHILD|ES_LEFT|ES_AUTOHSCROLL)/*(WS_CHILD|ES_LEFT)*/,/* �X�^�C��           */
                                                infoPtr->wtInfo.xPos,   /* x���W              */
                                                infoPtr->wtInfo.yPos,   /* y���W              */
                                                infoPtr->wtInfo.width,  /* ��                 */
                                                infoPtr->wtInfo.height, /* ����               */
                                                hwnd,(HMENU)(DX7_PARAM_CTRL_ID_OFFSET+(nowId*2)+1),/* �e�E�B���h�E,�q�E�B���h�EID */
                                                dx100ParamCtrlData.hInstance,NULL );                        /* �C���X�^���X�n���h��,�⏕���� */
#else
            infoPtr->hwnd_data = CreateWindowEx( exStyle,                                    /* �g���X�^�C��       */
                                                class,                                      /* �N���X��           */
                                                strText,                                    /* �E�B���h�E�e�L�X�g */
                                                style,                                      /* �X�^�C��           */
                                                infoPtr->wtInfo.xPos,   /* x���W              */
                                                infoPtr->wtInfo.yPos,   /* y���W              */
                                                infoPtr->wtInfo.width,  /* ��                 */
                                                infoPtr->wtInfo.height, /* ����               */
                                                hwnd,(HMENU)(DX7_PARAM_CTRL_ID_OFFSET+nowId),/* �e�E�B���h�E,�q�E�B���h�EID */
                                                dx7ParamCtrlData.hInstance,NULL );                        /* �C���X�^���X�n���h��,�⏕���� */
#endif
            if( infoPtr->hwnd_data != NULL )
            {
                INT j,iCbNum;
                TCHAR szBuff[128];

                SendMessage(infoPtr->hwnd_data, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));

                /* �ʂɏ���������� */
                switch( nowId )
                {
                default:
                    break;
                }

                ShowWindow(infoPtr->hwnd_data, SW_HIDE);

                if( tblPtr->type == PCT_COMBO )
                {
                    for(j=tblPtr->minValue; j<=tblPtr->maxValue; j++ )
                    {
                        if( tblPtr->ptstrDataDisp )
                        {
                            iCbNum = SendMessage(infoPtr->hwnd_data , CB_ADDSTRING, 0, (LPARAM)tblPtr->ptstrDataDisp[j] );
                        }
                        else
                        {
                            wsprintf(szBuff,TEXT("%d"),j);
                            iCbNum = SendMessage(infoPtr->hwnd_data , CB_ADDSTRING, 0, (LPARAM)&szBuff[0]);
                        }

                        SendMessage( infoPtr->hwnd_data , CB_SETITEMDATA, iCbNum, (LPARAM)j );
                    }
                    SendMessage( infoPtr->hwnd_data , CB_SETCURSEL, 0, (LPARAM)0 );
                }
                else
                {
                    nop();
                }

                if( tblPtr->bEnable )
                {
                    nop();
                }
                else
                {
                    EnableWindow( infoPtr->hwnd_data, FALSE);
                }
            }
            else
            {
                /* do nothing */
            }
            infoPtr->exist = TRUE;
            infoPtr->groupId = groupId;
        }
    }
    else
    {
        nop();
    }

    return TRUE;
}

/********************************************************************************
 * ���e  : �w��̃O���[�v�̃p�����[�^�R���g���[����\��
 * ����  : DX7_PARAM_CTRL_GROUP_ID groupId
 * �߂�l: BOOL
 ***************************************/
BOOL
Dx7ParamCtrlGroupDisplay( DX7_PARAM_CTRL_GROUP_ID groupId )
{
    int nowId;

    for(nowId=0; nowId<DX7_PARAM_CTRL_MAX; nowId++ )
    {
        S_DX7_PARAM_INFO *infoPtr = &dx7ctrlParamInfo[nowId];

        if( infoPtr->groupId == groupId )
        {
            ShowWindow(infoPtr->hwnd_data, SW_SHOW);
            if( infoPtr->hwnd_static_text )
            {
                ShowWindow(infoPtr->hwnd_static_text, SW_SHOW);
            }
            else
            {
                nop();
            }
        }
        else
        {
            ShowWindow(infoPtr->hwnd_data, SW_HIDE);
            if( infoPtr->hwnd_static_text )
            {
                ShowWindow(infoPtr->hwnd_static_text, SW_HIDE);
            }
            else
            {
                nop();
            }
        }
    }

    return TRUE;
}


/********************************************************************************
 * ���e  : �E�B���h�E�n���h�����擾����
 * ����  : DX7_PARAM_CTRL_ID id
 * �߂�l: HWND
 ***************************************/
HWND
Dx7ParamCtrlGetHWND( DX7_PARAM_CTRL_ID id )
{
    HWND rtn = (HWND)0;

    if( id < DX7_PARAM_CTRL_MAX )
    {
        S_DX7_PARAM_INFO *infoPtr = &dx7ctrlParamInfo[id];

        if( infoPtr->exist == TRUE )
        {
            rtn = infoPtr->hwnd_data;
        }
        else
        {
            nop();
        }
    }
    else
    {
        nop();
    }

    return rtn;
}

/********************************************************************************
 * ���e  : �E�B���h�E����e�L�X�g���擾����
 * ����  : DX7_PARAM_CTRL_ID id
 * ����  : PTSTR ptstrText
 * �߂�l: BOOL
 ***************************************/
BOOL
Dx7ParamCtrlGetText( DX7_PARAM_CTRL_ID id, PTSTR ptstrText )
{
    BOOL rtn = FALSE;
    INT iLength;

    if( id < DX7_PARAM_CTRL_MAX )
    {
        S_DX7_PARAM_INFO *infoPtr = &dx7ctrlParamInfo[id];

        if( infoPtr->exist == TRUE )
        {
            iLength = GetWindowTextLength( infoPtr->hwnd_data );
            GetWindowText( infoPtr->hwnd_data, ptstrText, iLength + 1 );
            rtn = TRUE;
        }
        else
        {
            /* error */
        }
    }
    else
    {
        nop();
    }

    return rtn;
}

/********************************************************************************
 * ���e  : �R���g���[���̃T�C�Y�𒲐�����
 * ����  : int xPos
 * ����  : int yPos
 * �߂�l: �Ȃ�
 ***************************************/
void
Dx7ParamCtrlSetSize( int xPos, int yPos )
{
    int i;

    for(i=0; i<DX7_PARAM_CTRL_MAX; i++)
    {
        S_DX7_PARAM_INFO *infoPtr = &dx7ctrlParamInfo[i];

        MoveWindow( infoPtr->hwnd_data,
                    infoPtr->wtInfo.xPos - xPos,   /* x���W              */
                    infoPtr->wtInfo.yPos - yPos,   /* y���W              */
                    infoPtr->wtInfo.width,  /* ��                 */
                    infoPtr->wtInfo.height, /* ����               */
                    TRUE);
    }
}
