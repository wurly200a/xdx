#ifndef MAINWND_SCROLL_H

/********************************************************************************
 * 内容  : スクロール情報のセット
 * 引数  : HWND hwnd
 * 戻り値: なし
 ***************************************/
void InitMainWndScrollInfo( HWND hwnd );

/********************************************************************************
 * 内容  : スクロール
 * 引数  : HWND hwnd
 * 戻り値: なし
 ***************************************/
void MainWndHscroll( HWND hwnd, WPARAM wParam );

/********************************************************************************
 * 内容  : スクロール
 * 引数  : HWND hwnd
 * 戻り値: なし
 ***************************************/
void MainWndVscroll( HWND hwnd, WPARAM wParam );

/********************************************************************************
 * 内容  : スクロール情報のセット
 * 引数  : HWND hwnd
 * 引数  : cxClient
 * 引数  : cyClient
 * 戻り値: なし
 ***************************************/
void SetMainWndAllScrollInfo( HWND hwnd, int cxClient, int cyClient );

/********************************************************************************
 * 内容  : スクロール情報の取得
 * 引数  : HWND hwnd
 * 引数  : *iHorzPosPtr
 * 引数  : *iVertPosPtr
 * 戻り値: なし
 ***************************************/
void GetMainWndAllScrollInfo( HWND hwnd, int *iHorzPosPtr, int *iVertPosPtr );

/********************************************************************************
 * 内容  : スクロール位置のセット
 * 引数  : HWND hwnd
 * 引数  : int   fnBar スクロールバーのタイプ
 * 引数  : DWORD nPos  位置
 * 戻り値: なし
 ***************************************/
void SetMainWndScrollPos( HWND hwnd, int fnBar, DWORD nPos );

#define MAINWND_SCROLL_H
#endif /* MAINWND_SCROLL_H */
