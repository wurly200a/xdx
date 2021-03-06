/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */

/* 外部関数定義 */

/* 外部変数定義 */

/* 内部関数定義 */
#include "SomeCtrl.h"
/* 内部変数定義 */

typedef struct
{
    SOME_CTRL_GROUP_ID groupId;
    PTSTR              class;       /* ウィンドウクラス名                         */
    PTSTR              value;       /* ウィンドウテキスト                         */
    int                x;           /* クライアント領域の x座標                   */
    int                y;           /* クライアント領域の y座標                   */
    int                width;       /* 初期幅                                     */
    int                height;      /* 初期高さ                                   */
    BOOL               bSizeAdj;    /* サイズ調整を行うか                         */
    int                widthOfst;   /* クライアント領域の幅サイズからのオフセット */
    int                heightOfst;  /* 1文字分の高さからのオフセット              */
    DWORD              exStyle;     /* 拡張スタイル                               */
    DWORD              style;       /* スタイル                                   */
} S_SOME_CTRL;

static S_SOME_CTRL ctrlListTbl[SOME_CTRL_MAX] =
{
    /*                        , class            , value              , x , y  , w  , h ,bSAdj  ,wOfst, hOfst,exStyle,style  */
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("static")   , TEXT("DEVICE")     ,600, 25,  50 , 15 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("combobox") , TEXT("")           ,650, 20, 150 ,300 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|CBS_SORT|CBS_DROPDOWNLIST)    },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("button")   , TEXT("MIDI")       ,220,  5, 350 ,100 ,FALSE ,0    , 0    ,WS_EX_TRANSPARENT,(WS_CHILD|BS_GROUPBOX)                  },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("static")   , TEXT("MIDI IN")    ,230, 25,  80 , 15 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("combobox") , TEXT("")           ,295, 20, 200 , 300,FALSE ,0    , 0    ,0                ,(WS_CHILD|CBS_SORT|CBS_DROPDOWNLIST)    },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("button")   , TEXT("Open")       ,510, 20,  40 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("static")   , TEXT("MIDI OUT")   ,230, 45,  80 , 15 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("combobox") , TEXT("")           ,295, 40, 200 , 300,FALSE ,0    , 0    ,0                ,(WS_CHILD|CBS_SORT|CBS_DROPDOWNLIST)    },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("button")   , TEXT("Open")       ,510, 40,  40 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_DISABLE  , TEXT("static")   , TEXT("MIDI KEY IN"),365, 55,  80 , 15 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_DISABLE  , TEXT("combobox") , TEXT("")           ,430, 50, 200 , 300,FALSE ,0    , 0    ,0                ,(WS_CHILD|CBS_SORT|CBS_DROPDOWNLIST)    },
    {SOME_CTRL_GROUP_DISABLE  , TEXT("button")   , TEXT("Open")       ,645, 50,  40 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_DISABLE  , TEXT("button")   , TEXT("DEBUG")      ,720, 30,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },

    {SOME_CTRL_GROUP_ALWAYS   , TEXT("button")   , TEXT("MODE")       , 10,  5, 200 , 45 ,FALSE ,0    , 0    ,WS_EX_TRANSPARENT,(WS_CHILD|BS_GROUPBOX)                  },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("button")   , TEXT("1VOICE")     , 20, 20,  70 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("button")   , TEXT("ALL VOICE")  ,120, 20,  70 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_DISABLE  , TEXT("button")   , TEXT("SYSTEM")     ,240, 20,  70 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },

    {SOME_CTRL_GROUP_1VOICE   , TEXT("button")   , TEXT("FILE")       , 10, 60, 175 , 45 ,FALSE ,0    , 0    ,WS_EX_TRANSPARENT,(WS_CHILD|BS_GROUPBOX)                  },
    {SOME_CTRL_GROUP_1VOICE   , TEXT("button")   , TEXT("LOAD")       , 20, 75,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_1VOICE   , TEXT("button")   , TEXT("SAVE")       ,100, 75,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_1VOICE   , TEXT("button")   , TEXT("GET")        ,325, 75,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_1VOICE   , TEXT("button")   , TEXT("SET")        ,405, 75,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },

    {SOME_CTRL_GROUP_ALL_VOICE, TEXT("button")   , TEXT("FILE")       , 10, 60, 175 , 45 ,FALSE ,0    , 0    ,WS_EX_TRANSPARENT,(WS_CHILD|BS_GROUPBOX)                  },
    {SOME_CTRL_GROUP_ALL_VOICE, TEXT("button")   , TEXT("LOAD")       , 20, 75,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_ALL_VOICE, TEXT("button")   , TEXT("SAVE")       ,100, 75,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_ALL_VOICE, TEXT("button")   , TEXT("GET")        ,325, 75,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_ALL_VOICE, TEXT("button")   , TEXT("SET")        ,405, 75,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },

    {SOME_CTRL_GROUP_DISABLE  , TEXT("button")   , TEXT("ACTION")     , 10, 60, 350 , 45 ,FALSE ,0    , 0    ,WS_EX_TRANSPARENT,(WS_CHILD|BS_GROUPBOX)                  },
    {SOME_CTRL_GROUP_DISABLE  , TEXT("button")   , TEXT("GET")        , 50, 75,  40 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_DISABLE  , TEXT("button")   , TEXT("SET")        ,150, 75,  40 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
};

