/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */
#include "EditWndDef.h"

/* �O���֐���` */
#include "EditWndBuffer.h"
#include "DebugWnd.h"

/* �O���ϐ���` */

/* �����֐���` */
#include "EditWnd.h"
LRESULT CALLBACK EditWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static EDITWND_INDEX editWndConvertMSGtoINDEX( UINT message );
static LRESULT editOnCreate             ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnPaint              ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnSize               ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnClose              ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnDestroy            ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnCommand            ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnKeyUp              ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnKeyDown            ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnChar               ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnHscroll            ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnVscroll            ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnSetFocus           ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnKillFocus          ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnMouseActivate      ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnMouseWheel         ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnMouseMove          ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnLbuttonDown        ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnMbuttonDown        ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnRbuttonDown        ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnLbuttonUp          ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnMbuttonUp          ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnRbuttonUp          ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnImeStartComposition( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnCut                ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnCopy               ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnPaste              ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnClear              ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnUndo               ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnSetSel             ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnSetFont            ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnGetLineCount       ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnLineFromChar       ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
static LRESULT editOnDefault            ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );

static void editWndRemoveData( HWND hwnd, BOOL bBackSpace );
static void updateTextMetrics( HWND hwnd );
static void setAllScrollInfo( HWND hwnd );
static void getAllScrollInfo( HWND hwnd );
static void setScrollPos( HWND hwnd, int fnBar, DWORD nPos );

/* �����ϐ���` */
typedef struct
{
    int    cxChar;    /* �����T�C�Y(X)             */
    int    cyChar;    /* �����T�C�Y(Y)             */
    int    cxClient;  /* �N���C�A���g�̈�T�C�Y(X) */
    int    cyClient;  /* �N���C�A���g�̈�T�C�Y(Y) */
    int    cxBuffer;  /* �\���\�ȕ�����(X)       */
    int    cyBuffer;  /* �\���\�ȕ�����(Y)       */
    DWORD  dwCharSet;
    int    cxCaps;    /*  */
    int    iAccumDelta;
    int    iDeltaPerLine;
    int    iHorzPos;     /* �X�N���[���o�[�̉��ʒu  */
    int    iVertPos;     /* �X�N���[���o�[�̏c�ʒu  */
    HFONT  hFont;
    BOOL   bShiftKeyOn;
    H_EDITWND_BUFF hEditWndBuff;
    LONG   csStyle;
} S_EDITWND_DATA;

/* *INDENT-OFF* */
static LRESULT (*editWndProcTbl[EDITWND_MAX])( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ) =
{
    editOnCreate             , /* WM_CREATE              */
    editOnPaint              , /* WM_PAINT               */
    editOnSize               , /* WM_SIZE                */
    editOnClose              , /* WM_CLOSE               */
    editOnDestroy            , /* WM_DESTROY             */
    editOnCommand            , /* WM_COMMAND             */
    editOnKeyUp              , /* WM_KEYUP               */
    editOnKeyDown            , /* WM_KEYDOWN             */
    editOnChar               , /* WM_CHAR                */
    editOnHscroll            , /* WM_HSCROLL             */
    editOnVscroll            , /* WM_VSCROLL             */
    editOnSetFocus           , /* WM_SETFOCUS            */
    editOnKillFocus          , /* WM_KILLFOCUS           */
    editOnMouseActivate      , /* WM_MOUSEACTIVATE       */
    editOnMouseWheel         , /* WM_MOUSEWHEEL          */
    editOnMouseMove          , /* WM_MOUSEMOVE           */
    editOnLbuttonDown        , /* WM_LBUTTONDOWN         */
    editOnMbuttonDown        , /* WM_MBUTTONDOWN         */
    editOnRbuttonDown        , /* WM_RBUTTONDOWN         */
    editOnLbuttonUp          , /* WM_LBUTTONUP           */
    editOnMbuttonUp          , /* WM_MBUTTONUP           */
    editOnRbuttonUp          , /* WM_RBUTTONUP           */
    editOnImeStartComposition, /* WM_IME_STARTCOMPOSITION*/
    editOnCut                , /* WM_CUT                 */
    editOnCopy               , /* WM_COPY                */
    editOnPaste              , /* WM_PASTE               */
    editOnClear              , /* WM_CLEAR               */
    editOnUndo               , /* WM_UNDO                */
    editOnSetSel             , /* EM_SETSEL              */
    editOnSetFont            , /* WM_SETFONT             */
    editOnGetLineCount       , /* EM_GETLINECOUNT        */
    editOnLineFromChar       , /* EM_LINEFROMCHAR        */
    editOnDefault              /* default                */
};
/* *INDENT-ON* */

WORD DebugEditWndRect;

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�N���X�̓o�^
 * ����  : HINSTANCE hInst
 * �߂�l: BOOL
 ***************************************/
