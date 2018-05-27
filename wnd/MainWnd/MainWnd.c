/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */
#include "MainWndDef.h"
#include "MenuId.h"
#include "MainWndMenu.h"

/* 外部関数定義 */
#include "SomeCtrl.h"
#include "File.h"
#include "StsBar.h"
#include "Font.h"
#include "Config.h"
#include "DateTime.h"
#include "ModalDlg.h"
#include "MainWndScroll.h"

#include "DebugWnd.h"
#include "Midi.h"
#include "dx100Ctrl.h"
#include "dx7Ctrl.h"

/* 外部変数定義 */

/* 内部関数定義 */
#include "MainWnd.h"
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static MAINWND_INDEX convertMSGtoINDEX( UINT message );
static LRESULT onCreate          ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onPaint           ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onSize            ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onMove            ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onWindowPosChanged( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onClose           ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onDestroy         ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onCommand         ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onKeyUp           ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onKeyDown         ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onChar            ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onHscroll         ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onVscroll         ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onMouseWheel      ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onSetFocus        ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onKillFocus       ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onDropFiles       ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onInitMenuPopup   ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onFindMsgString   ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onTimer           ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onApp             ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT onDefault         ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );

int okMessage( HWND hwnd, TCHAR *szMessageFormat, ... );
void doCaption( HWND hwnd, TCHAR* szTitleName, BOOL bNeedSave );
static void someCtrlDisableOnMidiOpenOrClose( void );

/* 内部変数定義 */
static HWND hwndMain; /* メインウィンドウのハンドラ */
static S_MAINWND_DATA mainWndData;
static TCHAR szCmdLineLocal[1024];
static HMIDIIN hMidiIn;
static HMIDIOUT hMidiOut;

/* *INDENT-OFF* */
static LRESULT (*wndProcTbl[MAINWND_MAX])( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ) =
{
    onCreate          , /* WM_CREATE                     */
    onPaint           , /* WM_PAINT                      */
    onSize            , /* WM_SIZE                       */
    onMove            , /* WM_MOVE                       */
    onWindowPosChanged, /* WM_WINDOWPOSCHANGED           */
    onClose           , /* WM_CLOSE                      */
    onDestroy         , /* WM_DESTROY                    */
    onCommand         , /* WM_COMMAND                    */
    onKeyUp           , /* WM_KEYUP                      */
    onKeyDown         , /* WM_KEYDOWN                    */
    onChar            , /* WM_CHAR                       */
    onHscroll         , /* WM_HSCROLL                    */
    onVscroll         , /* WM_VSCROLL                    */
    onMouseWheel      , /* WM_MOUSEWHEEL                 */
    onSetFocus        , /* WM_SETFOCUS                   */
    onKillFocus       , /* WM_KILLFOCUS                  */
    onDropFiles       , /* WM_DROPFILES                  */
    onInitMenuPopup   , /* WM_INITMENUPOPUP              */
    onFindMsgString   , /* FINDMSGSTRINGの登録メッセージ */
    onTimer           , /* WM_TIMER                      */
    onApp             , /* WM_APP                        */
    onDefault           /* default                       */
};
/* *INDENT-ON* */

#define TIMER_ID      1
#define TIMER_CYCLE  100 /* msec */

/********************************************************************************
 * 内容  : メインウィンドウクラスの登録、ウィンドウの生成
 * 引数  : HINSTANCE hInst
 * 引数  : PTSTR szAppName
 * 引数  : LPSTR szCmdLine
 * 引数  : int nCmdShow
 * 引数  : HACCEL *hAccelPtr
 * 戻り値: HWND
 ***************************************/
HWND
MainWndCreate( HINSTANCE hInst, PTSTR szAppName, LPSTR szCmdLine, int nCmdShow, HACCEL *hAccelPtr )
{
    WNDCLASS wc = {0};
    PTSTR pAppName = szAppName;
    HMENU hMenu = NULL;

    hwndMain = NULL;

    /* メインウィンドウクラス */
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = (WNDPROC) WndProc;
    wc.cbClsExtra       = 0; /* クラス構造体の為の追加領域を予約する */
    wc.cbWndExtra       = 0; /* ウィンドウ構造体の為の追加領域を予約する */
    wc.hInstance        = hInst;
    wc.hIcon            = LoadIcon( hInst, pAppName );          /* アイコン */
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH) (COLOR_BTNFACE + 1); /* 背景 */
    wc.lpszMenuName     = pAppName;
    wc.lpszClassName    = pAppName;

    if( !RegisterClass(&wc) )
    { /* ウィンドウクラスの登録失敗 */
        MessageBox( NULL, TEXT("This program requires Windows 2000!"), pAppName, MB_ICONERROR );
    }
    else
    {
        memset( &mainWndData, 0, sizeof(mainWndData) );
        mainWndData.hInstance = hInst;
        mainWndData.szAppName = szAppName;

        /* 設定の初期化 */
        ConfigInit(mainWndData.hInstance,mainWndData.szAppName);

        /* メニューの生成 */
        hMenu = MenuCreate();

        /* アクセラレータの生成 */
        if( hAccelPtr != NULL )
        {
            *(hAccelPtr) = AccelCreate();
        }
        else
        {
            nop();
        }

        strncpy(szCmdLineLocal,szCmdLine,1024);

        /* メインウィンドウを作成 */
        InitCommonControls(); /* commctrl.hのインクルード、comctl32.libのプロジェクトへの参加が必要 */
        hwndMain = CreateWindowEx( /* WS_EX_OVERLAPPEDWINDOW | */ WS_EX_ACCEPTFILES,
                                   pAppName, pAppName,
                                   WS_OVERLAPPEDWINDOW /*& ~WS_THICKFRAME*/ | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VSCROLL | WS_HSCROLL,
                                   ConfigLoadDword(CONFIG_ID_WINDOW_POS_X) , ConfigLoadDword(CONFIG_ID_WINDOW_POS_Y) ,
                                   ConfigLoadDword(CONFIG_ID_WINDOW_POS_DX), ConfigLoadDword(CONFIG_ID_WINDOW_POS_DY),
                                   NULL, hMenu, hInst, NULL );

        if( hwndMain == NULL )
        { /* ウィンドウの生成失敗 */
            nop();
        }
        else
        {
            ShowWindow( hwndMain, nCmdShow ); /* ->WM_SIZE   */ /* ウインドウを表示 */
            UpdateWindow( hwndMain );         /* ->WM_PAINT  */
        }
    }

    return hwndMain;
}