typedef struct
{
    BOOL               exist                        ; /* ウィンドウが生成されているかどうか */
    HWND               hwnd                         ;
} S_SOME_CTRL_INFO;
static S_SOME_CTRL_INFO ctrlInfo[SOME_CTRL_MAX];

static SOME_CTRL_GROUP_ID nowGroupId;

/********************************************************************************
 * 内容  : 登録された全てのコントロールを生成する
 * 引数  : HINSTANCE hInst
 * 引数  : PTSTR szAppName
 * 引数  : HWND hwnd 親ウィンドウのハンドラ
 * 戻り値: BOOL
 ***************************************/
BOOL
SomeCtrlCreate( HINSTANCE hInst, PTSTR szAppName, HWND hwnd )
{
    int i,j,iCbNum;
    TCHAR szBuff[128];

    for(i=0; i<SOME_CTRL_MAX; i++)
    {
        S_SOME_CTRL *tblPtr = &(ctrlListTbl[i]);
        S_SOME_CTRL_INFO *infoPtr = &(ctrlInfo[i]);

        if( tblPtr->groupId != SOME_CTRL_GROUP_DISABLE )
        {
            infoPtr->hwnd = CreateWindowEx( tblPtr->exStyle,                  /* 拡張スタイル       */
                                            tblPtr->class,                    /* クラス名           */
                                            tblPtr->value,                    /* ウィンドウテキスト */
                                            tblPtr->style,                    /* スタイル           */
                                            tblPtr->x,tblPtr->y,       /* x座標,y座標        */
                                            tblPtr->width,                    /* 幅                 */
                                            tblPtr->height,                   /* 高さ               */
                                            hwnd,(HMENU)(SOME_CTRL_ID_OFFSET+i),     /* 親ウィンドウ,子ウィンドウID */
                                            hInst,NULL );                       /* インスタンスハンドル,補助引数 */
            if( infoPtr->hwnd != NULL )
            {
                SendMessage(infoPtr->hwnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));

                /* 個別に処理するもの */
                switch( i )
                {
                default:
                    break;
                }

                if( tblPtr->groupId == SOME_CTRL_GROUP_ALWAYS )
                {
                    ShowWindow(infoPtr->hwnd, SW_SHOW);
                }
                else
                {
                    nop();
                }
                infoPtr->exist = TRUE;
            }
            else
            {
                /* do nothing */
            }
        }
        else
        {
            nop();
        }
    }

    return TRUE;
}

/********************************************************************************
 * 内容  : 指定のグループのコントロールを表示する
 * 引数  : SOME_CTRL_GROUP_ID groupId
 * 戻り値: BOOL
 ***************************************/
