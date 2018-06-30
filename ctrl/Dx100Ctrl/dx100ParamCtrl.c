/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */

/* 外部関数定義 */
#include "WinMain.h"

/* 外部変数定義 */

/* 内部関数定義 */
#include "dx100ParamCtrl.h"

//#define DEBUG_DISP_STATIC_STRING

typedef struct
{
    HINSTANCE hInstance;
    PTSTR     szAppName;
    BOOL      bInitConfig;
} S_DX100_PARM_CTRL_DATA;

static S_DX100_PARM_CTRL_DATA dx100ParamCtrlData;

static BOOL dx100ParamCtrlCreate( HWND hwnd, DX100_PARAM_CTRL_GROUP_ID groupId, DX100_PARAM_CTRL_ID startId, DX100_PARAM_CTRL_ID endId );
/* 内部変数定義 */

static char *strTblOnOff[2] = {"OFF","ON"};
static char *strTblAlgorithm[8] = {"1","2","3","4","5","6","7","8"};
static char *strTblLfoWave[4]   = {"SAW","SQU","TRI","S/H"};
static char *strTblTranspose[49]  = {"C 1","C#1","D 1","D#1","E 1","F 1","F#1","G 1","G#1","A 1","A#1","B 1","C 2","C#2","D 2","D#2","E 2","F 2","F#2","G 2","G#2","A 2","A#2","B 2","C 3","C#3","D 3","D#3","E 3","F 3","F#3","G 3","G#3","A 3","A#3","B 3","C 4","C#4","D 4","D#4","E 4","F 4","F#4","G 4","G#4","A 4","A#4","B 4","C 5"};
static char *strTblPolyMono[2] = {"POLY","MONO"};
static char *strTblPortamentoMode[2] = {"Full","Fing"};
static char *strTblDetune[7] = {"-3","-2","-1","0","+1","+2","+3"};
static char *strTblFrequency[64] = {"0.50","0.71","0.78","0.87","1.00","1.41","1.57","1.73","2.00","2.82","3.00","3.14","3.46","4.00","4.24","4.71","5.00","5.19","5.65","6.00","6.28","6.92","7.00","7.07","7.85","8.00","8.48","8.65","9.00","9.42","9.89","10.00","10.38","10.99","11.00","11.30","12.00","12.11","12.56","12.72","13.00","13.84","14.00","14.10","14.13","15.00","15.55","15.57","15.70","16.96","17.27","17.30","18.37","18.84","19.03","19.78","20.41","20.76","21.20","21.98","22.49","23.55","24.22","25.95"};
static char *strTblVoiceNum[32] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32"};

/* X方向 */
#define PARAM_STATIC_X_BASE_POS   5
#define PARAM_DATA_X_BASE_POS     5
#define PARAM_DATA_WIDTH         50
#define PARAM_STATIC_WIDTH       70
#define PARAM_WRAP_X_UNIT        70

/* Y方向 */
#define PARAM_STATIC_Y_BASE_POS 130
#define PARAM_DATA_Y_BASE_POS   125
#define PARAM_DATA_Y_UNIT        10

#define PARAM_STATIC_HEIGHT      15
#define PARAM_DATA_HEIGHT       300
#define PARAM_BUTTON_HEIGHT      20
#define PARAM_EDIT_HEIGHT        20

#define ROW_POS_EDIT0    0 /* PATCHNAME                */
#define ROW_POS_STATIC0  1 /*           LFO            */
#define ROW_POS_STATIC1  2 /* ALGORITHM                */
#define ROW_POS_EDIT1    3 /* [      ]      [    ]     */
#define ROW_POS_EDIT2    4 /*               [    ]     */
#define ROW_POS_EDIT3    5 /*               [    ]     */
#define ROW_POS_EDIT4    6 /*               [    ]     */
#define ROW_POS_STATIC2  7 /* OSCILLATOR               */
#define ROW_POS_STATIC3  8 /* RATIO                    */
#define ROW_POS_EDIT5    9 /* OSCILLATOR1   [    ]     */
#define ROW_POS_EDIT6   10 /* OSCILLATOR2   [    ]     */
#define ROW_POS_EDIT7   11 /* OSCILLATOR3   [    ]     */
#define ROW_POS_EDIT8   12 /* OSCILLATOR4   [    ]     */
#define ROW_POS_STATIC4 13 /*               PITCH BEND */
#define ROW_POS_STATIC5 14 /* POLY/MONO                */
#define ROW_POS_EDIT9   15 /* [      ]                 */

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

enum
{
   PCT_COMBO ,
   PCT_EDIT  ,
   PCT_BUTTON,
   PCT_STATIC,
   PCT_MAX
};
typedef BYTE DX100_PARAM_CTRL_TYPE;

typedef struct
{
    DX100_PARAM_CTRL_TYPE type         ;
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
} S_DX100_PARAM_CTRL;

