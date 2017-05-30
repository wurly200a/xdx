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
    INT             minValue      ;
    INT             maxValue      ;
    INT             xBasePosOffset;
} S_PARAM_CTRL;

S_PARAM_CTRL paramListTbl[PARAM_CTRL_MAX] =
{
    {PCT_COMBO,TEXT("SOUNDMODE                "),0,  2,170},

    {PCT_EDIT ,TEXT("PATCHNAME"             ),0,127,110},

    {PCT_COMBO,TEXT("TONESWITCH                 "),0,127,170},
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

#define PARAM_DATA_Y_BASE_POS  150
#define PARAM_DATA_WIDTH        40
#define PARAM_DATA_HEIGHT      300

#define PARAM_DATA_Y_UNIT       20

#define PARAM_WRAP_NUM      26 /* 1列あたりの行数 */
#define PARAM_WRAP_UNIT    220

/********************************************************************************
 * 内容  : 登録された全てのパラメータコントロールを生成する
 * 引数  : HWND hwnd 親ウィンドウのハンドラ
 * 戻り値: BOOL
 ***************************************/
BOOL
ParamCtrlCreate( HWND hwnd )
{
    paramCtrlCreate(hwnd,PARAM_CTRL_GROUP_SYSTEM_COMMON,PARAM_CTRL_SYSCMN_SOUNDMODE  ,PARAM_CTRL_SYSCMN_SOUNDMODE  );
    paramCtrlCreate(hwnd,PARAM_CTRL_GROUP_PATCH_COMMON ,PARAM_CTRL_PATCHCMN_PATCHNAME,PARAM_CTRL_PATCHCMN_PATCHNAME);
    paramCtrlCreate(hwnd,PARAM_CTRL_GROUP_PATCH_TONE   ,PARAM_CTRL_PATCHTN_TONESWITCH,PARAM_CTRL_PATCHTN_TONESWITCH);

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

    if( (startId < endId) && (endId < PARAM_CTRL_MAX) )
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

            if( !numForDisp || numForDisp%PARAM_WRAP_NUM )
            {
                nop();
            }
            else
            {
                columnNum++;
            }

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
                exStyle = WS_EX_OVERLAPPEDWINDOW;
                height = 20;
                width  = 100;
            }

            /* スタティックコントロール(ここから) */
            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].xPos   = PARAM_STATIC_X_BASE_POS+(PARAM_WRAP_UNIT*columnNum);
            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].yPos   = PARAM_STATIC_Y_BASE_POS+(PARAM_STATIC_Y_UNIT*(numForDisp-(PARAM_WRAP_NUM*columnNum)));

//        infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].width  = PARAM_STATIC_WIDTH;
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
            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].xPos   = tblPtr->xBasePosOffset+(PARAM_WRAP_UNIT*columnNum);
            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].yPos   = PARAM_DATA_Y_BASE_POS+(PARAM_DATA_Y_UNIT*(numForDisp-(PARAM_WRAP_NUM*columnNum)));
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
                        wsprintf(szBuff,TEXT("%d"),j);
                        iCbNum = SendMessage(infoPtr->hwnd_data , CB_ADDSTRING, 0, (LPARAM)&szBuff[0]);
                        SendMessage( infoPtr->hwnd_data , CB_SETITEMDATA, iCbNum, (LPARAM)j );
                    }
                    SendMessage( infoPtr->hwnd_data , CB_SETCURSEL, 0, (LPARAM)0 );
                }
                else
                {
                    nop();
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
 * 内容  : コントロールのサイズを調整する
 * 引数  : PARAM_CTRL_ID id
 * 引数  : PARAM_CTRL_TARGET target
 * 引数  : INT           width
 * 戻り値: なし
 ***************************************/
void
ParamCtrlSetSize( PARAM_CTRL_ID id, PARAM_CTRL_TARGET target, INT width )
{
    int i;

    if( (id < PARAM_CTRL_MAX) && (target < PARAM_CTRL_TARGET_MAX) )
    {
        for(i=0; i<PARAM_CTRL_MAX; i++)
        {
            S_PARAM_INFO *infoPtr = &ctrlParamInfo[i];

            if( target == PARAM_CTRL_TARGET_NAME )
            {
                MoveWindow( infoPtr->hwnd_name,
                            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].xPos,   /* x座標              */
                            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].yPos,   /* y座標              */
                            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].width,  /* 幅                 */
                            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].height, /* 高さ               */
                            TRUE);
            }
            else
            {
                MoveWindow( infoPtr->hwnd_data,
                            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].xPos,   /* x座標              */
                            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].yPos,   /* y座標              */
                            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].width,  /* 幅                 */
                            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].height, /* 高さ               */
                            TRUE);
            }
        }
    }
    else
    {
        nop();
    }
}
