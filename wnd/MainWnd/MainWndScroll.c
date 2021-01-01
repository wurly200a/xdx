/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */

/* �O���֐���` */
#include "DebugWnd.h"

/* �O���ϐ���` */

/* �����֐���` */
void setAllScrollInfo( HWND hwnd );
void getAllScrollInfo( HWND hwnd );
void setScrollPos( HWND hwnd, int fnBar, DWORD nPos );

/* �����ϐ���` */
typedef struct
{
    int    cxClient;  /* �N���C�A���g�̈�T�C�Y(X) */
    int    cyClient;  /* �N���C�A���g�̈�T�C�Y(Y) */
    int    iHorzPos;  /* �X�N���[���o�[�̉��ʒu    */
    int    iVertPos;  /* �X�N���[���o�[�̏c�ʒu    */
} S_MAINWND_SCROLL_DATA;

static S_MAINWND_SCROLL_DATA mainWndScrollData;

#define SCROLL_RANGE_HORZ_MAX 1100
#define SCROLL_RANGE_VERT_MAX  810

/********************************************************************************
 * ���e  : �X�N���[�����̃Z�b�g
 * ����  : HWND hwnd
 * �߂�l: �Ȃ�
 ***************************************/
void
InitMainWndScrollInfo( HWND hwnd )
{

}

/********************************************************************************
 * ���e  : �X�N���[��
 * ����  : HWND hwnd
 * �߂�l: �Ȃ�
 ***************************************/
void
MainWndHscroll( HWND hwnd, WPARAM wParam )
{
    SCROLLINFO si;
    int iHorzPos;
    S_MAINWND_SCROLL_DATA *ptr = &mainWndScrollData;

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
    case SB_THUMBTRACK:
        si.nPos = si.nTrackPos;
        /*si.nPos = HIWORD(wParam);�ł��悢*/
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
        ScrollWindow( hwnd, iHorzPos - si.nPos, 0, NULL, NULL );
        UpdateWindow(hwnd);
    }

    GetScrollInfo( hwnd, SB_HORZ, &si );
    ptr->iHorzPos = si.nPos;
}

/********************************************************************************
 * ���e  : �X�N���[��
 * ����  : HWND hwnd
 * �߂�l: �Ȃ�
 ***************************************/
void
MainWndVscroll( HWND hwnd, WPARAM wParam )
{
    SCROLLINFO si;
    int iVertPos;
    S_MAINWND_SCROLL_DATA *ptr = &mainWndScrollData;

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
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        si.nPos = si.nTrackPos;
        /*si.nPos = HIWORD(wParam);�ł��悢*/
    default:
        break;
    }
    si.fMask = SIF_POS;
    SetScrollInfo(hwnd, SB_VERT, &si, TRUE); /* �X�N���[���o�[���Z�b�g���ĕ`�� */
    GetScrollInfo(hwnd, SB_VERT, &si);       /* �X�N���[���o�[���Ď擾         */

    if( si.nPos != iVertPos )
    {
        ScrollWindow(hwnd, 0, iVertPos - si.nPos,NULL, NULL);
        UpdateWindow(hwnd);
    }

    GetScrollInfo(hwnd, SB_VERT, &si);
    ptr->iVertPos = si.nPos;
}

/********************************************************************************
 * ���e  : �X�N���[�����̃Z�b�g
 * ����  : HWND hwnd
 * ����  : cxClient
 * ����  : cyClient
 * �߂�l: �Ȃ�
 ***************************************/
void
SetMainWndAllScrollInfo( HWND hwnd, int cxClient, int cyClient )
{
    SCROLLINFO si;
    S_MAINWND_SCROLL_DATA *ptr = &mainWndScrollData;

    DebugWndPrintf("SetMainWndAllScrollInfo,%d,%d \r\n",cxClient,cyClient);

    ptr->cxClient = cxClient;
    ptr->cyClient = cyClient;

    /* Set vertical scroll bar range and page size */
    si.cbSize = sizeof(si);
    si.fMask  = SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
    si.nMin   = 0;                                                                /* �͈͂̍ŏ��l */
    si.nMax   = SCROLL_RANGE_VERT_MAX;
    si.nPage  = (ptr->cyClient ); /* �y�[�W�T�C�Y */
    SetScrollInfo( hwnd, SB_VERT, &si, TRUE );

    /* Set horizontal scroll bar range and page size*/
    si.cbSize = sizeof(si);
    si.fMask  = SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
    si.nMin   = 0;
    si.nMax   = SCROLL_RANGE_HORZ_MAX;
    si.nPage  = (ptr->cxClient );
    SetScrollInfo( hwnd, SB_HORZ, &si, TRUE );

}

/********************************************************************************
 * ���e  : �X�N���[�����̎擾
 * ����  : HWND hwnd
 * ����  : *iHorzPosPtr
 * ����  : *iVertPosPtr
 * �߂�l: �Ȃ�
 ***************************************/
void
GetMainWndAllScrollInfo( HWND hwnd, int *iHorzPosPtr, int *iVertPosPtr )
{
    SCROLLINFO si;
    S_MAINWND_SCROLL_DATA *ptr = &mainWndScrollData;

    si.cbSize = sizeof(si);
    si.fMask  = SIF_POS;
    GetScrollInfo( hwnd, SB_VERT, &si ); /* �c�X�N���[���o�[�̈ʒu���擾 */
    ptr->iVertPos = si.nPos;

    GetScrollInfo( hwnd, SB_HORZ, &si);   /* ���X�N���[���o�[�̈ʒu���擾 */
    ptr->iHorzPos = si.nPos;

    *iVertPosPtr = ptr->iVertPos;
    *iHorzPosPtr = ptr->iHorzPos;
}

/********************************************************************************
 * ���e  : �X�N���[���ʒu�̃Z�b�g
 * ����  : HWND hwnd
 * ����  : int   fnBar �X�N���[���o�[�̃^�C�v
 * ����  : DWORD nPos  �ʒu
 * �߂�l: �Ȃ�
 ***************************************/
void
SetMainWndScrollPos( HWND hwnd, int fnBar, DWORD nPos )
{
    SCROLLINFO si;

    si.cbSize = sizeof(si);
    si.fMask  = SIF_POS;
    GetScrollInfo( hwnd, fnBar, &si );
    si.nPos   = nPos;
    SetScrollInfo( hwnd, fnBar, &si, TRUE );
}