BOOL
EditWndRegisterClass( HINSTANCE hInst )
{
    BOOL rtn = TRUE;
    WNDCLASS wc = {0};

    wc.lpfnWndProc      = (WNDPROC) EditWndProc;
    wc.hInstance        = hInst;
    wc.hIcon            = (HICON)0;
    wc.hCursor          = LoadCursor(NULL, IDC_IBEAM);
    wc.hbrBackground    = (HBRUSH) CreateSolidBrush( BG_COLOR_RGB ); /* �㏑�������̂ŉ��ł����� */
    wc.lpszClassName    = "teddedit";
    wc.lpszMenuName     = NULL;
    wc.cbWndExtra       = sizeof(S_EDITWND_DATA);

    if( !RegisterClass(&wc) )
    {
        MessageBox( NULL, TEXT("RegisterClass Failed!"), TEXT("teddedit"), MB_ICONERROR );
        rtn = FALSE;
    }
    else
    {
        nop();
    }

    return rtn;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�̏�����
 * ����  : HWND hwnd
 * �߂�l: �Ȃ�
 ***************************************/
void
EditWndDataInit( HWND hwnd )
{
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    EditWndBuffInit(editWndDataPtr->hEditWndBuff);

    setAllScrollInfo(hwnd);
    InvalidateRect( hwnd, NULL, TRUE );
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�̃f�[�^�Z�b�g
 * ����  : HWND hwnd
 * ����  : TCHAR* dataPtr
 * ����  : DWORD  length
 * ����  : BOOL   bInit (TRUE:�����f�[�^���N���A,FALSE:�N���A���Ȃ�)
 * �߂�l: �Ȃ�
 ***************************************/
void
EditWndDataSet( HWND hwnd, TCHAR* dataPtr, DWORD length, BOOL bInit )
{
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    EditWndBuffDataSet( editWndDataPtr->hEditWndBuff, dataPtr, length, bInit );

    if( bInit )
    {
        nop();
    }
    else
    {
        SendMessage(GetParent(hwnd), (UINT)WM_COMMAND, MAKEWPARAM(0,EN_CHANGE), (LPARAM)hwnd);
    }

    EditWndBuffSelectOff(editWndDataPtr->hEditWndBuff);
    setAllScrollInfo(hwnd);
    InvalidateRect( hwnd, NULL, TRUE );
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�̃f�[�^�擾
 * ����  : HWND hwnd
 * ����  : TCHAR *dataPtr
 * ����  : DWORD dataSize
 * ����  : EDITWND_REGION region
 * �߂�l: BOOL
 ***************************************/
BOOL
EditWndDataGet( HWND hwnd, TCHAR *dataPtr, DWORD dataSize, EDITWND_REGION region )
{
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    return EditWndBuffDataGet( editWndDataPtr->hEditWndBuff, dataPtr, dataSize, region );
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�̃f�[�^�T�C�Y�擾
 * ����  : HWND hwnd
 * ����  : EDITWND_REGION region
 * �߂�l: DWORD
 ***************************************/
DWORD
EditWndGetDataSize( HWND hwnd, EDITWND_REGION region )
{
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    return EditWndBuffGetDataSize( editWndDataPtr->hEditWndBuff, region );
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̍ő啶���T�C�Y�擾
 * ����  : HWND hwnd
 * �߂�l: DWORD
 ***************************************/
DWORD
EditWndGetColumnMaxSize( HWND hwnd )
{
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    return EditWndBuffGetColumnMaxSize( editWndDataPtr->hEditWndBuff );
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃L�����b�gX�ʒu�擾
 * ����  : HWND hwnd
 * �߂�l: DWORD
 ***************************************/
DWORD
EditWndGetCaretXpos( HWND hwnd )
{
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);
    return EditWndBuffGetCaretXpos( editWndDataPtr->hEditWndBuff );
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�̉��s�R�[�h�Z�b�g
 * ����  : HWND hwnd
 * ����  : NEWLINECODE_TYPE newLineCodeType
 * �߂�l: BOOL (TRUE:�\�����������K�v)
 ***************************************/
BOOL
EditWndNewLineCodeSet( HWND hwnd, NEWLINECODE_TYPE newLineCodeType )
{
    DWORD allDataSize;
    TCHAR *dataTopPtr;
    BOOL bRtn = FALSE;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    EditWndBuffSetNewLineCode( editWndDataPtr->hEditWndBuff, newLineCodeType );

    allDataSize = EditWndBuffGetDataSize(editWndDataPtr->hEditWndBuff, BUFF_ALL);
    dataTopPtr  = malloc( sizeof(TCHAR) * allDataSize );
    if( dataTopPtr != NULL )
    {
        EditWndBuffDataGet( editWndDataPtr->hEditWndBuff, dataTopPtr, allDataSize, BUFF_ALL );
        EditWndBuffDataSet( editWndDataPtr->hEditWndBuff, dataTopPtr, allDataSize, TRUE );
        setAllScrollInfo(hwnd);
        InvalidateRect( hwnd, NULL, TRUE );

        free( dataTopPtr );
        bRtn = TRUE;
    }
    else
    {
        nop();
    }

    return bRtn;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�̌���������f�[�^�Z�b�g
 * ����  : HWND hwnd
 * ����  : TCHAR* dataPtr
 * ����  : DWORD  length
 * ����  : BOOL bDirectionUp
 * �߂�l: BOOL
 ***************************************/
BOOL
EditWndFindDataSet( HWND hwnd, TCHAR* dataPtr, DWORD length, BOOL bDirectionUp )
{
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);
    BOOL rtn = (BOOL)FALSE;

    rtn = EditWndBuffFindDataSet( editWndDataPtr->hEditWndBuff, dataPtr, length, bDirectionUp );

    setAllScrollInfo(hwnd);

#if 1 /* editOnKeyDown()�̏�������R�s�[���Ă������́B�ł���Ί֐���  */
    /* �L�����b�g���\���͈͊O�ɗL�����ꍇ�̏���(������) */
    if( EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff) < editWndDataPtr->iHorzPos )
    {
        setScrollPos( hwnd, SB_HORZ, EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff) );
    }
    else if( (editWndDataPtr->iHorzPos+editWndDataPtr->cxBuffer-1) < EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff) )
    {
        setScrollPos( hwnd, SB_HORZ, (editWndDataPtr->iHorzPos+editWndDataPtr->cxBuffer-1) );
    }
    else
    {
        nop();
    }

    /* �L�����b�g���\���͈͊O�ɗL�����ꍇ�̏���(�c����) */
    if( EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff) < editWndDataPtr->iVertPos )
    {
        setScrollPos( hwnd, SB_VERT, EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff) );
    }
    else if( (editWndDataPtr->iVertPos+editWndDataPtr->cyBuffer-1) < EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff) )
    {
        setScrollPos( hwnd, SB_VERT, EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff) - (editWndDataPtr->cyBuffer-1) );
    }
    else
    {
        nop();
    }
#endif

    InvalidateRect( hwnd, NULL, TRUE );

    return rtn;
}

/********************************************************************************
 * ���e  : ���C���E�B���h�E���ŏ������郁�b�Z�[�W�𔻒肷��
 * ����  : MSG *msg
 * �߂�l: BOOL
 ***************************************/
BOOL
IsEditWndMessage( MSG *msg )
{
    return FALSE;
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
EditWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
#if 0
    DebugWndPrintf("EditWndProc,%X,%X,%X\r\n",message,wParam,lParam);
#endif
    return editWndProcTbl[editWndConvertMSGtoINDEX(message)]( hwnd, message, wParam, lParam );
}

/********************************************************************************
 * ���e  : �E�B���h�E�Y���b�Z�[�W����Ή�����C���f�b�N�X�ɕϊ�����
 * ����  : UINT message
 * �߂�l: MAIN_WNDPRC_INDEX
 ***************************************/
static EDITWND_INDEX
editWndConvertMSGtoINDEX( UINT message )
{
    EDITWND_INDEX rtn;

    /* *INDENT-OFF* */
    switch( message )
    {
    case WM_CREATE              :rtn = EDITWND_ON_CREATE              ;break;
    case WM_PAINT               :rtn = EDITWND_ON_PAINT               ;break;
    case WM_SIZE                :rtn = EDITWND_ON_SIZE                ;break;
    case WM_CLOSE               :rtn = EDITWND_ON_CLOSE               ;break;
    case WM_DESTROY             :rtn = EDITWND_ON_DESTROY             ;break;
    case WM_COMMAND             :rtn = EDITWND_ON_COMMAND             ;break;
    case WM_KEYUP               :rtn = EDITWND_ON_KEYUP               ;break;
    case WM_KEYDOWN             :rtn = EDITWND_ON_KEYDOWN             ;break;
    case WM_CHAR                :rtn = EDITWND_ON_CHAR                ;break;
    case WM_HSCROLL             :rtn = EDITWND_ON_HSCROLL             ;break;
    case WM_VSCROLL             :rtn = EDITWND_ON_VSCROLL             ;break;
    case WM_SETFOCUS            :rtn = EDITWND_ON_SETFOCUS            ;break;
    case WM_KILLFOCUS           :rtn = EDITWND_ON_KILLFOCUS           ;break;
    case WM_MOUSEACTIVATE       :rtn = EDITWND_ON_MOUSEACTIVATE       ;break;
    case WM_MOUSEWHEEL          :rtn = EDITWND_ON_MOUSEWHEEL          ;break;
    case WM_MOUSEMOVE           :rtn = EDITWND_ON_MOUSEMOVE           ;break;
    case WM_LBUTTONDOWN         :rtn = EDITWND_ON_LBUTTONDOWN         ;break;
    case WM_MBUTTONDOWN         :rtn = EDITWND_ON_MBUTTONDOWN         ;break;
    case WM_RBUTTONDOWN         :rtn = EDITWND_ON_RBUTTONDOWN         ;break;
    case WM_LBUTTONUP           :rtn = EDITWND_ON_LBUTTONUP           ;break;
    case WM_MBUTTONUP           :rtn = EDITWND_ON_MBUTTONUP           ;break;
    case WM_RBUTTONUP           :rtn = EDITWND_ON_RBUTTONUP           ;break;
    case WM_IME_STARTCOMPOSITION:rtn = EDITWND_ON_IME_STARTCOMPOSITION;break;
    case WM_CUT                 :rtn = EDITWND_ON_CUT                 ;break;
    case WM_COPY                :rtn = EDITWND_ON_COPY                ;break;
    case WM_PASTE               :rtn = EDITWND_ON_PASTE               ;break;
    case WM_CLEAR               :rtn = EDITWND_ON_CLEAR               ;break;
    case WM_UNDO                :rtn = EDITWND_ON_UNDO                ;break;
    case EM_SETSEL              :rtn = EDITWND_ON_SETSEL              ;break;
    case WM_SETFONT             :rtn = EDITWND_ON_SETFONT             ;break;
    case EM_GETLINECOUNT        :rtn = EDITWND_ON_GETLINECOUNT        ;break;
    case EM_LINEFROMCHAR        :rtn = EDITWND_ON_LINEFROMCHAR        ;break;

    case WM_NCMOUSEMOVE         :rtn = EDITWND_ON_DEFAULT             ;break;
    case WM_SETCURSOR           :rtn = EDITWND_ON_DEFAULT             ;break;
    case WM_NCPAINT             :rtn = EDITWND_ON_DEFAULT             ;break;
    case WM_NCHITTEST           :rtn = EDITWND_ON_DEFAULT             ;break;
    case WM_ERASEBKGND          :rtn = EDITWND_ON_DEFAULT             ;break;

    case WM_IME_COMPOSITION     :rtn = EDITWND_ON_DEFAULT             ;break;
    case WM_IME_NOTIFY          :rtn = EDITWND_ON_DEFAULT             ;break;
    case WM_IME_SETCONTEXT      :rtn = EDITWND_ON_DEFAULT             ;break;
    case WM_IME_COMPOSITIONFULL :rtn = EDITWND_ON_DEFAULT             ;break;

    default                     :rtn = EDITWND_ON_DEFAULT             ;break;
    }
    /* *INDENT-ON* */

    return rtn;
}

/********************************************************************************
 * ���e  : WM_CREATE �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (LPCREATESTRUCT)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnCreate( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    int iDelta;          /* for MouseWheel */
    ULONG ulScrollLines; /* for MouseWheel */
    S_EDITWND_DATA *editWndDataPtr;
    LOGFONT logFont;
    LPCREATESTRUCT csPtr = (LPCREATESTRUCT)lParam;

    editWndDataPtr = (S_EDITWND_DATA *)malloc( sizeof(S_EDITWND_DATA) );
    memset( editWndDataPtr, 0, sizeof(S_EDITWND_DATA) );

    editWndDataPtr->csStyle = csPtr->style;

    GetObject( GetStockObject(SYSTEM_FIXED_FONT), sizeof(LOGFONT), (PTSTR)&(logFont) ); /* �{�ƃG�f�B�b�g�R���g���[���ƈقȂ�B�Œ蕝�݂̂Ȃ̂� */
    editWndDataPtr->hFont = CreateFontIndirect(&logFont);

    editWndDataPtr->hEditWndBuff = EditWndBuffCreate();
    SetWindowLongPtr(hwnd,0,(LONG_PTR)editWndDataPtr);

    updateTextMetrics( hwnd );

    EditWndBuffInit(editWndDataPtr->hEditWndBuff);

    /* for MouseWheel */
    SystemParametersInfo(SPI_GETWHEELSCROLLLINES,0, &ulScrollLines, 0);
    iDelta = HIWORD(wParam);
    if( ulScrollLines )
    {
        editWndDataPtr->iDeltaPerLine = WHEEL_DELTA / ulScrollLines;
    }
    else
    {
        editWndDataPtr->iDeltaPerLine = 0;
    }

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
editOnPaint( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC         hdc;
    int         iHorzPos,iVertPos;
    int         iPaintBeg,iPaintEnd,y;
    int         x;
    COLORREF    bkColor,textColor;
    S_BUFF_DISP_DATA buffDispData;
    H_EDITWND_BUFF_LINE hLineData;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    hdc = BeginPaint( hwnd, &ps );
    SelectObject( hdc, editWndDataPtr->hFont );

    getAllScrollInfo(hwnd);
    iHorzPos = editWndDataPtr->iHorzPos;
    iVertPos = editWndDataPtr->iVertPos;

    iPaintBeg = max(0, ps.rcPaint.top / editWndDataPtr->cyChar);
    iPaintEnd = ps.rcPaint.bottom / editWndDataPtr->cyChar;

    for( y=iPaintBeg,hLineData=EditWndBuffGetLinePtr(editWndDataPtr->hEditWndBuff,y+iVertPos); y<=iPaintEnd; y++ )
    { /* �ĕ`��̈�̂�1�s������ */
        for( x=0; x<(ps.rcPaint.right/editWndDataPtr->cxChar)+1;x++ )
        {
            if( hLineData != NULL )
            {
                EditWndBuffGetDispData(editWndDataPtr->hEditWndBuff,hLineData,x+iHorzPos,&buffDispData );
            }
            else
            {
                buffDispData.size = 0;
            }

            if( buffDispData.size )
            {
                bkColor = GetBkColor(hdc);
                textColor = GetTextColor(hdc);

                if( editWndDataPtr->csStyle & ES_READONLY )
                {
                    SetBkColor(hdc, GetSysColor(COLOR_BTNFACE) );
                }
                else
                {
                    if( buffDispData.type == TAB_CHAR )
                    {
                        SetBkColor(hdc, TAB_BK_COLOR_RGB );
                    }
                    else
                    {
                        SetBkColor(hdc, BK_COLOR_RGB );
                    }
                }

                if( DebugEditWndRect )
                { /* �f�o�b�O�p */
                    SetBkColor(hdc, RGB(0xFF,0x00,0x00) );
                }

                if( buffDispData.bSelect )
                {
                    SetBkColor(hdc, SELECT_BK_COLOR_RGB );
                    SetTextColor(hdc, SELECT_TEXT_COLOR_RGB );
                }
                else
                {
                    SetTextColor(hdc, TEXT_COLOR_RGB );
                }

                TextOut(hdc,
                        (x*editWndDataPtr->cxChar) - (buffDispData.offset*editWndDataPtr->cxChar), /* x���W */
                        y * editWndDataPtr->cyChar, /* y���W */
                        buffDispData.data,               /* ������ւ̃|�C���^ */
                        buffDispData.size                /* ������ */
                    );

                SetTextColor(hdc,textColor);
                SetBkColor(hdc,bkColor);
            }
            else
            {
                bkColor = GetBkColor(hdc);
                textColor = GetTextColor(hdc);

                if( editWndDataPtr->csStyle & ES_READONLY )
                {
                    SetBkColor(hdc, GetSysColor(COLOR_BTNFACE) );
                }
                else
                {
                    SetBkColor(hdc, BK_COLOR_RGB );
                }
                SetTextColor(hdc, TEXT_COLOR_RGB );

                TextOut(hdc,
                        (x*editWndDataPtr->cxChar), /* x���W */
                        y * editWndDataPtr->cyChar, /* y���W */
                        " ",               /* ������ւ̃|�C���^ */
                        1                  /* ������ */
                    );

                SetTextColor(hdc,textColor);
                SetBkColor(hdc,bkColor);
            }
        }

        if( hLineData != NULL )
        {
            hLineData = EditWndBuffGetLineNextPtr(hLineData);
        }
        else
        {
            nop();
        }
    }

    EndPaint( hwnd, &ps );

    SetCaretPos( (EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff)-editWndDataPtr->iHorzPos)*editWndDataPtr->cxChar, (EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff)-editWndDataPtr->iVertPos)*editWndDataPtr->cyChar);
    SendMessage(GetParent(hwnd), WM_COMMAND, MAKEWPARAM(0,EN_UPDATE), (LPARAM)hwnd);

    DebugEditWndRect = 0; /* �f�o�b�O�p */

    return 0;
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
editOnSize( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    editWndDataPtr->cxClient = LOWORD( lParam );
    editWndDataPtr->cyClient = HIWORD( lParam );

    setAllScrollInfo(hwnd);

    editWndDataPtr->cxBuffer = max( 1, editWndDataPtr->cxClient / editWndDataPtr->cxChar );
    editWndDataPtr->cyBuffer = max( 1, editWndDataPtr->cyClient / editWndDataPtr->cyChar );

    if( hwnd == GetFocus() )
    {
        SetCaretPos( EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff) * editWndDataPtr->cxChar, EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff) * editWndDataPtr->cyChar );
    }
    else
    {
        /* do nothing */
    }

    return 0;
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
editOnClose( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
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
editOnDestroy( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    EditWndBuffEnd(editWndDataPtr->hEditWndBuff);
    EditWndBuffDestroy(editWndDataPtr->hEditWndBuff);
    free( editWndDataPtr );

    PostQuitMessage(0); /* WM_QUIT���b�Z�[�W���|�X�g���� */
    return 0;
}

/********************************************************************************
 * ���e  : WM_COMMAND �̏���
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnCommand( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    switch( LOWORD(wParam) )
    {
    case IDM_EDIT_CUT:
        rtn = editOnCut( hwnd, message, wParam, lParam );
        break;

    case IDM_EDIT_COPY:
        rtn = editOnCopy( hwnd, message, wParam, lParam );
        break;

    case IDM_EDIT_PASTE:
        rtn = editOnPaste( hwnd, message, wParam, lParam );
        break;

    case IDM_EDIT_DELETE:
        rtn = editOnClear( hwnd, message, wParam, lParam );
        break;

    case IDM_EDIT_SELECT_ALL:
        rtn = editOnSetSel( hwnd, message, 0, -1 );
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
editOnKeyUp( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    switch(wParam)
    {
    case VK_SHIFT:
        editWndDataPtr->bShiftKeyOn = FALSE;
        break;
    default:
        break;
    }

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
editOnKeyDown( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    BOOL bErase = TRUE;
    BOOL bProc = TRUE;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    if( wParam == VK_SHIFT )
    {
        editWndDataPtr->bShiftKeyOn = TRUE;
        EditWndBuffSelectOn(editWndDataPtr->hEditWndBuff);
    }
    else if( wParam == VK_CONTROL )
    {
        nop();
    }
    else
    {
        getAllScrollInfo(hwnd);

        if( (wParam==VK_LEFT)||(wParam==VK_RIGHT)||(wParam==VK_UP)||(wParam==VK_DOWN) )
        {
            if( editWndDataPtr->bShiftKeyOn )
            {
                EditWndBuffSelectOn(editWndDataPtr->hEditWndBuff);
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

        switch(wParam)
        {
        case VK_LEFT:
            bErase = FALSE;
            EditWndBuffDecCaretXpos(editWndDataPtr->hEditWndBuff);
            break;
        case VK_RIGHT:
            bErase = FALSE;
            EditWndBuffIncCaretXpos(editWndDataPtr->hEditWndBuff);
            break;
        case VK_UP:
            bErase = FALSE;
            EditWndBuffDecCaretYpos(editWndDataPtr->hEditWndBuff);
            break;
        case VK_DOWN:
            bErase = FALSE;
            EditWndBuffIncCaretYpos(editWndDataPtr->hEditWndBuff);
            break;
        case VK_DELETE:
            if( editWndDataPtr->csStyle & ES_READONLY )
            {
                bProc = FALSE;
            }
            else
            {
                editWndRemoveData( hwnd, FALSE );
            }
            break;
        case VK_BACK:
            if( editWndDataPtr->csStyle & ES_READONLY )
            {
                bProc = FALSE;
            }
            else
            {
                editWndRemoveData( hwnd, TRUE );
            }
            break;
        default:
            bProc = FALSE;
            break;
        }

        if( bProc )
        {
            if( editWndDataPtr->bShiftKeyOn )
            {
                nop();
            }
            else
            {
                EditWndBuffSelectOff(editWndDataPtr->hEditWndBuff);
            }

            /* �L�����b�g���\���͈͊O�ɗL�����ꍇ�̏���(������) */
            if( EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff) < editWndDataPtr->iHorzPos )
            {
                setScrollPos( hwnd, SB_HORZ, EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff) );
                bErase = TRUE;
            }
            else if( (editWndDataPtr->iHorzPos+editWndDataPtr->cxBuffer-1) < EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff) )
            {
                setScrollPos( hwnd, SB_HORZ, (editWndDataPtr->iHorzPos+editWndDataPtr->cxBuffer-1) );
                bErase = TRUE;
            }
            else
            {
                nop();
            }

            /* �L�����b�g���\���͈͊O�ɗL�����ꍇ�̏���(�c����) */
            if( EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff) < editWndDataPtr->iVertPos )
            {
                setScrollPos( hwnd, SB_VERT, EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff) );
                bErase = TRUE;
            }
            else if( (editWndDataPtr->iVertPos+editWndDataPtr->cyBuffer-1) < EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff) )
            {
                setScrollPos( hwnd, SB_VERT, EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff) - (editWndDataPtr->cyBuffer-1) );
                bErase = TRUE;
            }
            else
            {
                nop();
            }

            HideCaret(hwnd);
            InvalidateRect( hwnd, NULL, bErase );
            SetCaretPos( (EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff)-editWndDataPtr->iHorzPos)*editWndDataPtr->cxChar, (EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff)-editWndDataPtr->iVertPos)*editWndDataPtr->cyChar);
            ShowCaret(hwnd);
        }
        else
        {
            nop();
        }
    }

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
editOnChar( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    TCHAR data[2];
    int i;
    INT size;
    RECT rect;
    BOOL bRectSelect = FALSE;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    if( editWndDataPtr->csStyle & ES_READONLY )
    {
        nop();
    }
    else
    {
        EditWndBuffSelectOff(editWndDataPtr->hEditWndBuff);

        for( i=0; i<(int) LOWORD(lParam); i++ )
        {
            size = 0;

            switch( wParam )
            {
            case '\b':  /* backspace */
                break;
            case '\x1B':/* escape */
                break;
            case '\n':  /* line feed */
                break;
            case '\r':  /* carriage return */
                size = EditWndBuffGetNewLineData(editWndDataPtr->hEditWndBuff,data);
                break;
            case '\t':  /* tab */
            default:
                /* �������� */
                data[0] = (TCHAR)wParam;
                size = 1;
                rect.left   = (EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff)-editWndDataPtr->iHorzPos)*editWndDataPtr->cxChar;
                rect.top    = (EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff)-editWndDataPtr->iVertPos)*editWndDataPtr->cyChar;
                rect.right  = editWndDataPtr->cxClient;
                rect.bottom = rect.top + editWndDataPtr->cyChar;
                bRectSelect = TRUE;
                break;
            }

            if( size )
            {
                EditWndBuffDataSet( editWndDataPtr->hEditWndBuff, data,size,FALSE );
                if( bRectSelect )
                {
#if 0 /* �f�o�b�O�p */
                    DebugEditWndRect = 1;
#endif
                    InvalidateRect( hwnd, &rect, TRUE );
                }
                else
                {
                    InvalidateRect( hwnd, NULL, TRUE );
                }
                SendMessage(GetParent(hwnd), WM_COMMAND, MAKEWPARAM(0,EN_CHANGE), (LPARAM)hwnd);
            }
            else
            {
                nop();
            }
        }

        editOnImeStartComposition( hwnd, message, wParam, lParam );

        SetCaretPos( (EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff)-editWndDataPtr->iHorzPos)*editWndDataPtr->cxChar, (EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff)-editWndDataPtr->iVertPos)*editWndDataPtr->cyChar);
        setAllScrollInfo(hwnd);
    }

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
editOnHscroll( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    SCROLLINFO si;
    int iHorzPos;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

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
        si.nPos = si.nTrackPos;
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
        ScrollWindow( hwnd, editWndDataPtr->cxChar * (iHorzPos - si.nPos), 0, NULL, NULL );
        UpdateWindow(hwnd);
    }

    GetScrollInfo( hwnd, SB_HORZ, &si );
    editWndDataPtr->iHorzPos = si.nPos;

    return rtn;
}

