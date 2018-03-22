/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */

/* 外部関数定義 */
#include "WinMain.h"

/* 外部変数定義 */

/* 内部関数定義 */
#include "ParamCtrl.h"

//#define DEBUG_DISP_STATIC_STRING

typedef struct
{
    HINSTANCE hInstance;
    PTSTR     szAppName;
    BOOL      bInitConfig;
} S_PARM_CTRL_DATA;

static S_PARM_CTRL_DATA paramCtrlData;

static BOOL paramCtrlCreate( HWND hwnd, PARAM_CTRL_GROUP_ID groupId, PARAM_CTRL_ID startId, PARAM_CTRL_ID endId );
/* 内部変数定義 */

char *strTblOnOff[2] = {"OFF","ON"};
char *strTblAlgorithm[8] = {"1","2","3","4","5","6","7","8"};
char *strTblLfoWave[4]   = {"SAW","SQU","TRI","S/H"};
char *strTblTranspose[49]  = {"C 1","C#1","D 1","D#1","E 1","F 1","F#1","G 1","G#1","A 1","A#1","B 1","C 2","C#2","D 2","D#2","E 2","F 2","F#2","G 2","G#2","A 2","A#2","B 2","C 3","C#3","D 3","D#3","E 3","F 3","F#3","G 3","G#3","A 3","A#3","B 3","C 4","C#4","D 4","D#4","E 4","F 4","F#4","G 4","G#4","A 4","A#4","B 4","C 5"};
char *strTblPolyMono[2] = {"POLY","MONO"};
char *strTblPortamentoMode[2] = {"Full","Fing"};
char *strTblDetune[7] = {"-3","-2","-1","0","+1","+2","+3"};
char *strTblFrequency[64] = {"0.50","0.71","0.78","0.87","1.00","1.41","1.57","1.73","2.00","2.82","3.00","3.14","3.46","4.00","4.24","4.71","5.00","5.19","5.65","6.00","6.28","6.92","7.00","7.07","7.85","8.00","8.48","8.65","9.00","9.42","9.89","10.00","10.38","10.99","11.00","11.30","12.00","12.11","12.56","12.72","13.00","13.84","14.00","14.10","14.13","15.00","15.55","15.57","15.70","16.96","17.27","17.30","18.37","18.84","19.03","19.78","20.41","20.76","21.20","21.98","22.49","23.55","24.22","25.95"};

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

#define ROW_POS_EDIT0    0
#define ROW_POS_STATIC0  1
#define ROW_POS_STATIC1  2
#define ROW_POS_EDIT1    3
#define ROW_POS_EDIT2    4
#define ROW_POS_EDIT3    5
#define ROW_POS_EDIT4    6
#define ROW_POS_STATIC2  7
#define ROW_POS_STATIC3  8
#define ROW_POS_EDIT5    9
#define ROW_POS_EDIT6   10
#define ROW_POS_EDIT7   11
#define ROW_POS_EDIT8   12
#define ROW_POS_STATIC4 13
#define ROW_POS_STATIC5 14
#define ROW_POS_EDIT9   15

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
   PCT_STATIC,
   PCT_MAX
};
typedef BYTE PARAM_CTRL_TYPE;

typedef struct
{
    PARAM_CTRL_TYPE type         ;
    PTSTR           strText      ; /* ウィンドウテキスト                         */
    BOOL            bEnable      ;
    INT             minValue     ;
    INT             maxValue     ;
    char **         ptstrDataDisp;
    INT             addWidth     ;
    INT             rowPos       ;/* 行 */
    INT             colPos       ;/* 列 */
} S_PARAM_CTRL;