/********************************************************************************
 * 内容  : メインウィンドウ内で処理するメッセージを判定する
 * 引数  : MSG *msgPtr
 * 戻り値: BOOL
 ***************************************/
BOOL
IsMainWndMessage( MSG *msgPtr )
{
    BOOL bRtn = FALSE;

    if( (mainWndData.hDlgModeless) &&                      /* モードレスダイアログボックス表示中で、 */
        IsDialogMessage(mainWndData.hDlgModeless,msgPtr) ) /* 本メッセージがモードレスダイアログボックスのウィンドウプロシージャで処理された */
    {
        bRtn = TRUE;
    }
    else
    {
        nop();
    }

    return bRtn;
}

/********************************************************************************
 * 内容  : ウィンドウプロシージャ。WINDOWSから受け取ったメッセージの処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
LRESULT CALLBACK
WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
#if 0
    DebugWndPrintf("WndProc,%X,%X,%X\r\n",message,wParam,lParam);
#endif
    return wndProcTbl[convertMSGtoINDEX(message)]( hwnd, message, wParam, lParam );
}

/********************************************************************************
 * 関数名 : ConvertMSGtoINDEX()
 * 引数   : UINT message
 * 戻り値 : WNDPRC_INDEX
 * 内容   : ウィンドウズメッセージから対応するインデックスに変換する
 ***************************************/
static MAINWND_INDEX
convertMSGtoINDEX( UINT message )
{
    MAINWND_INDEX rtn;

    /* *INDENT-OFF* */
    switch( message )
    {
    case WM_CREATE          :rtn = MAINWND_ON_CREATE          ;break;
    case WM_PAINT           :rtn = MAINWND_ON_PAINT           ;break;
    case WM_SIZE            :rtn = MAINWND_ON_SIZE            ;break;
    case WM_MOVE            :rtn = MAINWND_ON_MOVE            ;break;
    case WM_WINDOWPOSCHANGED:rtn = MAINWND_ON_WINDOWPOSCHANGED;break;
    case WM_CLOSE           :rtn = MAINWND_ON_CLOSE           ;break;
    case WM_DESTROY         :rtn = MAINWND_ON_DESTROY         ;break;
    case WM_COMMAND         :rtn = MAINWND_ON_COMMAND         ;break;
    case WM_TIMER           :rtn = MAINWND_ON_TIMER           ;break;
    case WM_APP             :rtn = MAINWND_ON_APP             ;break;
    case WM_KEYUP           :rtn = MAINWND_ON_KEYUP           ;break;
    case WM_KEYDOWN         :rtn = MAINWND_ON_KEYDOWN         ;break;
    case WM_CHAR            :rtn = MAINWND_ON_CHAR            ;break;
    case WM_HSCROLL         :rtn = MAINWND_ON_HSCROLL         ;break;
    case WM_VSCROLL         :rtn = MAINWND_ON_VSCROLL         ;break;
    case WM_MOUSEWHEEL      :rtn = MAINWND_ON_MOUSEWHEEL      ;break;
    case WM_SETFOCUS        :rtn = MAINWND_ON_SETFOCUS        ;break;
    case WM_KILLFOCUS       :rtn = MAINWND_ON_KILLFOCUS       ;break;
    case WM_DROPFILES       :rtn = MAINWND_ON_DROPFILES       ;break;
    case WM_INITMENUPOPUP   :rtn = MAINWND_ON_INITMENUPOPUP   ;break;
    default                 :
        if( message == mainWndData.messageFindReplace )
        {
            rtn = MAINWND_ON_FINDMSGSTRING;
        }
        else
        {
            rtn = MAINWND_ON_DEFAULT;
        }
        break;
    }
    /* *INDENT-ON* */

    return rtn;
}

/********************************************************************************
 * 内容  : WM_CREATE の処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onCreate( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    HDC        hdc;
    TEXTMETRIC tm;
    HFONT hFont,hOldFont;

    hdc = GetDC( hwnd );
    hFont = GetStockObject(DEFAULT_GUI_FONT);
    hOldFont = SelectObject(hdc, hFont);
    GetTextMetrics( hdc, &tm );
    mainWndData.cxChar = tm.tmAveCharWidth;
    mainWndData.cyChar = tm.tmHeight + (tm.tmHeight/2) + (GetSystemMetrics(SM_CYEDGE) * 2);

    mainWndData.messageFindReplace = RegisterWindowMessage(FINDMSGSTRING);

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
    ReleaseDC( hwnd,hdc );

    if( ConfigLoadDebugValue() == 0xDDDDDDDD )
    {
        mainWndData.hWndDebug = DebugWndCreate(mainWndData.hInstance,mainWndData.szAppName,TRUE);
        DebugWndPrintf("DEBUG=%08lX\r\n",ConfigLoadDebugValue());
    }
    else
    {
        nop();
    }

    ModalDlgInit(mainWndData.hInstance,mainWndData.szAppName);
    FileInitialize( hwnd ); /* ファイル初期化     */
    FileSetDir( FILE_ID_DX7_1VOICE_DATA     , ConfigLoadString(CONFIG_ID_INIT_DIR_DX7_1VOICE_DATA     ) );
    FileSetDir( FILE_ID_DX7_ALL_VOICE_DATA  , ConfigLoadString(CONFIG_ID_INIT_DIR_DX7_ALL_VOICE_DATA  ) );
    FileSetDir( FILE_ID_DX100_1VOICE_DATA   , ConfigLoadString(CONFIG_ID_INIT_DIR_DX100_1VOICE_DATA   ) );
    FileSetDir( FILE_ID_DX100_ALL_VOICE_DATA, ConfigLoadString(CONFIG_ID_INIT_DIR_DX100_ALL_VOICE_DATA) );

    FontInit();

    mainWndData.hFontIo = NULL;

    InitMainWndScrollInfo( hwnd );

    SomeCtrlCreate( mainWndData.hInstance, mainWndData.szAppName, hwnd ); /* コントロールを生成 */
    SomeCtrlGroupDisplay(SOME_CTRL_GROUP_1VOICE);
    someCtrlDisableOnMidiOpenOrClose();

