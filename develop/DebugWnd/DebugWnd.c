/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */
#include "DebugWndDef.h"
#include "DebugMenuId.h"
#include "DebugWndMenu.h"

/* �O���֐���` */
#include "DebugWndFont.h"
#include "DebugWndConfig.h"

/* �O���ϐ���` */

/* �����֐���` */
#include "DebugWnd.h"
LRESULT CALLBACK DebugWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static DEBUGWND_INDEX debugConvertMSGtoINDEX( UINT message );
static LRESULT debugOnCreate          ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnPaint           ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnSize            ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnMove            ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnWindowPosChanged( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnClose           ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnDestroy         ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnCommand         ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnKeyUp           ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnKeyDown         ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnChar            ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnHscroll         ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnVscroll         ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnMouseWheel      ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnSetFocus        ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnKillFocus       ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnInitMenuPopup   ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnFindMsgString   ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnApp             ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT debugOnDefault         ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );

static PTSTR getModuleString( void );
void debugDoCaption( HWND hwnd, TCHAR* szTitleName );

/* �����ϐ���` */
static HWND hDebugWnd; /* �f�o�b�O�E�B���h�E�̃n���h�� */
static S_DEBUGWND_DATA debugWndData;

/* *INDENT-OFF* */
static LRESULT (*wndProcTbl[DEBUGWND_MAX])( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ) =
{
    debugOnCreate          , /* WM_CREATE                     */
    debugOnPaint           , /* WM_PAINT                      */
    debugOnSize            , /* WM_SIZE                       */
    debugOnMove            , /* WM_MOVE                       */
    debugOnWindowPosChanged, /* WM_WINDOWPOSCHANGED           */
    debugOnClose           , /* WM_CLOSE                      */
    debugOnDestroy         , /* WM_DESTROY                    */
    debugOnCommand         , /* WM_COMMAND                    */
    debugOnKeyUp           , /* WM_KEYUP                      */
    debugOnKeyDown         , /* WM_KEYDOWN                    */
    debugOnChar            , /* WM_CHAR                       */
    debugOnHscroll         , /* WM_HSCROLL                    */
    debugOnVscroll         , /* WM_VSCROLL                    */
    debugOnMouseWheel      , /* WM_MOUSEWHEEL                 */
    debugOnSetFocus        , /* WM_SETFOCUS                   */
    debugOnKillFocus       , /* WM_KILLFOCUS                  */
    debugOnInitMenuPopup   , /* WM_INITMENUPOPUP              */
    debugOnFindMsgString   , /* FINDMSGSTRING�̓o�^���b�Z�[�W */
    debugOnApp             , /* WM_APP                        */
    debugOnDefault           /* default                       */
};
/* *INDENT-ON* */

static TCHAR szModuleName[] = TEXT("Debug"); /* �A�v���P�[�V�����̖��� */

#define WND_INFO_BUFSIZE (16*1024)

/********************************************************************************
 * ���e  : �f�o�b�O�E�B���h�E�N���X�̓o�^�A�E�B���h�E�̐���
 * ����  : HINSTANCE hInst
 * ����  : PTSTR szAppName
 * ����  : int nCmdShow
 * �߂�l: HWND
 ***************************************/
HWND
DebugWndCreate( HINSTANCE hInst, PTSTR szAppName, int nCmdShow )
{
    WNDCLASS wc = {0};
    PTSTR pAppName = getModuleString();
    HMENU hMenu = NULL;

    hDebugWnd = NULL;

    /* �f�o�b�O�E�B���h�E�N���X */
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = (WNDPROC) DebugWndProc;
    wc.cbClsExtra       = 0; /* �N���X�\���ׂ̂̈̒ǉ��̈��\�񂷂� */
    wc.cbWndExtra       = 0; /* �E�B���h�E�\���ׂ̂̈̒ǉ��̈��\�񂷂� */
    wc.hInstance        = hInst;
    wc.hIcon            = LoadIcon( hInst, pAppName );          /* �A�C�R�� */
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
        memset( &debugWndData, 0, sizeof(debugWndData) );
        debugWndData.hInstance = hInst;    
        debugWndData.szAppName = szAppName;

        DebugConfigInit();
        DebugFontInit();

        /* ���j���[�̐��� */
        hMenu = DebugMenuCreate();

        /* �f�o�b�O�E�B���h�E���쐬 */
        InitCommonControls(); /* commctrl.h�̃C���N���[�h�Acomctl32.lib�̃v���W�F�N�g�ւ̎Q�����K�v */
        hDebugWnd = CreateWindowEx(0,
                                   pAppName, pAppName,
                                   WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS /* | WS_VSCROLL | WS_HSCROLL*/,
                                   DebugConfigLoadDword(DEBUG_CONFIG_ID_WINDOW_POS_X) , DebugConfigLoadDword(DEBUG_CONFIG_ID_WINDOW_POS_Y) ,
                                   DebugConfigLoadDword(DEBUG_CONFIG_ID_WINDOW_POS_DX), DebugConfigLoadDword(DEBUG_CONFIG_ID_WINDOW_POS_DY),
                                   NULL, hMenu, hInst, NULL );

        if( hDebugWnd == NULL )
        { /* �E�B���h�E�̐������s */
            nop();
        }
        else
        {
            ShowWindow( hDebugWnd, nCmdShow ); /* ->WM_SIZE   */ /* �E�C���h�E��\�� */
            UpdateWindow( hDebugWnd );         /* ->WM_PAINT  */
        }
    }

    return hDebugWnd;
}

