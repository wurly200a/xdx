#ifndef EDITWND_H

/* [メッセージ]                                                                 */
/* WM_CUT                        ○                                             */
/* WM_COPY                       ○                                             */
/* WM_PASTE                      ○                                             */
/* WM_CLEAR                      ○                                             */
/* WM_SETFONT                    ○                                             */
/* EM_GETSEL                     ×                                             */
/* EM_SETSEL                     △(wParam==0,lParam=-1の時のみ)                */
/* EM_GETRECT                    ×                                             */
/* EM_SETRECT                    ×                                             */
/* EM_SETRECTNP                  ×                                             */
/* EM_SCROLL                     ×                                             */
/* EM_LINESCROLL                 ×                                             */
/* EM_SCROLLCARET                ×                                             */
/* EM_GETMODIFY                  ×                                             */
/* EM_SETMODIFY                  ×                                             */
/* EM_GETLINECOUNT               ○                                             */
/* EM_LINEINDEX                  ×                                             */
/* EM_SETHANDLE                  ×                                             */
/* EM_GETHANDLE                  ×                                             */
/* EM_GETTHUMB                   ×                                             */
/* EM_LINELENGTH                 ×                                             */
/* EM_REPLACESEL                 ×                                             */
/* EM_GETLINE                    ×                                             */
/* EM_CANUNDO                    ×                                             */
/* EM_UNDO                       ×                                             */
/* EM_FMTLINES                   ×                                             */
/* EM_LINEFROMCHAR               △(wParam==-1の時のみ)                         */
/* EM_SETTABSTOPS                ×                                             */
/* EM_SETPASSWORDCHAR            ×                                             */
/* EM_EMPTYUNDOBUFFER            ×                                             */
/* EM_GETFIRSTVISIBLELINE        ×                                             */
/* EM_SETREADONLY                ×                                             */
/* EM_SETWORDBREAKPROC           ×                                             */
/* EM_GETWORDBREAKPROC           ×                                             */
/* EM_GETPASSWORDCHAR            ×                                             */
/* EM_SETMARGINS                 ×                                             */
/* EM_GETMARGINS                 ×                                             */
/* EM_SETLIMITTEXT (EM_LIMITTEXT)×                                             */
/* EM_GETLIMITTEXT               ×                                             */
/* EM_POSFROMCHAR                ×                                             */
/* EM_CHARFROMPOS                ×                                             */
/* EM_SETIMESTATUS               ×                                             */
/* EM_GETIMESTATUS               ×                                             */
/*                                                                              */
/* [関数]                                                                       */
/* GetWindowTextLength(HWND hWnd)                           ×                  */
/* GetWindowText(HWND hWnd,LPTSTR lpString,int nMaxCount)   ×                  */
/* SetWindowText(HWND hWnd,LPCTSTR lpString)                ×                  */
/*                                                                              */

/********************************************************************************
 * 内容  : EDITウィンドウクラスの登録
 * 引数  : HINSTANCE hInst
 * 戻り値: BOOL
 ***************************************/
BOOL EditWndRegisterClass( HINSTANCE hInst );

/********************************************************************************
 * 内容  : EDITウィンドウの初期化
 * 引数  : HWND hwnd
 * 戻り値: なし
 ***************************************/
void EditWndDataInit( HWND hwnd );

/********************************************************************************
 * 内容  : EDITウィンドウのデータセット
 * 引数  : HWND hwnd
 * 引数  : TCHAR* dataPtr
 * 引数  : DWORD  length
 * 引数  : BOOL   bInit (TRUE:既存データをクリア,FALSE:クリアしない)
 * 戻り値: なし
 ***************************************/
void EditWndDataSet( HWND hwnd, TCHAR* dataPtr, DWORD length, BOOL bInit );

typedef enum
{
    EDITWND_ALL,
    EDITWND_SELECTED
} EDITWND_REGION;

/********************************************************************************
 * 内容  : EDITウィンドウのデータ取得
 * 引数  : HWND hwnd
 * 引数  : TCHAR *dataPtr
 * 引数  : DWORD dataSize
 * 引数  : EDITWND_REGION region
 * 戻り値: BOOL
 ***************************************/
BOOL EditWndDataGet( HWND hwnd, TCHAR *dataPtr, DWORD dataSize, EDITWND_REGION region );

/********************************************************************************
 * 内容  : EDITウィンドウのデータサイズ取得
 * 引数  : HWND hwnd
 * 引数  : EDITWND_REGION region
 * 戻り値: DWORD
 ***************************************/
DWORD EditWndGetDataSize( HWND hwnd, EDITWND_REGION region );

/********************************************************************************
 * 内容  : EDITウィンドウバッファの最大文字サイズ取得
 * 引数  : HWND hwnd
 * 戻り値: DWORD
 ***************************************/
DWORD EditWndGetColumnMaxSize( HWND hwnd );

/********************************************************************************
 * 内容  : EDITウィンドウバッファのキャレットX位置取得
 * 引数  : HWND hwnd
 * 戻り値: DWORD
 ***************************************/
DWORD EditWndGetCaretXpos( HWND hwnd );

enum
{
    NEWLINECODE_CRLF = 0,
    NEWLINECODE_LF   ,
    NEWLINECODE_CR   ,
    NEWLINECODE_NONE ,
};
typedef UINT NEWLINECODE_TYPE;
/********************************************************************************
 * 内容  : EDITウィンドウの改行コードセット
 * 引数  : HWND hwnd
 * 引数  : NEWLINECODE_TYPE newLineCodeType
 * 戻り値: BOOL (TRUE:表示書き換え必要)
 ***************************************/
BOOL EditWndNewLineCodeSet( HWND hwnd, NEWLINECODE_TYPE newLineCodeType );

/********************************************************************************
 * 内容  : EDITウィンドウの検索文字列データセット
 * 引数  : HWND hwnd
 * 引数  : TCHAR* dataPtr
 * 引数  : DWORD  length
 * 引数  : BOOL bDirectionUp
 * 戻り値: BOOL
 ***************************************/
BOOL EditWndFindDataSet( HWND hwnd, TCHAR* dataPtr, DWORD length, BOOL bDirectionUp );

#define EDITWND_H
#endif /* EDITWND_H */
