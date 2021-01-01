/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */
#include "MainWndDef.h"
#include "MenuId.h"
#include "MainWndMenu.h"

/* �O���֐���` */
#include "WinMain.h"
#include "SomeCtrl.h"
#include "ParamCtrl.h"
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

/* �O���ϐ���` */

/* �����֐���` */
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
short AskAboutSave( HWND hwnd, TCHAR * szTitleName );
static void someCtrlDisableOnMidiOpenOrClose( void );

/* �����ϐ���` */
static HWND hwndMain; /* ���C���E�B���h�E�̃n���h�� */
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
    onFindMsgString   , /* FINDMSGSTRING�̓o�^���b�Z�[�W */
    onTimer           , /* WM_TIMER                      */
    onApp             , /* WM_APP                        */
    onDefault           /* default                       */
};
/* *INDENT-ON* */

#define TIMER_ID      1
#define TIMER_CYCLE  100 /* msec */

/********************************************************************************
 * ���e  : ���C���E�B���h�E�N���X�̓o�^�A�E�B���h�E�̐���
 * ����  : LPSTR szCmdLine
 * ����  : int nCmdShow
 * ����  : HACCEL *hAccelPtr
 * �߂�l: HWND
 ***************************************/
HWND
MainWndCreate( LPSTR szCmdLine, int nCmdShow, HACCEL *hAccelPtr )
{
    WNDCLASS wc = {0};
    HINSTANCE hInst = GetHinst();
    PTSTR pAppName = GetAppName();
    HMENU hMenu = NULL;

    hwndMain = NULL;

    /* ���C���E�B���h�E�N���X */
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = (WNDPROC) WndProc;
    wc.cbClsExtra       = 0; /* �N���X�\���ׂ̂̈̒ǉ��̈��\�񂷂� */
    wc.cbWndExtra       = 0; /* �E�B���h�E�\���ׂ̂̈̒ǉ��̈��\�񂷂� */
    wc.hInstance        = hInst;
    wc.hIcon            = LoadIcon( hInst, TEXT("MAIN_ICON") );          /* �A�C�R�� */
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH) (COLOR_BTNFACE + 1); /* �w�i */
    wc.lpszMenuName     = pAppName;
    wc.lpszClassName    = pAppName;

    if( !RegisterClass(&wc) )
    { /* �E�B���h�E�N���X�̓o�^���s */
        MessageBox( NULL, TEXT("This program requires Windows 2000!"), pAppName, MB_ICONERROR );
    }
    else
    {
        /* �ݒ�̏����� */
        ConfigInit();

        /* ���j���[�̐��� */
        hMenu = MenuCreate();

        /* �A�N�Z�����[�^�̐��� */
        if( hAccelPtr != NULL )
        {
            *(hAccelPtr) = AccelCreate();
        }
        else
        {
            nop();
        }

        strncpy(szCmdLineLocal,szCmdLine,1024);

        /* ���C���E�B���h�E���쐬 */
        InitCommonControls(); /* commctrl.h�̃C���N���[�h�Acomctl32.lib�̃v���W�F�N�g�ւ̎Q�����K�v */
        hwndMain = CreateWindowEx( /* WS_EX_OVERLAPPEDWINDOW | */ WS_EX_ACCEPTFILES,
                                   pAppName, pAppName,
                                   WS_OVERLAPPEDWINDOW /*& ~WS_THICKFRAME*/ | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VSCROLL | WS_HSCROLL,
                                   ConfigLoadDword(CONFIG_ID_WINDOW_POS_X) , ConfigLoadDword(CONFIG_ID_WINDOW_POS_Y) ,
                                   ConfigLoadDword(CONFIG_ID_WINDOW_POS_DX), ConfigLoadDword(CONFIG_ID_WINDOW_POS_DY),
                                   NULL, hMenu, hInst, NULL );

        if( hwndMain == NULL )
        { /* �E�B���h�E�̐������s */
            nop();
        }
        else
        {
            ShowWindow( hwndMain, nCmdShow ); /* ->WM_SIZE   */ /* �E�C���h�E��\�� */
            UpdateWindow( hwndMain );         /* ->WM_PAINT  */
        }
    }

    return hwndMain;
}

