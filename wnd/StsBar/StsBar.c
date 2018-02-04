/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */

/* 外部関数定義 */

/* 外部変数定義 */

/* 内部関数定義 */
#include "StsBar.h"

/* 内部変数定義 */
static HWND hwndSbar;
static int sbarColWidth[STS_BAR_MAX+1];
static BOOL fSbarEnable;

/********************************************************************************
 * 内容  : ステータスバー生成
 * 引数  : HINSTANCE hInst
 * 引数  : PTSTR szAppName
 * 引数  : HWND hwnd 親ウィンドウのハンドラ
 * 引数  : BOOL fShow デフォルト表示するか否か
 * 戻り値: HWND
 ***************************************/
HWND
StsBarCreate( HINSTANCE hInst, PTSTR szAppName, HWND hwnd, BOOL fShow )
{
    hwndSbar = CreateWindowEx(0,
                              STATUSCLASSNAME, NULL,
                              WS_CHILD | SBS_SIZEGRIP | WS_CLIPSIBLINGS | SBT_NOBORDERS,
                              0, 0, 0, 0,
                              hwnd, (HMENU)1500, hInst, NULL);
    if( hwndSbar != NULL )
    {
        if( fShow )
        {
            StsBarShowWindow( TRUE );
        }
        else
        {
            StsBarShowWindow( FALSE );
        }
    }
    else
    {
        nop();
    }

    return hwndSbar;
}

/********************************************************************************
 * 内容  : ステータスバーのサイズ調整
 * 引数  : int cxClient
 * 引数  : int cyClient
 * 戻り値: LONG 縦方向のサイズ
 ***************************************/
LONG
StsBarSize( int cxClient,int cyClient )
{
    RECT RectSbar;
    LONG vertSize = 0;

    sbarColWidth[STS_BAR_0]   = (cxClient/4);
    sbarColWidth[STS_BAR_1]   = cxClient - (cxClient/4);
    sbarColWidth[STS_BAR_MAX] = -1;

    SendMessage(hwndSbar, SB_SETPARTS, sizeof(sbarColWidth)/sizeof(int), (LPARAM)sbarColWidth);
    SendMessage(hwndSbar, WM_SIZE, SIZE_RESTORED, MAKELPARAM(cxClient, cyClient));
    GetClientRect(hwndSbar, &RectSbar);

    if( fSbarEnable )
    {
        vertSize = RectSbar.bottom - RectSbar.top;
    }
    else
    {
        nop();
    }

    return vertSize;
}

/********************************************************************************
 * 内容  : ステータスバーへの文字列セット
 * 引数  : STS_BAR_ID id
 * 引数  : PTSTR ptstrFormat, ...
 * 戻り値: なし
 ***************************************/
void
StsBarSetText( STS_BAR_ID id, PTSTR ptstrFormat, ... )
{
    TCHAR szBuf[50];
    RECT  RectSbar;
    va_list vaArgs;

    va_start(vaArgs, ptstrFormat);
    if( wvsprintf(szBuf, ptstrFormat, vaArgs) != EOF )
    {
        SendMessage(hwndSbar, SB_SETTEXT, id, (LPARAM)szBuf);
        GetClientRect(hwndSbar, &RectSbar);
    }
    else
    {
        /* do nothing */
    }
    va_end(vaArgs);
}

/********************************************************************************
 * 内容  : ステータスバーの表示
 * 引数  : BOOL fShow
 * 戻り値: BOOL
 ***************************************/
BOOL
StsBarShowWindow( BOOL fShow )
{
    BOOL rtn = FALSE;

    if( hwndSbar != NULL )
    {
        if( fShow )
        {
            fSbarEnable = TRUE;
            rtn = ShowWindow(hwndSbar, SW_SHOW);
        }
        else
        {
            fSbarEnable = FALSE;
            rtn = ShowWindow(hwndSbar, SW_HIDE);
        }
    }
    else
    {
        nop();
    }

    return rtn;
}
