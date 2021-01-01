/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */
#include <winnls.h>

/* �O���֐���` */
#include "WinMain.h"
#include "Version.h"

/* �O���ϐ���` */

/* �����֐���` */
#include "ModalDlg.h"

LRESULT CALLBACK AboutDlgProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );

/* �����ϐ���` */
static TCHAR szAboutTitleName[64];

typedef struct
{
    PTSTR   className;
    PTSTR   titleName;
    WNDPROC wndPrc   ;
    int     xOffset  ;
    int     yOffset  ;
    int     nWidth   ;
    int     nHeight  ;
} S_MODAL_DLG_INFO;

S_MODAL_DLG_INFO modalDlgInfoTbl[MODAL_DLG_ID_MAX] =
{
    /* className      , titleName             , wndPrc  */
    { TEXT("AboutDlg"), TEXT("About"), AboutDlgProc, 25, 25, 487, 327 },
};

/********************************************************************************
 * ���e  : ���[�_���_�C�A���O�{�b�N�X������
 * ����  : �Ȃ�
 * �߂�l: BOOL
 ***************************************/
BOOL
ModalDlgInit( void )
{
    INT i;
    WNDCLASS wc = {0};

    for( i=0; i<MODAL_DLG_ID_MAX; i++ )
    {
        wc.style         = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc   = modalDlgInfoTbl[i].wndPrc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = GetHinst();
        wc.hIcon         = NULL;
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
        wc.lpszMenuName  = NULL;
        wc.lpszClassName = modalDlgInfoTbl[i].className;

        switch( i )
        {
        case MODAL_DLG_ID_ABOUT:
            wsprintf( szAboutTitleName, "About %s",GetAppName() );
            modalDlgInfoTbl[i].titleName = szAboutTitleName;
            break;
        default:
            break;
        }

        RegisterClass(&wc); /* �E�C���h�E�N���X�o�^ */
    }

    return TRUE;
}

/********************************************************************************
 * ���e  : ���[�_���_�C�A���O�{�b�N�X
 * ����  : MODAL_DLG_ID      id
 * ����  : S_MODAL_DLG_DATA *dataPtr
 * ����  : HWND              hwnd
 * ����  : int               x
 * ����  : int               y
 * �߂�l: BOOL
 ***************************************/
BOOL
ModalDlg( MODAL_DLG_ID id, S_MODAL_DLG_DATA *dataPtr, HWND hwnd, int x, int y )
{
    BOOL rtn = TRUE;
    HWND hDlg;
    MSG msg;

    if( id < MODAL_DLG_ID_MAX )
    {
        /* �_�C�A���O�{�b�N�X�쐬 */
        hDlg = CreateWindowEx( WS_EX_DLGMODALFRAME,
                               modalDlgInfoTbl[id].className,modalDlgInfoTbl[id].titleName,
                               WS_VISIBLE | (WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU/*|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX */),
                               x+modalDlgInfoTbl[id].xOffset, y+modalDlgInfoTbl[id].yOffset,
                               modalDlgInfoTbl[id].nWidth   , modalDlgInfoTbl[id].nHeight  ,
                               hwnd , NULL, GetHinst(), NULL );

        EnableWindow(hwnd,FALSE); /* ���C���E�C���h�E�𖳌������ă��[�_���� */

        while( GetMessage(&msg,NULL,0,0) )
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        EnableWindow(hwnd,TRUE); /* ���C���E�C���h�E��L���ɂ��ă��[�_������ */
        BringWindowToTop(hwnd);
    }
    else
    {
        rtn = FALSE;
    }

    return rtn;
}

/********************************************************************************
 * ���e  : �o�[�W�������̃��[�_���_�C�A���O�{�b�N�X
 * ����  : HWND   hwnd
 * ����  : UINT   message
 * ����  : WPARAM wParam
 * ����  : LPARAM lParam
 * �߂�l: LRESULT
 ***************************************/
LRESULT CALLBACK
AboutDlgProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LRESULT rtn = 0;
    HWND hCtrl;
    MEMORYSTATUSEX memSts;
    TCHAR szTemp[256];
    TCHAR szTemp2[256];
    TCHAR szTemp3[256];
    INT x,y;
#if 0
    static HBITMAP hBitmap;
