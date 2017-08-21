/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */

/* 外部関数定義 */
#include "DebugWnd.h"

/* 外部変数定義 */

/* 内部関数定義 */
void setAllScrollInfo( HWND hwnd );
void getAllScrollInfo( HWND hwnd );
void setScrollPos( HWND hwnd, int fnBar, DWORD nPos );

/* 内部変数定義 */
typedef struct
{
    int    cxClient;  /* クライアント領域サイズ(X) */
    int    cyClient;  /* クライアント領域サイズ(Y) */
    int    iHorzPos;  /* スクロールバーの横位置    */
    int    iVertPos;  /* スクロールバーの縦位置    */
} S_MAINWND_SCROLL_DATA;

static S_MAINWND_SCROLL_DATA mainWndScrollData;

#define SCROLL_RANGE_HORZ_MAX 1100
#define SCROLL_RANGE_VERT_MAX  810

/********************************************************************************
 * 内容  : スクロール情報のセット
 * 引数  : HWND hwnd
 * 戻り値: なし
 ***************************************/
void
InitMainWndScrollInfo( HWND hwnd )
{

}

/********************************************************************************
 * 内容  : スクロール
 * 引数  : HWND hwnd
 * 戻り値: なし
 ***************************************/
void
MainWndHscroll( HWND hwnd, WPARAM wParam )
{
    SCROLLINFO si;
    int iHorzPos;
    S_MAINWND_SCROLL_DATA *ptr = &mainWndScrollData;

    /* Get all the vertial scroll bar information */
    si.cbSize = sizeof(si);
    si.fMask  = SIF_ALL;

    /* Save the position for comparison later on */
    GetScrollInfo( hwnd, SB_HORZ, &si );
    iHorzPos = si.nPos;

    switch(LOWORD(wParam))
    {
    case SB_LINELEFT:
        si.nPos -= 1;
        break;
    case SB_LINERIGHT:
        si.nPos += 1;
        break;
    case SB_PAGELEFT:
        si.nPos -= si.nPage;
        break;
    case SB_PAGERIGHT:
        si.nPos += si.nPage;
        break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        si.nPos = si.nTrackPos;
        /*si.nPos = HIWORD(wParam);でもよい*/
        break;
    default :
        break;
    }

    /* Set the position and then retrieve it.  Due to adjustments */
    /* by Windows it may not be the same as the value set. */
    si.fMask = SIF_POS;
    SetScrollInfo( hwnd, SB_HORZ, &si, TRUE );
    GetScrollInfo( hwnd, SB_HORZ, &si );

    /* If the position has changed, scroll the window */
    if( si.nPos != iHorzPos )
    {
        ScrollWindow( hwnd, iHorzPos - si.nPos, 0, NULL, NULL );
        UpdateWindow(hwnd);
    }

    GetScrollInfo( hwnd, SB_HORZ, &si );
    ptr->iHorzPos = si.nPos;
}

/********************************************************************************
 * 内容  : スクロール
 * 引数  : HWND hwnd
 * 戻り値: なし
 ***************************************/
