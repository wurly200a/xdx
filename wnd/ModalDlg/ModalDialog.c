/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */
#include <winnls.h>

/* �O���֐���` */
#include "Version.h"

/* �O���ϐ���` */

/* �����֐���` */
#include "ModalDlg.h"

BOOL CALLBACK AboutDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );

/* �����ϐ���` */

typedef struct
{
    HINSTANCE         hInstance;
    PTSTR             szAppName;
    S_MODAL_DLG_PARAM *paramPtr[MODAL_DLG_ID_MAX];
} S_MODAL_DLG_DATA;

static S_MODAL_DLG_DATA modalDlgData;

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
    /* className         , titleName             , wndPrc         , x , y , w  , h*/
    { TEXT("ABOUTDLG")   , TEXT("�o�[�W�������"), AboutDlgProc   , 25, 25, 487, 327 },
};

/********************************************************************************
 * ���e  : ���[�_���_�C�A���O�{�b�N�X������
 * ����  : HINSTANCE hInst
 * ����  : PTSTR szAppName
 * �߂�l: BOOL
 ***************************************/
BOOL
ModalDlgInit( HINSTANCE hInst, PTSTR szAppName )
{
    INT i;
    WNDCLASS wc = {0};

    modalDlgData.hInstance = hInst;
    modalDlgData.szAppName = szAppName;

    return TRUE;
}

/********************************************************************************
 * ���e  : ���[�_���_�C�A���O�{�b�N�X
 * ����  : MODAL_DLG_ID      id
 * ����  : S_MODAL_DLG_PARAM *dataPtr
 * ����  : HWND              hwnd
 * ����  : int               x
 * ����  : int               y
 * �߂�l: BOOL
 ***************************************/
BOOL
ModalDlg( MODAL_DLG_ID id, S_MODAL_DLG_PARAM *dataPtr, HWND hwnd, int x, int y )
{
    BOOL rtn = TRUE;
    HWND hDlg;
    MSG msg;

    if( id < MODAL_DLG_ID_MAX )
    {
        modalDlgData.paramPtr[id] = dataPtr;

        /* �_�C�A���O�{�b�N�X�쐬 */
        DialogBoxParam(modalDlgData.hInstance, modalDlgInfoTbl[id].className, hwnd, (DLGPROC)modalDlgInfoTbl[id].wndPrc,id);
    }
    else
    {
        rtn = FALSE;
    }

    return rtn;
}

/********************************************************************************
 * ���e  : �o�[�W�������̃��[�_���_�C�A���O�{�b�N�X
 * ����  : HWND   hDlg
 * ����  : UINT   message
 * ����  : WPARAM wParam
 * ����  : LPARAM lParam
 * �߂�l: BOOL
 ***************************************/
BOOL CALLBACK
AboutDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
    BOOL rtn = FALSE;
    static  MODAL_DLG_ID modalDlgId;
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
    HINSTANCE hInst = modalDlgData.hInstance;

    NUMBERFMT numberFormat = {0};
    numberFormat.NumDigits = 0;
    numberFormat.LeadingZero = 1;
    numberFormat.Grouping = 3;
    numberFormat.lpDecimalSep = ".";
    numberFormat.lpThousandSep = ",";
    numberFormat.NegativeOrder = 1;

#if 0
    DebugWndPrintf("message=:0x%04X\r\n",message);