/********************************************************************************
 * ���e  : ���C���E�B���h�E���ŏ������郁�b�Z�[�W�𔻒肷��
 * ����  : MSG *msgPtr
 * �߂�l: BOOL
 ***************************************/
BOOL
IsMainWndMessage( MSG *msgPtr )
{
    BOOL bRtn = FALSE;

    if( (mainWndData.hDlgModeless) &&                      /* ���[�h���X�_�C�A���O�{�b�N�X�\�����ŁA */
        IsDialogMessage(mainWndData.hDlgModeless,msgPtr) ) /* �{���b�Z�[�W�����[�h���X�_�C�A���O�{�b�N�X�̃E�B���h�E�v���V�[�W���ŏ������ꂽ */
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
 * ���e  : �E�B���h�E�v���V�[�W���BWINDOWS����󂯎�������b�Z�[�W�̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
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
 * �֐��� : ConvertMSGtoINDEX()
 * ����   : UINT message
 * �߂�l : WNDPRC_INDEX
 * ���e   : �E�B���h�E�Y���b�Z�[�W����Ή�����C���f�b�N�X�ɕϊ�����
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
 * ���e  : WM_CREATE �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onCreate( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    HDC        hdc;
    TEXTMETRIC tm;
    HFONT hFont,hOldFont;

    memset( &mainWndData, 0, sizeof(mainWndData) );

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
        mainWndData.hWndDebug = DebugWndCreate(TRUE);
        DebugWndPrintf("DEBUG=%08lX\r\n",ConfigLoadDebugValue());
    }
    else
    {
        nop();
    }

    ModalDlgInit();
    FileInitialize( hwnd ); /* �t�@�C��������     */
    FontInit();

    mainWndData.hFontIo = NULL;

    InitMainWndScrollInfo( hwnd );

    SomeCtrlCreate( hwnd ); /* �R���g���[���𐶐� */
    SomeCtrlGroupDisplay(SOME_CTRL_GROUP_1VOICE);
    someCtrlDisableOnMidiOpenOrClose();

    ParamCtrlCreate( hwnd ); /* �R���g���[���𐶐� */
    ParamCtrlGroupDisplay(PARAM_CTRL_GROUP_1VOICE);

    StsBarCreate( hwnd, TRUE ); /* �X�e�[�^�X�o�[�����A�f�t�H���g�\�� */

    MenuCheckItem( IDM_VIEW_STS_BAR );
    MenuCheckItem( IDM_EXTEND_NEWLINE_CRLF );

    MidiInit();
    Dx100CtrlInit();

    return rtn;
}

/********************************************************************************
 * ���e  : WM_PAINT �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onPaint( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    GetMainWndAllScrollInfo(hwnd,&(mainWndData.iHorzPos),&(mainWndData.iVertPos));

    return DefWindowProc( hwnd, message, wParam, lParam );
}

/********************************************************************************
 * ���e  : WM_SIZE �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onSize( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LONG topSizeSum=0,bottomSizeSum=0;

    mainWndData.cxClient = LOWORD( lParam );
    mainWndData.cyClient = HIWORD( lParam );

    SetMainWndAllScrollInfo(hwnd,mainWndData.cxClient,mainWndData.cyClient);

    topSizeSum += SomeCtrlSize( mainWndData.cxClient, mainWndData.cyChar ); /* �R���g���[��   */
    bottomSizeSum += StsBarSize( mainWndData.cxClient, mainWndData.cyChar ); /* �X�e�[�^�X�o�[ */

    GetMainWndAllScrollInfo(hwnd,&(mainWndData.iHorzPos),&(mainWndData.iVertPos));
//    ParamCtrlSetSize(mainWndData.iHorzPos,mainWndData.iVertPos);

    return 0;
}

/********************************************************************************
 * ���e  : WM_MOVE �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onMove( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    mainWndData.xPos = LOWORD( lParam );
    mainWndData.yPos = HIWORD( lParam );

    return 0;
}

/********************************************************************************
 * ���e  : WM_WINDOWPOSCHANGED �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
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
 * ���e  : WM_CLOSE �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
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

        ConfigSaveDword( CONFIG_ID_WINDOW_POS_X , mainWndData.xWindowPos );
        ConfigSaveDword( CONFIG_ID_WINDOW_POS_Y , mainWndData.yWindowPos );
        ConfigSaveDword( CONFIG_ID_WINDOW_POS_DX, mainWndData.cxWindow   );
        ConfigSaveDword( CONFIG_ID_WINDOW_POS_DY, mainWndData.cyWindow   );

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

            iReturn = MessageBox( hwnd,TEXT("�ݒ�͕ύX����Ă��܂��B\n\n�ύX��ۑ����܂���?"),GetAppName(),MB_YESNOCANCEL|MB_ICONEXCLAMATION );

            if( (iReturn == IDYES) )
            {
                ConfigWrite(0,CONFIG_ID_MAX);
            }
            else
            {
                nop();
            }
        }

        DestroyWindow( hwnd );
    }

    return 0;
}

/********************************************************************************
 * ���e  : WM_DESTROY �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onDestroy( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    KillTimer( hwnd, TIMER_ID );
    MidiInClose();
    MidiOutClose();

    DestroyAcceleratorTable( mainWndData.hAccel );

    FileEnd();

    if( mainWndData.hWndDebug )
    {
        DestroyWindow( mainWndData.hWndDebug );
    }
    else
    {
        nop();
    }

    PostQuitMessage(0); /* WM_QUIT���b�Z�[�W���|�X�g���� */
    return 0;
}

/********************************************************************************
 * ���e  : WM_COMMAND �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam �ʏ��LOWORD���ʒm�R�[�h�B(�R���g���[������̒ʒm����HIWORD���ʒm�R�[�h�ALOWORD���R���g���[��ID)
 * ����  : LPARAM lParam �ʏ��NULL�B(�R���g���[������̒ʒm���̓E�B���h�E�n���h��)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onCommand( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    DWORD dwSize;
    PBYTE dataPtr;
    PTSTR strPtr;
    S_MODAL_DLG_DATA modalDlgData;
    static FINDREPLACE fr;
    static TCHAR strFind[80],strRep[80],strMsg[1024];
    HFONT hFontOld;

    static TCHAR szMidiDev[50];
    int iCbNum,iDevNum;
    HWND hComboBox;
    int iReturn;

    switch( LOWORD(wParam) )
    {
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
        MessageBox(NULL, "�c�O��", NULL, MB_OK);
        break;

    case (SOME_CTRL_MODE_SYSTEM     +SOME_CTRL_ID_OFFSET):
        DebugWndPrintf("SYSTEM MODE\r\n");
        Dx100CtrlModeSet(DX100_CTRL_MODE_SYSTEM);
        SomeCtrlGroupDisplay(SOME_CTRL_GROUP_SYSTEM);
        ParamCtrlGroupDisplay(PARAM_CTRL_GROUP_SYSTEM_COMMON);
        break;

    case (SOME_CTRL_MODE_1VOICE     +SOME_CTRL_ID_OFFSET):
        DebugWndPrintf("1VOICE MODE\r\n");
        Dx100CtrlModeSet(DX100_CTRL_MODE_PATCH);
        SomeCtrlGroupDisplay(SOME_CTRL_GROUP_1VOICE);
        ParamCtrlGroupDisplay(PARAM_CTRL_GROUP_1VOICE);
        break;

    case (SOME_CTRL_MODE_ALL_VOICE+SOME_CTRL_ID_OFFSET):
        DebugWndPrintf("ALL VOICE MODE\r\n");
        Dx100CtrlModeSet(DX100_CTRL_MODE_ALL_VOICE);
        SomeCtrlGroupDisplay(SOME_CTRL_GROUP_ALL_VOICE);
        ParamCtrlGroupDisplay(PARAM_CTRL_GROUP_ALL_VOICE);
        break;

    case (SOME_CTRL_DEBUG_BUTTON+SOME_CTRL_ID_OFFSET):
        Dx100CtrlDisplayContents();
        break;

    case (SOME_CTRL_VOICE_GET_BUTTON+SOME_CTRL_ID_OFFSET):
        Dx100CtrlSeqStart(DX100_CTRL_SEQ_METHOD_GET,DX100_CTRL_SEQ_1VOICE,DX100_CTRL_SEQ_1VOICE);
        break;

    case (SOME_CTRL_VOICE_SET_BUTTON+SOME_CTRL_ID_OFFSET):
        iReturn = MessageBox( hwnd,TEXT("�{���ɁA�{���ɓ]�����܂����H�H�H�H�H"),GetAppName(),MB_YESNO|MB_ICONEXCLAMATION );
        if( (iReturn == IDYES) )
        {
            Dx100CtrlSeqStart(DX100_CTRL_SEQ_METHOD_SET,DX100_CTRL_SEQ_1VOICE,DX100_CTRL_SEQ_1VOICE);
        }
        else
        {
            nop();
        }
        break;

    case (SOME_CTRL_ALL_VOICE_LOAD_BUTTON+SOME_CTRL_ID_OFFSET):
        if( FileOpenDlg( hwnd,FILE_ID_ALL_VOICE_DATA ) )
        {
//                mainWndData.bNeedSave = FALSE;
            dataPtr = FileReadByte(FILE_ID_ALL_VOICE_DATA,&dwSize);
            Dx100DataSet( DX100_CTRL_SEQ_ALL_VOICE, dataPtr, dwSize );
//                doCaption( hwnd, FileGetTitleName(FILE_ID_BIN), FALSE );
            Dx100CtrlDisplayUpdate();
        }
        else
        {
            /* �L�����Z�����ꂽ�B���̓G���[ */
        }
        break;

    case (SOME_CTRL_ALL_VOICE_SAVE_BUTTON+SOME_CTRL_ID_OFFSET):
        dwSize = Dx100GetDataSize(DX100_CTRL_SEQ_ALL_VOICE);
        dataPtr = malloc( dwSize * sizeof(TCHAR) );
        if( dataPtr != NULL )
        {
            Dx100DataGet( DX100_CTRL_SEQ_ALL_VOICE,dataPtr,dwSize );
            if( FileSaveDlg( hwnd,FILE_ID_ALL_VOICE_DATA ) )
            {
//                    mainWndData.bNeedSave = FALSE;
//                    doCaption( hwnd, FileGetTitleName(FILE_ID_BIN),FALSE );
                FileWrite( FILE_ID_ALL_VOICE_DATA, dataPtr, dwSize );
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
        break;

    case (SOME_CTRL_ALL_VOICE_GET_BUTTON+SOME_CTRL_ID_OFFSET):
        Dx100CtrlSeqStart(DX100_CTRL_SEQ_METHOD_GET,DX100_CTRL_SEQ_ALL_VOICE,DX100_CTRL_SEQ_ALL_VOICE);
        break;

    case (SOME_CTRL_ALL_VOICE_SET_BUTTON+SOME_CTRL_ID_OFFSET):
        iReturn = MessageBox( hwnd,TEXT("�{���ɁA�{���ɓ]�����܂����H�H�H�H�H"),GetAppName(),MB_YESNO|MB_ICONEXCLAMATION );
        if( (iReturn == IDYES) )
        {
            Dx100CtrlSeqStart(DX100_CTRL_SEQ_METHOD_SET,DX100_CTRL_SEQ_ALL_VOICE,DX100_CTRL_SEQ_ALL_VOICE);
        }
        else
        {
            nop();
        }
        break;

    case (SOME_CTRL_SYSTEM_GET_BUTTON+SOME_CTRL_ID_OFFSET):
//            Dx100CtrlSeqStart(DX100_CTRL_SEQ_METHOD_GET,DX100_CTRL_SEQ_SYS_COMMON,DX100_CTRL_SEQ_SYS_COMMON);
        break;

    case (SOME_CTRL_SYSTEM_SET_BUTTON+SOME_CTRL_ID_OFFSET):
//            Dx100CtrlSeqStart(DX100_CTRL_SEQ_METHOD_SET,DX100_CTRL_SEQ_SYS_COMMON,DX100_CTRL_SEQ_SYS_COMMON);
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
            StsBarShowWindow( FALSE );
        }
        else
        {
            MenuCheckItem( IDM_VIEW_STS_BAR );
            StsBarShowWindow( TRUE );
        }
        SendMessage(hwnd,WM_SIZE,0,MAKELONG(mainWndData.cxClient,mainWndData.cyClient));
        break;

    case IDM_FILE_EXIT:
        SendMessage( hwnd, WM_CLOSE, 0, 0 );
        break;

    case IDM_HELP_ABOUT:
        ModalDlg( MODAL_DLG_ID_ABOUT, &modalDlgData, hwnd, mainWndData.xPos, mainWndData.yPos );
        break;

    default:
        break;
    }

    return rtn;
}