//    StsBarCreate( hwnd, TRUE ); /* ステータスバー生成、デフォルト表示 */

    MenuCheckItem( IDM_VIEW_STS_BAR );
    MenuCheckItem( IDM_EXTEND_NEWLINE_CRLF );

    MidiInit();

    {
        int iCbNum;

        iCbNum = SendMessage( SomeCtrlGetHWND(SOME_CTRL_DEVICE_SELECT), CB_ADDSTRING, 0, (LPARAM)TEXT("6 OPERATOR(DX7)") );
        SendMessage( SomeCtrlGetHWND(SOME_CTRL_DEVICE_SELECT)         , CB_SETITEMDATA, iCbNum, (LPARAM)1 );

        iCbNum = SendMessage( SomeCtrlGetHWND(SOME_CTRL_DEVICE_SELECT), CB_ADDSTRING, 0, (LPARAM)TEXT("4 OPERATOR(DX100)") );
        SendMessage( SomeCtrlGetHWND(SOME_CTRL_DEVICE_SELECT)         , CB_SETITEMDATA, iCbNum, (LPARAM)2 );

        SendMessage( SomeCtrlGetHWND(SOME_CTRL_DEVICE_SELECT)         , CB_SETCURSEL, 1, (LPARAM)0 );
    }

    mainWndData.dxDeviceMode = DX_DEVICE_MODE_DX7;

    Dx100CtrlInit( mainWndData.hInstance, mainWndData.szAppName, hwnd );
    Dx7CtrlInit( mainWndData.hInstance, mainWndData.szAppName, hwnd );

//    Dx100CtrlModeSet(DX100_CTRL_MODE_PATCH);
    Dx7CtrlModeSet(DX7_CTRL_MODE_PATCH);

    return rtn;
}

/********************************************************************************
 * 内容  : WM_PAINT の処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onPaint( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    GetMainWndAllScrollInfo(hwnd,&(mainWndData.iHorzPos),&(mainWndData.iVertPos));

    return DefWindowProc( hwnd, message, wParam, lParam );
}

/********************************************************************************
 * 内容  : WM_SIZE の処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onSize( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LONG topSizeSum=0,bottomSizeSum=0;

    mainWndData.cxClient = LOWORD( lParam );
    mainWndData.cyClient = HIWORD( lParam );

    SetMainWndAllScrollInfo(hwnd,mainWndData.cxClient,mainWndData.cyClient);

    topSizeSum += SomeCtrlSize( mainWndData.cxClient, mainWndData.cyChar ); /* コントロール   */
//    bottomSizeSum += StsBarSize( mainWndData.cxClient, mainWndData.cyChar ); /* ステータスバー */

    GetMainWndAllScrollInfo(hwnd,&(mainWndData.iHorzPos),&(mainWndData.iVertPos));

    return 0;
}

/********************************************************************************
 * 内容  : WM_MOVE の処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onMove( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    mainWndData.xPos = LOWORD( lParam );
    mainWndData.yPos = HIWORD( lParam );

    return 0;
}

/********************************************************************************
 * 内容  : WM_WINDOWPOSCHANGED の処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onWindowPosChanged( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    mainWndData.xWindowPos = ((WINDOWPOS *)lParam)->x ;
    mainWndData.yWindowPos = ((WINDOWPOS *)lParam)->y ;
    mainWndData.cxWindow   = ((WINDOWPOS *)lParam)->cx;
    mainWndData.cyWindow   = ((WINDOWPOS *)lParam)->cy;

    return DefWindowProc( hwnd, message, wParam, lParam );
}

/********************************************************************************
 * 内容  : WM_CLOSE の処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onClose( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    if( 0/*mainWndData.bNeedSave && ((AskAboutSave( hwnd, FileGetTitleName(FILE_ID_BIN))) == IDCANCEL)*/ )
    {
        nop();
    }
    else
    {
        int iCbNum;
        HWND hComboBox;
        BOOL bCancel = (BOOL)FALSE;

        ConfigSaveDword( CONFIG_ID_WINDOW_POS_X , mainWndData.xWindowPos );
        ConfigSaveDword( CONFIG_ID_WINDOW_POS_Y , mainWndData.yWindowPos );
        ConfigSaveDword( CONFIG_ID_WINDOW_POS_DX, mainWndData.cxWindow   );
        ConfigSaveDword( CONFIG_ID_WINDOW_POS_DY, mainWndData.cyWindow   );

        ConfigSaveString( CONFIG_ID_INIT_DIR_DX7_1VOICE_DATA     , FileGetDir(FILE_ID_DX7_1VOICE_DATA     ) );
        ConfigSaveString( CONFIG_ID_INIT_DIR_DX7_ALL_VOICE_DATA  , FileGetDir(FILE_ID_DX7_ALL_VOICE_DATA  ) );
        ConfigSaveString( CONFIG_ID_INIT_DIR_DX100_1VOICE_DATA   , FileGetDir(FILE_ID_DX100_1VOICE_DATA   ) );
        ConfigSaveString( CONFIG_ID_INIT_DIR_DX100_ALL_VOICE_DATA, FileGetDir(FILE_ID_DX100_ALL_VOICE_DATA) );

        hComboBox = SomeCtrlGetHWND(SOME_CTRL_MIDI_IN);
        iCbNum = SendMessage(hComboBox,CB_GETCURSEL,0,0);
        if( iCbNum != CB_ERR )
        {
            ConfigSaveDword(CONFIG_ID_MIDI_IN_NUM,iCbNum);
        }
        else
        {
            nop();
        }

        hComboBox = SomeCtrlGetHWND(SOME_CTRL_MIDI_OUT);
        iCbNum = SendMessage(hComboBox,CB_GETCURSEL,0,0);
        if( iCbNum != CB_ERR )
        {
            ConfigSaveDword(CONFIG_ID_MIDI_OUT_NUM,iCbNum);
        }
        else
        {
            nop();
        }

        hComboBox = SomeCtrlGetHWND(SOME_CTRL_MIDI_KEY_IN);
        iCbNum = SendMessage(hComboBox,CB_GETCURSEL,0,0);
        if( iCbNum != CB_ERR )
        {
            ConfigSaveDword(CONFIG_ID_MIDI_KEY_IN_NUM,iCbNum);
        }
        else
        {
            nop();
        }

        if( ConfigIsSame(0,CONFIG_ID_MAX) )
        {
            nop();
        }
        else
        {
            int iReturn;

//            iReturn = MessageBox( hwnd,TEXT("設定は変更されています。\n\n変更を保存しますか?"),mainWndData.szAppName,MB_YESNOCANCEL|MB_ICONEXCLAMATION );
            iReturn = MessageBox( hwnd,TEXT("Configuration is changed.\n\nSave Changes?"),mainWndData.szAppName,MB_YESNOCANCEL|MB_ICONEXCLAMATION );

            if( (iReturn == IDYES) )
            {
                ConfigWrite(0,CONFIG_ID_MAX);
            }
            else if( iReturn == IDCANCEL )
            {
                bCancel = (BOOL)TRUE;
            }
            else
            {
                nop();
            }
        }

        if( !bCancel )
        {
            DestroyWindow( hwnd );
        }
        else
        {
            nop();
        }
    }

    return 0;
}

