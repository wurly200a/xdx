/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */

/* 外部関数定義 */
#include "WinMain.h"

/* 外部変数定義 */

/* 内部関数定義 */
#include "ParamCtrl.h"

static BOOL paramCtrlCreate( HWND hwnd, PARAM_CTRL_GROUP_ID groupId, PARAM_CTRL_ID startId, PARAM_CTRL_ID endId );
/* 内部変数定義 */


char *strTblOnOff[2] = {"off","on"};
char *strTblAlgorithm[8] = {"1","2","3","4","5","6","7","8"};
char *strTblLfoWave[4]   = {"saw up","square","triangl","S/Hold"};
char *strTblTranspose[49]  = {"C 1","C#1","D 1","D#1","E 1","F 1","F#1","G 1","G#1","A 1","A#1","B 1","C 2","C#2","D 2","D#2","E 2","F 2","F#2","G 2","G#2","A 2","A#2","B 2","C 3","C#3","D 3","D#3","E 3","F 3","F#3","G 3","G#3","A 3","A#3","B 3","C 4","C#4","D 4","D#4","E 4","F 4","F#4","G 4","G#4","A 4","A#4","B 4","C 5"};
char *strTblPolyMono[2] = {"Poly Mode","Mono Mode"};
char *strTblPortamentoMode[2] = {"Full Time Porta","Fingered Porta"};
char *strTblFootSwAssign[2] = {"Portamento","Sustain"};

enum
{
   PCT_COMBO,
   PCT_EDIT ,
   PCT_MAX
};
typedef BYTE PARAM_CTRL_TYPE;


typedef struct
{
    PARAM_CTRL_TYPE type          ;
    PTSTR           strText       ; /* ウィンドウテキスト                         */
    BOOL            bEnable       ;
    INT             minValue      ;
    INT             maxValue      ;
    char **         ptstrDataDisp ;
    INT             xBasePosOffset; /* ベースx座標 */
    INT             yPosAdd       ; /* y座標加算値 */
    BOOL            bWrapTrig     ;
} S_PARAM_CTRL;