static S_DX100_PARAM_CTRL paramListTbl[DX100_PARAM_CTRL_MAX] =
{/*  type     ,strText                               ,wTLen,bEnable  ,minValue,maxValue,ptstrDataDisp   ,addWidth,rowPos          ,colPos  ,yPosAdj*/       /*strFullText                                    */
    {PCT_COMBO ,TEXT("AR"                            ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT8  ,COLPOS03,0   },      /*"00 OP4 ATTACK RATE"                            */
    {PCT_COMBO ,TEXT("D1R"                           ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT8  ,COLPOS04,0   },      /*"01 OP4 DECAY 1 RATE"                           */
    {PCT_COMBO ,TEXT("D2R"                           ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT8  ,COLPOS06,0   },      /*"02 OP4 DECAY 2 RATE"                           */
    {PCT_COMBO ,TEXT("RR"                            ),0    ,TRUE    ,0       , 15     , NULL                ,  0, ROW_POS_EDIT8  ,COLPOS07,0   },      /*"03 OP4 RELEASE RATE"                           */
    {PCT_COMBO ,TEXT("D1L"                           ),0    ,TRUE    ,0       , 15     , NULL                ,  0, ROW_POS_EDIT8  ,COLPOS05,0   },      /*"04 OP4 DECAY 1 LEVEL"                          */
    {PCT_COMBO ,TEXT("LEVEL"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT8  ,COLPOS10,0   },      /*"05 OP4 KEYBOARD SCALING LEVEL"                 */
    {PCT_COMBO ,TEXT("RATE"                          ),0    ,TRUE    ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT8  ,COLPOS09,0   },      /*"06 OP4 KEYBOARD SCALING RATE"                  */
    {PCT_COMBO ,TEXT("EG BIAS"                       ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT4  ,COLPOS12,0   },      /*"07 OP4 EG BIAS SENSITIVITY"                    */
    {PCT_COMBO ,TEXT("AME"                           ),0    ,TRUE    ,0       ,  1     , NULL                ,  0, ROW_POS_EDIT4  ,COLPOS11,0   },      /*"08 OP4 AMPLITUDE MODULATION ENABLE"            */
    {PCT_COMBO ,TEXT("VELOCITY"                      ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT4  ,COLPOS13,0   },      /*"09 OP4 KEY VELOCITY"                           */
    {PCT_COMBO ,TEXT("OUT LEVEL"                     ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT8  ,COLPOS08,0   },      /*"10 OP4 OUTPUT LEVEL"                           */
    {PCT_COMBO ,TEXT("FREQ RATIO"                    ),0    ,TRUE    ,0       , 63     , strTblFrequency     ,  0, ROW_POS_EDIT8  ,COLPOS01,0   },      /*"11 OP4 OSCILLATOR FREQUENCY"                   */
    {PCT_COMBO ,TEXT("DETUNE"                        ),0    ,TRUE    ,0       ,  6     , strTblDetune        ,  0, ROW_POS_EDIT8  ,COLPOS02,0   },      /*"12 OP4 DETUNE"                                 */
    {PCT_COMBO ,TEXT("AR"                            ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT6  ,COLPOS03,0   },      /*"13 OP2 ATTACK RATE"                            */
    {PCT_COMBO ,TEXT("D1R"                           ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT6  ,COLPOS04,0   },      /*"14 OP2 DECAY 1 RATE"                           */
    {PCT_COMBO ,TEXT("D2R"                           ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT6  ,COLPOS06,0   },      /*"15 OP2 DECAY 2 RATE"                           */
    {PCT_COMBO ,TEXT("RR"                            ),0    ,TRUE    ,0       , 15     , NULL                ,  0, ROW_POS_EDIT6  ,COLPOS07,0   },      /*"16 OP2 RELEASE RATE"                           */
    {PCT_COMBO ,TEXT("D1L"                           ),0    ,TRUE    ,0       , 15     , NULL                ,  0, ROW_POS_EDIT6  ,COLPOS05,0   },      /*"17 OP2 DECAY 1 LEVEL"                          */
    {PCT_COMBO ,TEXT("LEVEL"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT6  ,COLPOS10,0   },      /*"18 OP2 KEYBOARD SCALING LEVEL"                 */
    {PCT_COMBO ,TEXT("RATE"                          ),0    ,TRUE    ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT6  ,COLPOS09,0   },      /*"19 OP2 KEYBOARD SCALING RATE"                  */
    {PCT_COMBO ,TEXT("EG BIAS"                       ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT2  ,COLPOS12,0   },      /*"20 OP2 EG BIAS SENSITIVITY"                    */
    {PCT_COMBO ,TEXT("AME"                           ),0    ,TRUE    ,0       ,  1     , NULL                ,  0, ROW_POS_EDIT2  ,COLPOS11,0   },      /*"21 OP2 AMPLITUDE MODULATION ENABLE"            */
    {PCT_COMBO ,TEXT("VELOCITY"                      ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT2  ,COLPOS13,0   },      /*"22 OP2 KEY VELOCITY"                           */
    {PCT_COMBO ,TEXT("OUT LEVEL"                     ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT6  ,COLPOS08,0   },      /*"23 OP2 OUTPUT LEVEL"                           */
    {PCT_COMBO ,TEXT("FREQ RATIO"                    ),0    ,TRUE    ,0       , 63     , strTblFrequency     ,  0, ROW_POS_EDIT6  ,COLPOS01,0   },      /*"24 OP2 OSCILLATOR FREQUENCY"                   */
    {PCT_COMBO ,TEXT("DETUNE"                        ),0    ,TRUE    ,0       ,  6     , strTblDetune        ,  0, ROW_POS_EDIT6  ,COLPOS02,0   },      /*"25 OP2 DETUNE"                                 */
    {PCT_COMBO ,TEXT("AR"                            ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT7  ,COLPOS03,0   },      /*"26 OP3 ATTACK RATE"                            */
    {PCT_COMBO ,TEXT("D1R"                           ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT7  ,COLPOS04,0   },      /*"27 OP3 DECAY 1 RATE"                           */
    {PCT_COMBO ,TEXT("D2R"                           ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT7  ,COLPOS06,0   },      /*"28 OP3 DECAY 2 RATE"                           */
    {PCT_COMBO ,TEXT("RR"                            ),0    ,TRUE    ,0       , 15     , NULL                ,  0, ROW_POS_EDIT7  ,COLPOS07,0   },      /*"29 OP3 RELEASE RATE"                           */
    {PCT_COMBO ,TEXT("D1L"                           ),0    ,TRUE    ,0       , 15     , NULL                ,  0, ROW_POS_EDIT7  ,COLPOS05,0   },      /*"30 OP3 DECAY 1 LEVEL"                          */
    {PCT_COMBO ,TEXT("LEVEL"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT7  ,COLPOS10,0   },      /*"31 OP3 KEYBOARD SCALING LEVEL"                 */
    {PCT_COMBO ,TEXT("RATE"                          ),0    ,TRUE    ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT7  ,COLPOS09,0   },      /*"32 OP3 KEYBOARD SCALING RATE"                  */
    {PCT_COMBO ,TEXT("EG BIAS"                       ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT3  ,COLPOS12,0   },      /*"33 OP3 EG BIAS SENSITIVITY"                    */
    {PCT_COMBO ,TEXT("AME"                           ),0    ,TRUE    ,0       ,  1     , NULL                ,  0, ROW_POS_EDIT3  ,COLPOS11,0   },      /*"34 OP3 AMPLITUDE MODULATION ENABLE"            */
    {PCT_COMBO ,TEXT("VELOCITY"                      ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT3  ,COLPOS13,0   },      /*"35 OP3 KEY VELOCITY"                           */
    {PCT_COMBO ,TEXT("OUT LEVEL"                     ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT7  ,COLPOS08,0   },      /*"36 OP3 OUTPUT LEVEL"                           */
    {PCT_COMBO ,TEXT("FREQ RATIO"                    ),0    ,TRUE    ,0       , 63     , strTblFrequency     ,  0, ROW_POS_EDIT7  ,COLPOS01,0   },      /*"37 OP3 OSCILLATOR FREQUENCY"                   */
    {PCT_COMBO ,TEXT("DETUNE"                        ),0    ,TRUE    ,0       ,  6     , strTblDetune        ,  0, ROW_POS_EDIT7  ,COLPOS02,0   },      /*"38 OP3 DETUNE"                                 */
    {PCT_COMBO ,TEXT("AR"                            ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS03,0   },      /*"39 OP1 ATTACK RATE"                            */
    {PCT_COMBO ,TEXT("D1R"                           ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS04,0   },      /*"40 OP1 DECAY 1 RATE"                           */
    {PCT_COMBO ,TEXT("D2R"                           ),0    ,TRUE    ,0       , 31     , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS06,0   },      /*"41 OP1 DECAY 2 RATE"                           */
    {PCT_COMBO ,TEXT("RR"                            ),0    ,TRUE    ,0       , 15     , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS07,0   },      /*"42 OP1 RELEASE RATE"                           */
    {PCT_COMBO ,TEXT("D1L"                           ),0    ,TRUE    ,0       , 15     , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS05,0   },      /*"43 OP1 DECAY 1 LEVEL"                          */
    {PCT_COMBO ,TEXT("LEVEL"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS10,0   },      /*"44 OP1 KEYBOARD SCALING LEVEL"                 */
    {PCT_COMBO ,TEXT("RATE"                          ),0    ,TRUE    ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS09,0   },      /*"45 OP1 KEYBOARD SCALING RATE"                  */
    {PCT_COMBO ,TEXT("EG BIAS"                       ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT1  ,COLPOS12,0   },      /*"46 OP1 EG BIAS SENSITIVITY"                    */
    {PCT_COMBO ,TEXT("AME"                           ),0    ,TRUE    ,0       ,  1     , NULL                ,  0, ROW_POS_EDIT1  ,COLPOS11,0   },      /*"47 OP1 AMPLITUDE MODULATION ENABLE"            */
    {PCT_COMBO ,TEXT("VELOCITY"                      ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT1  ,COLPOS13,0   },      /*"48 OP1 KEY VELOCITY"                           */
    {PCT_COMBO ,TEXT("OUT LEVEL"                     ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS08,0   },      /*"49 OP1 OUTPUT LEVEL"                           */
    {PCT_COMBO ,TEXT("FREQ RATIO"                    ),0    ,TRUE    ,0       , 63     , strTblFrequency     ,  0, ROW_POS_EDIT5  ,COLPOS01,0   },      /*"50 OP1 OSCILLATOR FREQUENCY"                   */
    {PCT_COMBO ,TEXT("DETUNE"                        ),0    ,TRUE    ,0       ,  6     , strTblDetune        ,  0, ROW_POS_EDIT5  ,COLPOS02,0   },      /*"51 OP1 DETUNE"                                 */
    {PCT_COMBO ,TEXT("ALGORITHM"                     ),0    ,TRUE    ,0       ,  7     , strTblAlgorithm     ,  0, ROW_POS_EDIT1  ,COLPOS01,0   },      /*"52 ALGORITHM"                                  */
    {PCT_COMBO ,TEXT("FEEDBACK"                      ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT1  ,COLPOS02,0   },      /*"53 FEEDBACK LEVEL"                             */
    {PCT_COMBO ,TEXT("SPEED"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT1  ,COLPOS04,0   },      /*"54 LFO SPEED"                                  */
    {PCT_COMBO ,TEXT("DELAY"                         ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT1  ,COLPOS05,0   },      /*"55 LFO DELAY"                                  */
    {PCT_COMBO ,TEXT("PMD"                           ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT1  ,COLPOS06,0   },      /*"56 PITCH MODULATION DEPTH"                     */
    {PCT_COMBO ,TEXT("AMD"                           ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT1  ,COLPOS07,0   },      /*"57 AMPLITUDE MODULATION DEPTH"                 */
    {PCT_COMBO ,TEXT("SYNC"                          ),0    ,TRUE    ,0       ,  1     , strTblOnOff         ,  0, ROW_POS_EDIT1  ,COLPOS08,0   },      /*"58 LFO SYNC"                                   */
    {PCT_COMBO ,TEXT("WAVE"                          ),0    ,TRUE    ,0       ,  3     , strTblLfoWave       ,  0, ROW_POS_EDIT1  ,COLPOS03,0   },      /*"59 LFO WAVE"                                   */
    {PCT_COMBO ,TEXT("PITCH"                         ),0    ,TRUE    ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT1  ,COLPOS09,0   },      /*"60 PITCH MODULATION SENSITIVITY"               */
    {PCT_COMBO ,TEXT("AMPLITUDE"                     ),0    ,TRUE    ,0       ,  3/*7*/, NULL                ,  0, ROW_POS_EDIT1  ,COLPOS10,0   },      /*"61 AMPLITUDE MODULATION SENSITIVITY"           */
    {PCT_COMBO ,TEXT("TRANSPOSE"                     ),0    ,TRUE    ,0       , 48     , strTblTranspose     , 20, ROW_POS_EDIT9  ,COLPOS15,0   },      /*"62 TRANSPOSE"                                  */
    {PCT_COMBO ,TEXT("POLY/MONO"                     ),0    ,TRUE    ,0       ,  1     , strTblPolyMono      ,  0, ROW_POS_EDIT9  ,COLPOS01,0   },      /*"63 PLAY MODE POLY/MONO"                        */
    {PCT_COMBO ,TEXT("PB RANGE"                      ),0    ,TRUE    ,0       , 12     , NULL                ,  0, ROW_POS_EDIT9  ,COLPOS02,0   },      /*"64 PITCH BEND RANGE"                           */
    {PCT_COMBO ,TEXT("PORTA MODE"                    ),0    ,TRUE    ,0       ,  1     , strTblPortamentoMode,  0, ROW_POS_EDIT9  ,COLPOS03,0   },      /*"65 PORTAMENTO MODE"                            */
    {PCT_COMBO ,TEXT("PORTA TIME"                    ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9  ,COLPOS04,0   },      /*"66 PORTAMENTO TIME"                            */
    {PCT_COMBO ,TEXT("FOOT VOLUME RANGE"             ),0    ,TRUE/**/,0       ,127     , NULL                ,  0, ROW_POS_EDIT9  ,COLPOS06,0   },      /*"67 FOOT VOLUME RANGE"                          */
    {PCT_COMBO ,TEXT("SUSTAIN FOOT SW"               ),0    ,TRUE    ,0       ,  1     , strTblOnOff         ,  0, ROW_POS_EDIT9  ,COLPOS07,0   },      /*"68 SUSTAIN FOOT SWITCH"                        */
    {PCT_COMBO ,TEXT("PORTAMENT FOOT SW"             ),0    ,TRUE    ,0       ,  1     , strTblOnOff         ,  0, ROW_POS_EDIT9  ,COLPOS05,0   },      /*"69 PORTAMENT FOOT SWITCH"                      */
    {PCT_COMBO ,TEXT("CHORUS SWITCH"                 ),0    ,TRUE/**/,0       ,  1     , strTblOnOff         ,  0, ROW_POS_EDIT9  ,COLPOS14,0   },      /*"70 CHORUS SWITCH"                              */
    {PCT_COMBO ,TEXT("WR PITCH"                      ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9  ,COLPOS08,0   },      /*"71 MODULATION WHEEL PITCH MODULATION RANGE"    */
    {PCT_COMBO ,TEXT("WR AMPLITUDE"                  ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9  ,COLPOS09,0   },      /*"72 MODULATION WHEEL AMPLITUDE MODULATION RANGE"*/
    {PCT_COMBO ,TEXT("BC PITCH"                      ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9  ,COLPOS10,0   },      /*"73 BREATH CONTROL PITCH MODULATION RANGE"      */
    {PCT_COMBO ,TEXT("BC AMPLITUDE"                  ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9  ,COLPOS11,0   },      /*"74 BREATH CONTROL AMPLITUDE MODULATION RANGE"  */
    {PCT_COMBO ,TEXT("BC PITCH BIAS"                 ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9  ,COLPOS12,0   },      /*"75 BREATH CONTROL PITCH BIAS RANGE"            */
    {PCT_COMBO ,TEXT("BC EG BIAS"                    ),0    ,TRUE    ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9  ,COLPOS13,0   },      /*"76 BREATH CONTROL EG BIAS RANGE"               */
    {PCT_EDIT  ,TEXT("PATCHNAME"                     ),0    ,TRUE    ,0       ,127     , NULL                , 50, ROW_POS_EDIT0  ,COLPOS01,0   },      /*"77-86 PATCHNAME"                               */
    {PCT_COMBO ,TEXT("PR1"                           ),0    ,TRUE/**/,0       ,127     , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS11,0   },      /*"87 PITCH EG RATE 1"                            */
    {PCT_COMBO ,TEXT("PR2"                           ),0    ,TRUE/**/,0       ,127     , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS13,0   },      /*"88 PITCH EG RATE 2"                            */
    {PCT_COMBO ,TEXT("PR3"                           ),0    ,TRUE/**/,0       ,127     , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS15,0   },      /*"89 PITCH EG RATE 3"                            */
    {PCT_COMBO ,TEXT("PL1"                           ),0    ,TRUE/**/,0       ,127     , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS12,0   },      /*"90 PITCH EG LEVEL 1"                           */
    {PCT_COMBO ,TEXT("PL2"                           ),0    ,TRUE/**/,0       ,127     , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS14,0   },      /*"91 PITCH EG LEVEL 2"                           */
    {PCT_COMBO ,TEXT("PL3"                           ),0    ,TRUE/**/,0       ,127     , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS16,0   },      /*"92 PITCH EG LEVEL 3"                           */
    {PCT_BUTTON,TEXT("TO ALL VOICE"                  ),0    ,TRUE    ,0       ,0       , NULL                , 50, ROW_POS_EDIT0  ,COLPOS11,0   },      /**/
    {PCT_COMBO ,TEXT("COMBO_TO_ALL_VOICE"            ),0    ,TRUE    ,0       ,31      , strTblVoiceNum      ,  0, ROW_POS_EDIT0  ,COLPOS10,0   },      /**/
    {PCT_BUTTON,TEXT("TO 6 OPERATOR"                 ),0    ,TRUE    ,0       ,0       , NULL                , 50, 17             ,0       ,0   },      /**/
    {PCT_STATIC,TEXT("OPERATOR1"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS00,0   },    /*"OPERATOR1"                                     */
    {PCT_STATIC,TEXT("OPERATOR2"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT6  ,COLPOS00,0   },    /*"OPERATOR2"                                     */
    {PCT_STATIC,TEXT("OPERATOR3"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT7  ,COLPOS00,0   },    /*"OPERATOR3"                                     */
    {PCT_STATIC,TEXT("OPERATOR4"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT8  ,COLPOS00,0   },    /*"OPERATOR4"                                     */
    {PCT_STATIC,TEXT("OPERATOR1"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT1  ,COLPOS14,0   },    /*"OPERATOR1"*/
    {PCT_STATIC,TEXT("OPERATOR2"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT2  ,COLPOS14,0   },    /*"OPERATOR2"*/
    {PCT_STATIC,TEXT("OPERATOR3"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT3  ,COLPOS14,0   },    /*"OPERATOR3"*/
    {PCT_STATIC,TEXT("OPERATOR4"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT4  ,COLPOS14,0   },    /*"OPERATOR4"*/
    {PCT_STATIC,TEXT("PATCHNAME"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_EDIT0  ,COLPOS00,0   },    /*"PATCHNAME"                                     */
    {PCT_STATIC,TEXT("LFO"                           ),0    ,TRUE    ,0       ,0       , NULL                ,300, ROW_POS_STATIC0,COLPOS03,5   },    /*"ALGORITHM"                                     */
    {PCT_STATIC,TEXT("MODULATION SENSITVITY"         ),0    ,TRUE    ,0       ,0       , NULL                ,200, ROW_POS_STATIC0,COLPOS09,5   },    /*"MODULATION SENSITIVITY"                        */
    {PCT_STATIC,TEXT("KEY"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS13,5   },    /*"KEY"                                           */
    {PCT_STATIC,TEXT("ALGORITHM"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS01,0   },    /*"ALGORITHM"                                     */
    {PCT_STATIC,TEXT("FEEDBACK"                      ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS02,0   },    /*"FEEDBACK LEVEL"                                */
    {PCT_STATIC,TEXT("WAVE"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS03,0   },    /*"LFO WAVE"                                      */
    {PCT_STATIC,TEXT("SPEED"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS04,0   },    /*"LFO SPEED"                                     */
    {PCT_STATIC,TEXT("DELAY"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS05,0   },    /*"LFO DELAY"                                     */
    {PCT_STATIC,TEXT("PMD"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS06,0   },    /*"PITCH MODULATION DEPTH"                        */
    {PCT_STATIC,TEXT("AMD"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS07,0   },    /*"AMPLITUDE MODULATION DEPTH"                    */
    {PCT_STATIC,TEXT("SYNC"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS08,0   },    /*"LFO SYNC"                                      */
    {PCT_STATIC,TEXT("PITCH"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS09,0   },    /*"PITCH MODULATION SENSITIVITY"                  */
    {PCT_STATIC,TEXT("AMPLITUDE"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS10,0   },    /*"AMPLITUDE MODULATION SENSITIVITY"              */
    {PCT_STATIC,TEXT("AME"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS11,0   },    /*"OP? AMPLITUDE MODULATION ENABLE"               */
    {PCT_STATIC,TEXT("EG BIAS"                       ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS12,0   },    /*"OP? EG BIAS SENSITIVITY"                       */
    {PCT_STATIC,TEXT("VELOCITY"                      ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS13,0   },    /*"OP? KEY VELOCITY"                              */
    {PCT_STATIC,TEXT("OSCILLATOR"                    ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS01,5   },    /*""*/
    {PCT_STATIC,TEXT("ENVELOPE GENERATOR"            ),0    ,TRUE    ,0       ,0       , NULL                ,300, ROW_POS_STATIC2,COLPOS03,5   },    /*""*/
    {PCT_STATIC,TEXT("OPERATOR"                      ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS08,5   },    /*""*/
    {PCT_STATIC,TEXT("KEY SCALING"                   ),0    ,TRUE    ,0       ,0       , NULL                ,200, ROW_POS_STATIC2,COLPOS09,5   },    /*""*/
    {PCT_STATIC,TEXT("PITCH ENVELOPE GENERATOR"      ),0    ,TRUE    ,0       ,0       , NULL                ,300, ROW_POS_STATIC2,COLPOS11,5   },    /*""*/
    {PCT_STATIC,TEXT("RATIO"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS01,0   },    /*"OP? OSCILLATOR FREQUENCY"                      */
    {PCT_STATIC,TEXT("DETUNE"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS02,0   },    /*"OP? DETUNE"                                    */
    {PCT_STATIC,TEXT("AR"                            ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS03,0   },    /*"OP? ATTACK RATE"                               */
    {PCT_STATIC,TEXT("D1R"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS04,0   },    /*"OP? DECAY 1 RATE"                              */
    {PCT_STATIC,TEXT("D1L"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS05,0   },    /*"OP? DECAY 1 LEVEL"                             */
    {PCT_STATIC,TEXT("D2R"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS06,0   },    /*"OP? DECAY 2 RATE"                              */
    {PCT_STATIC,TEXT("RR"                            ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS07,0   },    /*"OP? RELEASE RATE"                              */
    {PCT_STATIC,TEXT("OUT LEVEL"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS08,0   },    /*"OP? OUTPUT LEVEL"                              */
    {PCT_STATIC,TEXT("RATE"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS09,0   },    /*"OP? KEYBOARD SCALING RATE"                     */
    {PCT_STATIC,TEXT("LEVEL"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS10,0   },    /*"OP? KEYBOARD SCALING LEVEL"                    */
    {PCT_STATIC,TEXT("PR1"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS11,0   },    /*"PITCH EG RATE 1"                               */
    {PCT_STATIC,TEXT("PL1"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS12,0   },    /*"PITCH EG LEVEL 1"                              */
    {PCT_STATIC,TEXT("PR2"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS13,0   },    /*"PITCH EG RATE 2"                               */
    {PCT_STATIC,TEXT("PL2"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS14,0   },    /*"PITCH EG LEVEL 2"                              */
    {PCT_STATIC,TEXT("PR3"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS15,0   },    /*"PITCH EG RATE 3"                               */
    {PCT_STATIC,TEXT("PL3"                           ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS16,0   },    /*"PITCH EG LEVEL 3"                              */
    {PCT_STATIC,TEXT("PITCH BEND"                    ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC4,COLPOS02,5   },    /*""                                              */
    {PCT_STATIC,TEXT("PORTAMENTO"                    ),0    ,TRUE    ,0       ,0       , NULL                ,200, ROW_POS_STATIC4,COLPOS03,5   },    /*""                                              */
    {PCT_STATIC,TEXT("FOOT CONTROL"                  ),0    ,TRUE    ,0       ,0       , NULL                ,200, ROW_POS_STATIC4,COLPOS06,5   },    /*""                                              */
    {PCT_STATIC,TEXT("WHEEL RANGE"                   ),0    ,TRUE    ,0       ,0       , NULL                ,200, ROW_POS_STATIC4,COLPOS08,5   },    /*""                                              */
    {PCT_STATIC,TEXT("BREATH CONTROLLER RANGE"       ),0    ,TRUE    ,0       ,0       , NULL                ,300, ROW_POS_STATIC4,COLPOS10,5   },    /*""                                              */
    {PCT_STATIC,TEXT("POLY/MONO"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS01,0   },    /*"PLAY MODE POLY/MONO"                           */
    {PCT_STATIC,TEXT("RANGE"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS02,0   },    /*"PITCH BEND RANGE"                              */
    {PCT_STATIC,TEXT("MODE"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS03,0   },    /*"PORTAMENTO MODE"                               */
    {PCT_STATIC,TEXT("TIME"                          ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS04,0   },    /*"PORTAMENTO TIME"                               */
    {PCT_STATIC,TEXT("FOOT SW"                       ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS05,0   },    /*"PORTAMENT FOOT SWITCH"                         */
    {PCT_STATIC,TEXT("VOLUME"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS06,0   },    /*"FOOT VOLUME RANGE"                             */
    {PCT_STATIC,TEXT("SUSTAIN"                       ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS07,0   },    /*"SUSTAIN FOOT SWITCH"                           */
    {PCT_STATIC,TEXT("PITCH"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS08,0   },    /*"MODULATION WHEEL PITCH MODULATION RANGE"       */
    {PCT_STATIC,TEXT("AMPLITUDE"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS09,0   },    /*"MODULATION WHEEL AMPLITUDE MODULATION RANGE"   */
    {PCT_STATIC,TEXT("PITCH"                         ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS10,0   },    /*"BREATH CONTROL PITCH MODULATION RANGE"         */
    {PCT_STATIC,TEXT("AMPLITUDE"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS11,0   },    /*"BREATH CONTROL AMPLITUDE MODULATION RANGE"     */
    {PCT_STATIC,TEXT("PITCH BIAS"                    ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS12,0   },    /*"BREATH CONTROL PITCH BIAS RANGE"               */
    {PCT_STATIC,TEXT("EG BIAS"                       ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS13,0   },    /*"BREATH CONTROL EG BIAS RANGE"                  */
    {PCT_STATIC,TEXT("CHORUS"                        ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS14,0   },    /*"CHORUS SWITCH"                                 */
    {PCT_STATIC,TEXT("TRANSPOSE"                     ),0    ,TRUE    ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS15,0   },    /*"TRANSPOSE"                                     */
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
    {PCT_BUTTON,TEXT("TO 6 OPERATOR"                 ),0    ,TRUE    ,0       ,0       , NULL                , 50, 9              ,0       ,0   },    /**/
};

typedef struct
{
    INT xPos  ;
    INT yPos  ;
    INT width ;
    INT height;
} S_DX100_PARAM_WINDOW_INFO;

typedef struct
{
    BOOL                exist                        ; /* ウィンドウが生成されているかどうか */
    HWND                hwnd_static_text             ; /* ウィンドウのハンドラ               */
    HWND                hwnd_data                    ; /* ウィンドウのハンドラ               */
    S_DX100_PARAM_WINDOW_INFO wtInfo                       ;
    DX100_PARAM_CTRL_GROUP_ID groupId                      ;
} S_DX100_PARAM_INFO;

static S_DX100_PARAM_INFO dx100ctrlParamInfo[DX100_PARAM_CTRL_MAX];

/********************************************************************************
 * 内容  : 登録された全てのパラメータコントロールを生成する
 * 引数  : HINSTANCE hInst
 * 引数  : PTSTR szAppName
 * 引数  : HWND hwnd 親ウィンドウのハンドラ
 * 戻り値: BOOL
 ***************************************/
BOOL
Dx100ParamCtrlCreate( HINSTANCE hInst, PTSTR szAppName, HWND hwnd )
{
    dx100ParamCtrlData.bInitConfig = TRUE;
    dx100ParamCtrlData.hInstance   = hInst;
    dx100ParamCtrlData.szAppName   = szAppName;

    dx100ParamCtrlCreate(hwnd,DX100_PARAM_CTRL_GROUP_1VOICE       ,DX100_PARAM_CTRL_1VOICE_START      ,DX100_PARAM_CTRL_1VOICE_END        );
    dx100ParamCtrlCreate(hwnd,DX100_PARAM_CTRL_GROUP_ALL_VOICE    ,DX100_PARAM_CTRL_ALL_VOICE_START   ,DX100_PARAM_CTRL_ALL_VOICE_END     );

    return TRUE;
}

/********************************************************************************
 * 内容  : 登録された全てのパラメータコントロールを生成する
 * 引数  : HWND hwnd 親ウィンドウのハンドラ
 * 引数  : DX100_PARAM_CTRL_GROUP_ID groupId
 * 引数  : DX100_PARAM_CTRL_ID       startId
 * 引数  : DX100_PARAM_CTRL_ID       endId
 * 戻り値: BOOL
 ***************************************/
static BOOL
dx100ParamCtrlCreate( HWND hwnd, DX100_PARAM_CTRL_GROUP_ID groupId, DX100_PARAM_CTRL_ID startId, DX100_PARAM_CTRL_ID endId )
{
    int nowId;

    if( (startId <= endId) && (endId < DX100_PARAM_CTRL_MAX) )
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
            S_DX100_PARAM_INFO *infoPtr = &dx100ctrlParamInfo[nowId];
            S_DX100_PARAM_CTRL *tblPtr = &paramListTbl[nowId];

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

                if( DX100_PARAM_CTRL_ALL_VOICE_START <= nowId && nowId <= DX100_PARAM_CTRL_ALL_VOICE_END )
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
                infoPtr->hwnd_static_text = CreateWindowEx( 0,                                          /* 拡張スタイル       */
                                            TEXT("static"),                             /* クラス名           */
                                            tblPtr->strText,                            /* ウィンドウテキスト */
                                            (WS_CHILD|ES_LEFT),                         /* スタイル           */
                                            infoPtr->wtInfo.xPos,                  /* x座標              */
                                            PARAM_STATIC_Y_BASE_POS + yBasePos,   /* y座標              */
                                            tblPtr->withTextLen,    /* 幅                 */
                                            infoPtr->wtInfo.height, /* 高さ               */
                                            hwnd,(HMENU)0,
                                            dx100ParamCtrlData.hInstance,NULL );               /* インスタンスハンドル,補助引数 */

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

            infoPtr->hwnd_data = CreateWindowEx( WS_EX_OVERLAPPEDWINDOW,                    /* 拡張スタイル       */
                                                (PTSTR)TEXT("edit")/*TEXT("static")*/,      /* クラス名           */
                                                tblPtr->strText,                             /* ウィンドウテキスト */
                                                (WS_CHILD|ES_LEFT|ES_AUTOHSCROLL)/*(WS_CHILD|ES_LEFT)*/,/* スタイル           */
                                                infoPtr->wtInfo.xPos,   /* x座標              */
                                                infoPtr->wtInfo.yPos,   /* y座標              */
                                                infoPtr->wtInfo.width,  /* 幅                 */
                                                infoPtr->wtInfo.height, /* 高さ               */
                                                hwnd,(HMENU)(DX100_PARAM_CTRL_ID_OFFSET+(nowId*2)+1),/* 親ウィンドウ,子ウィンドウID */
                                                dx100ParamCtrlData.hInstance,NULL );                        /* インスタンスハンドル,補助引数 */
#else
            infoPtr->hwnd_data = CreateWindowEx( exStyle,                                    /* 拡張スタイル       */
                                                class,                                      /* クラス名           */
                                                strText,                                    /* ウィンドウテキスト */
                                                style,                                      /* スタイル           */
                                                infoPtr->wtInfo.xPos,   /* x座標              */
                                                infoPtr->wtInfo.yPos,   /* y座標              */
                                                infoPtr->wtInfo.width,  /* 幅                 */
                                                infoPtr->wtInfo.height, /* 高さ               */
                                                hwnd,(HMENU)(DX100_PARAM_CTRL_ID_OFFSET+nowId),/* 親ウィンドウ,子ウィンドウID */
                                                dx100ParamCtrlData.hInstance,NULL );                        /* インスタンスハンドル,補助引数 */
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
 * 引数  : DX100_PARAM_CTRL_GROUP_ID groupId
 * 戻り値: BOOL
 ***************************************/
BOOL
Dx100ParamCtrlGroupDisplay( DX100_PARAM_CTRL_GROUP_ID groupId )
{
    int nowId;

    for(nowId=0; nowId<DX100_PARAM_CTRL_MAX; nowId++ )
    {
        S_DX100_PARAM_INFO *infoPtr = &dx100ctrlParamInfo[nowId];

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
 * 引数  : DX100_PARAM_CTRL_ID id
 * 戻り値: HWND
 ***************************************/
HWND
Dx100ParamCtrlGetHWND( DX100_PARAM_CTRL_ID id )
{
    HWND rtn = (HWND)0;

    if( id < DX100_PARAM_CTRL_MAX )
    {
        S_DX100_PARAM_INFO *infoPtr = &dx100ctrlParamInfo[id];

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
 * 引数  : DX100_PARAM_CTRL_ID id
 * 引数  : PTSTR ptstrText
 * 戻り値: BOOL
 ***************************************/
BOOL
Dx100ParamCtrlGetText( DX100_PARAM_CTRL_ID id, PTSTR ptstrText )
{
    BOOL rtn = FALSE;
    INT iLength;

    if( id < DX100_PARAM_CTRL_MAX )
    {
        S_DX100_PARAM_INFO *infoPtr = &dx100ctrlParamInfo[id];

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
 * 内容  : コントロールのサイズを調整する
 * 引数  : int xPos
 * 引数  : int yPos
 * 戻り値: なし
 ***************************************/
void
Dx100ParamCtrlSetSize( int xPos, int yPos )
{
    int i;

    for(i=0; i<DX100_PARAM_CTRL_MAX; i++)
    {
        S_DX100_PARAM_INFO *infoPtr = &dx100ctrlParamInfo[i];

        MoveWindow( infoPtr->hwnd_data,
                    infoPtr->wtInfo.xPos - xPos,   /* x座標              */
                    infoPtr->wtInfo.yPos - yPos,   /* y座標              */
                    infoPtr->wtInfo.width,  /* 幅                 */
                    infoPtr->wtInfo.height, /* 高さ               */
                    TRUE);
    }
}