/********************************************************************************
 * 内容  : WM_DESTROY の処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onDestroy( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    KillTimer( hwnd, TIMER_ID );
    MidiInClose();
    MidiOutClose();

    DestroyAcceleratorTable( mainWndData.hAccel );

    FileEnd();

    DestroyWindow( mainWndData.hWndDebug );

    PostQuitMessage(0); /* WM_QUITメッセージをポストする */
    return 0;
}

/********************************************************************************
 * 内容  : WM_COMMAND の処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam 通常はLOWORDが通知コード。(コントロールからの通知時はHIWORDが通知コード、LOWORDがコントロールID)
 * 引数  : LPARAM lParam 通常はNULL。(コントロールからの通知時はウィンドウハンドル)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onCommand( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    DWORD dwSize;
    PBYTE dataPtr;
    PTSTR strPtr;
    S_MODAL_DLG_PARAM modalDlgParam;
    static FINDREPLACE fr;
    static TCHAR strFind[80],strRep[80],strMsg[1024];
    HFONT hFontOld;

    static TCHAR szMidiDev[50];
    static TCHAR szDevice[50];
    int iCbNum,iDevNum;
    HWND hComboBox;
    int iReturn;

    switch( LOWORD(wParam) )
    {
    case (SOME_CTRL_DEVICE_SELECT+SOME_CTRL_ID_OFFSET):
//        DebugWndPrintf("WndProc,message=0x%08lX,wParam=0x%08lX,wParam=0x%08lX\r\n",message,wParam,lParam);
        if( HIWORD(wParam) == CBN_SELCHANGE )
        {
            hComboBox = SomeCtrlGetHWND(SOME_CTRL_DEVICE_SELECT);
            iCbNum = SendMessage(hComboBox,CB_GETCURSEL,0,0);

            if( iCbNum != CB_ERR )
            {
                SOME_CTRL_GROUP_ID someCtrlGroupId = SomeCtrlGetNowGroupId();

                iDevNum = SendMessage( hComboBox , CB_GETITEMDATA, iCbNum, (LPARAM)0 );
                SomeCtrlGetText( SOME_CTRL_DEVICE_SELECT, szDevice );
                DebugWndPrintf("%s:%d\r\n",szDevice,iDevNum);

                Dx7CtrlModeSet(DX7_CTRL_MODE_NONE);
                Dx100CtrlModeSet(DX100_CTRL_MODE_NONE);

                if( iDevNum == 2 )
                {
                    mainWndData.dxDeviceMode = DX_DEVICE_MODE_DX100;
                }
                else
                {
                    mainWndData.dxDeviceMode = DX_DEVICE_MODE_DX7;
                }

                if( mainWndData.dxDeviceMode == DX_DEVICE_MODE_DX100 )
                {
                    if( someCtrlGroupId == SOME_CTRL_GROUP_ALL_VOICE )
                    {
                        Dx100CtrlModeSet(DX100_CTRL_MODE_ALL_VOICE);
                    }
                    else
                    {
                        Dx100CtrlModeSet(DX100_CTRL_MODE_PATCH);
                    }
                }
                else
                {
                    if( someCtrlGroupId == SOME_CTRL_GROUP_ALL_VOICE )
                    {
                        Dx7CtrlModeSet(DX7_CTRL_MODE_ALL_VOICE);
                    }
                    else
                    {
                        Dx7CtrlModeSet(DX7_CTRL_MODE_PATCH);
                    }
                }
            }
            else
            {
                DebugWndPrintf("Not Selected\r\n");
            }
        }
        else
        {
            nop();
        }
        break;

    case (SOME_CTRL_MIDI_IN_OPEN_BUTTON+SOME_CTRL_ID_OFFSET):
        if( !hMidiIn )
        {
            DebugWndPrintf("OPEN MIDI_IN\r\n");

            hComboBox = SomeCtrlGetHWND(SOME_CTRL_MIDI_IN);
            iCbNum = SendMessage(hComboBox,CB_GETCURSEL,0,0);

            if( iCbNum != CB_ERR )
            {
                iDevNum = SendMessage( hComboBox , CB_GETITEMDATA, iCbNum, (LPARAM)0 );
                SomeCtrlGetText( SOME_CTRL_MIDI_IN, szMidiDev );
                DebugWndPrintf("%s:%d\r\n",szMidiDev,iDevNum);

                hMidiIn = MidiInOpen(iDevNum);
            }
            else
            {
                DebugWndPrintf("Not Selected\r\n");
            }

            if( hMidiIn )
            {
                SetWindowText( SomeCtrlGetHWND(SOME_CTRL_MIDI_IN_OPEN_BUTTON), TEXT("Close") );
                SomeCtrlDisable( SOME_CTRL_MIDI_IN );

                SetTimer( hwnd, TIMER_ID, TIMER_CYCLE, NULL );
            }
            else
            {
            }
        }
        else
        {
            MidiInClose();
            KillTimer( hwnd, TIMER_ID );
            hMidiIn = FALSE;
            SomeCtrlEnable( SOME_CTRL_MIDI_IN );
            SetWindowText( SomeCtrlGetHWND(SOME_CTRL_MIDI_IN_OPEN_BUTTON), TEXT("Open") );
        }
        someCtrlDisableOnMidiOpenOrClose();
        break;

    case (SOME_CTRL_MIDI_OUT_OPEN_BUTTON+SOME_CTRL_ID_OFFSET):
        if( !hMidiOut )
        {
            DebugWndPrintf("OPEN MIDI_OUT\r\n");

            hComboBox = SomeCtrlGetHWND(SOME_CTRL_MIDI_OUT);
            iCbNum = SendMessage(hComboBox,CB_GETCURSEL,0,0);

            if( iCbNum != CB_ERR )
            {
                iDevNum = SendMessage( hComboBox , CB_GETITEMDATA, iCbNum, (LPARAM)0 );
                SomeCtrlGetText( SOME_CTRL_MIDI_OUT, szMidiDev );
                DebugWndPrintf("%s:%d\r\n",szMidiDev,iDevNum);

                hMidiOut = MidiOutOpen(iDevNum);
            }
            else
            {
                DebugWndPrintf("Not Selected\r\n");
            }

            if( hMidiOut )
            {
                SetWindowText( SomeCtrlGetHWND(SOME_CTRL_MIDI_OUT_OPEN_BUTTON), TEXT("Close") );
                SomeCtrlDisable( SOME_CTRL_MIDI_OUT );
            }
            else
            {
            }
        }
        else
        {
            MidiOutClose();
            hMidiOut = FALSE;
            SomeCtrlEnable( SOME_CTRL_MIDI_OUT );
            SetWindowText( SomeCtrlGetHWND(SOME_CTRL_MIDI_OUT_OPEN_BUTTON), TEXT("Open") );
        }
        someCtrlDisableOnMidiOpenOrClose();
        break;

    case (SOME_CTRL_MIDI_KEY_IN_OPEN_BUTTON+SOME_CTRL_ID_OFFSET):
        MessageBox(NULL, "残念賞", NULL, MB_OK);
        break;

    case (SOME_CTRL_MODE_SYSTEM     +SOME_CTRL_ID_OFFSET):
        DebugWndPrintf("SYSTEM MODE\r\n");
        break;

    case (SOME_CTRL_MODE_1VOICE     +SOME_CTRL_ID_OFFSET):
        DebugWndPrintf("1VOICE MODE\r\n");

        Dx100CtrlModeSet(DX100_CTRL_MODE_NONE);
        Dx7CtrlModeSet(DX7_CTRL_MODE_NONE  );
        if( mainWndData.dxDeviceMode == DX_DEVICE_MODE_DX100 )
        {
            Dx100CtrlModeSet(DX100_CTRL_MODE_PATCH);
        }
        else
        {
            Dx7CtrlModeSet(DX7_CTRL_MODE_PATCH);
        }
        SomeCtrlGroupDisplay(SOME_CTRL_GROUP_1VOICE);
        break;

    case (SOME_CTRL_MODE_ALL_VOICE+SOME_CTRL_ID_OFFSET):
        DebugWndPrintf("ALL VOICE MODE\r\n");

        Dx100CtrlModeSet(DX100_CTRL_MODE_NONE);
        Dx7CtrlModeSet(DX7_CTRL_MODE_NONE  );
        if( mainWndData.dxDeviceMode == DX_DEVICE_MODE_DX100 )
        {
            Dx100CtrlModeSet(DX100_CTRL_MODE_ALL_VOICE);
        }
        else
        {
            Dx7CtrlModeSet(DX7_CTRL_MODE_ALL_VOICE);
        }
        SomeCtrlGroupDisplay(SOME_CTRL_GROUP_ALL_VOICE);
        break;

    case (SOME_CTRL_DEBUG_BUTTON+SOME_CTRL_ID_OFFSET):
//        Dx100CtrlDisplayContents();
        DestroyWindow( mainWndData.hWndDebug );
        break;

    case (SOME_CTRL_VOICE_GET_BUTTON+SOME_CTRL_ID_OFFSET):
        if( mainWndData.dxDeviceMode == DX_DEVICE_MODE_DX100 )
        {
            Dx100CtrlSeqStart(DX100_CTRL_SEQ_METHOD_GET,DX100_CTRL_SEQ_1VOICE,DX100_CTRL_SEQ_1VOICE);
        }
        else
        {
            Dx7CtrlSeqStart(DX7_CTRL_SEQ_METHOD_GET,DX7_CTRL_SEQ_1VOICE,DX7_CTRL_SEQ_1VOICE);
        }
        break;

    case (SOME_CTRL_VOICE_SET_BUTTON+SOME_CTRL_ID_OFFSET):
        iReturn = MessageBox( hwnd,TEXT("Are you sure?"),mainWndData.szAppName,MB_YESNO|MB_ICONEXCLAMATION );
        if( (iReturn == IDYES) )
        {
            if( mainWndData.dxDeviceMode == DX_DEVICE_MODE_DX100 )
            {
                Dx100CtrlSeqStart(DX100_CTRL_SEQ_METHOD_SET,DX100_CTRL_SEQ_1VOICE,DX100_CTRL_SEQ_1VOICE);
            }
            else
            {
                Dx7CtrlSeqStart(DX7_CTRL_SEQ_METHOD_SET,DX7_CTRL_SEQ_1VOICE,DX7_CTRL_SEQ_1VOICE);
            }
        }
        else
        {
            nop();
        }
        break;

    case (SOME_CTRL_VOICE_LOAD_BUTTON+SOME_CTRL_ID_OFFSET):
        if( mainWndData.dxDeviceMode == DX_DEVICE_MODE_DX100 )
        {
            if( FileOpenDlg( hwnd,FILE_ID_DX100_1VOICE_DATA ) )
            {
                dataPtr = FileReadByte(FILE_ID_DX100_1VOICE_DATA,&dwSize);
                Dx100DataSet( DX100_CTRL_SEQ_1VOICE, dataPtr, dwSize );
                Dx100CtrlDisplayUpdate();
            }
            else
            {
                /* キャンセルされた。又はエラー */
            }
        }
        else
        {
            if( FileOpenDlg( hwnd,FILE_ID_DX7_1VOICE_DATA ) )
            {
                dataPtr = FileReadByte(FILE_ID_DX7_1VOICE_DATA,&dwSize);
                Dx7DataSet( DX7_CTRL_SEQ_1VOICE, dataPtr, dwSize );
                Dx7CtrlDisplayUpdate();
            }
            else
            {
                /* キャンセルされた。又はエラー */
            }
        }
        break;

    case (SOME_CTRL_VOICE_SAVE_BUTTON+SOME_CTRL_ID_OFFSET):
        if( mainWndData.dxDeviceMode == DX_DEVICE_MODE_DX100 )
        {
            dwSize = Dx100GetDataSize(DX100_CTRL_SEQ_1VOICE);
            dataPtr = malloc( dwSize * sizeof(TCHAR) );
            if( dataPtr != NULL )
            {
                Dx100DataGet( DX100_CTRL_SEQ_1VOICE,dataPtr,dwSize );
                if( FileSaveDlg( hwnd,FILE_ID_DX100_1VOICE_DATA ) )
                {
                    FileWrite( FILE_ID_DX100_1VOICE_DATA, dataPtr, dwSize );
                }
                else
                {
                    nop();
                }
                free( dataPtr );
            }
            else
            {
                nop();
            }
        }
        else
        {
            dwSize = Dx7GetDataSize(DX7_CTRL_SEQ_1VOICE);
            dataPtr = malloc( dwSize * sizeof(TCHAR) );
            if( dataPtr != NULL )
            {
                Dx7DataGet( DX7_CTRL_SEQ_1VOICE,dataPtr,dwSize );
                if( FileSaveDlg( hwnd,FILE_ID_DX7_1VOICE_DATA ) )
                {
                    FileWrite( FILE_ID_DX7_1VOICE_DATA, dataPtr, dwSize );
                }
                else
                {
                    nop();
                }
                free( dataPtr );
            }
            else
            {
                nop();
            }
        }
        break;

    case (SOME_CTRL_ALL_VOICE_LOAD_BUTTON+SOME_CTRL_ID_OFFSET):
        if( mainWndData.dxDeviceMode == DX_DEVICE_MODE_DX100 )
        {
            if( FileOpenDlg( hwnd,FILE_ID_DX100_ALL_VOICE_DATA ) )
            {
//                mainWndData.bNeedSave = FALSE;
                dataPtr = FileReadByte(FILE_ID_DX100_ALL_VOICE_DATA,&dwSize);

                Dx100DataSet( DX100_CTRL_SEQ_ALL_VOICE, dataPtr, dwSize );
                Dx100CtrlDisplayUpdate();
            }
            else
            {
                /* キャンセルされた。又はエラー */
            }
        }
        else
        {
            if( FileOpenDlg( hwnd,FILE_ID_DX7_ALL_VOICE_DATA ) )
            {
//                mainWndData.bNeedSave = FALSE;
                dataPtr = FileReadByte(FILE_ID_DX7_ALL_VOICE_DATA,&dwSize);

                Dx7DataSet( DX7_CTRL_SEQ_ALL_VOICE, dataPtr, dwSize );
                Dx7CtrlDisplayUpdate();
            }
            else
            {
                /* キャンセルされた。又はエラー */
            }
        }
        break;

    case (SOME_CTRL_ALL_VOICE_SAVE_BUTTON+SOME_CTRL_ID_OFFSET):
        if( mainWndData.dxDeviceMode == DX_DEVICE_MODE_DX100 )
        {
            dwSize = Dx100GetDataSize(DX100_CTRL_SEQ_ALL_VOICE);
            dataPtr = malloc( dwSize * sizeof(TCHAR) );

            if( dataPtr != NULL )
            {
                Dx100DataGet( DX100_CTRL_SEQ_ALL_VOICE,dataPtr,dwSize );
                if( FileSaveDlg( hwnd,FILE_ID_DX100_ALL_VOICE_DATA ) )
                {
//                    mainWndData.bNeedSave = FALSE;
//                    doCaption( hwnd, FileGetTitleName(FILE_ID_DX100_BIN),FALSE );
                    FileWrite( FILE_ID_DX100_ALL_VOICE_DATA, dataPtr, dwSize );
                }
                else
                {
                    nop();
                }
                free( dataPtr );
            }
            else
            {
                nop();
            }
        }
        else
        {
            dwSize = Dx7GetDataSize(DX7_CTRL_SEQ_ALL_VOICE);

            dataPtr = malloc( dwSize * sizeof(TCHAR) );

            if( dataPtr != NULL )
            {
                Dx7DataGet( DX7_CTRL_SEQ_ALL_VOICE,dataPtr,dwSize );
                if( FileSaveDlg( hwnd,FILE_ID_DX7_ALL_VOICE_DATA ) )
                {
//                    mainWndData.bNeedSave = FALSE;
//                    doCaption( hwnd, FileGetTitleName(FILE_ID_DX7_BIN),FALSE );
                    FileWrite( FILE_ID_DX7_ALL_VOICE_DATA, dataPtr, dwSize );
                }
                else
                {
                    nop();
                }
                free( dataPtr );
            }
            else
            {
                nop();
            }
        }
        break;

    case (SOME_CTRL_ALL_VOICE_GET_BUTTON+SOME_CTRL_ID_OFFSET):
        if( mainWndData.dxDeviceMode == DX_DEVICE_MODE_DX100 )
        {
            Dx100CtrlSeqStart(DX100_CTRL_SEQ_METHOD_GET,DX100_CTRL_SEQ_ALL_VOICE,DX100_CTRL_SEQ_ALL_VOICE);
        }
        else
        {
            Dx7CtrlSeqStart(DX7_CTRL_SEQ_METHOD_GET,DX7_CTRL_SEQ_ALL_VOICE,DX7_CTRL_SEQ_ALL_VOICE);
        }
        break;

    case (SOME_CTRL_ALL_VOICE_SET_BUTTON+SOME_CTRL_ID_OFFSET):
        iReturn = MessageBox( hwnd,TEXT("Are you sure?"),mainWndData.szAppName,MB_YESNO|MB_ICONEXCLAMATION );
        if( (iReturn == IDYES) )
        {
            if( mainWndData.dxDeviceMode == DX_DEVICE_MODE_DX100 )
            {
                Dx100CtrlSeqStart(DX100_CTRL_SEQ_METHOD_SET,DX100_CTRL_SEQ_ALL_VOICE,DX100_CTRL_SEQ_ALL_VOICE);
            }
            else
            {
                Dx7CtrlSeqStart(DX7_CTRL_SEQ_METHOD_SET,DX7_CTRL_SEQ_ALL_VOICE,DX7_CTRL_SEQ_ALL_VOICE);
            }
        }
        else
        {
            nop();
        }
        break;

    case (SOME_CTRL_SYSTEM_GET_BUTTON+SOME_CTRL_ID_OFFSET):
        break;

    case (SOME_CTRL_SYSTEM_SET_BUTTON+SOME_CTRL_ID_OFFSET):
        break;

    case IDM_FILE_NEW:
        break;
    case IDM_FILE_OPEN:
        break;
    case IDM_FILE_SAVE:
        break;
    case IDM_FILE_SAVE_AS:
        break;

    case IDM_EDIT_UNDO:
        break;

    case IDM_EDIT_CUT:
        break;

    case IDM_EDIT_COPY:
        break;

    case IDM_EDIT_PASTE:
        break;

    case IDM_EDIT_DELETE:
        break;

    case IDM_EDIT_FIND:
        fr.lStructSize   = sizeof (FINDREPLACE);
        fr.hwndOwner     = hwnd;
        fr.Flags         = FR_DOWN|/*FR_MATCHCASE|*/FR_HIDEWHOLEWORD|FR_HIDEMATCHCASE;
        fr.lpstrFindWhat = strFind;
        fr.wFindWhatLen  = 80;
        mainWndData.hDlgModeless = FindText(&fr);
        break;

    case IDM_EDIT_REPLACE:
        fr.lStructSize      = sizeof(FINDREPLACE);
        fr.hwndOwner        = hwnd;
        fr.Flags            = FR_DOWN|/*FR_MATCHCASE|*/FR_HIDEWHOLEWORD|FR_HIDEMATCHCASE;
        fr.lpstrFindWhat    = strFind;
        fr.lpstrReplaceWith = strRep;
        fr.wReplaceWithLen  = fr.wFindWhatLen = 80;
        mainWndData.hDlgModeless = ReplaceText(&fr);
        break;

    case IDM_EDIT_FIND_NEXT:
        break;

    case IDM_EDIT_SELECT_ALL:
        break;

    case IDM_EDIT_DATETIME:
        strPtr = DateTimeGetString();
        break;

    case IDM_FORMAT_FONT:
        break;

    case IDM_VIEW_STS_BAR:
        if( MenuInqItemChecked(IDM_VIEW_STS_BAR) )
        {
            MenuUnCheckItem( IDM_VIEW_STS_BAR );
//            StsBarShowWindow( FALSE );
        }
        else
        {
            MenuCheckItem( IDM_VIEW_STS_BAR );
//            StsBarShowWindow( TRUE );
        }
        SendMessage(hwnd,WM_SIZE,0,MAKELONG(mainWndData.cxClient,mainWndData.cyClient));
        break;

    case IDM_FILE_EXIT:
        SendMessage( hwnd, WM_CLOSE, 0, 0 );
        break;

    case IDM_HELP_ABOUT:
        ModalDlg( MODAL_DLG_ID_ABOUT, &modalDlgParam, hwnd, mainWndData.xPos, mainWndData.yPos );
        break;

    default:
        if( (mainWndData.dxDeviceMode==DX_DEVICE_MODE_DX7) && (Dx7CtrlOnCommand(LOWORD(wParam),HIWORD(wParam))) )
        {
            Dx7CtrlModeSet(DX7_CTRL_MODE_PATCH);
            SomeCtrlGroupDisplay(SOME_CTRL_GROUP_1VOICE);
        }
        else if( (mainWndData.dxDeviceMode==DX_DEVICE_MODE_DX100) && (Dx100CtrlOnCommand(LOWORD(wParam),HIWORD(wParam))) )
        {
            Dx100CtrlModeSet(DX100_CTRL_MODE_PATCH);
            SomeCtrlGroupDisplay(SOME_CTRL_GROUP_1VOICE);
        }
        else
        {
            nop();
        }
        break;
    }

    return rtn;
}

