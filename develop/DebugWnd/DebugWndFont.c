/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */

typedef struct
{
    LOGFONT logfont;
} S_FONT_DATA;

/* �O���֐���` */

/* �O���ϐ���` */

/* �����֐���` */
#include "DebugWndFont.h"

/* �����ϐ���` */
static S_FONT_DATA fontData;

/********************************************************************************
 * ���e  : �t�H���g������
 * ����  : �Ȃ�
 * �߂�l: �Ȃ�
 ***************************************/
void
DebugFontInit( void )
{
    GetObject( GetStockObject(SYSTEM_FIXED_FONT), sizeof(LOGFONT), (PTSTR)&(fontData.logfont) );
}

/********************************************************************************
 * ���e  : �t�H���g�I��
 * ����  : HWND hwnd
 * �߂�l: BOOL (FALSE:�L�����Z�����ꂽ)
 ***************************************/
BOOL
DebugFontChooseFont( HWND hwnd )
{
    BOOL rtn = FALSE;
    CHOOSEFONT cf;

    cf.lStructSize    = sizeof(CHOOSEFONT);
    cf.hwndOwner      = hwnd;
    cf.hDC            = NULL;
    cf.lpLogFont      = &(fontData.logfont);
    cf.iPointSize     = 0;
    cf.Flags          = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_FIXEDPITCHONLY/*| CF_EFFECTS */;
    cf.rgbColors      = 0;
    cf.lCustData      = 0;
    cf.lpfnHook       = NULL;
    cf.lpTemplateName = NULL;
    cf.hInstance      = NULL;
    cf.lpszStyle      = NULL;
    cf.nFontType      = 0;
    cf.nSizeMin       = 0;
    cf.nSizeMax       = 0;

    rtn = ChooseFont(&cf);

    return rtn;
}

/********************************************************************************
 * ���e  : �_���t�H���g�擾
 * ����  : �Ȃ�
 * �߂�l: LOGFONT *
 ***************************************/
LOGFONT *
DebugFontGetLogFont( void )
{
    LOGFONT *rtnPtr = NULL;

    rtnPtr = &(fontData.logfont);

    return rtnPtr;
}