#endif
    static HICON hIcon;

    NUMBERFMT numberFormat = {0};
    numberFormat.NumDigits = 0;
    numberFormat.LeadingZero = 1;
    numberFormat.Grouping = 3;
    numberFormat.lpDecimalSep = ".";
    numberFormat.lpThousandSep = ",";
    numberFormat.NegativeOrder = 1;

    switch( message )
    {
    case WM_CREATE:
#if 0
        /* ����` */
        CreateWindow(TEXT("static"),TEXT(""),WS_CHILD|WS_VISIBLE|SS_WHITERECT,0,0,481,90,hwnd,(HMENU)-1,GetHinst(),NULL);
#endif
#if 0
        /* bar�摜 */
        hCtrl = CreateWindow( TEXT("Static"), TEXT(""), WS_CHILD|WS_VISIBLE|SS_BITMAP,0,0,481,90, hwnd, (HMENU)-1, GetHinst(), NULL);
        hBitmap = (HBITMAP)LoadImage(GetHinst(), TEXT("BAR"), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR );
        SendMessage(hCtrl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
#endif
        /* �A�C�R���摜 */
        hCtrl = CreateWindow( TEXT("Static"), TEXT(""), WS_CHILD|WS_VISIBLE|SS_ICON,55,60,0,0, hwnd, (HMENU)-1, GetHinst(), NULL);
        hIcon = (HICON)LoadImage(GetHinst(), "MAIN_ICON", IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR );
        SendMessage(hCtrl, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);

        x = 60;
        y = 100;
        /* �A�v���P�[�V������ */
        hCtrl = CreateWindow( TEXT("static"), GetAppName(), WS_CHILD|WS_VISIBLE, x, y, 400,19, hwnd, (HMENU)-1, GetHinst(), NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );

#if 0
        /* �A�v���P�[�V������(�⑫) */
        hCtrl = CreateWindow( TEXT("static"), TEXT("(Text EDitor for Developers)"), WS_CHILD|WS_VISIBLE, x+35, y, 400,19, hwnd, (HMENU)-1, GetHinst(), NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );
#endif

        /* �o�[�W���� */
        y += 15;
        wsprintf( szTemp, TEXT("Version %s"),VersionStringGet() );
        hCtrl = CreateWindow( TEXT("static"), szTemp, WS_CHILD|WS_VISIBLE, x, y, 400,19, hwnd, (HMENU)-1, GetHinst(), NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );

        /* Copyright */
        y += 15;
        hCtrl = CreateWindow( TEXT("static"), TEXT("Copyright (C) 2017 Wurly"), WS_CHILD|WS_VISIBLE, x, y, 400,19, hwnd, (HMENU)-1, GetHinst(), NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );

#if 0
        /* ���C�Z���X */
        y = 180;
        hCtrl = CreateWindow( TEXT("static"), TEXT("���̐��i�̓t���[�\�t�g�E�F�A�ł��B"), WS_CHILD|WS_VISIBLE, x, y, 400,19, hwnd, (HMENU)-1, GetHinst(), NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );
#endif

        /* �� */
        hCtrl = CreateWindow( TEXT("static"), TEXT(""), WS_CHILD|WS_VISIBLE|SS_SUNKEN, 60, 242,410,2, hwnd, (HMENU)-1, GetHinst(), NULL );

#if 0
        /* �����������T�C�Y */
        memSts.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx( &memSts );
        wsprintf( szTemp3, TEXT("%ld"),(memSts.ullTotalPhys/1024) );
        GetNumberFormat( LOCALE_SYSTEM_DEFAULT, 0, szTemp3, &numberFormat, szTemp2, 256 );
        wsprintf( szTemp, TEXT("Windows ���g�p�ł��镨��������:\t        %s KB"),szTemp2 );
        hCtrl = CreateWindow( TEXT("static"), szTemp, WS_CHILD|WS_VISIBLE, 60, 252,400,19, hwnd, (HMENU)-1, GetHinst(), NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );
#endif

        /* OK�{�^�� */
        hCtrl = CreateWindow( TEXT("Button"), TEXT("OK"), WS_CHILD|WS_VISIBLE, 382,270,90,19, hwnd, (HMENU)0, GetHinst(), NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );

        SetFocus( hCtrl );
        break;
    case WM_DESTROY:
#if 0
        DeleteObject( hBitmap );
#endif
        DeleteObject( hIcon );
        PostQuitMessage(0); /* WM_QUIT���b�Z�[�W���|�X�g���� */
        break;
    case WM_COMMAND:
        switch( LOWORD(wParam) )
        {
        case 0:
            DestroyWindow( hwnd );
            break;
        default:
            break;
        }
        break;
    default:
        rtn = DefWindowProc( hwnd, message, wParam, lParam );
    }

    return rtn;
}