/********************************************************************************
 * 内容  : WM_KEYUP の処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onKeyUp( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * 内容  : WM_KEYDOWN の処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onKeyDown( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * 内容  : WM_CHAR の処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onChar( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * 内容  : WM_HSCROLL の処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onHscroll( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

//    DebugWndPrintf("onHscroll,0x%08lX \r\n",wParam);

    MainWndHscroll(hwnd,wParam);

//    GetMainWndAllScrollInfo(hwnd,&(mainWndData.iHorzPos),&(mainWndData.iVertPos));

    return rtn;
}

/********************************************************************************
 * 内容  : WM_VSCROLL の処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onVscroll( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    MainWndVscroll(hwnd,wParam);

//    GetMainWndAllScrollInfo(hwnd,&(mainWndData.iHorzPos),&(mainWndData.iVertPos));

    return rtn;
}

/********************************************************************************
 * 内容  : WM_MOUSEWHEEL を処理する
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onMouseWheel( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * 内容  : WM_SETFOCUS を処理する
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onSetFocus( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * 内容  : WM_KILLFOCUS を処理する
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onKillFocus( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * 内容  : WM_DROPFILES を処理する
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onDropFiles( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    static TCHAR szFileName[1024];
    DWORD dwSize;
    PBYTE dataPtr;

    return rtn;
}

/********************************************************************************
 * 内容  : WM_INITMENUPOPUP を処理する
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onInitMenuPopup( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    if( LOWORD(lParam) == 1 )
    { /* 「編集」 */
        if( IsClipboardFormatAvailable(CF_TEXT) )
        {
            MenuEnableItem( IDM_EDIT_PASTE );
        }
        else
        {
            MenuUnEnableItem( IDM_EDIT_PASTE );
        }
    }
    else
    {
        nop();
    }

    return rtn;
}

