#include "common.h"

/* 外部関数定義 */
#include "MainWnd.h"

/* 外部変数定義 */
/* 内部関数定義 */
static HWND initializeApp( HINSTANCE hInst, LPSTR szCmdLine, int nCmdShow, HACCEL *hAccelPtr );

/* 内部変数定義 */
static HINSTANCE hInstance;    /* インスタンスのハンドラ     */
static TCHAR szAppName[] = TEXT("DX7/100Editor"); /* アプリケーションの名称 */

/********************************************************************************
 * 内容  : WINDOWSプログラムのエントリポイント
 * 引数  : HINSTANCE hInstance     インスタンスのハンドラ
 * 引数  : HINSTANCE hPrevInstance このプログラムが直前に実行されていたインスタンスのハンドラ
 * 引数  : LPSTR szCmdLine         プログラムの起動のために使われたコマンド行
 * 引数  : int iCmdShow            プログラムの初期表示の形態の指定
 * 戻り値: int
 ***************************************/
int WINAPI
WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow )
{
    MSG msg;
    HACCEL hAccel;
    HWND hWnd;

    hWnd = initializeApp( hInstance, szCmdLine, nCmdShow, &hAccel );
    if( hWnd == NULL )
    {
        MessageBox(NULL, "couldn't start!", NULL, MB_OK);
        return 0;
    }

    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        if( !IsMainWndMessage( &msg ) )
        {
            if( !TranslateAccelerator(hWnd,hAccel,&msg) )
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
            else
            {
                nop();
            }
        }
    }

    return (int)msg.wParam; /* PostQuitMessage()の引数 */
}


/********************************************************************************
 * 内容   : アプリケーションの初期化
 * 引数   : HINSTANCE hInst
 * 引数   : LPSTR szCmdLine
 * 引数   : int nCmdShow
 * 引数   : HACCEL *hAccelPtr
 * 戻り値 : HWND
 ***************************************/
static HWND
initializeApp( HINSTANCE hInst, LPSTR szCmdLine, int nCmdShow, HACCEL *hAccelPtr )
{
    hInstance = hInst;
    return MainWndCreate( hInst, szAppName, szCmdLine, nCmdShow, hAccelPtr ); /* メインウィンドウ生成 */
}