S_PARAM_CTRL paramListTbl[PARAM_CTRL_MAX] =
{/*  type     ,strText                                ,bEnable,minValue,maxValue,ptstrDataDisp   ,addWidth,rowPos        ,colPos */       /*strFullText                                    */
    {PCT_COMBO,TEXT("SOUNDMODE"                      ),TRUE   ,0       ,  2     , NULL                ,  0, ROW_POS_EDIT1,COLPOS01},      /*"SOUNDMODE"                                     */
    {PCT_COMBO,TEXT("AR"                             ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT8,COLPOS03},      /*"00 OP4 ATTACK RATE"                            */
    {PCT_COMBO,TEXT("D1R"                            ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT8,COLPOS04},      /*"01 OP4 DECAY 1 RATE"                           */
    {PCT_COMBO,TEXT("D2R"                            ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT8,COLPOS06},      /*"02 OP4 DECAY 2 RATE"                           */
    {PCT_COMBO,TEXT("RR"                             ),TRUE   ,0       , 15     , NULL                ,  0, ROW_POS_EDIT8,COLPOS07},      /*"03 OP4 RELEASE RATE"                           */
    {PCT_COMBO,TEXT("D1L"                            ),TRUE   ,0       , 15     , NULL                ,  0, ROW_POS_EDIT8,COLPOS05},      /*"04 OP4 DECAY 1 LEVEL"                          */
    {PCT_COMBO,TEXT("LEVEL"                          ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT8,COLPOS10},      /*"05 OP4 KEYBOARD SCALING LEVEL"                 */
    {PCT_COMBO,TEXT("RATE"                           ),TRUE   ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT8,COLPOS09},      /*"06 OP4 KEYBOARD SCALING RATE"                  */
    {PCT_COMBO,TEXT("EG BIAS"                        ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT4,COLPOS12},      /*"07 OP4 EG BIAS SENSITIVITY"                    */
    {PCT_COMBO,TEXT("AME"                            ),TRUE   ,0       ,  1     , NULL                ,  0, ROW_POS_EDIT4,COLPOS11},      /*"08 OP4 AMPLITUDE MODULATION ENABLE"            */
    {PCT_COMBO,TEXT("VELOCITY"                       ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT4,COLPOS13},      /*"09 OP4 KEY VELOCITY"                           */
    {PCT_COMBO,TEXT("OUT LEVEL"                      ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT8,COLPOS08},      /*"10 OP4 OUTPUT LEVEL"                           */
    {PCT_COMBO,TEXT("FREQ RATIO"                     ),TRUE   ,0       , 63     , strTblFrequency     ,  0, ROW_POS_EDIT8,COLPOS01},      /*"11 OP4 OSCILLATOR FREQUENCY"                   */
    {PCT_COMBO,TEXT("DETUNE"                         ),TRUE   ,0       ,  6     , strTblDetune        ,  0, ROW_POS_EDIT8,COLPOS02},      /*"12 OP4 DETUNE"                                 */
    {PCT_COMBO,TEXT("AR"                             ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT6,COLPOS03},      /*"13 OP2 ATTACK RATE"                            */
    {PCT_COMBO,TEXT("D1R"                            ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT6,COLPOS04},      /*"14 OP2 DECAY 1 RATE"                           */
    {PCT_COMBO,TEXT("D2R"                            ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT6,COLPOS06},      /*"15 OP2 DECAY 2 RATE"                           */
    {PCT_COMBO,TEXT("RR"                             ),TRUE   ,0       , 15     , NULL                ,  0, ROW_POS_EDIT6,COLPOS07},      /*"16 OP2 RELEASE RATE"                           */
    {PCT_COMBO,TEXT("D1L"                            ),TRUE   ,0       , 15     , NULL                ,  0, ROW_POS_EDIT6,COLPOS05},      /*"17 OP2 DECAY 1 LEVEL"                          */
    {PCT_COMBO,TEXT("LEVEL"                          ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT6,COLPOS10},      /*"18 OP2 KEYBOARD SCALING LEVEL"                 */
    {PCT_COMBO,TEXT("RATE"                           ),TRUE   ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT6,COLPOS09},      /*"19 OP2 KEYBOARD SCALING RATE"                  */
    {PCT_COMBO,TEXT("EG BIAS"                        ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT2,COLPOS12},      /*"20 OP2 EG BIAS SENSITIVITY"                    */
    {PCT_COMBO,TEXT("AME"                            ),TRUE   ,0       ,  1     , NULL                ,  0, ROW_POS_EDIT2,COLPOS11},      /*"21 OP2 AMPLITUDE MODULATION ENABLE"            */
    {PCT_COMBO,TEXT("VELOCITY"                       ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT2,COLPOS13},      /*"22 OP2 KEY VELOCITY"                           */
    {PCT_COMBO,TEXT("OUT LEVEL"                      ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT6,COLPOS08},      /*"23 OP2 OUTPUT LEVEL"                           */
    {PCT_COMBO,TEXT("FREQ RATIO"                     ),TRUE   ,0       , 63     , strTblFrequency     ,  0, ROW_POS_EDIT6,COLPOS01},      /*"24 OP2 OSCILLATOR FREQUENCY"                   */
    {PCT_COMBO,TEXT("DETUNE"                         ),TRUE   ,0       ,  6     , strTblDetune        ,  0, ROW_POS_EDIT6,COLPOS02},      /*"25 OP2 DETUNE"                                 */
    {PCT_COMBO,TEXT("AR"                             ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT7,COLPOS03},      /*"26 OP3 ATTACK RATE"                            */
    {PCT_COMBO,TEXT("D1R"                            ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT7,COLPOS04},      /*"27 OP3 DECAY 1 RATE"                           */
    {PCT_COMBO,TEXT("D2R"                            ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT7,COLPOS06},      /*"28 OP3 DECAY 2 RATE"                           */
    {PCT_COMBO,TEXT("RR"                             ),TRUE   ,0       , 15     , NULL                ,  0, ROW_POS_EDIT7,COLPOS07},      /*"29 OP3 RELEASE RATE"                           */
    {PCT_COMBO,TEXT("D1L"                            ),TRUE   ,0       , 15     , NULL                ,  0, ROW_POS_EDIT7,COLPOS05},      /*"30 OP3 DECAY 1 LEVEL"                          */
    {PCT_COMBO,TEXT("LEVEL"                          ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT7,COLPOS10},      /*"31 OP3 KEYBOARD SCALING LEVEL"                 */
    {PCT_COMBO,TEXT("RATE"                           ),TRUE   ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT7,COLPOS09},      /*"32 OP3 KEYBOARD SCALING RATE"                  */
    {PCT_COMBO,TEXT("EG BIAS"                        ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT3,COLPOS12},      /*"33 OP3 EG BIAS SENSITIVITY"                    */
    {PCT_COMBO,TEXT("AME"                            ),TRUE   ,0       ,  1     , NULL                ,  0, ROW_POS_EDIT3,COLPOS11},      /*"34 OP3 AMPLITUDE MODULATION ENABLE"            */
    {PCT_COMBO,TEXT("VELOCITY"                       ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT3,COLPOS13},      /*"35 OP3 KEY VELOCITY"                           */
    {PCT_COMBO,TEXT("OUT LEVEL"                      ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT7,COLPOS08},      /*"36 OP3 OUTPUT LEVEL"                           */
    {PCT_COMBO,TEXT("FREQ RATIO"                     ),TRUE   ,0       , 63     , strTblFrequency     ,  0, ROW_POS_EDIT7,COLPOS01},      /*"37 OP3 OSCILLATOR FREQUENCY"                   */
    {PCT_COMBO,TEXT("DETUNE"                         ),TRUE   ,0       ,  6     , strTblDetune        ,  0, ROW_POS_EDIT7,COLPOS02},      /*"38 OP3 DETUNE"                                 */
    {PCT_COMBO,TEXT("AR"                             ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT5,COLPOS03},      /*"39 OP1 ATTACK RATE"                            */
    {PCT_COMBO,TEXT("D1R"                            ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT5,COLPOS04},      /*"40 OP1 DECAY 1 RATE"                           */
    {PCT_COMBO,TEXT("D2R"                            ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT5,COLPOS06},      /*"41 OP1 DECAY 2 RATE"                           */
    {PCT_COMBO,TEXT("RR"                             ),TRUE   ,0       , 15     , NULL                ,  0, ROW_POS_EDIT5,COLPOS07},      /*"42 OP1 RELEASE RATE"                           */
    {PCT_COMBO,TEXT("D1L"                            ),TRUE   ,0       , 15     , NULL                ,  0, ROW_POS_EDIT5,COLPOS05},      /*"43 OP1 DECAY 1 LEVEL"                          */
    {PCT_COMBO,TEXT("LEVEL"                          ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT5,COLPOS10},      /*"44 OP1 KEYBOARD SCALING LEVEL"                 */
    {PCT_COMBO,TEXT("RATE"                           ),TRUE   ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT5,COLPOS09},      /*"45 OP1 KEYBOARD SCALING RATE"                  */
    {PCT_COMBO,TEXT("EG BIAS"                        ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT1,COLPOS12},      /*"46 OP1 EG BIAS SENSITIVITY"                    */
    {PCT_COMBO,TEXT("AME"                            ),TRUE   ,0       ,  1     , NULL                ,  0, ROW_POS_EDIT1,COLPOS11},      /*"47 OP1 AMPLITUDE MODULATION ENABLE"            */
    {PCT_COMBO,TEXT("VELOCITY"                       ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT1,COLPOS13},      /*"48 OP1 KEY VELOCITY"                           */
    {PCT_COMBO,TEXT("OUT LEVEL"                      ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT5,COLPOS08},      /*"49 OP1 OUTPUT LEVEL"                           */
    {PCT_COMBO,TEXT("FREQ RATIO"                     ),TRUE   ,0       , 63     , strTblFrequency     ,  0, ROW_POS_EDIT5,COLPOS01},      /*"50 OP1 OSCILLATOR FREQUENCY"                   */
    {PCT_COMBO,TEXT("DETUNE"                         ),TRUE   ,0       ,  6     , strTblDetune        ,  0, ROW_POS_EDIT5,COLPOS02},      /*"51 OP1 DETUNE"                                 */
    {PCT_COMBO,TEXT("ALGORITHM"                      ),TRUE   ,0       ,  7     , strTblAlgorithm     ,  0, ROW_POS_EDIT1,COLPOS01},      /*"52 ALGORITHM"                                  */
    {PCT_COMBO,TEXT("FEEDBACK"                       ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT1,COLPOS02},      /*"53 FEEDBACK LEVEL"                             */
    {PCT_COMBO,TEXT("SPEED"                          ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT1,COLPOS04},      /*"54 LFO SPEED"                                  */
    {PCT_COMBO,TEXT("DELAY"                          ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT1,COLPOS05},      /*"55 LFO DELAY"                                  */
    {PCT_COMBO,TEXT("PMD"                            ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT1,COLPOS06},      /*"56 PITCH MODULATION DEPTH"                     */
    {PCT_COMBO,TEXT("AMD"                            ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT1,COLPOS07},      /*"57 AMPLITUDE MODULATION DEPTH"                 */
    {PCT_COMBO,TEXT("SYNC"                           ),TRUE   ,0       ,  1     , strTblOnOff         ,  0, ROW_POS_EDIT1,COLPOS08},      /*"58 LFO SYNC"                                   */
    {PCT_COMBO,TEXT("WAVE"                           ),TRUE   ,0       ,  3     , strTblLfoWave       ,  0, ROW_POS_EDIT1,COLPOS03},      /*"59 LFO WAVE"                                   */
    {PCT_COMBO,TEXT("PITCH"                          ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT1,COLPOS09},      /*"60 PITCH MODULATION SENSITIVITY"               */
    {PCT_COMBO,TEXT("AMPLITUDE"                      ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT1,COLPOS10},      /*"61 AMPLITUDE MODULATION SENSITIVITY"           */
    {PCT_COMBO,TEXT("TRANSPOSE"                      ),TRUE   ,0       , 48     , strTblTranspose     , 20, ROW_POS_EDIT9,COLPOS15},      /*"62 TRANSPOSE"                                  */
    {PCT_COMBO,TEXT("POLY/MONO"                      ),TRUE   ,0       ,  1     , strTblPolyMono      ,  0, ROW_POS_EDIT9,COLPOS01},      /*"63 PLAY MODE POLY/MONO"                        */
    {PCT_COMBO,TEXT("PB RANGE"                       ),TRUE   ,0       , 12     , NULL                ,  0, ROW_POS_EDIT9,COLPOS02},      /*"64 PITCH BEND RANGE"                           */
    {PCT_COMBO,TEXT("PORTA MODE"                     ),TRUE   ,0       ,  1     , strTblPortamentoMode,  0, ROW_POS_EDIT9,COLPOS03},      /*"65 PORTAMENTO MODE"                            */
    {PCT_COMBO,TEXT("PORTA TIME"                     ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9,COLPOS04},      /*"66 PORTAMENTO TIME"                            */
    {PCT_COMBO,TEXT("FOOT VOLUME RANGE"              ),FALSE  ,0       ,127     , NULL                ,  0, ROW_POS_EDIT9,COLPOS06},      /*"67 FOOT VOLUME RANGE"                          */
    {PCT_COMBO,TEXT("SUSTAIN FOOT SW"                ),TRUE   ,0       ,  1     , strTblOnOff         ,  0, ROW_POS_EDIT9,COLPOS07},      /*"68 SUSTAIN FOOT SWITCH"                        */
    {PCT_COMBO,TEXT("PORTAMENT FOOT SW"              ),TRUE   ,0       ,  1     , strTblOnOff         ,  0, ROW_POS_EDIT9,COLPOS05},      /*"69 PORTAMENT FOOT SWITCH"                      */
    {PCT_COMBO,TEXT("CHORUS SWITCH"                  ),FALSE  ,0       ,  1     , strTblOnOff         ,  0, ROW_POS_EDIT9,COLPOS14},      /*"70 CHORUS SWITCH"                              */
    {PCT_COMBO,TEXT("WR PITCH"                       ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9,COLPOS08},      /*"71 MODULATION WHEEL PITCH MODULATION RANGE"    */
    {PCT_COMBO,TEXT("WR AMPLITUDE"                   ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9,COLPOS09},      /*"72 MODULATION WHEEL AMPLITUDE MODULATION RANGE"*/
    {PCT_COMBO,TEXT("BC PITCH"                       ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9,COLPOS10},      /*"73 BREATH CONTROL PITCH MODULATION RANGE"      */
    {PCT_COMBO,TEXT("BC AMPLITUDE"                   ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9,COLPOS11},      /*"74 BREATH CONTROL AMPLITUDE MODULATION RANGE"  */
    {PCT_COMBO,TEXT("BC PITCH BIAS"                  ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9,COLPOS12},      /*"75 BREATH CONTROL PITCH BIAS RANGE"            */
    {PCT_COMBO,TEXT("BC EG BIAS"                     ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9,COLPOS13},      /*"76 BREATH CONTROL EG BIAS RANGE"               */
    {PCT_EDIT ,TEXT("PATCHNAME"                      ),TRUE   ,0       ,127     , NULL                , 50, ROW_POS_EDIT0,COLPOS01},      /*"77-86 PATCHNAME"                               */
    {PCT_COMBO,TEXT("PR1"                            ),FALSE  ,0       ,127     , NULL                ,  0, ROW_POS_EDIT5,COLPOS11},      /*"87 PITCH EG RATE 1"                            */
    {PCT_COMBO,TEXT("PR2"                            ),FALSE  ,0       ,127     , NULL                ,  0, ROW_POS_EDIT5,COLPOS13},      /*"88 PITCH EG RATE 2"                            */
    {PCT_COMBO,TEXT("PR3"                            ),FALSE  ,0       ,127     , NULL                ,  0, ROW_POS_EDIT5,COLPOS15},      /*"89 PITCH EG RATE 3"                            */
    {PCT_COMBO,TEXT("PL1"                            ),FALSE  ,0       ,127     , NULL                ,  0, ROW_POS_EDIT5,COLPOS12},      /*"90 PITCH EG LEVEL 1"                           */
    {PCT_COMBO,TEXT("PL2"                            ),FALSE  ,0       ,127     , NULL                ,  0, ROW_POS_EDIT5,COLPOS14},      /*"91 PITCH EG LEVEL 2"                           */
    {PCT_COMBO,TEXT("PL3"                            ),FALSE  ,0       ,127     , NULL                ,  0, ROW_POS_EDIT5,COLPOS16},      /*"92 PITCH EG LEVEL 3"                           */
                                                                                                                                          /*"                                               */
    {PCT_STATIC,TEXT("OPERATOR1"                     ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS00},    /*"OPERATOR1"                                     */
    {PCT_STATIC,TEXT("OPERATOR2"                     ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_EDIT6  ,COLPOS00},    /*"OPERATOR2"                                     */
    {PCT_STATIC,TEXT("OPERATOR3"                     ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_EDIT7  ,COLPOS00},    /*"OPERATOR3"                                     */
    {PCT_STATIC,TEXT("OPERATOR4"                     ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_EDIT8  ,COLPOS00},    /*"OPERATOR4"                                     */
    {PCT_STATIC,TEXT("PATCHNAME"                     ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_EDIT0  ,COLPOS00},    /*"PATCHNAME"                                     */

    {PCT_STATIC,TEXT("LFO"                           ),TRUE   ,0       ,0       , NULL                ,300, ROW_POS_STATIC0,COLPOS03},    /*"ALGORITHM"                                     */
    {PCT_STATIC,TEXT("MODULATION SENSITVITY"         ),TRUE   ,0       ,0       , NULL                ,200, ROW_POS_STATIC0,COLPOS09},    /*"MODULATION SENSITIVITY"                        */
    {PCT_STATIC,TEXT("KEY"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS13},    /*"KEY"                                           */

    {PCT_STATIC,TEXT("ALGORITHM"                     ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS01},    /*"ALGORITHM"                                     */
    {PCT_STATIC,TEXT("FEEDBACK"                      ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS02},    /*"FEEDBACK LEVEL"                                */
    {PCT_STATIC,TEXT("WAVE"                          ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS03},    /*"LFO WAVE"                                      */
    {PCT_STATIC,TEXT("SPEED"                         ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS04},    /*"LFO SPEED"                                     */
    {PCT_STATIC,TEXT("DELAY"                         ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS05},    /*"LFO DELAY"                                     */
    {PCT_STATIC,TEXT("PMD"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS06},    /*"PITCH MODULATION DEPTH"                        */
    {PCT_STATIC,TEXT("AMD"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS07},    /*"AMPLITUDE MODULATION DEPTH"                    */
    {PCT_STATIC,TEXT("SYNC"                          ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS08},    /*"LFO SYNC"                                      */
    {PCT_STATIC,TEXT("PITCH"                         ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS09},    /*"PITCH MODULATION SENSITIVITY"                  */
    {PCT_STATIC,TEXT("AMPLITUDE"                     ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS10},    /*"AMPLITUDE MODULATION SENSITIVITY"              */
    {PCT_STATIC,TEXT("AME"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS11},    /*"OP? AMPLITUDE MODULATION ENABLE"               */
    {PCT_STATIC,TEXT("EG BIAS"                       ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS12},    /*"OP? EG BIAS SENSITIVITY"                       */
    {PCT_STATIC,TEXT("VELOCITY"                      ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS13},    /*"OP? KEY VELOCITY"                              */

    {PCT_STATIC,TEXT("OSCILLATOR"                    ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS01},    /*""*/
    {PCT_STATIC,TEXT("ENVELOPE GENERATOR"            ),TRUE   ,0       ,0       , NULL                ,300, ROW_POS_STATIC2,COLPOS03},    /*""*/
    {PCT_STATIC,TEXT("OPERATOR"                      ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS08},    /*""*/
    {PCT_STATIC,TEXT("KEY SCALING"                   ),TRUE   ,0       ,0       , NULL                ,200, ROW_POS_STATIC2,COLPOS09},    /*""*/
    {PCT_STATIC,TEXT("PITCH ENVELOPE GENERATOR"      ),TRUE   ,0       ,0       , NULL                ,300, ROW_POS_STATIC2,COLPOS11},    /*""*/

    {PCT_STATIC,TEXT("RATIO"                         ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS01},    /*"OP? OSCILLATOR FREQUENCY"                      */
    {PCT_STATIC,TEXT("DETUNE"                        ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS02},    /*"OP? DETUNE"                                    */
    {PCT_STATIC,TEXT("AR"                            ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS03},    /*"OP? ATTACK RATE"                               */
    {PCT_STATIC,TEXT("D1R"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS04},    /*"OP? DECAY 1 RATE"                              */
    {PCT_STATIC,TEXT("D1L"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS05},    /*"OP? DECAY 1 LEVEL"                             */
    {PCT_STATIC,TEXT("D2R"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS06},    /*"OP? DECAY 2 RATE"                              */
    {PCT_STATIC,TEXT("RR"                            ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS07},    /*"OP? RELEASE RATE"                              */
    {PCT_STATIC,TEXT("OUT LEVEL"                     ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS08},    /*"OP? OUTPUT LEVEL"                              */
    {PCT_STATIC,TEXT("RATE"                          ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS09},    /*"OP? KEYBOARD SCALING RATE"                     */
    {PCT_STATIC,TEXT("LEVEL"                         ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS10},    /*"OP? KEYBOARD SCALING LEVEL"                    */
    {PCT_STATIC,TEXT("PR1"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS11},    /*"PITCH EG RATE 1"                               */
    {PCT_STATIC,TEXT("PL1"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS12},    /*"PITCH EG LEVEL 1"                              */
    {PCT_STATIC,TEXT("PR2"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS13},    /*"PITCH EG RATE 2"                               */
    {PCT_STATIC,TEXT("PL2"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS14},    /*"PITCH EG LEVEL 2"                              */
    {PCT_STATIC,TEXT("PR3"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS15},    /*"PITCH EG RATE 3"                               */
    {PCT_STATIC,TEXT("PL3"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC3,COLPOS16},    /*"PITCH EG LEVEL 3"                              */

    {PCT_STATIC,TEXT("PITCH BEND"                    ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC4,COLPOS02},    /*""                                              */
    {PCT_STATIC,TEXT("PORTAMENTO"                    ),TRUE   ,0       ,0       , NULL                ,200, ROW_POS_STATIC4,COLPOS03},    /*""                                              */
    {PCT_STATIC,TEXT("FOOT CONTROL"                  ),TRUE   ,0       ,0       , NULL                ,200, ROW_POS_STATIC4,COLPOS06},    /*""                                              */
    {PCT_STATIC,TEXT("WHEEL RANGE"                   ),TRUE   ,0       ,0       , NULL                ,200, ROW_POS_STATIC4,COLPOS08},    /*""                                              */
    {PCT_STATIC,TEXT("BREATH CONTROLLER RANGE"       ),TRUE   ,0       ,0       , NULL                ,300, ROW_POS_STATIC4,COLPOS10},    /*""                                              */

    {PCT_STATIC,TEXT("POLY/MONO"                     ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS01},    /*"PLAY MODE POLY/MONO"                           */
    {PCT_STATIC,TEXT("RANGE"                         ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS02},    /*"PITCH BEND RANGE"                              */
    {PCT_STATIC,TEXT("MODE"                          ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS03},    /*"PORTAMENTO MODE"                               */
    {PCT_STATIC,TEXT("TIME"                          ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS04},    /*"PORTAMENTO TIME"                               */
    {PCT_STATIC,TEXT("FOOT SW"                       ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS05},    /*"PORTAMENT FOOT SWITCH"                         */
    {PCT_STATIC,TEXT("VOLUME"                        ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS06},    /*"FOOT VOLUME RANGE"                             */
    {PCT_STATIC,TEXT("SUSTAIN"                       ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS07},    /*"SUSTAIN FOOT SWITCH"                           */
    {PCT_STATIC,TEXT("PITCH"                         ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS08},    /*"MODULATION WHEEL PITCH MODULATION RANGE"       */
    {PCT_STATIC,TEXT("AMPLITUDE"                     ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS09},    /*"MODULATION WHEEL AMPLITUDE MODULATION RANGE"   */
    {PCT_STATIC,TEXT("PITCH"                         ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS10},    /*"BREATH CONTROL PITCH MODULATION RANGE"         */
    {PCT_STATIC,TEXT("AMPLITUDE"                     ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS11},    /*"BREATH CONTROL AMPLITUDE MODULATION RANGE"     */
    {PCT_STATIC,TEXT("PITCH BIAS"                    ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS12},    /*"BREATH CONTROL PITCH BIAS RANGE"               */
    {PCT_STATIC,TEXT("EG BIAS"                       ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS13},    /*"BREATH CONTROL EG BIAS RANGE"                  */
    {PCT_STATIC,TEXT("CHORUS"                        ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS14},    /*"CHORUS SWITCH"                                 */
    {PCT_STATIC,TEXT("TRANSPOSE"                     ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC5,COLPOS15},    /*"TRANSPOSE"                                     */

    {PCT_EDIT ,TEXT("VOICE_NAME_01"                  ),TRUE   ,0       ,127     , NULL                , 50, 0    , 0},                    /*"VOICE_NAME_01"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_02"                  ),TRUE   ,0       ,127     , NULL                , 50, 1    , 0},                    /*"VOICE_NAME_02"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_03"                  ),TRUE   ,0       ,127     , NULL                , 50, 2    , 0},                    /*"VOICE_NAME_03"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_04"                  ),TRUE   ,0       ,127     , NULL                , 50, 3    , 0},                    /*"VOICE_NAME_04"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_05"                  ),TRUE   ,0       ,127     , NULL                , 50, 4    , 0},                    /*"VOICE_NAME_05"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_06"                  ),TRUE   ,0       ,127     , NULL                , 50, 5    , 0},                    /*"VOICE_NAME_06"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_07"                  ),TRUE   ,0       ,127     , NULL                , 50, 6    , 0},                    /*"VOICE_NAME_07"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_08"                  ),TRUE   ,0       ,127     , NULL                , 50, 7    , 0},                    /*"VOICE_NAME_08"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_09"                  ),TRUE   ,0       ,127     , NULL                , 50, 8    , 0},                    /*"VOICE_NAME_09"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_10"                  ),TRUE   ,0       ,127     , NULL                , 50, 9    , 0},                    /*"VOICE_NAME_10"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_11"                  ),TRUE   ,0       ,127     , NULL                , 50,10    , 0},                    /*"VOICE_NAME_11"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_12"                  ),TRUE   ,0       ,127     , NULL                , 50,11    , 0},                    /*"VOICE_NAME_12"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_13"                  ),TRUE   ,0       ,127     , NULL                , 50,12    , 0},                    /*"VOICE_NAME_13"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_14"                  ),TRUE   ,0       ,127     , NULL                , 50,13    , 0},                    /*"VOICE_NAME_14"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_15"                  ),TRUE   ,0       ,127     , NULL                , 50,14    , 0},                    /*"VOICE_NAME_15"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_16"                  ),TRUE   ,0       ,127     , NULL                , 50,15    , 0},                    /*"VOICE_NAME_16"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_17"                  ),TRUE   ,0       ,127     , NULL                , 50,16    , 0},                    /*"VOICE_NAME_17"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_18"                  ),TRUE   ,0       ,127     , NULL                , 50,17    , 0},                    /*"VOICE_NAME_18"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_19"                  ),TRUE   ,0       ,127     , NULL                , 50,18    , 0},                    /*"VOICE_NAME_19"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_20"                  ),TRUE   ,0       ,127     , NULL                , 50,19    , 0},                    /*"VOICE_NAME_20"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_21"                  ),TRUE   ,0       ,127     , NULL                , 50,20    , 0},                    /*"VOICE_NAME_21"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_22"                  ),TRUE   ,0       ,127     , NULL                , 50,21    , 0},                    /*"VOICE_NAME_22"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_23"                  ),TRUE   ,0       ,127     , NULL                , 50,22    , 0},                    /*"VOICE_NAME_23"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_24"                  ),TRUE   ,0       ,127     , NULL                , 50,23    , 0},                    /*"VOICE_NAME_24"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_25"                  ),TRUE   ,0       ,127     , NULL                , 50,24    , 0},                    /*"VOICE_NAME_25"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_26"                  ),TRUE   ,0       ,127     , NULL                , 50,25    , 0},                    /*"VOICE_NAME_26"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_27"                  ),TRUE   ,0       ,127     , NULL                , 50,26    , 0},                    /*"VOICE_NAME_27"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_28"                  ),TRUE   ,0       ,127     , NULL                , 50,27    , 0},                    /*"VOICE_NAME_28"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_29"                  ),TRUE   ,0       ,127     , NULL                , 50,28    , 0},                    /*"VOICE_NAME_29"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_30"                  ),TRUE   ,0       ,127     , NULL                , 50,29    , 0},                    /*"VOICE_NAME_30"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_31"                  ),TRUE   ,0       ,127     , NULL                , 50,30    , 0},                    /*"VOICE_NAME_31"                                 */
    {PCT_EDIT ,TEXT("VOICE_NAME_32"                  ),TRUE   ,0       ,127     , NULL                , 50,31    , 0},                    /*"VOICE_NAME_32"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_01"                  ),TRUE   ,0       ,127     , NULL                ,250, 0    , 2},                    /*"VOICE_BULK_01"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_02"                  ),TRUE   ,0       ,127     , NULL                ,250, 1    , 2},                    /*"VOICE_BULK_02"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_03"                  ),TRUE   ,0       ,127     , NULL                ,250, 2    , 2},                    /*"VOICE_BULK_03"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_04"                  ),TRUE   ,0       ,127     , NULL                ,250, 3    , 2},                    /*"VOICE_BULK_04"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_05"                  ),TRUE   ,0       ,127     , NULL                ,250, 4    , 2},                    /*"VOICE_BULK_05"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_06"                  ),TRUE   ,0       ,127     , NULL                ,250, 5    , 2},                    /*"VOICE_BULK_06"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_07"                  ),TRUE   ,0       ,127     , NULL                ,250, 6    , 2},                    /*"VOICE_BULK_07"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_08"                  ),TRUE   ,0       ,127     , NULL                ,250, 7    , 2},                    /*"VOICE_BULK_08"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_09"                  ),TRUE   ,0       ,127     , NULL                ,250, 8    , 2},                    /*"VOICE_BULK_09"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_10"                  ),TRUE   ,0       ,127     , NULL                ,250, 9    , 2},                    /*"VOICE_BULK_10"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_11"                  ),TRUE   ,0       ,127     , NULL                ,250,10    , 2},                    /*"VOICE_BULK_11"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_12"                  ),TRUE   ,0       ,127     , NULL                ,250,11    , 2},                    /*"VOICE_BULK_12"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_13"                  ),TRUE   ,0       ,127     , NULL                ,250,12    , 2},                    /*"VOICE_BULK_13"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_14"                  ),TRUE   ,0       ,127     , NULL                ,250,13    , 2},                    /*"VOICE_BULK_14"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_15"                  ),TRUE   ,0       ,127     , NULL                ,250,14    , 2},                    /*"VOICE_BULK_15"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_16"                  ),TRUE   ,0       ,127     , NULL                ,250,15    , 2},                    /*"VOICE_BULK_16"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_17"                  ),TRUE   ,0       ,127     , NULL                ,250,16    , 2},                    /*"VOICE_BULK_17"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_18"                  ),TRUE   ,0       ,127     , NULL                ,250,17    , 2},                    /*"VOICE_BULK_18"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_19"                  ),TRUE   ,0       ,127     , NULL                ,250,18    , 2},                    /*"VOICE_BULK_19"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_20"                  ),TRUE   ,0       ,127     , NULL                ,250,19    , 2},                    /*"VOICE_BULK_20"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_21"                  ),TRUE   ,0       ,127     , NULL                ,250,20    , 2},                    /*"VOICE_BULK_21"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_22"                  ),TRUE   ,0       ,127     , NULL                ,250,21    , 2},                    /*"VOICE_BULK_22"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_23"                  ),TRUE   ,0       ,127     , NULL                ,250,22    , 2},                    /*"VOICE_BULK_23"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_24"                  ),TRUE   ,0       ,127     , NULL                ,250,23    , 2},                    /*"VOICE_BULK_24"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_25"                  ),TRUE   ,0       ,127     , NULL                ,250,24    , 2},                    /*"VOICE_BULK_25"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_26"                  ),TRUE   ,0       ,127     , NULL                ,250,25    , 2},                    /*"VOICE_BULK_26"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_27"                  ),TRUE   ,0       ,127     , NULL                ,250,26    , 2},                    /*"VOICE_BULK_27"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_28"                  ),TRUE   ,0       ,127     , NULL                ,250,27    , 2},                    /*"VOICE_BULK_28"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_29"                  ),TRUE   ,0       ,127     , NULL                ,250,28    , 2},                    /*"VOICE_BULK_29"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_30"                  ),TRUE   ,0       ,127     , NULL                ,250,29    , 2},                    /*"VOICE_BULK_30"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_31"                  ),TRUE   ,0       ,127     , NULL                ,250,30    , 2},                    /*"VOICE_BULK_31"                                 */
    {PCT_EDIT ,TEXT("VOICE_BULK_32"                  ),TRUE   ,0       ,127     , NULL                ,250,31    , 2},                    /*"VOICE_BULK_32"                                 */
};

typedef struct
{
    INT xPos  ;
    INT yPos  ;
    INT width ;
    INT height;
} S_PARAM_WINDOW_INFO;

typedef struct
{
    BOOL                exist                        ; /* ウィンドウが生成されているかどうか */
    HWND                hwnd_data                    ; /* ウィンドウのハンドラ               */
    S_PARAM_WINDOW_INFO wtInfo                       ;
    PARAM_CTRL_GROUP_ID groupId                      ;
} S_PARAM_INFO;

S_PARAM_INFO ctrlParamInfo[PARAM_CTRL_MAX];

/********************************************************************************
 * 内容  : 登録された全てのパラメータコントロールを生成する
 * 引数  : HINSTANCE hInst
 * 引数  : PTSTR szAppName
 * 引数  : HWND hwnd 親ウィンドウのハンドラ
 * 戻り値: BOOL
 ***************************************/
BOOL
ParamCtrlCreate( HINSTANCE hInst, PTSTR szAppName, HWND hwnd )
{
    paramCtrlData.bInitConfig = TRUE;
    paramCtrlData.hInstance   = hInst;
    paramCtrlData.szAppName   = szAppName;

    paramCtrlCreate(hwnd,PARAM_CTRL_GROUP_SYSTEM_COMMON,PARAM_CTRL_SYSCMN_SOUNDMODE  ,PARAM_CTRL_SYSCMN_SOUNDMODE  );
    paramCtrlCreate(hwnd,PARAM_CTRL_GROUP_1VOICE       ,PARAM_CTRL_1VOICE_START      ,PARAM_CTRL_1VOICE_END        );
    paramCtrlCreate(hwnd,PARAM_CTRL_GROUP_ALL_VOICE    ,PARAM_CTRL_ALL_VOICE_NAME_00 ,PARAM_CTRL_ALL_VOICE_BULK_31 );

    return TRUE;
}

/********************************************************************************
 * 内容  : 登録された全てのパラメータコントロールを生成する
 * 引数  : HWND hwnd 親ウィンドウのハンドラ
 * 引数  : PARAM_CTRL_GROUP_ID groupId
 * 引数  : PARAM_CTRL_ID       startId
 * 引数  : PARAM_CTRL_ID       endId
 * 戻り値: BOOL
 ***************************************/
static BOOL
paramCtrlCreate( HWND hwnd, PARAM_CTRL_GROUP_ID groupId, PARAM_CTRL_ID startId, PARAM_CTRL_ID endId )
{
    int nowId;

    if( (startId <= endId) && (endId < PARAM_CTRL_MAX) )
    {
        for(nowId=startId; nowId<=endId; nowId++ )
        {
            PTSTR class;
            DWORD style;
            DWORD exStyle;
            PTSTR strText;
            DWORD height;
            INT   width;
            INT xPos;
            S_PARAM_INFO *infoPtr = &ctrlParamInfo[nowId];
            S_PARAM_CTRL *tblPtr = &paramListTbl[nowId];

            if( tblPtr->type == PCT_COMBO )
            {
                class = (PTSTR)TEXT("combobox");
                strText = (PTSTR)0;
                style = (WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
                exStyle = 0;
                height = PARAM_DATA_HEIGHT;
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

                if( PARAM_CTRL_ALL_VOICE_NAME_00 <= nowId && nowId <= PARAM_CTRL_ALL_VOICE_BULK_31)
                {
                    style |= ES_READONLY;
                }
                else
                {
                    nop();
                }

                exStyle = WS_EX_OVERLAPPEDWINDOW;
                height = 20;
            }

            if( tblPtr->type == PCT_STATIC )
            {
                width = PARAM_STATIC_WIDTH;
                infoPtr->wtInfo.xPos   = PARAM_DATA_X_BASE_POS+(PARAM_WRAP_X_UNIT*tblPtr->colPos);
                infoPtr->wtInfo.yPos   = PARAM_STATIC_Y_BASE_POS+(PARAM_DATA_Y_UNIT*2*tblPtr->rowPos);
            }
            else
            {
                width = PARAM_DATA_WIDTH;
                infoPtr->wtInfo.xPos   = PARAM_DATA_X_BASE_POS+(PARAM_WRAP_X_UNIT*tblPtr->colPos);
                infoPtr->wtInfo.yPos   = PARAM_DATA_Y_BASE_POS+(PARAM_DATA_Y_UNIT*2*tblPtr->rowPos);
            }
            infoPtr->wtInfo.width  = width + tblPtr->addWidth;
            infoPtr->wtInfo.height = height;

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
                                                hwnd,(HMENU)(PARAM_CTRL_ID_OFFSET+(nowId*2)+1),/* 親ウィンドウ,子ウィンドウID */
                                                paramCtrlData.hInstance,NULL );                        /* インスタンスハンドル,補助引数 */
#else
            infoPtr->hwnd_data = CreateWindowEx( exStyle,                                    /* 拡張スタイル       */
                                                class,                                      /* クラス名           */
                                                strText,                                    /* ウィンドウテキスト */
                                                style,                                      /* スタイル           */
                                                infoPtr->wtInfo.xPos,   /* x座標              */
                                                infoPtr->wtInfo.yPos,   /* y座標              */
                                                infoPtr->wtInfo.width,  /* 幅                 */
                                                infoPtr->wtInfo.height, /* 高さ               */
                                                hwnd,(HMENU)(PARAM_CTRL_ID_OFFSET+(nowId*2)+1),/* 親ウィンドウ,子ウィンドウID */
                                                paramCtrlData.hInstance,NULL );                        /* インスタンスハンドル,補助引数 */
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
 * 引数  : PARAM_CTRL_GROUP_ID groupId
 * 戻り値: BOOL
 ***************************************/
BOOL
ParamCtrlGroupDisplay( PARAM_CTRL_GROUP_ID groupId )
{
    int nowId;

    for(nowId=0; nowId<PARAM_CTRL_MAX; nowId++ )
    {
        S_PARAM_INFO *infoPtr = &ctrlParamInfo[nowId];

        if( infoPtr->groupId == groupId )
        {
            ShowWindow(infoPtr->hwnd_data, SW_SHOW);
        }
        else
        {
            ShowWindow(infoPtr->hwnd_data, SW_HIDE);
        }
    }

    return TRUE;
}


/********************************************************************************
 * 内容  : ウィンドウハンドラを取得する
 * 引数  : PARAM_CTRL_ID id
 * 戻り値: HWND
 ***************************************/
HWND
ParamCtrlGetHWND( PARAM_CTRL_ID id )
{
    HWND rtn = (HWND)0;

    if( id < PARAM_CTRL_MAX )
    {
        S_PARAM_INFO *infoPtr = &ctrlParamInfo[id];

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
 * 引数  : PARAM_CTRL_ID id
 * 引数  : PTSTR ptstrText
 * 戻り値: BOOL
 ***************************************/
BOOL
ParamCtrlGetText( PARAM_CTRL_ID id, PTSTR ptstrText )
{
    BOOL rtn = FALSE;
    INT iLength;

    if( id < PARAM_CTRL_MAX )
    {
        S_PARAM_INFO *infoPtr = &ctrlParamInfo[id];

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
ParamCtrlSetSize( int xPos, int yPos )
{
    int i;

    for(i=0; i<PARAM_CTRL_MAX; i++)
    {
        S_PARAM_INFO *infoPtr = &ctrlParamInfo[i];

        MoveWindow( infoPtr->hwnd_data,
                    infoPtr->wtInfo.xPos - xPos,   /* x座標              */
                    infoPtr->wtInfo.yPos - yPos,   /* y座標              */
                    infoPtr->wtInfo.width,  /* 幅                 */
                    infoPtr->wtInfo.height, /* 高さ               */
                    TRUE);
    }
}
