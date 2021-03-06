/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */

/* 外部関数定義 */
#include "WinMain.h"

/* 外部変数定義 */

/* 内部関数定義 */
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
/* 内部変数定義 */

static char *strTblOnOff[2] = {"OFF","ON"};
static char *strTblAlgorithm[32] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32"};
static char *strTblLfoWave[6]   = {"TRI","SAW DWN","SAW UP","SQU","SINE","S/H"};
static char *strTblTranspose[49]  = {"C 1","C#1","D 1","D#1","E 1","F 1","F#1","G 1","G#1","A 1","A#1","B 1","C 2","C#2","D 2","D#2","E 2","F 2","F#2","G 2","G#2","A 2","A#2","B 2","C 3","C#3","D 3","D#3","E 3","F 3","F#3","G 3","G#3","A 3","A#3","B 3","C 4","C#4","D 4","D#4","E 4","F 4","F#4","G 4","G#4","A 4","A#4","B 4","C 5"};
static char *strTblFreqMode[2] = {"RATIO","FIXED"};
static char *strTblBreakPoint[100]  = {"A -1","A#-1","B -1","C 0","C#0","D 0","D#0","E 0","F 0","F#0","G 0","G#0","A 0","A#0","B 0","C 1","C#1","D 1","D#1","E 1","F 1","F#1","G 1","G#1","A 1","A#1","B 1","C 2","C#2","D 2","D#2","E 2","F 2","F#2","G 2","G#2","A 2","A#2","B 2","C 3","C#3","D 3","D#3","E 3","F 3","F#3","G 3","G#3","A 3","A#3","B 3","C 4","C#4","D 4","D#4","E 4","F 4","F#4","G 4","G#4","A 4","A#4","B 4","C 5","C#5","D 5","D#5","E 5","F 5","F#5","G 5","G#5","A 5","A#5","B 5","C 6","C#6","D 6","D#6","E 6","F 6","F#6","G 6","G#6","A 6","A#6","B 6","C 7","C#7","D 7","D#7","E 7","F 7","F#7","G 7","G#7","A 7","A#7","B 7","C 8"};
static char *strTblCurve[4] = {"-LIN","-EXP","+EXP","+LIN"};
static char *strTblDetune[15] = {"-7","-6","-5","-4","-3","-2","-1","0","+1","+2","+3","+4","+5","+6","+7"};
static char *strTblVoiceNum[32] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32"};

#if 0
static char *strTblPolyMono[2] = {"POLY","MONO"};
static char *strTblPortamentoMode[2] = {"Full","Fing"};
static char *strTblFrequency[64] = {"0.50","0.71","0.78","0.87","1.00","1.41","1.57","1.73","2.00","2.82","3.00","3.14","3.46","4.00","4.24","4.71","5.00","5.19","5.65","6.00","6.28","6.92","7.00","7.07","7.85","8.00","8.48","8.65","9.00","9.42","9.89","10.00","10.38","10.99","11.00","11.30","12.00","12.11","12.56","12.72","13.00","13.84","14.00","14.10","14.13","15.00","15.55","15.57","15.70","16.96","17.27","17.30","18.37","18.84","19.03","19.78","20.41","20.76","21.20","21.98","22.49","23.55","24.22","25.95"};
#endif

/* X方向 */
#define PARAM_STATIC_X_BASE_POS   5
#define PARAM_DATA_X_BASE_POS     5
#define PARAM_DATA_WIDTH         50
#define PARAM_STATIC_WIDTH       70
#define PARAM_WRAP_X_UNIT        70

/* Y方向 */
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
    PTSTR           strText      ; /* ウィンドウテキスト                         */
    INT             withTextLen  ; /* 左側にテキストを追加する場合の長さ(0の時追加しない) */
    BOOL            bEnable      ;
    INT             minValue     ;
    INT             maxValue     ;
    char **         ptstrDataDisp;
    INT             addWidth     ;
    INT             rowPos       ;/* 行 */
    INT             colPos       ;/* 列 */
    INT             yPosAdj      ;
} S_DX7_PARAM_CTRL;