/********************************************************************************
 * 内容  : FINDMSGSTRINGの登録メッセージ の処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onFindMsgString( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * 内容  : WM_TIMER を処理する
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onTimer( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    TCHAR szCtrlTemp[50];

    SomeCtrlGetText( SOME_CTRL_MIDI_IN_OPEN_BUTTON, szCtrlTemp );

    if( !strcmp( szCtrlTemp, TEXT("Close")) )
    {
        MidiCycleProc();
        Dx100CtrlCycleProc();
        Dx7CtrlCycleProc();
    }
    else
    {
        nop();
    }

    SetTimer( hwnd, TIMER_ID, TIMER_CYCLE, NULL );

    return rtn;
}

/********************************************************************************
 * 内容  : WM_APP の処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onApp( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    switch( LOWORD(wParam) )
    {
    default:
        break;
    }

    return rtn;
}

/********************************************************************************
 * 内容  : メッセージのデフォルト処理
 * 引数  : HWND hwnd
 * 引数  : UINT message
 * 引数  : WPARAM wParam (内容はメッセージの種類により異なる)
 * 引数  : LPARAM lParam (内容はメッセージの種類により異なる)
 * 戻り値: LRESULT
 ***************************************/
static LRESULT
onDefault( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    return DefWindowProc( hwnd, message, wParam, lParam );
}

