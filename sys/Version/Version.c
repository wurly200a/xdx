/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */

/* �O���֐���` */
#include "WinMain.h"

/* �O���ϐ���` */
/* �����֐���` */
#include "Version.h"
/* �����ϐ���` */
static WORD versionData = 0x0310;
static TCHAR szVersion[64];

/********************************************************************************
 * ���e  : �o�[�W����������擾
 * ����  : �Ȃ�
 * �߂�l: PTSTR
 ***************************************/
PTSTR
VersionStringGet( void )
{
    wsprintf( szVersion, TEXT("%01x.%02x (%s)"),HIBYTE(versionData),LOBYTE(versionData),__DATE__ );

    return szVersion;
}