BOOL
SomeCtrlGroupDisplay( SOME_CTRL_GROUP_ID groupId )
{
    SOME_CTRL_ID id;

    if( groupId < SOME_CTRL_GROUP_MAX )
    {
        for(id=0; id<SOME_CTRL_MAX; id++)
        {
            S_SOME_CTRL *tblPtr = &(ctrlListTbl[id]);
            S_SOME_CTRL_INFO *infoPtr = &(ctrlInfo[id]);

            if( (tblPtr->groupId == SOME_CTRL_GROUP_ALWAYS) ||
                (tblPtr->groupId == groupId) )
            {
                ShowWindow(infoPtr->hwnd, SW_SHOW);
            }
            else
            {
                ShowWindow(infoPtr->hwnd, SW_HIDE);
            }
        }

        switch( groupId )
        {
        case SOME_CTRL_GROUP_1VOICE   :
            SomeCtrlDisable( SOME_CTRL_MODE_1VOICE    );
            SomeCtrlEnable ( SOME_CTRL_MODE_ALL_VOICE );
            break;
        case SOME_CTRL_GROUP_ALL_VOICE:
            SomeCtrlEnable ( SOME_CTRL_MODE_1VOICE    );
            SomeCtrlDisable( SOME_CTRL_MODE_ALL_VOICE );
            break;
        default:
            nop();
            break;
        }

        nowGroupId = groupId;
    }
    else
    {
        nop();
    }

    return TRUE;
}

/********************************************************************************
 * 内容  : グループIDの取得
 * 引数  : なし
 * 戻り値: SOME_CTRL_GROUP_ID
 ***************************************/
SOME_CTRL_GROUP_ID
SomeCtrlGetNowGroupId( void )
{
    return nowGroupId;
}

/********************************************************************************
 * 内容  : 登録された全てのコントロールのサイズを調整する
 * 引数  : int cxClient
 * 引数  : int cyClient
 * 戻り値: LONG コントロールを配置する矩形の縦方向のサイズ
 ***************************************/
LONG
SomeCtrlSize( int cxClient,int cyClient )
{
    int i;

    for(i=0; i<SOME_CTRL_MAX; i++)
    {
        S_SOME_CTRL *tblPtr = &(ctrlListTbl[i]);
        S_SOME_CTRL_INFO *infoPtr = &(ctrlInfo[i]);

        if( tblPtr->bSizeAdj )
        {
            MoveWindow( infoPtr->hwnd, tblPtr->x,tblPtr->y, (cxClient + tblPtr->widthOfst),cyClient + tblPtr->heightOfst, TRUE);
        }
        else
        {
            /* do nothing */
        }
    }

    return (LONG)700;
}

/********************************************************************************
 * 内容  : コントロールのウィンドウハンドラを取得する
 * 引数  : SOME_CTRL_ID id
 * 戻り値: HWND
 ***************************************/
HWND
SomeCtrlGetHWND( SOME_CTRL_ID id )
{
    HWND rtn;

    if( ctrlInfo[id].exist == TRUE )
    {
        rtn = ctrlInfo[id].hwnd;
    }
    else
    {
        rtn = (HWND)0;
    }

    return rtn;
}

/********************************************************************************
 * 内容  : コントロールを有効にする
 * 引数  : SOME_CTRL_ID id
 * 戻り値: なし
 ***************************************/
void
SomeCtrlEnable( SOME_CTRL_ID id )
{
    EnableWindow( ctrlInfo[id].hwnd, TRUE);
}

/********************************************************************************
 * 内容  : コントロールを無効にする
 * 引数  : SOME_CTRL_ID id
 * 戻り値: なし
 ***************************************/
void
SomeCtrlDisable( SOME_CTRL_ID id )
{
    EnableWindow( ctrlInfo[id].hwnd, FALSE);
}

/********************************************************************************
 * 内容  : ウィンドウからテキストを取得する
 * 引数  : SOME_CTRL_ID id
 * 引数  : PTSTR ptstrText
 * 戻り値: BOOL
 ***************************************/
BOOL
SomeCtrlGetText( SOME_CTRL_ID id, PTSTR ptstrText )
{
    BOOL rtn = FALSE;
    INT iLength;

    if( ctrlInfo[id].exist == TRUE )
    {
        iLength = GetWindowTextLength( ctrlInfo[id].hwnd );
        GetWindowText( ctrlInfo[id].hwnd, ptstrText, iLength + 1 );
        rtn = TRUE;
    }
    else
    {
        /* error */
    }

    return rtn;
}