/********************************************************************************
 * ���e  : WM_KEYUP �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onKeyUp( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * ���e  : WM_KEYDOWN �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onKeyDown( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * ���e  : WM_CHAR �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onChar( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * ���e  : WM_HSCROLL �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onHscroll( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

//    DebugWndPrintf("onHscroll,0x%08lX \r\n",wParam);

    MainWndHscroll(hwnd,wParam);

//    GetMainWndAllScrollInfo(hwnd,&(mainWndData.iHorzPos),&(mainWndData.iVertPos));
//    ParamCtrlSetSize(mainWndData.iHorzPos,mainWndData.iVertPos);

    return rtn;
}

/********************************************************************************
 * ���e  : WM_VSCROLL �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onVscroll( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    MainWndVscroll(hwnd,wParam);

//    GetMainWndAllScrollInfo(hwnd,&(mainWndData.iHorzPos),&(mainWndData.iVertPos));
//    ParamCtrlSetSize(mainWndData.iHorzPos,mainWndData.iVertPos);

    return rtn;
}

/********************************************************************************
 * ���e  : WM_MOUSEWHEEL ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onMouseWheel( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * ���e  : WM_SETFOCUS ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onSetFocus( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * ���e  : WM_KILLFOCUS ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onKillFocus( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * ���e  : WM_DROPFILES ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
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
 * ���e  : WM_INITMENUPOPUP ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onInitMenuPopup( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    if( LOWORD(lParam) == 1 )
    { /* �u�ҏW�v */
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
 * ���e  : FINDMSGSTRING�̓o�^���b�Z�[�W �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onFindMsgString( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * ���e  : WM_TIMER ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
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
    }
    else
    {
        nop();
    }

    SetTimer( hwnd, TIMER_ID, TIMER_CYCLE, NULL );

    return rtn;
}

