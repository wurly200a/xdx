/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */

/* �O���֐���` */
#include "WinMain.h"

/* �O���ϐ���` */
/* �����֐���` */
#include "DateTime.h"
/* �����ϐ���` */
static TCHAR szDateTime[64];

/********************************************************************************
 * ���e  : ���݂̓��t�����̕�������擾����
 * ����  : �Ȃ�
 * �߂�l: PTSTR
 ***************************************/
PTSTR
DateTimeGetString( void )
{
    SYSTEMTIME systime;

    GetLocalTime(&systime);
    wsprintf( szDateTime, "%d:%02d %04d/%02d/%02d",systime.wHour,systime.wMinute,systime.wYear,systime.wMonth,systime.wDay );

    return szDateTime;
}