/********************************************************************************
 * ���e  : WM_VSCROLL ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnVscroll( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    SCROLLINFO si;
    int iVertPos;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    si.cbSize = sizeof(si);
    si.fMask  = SIF_ALL;
    GetScrollInfo( hwnd, SB_VERT, &si ); /* �X�N���[���o�[���擾 */
    iVertPos = si.nPos;                  /* ���݂̈ʒu               */

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
    case SB_THUMBTRACK:
        si.nPos = si.nTrackPos;
        break;
    default:
        break;
    }
    si.fMask = SIF_POS;
    SetScrollInfo(hwnd, SB_VERT, &si, TRUE); /* �X�N���[���o�[���Z�b�g���ĕ`�� */
    GetScrollInfo(hwnd, SB_VERT, &si);       /* �X�N���[���o�[���Ď擾         */

    if( si.nPos != iVertPos )
    {
        ScrollWindow(hwnd, 0, editWndDataPtr->cyChar * (iVertPos - si.nPos),NULL, NULL);
        UpdateWindow(hwnd);
    }

    GetScrollInfo(hwnd, SB_VERT, &si);
    editWndDataPtr->iVertPos = si.nPos;

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
editOnSetFocus( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    CreateCaret( hwnd,NULL,1,editWndDataPtr->cyChar ); /* ��=1,����=�����T�C�Y */
    SetCaretPos( EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff) * editWndDataPtr->cxChar, EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff) * editWndDataPtr->cyChar);
    ShowCaret( hwnd );

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
editOnKillFocus( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    HideCaret( hwnd );
    DestroyCaret();

    return rtn;
}