/********************************************************************************
 * ���e  : WM_APP �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
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
 * ���e  : ���b�Z�[�W�̃f�t�H���g����
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
onDefault( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    return DefWindowProc( hwnd, message, wParam, lParam );
}

/********************************************************************************
 * ���e  :
 * ����  : HWND hwnd
 * ����  : TCHAR *szMessageFormat, ...
 * �߂�l: int
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

    return MessageBox( hwnd, szBuffer, GetAppName(), MB_OK | MB_ICONINFORMATION );
}

/********************************************************************************
 * ���e  :
 * ����  : HWND hwnd
 * ����  : TCHAR* szTitleName
 * ����  : BOOL bNeedSave
 * �߂�l: �Ȃ�
 ***************************************/
void
doCaption( HWND hwnd, TCHAR* szTitleName, BOOL bNeedSave )
{
     TCHAR szCaption[64 + MAX_PATH];

     if( bNeedSave )
     {
         wsprintf( szCaption, TEXT ("*%s - %s"), (szTitleName[0] ? szTitleName : TEXT("����")),GetAppName() );
     }
     else
     {
         wsprintf( szCaption, TEXT ("%s - %s"), (szTitleName[0] ? szTitleName : TEXT("����")),GetAppName() );
     }

     SetWindowText( hwnd, szCaption );
}

/********************************************************************************
 * ���e  :
 * ����  : HWND hwnd
 * ����  : TCHAR* szTitleName
 * �߂�l: �Ȃ�
 ***************************************/
short
AskAboutSave( HWND hwnd, TCHAR * szTitleName )
{
    TCHAR szBuffer[64 + MAX_PATH];
    int   iReturn;

    wsprintf(szBuffer, TEXT("�t�@�C�� %s �̓��e�͕ύX����Ă��܂��B\n\n�ύX��ۑ����܂���?"), szTitleName[0] ? szTitleName : TEXT("����") );

    iReturn = MessageBox( hwnd,szBuffer,GetAppName(),MB_YESNOCANCEL|MB_ICONEXCLAMATION );

    if( iReturn == IDYES )
    {
        if( !SendMessage( hwnd,WM_COMMAND,IDM_FILE_SAVE,0) )
        {
            iReturn = IDCANCEL;
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

    return iReturn;
}

/********************************************************************************
 * ���e  :
 * ����  : �Ȃ�
 * �߂�l: �Ȃ�
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