S_PARAM_CTRL paramListTbl[PARAM_CTRL_MAX] =
{/*  type     ,strText                                               ,bEnable,minValue,maxValue,ptstrDataDisp,xBasePosOffset,yPosAdd,bWrapTrig */
    {PCT_COMBO,TEXT("SOUNDMODE"                                     ),TRUE   ,0,  2, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("00 OP4 ATTACK RATE"                            ),TRUE   ,0, 31, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("01 OP4 DECAY 1 RATE"                           ),TRUE   ,0, 31, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("02 OP4 DECAY 2 RATE"                           ),TRUE   ,0, 31, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("03 OP4 RELEASE RATE"                           ),TRUE   ,0, 15, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("04 OP4 DECAY 1 LEVEL"                          ),TRUE   ,0, 15, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("05 OP4 KEYBOARD SCALING LEVEL"                 ),TRUE   ,0, 99, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("06 OP4 KEYBOARD SCALING RATE"                  ),TRUE   ,0,  3, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("07 OP4 EG BIAS SENSITIVITY"                    ),TRUE   ,0,  7, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("08 OP4 AMPLITUDE MODULATION ENABLE"            ),TRUE   ,0,  1, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("09 OP4 KEY VELOCITY"                           ),TRUE   ,0,  7, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("10 OP4 OUTPUT LEVEL"                           ),TRUE   ,0, 99, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("11 OP4 OSCILLATOR FREQUENCY"                   ),TRUE   ,0, 63, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("12 OP4 DETUNE"                                 ),TRUE   ,0,  6, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("13 OP3 ATTACK RATE"                            ),TRUE   ,0, 31, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("14 OP3 DECAY 1 RATE"                           ),TRUE   ,0, 31, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("15 OP3 DECAY 2 RATE"                           ),TRUE   ,0, 31, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("16 OP3 RELEASE RATE"                           ),TRUE   ,0, 15, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("17 OP3 DECAY 1 LEVEL"                          ),TRUE   ,0, 15, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("18 OP3 KEYBOARD SCALING LEVEL"                 ),TRUE   ,0, 99, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("19 OP3 KEYBOARD SCALING RATE"                  ),TRUE   ,0,  3, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("20 OP3 EG BIAS SENSITIVITY"                    ),TRUE   ,0,  7, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("21 OP3 AMPLITUDE MODULATION ENABLE"            ),TRUE   ,0,  1, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("22 OP3 KEY VELOCITY"                           ),TRUE   ,0,  7, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("23 OP3 OUTPUT LEVEL"                           ),TRUE   ,0, 99, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("24 OP3 OSCILLATOR FREQUENCY"                   ),TRUE   ,0, 63, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("25 OP3 DETUNE"                                 ),TRUE   ,0,  6, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("26 OP2 ATTACK RATE"                            ),TRUE   ,0, 31, NULL                ,0, 0,TRUE },
    {PCT_COMBO,TEXT("27 OP2 DECAY 1 RATE"                           ),TRUE   ,0, 31, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("28 OP2 DECAY 2 RATE"                           ),TRUE   ,0, 31, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("29 OP2 RELEASE RATE"                           ),TRUE   ,0, 15, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("30 OP2 DECAY 1 LEVEL"                          ),TRUE   ,0, 15, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("31 OP2 KEYBOARD SCALING LEVEL"                 ),TRUE   ,0, 99, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("32 OP2 KEYBOARD SCALING RATE"                  ),TRUE   ,0,  3, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("33 OP2 EG BIAS SENSITIVITY"                    ),TRUE   ,0,  7, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("34 OP2 AMPLITUDE MODULATION ENABLE"            ),TRUE   ,0,  1, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("35 OP2 KEY VELOCITY"                           ),TRUE   ,0,  7, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("36 OP2 OUTPUT LEVEL"                           ),TRUE   ,0, 99, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("37 OP2 OSCILLATOR FREQUENCY"                   ),TRUE   ,0, 63, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("38 OP2 DETUNE"                                 ),TRUE   ,0,  6, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("39 OP1 ATTACK RATE"                            ),TRUE   ,0, 31, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("40 OP1 DECAY 1 RATE"                           ),TRUE   ,0, 31, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("41 OP1 DECAY 2 RATE"                           ),TRUE   ,0, 31, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("42 OP1 RELEASE RATE"                           ),TRUE   ,0, 15, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("43 OP1 DECAY 1 LEVEL"                          ),TRUE   ,0, 15, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("44 OP1 KEYBOARD SCALING LEVEL"                 ),TRUE   ,0, 99, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("45 OP1 KEYBOARD SCALING RATE"                  ),TRUE   ,0,  3, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("46 OP1 EG BIAS SENSITIVITY"                    ),TRUE   ,0,  7, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("47 OP1 AMPLITUDE MODULATION ENABLE"            ),TRUE   ,0,  1, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("48 OP1 KEY VELOCITY"                           ),TRUE   ,0,  7, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("49 OP1 OUTPUT LEVEL"                           ),TRUE   ,0, 99, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("50 OP1 OSCILLATOR FREQUENCY"                   ),TRUE   ,0, 63, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("51 OP1 DETUNE"                                 ),TRUE   ,0,  6, NULL                ,0,20,FALSE},
    {PCT_COMBO,TEXT("52 ALGORITHM"                                  ),TRUE   ,0,  7, strTblAlgorithm     ,0, 0,TRUE },
    {PCT_COMBO,TEXT("53 FEEDBACK LEVEL"                             ),TRUE   ,0,  7, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("54 LFO SPEED"                                  ),TRUE   ,0, 99, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("55 LFO DELAY"                                  ),TRUE   ,0, 99, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("56 PITCH MODULATION DEPTH"                     ),TRUE   ,0, 99, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("57 AMPLITUDE MODULATION DEPTH"                 ),TRUE   ,0, 99, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("58 LFO SYNC"                                   ),TRUE   ,0,  1, strTblOnOff         ,0, 0,FALSE},
    {PCT_COMBO,TEXT("59 LFO WAVE"                                   ),TRUE   ,0,  3, strTblLfoWave       ,0, 0,FALSE},
    {PCT_COMBO,TEXT("60 PITCH MODULATION SENSITIVITY"               ),TRUE   ,0,  7, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("61 AMPLITUDE MODULATION SENSITIVITY"           ),TRUE   ,0,  7, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("62 TRANSPOSE"                                  ),TRUE   ,0, 48, strTblTranspose     ,0, 0,FALSE},
    {PCT_COMBO,TEXT("63 PLAY MODE POLY/MONO"                        ),TRUE   ,0,  1, strTblPolyMono      ,0, 0,FALSE},
    {PCT_COMBO,TEXT("64 PITCH BEND RANGE"                           ),TRUE   ,0, 12, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("65 PORTAMENTO MODE"                            ),TRUE   ,0,  1, strTblPortamentoMode,0, 0,FALSE},
    {PCT_COMBO,TEXT("66 PORTAMENTO TIME"                            ),TRUE   ,0, 99, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("67 FOOT VOLUME RANGE"                          ),FALSE  ,0,127, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("68 SUSTAIN FOOT SWITCH"                        ),TRUE   ,0,  1, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("69 PORTAMENT FOOT SWITCH"                      ),TRUE   ,0,  1, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("70 CHORUS SWITCH"                              ),FALSE  ,0,  1, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("71 MODULATION WHEEL PITCH MODULATION RANGE"    ),TRUE   ,0, 99, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("72 MODULATION WHEEL AMPLITUDE MODULATION RANGE"),TRUE   ,0, 99, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("73 BREATH CONTROL PITCH MODULATION RANGE"      ),TRUE   ,0, 99, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("74 BREATH CONTROL AMPLITUDE MODULATION RANGE"  ),TRUE   ,0, 99, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("75 BREATH CONTROL PITCH BIAS RANGE"            ),TRUE   ,0, 99, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("76 BREATH CONTROL EG BIAS RANGE"               ),TRUE   ,0, 99, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("77-86 PATCHNAME"                               ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("87 PITCH EG RATE 1"                            ),FALSE  ,0,127, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("88 PITCH EG RATE 2"                            ),FALSE  ,0,127, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("89 PITCH EG RATE 3"                            ),FALSE  ,0,127, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("90 PITCH EG LEVEL 1"                           ),FALSE  ,0,127, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("91 PITCH EG LEVEL 2"                           ),FALSE  ,0,127, NULL                ,0, 0,FALSE},
    {PCT_COMBO,TEXT("92 PITCH EG LEVEL 3"                           ),FALSE  ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_00"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_01"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_02"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_03"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_04"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_05"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_06"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_07"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_08"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_09"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_10"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_11"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_12"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_13"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_14"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_15"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_16"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_17"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_18"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_19"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_20"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_21"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_22"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
    {PCT_EDIT ,TEXT("VOICE_NAME_23"                                 ),TRUE   ,0,127, NULL                ,0, 0,FALSE},
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
    HWND                hwnd_name                    ; /* ウィンドウのハンドラ               */
    HWND                hwnd_data                    ; /* ウィンドウのハンドラ               */
    S_PARAM_WINDOW_INFO wtInfo[PARAM_CTRL_TARGET_MAX];
    PARAM_CTRL_GROUP_ID groupId                      ;
} S_PARAM_INFO;

S_PARAM_INFO ctrlParamInfo[PARAM_CTRL_MAX];

#define PARAM_STATIC_X_BASE_POS  10
#define PARAM_STATIC_Y_BASE_POS 150
#define PARAM_STATIC_WIDTH      150
#define PARAM_STATIC_HEIGHT      15

#define PARAM_STATIC_Y_UNIT      20

#define PARAM_DATA_X_BASE_POS  250
#define PARAM_DATA_Y_BASE_POS  150
#define PARAM_DATA_WIDTH        40
#define PARAM_DATA_HEIGHT      300

#define PARAM_DATA_Y_UNIT       20

#define PARAM_WRAP_NUM      26 /* 1列あたりの行数 */
#define PARAM_WRAP_UNIT    /*220*/ 300      /* 列のピッチ */

/********************************************************************************
 * 内容  : 登録された全てのパラメータコントロールを生成する
 * 引数  : HWND hwnd 親ウィンドウのハンドラ
 * 戻り値: BOOL
 ***************************************/
BOOL
ParamCtrlCreate( HWND hwnd )
{
    paramCtrlCreate(hwnd,PARAM_CTRL_GROUP_SYSTEM_COMMON,PARAM_CTRL_SYSCMN_SOUNDMODE  ,PARAM_CTRL_SYSCMN_SOUNDMODE  );
    paramCtrlCreate(hwnd,PARAM_CTRL_GROUP_1VOICE       ,PARAM_CTRL_VOICE_00          ,PARAM_CTRL_VOICE_92          );
    paramCtrlCreate(hwnd,PARAM_CTRL_GROUP_ALL_VOICE    ,PARAM_CTRL_ALL_VOICE_NAME_00 ,PARAM_CTRL_ALL_VOICE_NAME_23 );

    ParamCtrlGroupDisplay(PARAM_CTRL_GROUP_SYSTEM_COMMON);

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
    int nowId,numForDisp;
    INT columnNum = 0;

    if( (startId <= endId) && (endId < PARAM_CTRL_MAX) )
    {
        for(nowId=startId,numForDisp=0; nowId<=endId; nowId++,numForDisp++)
        {
            PTSTR class;
            DWORD style;
            DWORD exStyle;
            DWORD height;
            INT   width;
            INT xPos;
            S_PARAM_INFO *infoPtr = &ctrlParamInfo[nowId];
            S_PARAM_CTRL *tblPtr = &paramListTbl[nowId];

#if 1
            if( tblPtr->bWrapTrig )
            {
                columnNum++;
            }
            else
            {
                nop();
            }
#else
            if( !numForDisp || numForDisp%PARAM_WRAP_NUM )
            {
                nop();
            }
            else
            {
                columnNum++;
            }
#endif

            if( tblPtr->type == PCT_COMBO )
            {
                class = (PTSTR)TEXT("combobox");
                style = (WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
                exStyle = 0;
                height = PARAM_DATA_HEIGHT;
                width  = PARAM_DATA_WIDTH;
            }
            else
            {
                class = (PTSTR)TEXT("edit");
                style = (WS_CHILD|ES_LEFT);

                if( PARAM_CTRL_ALL_VOICE_NAME_00 <= nowId && nowId <= PARAM_CTRL_ALL_VOICE_NAME_23)
                {
                    style |= ES_READONLY;
                }
                else
                {
                    nop();
                }

                exStyle = WS_EX_OVERLAPPEDWINDOW;
                height = 20;
                width  = 100;
            }

            /* スタティックコントロール(ここから) */
            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].xPos   = tblPtr->xBasePosOffset + PARAM_STATIC_X_BASE_POS+(PARAM_WRAP_UNIT*columnNum);
            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].yPos   = tblPtr->yPosAdd + PARAM_STATIC_Y_BASE_POS+(PARAM_STATIC_Y_UNIT*(numForDisp-(PARAM_WRAP_NUM*columnNum)));
            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].width  = strlen(tblPtr->strText)*8;
            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].height = PARAM_STATIC_HEIGHT;

            infoPtr->hwnd_name = CreateWindowEx( 0,                                              /* 拡張スタイル       */
                                                TEXT("static"),                                 /* クラス名           */
                                                tblPtr->strText,                        /* ウィンドウテキスト */
                                                (WS_CHILD|ES_LEFT),                             /* スタイル           */
                                                infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].xPos,   /* x座標              */
                                                infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].yPos,   /* y座標              */
                                                infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].width,  /* 幅                 */
                                                infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].height, /* 高さ               */
                                                hwnd,(HMENU)(PARAM_CTRL_ID_OFFSET+nowId*2),         /* 親ウィンドウ,子ウィンドウID */
                                                GetHinst(),NULL );                              /* インスタンスハンドル,補助引数 */
            if( infoPtr->hwnd_name != NULL )
            {
                SendMessage(infoPtr->hwnd_name, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));

                /* 個別に処理するもの */
                switch( nowId )
                {
                default:
                    break;
                }

                ShowWindow(infoPtr->hwnd_name, SW_HIDE);
            }
            else
            {
                /* do nothing */
            }
            /* スタティックコントロール(ここまで) */

            /* パラメータを管理するコントロール(ここから) */
            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].xPos   = tblPtr->xBasePosOffset + PARAM_DATA_X_BASE_POS+(PARAM_WRAP_UNIT*columnNum);
            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].yPos   = tblPtr->yPosAdd + PARAM_DATA_Y_BASE_POS+(PARAM_DATA_Y_UNIT*(numForDisp-(PARAM_WRAP_NUM*columnNum)));
            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].width  = width;
            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].height = height;

            infoPtr->hwnd_data = CreateWindowEx( exStyle,                                    /* 拡張スタイル       */
                                                class,                                      /* クラス名           */
                                                0,                                          /* ウィンドウテキスト */
                                                style,                                      /* スタイル           */
                                                infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].xPos,   /* x座標              */
                                                infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].yPos,   /* y座標              */
                                                infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].width,  /* 幅                 */
                                                infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].height, /* 高さ               */
                                                hwnd,(HMENU)(PARAM_CTRL_ID_OFFSET+(nowId*2)+1),/* 親ウィンドウ,子ウィンドウID */
                                                GetHinst(),NULL );                        /* インスタンスハンドル,補助引数 */
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
            ShowWindow(infoPtr->hwnd_name, SW_SHOW);
            ShowWindow(infoPtr->hwnd_data, SW_SHOW);
        }
        else
        {
            ShowWindow(infoPtr->hwnd_name, SW_HIDE);
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

        MoveWindow( infoPtr->hwnd_name,
                    infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].xPos - xPos,   /* x座標              */
                    infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].yPos - yPos,   /* y座標              */
                    infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].width,  /* 幅                 */
                    infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].height, /* 高さ               */
                    TRUE);

        MoveWindow( infoPtr->hwnd_data,
                    infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].xPos - xPos,   /* x座標              */
                    infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].yPos - yPos,   /* y座標              */
                    infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].width,  /* 幅                 */
                    infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].height, /* 高さ               */
                    TRUE);
    }
}