/********************************************************************************
 * ���e  : WM_MOUSEACTIVATE ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnMouseActivate( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    SetFocus(hwnd);

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
editOnMouseWheel( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    if( editWndDataPtr->iDeltaPerLine )
    {
        editWndDataPtr->iAccumDelta += (short)HIWORD(wParam);

        while( editWndDataPtr->iAccumDelta >= editWndDataPtr->iDeltaPerLine )
        {
            SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);
            editWndDataPtr->iAccumDelta -= editWndDataPtr->iDeltaPerLine;
        }
        while( editWndDataPtr->iAccumDelta <= -editWndDataPtr->iDeltaPerLine)
        {
            SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);
            editWndDataPtr->iAccumDelta += editWndDataPtr->iDeltaPerLine;
        }
    }
    else
    {
        /* do nothing */
    }

    return rtn;
}

/********************************************************************************
 * ���e  : WM_MOUSEMOVE ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnMouseMove( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    SHORT x,y;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);
    BOOL bCaretPosChange;

    x = max(0,(SHORT)LOWORD(lParam));
    y = max(0,(SHORT)HIWORD(lParam));

    if( (wParam & MK_LBUTTON) )
    {
        bCaretPosChange = EditWndBuffSetCaretPos( editWndDataPtr->hEditWndBuff, ((x + (editWndDataPtr->iHorzPos*editWndDataPtr->cxChar))/editWndDataPtr->cxChar), ((y + (editWndDataPtr->iVertPos*editWndDataPtr->cyChar))/editWndDataPtr->cyChar) );
        if( bCaretPosChange )
        {
            SetCaretPos( (EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff)-editWndDataPtr->iHorzPos)*editWndDataPtr->cxChar, (EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff)-editWndDataPtr->iVertPos)*editWndDataPtr->cyChar);
            InvalidateRect( hwnd, NULL, FALSE );
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

    return rtn;
}

/********************************************************************************
 * ���e  : WM_LBUTTONDOWN ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnLbuttonDown( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    int x,y;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);
    BOOL bSelectOff = FALSE;

    getAllScrollInfo(hwnd);

    x = LOWORD(lParam);
    y = HIWORD(lParam);

    if( wParam & MK_SHIFT )
    {
        nop();
    }
    else
    {
        bSelectOff = EditWndBuffSelectOff(editWndDataPtr->hEditWndBuff);
    }

    EditWndBuffSetCaretPos( editWndDataPtr->hEditWndBuff, ((x + (editWndDataPtr->iHorzPos*editWndDataPtr->cxChar))/editWndDataPtr->cxChar), ((y + (editWndDataPtr->iVertPos*editWndDataPtr->cyChar))/editWndDataPtr->cyChar) );

    SetCaretPos( (EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff)-editWndDataPtr->iHorzPos)*editWndDataPtr->cxChar, (EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff)-editWndDataPtr->iVertPos)*editWndDataPtr->cyChar);

    if( wParam & MK_SHIFT )
    {
        nop();
    }
    else
    {
        SetCapture(hwnd);
        EditWndBuffSelectOn(editWndDataPtr->hEditWndBuff);
    }

    if( bSelectOff )
    {
        InvalidateRect( hwnd, NULL, TRUE );
    }
    else
    {
        nop();
    }

    return rtn;
}

/********************************************************************************
 * ���e  : WM_MBUTTONDOWN ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnMbuttonDown( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * ���e  : WM_RBUTTONDOWN ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnRbuttonDown( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * ���e  : WM_LBUTTONUP ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnLbuttonUp( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    ReleaseCapture();

    return rtn;
}

/********************************************************************************
 * ���e  : WM_MBUTTONUP ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnMbuttonUp( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;

    return rtn;
}

/********************************************************************************
 * ���e  : WM_RBUTTONUP ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnRbuttonUp( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    HMENU hMenu;
    POINT point;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    point.x = LOWORD(lParam);
    point.y = HIWORD(lParam);

    ClientToScreen(hwnd,&point);

    hMenu = CreatePopupMenu();
    if( editWndDataPtr->csStyle & ES_READONLY )
    {
        AppendMenu( hMenu, MF_STRING   , IDM_EDIT_COPY       , TEXT("�R�s�[(&C)") );
        AppendMenu( hMenu, MF_SEPARATOR, 0                   , NULL );
        AppendMenu( hMenu, MF_STRING   , IDM_EDIT_SELECT_ALL , TEXT("���ׂđI��(&A)") );

        if( EditWndGetDataSize(hwnd,EDITWND_SELECTED) )
        {
            EnableMenuItem( hMenu, IDM_EDIT_COPY  , MF_ENABLED );
        }
        else
        {
            EnableMenuItem( hMenu, IDM_EDIT_COPY  , MF_GRAYED );
        }
    }
    else
    {
        AppendMenu( hMenu, MF_STRING   , IDM_EDIT_UNDO       , TEXT("���ɖ߂�(&U)") );
        AppendMenu( hMenu, MF_SEPARATOR, 0                   , NULL );
        AppendMenu( hMenu, MF_STRING   , IDM_EDIT_CUT        , TEXT("�؂���(&T)") );
        AppendMenu( hMenu, MF_STRING   , IDM_EDIT_COPY       , TEXT("�R�s�[(&C)") );
        AppendMenu( hMenu, MF_STRING   , IDM_EDIT_PASTE      , TEXT("�\��t��(&P)") );
        AppendMenu( hMenu, MF_STRING   , IDM_EDIT_DELETE     , TEXT("�폜(&D)") );
        AppendMenu( hMenu, MF_SEPARATOR, 0                   , NULL );
        AppendMenu( hMenu, MF_STRING   , IDM_EDIT_SELECT_ALL , TEXT("���ׂđI��(&A)") );

        EnableMenuItem( hMenu, IDM_EDIT_UNDO, MF_GRAYED );

        if( EditWndGetDataSize(hwnd,EDITWND_SELECTED) )
        {
            EnableMenuItem( hMenu, IDM_EDIT_CUT   , MF_ENABLED );
            EnableMenuItem( hMenu, IDM_EDIT_COPY  , MF_ENABLED );
            EnableMenuItem( hMenu, IDM_EDIT_DELETE, MF_ENABLED );
        }
        else
        {
            EnableMenuItem( hMenu, IDM_EDIT_CUT   , MF_GRAYED );
            EnableMenuItem( hMenu, IDM_EDIT_COPY  , MF_GRAYED );
            EnableMenuItem( hMenu, IDM_EDIT_DELETE, MF_GRAYED );
        }

        if( IsClipboardFormatAvailable(CF_TEXT) )
        {
            EnableMenuItem( hMenu, IDM_EDIT_PASTE, MF_ENABLED );
        }
        else
        {
            EnableMenuItem( hMenu, IDM_EDIT_PASTE, MF_GRAYED );
        }
    }

    TrackPopupMenu(hMenu,TPM_RIGHTBUTTON,point.x,point.y,0,hwnd,NULL);
    DestroyMenu(hMenu);

    return rtn;
}

/********************************************************************************
 * ���e  : WM_IME_STARTCOMPOSTION ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnImeStartComposition( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    HIMC hImc;
    COMPOSITIONFORM cf;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);
    LOGFONT logFont;

    GetObject( editWndDataPtr->hFont, sizeof(LOGFONT), (PTSTR)&(logFont) );

    cf.dwStyle = CFS_POINT;
    cf.ptCurrentPos.x = EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff) * editWndDataPtr->cxChar;
    cf.ptCurrentPos.y = EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff) * editWndDataPtr->cyChar;

    hImc = ImmGetContext( hwnd );
    ImmSetCompositionWindow( hImc, &cf );
    ImmSetCompositionFont( hImc,&logFont );
    ImmReleaseContext( hwnd, hImc );

    return DefWindowProc(hwnd,message, wParam, lParam);
}

/********************************************************************************
 * ���e  : WM_CUT ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnCut( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    DWORD   dwSize;
    HGLOBAL hGlobal;
    PTSTR   pGlobal;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    if( editWndDataPtr->csStyle & ES_READONLY )
    {
        nop();
    }
    else
    {
        dwSize = EditWndGetDataSize(hwnd,EDITWND_SELECTED);
        if( dwSize )
        {
            hGlobal = GlobalAlloc( GHND|GMEM_SHARE, dwSize+1 );
            pGlobal = GlobalLock( hGlobal );
            GlobalUnlock(hGlobal);
            EditWndBuffDataGet( editWndDataPtr->hEditWndBuff, pGlobal,dwSize,EDITWND_SELECTED );
            OpenClipboard(hwnd);
            EmptyClipboard();
            SetClipboardData( CF_TEXT, hGlobal );
            CloseClipboard();

            /* CUT�̂� */
            editWndRemoveData( hwnd,FALSE );
            InvalidateRect( hwnd, NULL, TRUE );
        }
        else
        {
            nop();
        }
    }

    return rtn;
}