static S_DX7_PARAM_CTRL paramListTbl[DX7_PARAM_CTRL_MAX] =
{/*  type     ,strText                               ,wTLen,bEnable  ,minValue,maxValue,ptstrDataDisp   ,addWidth,rowPos          ,colPos  ,yPosAdj*/
    {PCT_COMBO ,TEXT("RATE1"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS09,0   },/* 0     OP6 EG RATE 1                            */
    {PCT_COMBO ,TEXT("RATE2"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS10,0   },/* 1     OP6 EG RATE 2                            */
    {PCT_COMBO ,TEXT("RATE3"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS11,0   },/* 2     OP6 EG RATE 3                            */
    {PCT_COMBO ,TEXT("RATE4"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS12,0   },/* 3     OP6 EG RATE 4                            */
    {PCT_COMBO ,TEXT("LEVEL1"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS13,0   },/* 4     OP6 EG LEVEL 1                           */
    {PCT_COMBO ,TEXT("LEVEL2"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS14,0   },/* 5     OP6 EG LEVEL 2                           */
    {PCT_COMBO ,TEXT("LEVEL3"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS15,0   },/* 6     OP6 EG LEVEL 3                           */
    {PCT_COMBO ,TEXT("LEVEL4"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS16,0   },/* 7     OP6 EG LEVEL 4                           */
    {PCT_COMBO ,TEXT("BREAK POINT"                   ),0    ,TRUE    ,0       , 99     , strTblBreakPoint    ,  0, ROW_POS_EDIT18 ,COLPOS01,0   },/* 8     OP6 BREAK POINT                          */
    {PCT_COMBO ,TEXT("L DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT18 ,COLPOS04,0   },/* 9     OP6 LEFT DEPTH                           */
    {PCT_COMBO ,TEXT("R DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT18 ,COLPOS05,0   },/*10     OP6 RIGHT DEPTH                          */
    {PCT_COMBO ,TEXT("L CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT18 ,COLPOS02,0   },/*11     OP6 LEFT CURVE                           */
    {PCT_COMBO ,TEXT("R CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT18 ,COLPOS03,0   },/*12     OP6 RIGHT CURVE                          */
    {PCT_COMBO ,TEXT("KEY BOARD RATE SCALING"        ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT18 ,COLPOS06,0   },/*13     OP6 KEY BOARD RATE SCALING               */
    {PCT_COMBO ,TEXT("MOD AMP"                       ),0    ,TRUE    ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS02,0   },/*14     OP6 MOD SENSITIVITY AMPLITUDE            */
    {PCT_COMBO ,TEXT("VELOCITY SENS"                 ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT18 ,COLPOS08,0   },/*15     OP6 OPERATOR KEY VELOCITY SENSITIVITY    */
    {PCT_COMBO ,TEXT("OUTPUT LEVEL"                  ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT18 ,COLPOS07,0   },/*16     OP6 OPERATOR OUTPUT LEVEL                */
    {PCT_COMBO ,TEXT("MODE"                          ),0    ,TRUE    ,0       ,  1     , strTblFreqMode      ,  0, ROW_POS_EDIT12 ,COLPOS03,0   },/*17     OP6 OSILLATOR MODE                       */
    {PCT_COMBO ,TEXT("FREQ COARSE"                   ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS06,0   },/*18     OP6 OSILLATOR FREQUENCY COARSE           */
    {PCT_COMBO ,TEXT("FREQ  FINE"                    ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS07,0   },/*19     OP6 OSILLATOR FREQUENCY FINE             */
    {PCT_COMBO ,TEXT("DETUNE"                        ),0    ,TRUE    ,0       , 14     , strTblDetune        ,  0, ROW_POS_EDIT12 ,COLPOS08,0   },/*20     OP6 DETUNE                               */
    {PCT_COMBO ,TEXT("RATE1"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS09,0   },/*21     OP5 EG RATE 1                            */
    {PCT_COMBO ,TEXT("RATE2"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS10,0   },/*22     OP5 EG RATE 2                            */
    {PCT_COMBO ,TEXT("RATE3"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS11,0   },/*23     OP5 EG RATE 3                            */
    {PCT_COMBO ,TEXT("RATE4"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS12,0   },/*24     OP5 EG RATE 4                            */
    {PCT_COMBO ,TEXT("LEVEL1"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS13,0   },/*25     OP5 EG LEVEL 1                           */
    {PCT_COMBO ,TEXT("LEVEL2"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS14,0   },/*26     OP5 EG LEVEL 2                           */
    {PCT_COMBO ,TEXT("LEVEL3"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS15,0   },/*27     OP5 EG LEVEL 3                           */
    {PCT_COMBO ,TEXT("LEVEL4"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS16,0   },/*28     OP5 EG LEVEL 4                           */
    {PCT_COMBO ,TEXT("BREAK POINT"                   ),0    ,TRUE    ,0       , 99     , strTblBreakPoint    ,  0, ROW_POS_EDIT17 ,COLPOS01,0   },/*29     OP5 BREAK POINT                          */
    {PCT_COMBO ,TEXT("L DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT17 ,COLPOS04,0   },/*30     OP5 LEFT DEPTH                           */
    {PCT_COMBO ,TEXT("R DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT17 ,COLPOS05,0   },/*31     OP5 RIGHT DEPTH                          */
    {PCT_COMBO ,TEXT("L CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT17 ,COLPOS02,0   },/*32     OP5 LEFT CURVE                           */
    {PCT_COMBO ,TEXT("R CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT17 ,COLPOS03,0   },/*33     OP5 RIGHT CURVE                          */
    {PCT_COMBO ,TEXT("KEY BOARD RATE SCALING"        ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT17 ,COLPOS06,0   },/*34     OP5 KEY BOARD RATE SCALING               */
    {PCT_COMBO ,TEXT("MOD AMP"                       ),0    ,TRUE    ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS02,0   },/*35     OP5 MOD SENSITIVITY AMPLITUDE            */
    {PCT_COMBO ,TEXT("VELOCITY SENS"                 ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT17 ,COLPOS08,0   },/*36     OP5 OPERATOR KEY VELOCITY SENSITIVITY    */
    {PCT_COMBO ,TEXT("OUTPUT LEVEL"                  ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT17 ,COLPOS07,0   },/*37     OP5 OPERATOR OUTPUT LEVEL                */
    {PCT_COMBO ,TEXT("MODE"                          ),0    ,TRUE    ,0       ,  1     , strTblFreqMode      ,  0, ROW_POS_EDIT11 ,COLPOS03,0   },/*38     OP5 OSILLATOR MODE                       */
    {PCT_COMBO ,TEXT("FREQ COARSE"                   ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS06,0   },/*39     OP5 OSILLATOR FREQUENCY COARSE           */
    {PCT_COMBO ,TEXT("FREQ  FINE"                    ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS07,0   },/*40     OP5 OSILLATOR FREQUENCY FINE             */
    {PCT_COMBO ,TEXT("DETUNE"                        ),0    ,TRUE    ,0       , 14     , strTblDetune        ,  0, ROW_POS_EDIT11 ,COLPOS08,0   },/*41     OP5 DETUNE                               */
    {PCT_COMBO ,TEXT("RATE1"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS09,0   },/*42     OP4 EG RATE 1                            */
    {PCT_COMBO ,TEXT("RATE2"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS10,0   },/*43     OP4 EG RATE 2                            */
    {PCT_COMBO ,TEXT("RATE3"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS11,0   },/*44     OP4 EG RATE 3                            */
    {PCT_COMBO ,TEXT("RATE4"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS12,0   },/*45     OP4 EG RATE 4                            */
    {PCT_COMBO ,TEXT("LEVEL1"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS13,0   },/*46     OP4 EG LEVEL 1                           */
    {PCT_COMBO ,TEXT("LEVEL2"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS14,0   },/*47     OP4 EG LEVEL 2                           */
    {PCT_COMBO ,TEXT("LEVEL3"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS15,0   },/*48     OP4 EG LEVEL 3                           */
    {PCT_COMBO ,TEXT("LEVEL4"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS16,0   },/*49     OP4 EG LEVEL 4                           */
    {PCT_COMBO ,TEXT("BREAK POINT"                   ),0    ,TRUE    ,0       , 99     , strTblBreakPoint    ,  0, ROW_POS_EDIT16 ,COLPOS01,0   },/*50     OP4 BREAK POINT                          */
    {PCT_COMBO ,TEXT("L DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT16 ,COLPOS04,0   },/*51     OP4 LEFT DEPTH                           */
    {PCT_COMBO ,TEXT("R DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT16 ,COLPOS05,0   },/*52     OP4 RIGHT DEPTH                          */
    {PCT_COMBO ,TEXT("L CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT16 ,COLPOS02,0   },/*53     OP4 LEFT CURVE                           */
    {PCT_COMBO ,TEXT("R CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT16 ,COLPOS03,0   },/*54     OP4 RIGHT CURVE                          */
    {PCT_COMBO ,TEXT("KEY BOARD RATE SCALING"        ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT16 ,COLPOS06,0   },/*55     OP4 KEY BOARD RATE SCALING               */
    {PCT_COMBO ,TEXT("MOD AMP"                       ),0    ,TRUE    ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS02,0   },/*56     OP4 MOD SENSITIVITY AMPLITUDE            */
    {PCT_COMBO ,TEXT("VELOCITY SENS"                 ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT16 ,COLPOS08,0   },/*57     OP4 OPERATOR KEY VELOCITY SENSITIVITY    */
    {PCT_COMBO ,TEXT("OUTPUT LEVEL"                  ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT16 ,COLPOS07,0   },/*58     OP4 OPERATOR OUTPUT LEVEL                */
    {PCT_COMBO ,TEXT("MODE"                          ),0    ,TRUE    ,0       ,  1     , strTblFreqMode      ,  0, ROW_POS_EDIT10 ,COLPOS03,0   },/*59     OP4 OSILLATOR MODE                       */
    {PCT_COMBO ,TEXT("FREQ COARSE"                   ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS06,0   },/*60     OP4 OSILLATOR FREQUENCY COARSE           */
    {PCT_COMBO ,TEXT("FREQ  FINE"                    ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS07,0   },/*61     OP4 OSILLATOR FREQUENCY FINE             */
    {PCT_COMBO ,TEXT("DETUNE"                        ),0    ,TRUE    ,0       , 14     , strTblDetune        ,  0, ROW_POS_EDIT10 ,COLPOS08,0   },/*62     OP4 DETUNE                               */
    {PCT_COMBO ,TEXT("RATE1"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS09,0   },/*63     OP3 EG RATE 1                            */
    {PCT_COMBO ,TEXT("RATE2"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS10,0   },/*64     OP3 EG RATE 2                            */
    {PCT_COMBO ,TEXT("RATE3"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS11,0   },/*65     OP3 EG RATE 3                            */
    {PCT_COMBO ,TEXT("RATE4"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS12,0   },/*66     OP3 EG RATE 4                            */
    {PCT_COMBO ,TEXT("LEVEL1"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS13,0   },/*67     OP3 EG LEVEL 1                           */
    {PCT_COMBO ,TEXT("LEVEL2"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS14,0   },/*68     OP3 EG LEVEL 2                           */
    {PCT_COMBO ,TEXT("LEVEL3"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS15,0   },/*69     OP3 EG LEVEL 3                           */
    {PCT_COMBO ,TEXT("LEVEL4"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS16,0   },/*70     OP3 EG LEVEL 4                           */
    {PCT_COMBO ,TEXT("BREAK POINT"                   ),0    ,TRUE    ,0       , 99     , strTblBreakPoint    ,  0, ROW_POS_EDIT15 ,COLPOS01,0   },/*71     OP3 BREAK POINT                          */
    {PCT_COMBO ,TEXT("L DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT15 ,COLPOS04,0   },/*72     OP3 LEFT DEPTH                           */
    {PCT_COMBO ,TEXT("R DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT15 ,COLPOS05,0   },/*73     OP3 RIGHT DEPTH                          */
    {PCT_COMBO ,TEXT("L CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT15 ,COLPOS02,0   },/*74     OP3 LEFT CURVE                           */
    {PCT_COMBO ,TEXT("R CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT15 ,COLPOS03,0   },/*75     OP3 RIGHT CURVE                          */
    {PCT_COMBO ,TEXT("KEY BOARD RATE SCALING"        ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT15 ,COLPOS06,0   },/*76     OP3 KEY BOARD RATE SCALING               */
    {PCT_COMBO ,TEXT("MOD AMP"                       ),0    ,TRUE    ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS02,0   },/*77     OP3 MOD SENSITIVITY AMPLITUDE            */
    {PCT_COMBO ,TEXT("VELOCITY SENS"                 ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT15 ,COLPOS08,0   },/*78     OP3 OPERATOR KEY VELOCITY SENSITIVITY    */
    {PCT_COMBO ,TEXT("OUTPUT LEVEL"                  ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT15 ,COLPOS07,0   },/*79     OP3 OPERATOR OUTPUT LEVEL                */
    {PCT_COMBO ,TEXT("MODE"                          ),0    ,TRUE    ,0       ,  1     , strTblFreqMode      ,  0, ROW_POS_EDIT09 ,COLPOS03,0   },/*80     OP3 OSILLATOR MODE                       */
    {PCT_COMBO ,TEXT("FREQ COARSE"                   ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS06,0   },/*81     OP3 OSILLATOR FREQUENCY COARSE           */
    {PCT_COMBO ,TEXT("FREQ  FINE"                    ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS07,0   },/*82     OP3 OSILLATOR FREQUENCY FINE             */
    {PCT_COMBO ,TEXT("DETUNE"                        ),0    ,TRUE    ,0       , 14     , strTblDetune        ,  0, ROW_POS_EDIT09 ,COLPOS08,0   },/*83     OP3 DETUNE                               */
    {PCT_COMBO ,TEXT("RATE1"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS09,0   },/*84     OP2 EG RATE 1                            */
    {PCT_COMBO ,TEXT("RATE2"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS10,0   },/*85     OP2 EG RATE 2                            */
    {PCT_COMBO ,TEXT("RATE3"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS11,0   },/*86     OP2 EG RATE 3                            */
    {PCT_COMBO ,TEXT("RATE4"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS12,0   },/*87     OP2 EG RATE 4                            */
    {PCT_COMBO ,TEXT("LEVEL1"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS13,0   },/*88     OP2 EG LEVEL 1                           */
    {PCT_COMBO ,TEXT("LEVEL2"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS14,0   },/*89     OP2 EG LEVEL 2                           */
    {PCT_COMBO ,TEXT("LEVEL3"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS15,0   },/*90     OP2 EG LEVEL 3                           */
    {PCT_COMBO ,TEXT("LEVEL4"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS16,0   },/*91     OP2 EG LEVEL 4                           */
    {PCT_COMBO ,TEXT("BREAK POINT"                   ),0    ,TRUE    ,0       , 99     , strTblBreakPoint    ,  0, ROW_POS_EDIT14 ,COLPOS01,0   },/*92     OP2 BREAK POINT                          */
    {PCT_COMBO ,TEXT("L DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT14 ,COLPOS04,0   },/*93     OP2 LEFT DEPTH                           */
    {PCT_COMBO ,TEXT("R DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT14 ,COLPOS05,0   },/*94     OP2 RIGHT DEPTH                          */
    {PCT_COMBO ,TEXT("L CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT14 ,COLPOS02,0   },/*95     OP2 LEFT CURVE                           */
    {PCT_COMBO ,TEXT("R CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT14 ,COLPOS03,0   },/*96     OP2 RIGHT CURVE                          */
    {PCT_COMBO ,TEXT("KEY BOARD RATE SCALING"        ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT14 ,COLPOS06,0   },/*97     OP2 KEY BOARD RATE SCALING               */
    {PCT_COMBO ,TEXT("MOD AMP"                       ),0    ,TRUE    ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS02,0   },/*98     OP2 MOD SENSITIVITY AMPLITUDE            */
    {PCT_COMBO ,TEXT("VELOCITY SENS"                 ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT14 ,COLPOS08,0   },/*99     OP2 OPERATOR KEY VELOCITY SENSITIVITY    */
    {PCT_COMBO ,TEXT("OUTPUT LEVEL"                  ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT14 ,COLPOS07,0   },/*100    OP2 OPERATOR OUTPUT LEVEL                */
    {PCT_COMBO ,TEXT("MODE"                          ),0    ,TRUE    ,0       ,  1     , strTblFreqMode      ,  0, ROW_POS_EDIT08 ,COLPOS03,0   },/*101    OP2 OSILLATOR MODE                       */
    {PCT_COMBO ,TEXT("FREQ COARSE"                   ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS06,0   },/*102    OP2 OSILLATOR FREQUENCY COARSE           */
    {PCT_COMBO ,TEXT("FREQ  FINE"                    ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS07,0   },/*103    OP2 OSILLATOR FREQUENCY FINE             */
    {PCT_COMBO ,TEXT("DETUNE"                        ),0    ,TRUE    ,0       , 14     , strTblDetune        ,  0, ROW_POS_EDIT08 ,COLPOS08,0   },/*104    OP2 DETUNE                               */
    {PCT_COMBO ,TEXT("RATE1"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS09,0   },/*105    OP1 EG RATE 1                            */
    {PCT_COMBO ,TEXT("RATE2"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS10,0   },/*106    OP1 EG RATE 2                            */
    {PCT_COMBO ,TEXT("RATE3"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS11,0   },/*107    OP1 EG RATE 3                            */
    {PCT_COMBO ,TEXT("RATE4"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS12,0   },/*108    OP1 EG RATE 4                            */
    {PCT_COMBO ,TEXT("LEVEL1"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS13,0   },/*109    OP1 EG LEVEL 1                           */
    {PCT_COMBO ,TEXT("LEVEL2"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS14,0   },/*110    OP1 EG LEVEL 2                           */
    {PCT_COMBO ,TEXT("LEVEL3"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS15,0   },/*111    OP1 EG LEVEL 3                           */
    {PCT_COMBO ,TEXT("LEVEL4"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS16,0   },/*112    OP1 EG LEVEL 4                           */
    {PCT_COMBO ,TEXT("BREAK POINT"                   ),0    ,TRUE    ,0       , 99     , strTblBreakPoint    ,  0, ROW_POS_EDIT13 ,COLPOS01,0   },/*113    OP1 BREAK POINT                          */
    {PCT_COMBO ,TEXT("L DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS04,0   },/*114    OP1 LEFT DEPTH                           */
    {PCT_COMBO ,TEXT("R DEPTH"                       ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS05,0   },/*115    OP1 RIGHT DEPTH                          */
    {PCT_COMBO ,TEXT("L CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT13 ,COLPOS02,0   },/*116    OP1 LEFT CURVE                           */
    {PCT_COMBO ,TEXT("R CURVE"                       ),0    ,TRUE    ,0       ,  3     , strTblCurve         ,  0, ROW_POS_EDIT13 ,COLPOS03,0   },/*117    OP1 RIGHT CURVE                          */
    {PCT_COMBO ,TEXT("KEY BOARD RATE SCALING"        ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS06,0   },/*118    OP1 KEY BOARD RATE SCALING               */
    {PCT_COMBO ,TEXT("MOD AMP"                       ),0    ,TRUE    ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS02,0   },/*119    OP1 MOD SENSITIVITY AMPLITUDE            */
    {PCT_COMBO ,TEXT("VELOCITY SENS"                 ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS08,0   },/*120    OP1 OPERATOR KEY VELOCITY SENSITIVITY    */
    {PCT_COMBO ,TEXT("OUTPUT LEVEL"                  ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS07,0   },/*121    OP1 OPERATOR OUTPUT LEVEL                */
    {PCT_COMBO ,TEXT("MODE"                          ),0    ,TRUE    ,0       ,  1     , strTblFreqMode      ,  0, ROW_POS_EDIT07 ,COLPOS03,0   },/*122    OP1 OSILLATOR MODE                       */
    {PCT_COMBO ,TEXT("FREQ COARSE"                   ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS06,0   },/*123    OP1 OSILLATOR FREQUENCY COARSE           */
    {PCT_COMBO ,TEXT("FREQ  FINE"                    ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS07,0   },/*124    OP1 OSILLATOR FREQUENCY FINE             */
    {PCT_COMBO ,TEXT("DETUNE"                        ),0    ,TRUE    ,0       , 14     , strTblDetune        ,  0, ROW_POS_EDIT07 ,COLPOS08,0   },/*125    OP1 DETUNE                               */
    {PCT_COMBO ,TEXT("PEG R1"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS09,0   },/*126        PITCH EG RATE 1                      */
    {PCT_COMBO ,TEXT("PEG R2"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS10,0   },/*127        PITCH EG RATE 2                      */
    {PCT_COMBO ,TEXT("PEG R3"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS11,0   },/*128        PITCH EG RATE 3                      */
    {PCT_COMBO ,TEXT("PEG R4"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS12,0   },/*129        PITCH EG RATE 4                      */
    {PCT_COMBO ,TEXT("PEG L1"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS13,0   },/*130        PITCH EG LEVEL 1                     */
    {PCT_COMBO ,TEXT("PEG L2"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS14,0   },/*131        PITCH EG LEVEL 2                     */
    {PCT_COMBO ,TEXT("PEG L3"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS15,0   },/*132        PITCH EG LEVEL 3                     */
    {PCT_COMBO ,TEXT("PEG L4"                        ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS16,0   },/*133        PITCH EG LEVEL 4                     */
    {PCT_COMBO ,TEXT("ALGORITHM SELECT"              ),0    ,TRUE    ,0       , 31     , strTblAlgorithm     ,  0, ROW_POS_EDIT01 ,COLPOS01,0   },/*134        ALGORITHM SELECT                     */
    {PCT_COMBO ,TEXT("FEED BACK"                     ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT01 ,COLPOS02,0   },/*135        FEED BACK                            */
    {PCT_COMBO ,TEXT("SYNC"                          ),0    ,TRUE    ,0       ,  1     , strTblOnOff         ,  0, ROW_POS_EDIT07 ,COLPOS04,0   },/*136        OSILLATOR SYNC                       */
    {PCT_COMBO ,TEXT("SPEED"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT01 ,COLPOS04,0   },/*137        LFO SPEED                            */
    {PCT_COMBO ,TEXT("DELAY"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT01 ,COLPOS05,0   },/*138        LFO DELAY                            */
    {PCT_COMBO ,TEXT("PMD"                           ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT01 ,COLPOS06,0   },/*139        LFO PMD                              */
    {PCT_COMBO ,TEXT("AMD"                           ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT01 ,COLPOS07,0   },/*140        LFO AMD                              */
    {PCT_COMBO ,TEXT("SYNC"                          ),0    ,TRUE    ,0       ,  1     , strTblOnOff         ,  0, ROW_POS_EDIT01 ,COLPOS08,0   },/*141        LFO SYNC                             */
    {PCT_COMBO ,TEXT("WAVE"                          ),0    ,TRUE    ,0       ,  5     , strTblLfoWave       ,  0, ROW_POS_EDIT01 ,COLPOS03,0   },/*142        LFO WAVE                             */
    {PCT_COMBO ,TEXT("PITCH"                         ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS01,0   },/*143        MOD SENSITIVITY PITCH                */
    {PCT_COMBO ,TEXT("TRANSPOSE"                     ),0    ,TRUE    ,0       , 48     , strTblTranspose     ,  0, ROW_POS_EDIT19 ,COLPOS01,0   },/*144        TRANSPOSE                            */
    {PCT_EDIT  ,TEXT("PATCHNAME"                     ),0    ,TRUE    ,0       ,127     , NULL                , 50, ROW_POS_EDIT00 ,COLPOS01,0   },/*145-154    VOICE NAME                           */
  /*{PCT_COMBO ,TEXT("OPERATOR ON/OFF"               ),0    ,FALSE   ,0       ,127     , NULL                ,  0, ROW_POS_EDIT19 ,COLPOS02,0 },*//*155        OPERATOR ON/OFF                      */
    {PCT_BUTTON,TEXT("TO ALL VOICE"                  ),0    ,TRUE    ,0       ,0       , NULL                , 50, ROW_POS_EDIT00 ,COLPOS11,0   },/**/
    {PCT_COMBO ,TEXT("COMBO_TO_ALL_VOICE"            ),0    ,TRUE    ,0       ,31      , strTblVoiceNum      ,  0, ROW_POS_EDIT00 ,COLPOS10,0   },/**/
    {PCT_STATIC,TEXT("PATCHNAME"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT00 ,COLPOS00,0   },/**/
    {PCT_STATIC,TEXT("LFO"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS03,5   },/**/
    {PCT_STATIC,TEXT("         MODULATION SENSITVITY"),0    ,TRUE    ,0       ,0       , NULL                ,200, ROW_POS_STATIC2,COLPOS00,5   },/**/
    {PCT_STATIC,TEXT("ALGORITHM"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS01,0   },/**/
    {PCT_STATIC,TEXT("FEEDBACK"                      ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS02,0   },/**/
    {PCT_STATIC,TEXT("WAVE"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS03,0   },/**/
    {PCT_STATIC,TEXT("SPEED"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS04,0   },/**/
    {PCT_STATIC,TEXT("DELAY"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS05,0   },/**/
    {PCT_STATIC,TEXT("PMD"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS06,0   },/**/
    {PCT_STATIC,TEXT("AMD"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS07,0   },/**/
    {PCT_STATIC,TEXT("SYNC"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS08,0   },/**/
    {PCT_STATIC,TEXT("PITCH"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS01,0   },/**/
    {PCT_STATIC,TEXT("AMPLITUDE"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS02,0   },/**/
    {PCT_STATIC,TEXT("OPERATOR1"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS00,0   },/*OPERATOR1*/
    {PCT_STATIC,TEXT("OPERATOR2"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS00,0   },/*OPERATOR2*/
    {PCT_STATIC,TEXT("OPERATOR3"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS00,0   },/*OPERATOR3*/
    {PCT_STATIC,TEXT("OPERATOR4"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS00,0   },/*OPERATOR4*/
    {PCT_STATIC,TEXT("OPERATOR5"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS00,0   },/*OPERATOR5*/
    {PCT_STATIC,TEXT("OPERATOR6"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS00,0   },/*OPERATOR6*/
    {PCT_STATIC,TEXT("OPERATOR1"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT13 ,COLPOS00,0   },/*OPERATOR1*/
    {PCT_STATIC,TEXT("OPERATOR2"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT14 ,COLPOS00,0   },/*OPERATOR2*/
    {PCT_STATIC,TEXT("OPERATOR3"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT15 ,COLPOS00,0   },/*OPERATOR3*/
    {PCT_STATIC,TEXT("OPERATOR4"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT16 ,COLPOS00,0   },/*OPERATOR4*/
    {PCT_STATIC,TEXT("OPERATOR5"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT17 ,COLPOS00,0   },/*OPERATOR5*/
    {PCT_STATIC,TEXT("OPERATOR6"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT18 ,COLPOS00,0   },/*OPERATOR6*/
    {PCT_STATIC,TEXT("OSCILLATOR"                    ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS03,5   },/**/
    {PCT_STATIC,TEXT("MODE"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS03,0   },/**/
    {PCT_STATIC,TEXT("SYNC"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS04,0   },/**/
    {PCT_STATIC,TEXT(""                              ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT07 ,COLPOS05,0   },/**/
    {PCT_STATIC,TEXT(""                              ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT08 ,COLPOS05,0   },/**/
    {PCT_STATIC,TEXT(""                              ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT09 ,COLPOS05,0   },/**/
    {PCT_STATIC,TEXT(""                              ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT10 ,COLPOS05,0   },/**/
    {PCT_STATIC,TEXT(""                              ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT11 ,COLPOS05,0   },/**/
    {PCT_STATIC,TEXT(""                              ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT12 ,COLPOS05,0   },/**/
    {PCT_STATIC,TEXT("FREQUENCY"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS06,5   },/**/
    {PCT_STATIC,TEXT("COARSE"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS06,0   },/**/
    {PCT_STATIC,TEXT("FINE"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS07,0   },/**/
    {PCT_STATIC,TEXT("DETUNE"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS08,0   },/**/
    {PCT_STATIC,TEXT("EG"                            ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS09,5   },/**/
    {PCT_STATIC,TEXT("RATE1"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS09,0   },/**/
    {PCT_STATIC,TEXT("RATE2"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS10,0   },/**/
    {PCT_STATIC,TEXT("RATE3"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS11,0   },/**/
    {PCT_STATIC,TEXT("RATE4"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS12,0   },/**/
    {PCT_STATIC,TEXT("LEVEL1"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS13,0   },/**/
    {PCT_STATIC,TEXT("LEVEL2"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS14,0   },/**/
    {PCT_STATIC,TEXT("LEVEL3"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS15,0   },/**/
    {PCT_STATIC,TEXT("LEVEL4"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS16,0   },/**/
    {PCT_STATIC,TEXT("KEYBOARD LEVEL SCALING"        ),0    ,TRUE    ,0       ,0       , NULL                ,300, ROW_POS_STATIC4,COLPOS01,5   },/**/
    {PCT_STATIC,TEXT("BREAK"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS01,0   },/**/
    {PCT_STATIC,TEXT("CURVE L"                       ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS02,0   },/**/
    {PCT_STATIC,TEXT("CURVE R"                       ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS03,0   },/**/
    {PCT_STATIC,TEXT("DEPTH L"                       ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS04,0   },/**/
    {PCT_STATIC,TEXT("DEPTH R"                       ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS05,0   },/**/
    {PCT_STATIC,TEXT("RATE"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC4,COLPOS06,5   },/**/
    {PCT_STATIC,TEXT("SCALING"                       ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS06,0   },/**/
    {PCT_STATIC,TEXT("OPERATOR"                      ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC4,COLPOS07,5   },/**/
    {PCT_STATIC,TEXT("OUT LEVEL"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS07,0   },/**/
    {PCT_STATIC,TEXT("VELO SENS"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS08,0   },/**/
    {PCT_STATIC,TEXT("PITCH EG"                      ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC4,COLPOS09,5   },/**/
    {PCT_STATIC,TEXT("RATE1"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS09,0   },/**/
    {PCT_STATIC,TEXT("RATE2"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS10,0   },/**/
    {PCT_STATIC,TEXT("RATE3"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS11,0   },/**/
    {PCT_STATIC,TEXT("RATE4"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS12,0   },/**/
    {PCT_STATIC,TEXT("LEVEL1"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS13,0   },/**/
    {PCT_STATIC,TEXT("LEVEL2"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS14,0   },/**/
    {PCT_STATIC,TEXT("LEVEL3"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS15,0   },/**/
    {PCT_STATIC,TEXT("LEVEL4"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS16,0   },/**/
    {PCT_STATIC,TEXT("KEY TRANSPOSE"                 ),0    ,TRUE    ,0       ,0       , NULL                ,200, ROW_POS_STATIC7,COLPOS01,0   },/**/
    {PCT_EDIT  ,TEXT("01"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  0              , 0     ,0   },/*"VOICE_NAME_01"                                 */
    {PCT_EDIT  ,TEXT("02"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  1              , 0     ,0   },/*"VOICE_NAME_02"                                 */
    {PCT_EDIT  ,TEXT("03"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  2              , 0     ,0   },/*"VOICE_NAME_03"                                 */
    {PCT_EDIT  ,TEXT("04"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  3              , 0     ,0   },/*"VOICE_NAME_04"                                 */
    {PCT_EDIT  ,TEXT("05"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  4              , 0     ,0   },/*"VOICE_NAME_05"                                 */
    {PCT_EDIT  ,TEXT("06"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  5              , 0     ,0   },/*"VOICE_NAME_06"                                 */
    {PCT_EDIT  ,TEXT("07"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  6              , 0     ,0   },/*"VOICE_NAME_07"                                 */
    {PCT_EDIT  ,TEXT("08"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  7              , 0     ,0   },/*"VOICE_NAME_08"                                 */
    {PCT_EDIT  ,TEXT("09"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  8              , 0     ,0   },/*"VOICE_NAME_09"                                 */
    {PCT_EDIT  ,TEXT("10"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  9              , 0     ,0   },/*"VOICE_NAME_10"                                 */
    {PCT_EDIT  ,TEXT("11"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 10              , 0     ,0   },/*"VOICE_NAME_11"                                 */
    {PCT_EDIT  ,TEXT("12"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 11              , 0     ,0   },/*"VOICE_NAME_12"                                 */
    {PCT_EDIT  ,TEXT("13"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 12              , 0     ,0   },/*"VOICE_NAME_13"                                 */
    {PCT_EDIT  ,TEXT("14"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 13              , 0     ,0   },/*"VOICE_NAME_14"                                 */
    {PCT_EDIT  ,TEXT("15"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 14              , 0     ,0   },/*"VOICE_NAME_15"                                 */
    {PCT_EDIT  ,TEXT("16"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 15              , 0     ,0   },/*"VOICE_NAME_16"                                 */
    {PCT_EDIT  ,TEXT("17"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  0              , 6     ,0   },/*"VOICE_NAME_17"                                 */
    {PCT_EDIT  ,TEXT("18"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  1              , 6     ,0   },/*"VOICE_NAME_18"                                 */
    {PCT_EDIT  ,TEXT("19"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  2              , 6     ,0   },/*"VOICE_NAME_19"                                 */
    {PCT_EDIT  ,TEXT("20"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  3              , 6     ,0   },/*"VOICE_NAME_20"                                 */
    {PCT_EDIT  ,TEXT("21"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  4              , 6     ,0   },/*"VOICE_NAME_21"                                 */
    {PCT_EDIT  ,TEXT("22"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  5              , 6     ,0   },/*"VOICE_NAME_22"                                 */
    {PCT_EDIT  ,TEXT("23"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  6              , 6     ,0   },/*"VOICE_NAME_23"                                 */
    {PCT_EDIT  ,TEXT("24"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  7              , 6     ,0   },/*"VOICE_NAME_24"                                 */
    {PCT_EDIT  ,TEXT("25"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  8              , 6     ,0   },/*"VOICE_NAME_25"                                 */
    {PCT_EDIT  ,TEXT("26"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50,  9              , 6     ,0   },/*"VOICE_NAME_26"                                 */
    {PCT_EDIT  ,TEXT("27"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 10              , 6     ,0   },/*"VOICE_NAME_27"                                 */
    {PCT_EDIT  ,TEXT("28"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 11              , 6     ,0   },/*"VOICE_NAME_28"                                 */
    {PCT_EDIT  ,TEXT("29"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 12              , 6     ,0   },/*"VOICE_NAME_29"                                 */
    {PCT_EDIT  ,TEXT("30"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 13              , 6     ,0   },/*"VOICE_NAME_30"                                 */
    {PCT_EDIT  ,TEXT("31"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 14              , 6     ,0   },/*"VOICE_NAME_31"                                 */
    {PCT_EDIT  ,TEXT("32"                            ),15   ,TRUE    ,0       ,127     , NULL                , 50, 15              , 6     ,0   },/*"VOICE_NAME_32"                                 */
    {PCT_EDIT  ,TEXT("EX1_PARAM_01"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  0              , 2     ,0   },/*"ALL_VOICE_EX1_PARAM_00"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_02"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  1              , 2     ,0   },/*"ALL_VOICE_EX1_PARAM_01"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_03"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  2              , 2     ,0   },/*"ALL_VOICE_EX1_PARAM_02"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_04"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  3              , 2     ,0   },/*"ALL_VOICE_EX1_PARAM_03"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_05"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  4              , 2     ,0   },/*"ALL_VOICE_EX1_PARAM_04"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_06"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  5              , 2     ,0   },/*"ALL_VOICE_EX1_PARAM_05"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_07"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  6              , 2     ,0   },/*"ALL_VOICE_EX1_PARAM_06"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_08"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  7              , 2     ,0   },/*"ALL_VOICE_EX1_PARAM_07"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_09"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  8              , 2     ,0   },/*"ALL_VOICE_EX1_PARAM_08"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_10"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  9              , 2     ,0   },/*"ALL_VOICE_EX1_PARAM_09"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_11"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 10              , 2     ,0   },/*"ALL_VOICE_EX1_PARAM_10"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_12"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 11              , 2     ,0   },/*"ALL_VOICE_EX1_PARAM_11"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_13"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 12              , 2     ,0   },/*"ALL_VOICE_EX1_PARAM_12"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_14"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 13              , 2     ,0   },/*"ALL_VOICE_EX1_PARAM_13"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_15"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 14              , 2     ,0   },/*"ALL_VOICE_EX1_PARAM_14"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_16"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 15              , 2     ,0   },/*"ALL_VOICE_EX1_PARAM_15"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_17"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  0              , 8     ,0   },/*"ALL_VOICE_EX1_PARAM_16"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_18"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  1              , 8     ,0   },/*"ALL_VOICE_EX1_PARAM_17"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_19"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  2              , 8     ,0   },/*"ALL_VOICE_EX1_PARAM_18"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_20"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  3              , 8     ,0   },/*"ALL_VOICE_EX1_PARAM_19"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_21"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  4              , 8     ,0   },/*"ALL_VOICE_EX1_PARAM_20"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_22"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  5              , 8     ,0   },/*"ALL_VOICE_EX1_PARAM_21"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_23"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  6              , 8     ,0   },/*"ALL_VOICE_EX1_PARAM_22"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_24"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  7              , 8     ,0   },/*"ALL_VOICE_EX1_PARAM_23"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_25"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  8              , 8     ,0   },/*"ALL_VOICE_EX1_PARAM_24"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_26"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  9              , 8     ,0   },/*"ALL_VOICE_EX1_PARAM_25"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_27"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 10              , 8     ,0   },/*"ALL_VOICE_EX1_PARAM_26"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_28"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 11              , 8     ,0   },/*"ALL_VOICE_EX1_PARAM_27"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_29"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 12              , 8     ,0   },/*"ALL_VOICE_EX1_PARAM_28"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_30"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 13              , 8     ,0   },/*"ALL_VOICE_EX1_PARAM_29"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_31"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 14              , 8     ,0   },/*"ALL_VOICE_EX1_PARAM_30"                        */
    {PCT_EDIT  ,TEXT("EX1_PARAM_32"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 15              , 8     ,0   },/*"ALL_VOICE_EX1_PARAM_31"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_01"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  0              , 3     ,0   },/*"ALL_VOICE_EX2_PARAM_00"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_02"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  1              , 3     ,0   },/*"ALL_VOICE_EX2_PARAM_01"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_03"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  2              , 3     ,0   },/*"ALL_VOICE_EX2_PARAM_02"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_04"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  3              , 3     ,0   },/*"ALL_VOICE_EX2_PARAM_03"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_05"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  4              , 3     ,0   },/*"ALL_VOICE_EX2_PARAM_04"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_06"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  5              , 3     ,0   },/*"ALL_VOICE_EX2_PARAM_05"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_07"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  6              , 3     ,0   },/*"ALL_VOICE_EX2_PARAM_06"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_08"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  7              , 3     ,0   },/*"ALL_VOICE_EX2_PARAM_07"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_09"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  8              , 3     ,0   },/*"ALL_VOICE_EX2_PARAM_08"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_10"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  9              , 3     ,0   },/*"ALL_VOICE_EX2_PARAM_09"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_11"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 10              , 3     ,0   },/*"ALL_VOICE_EX2_PARAM_10"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_12"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 11              , 3     ,0   },/*"ALL_VOICE_EX2_PARAM_11"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_13"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 12              , 3     ,0   },/*"ALL_VOICE_EX2_PARAM_12"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_14"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 13              , 3     ,0   },/*"ALL_VOICE_EX2_PARAM_13"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_15"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 14              , 3     ,0   },/*"ALL_VOICE_EX2_PARAM_14"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_16"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 15              , 3     ,0   },/*"ALL_VOICE_EX2_PARAM_15"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_17"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  0              , 9     ,0   },/*"ALL_VOICE_EX2_PARAM_16"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_18"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  1              , 9     ,0   },/*"ALL_VOICE_EX2_PARAM_17"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_19"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  2              , 9     ,0   },/*"ALL_VOICE_EX2_PARAM_18"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_20"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  3              , 9     ,0   },/*"ALL_VOICE_EX2_PARAM_19"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_21"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  4              , 9     ,0   },/*"ALL_VOICE_EX2_PARAM_20"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_22"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  5              , 9     ,0   },/*"ALL_VOICE_EX2_PARAM_21"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_23"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  6              , 9     ,0   },/*"ALL_VOICE_EX2_PARAM_22"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_24"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  7              , 9     ,0   },/*"ALL_VOICE_EX2_PARAM_23"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_25"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  8              , 9     ,0   },/*"ALL_VOICE_EX2_PARAM_24"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_26"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10,  9              , 9     ,0   },/*"ALL_VOICE_EX2_PARAM_25"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_27"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 10              , 9     ,0   },/*"ALL_VOICE_EX2_PARAM_26"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_28"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 11              , 9     ,0   },/*"ALL_VOICE_EX2_PARAM_27"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_29"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 12              , 9     ,0   },/*"ALL_VOICE_EX2_PARAM_28"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_30"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 13              , 9     ,0   },/*"ALL_VOICE_EX2_PARAM_29"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_31"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 14              , 9     ,0   },/*"ALL_VOICE_EX2_PARAM_30"                        */
    {PCT_EDIT  ,TEXT("EX2_PARAM_32"                  ),0    ,TRUE    ,0       ,127     , NULL                , 10, 15              , 9     ,0   },/*"ALL_VOICE_EX2_PARAM_31"                        */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  0              , 4     ,0   },/*"ALL_TO_ONEVOICE_01"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  1              , 4     ,0   },/*"ALL_TO_ONEVOICE_02"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  2              , 4     ,0   },/*"ALL_TO_ONEVOICE_03"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  3              , 4     ,0   },/*"ALL_TO_ONEVOICE_04"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  4              , 4     ,0   },/*"ALL_TO_ONEVOICE_05"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  5              , 4     ,0   },/*"ALL_TO_ONEVOICE_06"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  6              , 4     ,0   },/*"ALL_TO_ONEVOICE_07"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  7              , 4     ,0   },/*"ALL_TO_ONEVOICE_08"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  8              , 4     ,0   },/*"ALL_TO_ONEVOICE_09"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  9              , 4     ,0   },/*"ALL_TO_ONEVOICE_10"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 10              , 4     ,0   },/*"ALL_TO_ONEVOICE_11"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 11              , 4     ,0   },/*"ALL_TO_ONEVOICE_12"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 12              , 4     ,0   },/*"ALL_TO_ONEVOICE_13"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 13              , 4     ,0   },/*"ALL_TO_ONEVOICE_14"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 14              , 4     ,0   },/*"ALL_TO_ONEVOICE_15"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 15              , 4     ,0   },/*"ALL_TO_ONEVOICE_16"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  0              ,10     ,0   },/*"ALL_TO_ONEVOICE_17"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  1              ,10     ,0   },/*"ALL_TO_ONEVOICE_18"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  2              ,10     ,0   },/*"ALL_TO_ONEVOICE_19"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  3              ,10     ,0   },/*"ALL_TO_ONEVOICE_20"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  4              ,10     ,0   },/*"ALL_TO_ONEVOICE_21"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  5              ,10     ,0   },/*"ALL_TO_ONEVOICE_22"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  6              ,10     ,0   },/*"ALL_TO_ONEVOICE_23"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  7              ,10     ,0   },/*"ALL_TO_ONEVOICE_24"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  8              ,10     ,0   },/*"ALL_TO_ONEVOICE_25"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20,  9              ,10     ,0   },/*"ALL_TO_ONEVOICE_26"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 10              ,10     ,0   },/*"ALL_TO_ONEVOICE_27"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 11              ,10     ,0   },/*"ALL_TO_ONEVOICE_28"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 12              ,10     ,0   },/*"ALL_TO_ONEVOICE_29"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 13              ,10     ,0   },/*"ALL_TO_ONEVOICE_30"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 14              ,10     ,0   },/*"ALL_TO_ONEVOICE_31"                            */
    {PCT_BUTTON,TEXT("TO 1VOICE"                     ),0    ,TRUE    ,0       ,127     , NULL                , 20, 15              ,10     ,0   },/*"ALL_TO_ONEVOICE_32"                            */
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
    BOOL                exist                        ; /* ウィンドウが生成されているかどうか */
    HWND                hwnd_static_text             ; /* ウィンドウのハンドラ               */
    HWND                hwnd_data                    ; /* ウィンドウのハンドラ               */
    S_DX7_PARAM_WINDOW_INFO wtInfo                       ;
    DX7_PARAM_CTRL_GROUP_ID groupId                      ;
} S_DX7_PARAM_INFO;

static S_DX7_PARAM_INFO dx7ctrlParamInfo[DX7_PARAM_CTRL_MAX];

/********************************************************************************
 * 内容  : 登録された全てのパラメータコントロールを生成する
 * 引数  : HINSTANCE hInst
 * 引数  : PTSTR szAppName
 * 引数  : HWND hwnd 親ウィンドウのハンドラ
 * 戻り値: BOOL
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
 * 内容  : 登録された全てのパラメータコントロールを生成する
 * 引数  : HWND hwnd 親ウィンドウのハンドラ
 * 引数  : DX7_PARAM_CTRL_GROUP_ID groupId
 * 引数  : DX7_PARAM_CTRL_ID       startId
 * 引数  : DX7_PARAM_CTRL_ID       endId
 * 戻り値: BOOL
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
            S_DX7_PARAM_INFO *infoPtr = &dx7ctrlParamInfo[nowId];
            S_DX7_PARAM_CTRL *tblPtr = &paramListTbl[nowId];

            if( tblPtr->type == PCT_COMBO )
            {
                class = (PTSTR)TEXT("combobox");
                strText = (PTSTR)0;
                style = (WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
                exStyle = 0;
            }
            else if( tblPtr->type == PCT_BUTTON )
            {
                class = (PTSTR)TEXT("button");
                strText = (PTSTR)tblPtr->strText;
                style = (WS_CHILD);
                exStyle = 0;
            }
            else if( tblPtr->type == PCT_STATIC )
            {
                class = (PTSTR)TEXT("static");
                strText = (PTSTR)tblPtr->strText;
                style = (WS_CHILD|ES_LEFT);
                exStyle = 0;
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
            }

            if( tblPtr->withTextLen )
            {
                infoPtr->hwnd_static_text = CreateWindowEx( 0,                                          /* 拡張スタイル       */
                                            TEXT("static"),                             /* クラス名           */
                                            tblPtr->strText,                            /* ウィンドウテキスト */
                                            (WS_CHILD|ES_LEFT),                         /* スタイル           */
                                            0, /* x座標              */
                                            0, /* y座標              */
                                            0, /* 幅                 */
                                            0, /* 高さ               */
                                            hwnd,(HMENU)0,
                                            dx7ParamCtrlData.hInstance,NULL );               /* インスタンスハンドル,補助引数 */

                if( infoPtr->hwnd_static_text != NULL )
                {
                    SendMessage(infoPtr->hwnd_static_text, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));
                }
                else
                {
                }
            }
            else
            {
                nop();
            }

#ifdef DEBUG_DISP_STATIC_STRING
            infoPtr->hwnd_data = CreateWindowEx( WS_EX_OVERLAPPEDWINDOW,                    /* 拡張スタイル       */
                                                (PTSTR)TEXT("edit")/*TEXT("static")*/,      /* クラス名           */
                                                tblPtr->strText,                             /* ウィンドウテキスト */
                                                (WS_CHILD|ES_LEFT|ES_AUTOHSCROLL)/*(WS_CHILD|ES_LEFT)*/,/* スタイル           */
                                                0, /* x座標              */
                                                0, /* y座標              */
                                                0, /* 幅                 */
                                                0, /* 高さ               */
                                                hwnd,(HMENU)(DX7_PARAM_CTRL_ID_OFFSET+(nowId*2)+1),/* 親ウィンドウ,子ウィンドウID */
                                                dx100ParamCtrlData.hInstance,NULL );                        /* インスタンスハンドル,補助引数 */
#else
            infoPtr->hwnd_data = CreateWindowEx( exStyle,                                    /* 拡張スタイル       */
                                                class,                                      /* クラス名           */
                                                strText,                                    /* ウィンドウテキスト */
                                                style,                                      /* スタイル           */
                                                0, /* x座標              */
                                                0, /* y座標              */
                                                0, /* 幅                 */
                                                0, /* 高さ               */
                                                hwnd,(HMENU)(DX7_PARAM_CTRL_ID_OFFSET+nowId),/* 親ウィンドウ,子ウィンドウID */
                                                dx7ParamCtrlData.hInstance,NULL );                        /* インスタンスハンドル,補助引数 */
#endif
            if( infoPtr->hwnd_data != NULL )
            {
                INT j,iCbNum;
                TCHAR szBuff[128];

                SendMessage(infoPtr->hwnd_data, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));

                /* 個別に処理するもの */
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
 * 内容  : 指定のグループのパラメータコントロールを表示
 * 引数  : DX7_PARAM_CTRL_GROUP_ID groupId
 * 戻り値: BOOL
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
 * 内容  : ウィンドウハンドラを取得する
 * 引数  : DX7_PARAM_CTRL_ID id
 * 戻り値: HWND
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
 * 内容  : ウィンドウからテキストを取得する
 * 引数  : DX7_PARAM_CTRL_ID id
 * 引数  : PTSTR ptstrText
 * 戻り値: BOOL
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
 * 内容  : コントロールの位置を変更する
 * 引数  : BOOL bAlternate
 * 戻り値: なし
 ***************************************/
BOOL
Dx7ParamCtrlChangePosition( BOOL bAlternate )
{
    int nowId;
    INT rowPos;

    for(nowId=0; nowId<=DX7_PARAM_CTRL_MAX; nowId++ )
    {
        DWORD height;
        INT   width;
        INT   xBasePos,yBasePos;
        INT xPos;
        S_DX7_PARAM_INFO *infoPtr = &dx7ctrlParamInfo[nowId];
        S_DX7_PARAM_CTRL *tblPtr = &paramListTbl[nowId];

        if( tblPtr->type == PCT_COMBO )
        {
            height = PARAM_DATA_HEIGHT;
        }
        else if( tblPtr->type == PCT_BUTTON )
        {
            height = PARAM_BUTTON_HEIGHT;
        }
        else if( tblPtr->type == PCT_STATIC )
        {
            height = PARAM_STATIC_HEIGHT;
        }
        else
        {
            height = PARAM_EDIT_HEIGHT;
        }

        xBasePos = (PARAM_WRAP_X_UNIT*tblPtr->colPos);
        rowPos = tblPtr->rowPos;

        switch( nowId )
        {
        case DX7_PARAM_CTRL_VOICE_00 :  rowPos = ((bAlternate) ? ROW_POS_EDIT12 : ROW_POS_EDIT07 );break; /* 0  OP6 EG RATE 1                            */
        case DX7_PARAM_CTRL_VOICE_01 :  rowPos = ((bAlternate) ? ROW_POS_EDIT12 : ROW_POS_EDIT07 );break; /* 1  OP6 EG RATE 2                            */
        case DX7_PARAM_CTRL_VOICE_02 :  rowPos = ((bAlternate) ? ROW_POS_EDIT12 : ROW_POS_EDIT07 );break; /* 2  OP6 EG RATE 3                            */
        case DX7_PARAM_CTRL_VOICE_03 :  rowPos = ((bAlternate) ? ROW_POS_EDIT12 : ROW_POS_EDIT07 );break; /* 3  OP6 EG RATE 4                            */
        case DX7_PARAM_CTRL_VOICE_04 :  rowPos = ((bAlternate) ? ROW_POS_EDIT12 : ROW_POS_EDIT07 );break; /* 4  OP6 EG LEVEL 1                           */
        case DX7_PARAM_CTRL_VOICE_05 :  rowPos = ((bAlternate) ? ROW_POS_EDIT12 : ROW_POS_EDIT07 );break; /* 5  OP6 EG LEVEL 2                           */
        case DX7_PARAM_CTRL_VOICE_06 :  rowPos = ((bAlternate) ? ROW_POS_EDIT12 : ROW_POS_EDIT07 );break; /* 6  OP6 EG LEVEL 3                           */
        case DX7_PARAM_CTRL_VOICE_07 :  rowPos = ((bAlternate) ? ROW_POS_EDIT12 : ROW_POS_EDIT07 );break; /* 7  OP6 EG LEVEL 4                           */
        case DX7_PARAM_CTRL_VOICE_08 :  rowPos = ((bAlternate) ? ROW_POS_EDIT18 : ROW_POS_EDIT13 );break; /* 8  OP6 BREAK POINT                          */
        case DX7_PARAM_CTRL_VOICE_09 :  rowPos = ((bAlternate) ? ROW_POS_EDIT18 : ROW_POS_EDIT13 );break; /* 9  OP6 LEFT DEPTH                           */
        case DX7_PARAM_CTRL_VOICE_10 :  rowPos = ((bAlternate) ? ROW_POS_EDIT18 : ROW_POS_EDIT13 );break; /*10  OP6 RIGHT DEPTH                          */
        case DX7_PARAM_CTRL_VOICE_11 :  rowPos = ((bAlternate) ? ROW_POS_EDIT18 : ROW_POS_EDIT13 );break; /*11  OP6 LEFT CURVE                           */
        case DX7_PARAM_CTRL_VOICE_12 :  rowPos = ((bAlternate) ? ROW_POS_EDIT18 : ROW_POS_EDIT13 );break; /*12  OP6 RIGHT CURVE                          */
        case DX7_PARAM_CTRL_VOICE_13 :  rowPos = ((bAlternate) ? ROW_POS_EDIT18 : ROW_POS_EDIT13 );break; /*13  OP6 KEY BOARD RATE SCALING               */
        case DX7_PARAM_CTRL_VOICE_14 :  rowPos = ((bAlternate) ? ROW_POS_EDIT12 : ROW_POS_EDIT07 );break; /*14  OP6 MOD SENSITIVITY AMPLITUDE            */
        case DX7_PARAM_CTRL_VOICE_15 :  rowPos = ((bAlternate) ? ROW_POS_EDIT18 : ROW_POS_EDIT13 );break; /*15  OP6 OPERATOR KEY VELOCITY SENSITIVITY    */
        case DX7_PARAM_CTRL_VOICE_16 :  rowPos = ((bAlternate) ? ROW_POS_EDIT18 : ROW_POS_EDIT13 );break; /*16  OP6 OPERATOR OUTPUT LEVEL                */
        case DX7_PARAM_CTRL_VOICE_17 :  rowPos = ((bAlternate) ? ROW_POS_EDIT12 : ROW_POS_EDIT07 );break; /*17  OP6 OSILLATOR MODE                       */
        case DX7_PARAM_CTRL_VOICE_18 :  rowPos = ((bAlternate) ? ROW_POS_EDIT12 : ROW_POS_EDIT07 );break; /*18  OP6 OSILLATOR FREQUENCY COARSE           */
        case DX7_PARAM_CTRL_VOICE_19 :  rowPos = ((bAlternate) ? ROW_POS_EDIT12 : ROW_POS_EDIT07 );break; /*19  OP6 OSILLATOR FREQUENCY FINE             */
        case DX7_PARAM_CTRL_VOICE_20 :  rowPos = ((bAlternate) ? ROW_POS_EDIT12 : ROW_POS_EDIT07 );break; /*20  OP6 DETUNE                               */
        case DX7_PARAM_CTRL_VOICE_21 :  rowPos = ((bAlternate) ? ROW_POS_EDIT11 : ROW_POS_EDIT08 );break; /*21  OP5 EG RATE 1                            */
        case DX7_PARAM_CTRL_VOICE_22 :  rowPos = ((bAlternate) ? ROW_POS_EDIT11 : ROW_POS_EDIT08 );break; /*22  OP5 EG RATE 2                            */
        case DX7_PARAM_CTRL_VOICE_23 :  rowPos = ((bAlternate) ? ROW_POS_EDIT11 : ROW_POS_EDIT08 );break; /*23  OP5 EG RATE 3                            */
        case DX7_PARAM_CTRL_VOICE_24 :  rowPos = ((bAlternate) ? ROW_POS_EDIT11 : ROW_POS_EDIT08 );break; /*24  OP5 EG RATE 4                            */
        case DX7_PARAM_CTRL_VOICE_25 :  rowPos = ((bAlternate) ? ROW_POS_EDIT11 : ROW_POS_EDIT08 );break; /*25  OP5 EG LEVEL 1                           */
        case DX7_PARAM_CTRL_VOICE_26 :  rowPos = ((bAlternate) ? ROW_POS_EDIT11 : ROW_POS_EDIT08 );break; /*26  OP5 EG LEVEL 2                           */
        case DX7_PARAM_CTRL_VOICE_27 :  rowPos = ((bAlternate) ? ROW_POS_EDIT11 : ROW_POS_EDIT08 );break; /*27  OP5 EG LEVEL 3                           */
        case DX7_PARAM_CTRL_VOICE_28 :  rowPos = ((bAlternate) ? ROW_POS_EDIT11 : ROW_POS_EDIT08 );break; /*28  OP5 EG LEVEL 4                           */
        case DX7_PARAM_CTRL_VOICE_29 :  rowPos = ((bAlternate) ? ROW_POS_EDIT17 : ROW_POS_EDIT14 );break; /*29  OP5 BREAK POINT                          */
        case DX7_PARAM_CTRL_VOICE_30 :  rowPos = ((bAlternate) ? ROW_POS_EDIT17 : ROW_POS_EDIT14 );break; /*30  OP5 LEFT DEPTH                           */
        case DX7_PARAM_CTRL_VOICE_31 :  rowPos = ((bAlternate) ? ROW_POS_EDIT17 : ROW_POS_EDIT14 );break; /*31  OP5 RIGHT DEPTH                          */
        case DX7_PARAM_CTRL_VOICE_32 :  rowPos = ((bAlternate) ? ROW_POS_EDIT17 : ROW_POS_EDIT14 );break; /*32  OP5 LEFT CURVE                           */
        case DX7_PARAM_CTRL_VOICE_33 :  rowPos = ((bAlternate) ? ROW_POS_EDIT17 : ROW_POS_EDIT14 );break; /*33  OP5 RIGHT CURVE                          */
        case DX7_PARAM_CTRL_VOICE_34 :  rowPos = ((bAlternate) ? ROW_POS_EDIT17 : ROW_POS_EDIT14 );break; /*34  OP5 KEY BOARD RATE SCALING               */
        case DX7_PARAM_CTRL_VOICE_35 :  rowPos = ((bAlternate) ? ROW_POS_EDIT11 : ROW_POS_EDIT08 );break; /*35  OP5 MOD SENSITIVITY AMPLITUDE            */
        case DX7_PARAM_CTRL_VOICE_36 :  rowPos = ((bAlternate) ? ROW_POS_EDIT17 : ROW_POS_EDIT14 );break; /*36  OP5 OPERATOR KEY VELOCITY SENSITIVITY    */
        case DX7_PARAM_CTRL_VOICE_37 :  rowPos = ((bAlternate) ? ROW_POS_EDIT17 : ROW_POS_EDIT14 );break; /*37  OP5 OPERATOR OUTPUT LEVEL                */
        case DX7_PARAM_CTRL_VOICE_38 :  rowPos = ((bAlternate) ? ROW_POS_EDIT11 : ROW_POS_EDIT08 );break; /*38  OP5 OSILLATOR MODE                       */
        case DX7_PARAM_CTRL_VOICE_39 :  rowPos = ((bAlternate) ? ROW_POS_EDIT11 : ROW_POS_EDIT08 );break; /*39  OP5 OSILLATOR FREQUENCY COARSE           */
        case DX7_PARAM_CTRL_VOICE_40 :  rowPos = ((bAlternate) ? ROW_POS_EDIT11 : ROW_POS_EDIT08 );break; /*40  OP5 OSILLATOR FREQUENCY FINE             */
        case DX7_PARAM_CTRL_VOICE_41 :  rowPos = ((bAlternate) ? ROW_POS_EDIT11 : ROW_POS_EDIT08 );break; /*41  OP5 DETUNE                               */
        case DX7_PARAM_CTRL_VOICE_42 :  rowPos = ((bAlternate) ? ROW_POS_EDIT10 : ROW_POS_EDIT09 );break; /*42  OP4 EG RATE 1                            */
        case DX7_PARAM_CTRL_VOICE_43 :  rowPos = ((bAlternate) ? ROW_POS_EDIT10 : ROW_POS_EDIT09 );break; /*43  OP4 EG RATE 2                            */
        case DX7_PARAM_CTRL_VOICE_44 :  rowPos = ((bAlternate) ? ROW_POS_EDIT10 : ROW_POS_EDIT09 );break; /*44  OP4 EG RATE 3                            */
        case DX7_PARAM_CTRL_VOICE_45 :  rowPos = ((bAlternate) ? ROW_POS_EDIT10 : ROW_POS_EDIT09 );break; /*45  OP4 EG RATE 4                            */
        case DX7_PARAM_CTRL_VOICE_46 :  rowPos = ((bAlternate) ? ROW_POS_EDIT10 : ROW_POS_EDIT09 );break; /*46  OP4 EG LEVEL 1                           */
        case DX7_PARAM_CTRL_VOICE_47 :  rowPos = ((bAlternate) ? ROW_POS_EDIT10 : ROW_POS_EDIT09 );break; /*47  OP4 EG LEVEL 2                           */
        case DX7_PARAM_CTRL_VOICE_48 :  rowPos = ((bAlternate) ? ROW_POS_EDIT10 : ROW_POS_EDIT09 );break; /*48  OP4 EG LEVEL 3                           */
        case DX7_PARAM_CTRL_VOICE_49 :  rowPos = ((bAlternate) ? ROW_POS_EDIT10 : ROW_POS_EDIT09 );break; /*49  OP4 EG LEVEL 4                           */
        case DX7_PARAM_CTRL_VOICE_50 :  rowPos = ((bAlternate) ? ROW_POS_EDIT16 : ROW_POS_EDIT15 );break; /*50  OP4 BREAK POINT                          */
        case DX7_PARAM_CTRL_VOICE_51 :  rowPos = ((bAlternate) ? ROW_POS_EDIT16 : ROW_POS_EDIT15 );break; /*51  OP4 LEFT DEPTH                           */
        case DX7_PARAM_CTRL_VOICE_52 :  rowPos = ((bAlternate) ? ROW_POS_EDIT16 : ROW_POS_EDIT15 );break; /*52  OP4 RIGHT DEPTH                          */
        case DX7_PARAM_CTRL_VOICE_53 :  rowPos = ((bAlternate) ? ROW_POS_EDIT16 : ROW_POS_EDIT15 );break; /*53  OP4 LEFT CURVE                           */
        case DX7_PARAM_CTRL_VOICE_54 :  rowPos = ((bAlternate) ? ROW_POS_EDIT16 : ROW_POS_EDIT15 );break; /*54  OP4 RIGHT CURVE                          */
        case DX7_PARAM_CTRL_VOICE_55 :  rowPos = ((bAlternate) ? ROW_POS_EDIT16 : ROW_POS_EDIT15 );break; /*55  OP4 KEY BOARD RATE SCALING               */
        case DX7_PARAM_CTRL_VOICE_56 :  rowPos = ((bAlternate) ? ROW_POS_EDIT10 : ROW_POS_EDIT09 );break; /*56  OP4 MOD SENSITIVITY AMPLITUDE            */
        case DX7_PARAM_CTRL_VOICE_57 :  rowPos = ((bAlternate) ? ROW_POS_EDIT16 : ROW_POS_EDIT15 );break; /*57  OP4 OPERATOR KEY VELOCITY SENSITIVITY    */
        case DX7_PARAM_CTRL_VOICE_58 :  rowPos = ((bAlternate) ? ROW_POS_EDIT16 : ROW_POS_EDIT15 );break; /*58  OP4 OPERATOR OUTPUT LEVEL                */
        case DX7_PARAM_CTRL_VOICE_59 :  rowPos = ((bAlternate) ? ROW_POS_EDIT10 : ROW_POS_EDIT09 );break; /*59  OP4 OSILLATOR MODE                       */
        case DX7_PARAM_CTRL_VOICE_60 :  rowPos = ((bAlternate) ? ROW_POS_EDIT10 : ROW_POS_EDIT09 );break; /*60  OP4 OSILLATOR FREQUENCY COARSE           */
        case DX7_PARAM_CTRL_VOICE_61 :  rowPos = ((bAlternate) ? ROW_POS_EDIT10 : ROW_POS_EDIT09 );break; /*61  OP4 OSILLATOR FREQUENCY FINE             */
        case DX7_PARAM_CTRL_VOICE_62 :  rowPos = ((bAlternate) ? ROW_POS_EDIT10 : ROW_POS_EDIT09 );break; /*62  OP4 DETUNE                               */
        case DX7_PARAM_CTRL_VOICE_63 :  rowPos = ((bAlternate) ? ROW_POS_EDIT09 : ROW_POS_EDIT10 );break; /*63  OP3 EG RATE 1                            */
        case DX7_PARAM_CTRL_VOICE_64 :  rowPos = ((bAlternate) ? ROW_POS_EDIT09 : ROW_POS_EDIT10 );break; /*64  OP3 EG RATE 2                            */
        case DX7_PARAM_CTRL_VOICE_65 :  rowPos = ((bAlternate) ? ROW_POS_EDIT09 : ROW_POS_EDIT10 );break; /*65  OP3 EG RATE 3                            */
        case DX7_PARAM_CTRL_VOICE_66 :  rowPos = ((bAlternate) ? ROW_POS_EDIT09 : ROW_POS_EDIT10 );break; /*66  OP3 EG RATE 4                            */
        case DX7_PARAM_CTRL_VOICE_67 :  rowPos = ((bAlternate) ? ROW_POS_EDIT09 : ROW_POS_EDIT10 );break; /*67  OP3 EG LEVEL 1                           */
        case DX7_PARAM_CTRL_VOICE_68 :  rowPos = ((bAlternate) ? ROW_POS_EDIT09 : ROW_POS_EDIT10 );break; /*68  OP3 EG LEVEL 2                           */
        case DX7_PARAM_CTRL_VOICE_69 :  rowPos = ((bAlternate) ? ROW_POS_EDIT09 : ROW_POS_EDIT10 );break; /*69  OP3 EG LEVEL 3                           */
        case DX7_PARAM_CTRL_VOICE_70 :  rowPos = ((bAlternate) ? ROW_POS_EDIT09 : ROW_POS_EDIT10 );break; /*70  OP3 EG LEVEL 4                           */
        case DX7_PARAM_CTRL_VOICE_71 :  rowPos = ((bAlternate) ? ROW_POS_EDIT15 : ROW_POS_EDIT16 );break; /*71  OP3 BREAK POINT                          */
        case DX7_PARAM_CTRL_VOICE_72 :  rowPos = ((bAlternate) ? ROW_POS_EDIT15 : ROW_POS_EDIT16 );break; /*72  OP3 LEFT DEPTH                           */
        case DX7_PARAM_CTRL_VOICE_73 :  rowPos = ((bAlternate) ? ROW_POS_EDIT15 : ROW_POS_EDIT16 );break; /*73  OP3 RIGHT DEPTH                          */
        case DX7_PARAM_CTRL_VOICE_74 :  rowPos = ((bAlternate) ? ROW_POS_EDIT15 : ROW_POS_EDIT16 );break; /*74  OP3 LEFT CURVE                           */
        case DX7_PARAM_CTRL_VOICE_75 :  rowPos = ((bAlternate) ? ROW_POS_EDIT15 : ROW_POS_EDIT16 );break; /*75  OP3 RIGHT CURVE                          */
        case DX7_PARAM_CTRL_VOICE_76 :  rowPos = ((bAlternate) ? ROW_POS_EDIT15 : ROW_POS_EDIT16 );break; /*76  OP3 KEY BOARD RATE SCALING               */
        case DX7_PARAM_CTRL_VOICE_77 :  rowPos = ((bAlternate) ? ROW_POS_EDIT09 : ROW_POS_EDIT10 );break; /*77  OP3 MOD SENSITIVITY AMPLITUDE            */
        case DX7_PARAM_CTRL_VOICE_78 :  rowPos = ((bAlternate) ? ROW_POS_EDIT15 : ROW_POS_EDIT16 );break; /*78  OP3 OPERATOR KEY VELOCITY SENSITIVITY    */
        case DX7_PARAM_CTRL_VOICE_79 :  rowPos = ((bAlternate) ? ROW_POS_EDIT15 : ROW_POS_EDIT16 );break; /*79  OP3 OPERATOR OUTPUT LEVEL                */
        case DX7_PARAM_CTRL_VOICE_80 :  rowPos = ((bAlternate) ? ROW_POS_EDIT09 : ROW_POS_EDIT10 );break; /*80  OP3 OSILLATOR MODE                       */
        case DX7_PARAM_CTRL_VOICE_81 :  rowPos = ((bAlternate) ? ROW_POS_EDIT09 : ROW_POS_EDIT10 );break; /*81  OP3 OSILLATOR FREQUENCY COARSE           */
        case DX7_PARAM_CTRL_VOICE_82 :  rowPos = ((bAlternate) ? ROW_POS_EDIT09 : ROW_POS_EDIT10 );break; /*82  OP3 OSILLATOR FREQUENCY FINE             */
        case DX7_PARAM_CTRL_VOICE_83 :  rowPos = ((bAlternate) ? ROW_POS_EDIT09 : ROW_POS_EDIT10 );break; /*83  OP3 DETUNE                               */
        case DX7_PARAM_CTRL_VOICE_84 :  rowPos = ((bAlternate) ? ROW_POS_EDIT08 : ROW_POS_EDIT11 );break; /*84  OP2 EG RATE 1                            */
        case DX7_PARAM_CTRL_VOICE_85 :  rowPos = ((bAlternate) ? ROW_POS_EDIT08 : ROW_POS_EDIT11 );break; /*85  OP2 EG RATE 2                            */
        case DX7_PARAM_CTRL_VOICE_86 :  rowPos = ((bAlternate) ? ROW_POS_EDIT08 : ROW_POS_EDIT11 );break; /*86  OP2 EG RATE 3                            */
        case DX7_PARAM_CTRL_VOICE_87 :  rowPos = ((bAlternate) ? ROW_POS_EDIT08 : ROW_POS_EDIT11 );break; /*87  OP2 EG RATE 4                            */
        case DX7_PARAM_CTRL_VOICE_88 :  rowPos = ((bAlternate) ? ROW_POS_EDIT08 : ROW_POS_EDIT11 );break; /*88  OP2 EG LEVEL 1                           */
        case DX7_PARAM_CTRL_VOICE_89 :  rowPos = ((bAlternate) ? ROW_POS_EDIT08 : ROW_POS_EDIT11 );break; /*89  OP2 EG LEVEL 2                           */
        case DX7_PARAM_CTRL_VOICE_90 :  rowPos = ((bAlternate) ? ROW_POS_EDIT08 : ROW_POS_EDIT11 );break; /*90  OP2 EG LEVEL 3                           */
        case DX7_PARAM_CTRL_VOICE_91 :  rowPos = ((bAlternate) ? ROW_POS_EDIT08 : ROW_POS_EDIT11 );break; /*91  OP2 EG LEVEL 4                           */
        case DX7_PARAM_CTRL_VOICE_92 :  rowPos = ((bAlternate) ? ROW_POS_EDIT14 : ROW_POS_EDIT17 );break; /*92  OP2 BREAK POINT                          */
        case DX7_PARAM_CTRL_VOICE_93 :  rowPos = ((bAlternate) ? ROW_POS_EDIT14 : ROW_POS_EDIT17 );break; /*93  OP2 LEFT DEPTH                           */
        case DX7_PARAM_CTRL_VOICE_94 :  rowPos = ((bAlternate) ? ROW_POS_EDIT14 : ROW_POS_EDIT17 );break; /*94  OP2 RIGHT DEPTH                          */
        case DX7_PARAM_CTRL_VOICE_95 :  rowPos = ((bAlternate) ? ROW_POS_EDIT14 : ROW_POS_EDIT17 );break; /*95  OP2 LEFT CURVE                           */
        case DX7_PARAM_CTRL_VOICE_96 :  rowPos = ((bAlternate) ? ROW_POS_EDIT14 : ROW_POS_EDIT17 );break; /*96  OP2 RIGHT CURVE                          */
        case DX7_PARAM_CTRL_VOICE_97 :  rowPos = ((bAlternate) ? ROW_POS_EDIT14 : ROW_POS_EDIT17 );break; /*97  OP2 KEY BOARD RATE SCALING               */
        case DX7_PARAM_CTRL_VOICE_98 :  rowPos = ((bAlternate) ? ROW_POS_EDIT08 : ROW_POS_EDIT11 );break; /*98  OP2 MOD SENSITIVITY AMPLITUDE            */
        case DX7_PARAM_CTRL_VOICE_99 :  rowPos = ((bAlternate) ? ROW_POS_EDIT14 : ROW_POS_EDIT17 );break; /*99  OP2 OPERATOR KEY VELOCITY SENSITIVITY    */
        case DX7_PARAM_CTRL_VOICE_100:  rowPos = ((bAlternate) ? ROW_POS_EDIT14 : ROW_POS_EDIT17 );break; /*100 OP2 OPERATOR OUTPUT LEVEL                */
        case DX7_PARAM_CTRL_VOICE_101:  rowPos = ((bAlternate) ? ROW_POS_EDIT08 : ROW_POS_EDIT11 );break; /*101 OP2 OSILLATOR MODE                       */
        case DX7_PARAM_CTRL_VOICE_102:  rowPos = ((bAlternate) ? ROW_POS_EDIT08 : ROW_POS_EDIT11 );break; /*102 OP2 OSILLATOR FREQUENCY COARSE           */
        case DX7_PARAM_CTRL_VOICE_103:  rowPos = ((bAlternate) ? ROW_POS_EDIT08 : ROW_POS_EDIT11 );break; /*103 OP2 OSILLATOR FREQUENCY FINE             */
        case DX7_PARAM_CTRL_VOICE_104:  rowPos = ((bAlternate) ? ROW_POS_EDIT08 : ROW_POS_EDIT11 );break; /*104 OP2 DETUNE                               */
        case DX7_PARAM_CTRL_VOICE_105:  rowPos = ((bAlternate) ? ROW_POS_EDIT07 : ROW_POS_EDIT12 );break; /*105 OP1 EG RATE 1                            */
        case DX7_PARAM_CTRL_VOICE_106:  rowPos = ((bAlternate) ? ROW_POS_EDIT07 : ROW_POS_EDIT12 );break; /*106 OP1 EG RATE 2                            */
        case DX7_PARAM_CTRL_VOICE_107:  rowPos = ((bAlternate) ? ROW_POS_EDIT07 : ROW_POS_EDIT12 );break; /*107 OP1 EG RATE 3                            */
        case DX7_PARAM_CTRL_VOICE_108:  rowPos = ((bAlternate) ? ROW_POS_EDIT07 : ROW_POS_EDIT12 );break; /*108 OP1 EG RATE 4                            */
        case DX7_PARAM_CTRL_VOICE_109:  rowPos = ((bAlternate) ? ROW_POS_EDIT07 : ROW_POS_EDIT12 );break; /*109 OP1 EG LEVEL 1                           */
        case DX7_PARAM_CTRL_VOICE_110:  rowPos = ((bAlternate) ? ROW_POS_EDIT07 : ROW_POS_EDIT12 );break; /*110 OP1 EG LEVEL 2                           */
        case DX7_PARAM_CTRL_VOICE_111:  rowPos = ((bAlternate) ? ROW_POS_EDIT07 : ROW_POS_EDIT12 );break; /*111 OP1 EG LEVEL 3                           */
        case DX7_PARAM_CTRL_VOICE_112:  rowPos = ((bAlternate) ? ROW_POS_EDIT07 : ROW_POS_EDIT12 );break; /*112 OP1 EG LEVEL 4                           */
        case DX7_PARAM_CTRL_VOICE_113:  rowPos = ((bAlternate) ? ROW_POS_EDIT13 : ROW_POS_EDIT18 );break; /*113 OP1 BREAK POINT                          */
        case DX7_PARAM_CTRL_VOICE_114:  rowPos = ((bAlternate) ? ROW_POS_EDIT13 : ROW_POS_EDIT18 );break; /*114 OP1 LEFT DEPTH                           */
        case DX7_PARAM_CTRL_VOICE_115:  rowPos = ((bAlternate) ? ROW_POS_EDIT13 : ROW_POS_EDIT18 );break; /*115 OP1 RIGHT DEPTH                          */
        case DX7_PARAM_CTRL_VOICE_116:  rowPos = ((bAlternate) ? ROW_POS_EDIT13 : ROW_POS_EDIT18 );break; /*116 OP1 LEFT CURVE                           */
        case DX7_PARAM_CTRL_VOICE_117:  rowPos = ((bAlternate) ? ROW_POS_EDIT13 : ROW_POS_EDIT18 );break; /*117 OP1 RIGHT CURVE                          */
        case DX7_PARAM_CTRL_VOICE_118:  rowPos = ((bAlternate) ? ROW_POS_EDIT13 : ROW_POS_EDIT18 );break; /*118 OP1 KEY BOARD RATE SCALING               */
        case DX7_PARAM_CTRL_VOICE_119:  rowPos = ((bAlternate) ? ROW_POS_EDIT07 : ROW_POS_EDIT12 );break; /*119 OP1 MOD SENSITIVITY AMPLITUDE            */
        case DX7_PARAM_CTRL_VOICE_120:  rowPos = ((bAlternate) ? ROW_POS_EDIT13 : ROW_POS_EDIT18 );break; /*120 OP1 OPERATOR KEY VELOCITY SENSITIVITY    */
        case DX7_PARAM_CTRL_VOICE_121:  rowPos = ((bAlternate) ? ROW_POS_EDIT13 : ROW_POS_EDIT18 );break; /*121 OP1 OPERATOR OUTPUT LEVEL                */
        case DX7_PARAM_CTRL_VOICE_122:  rowPos = ((bAlternate) ? ROW_POS_EDIT07 : ROW_POS_EDIT12 );break; /*122 OP1 OSILLATOR MODE                       */
        case DX7_PARAM_CTRL_VOICE_123:  rowPos = ((bAlternate) ? ROW_POS_EDIT07 : ROW_POS_EDIT12 );break; /*123 OP1 OSILLATOR FREQUENCY COARSE           */
        case DX7_PARAM_CTRL_VOICE_124:  rowPos = ((bAlternate) ? ROW_POS_EDIT07 : ROW_POS_EDIT12 );break; /*124 OP1 OSILLATOR FREQUENCY FINE             */
        case DX7_PARAM_CTRL_VOICE_125:  rowPos = ((bAlternate) ? ROW_POS_EDIT07 : ROW_POS_EDIT12 );break; /*125 OP1 DETUNE                               */
        case DX7_PARAM_CTRL_STATIC_1_2: rowPos = ((bAlternate) ? ROW_POS_EDIT07 : ROW_POS_EDIT12 );break; /*OPERATOR1*/
        case DX7_PARAM_CTRL_STATIC_2_2: rowPos = ((bAlternate) ? ROW_POS_EDIT08 : ROW_POS_EDIT11 );break; /*OPERATOR2*/
        case DX7_PARAM_CTRL_STATIC_3_2: rowPos = ((bAlternate) ? ROW_POS_EDIT09 : ROW_POS_EDIT10 );break; /*OPERATOR3*/
        case DX7_PARAM_CTRL_STATIC_4_2: rowPos = ((bAlternate) ? ROW_POS_EDIT10 : ROW_POS_EDIT09 );break; /*OPERATOR4*/
        case DX7_PARAM_CTRL_STATIC_5_2: rowPos = ((bAlternate) ? ROW_POS_EDIT11 : ROW_POS_EDIT08 );break; /*OPERATOR5*/
        case DX7_PARAM_CTRL_STATIC_6_2: rowPos = ((bAlternate) ? ROW_POS_EDIT12 : ROW_POS_EDIT07 );break; /*OPERATOR6*/
        case DX7_PARAM_CTRL_STATIC_1_3: rowPos = ((bAlternate) ? ROW_POS_EDIT13 : ROW_POS_EDIT18 );break; /*OPERATOR1*/
        case DX7_PARAM_CTRL_STATIC_2_3: rowPos = ((bAlternate) ? ROW_POS_EDIT14 : ROW_POS_EDIT17 );break; /*OPERATOR2*/
        case DX7_PARAM_CTRL_STATIC_3_3: rowPos = ((bAlternate) ? ROW_POS_EDIT15 : ROW_POS_EDIT16 );break; /*OPERATOR3*/
        case DX7_PARAM_CTRL_STATIC_4_3: rowPos = ((bAlternate) ? ROW_POS_EDIT16 : ROW_POS_EDIT15 );break; /*OPERATOR4*/
        case DX7_PARAM_CTRL_STATIC_5_3: rowPos = ((bAlternate) ? ROW_POS_EDIT17 : ROW_POS_EDIT14 );break; /*OPERATOR5*/
        case DX7_PARAM_CTRL_STATIC_6_3: rowPos = ((bAlternate) ? ROW_POS_EDIT18 : ROW_POS_EDIT13 );break; /*OPERATOR6*/
        default:
            break;
        }

        yBasePos = (PARAM_DATA_Y_UNIT*2*rowPos) + tblPtr->yPosAdj;

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
            MoveWindow( infoPtr->hwnd_data                ,
                        infoPtr->wtInfo.xPos               ,   /* x座標              */
                        PARAM_STATIC_Y_BASE_POS + yBasePos ,   /* y座標              */
                        tblPtr->withTextLen                ,   /* 幅                 */
                        infoPtr->wtInfo.height             ,   /* 高さ               */
                        TRUE);

            infoPtr->wtInfo.xPos += tblPtr->withTextLen;
        }
        else
        {
            nop();
        }

#ifdef DEBUG_DISP_STATIC_STRING
        infoPtr->wtInfo.width  = width + tblPtr->addWidth;
        infoPtr->wtInfo.height = PARAM_STATIC_HEIGHT;

        MoveWindow( infoPtr->hwnd_data                ,
                    infoPtr->wtInfo.xPos  ,   /* x座標              */
                    infoPtr->wtInfo.yPos  ,   /* y座標              */
                    infoPtr->wtInfo.width ,   /* 幅                 */
                    infoPtr->wtInfo.height,   /* 高さ               */
                    TRUE);
#else
        MoveWindow( infoPtr->hwnd_data                ,
                    infoPtr->wtInfo.xPos  ,   /* x座標              */
                    infoPtr->wtInfo.yPos  ,   /* y座標              */
                    infoPtr->wtInfo.width ,   /* 幅                 */
                    infoPtr->wtInfo.height,   /* 高さ               */
                    TRUE);
#endif
        if( IsWindowVisible(infoPtr->hwnd_data) )
        {
            ShowWindow(infoPtr->hwnd_data, SW_HIDE);
            ShowWindow(infoPtr->hwnd_data, SW_SHOW);
        }
        else
        {
            nop();
        }
    }

    return TRUE;
}