/********************************************************************************
 * ���e  : �f�o�b�O�E�B���h�E���ŏ������郁�b�Z�[�W�𔻒肷��
 * ����  : MSG *msgPtr
 * �߂�l: BOOL
 ***************************************/
BOOL
IsDebugWndMessage( MSG *msgPtr )
{
    BOOL bRtn = FALSE;

    if( (debugWndData.hDlgModeless) &&                      /* ���[�h���X�_�C�A���O�{�b�N�X�\�����ŁA */
        IsDialogMessage(debugWndData.hDlgModeless,msgPtr) ) /* �{���b�Z�[�W�����[�h���X�_�C�A���O�{�b�N�X�̃E�B���h�E�v���V�[�W���ŏ������ꂽ */
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
 * ���e   : �f�o�b�O�E�B���h�E�ւ� printf
 * ����   : PTSTR ptstrFormat, ...
 * �߂�l : BOOL
 ********************************************************************************/
BOOL
DebugWndPrintf( PTSTR ptstrFormat, ...)
{
    va_list vaArgs;
    INT iPos;
    static TCHAR szBuf[1024];

    if( debugWndData.hWndEdit != NULL )
    {
        va_start(vaArgs, ptstrFormat);

        if( wvsprintf(szBuf, ptstrFormat, vaArgs) != EOF )
        {
            iPos = SendMessage(debugWndData.hWndEdit, WM_GETTEXTLENGTH, 0, 0); /* �����ʒu���擾*/

            /* �e�L�X�g�T�C�Y�̃��~�b�g�l���`�F�b�N */
            if((iPos + strlen(szBuf)) >= WND_INFO_BUFSIZE)
            {
                /* ���~�b�g���z�������Ȃ�A�擪������؂�̂Ă� */
                iPos = SendMessage(debugWndData.hWndEdit, EM_LINEFROMCHAR, WND_INFO_BUFSIZE/10, 0) + 1; /* 10����1�̈ʒu���擾                   */
                iPos = SendMessage(debugWndData.hWndEdit, EM_LINEINDEX, iPos, 0);                       /* �s�����擾                            */
                SendMessage(debugWndData.hWndEdit, EM_SETSEL, 0, iPos);                                 /* �擪����A10����1�̈ʒu(�̍s��)��I�� */
                SendMessage(debugWndData.hWndEdit, EM_REPLACESEL, FALSE, (LPARAM)"");                   /* �I�������̈������                    */

                iPos = SendMessage(debugWndData.hWndEdit, WM_GETTEXTLENGTH, 0, 0);                      /* �ēx�A�����ʒu���擾*/
            }

            SendMessage(debugWndData.hWndEdit, EM_SETSEL, iPos, iPos);
            SendMessage(debugWndData.hWndEdit, EM_REPLACESEL, FALSE, (LPARAM)szBuf);
        }
        else
        {
            /* do nothing */
        }
        va_end(vaArgs);
    }
    else
    {
        /* do nothing */
    }

    return TRUE;
}


/********************************************************************************
 * ���e   : �f�o�b�O�E�B���h�E�ւ� printf (�㏑��)
 * ����   : PTSTR ptstrFormat, ...
 * �߂�l : BOOL
 ********************************************************************************/
BOOL
DebugWndPrintfOW( PTSTR ptstrFormat, ...)
{
    va_list vaArgs;
    INT iPos,iPos2;
    static TCHAR szBuf[1024];

    if( debugWndData.hWndEdit != NULL )
    {
        va_start(vaArgs, ptstrFormat);

        if( wvsprintf(szBuf, ptstrFormat, vaArgs) != EOF )
        {
            iPos = SendMessage(debugWndData.hWndEdit, WM_GETTEXTLENGTH, 0, 0); /* �����ʒu���擾*/

            /* �e�L�X�g�T�C�Y�̃��~�b�g�l���`�F�b�N */
            if((iPos + strlen(szBuf)) >= WND_INFO_BUFSIZE)
            {
                /* ���~�b�g���z�������Ȃ�A�擪������؂�̂Ă� */
                iPos = SendMessage(debugWndData.hWndEdit, EM_LINEFROMCHAR, WND_INFO_BUFSIZE/10, 0) + 1; /* 10����1�̈ʒu���擾                   */
                iPos = SendMessage(debugWndData.hWndEdit, EM_LINEINDEX, iPos, 0);                       /* �s�����擾                            */
                SendMessage(debugWndData.hWndEdit, EM_SETSEL, 0, iPos);                                 /* �擪����A10����1�̈ʒu(�̍s��)��I�� */
                SendMessage(debugWndData.hWndEdit, EM_REPLACESEL, FALSE, (LPARAM)"");                   /* �I�������̈������                    */

                iPos = SendMessage(debugWndData.hWndEdit, WM_GETTEXTLENGTH, 0, 0);                      /* �ēx�A�����ʒu���擾*/
            }

            SendMessage(debugWndData.hWndEdit, EM_SETSEL, iPos, iPos);
            iPos2 = SendMessage(debugWndData.hWndEdit, EM_LINEINDEX, -1, 0);  /* �s�����擾 */ /* �Ⴄ�̂̓R�R��    */
            SendMessage(debugWndData.hWndEdit, EM_SETSEL, iPos2, iPos);                        /* �R�R����          */
            SendMessage(debugWndData.hWndEdit, EM_REPLACESEL, FALSE, (LPARAM)szBuf);
        }
        else
        {
            /* do nothing */
        }
        va_end(vaArgs);
    }
    else
    {
        /* do nothing */
    }

    return TRUE;
}


/********************************************************************************
 * ���e   : �f�o�b�O�E�B���h�E�̍ŏI�s�N���A
 * ����   : �Ȃ�
 * �߂�l : BOOL
 ********************************************************************************/
BOOL
DebugWndClearLine( void )
{
    INT iPos,iPos2;

    if( debugWndData.hWndEdit != NULL )
    {
        iPos  = SendMessage(debugWndData.hWndEdit, WM_GETTEXTLENGTH, 0, 0);   /* �����ʒu���擾     */
        iPos2 = SendMessage(debugWndData.hWndEdit, EM_LINEINDEX, -1, 0);      /* �s�����擾         */
        SendMessage(debugWndData.hWndEdit, EM_SETSEL, iPos2, iPos);           /* �s������s����I�� */
        SendMessage(debugWndData.hWndEdit, EM_REPLACESEL, FALSE, (LPARAM)""); /* �I�������̈������ */
    }
    else
    {
        /* do nothing */
    }

    return TRUE;
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
DebugWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    return wndProcTbl[debugConvertMSGtoINDEX(message)]( hwnd, message, wParam, lParam );
}

/********************************************************************************
 * �֐��� : ConvertMSGtoINDEX()
 * ����   : UINT message
 * �߂�l : WNDPRC_INDEX
 * ���e   : �E�B���h�E�Y���b�Z�[�W����Ή�����C���f�b�N�X�ɕϊ�����
 ***************************************/
static DEBUGWND_INDEX
debugConvertMSGtoINDEX( UINT message )
{
    DEBUGWND_INDEX rtn;

    /* *INDENT-OFF* */
    switch( message )
    {
    case WM_CREATE          :rtn = DEBUGWND_ON_CREATE          ;break;
    case WM_PAINT           :rtn = DEBUGWND_ON_PAINT           ;break;
    case WM_SIZE            :rtn = DEBUGWND_ON_SIZE            ;break;
    case WM_MOVE            :rtn = DEBUGWND_ON_MOVE            ;break;
    case WM_WINDOWPOSCHANGED:rtn = DEBUGWND_ON_WINDOWPOSCHANGED;break;
    case WM_CLOSE           :rtn = DEBUGWND_ON_CLOSE           ;break;
    case WM_DESTROY         :rtn = DEBUGWND_ON_DESTROY         ;break;
    case WM_COMMAND         :rtn = DEBUGWND_ON_COMMAND         ;break;
    case WM_APP             :rtn = DEBUGWND_ON_APP             ;break;
    case WM_KEYUP           :rtn = DEBUGWND_ON_KEYUP           ;break;
    case WM_KEYDOWN         :rtn = DEBUGWND_ON_KEYDOWN         ;break;
    case WM_CHAR            :rtn = DEBUGWND_ON_CHAR            ;break;
    case WM_HSCROLL         :rtn = DEBUGWND_ON_HSCROLL         ;break;
    case WM_VSCROLL         :rtn = DEBUGWND_ON_VSCROLL         ;break;
    case WM_MOUSEWHEEL      :rtn = DEBUGWND_ON_MOUSEWHEEL      ;break;
    case WM_SETFOCUS        :rtn = DEBUGWND_ON_SETFOCUS        ;break;
    case WM_KILLFOCUS       :rtn = DEBUGWND_ON_KILLFOCUS       ;break;
    case WM_INITMENUPOPUP   :rtn = DEBUGWND_ON_INITMENUPOPUP   ;break;
    default                 :
        if( message == debugWndData.messageFindReplace )
        {
            rtn = DEBUGWND_ON_FINDMSGSTRING;
        }
        else
        {
            rtn = DEBUGWND_ON_DEFAULT;
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
debugOnCreate( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    HDC        hdc;
    TEXTMETRIC tm;
    HFONT hFont,hOldFont;

    hdc = GetDC( hwnd );
    hFont = GetStockObject(DEFAULT_GUI_FONT);
    hOldFont = SelectObject(hdc, hFont);
    GetTextMetrics( hdc, &tm );
    debugWndData.cxChar = tm.tmAveCharWidth;
    debugWndData.cyChar = tm.tmHeight + (tm.tmHeight/2) + (GetSystemMetrics(SM_CYEDGE) * 2);

    debugWndData.messageFindReplace = RegisterWindowMessage(FINDMSGSTRING);

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
    ReleaseDC( hwnd,hdc );

    debugWndData.hWndEdit = CreateWindowEx( WS_EX_OVERLAPPEDWINDOW,
                                         TEXT ("edit"), NULL,
                                         WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
                                         ES_LEFT | ES_MULTILINE | ES_NOHIDESEL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_READONLY,
                                         0, 0, 0, 0,
                                         hwnd, (HMENU)0, debugWndData.hInstance, NULL) ;
    debugWndData.hFontIo = NULL;

    debugDoCaption( hwnd, "applicationName" );

    SetWindowText(debugWndData.hWndEdit,"-----Debug Console Start-----\r\n");

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
debugOnPaint( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
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
debugOnSize( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LONG topSizeSum=0,bottomSizeSum=0;

    debugWndData.cxClient = LOWORD( lParam );
    debugWndData.cyClient = HIWORD( lParam );

    MoveWindow( debugWndData.hWndEdit, 0, topSizeSum, debugWndData.cxClient, debugWndData.cyClient - topSizeSum - bottomSizeSum, TRUE) ;

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
debugOnMove( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    debugWndData.xPos = LOWORD( lParam );
    debugWndData.yPos = HIWORD( lParam );

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
debugOnWindowPosChanged( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    debugWndData.xWindowPos = ((WINDOWPOS *)lParam)->x ;
    debugWndData.yWindowPos = ((WINDOWPOS *)lParam)->y ;
    debugWndData.cxWindow   = ((WINDOWPOS *)lParam)->cx;
    debugWndData.cyWindow   = ((WINDOWPOS *)lParam)->cy;

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
debugOnClose( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    DestroyWindow( hwnd );

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
debugOnDestroy( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    DebugConfigSaveDword( DEBUG_CONFIG_ID_WINDOW_POS_X , debugWndData.xWindowPos );
    DebugConfigSaveDword( DEBUG_CONFIG_ID_WINDOW_POS_Y , debugWndData.yWindowPos );
    DebugConfigSaveDword( DEBUG_CONFIG_ID_WINDOW_POS_DX, debugWndData.cxWindow   );
    DebugConfigSaveDword( DEBUG_CONFIG_ID_WINDOW_POS_DY, debugWndData.cyWindow   );

    DestroyWindow( debugWndData.hWndEdit );

    memset( &debugWndData, 0, sizeof(debugWndData) );

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
debugOnCommand( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    DWORD dwSize;
    PBYTE dataPtr;
    PTSTR strPtr;
    static FINDREPLACE fr;
    static TCHAR strFind[80],strRep[80],strMsg[1024];
    HFONT hFontOld;

    if( (HWND)lParam == debugWndData.hWndEdit )
    {
        switch( HIWORD(wParam) )
        {
        case EN_UPDATE:
            break;
        case EN_CHANGE:
            break;
        default:
            break;
        }
    }
    else
    {
        switch( LOWORD(wParam) )
        {
        case IDM_DEBUG_FILE_NEW:
            SendMessage(debugWndData.hWndEdit, EM_SETSEL, 0, SendMessage(debugWndData.hWndEdit, WM_GETTEXTLENGTH, 0, 0));
            SendMessage(debugWndData.hWndEdit, EM_REPLACESEL, FALSE, (LPARAM)"");  /* �I�������̈������ */
            break;
        case IDM_DEBUG_FILE_SAVE_AS:
            break;

        case IDM_DEBUG_EDIT_COPY:
            SendMessage( debugWndData.hWndEdit, WM_COPY, 0, 0 );
            break;

        case IDM_DEBUG_EDIT_FIND:
            fr.lStructSize   = sizeof (FINDREPLACE);
            fr.hwndOwner     = hwnd;
            fr.Flags         = FR_MATCHCASE|FR_HIDEWHOLEWORD;
            fr.lpstrFindWhat = strFind;
            fr.wFindWhatLen  = 80;
            debugWndData.hDlgModeless = FindText(&fr);
            break;

        case IDM_DEBUG_EDIT_FIND_NEXT:
            break;

        case IDM_DEBUG_EDIT_SELECT_ALL:
            SendMessage( debugWndData.hWndEdit, EM_SETSEL, 0, -1 );
            break;

        case IDM_DEBUG_FORMAT_FONT:
            if( DebugFontChooseFont( hwnd ) )
            {
                hFontOld = debugWndData.hFontIo;
                debugWndData.hFontIo = CreateFontIndirect( DebugFontGetLogFont() );
                SendMessage( debugWndData.hWndEdit, WM_SETFONT, (WPARAM)debugWndData.hFontIo, (LPARAM)TRUE );

                if( hFontOld != NULL )
                {
                    DeleteObject(hFontOld);
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
            break;

        case IDM_DEBUG_FILE_EXIT:
            SendMessage( hwnd, WM_CLOSE, 0, 0 );
            break;

        default:
            break;
        }
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
debugOnKeyUp( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
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
debugOnKeyDown( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
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
debugOnChar( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
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
debugOnHscroll( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

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
debugOnVscroll( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

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
debugOnMouseWheel( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    SendMessage(debugWndData.hWndEdit,message,wParam,lParam);

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
debugOnSetFocus( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    SetFocus(debugWndData.hWndEdit);

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
debugOnKillFocus( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    SendMessage(debugWndData.hWndEdit,message,wParam,lParam);

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
debugOnInitMenuPopup( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    if( LOWORD(lParam) == 1 )
    { /* �u�ҏW�v */
        if( 1 )
        {
            DebugMenuEnableItem( IDM_DEBUG_EDIT_COPY   );
        }
        else
        {
            DebugMenuUnEnableItem( IDM_DEBUG_EDIT_COPY   );
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
debugOnFindMsgString( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    LPFINDREPLACE pfr;

    pfr = (LPFINDREPLACE)lParam;

    if( pfr->Flags & FR_DIALOGTERM )
    {
        debugWndData.hDlgModeless = NULL;
    }
    else
    {
        nop();
    }

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
debugOnApp( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
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
debugOnDefault( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    return DefWindowProc( hwnd, message, wParam, lParam );
}

/********************************************************************************
 * ���e   : ���W���[���̖��̕�����̎擾
 * ����   : �Ȃ�
 * �߂�l : PTSTR
 ***************************************/
static PTSTR
getModuleString( void )
{
    return szModuleName;
}

/********************************************************************************
 * ���e  :
 * ����  : HWND hwnd
 * ����  : TCHAR* szTitleName
 * �߂�l: �Ȃ�
 ***************************************/
void
debugDoCaption( HWND hwnd, TCHAR* szTitleName )
{
     TCHAR szCaption[64 + MAX_PATH];

     wsprintf( szCaption, TEXT ("%s - %s"), (szTitleName[0] ? szTitleName : TEXT("����")),getModuleString() );

     SetWindowText( hwnd, szCaption );
}