/********************************************************************************
 * ���e  : WM_COPY ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnCopy( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    DWORD   dwSize;
    HGLOBAL hGlobal;
    PTSTR   pGlobal;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    dwSize = EditWndGetDataSize(hwnd,EDITWND_SELECTED);
    if( dwSize )
    {
        hGlobal = GlobalAlloc( GHND|GMEM_SHARE, dwSize+1 );
        pGlobal = GlobalLock( hGlobal );
        GlobalUnlock(hGlobal);
        EditWndBuffDataGet( editWndDataPtr->hEditWndBuff, pGlobal,dwSize,EDITWND_SELECTED );
        OpenClipboard(hwnd);
        EmptyClipboard();
        SetClipboardData( CF_TEXT, hGlobal );
        CloseClipboard();
    }
    else
    {
        nop();
    }

    return rtn;
}

/********************************************************************************
 * ���e  : WM_PASTE ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnPaste( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    DWORD   dwSize,dwSize2;
    HGLOBAL hGlobal;
    PTSTR   pGlobal;
    PBYTE   dataPtr;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    if( editWndDataPtr->csStyle & ES_READONLY )
    {
        nop();
    }
    else
    {
        if( IsClipboardFormatAvailable(CF_TEXT) )
        {
            OpenClipboard(hwnd);

            hGlobal = GetClipboardData(CF_TEXT);
            dwSize = GlobalSize(hGlobal) - 1;
            dataPtr = (PBYTE)malloc(dwSize);
            pGlobal = GlobalLock( hGlobal );
            strncpy( dataPtr, pGlobal, dwSize );
            GlobalUnlock(hGlobal);
            CloseClipboard();
            dwSize2 = strlen( dataPtr );
            if( dwSize2 < dwSize )
            { /* �N���b�v�{�[�h�̍ő�T�C�Y�ɒB����O��NULL������� */
                dwSize = dwSize2; /* NULL�܂ł̃f�[�^��L���Ƃ��� (�A�v���P�[�V�����ɂ���Ă� �]����NULL����ʂɓ����Ă��邱�Ƃ�����) */
            }
            else
            {
                nop();
            }
            EditWndDataSet( hwnd, dataPtr,dwSize,FALSE );
            free( dataPtr );
        }
        else
        {
            nop();
        }
    }

    return rtn;
}

