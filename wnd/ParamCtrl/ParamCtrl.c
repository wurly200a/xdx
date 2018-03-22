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

#define ROW_POS_STATIC0  2
#define ROW_POS_EDIT1    3
#define ROW_POS_EDIT2    4
#define ROW_POS_EDIT3    5
#define ROW_POS_EDIT4    6

#define ROW_POS_STATIC1  8
#define ROW_POS_EDIT5    9
#define ROW_POS_EDIT6   10
#define ROW_POS_EDIT7   11
#define ROW_POS_EDIT8   12

#define ROW_POS_STATIC2 14
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
    PTSTR           strFullText  ; /* ウィンドウテキスト                         */
    BOOL            bEnable      ;
    INT             minValue     ;
    INT             maxValue     ;
    char **         ptstrDataDisp;
    INT             addWidth     ;
    INT             rowPos       ;/* 行 */
    INT             colPos       ;/* 列 */
} S_PARAM_CTRL;

S_PARAM_CTRL paramListTbl[PARAM_CTRL_MAX] =
{/*  type     ,strText                  ,strFullText                                           ,bEnable,minValue,maxValue,ptstrDataDisp   ,addWidth,rowPos        ,colPos */
    {PCT_COMBO,TEXT("SOUNDMODE"        ),TEXT("SOUNDMODE"                                     ),TRUE   ,0       ,  2     , NULL                ,  0, ROW_POS_EDIT1,COLPOS01},
    {PCT_COMBO,TEXT("AR"               ),TEXT("00 OP4 ATTACK RATE"                            ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT8,COLPOS03},
    {PCT_COMBO,TEXT("D1R"              ),TEXT("01 OP4 DECAY 1 RATE"                           ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT8,COLPOS04},
    {PCT_COMBO,TEXT("D2R"              ),TEXT("02 OP4 DECAY 2 RATE"                           ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT8,COLPOS06},
    {PCT_COMBO,TEXT("RR"               ),TEXT("03 OP4 RELEASE RATE"                           ),TRUE   ,0       , 15     , NULL                ,  0, ROW_POS_EDIT8,COLPOS07},
    {PCT_COMBO,TEXT("D1L"              ),TEXT("04 OP4 DECAY 1 LEVEL"                          ),TRUE   ,0       , 15     , NULL                ,  0, ROW_POS_EDIT8,COLPOS05},
    {PCT_COMBO,TEXT("LEVEL"            ),TEXT("05 OP4 KEYBOARD SCALING LEVEL"                 ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT8,COLPOS10},
    {PCT_COMBO,TEXT("RATE"             ),TEXT("06 OP4 KEYBOARD SCALING RATE"                  ),TRUE   ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT8,COLPOS09},
    {PCT_COMBO,TEXT("EG BIAS"          ),TEXT("07 OP4 EG BIAS SENSITIVITY"                    ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT4,COLPOS12},
    {PCT_COMBO,TEXT("AME"              ),TEXT("08 OP4 AMPLITUDE MODULATION ENABLE"            ),TRUE   ,0       ,  1     , NULL                ,  0, ROW_POS_EDIT4,COLPOS11},
    {PCT_COMBO,TEXT("VELOCITY"         ),TEXT("09 OP4 KEY VELOCITY"                           ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT4,COLPOS13},
    {PCT_COMBO,TEXT("OUT LEVEL"        ),TEXT("10 OP4 OUTPUT LEVEL"                           ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT8,COLPOS08},
    {PCT_COMBO,TEXT("FREQ RATIO"       ),TEXT("11 OP4 OSCILLATOR FREQUENCY"                   ),TRUE   ,0       , 63     , strTblFrequency     ,  0, ROW_POS_EDIT8,COLPOS01},
    {PCT_COMBO,TEXT("DETUNE"           ),TEXT("12 OP4 DETUNE"                                 ),TRUE   ,0       ,  6     , strTblDetune        ,  0, ROW_POS_EDIT8,COLPOS02},
    {PCT_COMBO,TEXT("AR"               ),TEXT("13 OP2 ATTACK RATE"                            ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT6,COLPOS03},
    {PCT_COMBO,TEXT("D1R"              ),TEXT("14 OP2 DECAY 1 RATE"                           ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT6,COLPOS04},
    {PCT_COMBO,TEXT("D2R"              ),TEXT("15 OP2 DECAY 2 RATE"                           ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT6,COLPOS06},
    {PCT_COMBO,TEXT("RR"               ),TEXT("16 OP2 RELEASE RATE"                           ),TRUE   ,0       , 15     , NULL                ,  0, ROW_POS_EDIT6,COLPOS07},
    {PCT_COMBO,TEXT("D1L"              ),TEXT("17 OP2 DECAY 1 LEVEL"                          ),TRUE   ,0       , 15     , NULL                ,  0, ROW_POS_EDIT6,COLPOS05},
    {PCT_COMBO,TEXT("LEVEL"            ),TEXT("18 OP2 KEYBOARD SCALING LEVEL"                 ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT6,COLPOS10},
    {PCT_COMBO,TEXT("RATE"             ),TEXT("19 OP2 KEYBOARD SCALING RATE"                  ),TRUE   ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT6,COLPOS09},
    {PCT_COMBO,TEXT("EG BIAS"          ),TEXT("20 OP2 EG BIAS SENSITIVITY"                    ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT2,COLPOS12},
    {PCT_COMBO,TEXT("AME"              ),TEXT("21 OP2 AMPLITUDE MODULATION ENABLE"            ),TRUE   ,0       ,  1     , NULL                ,  0, ROW_POS_EDIT2,COLPOS11},
    {PCT_COMBO,TEXT("VELOCITY"         ),TEXT("22 OP2 KEY VELOCITY"                           ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT2,COLPOS13},
    {PCT_COMBO,TEXT("OUT LEVEL"        ),TEXT("23 OP2 OUTPUT LEVEL"                           ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT6,COLPOS08},
    {PCT_COMBO,TEXT("FREQ RATIO"       ),TEXT("24 OP2 OSCILLATOR FREQUENCY"                   ),TRUE   ,0       , 63     , strTblFrequency     ,  0, ROW_POS_EDIT6,COLPOS01},
    {PCT_COMBO,TEXT("DETUNE"           ),TEXT("25 OP2 DETUNE"                                 ),TRUE   ,0       ,  6     , strTblDetune        ,  0, ROW_POS_EDIT6,COLPOS02},
    {PCT_COMBO,TEXT("AR"               ),TEXT("26 OP3 ATTACK RATE"                            ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT7,COLPOS03},
    {PCT_COMBO,TEXT("D1R"              ),TEXT("27 OP3 DECAY 1 RATE"                           ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT7,COLPOS04},
    {PCT_COMBO,TEXT("D2R"              ),TEXT("28 OP3 DECAY 2 RATE"                           ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT7,COLPOS06},
    {PCT_COMBO,TEXT("RR"               ),TEXT("29 OP3 RELEASE RATE"                           ),TRUE   ,0       , 15     , NULL                ,  0, ROW_POS_EDIT7,COLPOS07},
    {PCT_COMBO,TEXT("D1L"              ),TEXT("30 OP3 DECAY 1 LEVEL"                          ),TRUE   ,0       , 15     , NULL                ,  0, ROW_POS_EDIT7,COLPOS05},
    {PCT_COMBO,TEXT("LEVEL"            ),TEXT("31 OP3 KEYBOARD SCALING LEVEL"                 ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT7,COLPOS10},
    {PCT_COMBO,TEXT("RATE"             ),TEXT("32 OP3 KEYBOARD SCALING RATE"                  ),TRUE   ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT7,COLPOS09},
    {PCT_COMBO,TEXT("EG BIAS"          ),TEXT("33 OP3 EG BIAS SENSITIVITY"                    ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT3,COLPOS12},
    {PCT_COMBO,TEXT("AME"              ),TEXT("34 OP3 AMPLITUDE MODULATION ENABLE"            ),TRUE   ,0       ,  1     , NULL                ,  0, ROW_POS_EDIT3,COLPOS11},
    {PCT_COMBO,TEXT("VELOCITY"         ),TEXT("35 OP3 KEY VELOCITY"                           ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT3,COLPOS13},
    {PCT_COMBO,TEXT("OUT LEVEL"        ),TEXT("36 OP3 OUTPUT LEVEL"                           ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT7,COLPOS08},
    {PCT_COMBO,TEXT("FREQ RATIO"       ),TEXT("37 OP3 OSCILLATOR FREQUENCY"                   ),TRUE   ,0       , 63     , strTblFrequency     ,  0, ROW_POS_EDIT7,COLPOS01},
    {PCT_COMBO,TEXT("DETUNE"           ),TEXT("38 OP3 DETUNE"                                 ),TRUE   ,0       ,  6     , strTblDetune        ,  0, ROW_POS_EDIT7,COLPOS02},
    {PCT_COMBO,TEXT("AR"               ),TEXT("39 OP1 ATTACK RATE"                            ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT5,COLPOS03},
    {PCT_COMBO,TEXT("D1R"              ),TEXT("40 OP1 DECAY 1 RATE"                           ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT5,COLPOS04},
    {PCT_COMBO,TEXT("D2R"              ),TEXT("41 OP1 DECAY 2 RATE"                           ),TRUE   ,0       , 31     , NULL                ,  0, ROW_POS_EDIT5,COLPOS06},
    {PCT_COMBO,TEXT("RR"               ),TEXT("42 OP1 RELEASE RATE"                           ),TRUE   ,0       , 15     , NULL                ,  0, ROW_POS_EDIT5,COLPOS07},
    {PCT_COMBO,TEXT("D1L"              ),TEXT("43 OP1 DECAY 1 LEVEL"                          ),TRUE   ,0       , 15     , NULL                ,  0, ROW_POS_EDIT5,COLPOS05},
    {PCT_COMBO,TEXT("LEVEL"            ),TEXT("44 OP1 KEYBOARD SCALING LEVEL"                 ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT5,COLPOS10},
    {PCT_COMBO,TEXT("RATE"             ),TEXT("45 OP1 KEYBOARD SCALING RATE"                  ),TRUE   ,0       ,  3     , NULL                ,  0, ROW_POS_EDIT5,COLPOS09},
    {PCT_COMBO,TEXT("EG BIAS"          ),TEXT("46 OP1 EG BIAS SENSITIVITY"                    ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT1,COLPOS12},
    {PCT_COMBO,TEXT("AME"              ),TEXT("47 OP1 AMPLITUDE MODULATION ENABLE"            ),TRUE   ,0       ,  1     , NULL                ,  0, ROW_POS_EDIT1,COLPOS11},
    {PCT_COMBO,TEXT("VELOCITY"         ),TEXT("48 OP1 KEY VELOCITY"                           ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT1,COLPOS13},
    {PCT_COMBO,TEXT("OUT LEVEL"        ),TEXT("49 OP1 OUTPUT LEVEL"                           ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT5,COLPOS08},
    {PCT_COMBO,TEXT("FREQ RATIO"       ),TEXT("50 OP1 OSCILLATOR FREQUENCY"                   ),TRUE   ,0       , 63     , strTblFrequency     ,  0, ROW_POS_EDIT5,COLPOS01},
    {PCT_COMBO,TEXT("DETUNE"           ),TEXT("51 OP1 DETUNE"                                 ),TRUE   ,0       ,  6     , strTblDetune        ,  0, ROW_POS_EDIT5,COLPOS02},
    {PCT_COMBO,TEXT("ALGORITHM"        ),TEXT("52 ALGORITHM"                                  ),TRUE   ,0       ,  7     , strTblAlgorithm     ,  0, ROW_POS_EDIT1,COLPOS01},
    {PCT_COMBO,TEXT("FEEDBACK"         ),TEXT("53 FEEDBACK LEVEL"                             ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT1,COLPOS02},
    {PCT_COMBO,TEXT("SPEED"            ),TEXT("54 LFO SPEED"                                  ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT1,COLPOS04},
    {PCT_COMBO,TEXT("DELAY"            ),TEXT("55 LFO DELAY"                                  ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT1,COLPOS05},
    {PCT_COMBO,TEXT("PMD"              ),TEXT("56 PITCH MODULATION DEPTH"                     ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT1,COLPOS06},
    {PCT_COMBO,TEXT("AMD"              ),TEXT("57 AMPLITUDE MODULATION DEPTH"                 ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT1,COLPOS07},
    {PCT_COMBO,TEXT("LFO SYNC"         ),TEXT("58 LFO SYNC"                                   ),TRUE   ,0       ,  1     , strTblOnOff         ,  0, ROW_POS_EDIT1,COLPOS08},
    {PCT_COMBO,TEXT("LFO WAVE"         ),TEXT("59 LFO WAVE"                                   ),TRUE   ,0       ,  3     , strTblLfoWave       ,  0, ROW_POS_EDIT1,COLPOS03},
    {PCT_COMBO,TEXT("PITCH"            ),TEXT("60 PITCH MODULATION SENSITIVITY"               ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT1,COLPOS09},
    {PCT_COMBO,TEXT("AMPLITUDE"        ),TEXT("61 AMPLITUDE MODULATION SENSITIVITY"           ),TRUE   ,0       ,  7     , NULL                ,  0, ROW_POS_EDIT1,COLPOS10},
    {PCT_COMBO,TEXT("TRANSPOSE"        ),TEXT("62 TRANSPOSE"                                  ),TRUE   ,0       , 48     , strTblTranspose     , 20, ROW_POS_EDIT9,COLPOS15},
    {PCT_COMBO,TEXT("POLY/MONO"        ),TEXT("63 PLAY MODE POLY/MONO"                        ),TRUE   ,0       ,  1     , strTblPolyMono      ,  0, ROW_POS_EDIT9,COLPOS01},
    {PCT_COMBO,TEXT("PB RANGE"         ),TEXT("64 PITCH BEND RANGE"                           ),TRUE   ,0       , 12     , NULL                ,  0, ROW_POS_EDIT9,COLPOS02},
    {PCT_COMBO,TEXT("PORTA MODE"       ),TEXT("65 PORTAMENTO MODE"                            ),TRUE   ,0       ,  1     , strTblPortamentoMode,  0, ROW_POS_EDIT9,COLPOS03},
    {PCT_COMBO,TEXT("PORTA TIME"       ),TEXT("66 PORTAMENTO TIME"                            ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9,COLPOS04},
    {PCT_COMBO,TEXT("FOOT VOLUME RANGE"),TEXT("67 FOOT VOLUME RANGE"                          ),FALSE  ,0       ,127     , NULL                ,  0, ROW_POS_EDIT9,COLPOS06},
    {PCT_COMBO,TEXT("SUSTAIN FOOT SW"  ),TEXT("68 SUSTAIN FOOT SWITCH"                        ),TRUE   ,0       ,  1     , strTblOnOff         ,  0, ROW_POS_EDIT9,COLPOS07},
    {PCT_COMBO,TEXT("PORTAMENT FOOT SW"),TEXT("69 PORTAMENT FOOT SWITCH"                      ),TRUE   ,0       ,  1     , strTblOnOff         ,  0, ROW_POS_EDIT9,COLPOS05},
    {PCT_COMBO,TEXT("CHORUS SWITCH"    ),TEXT("70 CHORUS SWITCH"                              ),FALSE  ,0       ,  1     , strTblOnOff         ,  0, ROW_POS_EDIT9,COLPOS14},
    {PCT_COMBO,TEXT("WR PITCH"         ),TEXT("71 MODULATION WHEEL PITCH MODULATION RANGE"    ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9,COLPOS08},
    {PCT_COMBO,TEXT("WR AMPLITUDE"     ),TEXT("72 MODULATION WHEEL AMPLITUDE MODULATION RANGE"),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9,COLPOS09},
    {PCT_COMBO,TEXT("BC PITCH"         ),TEXT("73 BREATH CONTROL PITCH MODULATION RANGE"      ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9,COLPOS10},
    {PCT_COMBO,TEXT("BC AMPLITUDE"     ),TEXT("74 BREATH CONTROL AMPLITUDE MODULATION RANGE"  ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9,COLPOS11},
    {PCT_COMBO,TEXT("BC PITCH BIAS"    ),TEXT("75 BREATH CONTROL PITCH BIAS RANGE"            ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9,COLPOS12},
    {PCT_COMBO,TEXT("BC EG BIAS"       ),TEXT("76 BREATH CONTROL EG BIAS RANGE"               ),TRUE   ,0       , 99     , NULL                ,  0, ROW_POS_EDIT9,COLPOS13},
    {PCT_EDIT ,TEXT("PATCHNAME"        ),TEXT("77-86 PATCHNAME"                               ),TRUE   ,0       ,127     , NULL                , 50, ROW_POS_EDIT0,COLPOS01},
    {PCT_COMBO,TEXT("PR1"              ),TEXT("87 PITCH EG RATE 1"                            ),FALSE  ,0       ,127     , NULL                ,  0, ROW_POS_EDIT5,COLPOS11},
    {PCT_COMBO,TEXT("PR2"              ),TEXT("88 PITCH EG RATE 2"                            ),FALSE  ,0       ,127     , NULL                ,  0, ROW_POS_EDIT5,COLPOS13},
    {PCT_COMBO,TEXT("PR3"              ),TEXT("89 PITCH EG RATE 3"                            ),FALSE  ,0       ,127     , NULL                ,  0, ROW_POS_EDIT5,COLPOS15},
    {PCT_COMBO,TEXT("PL1"              ),TEXT("90 PITCH EG LEVEL 1"                           ),FALSE  ,0       ,127     , NULL                ,  0, ROW_POS_EDIT5,COLPOS12},
    {PCT_COMBO,TEXT("PL2"              ),TEXT("91 PITCH EG LEVEL 2"                           ),FALSE  ,0       ,127     , NULL                ,  0, ROW_POS_EDIT5,COLPOS14},
    {PCT_COMBO,TEXT("PL3"              ),TEXT("92 PITCH EG LEVEL 3"                           ),FALSE  ,0       ,127     , NULL                ,  0, ROW_POS_EDIT5,COLPOS16},

    {PCT_STATIC,TEXT("OPERATOR1"        ),TEXT("OPERATOR1"                                  ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_EDIT5  ,COLPOS00},
    {PCT_STATIC,TEXT("OPERATOR2"        ),TEXT("OPERATOR2"                                  ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_EDIT6  ,COLPOS00},
    {PCT_STATIC,TEXT("OPERATOR3"        ),TEXT("OPERATOR3"                                  ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_EDIT7  ,COLPOS00},
    {PCT_STATIC,TEXT("OPERATOR4"        ),TEXT("OPERATOR4"                                  ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_EDIT8  ,COLPOS00},
    {PCT_STATIC,TEXT("PATCHNAME"        ),TEXT("PATCHNAME"                                  ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_EDIT0  ,COLPOS00},
    {PCT_STATIC,TEXT("ALGORITHM"        ),TEXT("ALGORITHM"                                  ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS01},
    {PCT_STATIC,TEXT("FEEDBACK"         ),TEXT("FEEDBACK LEVEL"                             ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS02},
    {PCT_STATIC,TEXT("LFO WAVE"         ),TEXT("LFO WAVE"                                   ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS03},
    {PCT_STATIC,TEXT("SPEED"            ),TEXT("LFO SPEED"                                  ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS04},
    {PCT_STATIC,TEXT("DELAY"            ),TEXT("LFO DELAY"                                  ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS05},
    {PCT_STATIC,TEXT("PMD"              ),TEXT("PITCH MODULATION DEPTH"                     ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS06},
    {PCT_STATIC,TEXT("AMD"              ),TEXT("AMPLITUDE MODULATION DEPTH"                 ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS07},
    {PCT_STATIC,TEXT("LFO SYNC"         ),TEXT("LFO SYNC"                                   ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS08},
    {PCT_STATIC,TEXT("PITCH"            ),TEXT("PITCH MODULATION SENSITIVITY"               ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS09},
    {PCT_STATIC,TEXT("AMPLITUDE"        ),TEXT("AMPLITUDE MODULATION SENSITIVITY"           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS10},
    {PCT_STATIC,TEXT("AME"              ),TEXT("OP? AMPLITUDE MODULATION ENABLE"            ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS11},
    {PCT_STATIC,TEXT("EG BIAS"          ),TEXT("OP? EG BIAS SENSITIVITY"                    ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS12},
    {PCT_STATIC,TEXT("VELOCITY"         ),TEXT("OP? KEY VELOCITY"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC0,COLPOS13},
    {PCT_STATIC,TEXT("FREQ RATIO"       ),TEXT("OP? OSCILLATOR FREQUENCY"                   ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS01},
    {PCT_STATIC,TEXT("DETUNE"           ),TEXT("OP? DETUNE"                                 ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS02},
    {PCT_STATIC,TEXT("AR"               ),TEXT("OP? ATTACK RATE"                            ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS03},
    {PCT_STATIC,TEXT("D1R"              ),TEXT("OP? DECAY 1 RATE"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS04},
    {PCT_STATIC,TEXT("D1L"              ),TEXT("OP? DECAY 1 LEVEL"                          ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS05},
    {PCT_STATIC,TEXT("D2R"              ),TEXT("OP? DECAY 2 RATE"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS06},
    {PCT_STATIC,TEXT("RR"               ),TEXT("OP? RELEASE RATE"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS07},
    {PCT_STATIC,TEXT("OUT LEVEL"        ),TEXT("OP? OUTPUT LEVEL"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS08},
    {PCT_STATIC,TEXT("RATE"             ),TEXT("OP? KEYBOARD SCALING RATE"                  ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS09},
    {PCT_STATIC,TEXT("LEVEL"            ),TEXT("OP? KEYBOARD SCALING LEVEL"                 ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS10},
    {PCT_STATIC,TEXT("PR1"              ),TEXT("PITCH EG RATE 1"                            ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS11},
    {PCT_STATIC,TEXT("PL1"              ),TEXT("PITCH EG LEVEL 1"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS12},
    {PCT_STATIC,TEXT("PR2"              ),TEXT("PITCH EG RATE 2"                            ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS13},
    {PCT_STATIC,TEXT("PL2"              ),TEXT("PITCH EG LEVEL 2"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS14},
    {PCT_STATIC,TEXT("PR3"              ),TEXT("PITCH EG RATE 3"                            ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS15},
    {PCT_STATIC,TEXT("PL3"              ),TEXT("PITCH EG LEVEL 3"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC1,COLPOS16},
    {PCT_STATIC,TEXT("POLY/MONO"        ),TEXT("PLAY MODE POLY/MONO"                        ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS01},
    {PCT_STATIC,TEXT("PB RANGE"         ),TEXT("PITCH BEND RANGE"                           ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS02},
    {PCT_STATIC,TEXT("PORTA MODE"       ),TEXT("PORTAMENTO MODE"                            ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS03},
    {PCT_STATIC,TEXT("PORTA TIME"       ),TEXT("PORTAMENTO TIME"                            ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS04},
    {PCT_STATIC,TEXT("PORTAMENT FOOT SW"),TEXT("PORTAMENT FOOT SWITCH"                      ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS05},
    {PCT_STATIC,TEXT("FOOT VOLUME RANGE"),TEXT("FOOT VOLUME RANGE"                          ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS06},
    {PCT_STATIC,TEXT("SUSTAIN FOOT SW"  ),TEXT("SUSTAIN FOOT SWITCH"                        ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS07},
    {PCT_STATIC,TEXT("WR PITCH"         ),TEXT("MODULATION WHEEL PITCH MODULATION RANGE"    ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS08},
    {PCT_STATIC,TEXT("WR AMPLITUDE"     ),TEXT("MODULATION WHEEL AMPLITUDE MODULATION RANGE"),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS09},
    {PCT_STATIC,TEXT("BC PITCH"         ),TEXT("BREATH CONTROL PITCH MODULATION RANGE"      ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS10},
    {PCT_STATIC,TEXT("BC AMPLITUDE"     ),TEXT("BREATH CONTROL AMPLITUDE MODULATION RANGE"  ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS11},
    {PCT_STATIC,TEXT("BC PITCH BIAS"    ),TEXT("BREATH CONTROL PITCH BIAS RANGE"            ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS12},
    {PCT_STATIC,TEXT("BC EG BIAS"       ),TEXT("BREATH CONTROL EG BIAS RANGE"               ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS13},
    {PCT_STATIC,TEXT("CHORUS SWITCH"    ),TEXT("CHORUS SWITCH"                              ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS14},
    {PCT_STATIC,TEXT("TRANSPOSE"        ),TEXT("TRANSPOSE"                                  ),TRUE   ,0       ,0       , NULL                ,  0, ROW_POS_STATIC2,COLPOS15},

    {PCT_EDIT ,TEXT("VOICE_NAME_01"    ),TEXT("VOICE_NAME_01"                                 ),TRUE   ,0       ,127     , NULL                , 50, 0    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_02"    ),TEXT("VOICE_NAME_02"                                 ),TRUE   ,0       ,127     , NULL                , 50, 1    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_03"    ),TEXT("VOICE_NAME_03"                                 ),TRUE   ,0       ,127     , NULL                , 50, 2    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_04"    ),TEXT("VOICE_NAME_04"                                 ),TRUE   ,0       ,127     , NULL                , 50, 3    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_05"    ),TEXT("VOICE_NAME_05"                                 ),TRUE   ,0       ,127     , NULL                , 50, 4    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_06"    ),TEXT("VOICE_NAME_06"                                 ),TRUE   ,0       ,127     , NULL                , 50, 5    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_07"    ),TEXT("VOICE_NAME_07"                                 ),TRUE   ,0       ,127     , NULL                , 50, 6    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_08"    ),TEXT("VOICE_NAME_08"                                 ),TRUE   ,0       ,127     , NULL                , 50, 7    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_09"    ),TEXT("VOICE_NAME_09"                                 ),TRUE   ,0       ,127     , NULL                , 50, 8    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_10"    ),TEXT("VOICE_NAME_10"                                 ),TRUE   ,0       ,127     , NULL                , 50, 9    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_11"    ),TEXT("VOICE_NAME_11"                                 ),TRUE   ,0       ,127     , NULL                , 50,10    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_12"    ),TEXT("VOICE_NAME_12"                                 ),TRUE   ,0       ,127     , NULL                , 50,11    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_13"    ),TEXT("VOICE_NAME_13"                                 ),TRUE   ,0       ,127     , NULL                , 50,12    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_14"    ),TEXT("VOICE_NAME_14"                                 ),TRUE   ,0       ,127     , NULL                , 50,13    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_15"    ),TEXT("VOICE_NAME_15"                                 ),TRUE   ,0       ,127     , NULL                , 50,14    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_16"    ),TEXT("VOICE_NAME_16"                                 ),TRUE   ,0       ,127     , NULL                , 50,15    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_17"    ),TEXT("VOICE_NAME_17"                                 ),TRUE   ,0       ,127     , NULL                , 50,16    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_18"    ),TEXT("VOICE_NAME_18"                                 ),TRUE   ,0       ,127     , NULL                , 50,17    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_19"    ),TEXT("VOICE_NAME_19"                                 ),TRUE   ,0       ,127     , NULL                , 50,18    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_20"    ),TEXT("VOICE_NAME_20"                                 ),TRUE   ,0       ,127     , NULL                , 50,19    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_21"    ),TEXT("VOICE_NAME_21"                                 ),TRUE   ,0       ,127     , NULL                , 50,20    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_22"    ),TEXT("VOICE_NAME_22"                                 ),TRUE   ,0       ,127     , NULL                , 50,21    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_23"    ),TEXT("VOICE_NAME_23"                                 ),TRUE   ,0       ,127     , NULL                , 50,22    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_24"    ),TEXT("VOICE_NAME_24"                                 ),TRUE   ,0       ,127     , NULL                , 50,23    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_25"    ),TEXT("VOICE_NAME_25"                                 ),TRUE   ,0       ,127     , NULL                , 50,24    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_26"    ),TEXT("VOICE_NAME_26"                                 ),TRUE   ,0       ,127     , NULL                , 50,25    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_27"    ),TEXT("VOICE_NAME_27"                                 ),TRUE   ,0       ,127     , NULL                , 50,26    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_28"    ),TEXT("VOICE_NAME_28"                                 ),TRUE   ,0       ,127     , NULL                , 50,27    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_29"    ),TEXT("VOICE_NAME_29"                                 ),TRUE   ,0       ,127     , NULL                , 50,28    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_30"    ),TEXT("VOICE_NAME_30"                                 ),TRUE   ,0       ,127     , NULL                , 50,29    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_31"    ),TEXT("VOICE_NAME_31"                                 ),TRUE   ,0       ,127     , NULL                , 50,30    , 0},
    {PCT_EDIT ,TEXT("VOICE_NAME_32"    ),TEXT("VOICE_NAME_32"                                 ),TRUE   ,0       ,127     , NULL                , 50,31    , 0},
    {PCT_EDIT ,TEXT("VOICE_BULK_01"    ),TEXT("VOICE_BULK_01"                                 ),TRUE   ,0       ,127     , NULL                ,250, 0    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_02"    ),TEXT("VOICE_BULK_02"                                 ),TRUE   ,0       ,127     , NULL                ,250, 1    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_03"    ),TEXT("VOICE_BULK_03"                                 ),TRUE   ,0       ,127     , NULL                ,250, 2    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_04"    ),TEXT("VOICE_BULK_04"                                 ),TRUE   ,0       ,127     , NULL                ,250, 3    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_05"    ),TEXT("VOICE_BULK_05"                                 ),TRUE   ,0       ,127     , NULL                ,250, 4    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_06"    ),TEXT("VOICE_BULK_06"                                 ),TRUE   ,0       ,127     , NULL                ,250, 5    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_07"    ),TEXT("VOICE_BULK_07"                                 ),TRUE   ,0       ,127     , NULL                ,250, 6    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_08"    ),TEXT("VOICE_BULK_08"                                 ),TRUE   ,0       ,127     , NULL                ,250, 7    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_09"    ),TEXT("VOICE_BULK_09"                                 ),TRUE   ,0       ,127     , NULL                ,250, 8    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_10"    ),TEXT("VOICE_BULK_10"                                 ),TRUE   ,0       ,127     , NULL                ,250, 9    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_11"    ),TEXT("VOICE_BULK_11"                                 ),TRUE   ,0       ,127     , NULL                ,250,10    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_12"    ),TEXT("VOICE_BULK_12"                                 ),TRUE   ,0       ,127     , NULL                ,250,11    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_13"    ),TEXT("VOICE_BULK_13"                                 ),TRUE   ,0       ,127     , NULL                ,250,12    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_14"    ),TEXT("VOICE_BULK_14"                                 ),TRUE   ,0       ,127     , NULL                ,250,13    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_15"    ),TEXT("VOICE_BULK_15"                                 ),TRUE   ,0       ,127     , NULL                ,250,14    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_16"    ),TEXT("VOICE_BULK_16"                                 ),TRUE   ,0       ,127     , NULL                ,250,15    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_17"    ),TEXT("VOICE_BULK_17"                                 ),TRUE   ,0       ,127     , NULL                ,250,16    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_18"    ),TEXT("VOICE_BULK_18"                                 ),TRUE   ,0       ,127     , NULL                ,250,17    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_19"    ),TEXT("VOICE_BULK_19"                                 ),TRUE   ,0       ,127     , NULL                ,250,18    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_20"    ),TEXT("VOICE_BULK_20"                                 ),TRUE   ,0       ,127     , NULL                ,250,19    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_21"    ),TEXT("VOICE_BULK_21"                                 ),TRUE   ,0       ,127     , NULL                ,250,20    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_22"    ),TEXT("VOICE_BULK_22"                                 ),TRUE   ,0       ,127     , NULL                ,250,21    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_23"    ),TEXT("VOICE_BULK_23"                                 ),TRUE   ,0       ,127     , NULL                ,250,22    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_24"    ),TEXT("VOICE_BULK_24"                                 ),TRUE   ,0       ,127     , NULL                ,250,23    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_25"    ),TEXT("VOICE_BULK_25"                                 ),TRUE   ,0       ,127     , NULL                ,250,24    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_26"    ),TEXT("VOICE_BULK_26"                                 ),TRUE   ,0       ,127     , NULL                ,250,25    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_27"    ),TEXT("VOICE_BULK_27"                                 ),TRUE   ,0       ,127     , NULL                ,250,26    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_28"    ),TEXT("VOICE_BULK_28"                                 ),TRUE   ,0       ,127     , NULL                ,250,27    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_29"    ),TEXT("VOICE_BULK_29"                                 ),TRUE   ,0       ,127     , NULL                ,250,28    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_30"    ),TEXT("VOICE_BULK_30"                                 ),TRUE   ,0       ,127     , NULL                ,250,29    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_31"    ),TEXT("VOICE_BULK_31"                                 ),TRUE   ,0       ,127     , NULL                ,250,30    , 2},
    {PCT_EDIT ,TEXT("VOICE_BULK_32"    ),TEXT("VOICE_BULK_32"                                 ),TRUE   ,0       ,127     , NULL                ,250,31    , 2},
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
