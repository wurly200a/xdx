/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */

typedef struct
{
    LOGFONT logfont;
} S_FONT_DATA;

/* 外部関数定義 */

/* 外部変数定義 */

/* 内部関数定義 */
#include "DebugWndFont.h"

/* 内部変数定義 */
static S_FONT_DATA fontData;

/********************************************************************************
 * 内容  : フォント初期化
 * 引数  : なし
 * 戻り値: なし
 ***************************************/
void
DebugFontInit( void )
{
    GetObject( GetStockObject(SYSTEM_FIXED_FONT), sizeof(LOGFONT), (PTSTR)&(fontData.logfont) );
}

/********************************************************************************
 * 内容  : フォント選択
 * 引数  : HWND hwnd
 * 戻り値: BOOL (FALSE:キャンセルされた)
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
 * 内容  : 論理フォント取得
 * 引数  : なし
 * 戻り値: LOGFONT *
 ***************************************/
LOGFONT *
DebugFontGetLogFont( void )
{
    LOGFONT *rtnPtr = NULL;

    rtnPtr = &(fontData.logfont);

    return rtnPtr;
}