#endif

    switch( message )
    {
    case WM_INITDIALOG:
        modalDlgId = lParam;
#if 0
        /* ����` */
        CreateWindow(TEXT("static"),TEXT(""),WS_CHILD|WS_VISIBLE|SS_WHITERECT,0,0,481,90,hDlg,(HMENU)-1,hInst,NULL);
#endif
#if 0
        /* bar�摜 */
        hCtrl = CreateWindow( TEXT("Static"), TEXT(""), WS_CHILD|WS_VISIBLE|SS_BITMAP,0,0,481,90, hDlg, (HMENU)-1, hInst, NULL);
        hBitmap = (HBITMAP)LoadImage(hInst, TEXT("BAR"), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR );
        SendMessage(hCtrl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
#endif
        /* �A�C�R���摜 */
        hCtrl = CreateWindow( TEXT("Static"), TEXT(""), WS_CHILD|WS_VISIBLE|SS_ICON,55,60,0,0, hDlg, (HMENU)-1, hInst, NULL);
        hIcon = (HICON)LoadImage(hInst, modalDlgData.szAppName, IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR );
        SendMessage(hCtrl, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);

        x = 60;
        y = 100;
        /* �A�v���P�[�V������ */
        hCtrl = CreateWindow( TEXT("static"), modalDlgData.szAppName, WS_CHILD|WS_VISIBLE, x, y, 400,19, hDlg, (HMENU)-1, hInst, NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );

#if 0
        /* �A�v���P�[�V������(�⑫) */
        hCtrl = CreateWindow( TEXT("static"), TEXT("(Text EDitor for Developers)"), WS_CHILD|WS_VISIBLE, x+35, y, 400,19, hDlg, (HMENU)-1, hInst, NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );
#endif

        /* �o�[�W���� */
        y += 15;
        wsprintf( szTemp, TEXT("Version %s"),VersionStringGet() );
        hCtrl = CreateWindow( TEXT("static"), szTemp, WS_CHILD|WS_VISIBLE, x, y, 400,19, hDlg, (HMENU)-1, hInst, NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );

        /* Copyright */
        y += 15;
        hCtrl = CreateWindow( TEXT("static"), TEXT("Copyright (C) 2009-2017 Wurly"), WS_CHILD|WS_VISIBLE, x, y, 400,19, hDlg, (HMENU)-1, hInst, NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );

        /* ���C�Z���X */
        y = 180;
        hCtrl = CreateWindow( TEXT("static"), TEXT(""/*"���̐��i�̓t���[�\�t�g�E�F�A�ł��B"*/), WS_CHILD|WS_VISIBLE, x, y, 400,19, hDlg, (HMENU)-1, hInst, NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );

        /* �� */
        hCtrl = CreateWindow( TEXT("static"), TEXT(""), WS_CHILD|WS_VISIBLE|SS_SUNKEN, 60, 242,410,2, hDlg, (HMENU)-1, hInst, NULL );

        /* �����������T�C�Y */
        memSts.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx( &memSts );
        wsprintf( szTemp3, TEXT("%ld"),(memSts.ullTotalPhys/1024) );
        GetNumberFormat( LOCALE_SYSTEM_DEFAULT, 0, szTemp3, &numberFormat, szTemp2, 256 );
//        wsprintf( szTemp, TEXT("Windows ���g�p�ł��镨��������:\t        %s KB"),szTemp2 );
        hCtrl = CreateWindow( TEXT("static"), szTemp, WS_CHILD|WS_VISIBLE, 60, 252,400,19, hDlg, (HMENU)-1, hInst, NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );

        y = 280;
#ifdef _MSC_VER
        wsprintf( szTemp, TEXT("_MSC_VER = %d(%d.%d)"),_MSC_VER, _MSC_VER/100,_MSC_VER%100 );
        hCtrl = CreateWindow( TEXT("static"), szTemp, WS_CHILD|WS_VISIBLE, x, y, 400,19, hDlg, (HMENU)-1, hInst, NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );

        y += 15;
        wsprintf( szTemp, TEXT("_MSC_FULL_VER = %d(%d.%d.%d)"),_MSC_FULL_VER, _MSC_FULL_VER/10000000,(_MSC_FULL_VER%10000000)/100000,(_MSC_FULL_VER%10000000)%100000 );
        hCtrl = CreateWindow( TEXT("static"), szTemp, WS_CHILD|WS_VISIBLE, x, y, 400,19, hDlg, (HMENU)-1, hInst, NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );
#elif defined(__GNUC__)
        wsprintf( szTemp, TEXT("__GNUC__ = %d"),__GNUC__ );
        hCtrl = CreateWindow( TEXT("static"), szTemp, WS_CHILD|WS_VISIBLE, x, y, 400,19, hDlg, (HMENU)-1, hInst, NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );

        y += 15;
        wsprintf( szTemp, TEXT("__VERSION__ = %s"),__VERSION__ );
        hCtrl = CreateWindow( TEXT("static"), szTemp, WS_CHILD|WS_VISIBLE, x, y, 400,19, hDlg, (HMENU)-1, hInst, NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );
#endif

#ifdef WIN64
        y += 15;
        wsprintf( szTemp, TEXT("WIN64 = %d"),WIN64 );
        hCtrl = CreateWindow( TEXT("static"), szTemp, WS_CHILD|WS_VISIBLE, x, y, 400,19, hDlg, (HMENU)-1, hInst, NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );
#endif
#ifdef i386
        y += 15;
        wsprintf( szTemp, TEXT("i386 = %d"),i386 );
        hCtrl = CreateWindow( TEXT("static"), szTemp, WS_CHILD|WS_VISIBLE, x, y, 400,19, hDlg, (HMENU)-1, hInst, NULL );
        SendMessage( hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0) );
#endif
        rtn = TRUE;
        break;

    case WM_COMMAND:
        switch( LOWORD(wParam) )
        {
        case IDOK:
            EndDialog(hDlg,TRUE);
            rtn = TRUE;
            break;
        case IDCANCEL:
            EndDialog(hDlg,FALSE);
            rtn = TRUE;
            break;
        default:
            nop();
            break;
        }
        break;
    default:
        nop();
        break;
    }

    return rtn;
}