/********************************************************************************
 * ���e  : WM_CLEAR ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnClear( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    if( editWndDataPtr->csStyle & ES_READONLY )
    {
        nop();
    }
    else
    {
        editWndRemoveData( hwnd,FALSE );
        InvalidateRect( hwnd, NULL, TRUE );
    }

    return rtn;
}

/********************************************************************************
 * ���e  : WM_UNDO ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnUndo( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    if( EditWndBuffUndo(editWndDataPtr->hEditWndBuff) )
    {
        InvalidateRect( hwnd, NULL, TRUE );
    }
    else
    {
        nop();
    }

    return rtn;
}

/********************************************************************************
 * ���e  : EM_SETSEL ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * ����  : LPARAM lParam (���e�̓��b�Z�[�W�̎�ނɂ��قȂ�)
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnSetSel( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    if( (wParam == 0) && (lParam == -1) )
    { /* �h���ׂđI���h�̂ݑΉ� */
        if( EditWndBuffSelectAll(editWndDataPtr->hEditWndBuff) )
        {
            EditWndBuffSetCaretPos(editWndDataPtr->hEditWndBuff, EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff),EditWndBuffGetLineMaxSize(editWndDataPtr->hEditWndBuff));
            setScrollPos( hwnd, SB_VERT, EditWndBuffGetLineMaxSize(editWndDataPtr->hEditWndBuff) );
            InvalidateRect( hwnd, NULL, TRUE );
        }
        else
        {
            nop();
        }
    }
    else
    {
        /* ���Ή��B�b��ŁA�ŏI�ʒu��I������悤�ɂ��Ă��� */
        HideCaret(hwnd);
        EditWndBuffSetCaretPos(editWndDataPtr->hEditWndBuff,0xffffffff,EditWndBuffGetLineMaxSize(editWndDataPtr->hEditWndBuff));
        setScrollPos( hwnd, SB_VERT, EditWndBuffGetLineMaxSize(editWndDataPtr->hEditWndBuff) );
        InvalidateRect( hwnd, NULL, TRUE );
        SetCaretPos( (EditWndBuffGetCaretXpos(editWndDataPtr->hEditWndBuff)-editWndDataPtr->iHorzPos)*editWndDataPtr->cxChar, (EditWndBuffGetCaretYpos(editWndDataPtr->hEditWndBuff)-editWndDataPtr->iVertPos)*editWndDataPtr->cyChar);
        ShowCaret(hwnd);
    }

    return rtn;
}