void
MainWndVscroll( HWND hwnd, WPARAM wParam )
{
    SCROLLINFO si;
    int iVertPos;
    S_MAINWND_SCROLL_DATA *ptr = &mainWndScrollData;

    si.cbSize = sizeof(si);
    si.fMask  = SIF_ALL;
    GetScrollInfo( hwnd, SB_VERT, &si ); /* スクロールバー情報取得 */
    iVertPos = si.nPos;                  /* 現在の位置               */

    switch( LOWORD(wParam) )
    {
    case SB_TOP:
        si.nPos = si.nMin;
        break;
    case SB_BOTTOM:
        si.nPos = si.nMax;
        break;
    case SB_LINEUP:
        si.nPos -= 1;
        break;
    case SB_LINEDOWN:
        si.nPos += 1;
        break;
    case SB_PAGEUP:
        si.nPos -= si.nPage;
        break;
    case SB_PAGEDOWN:
        si.nPos += si.nPage;
        break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        si.nPos = si.nTrackPos;
        /*si.nPos = HIWORD(wParam);でもよい*/
    default:
        break;
    }
    si.fMask = SIF_POS;
    SetScrollInfo(hwnd, SB_VERT, &si, TRUE); /* スクロールバー情報セット＆再描画 */
    GetScrollInfo(hwnd, SB_VERT, &si);       /* スクロールバー情報再取得         */

    if( si.nPos != iVertPos )
    {
        ScrollWindow(hwnd, 0, iVertPos - si.nPos,NULL, NULL);
        UpdateWindow(hwnd);
    }

    GetScrollInfo(hwnd, SB_VERT, &si);
    ptr->iVertPos = si.nPos;
}

/********************************************************************************
 * 内容  : スクロール情報のセット
 * 引数  : HWND hwnd
 * 引数  : cxClient
 * 引数  : cyClient
 * 戻り値: なし
 ***************************************/
void
SetMainWndAllScrollInfo( HWND hwnd, int cxClient, int cyClient )
{
    SCROLLINFO si;
    S_MAINWND_SCROLL_DATA *ptr = &mainWndScrollData;

    DebugWndPrintf("SetMainWndAllScrollInfo,%d,%d \r\n",cxClient,cyClient);

    ptr->cxClient = cxClient;
    ptr->cyClient = cyClient;

    /* Set vertical scroll bar range and page size */
    si.cbSize = sizeof(si);
    si.fMask  = SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
    si.nMin   = 0;                                                                /* 範囲の最小値 */
    si.nMax   = SCROLL_RANGE_VERT_MAX;
    si.nPage  = (ptr->cyClient ); /* ページサイズ */
    SetScrollInfo( hwnd, SB_VERT, &si, TRUE );

    /* Set horizontal scroll bar range and page size*/
    si.cbSize = sizeof(si);
    si.fMask  = SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
    si.nMin   = 0;
    si.nMax   = SCROLL_RANGE_HORZ_MAX;
    si.nPage  = (ptr->cxClient );
    SetScrollInfo( hwnd, SB_HORZ, &si, TRUE );

}

/********************************************************************************
 * 内容  : スクロール情報の取得
 * 引数  : HWND hwnd
 * 引数  : *iHorzPosPtr
 * 引数  : *iVertPosPtr
 * 戻り値: なし
 ***************************************/
void
GetMainWndAllScrollInfo( HWND hwnd, int *iHorzPosPtr, int *iVertPosPtr )
{
    SCROLLINFO si;
    S_MAINWND_SCROLL_DATA *ptr = &mainWndScrollData;

    si.cbSize = sizeof(si);
    si.fMask  = SIF_POS;
    GetScrollInfo( hwnd, SB_VERT, &si ); /* 縦スクロールバーの位置を取得 */
    ptr->iVertPos = si.nPos;

    GetScrollInfo( hwnd, SB_HORZ, &si);   /* 横スクロールバーの位置を取得 */
    ptr->iHorzPos = si.nPos;

    *iVertPosPtr = ptr->iVertPos;
    *iHorzPosPtr = ptr->iHorzPos;
}

/********************************************************************************
 * 内容  : スクロール位置のセット
 * 引数  : HWND hwnd
 * 引数  : int   fnBar スクロールバーのタイプ
 * 引数  : DWORD nPos  位置
 * 戻り値: なし
 ***************************************/
void
SetMainWndScrollPos( HWND hwnd, int fnBar, DWORD nPos )
{
    SCROLLINFO si;

    si.cbSize = sizeof(si);
    si.fMask  = SIF_POS;
    GetScrollInfo( hwnd, fnBar, &si );
    si.nPos   = nPos;
    SetScrollInfo( hwnd, fnBar, &si, TRUE );
}
