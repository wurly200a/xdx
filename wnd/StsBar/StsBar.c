/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */

/* �O���֐���` */
#include "WinMain.h"

/* �O���ϐ���` */

/* �����֐���` */
#include "StsBar.h"

/* �����ϐ���` */
static HWND hwndSbar;
static int sbarColWidth[STS_BAR_MAX+1];
static BOOL fSbarEnable;

/********************************************************************************
 * ���e  : �X�e�[�^�X�o�[����
 * ����  : HWND hwnd �e�E�B���h�E�̃n���h��
 * ����  : BOOL fShow �f�t�H���g�\�����邩�ۂ�
 * �߂�l: HWND
 ***************************************/
HWND
StsBarCreate( HWND hwnd, BOOL fShow )
{
    hwndSbar = CreateWindowEx(0,
                              STATUSCLASSNAME, NULL,
                              WS_CHILD | SBS_SIZEGRIP | WS_CLIPSIBLINGS | SBT_NOBORDERS,
                              0, 0, 0, 0,
                              hwnd, (HMENU)1500, GetHinst(), NULL);
    if( hwndSbar != NULL )
    {
        if( fShow )
        {
            StsBarShowWindow( TRUE );
        }
        else
        {
            StsBarShowWindow( FALSE );
        }
    }
    else
    {
        nop();
    }

    return hwndSbar;
}

/********************************************************************************
 * ���e  : �X�e�[�^�X�o�[�̃T�C�Y����
 * ����  : int cxClient
 * ����  : int cyClient
 * �߂�l: LONG �c�����̃T�C�Y
 ***************************************/
LONG
StsBarSize( int cxClient,int cyClient )
{
    RECT RectSbar;
    LONG vertSize = 0;

    sbarColWidth[STS_BAR_0]   = (cxClient/4);
    sbarColWidth[STS_BAR_1]   = cxClient - (cxClient/4);
    sbarColWidth[STS_BAR_MAX] = -1;

    SendMessage(hwndSbar, SB_SETPARTS, sizeof(sbarColWidth)/sizeof(int), (LPARAM)sbarColWidth);
    SendMessage(hwndSbar, WM_SIZE, SIZE_RESTORED, MAKELPARAM(cxClient, cyClient));
    GetClientRect(hwndSbar, &RectSbar);

    if( fSbarEnable )
    {
        vertSize = RectSbar.bottom - RectSbar.top;
    }
    else
    {
        nop();
    }

    return vertSize;
}

/********************************************************************************
 * ���e  : �X�e�[�^�X�o�[�ւ̕�����Z�b�g
 * ����  : STS_BAR_ID id
 * ����  : PTSTR ptstrFormat, ...
 * �߂�l: �Ȃ�
 ***************************************/
void
StsBarSetText( STS_BAR_ID id, PTSTR ptstrFormat, ... )
{
    TCHAR szBuf[50];
    RECT  RectSbar;
    va_list vaArgs;

    va_start(vaArgs, ptstrFormat);
    if( wvsprintf(szBuf, ptstrFormat, vaArgs) != EOF )
    {
        SendMessage(hwndSbar, SB_SETTEXT, id, (LPARAM)szBuf);
        GetClientRect(hwndSbar, &RectSbar);
    }
    else
    {
        /* do nothing */
    }
    va_end(vaArgs);
}

/********************************************************************************
 * ���e  : �X�e�[�^�X�o�[�̕\��
 * ����  : BOOL fShow
 * �߂�l: BOOL
 ***************************************/
BOOL
StsBarShowWindow( BOOL fShow )
{
    BOOL rtn = FALSE;

    if( hwndSbar != NULL )
    {
        if( fShow )
        {
            fSbarEnable = TRUE;
            rtn = ShowWindow(hwndSbar, SW_SHOW);
        }
        else
        {
            fSbarEnable = FALSE;
            rtn = ShowWindow(hwndSbar, SW_HIDE);
        }
    }
    else
    {
        nop();
    }

    return rtn;
}
