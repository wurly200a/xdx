#ifndef STS_BAR_H

typedef enum
{
    STS_BAR_0,
    STS_BAR_1,
    STS_BAR_MAX
}STS_BAR_ID;

/********************************************************************************
 * 内容  : ステータスバー生成
 * 引数  : HINSTANCE hInst
 * 引数  : PTSTR szAppName
 * 引数  : HWND hwnd 親ウィンドウのハンドラ
 * 引数  : BOOL fShow デフォルト表示するか否か
 * 戻り値: HWND
 ***************************************/
HWND StsBarCreate( HINSTANCE hInst, PTSTR szAppName, HWND hwnd, BOOL fShow );

/********************************************************************************
 * 内容  : ステータスバーのサイズ調整
 * 引数  : int cxClient
 * 引数  : int cyClient
 * 戻り値: LONG 縦方向のサイズ
 ***************************************/
LONG StsBarSize( int cxClient,int cyClient );

/********************************************************************************
 * 内容  : ステータスバーへの文字列セット
 * 引数  : STS_BAR_ID id
 * 引数  : PTSTR ptstrFormat, ...
 * 戻り値: なし
 ***************************************/
void StsBarSetText( STS_BAR_ID id, PTSTR ptstrFormat, ... );

/********************************************************************************
 * 内容  : ステータスバーの表示
 * 引数  : BOOL fShow
 * 戻り値: BOOL
 ***************************************/
BOOL StsBarShowWindow( BOOL fShow );

#define STS_BAR_H
#endif /* STS_BAR_H */