/********************************************************************************
 * ���e  : WM_SETFONT ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam �t�H���g�̃n���h��
 * ����  : LPARAM lParam �t�H���g�ݒ��ɍĕ`�悷�邩�ǂ����BTRUE �ōĕ`��
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnSetFont( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    editWndDataPtr->hFont = (HFONT)wParam;

    updateTextMetrics( hwnd );

    if( lParam )
    {
        InvalidateRect( hwnd, NULL, TRUE );
    }
    else
    {
        nop();
    }

    return rtn;
}

/********************************************************************************
 * ���e  : EM_GETLINECOUNT ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam
 * ����  : LPARAM lParam
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnGetLineCount( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    return (LRESULT)((EditWndBuffGetLineMaxSize(editWndDataPtr->hEditWndBuff)) + 1);
}

/********************************************************************************
 * ���e  : EM_LINEFROMCHAR ����������
 * ����  : HWND hwnd
 * ����  : UINT message
 * ����  : WPARAM wParam
 * ����  : LPARAM lParam
 * �߂�l: LRESULT
 ***************************************/
static LRESULT
editOnLineFromChar( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);
    DWORD result = (DWORD)0;

    if( (wParam == -1) )
    {
        result = EditWndBuffGetCaretYpos( editWndDataPtr->hEditWndBuff );
    }
    else
    {
        nop();
    }

    return (LRESULT)result;
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
editOnDefault( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    return DefWindowProc( hwnd, message, wParam, lParam );
}