/********************************************************************************
 * 内容  :
 * 引数  : HWND hwnd
 * 引数  : TCHAR *szMessageFormat, ...
 * 戻り値: int
 ***************************************/
int
okMessage( HWND hwnd, TCHAR *szMessageFormat, ... )
{
    va_list vaArgs;
    TCHAR szBuffer[64 + MAX_PATH] ;
    BOOL bResult = (BOOL)FALSE;

    va_start(vaArgs, szMessageFormat);
    if( wvsprintf(szBuffer, szMessageFormat, vaArgs) != EOF )
    {
        bResult = (BOOL)TRUE;
    }
    else
    {
        /* do nothing */
    }
    va_end(vaArgs);

    if( bResult )
    {
        /* do nothing */
    }
    else
    {
        szBuffer[0] = '\0';
    }

    return MessageBox( hwnd, szBuffer, mainWndData.szAppName, MB_OK | MB_ICONINFORMATION );
}

/********************************************************************************
 * 内容  :
 * 引数  : HWND hwnd
 * 引数  : TCHAR* szTitleName
 * 引数  : BOOL bNeedSave
 * 戻り値: なし
 ***************************************/
void
doCaption( HWND hwnd, TCHAR* szTitleName, BOOL bNeedSave )
{
     TCHAR szCaption[64 + MAX_PATH];

     if( bNeedSave )
     {
         wsprintf( szCaption, TEXT ("*%s - %s"), (szTitleName[0] ? szTitleName : TEXT("無題")),mainWndData.szAppName );
     }
     else
     {
         wsprintf( szCaption, TEXT ("%s - %s"), (szTitleName[0] ? szTitleName : TEXT("無題")),mainWndData.szAppName );
     }

     SetWindowText( hwnd, szCaption );
}

/********************************************************************************
 * 内容  :
 * 引数  : なし
 * 戻り値: なし
 ***************************************/
static void
someCtrlDisableOnMidiOpenOrClose( void )
{
    if( hMidiIn && hMidiOut )
    {
        SomeCtrlEnable( SOME_CTRL_VOICE_GET_BUTTON );
        SomeCtrlEnable( SOME_CTRL_VOICE_SET_BUTTON );

        SomeCtrlEnable( SOME_CTRL_ALL_VOICE_GET_BUTTON );
        SomeCtrlEnable( SOME_CTRL_ALL_VOICE_SET_BUTTON );
    }
    else
    {
        SomeCtrlDisable( SOME_CTRL_VOICE_GET_BUTTON );
        SomeCtrlDisable( SOME_CTRL_VOICE_SET_BUTTON );

        SomeCtrlDisable( SOME_CTRL_ALL_VOICE_GET_BUTTON );
        SomeCtrlDisable( SOME_CTRL_ALL_VOICE_SET_BUTTON );
    }
}