/********************************************************************************
 * ���e  : �f�[�^�폜
 * ����  : HWND hwnd
 * ����  : bBackSpace
 * �߂�l: �Ȃ�
 ***************************************/
static void
editWndRemoveData( HWND hwnd, BOOL bBackSpace )
{
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    EditWndBuffRemoveData( editWndDataPtr->hEditWndBuff, bBackSpace );
    SendMessage(GetParent(hwnd), WM_COMMAND, MAKEWPARAM(0,EN_CHANGE), (LPARAM)hwnd);
}

/********************************************************************************
 * ���e  : �e�L�X�g���g���N�X�̍X�V
 * ����  : HWND hwnd
 * �߂�l: �Ȃ�
 ***************************************/
static void
updateTextMetrics( HWND hwnd )
{
    HDC        hdc;
    TEXTMETRIC tm;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    hdc = GetDC( hwnd );
    SelectObject( hdc, editWndDataPtr->hFont );

    GetTextMetrics( hdc, &tm );
    editWndDataPtr->cxChar = tm.tmAveCharWidth;
    editWndDataPtr->cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * editWndDataPtr->cxChar / 2;
    editWndDataPtr->cyChar = tm.tmHeight + tm.tmExternalLeading;

    ReleaseDC( hwnd, hdc );
}

/********************************************************************************
 * ���e  : �X�N���[�����̃Z�b�g
 * ����  : HWND hwnd
 * �߂�l: �Ȃ�
 ***************************************/
static void
setAllScrollInfo( HWND hwnd )
{
    SCROLLINFO si;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    /* Set vertical scroll bar range and page size */
    si.cbSize = sizeof(si);
    si.fMask  = SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
    si.nMin   = 0;                                                                /* �͈͂̍ŏ��l */
    si.nMax   = EditWndBuffGetLineMaxSize(editWndDataPtr->hEditWndBuff);
    si.nPage  = (editWndDataPtr->cyClient / editWndDataPtr->cyChar); /* �y�[�W�T�C�Y */
    SetScrollInfo( hwnd, SB_VERT, &si, TRUE );

    /* Set horizontal scroll bar range and page size*/
    si.cbSize = sizeof(si);
    si.fMask  = SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
    si.nMin   = 0;
    si.nMax   = max( EditWndBuffGetColumnMaxSize(editWndDataPtr->hEditWndBuff),(editWndDataPtr->cxClient/editWndDataPtr->cxChar))-1;
    si.nPage  = (editWndDataPtr->cxClient/editWndDataPtr->cxChar);
    SetScrollInfo( hwnd, SB_HORZ, &si, TRUE );
}

/********************************************************************************
 * ���e  : �X�N���[�����̎擾
 * ����  : HWND hwnd
 * �߂�l: �Ȃ�
 ***************************************/
static void
getAllScrollInfo( HWND hwnd )
{
    SCROLLINFO si;
    S_EDITWND_DATA *editWndDataPtr = (S_EDITWND_DATA *)(LONG_PTR)GetWindowLongPtr(hwnd,0);

    si.cbSize = sizeof(si);
    si.fMask  = SIF_POS;
    GetScrollInfo( hwnd, SB_VERT, &si ); /* �c�X�N���[���o�[�̈ʒu���擾 */
    editWndDataPtr->iVertPos = si.nPos;

    GetScrollInfo( hwnd, SB_HORZ, &si);   /* ���X�N���[���o�[�̈ʒu���擾 */
    editWndDataPtr->iHorzPos = si.nPos;
}

/********************************************************************************
 * ���e  : �X�N���[���ʒu�̃Z�b�g
 * ����  : HWND hwnd
 * ����  : int   fnBar �X�N���[���o�[�̃^�C�v
 * ����  : DWORD nPos  �ʒu
 * �߂�l: �Ȃ�
 ***************************************/
static void
setScrollPos( HWND hwnd, int fnBar, DWORD nPos )
{
    SCROLLINFO si;

    si.cbSize = sizeof(si);
    si.fMask  = SIF_POS;
    GetScrollInfo( hwnd, fnBar, &si );
    si.nPos   = nPos;
    SetScrollInfo( hwnd, fnBar, &si, TRUE );
}
