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
#include "Font.h"

/* 内部変数定義 */
static S_FONT_DATA fontData[FONT_ID_MAX];

/********************************************************************************
 * 内容  : フォント初期化
 * 引数  : なし
 * 戻り値: なし
 ***************************************/
void
FontInit( void )
{
    INT i;

    for( i=0; i<FONT_ID_MAX; i++ )
    {
        GetObject( GetStockObject(SYSTEM_FIXED_FONT), sizeof(LOGFONT), (PTSTR)&(fontData[i].logfont) );
    }
}

/********************************************************************************
 * 内容  : フォント選択
 * 引数  : HWND hwnd
 * 引数  : FONT_ID id
 * 戻り値: BOOL (FALSE:キャンセルされた)
 ***************************************/
BOOL
FontChooseFont( HWND hwnd, FONT_ID id )
{
    BOOL rtn = FALSE;
    CHOOSEFONT cf;

    if( id < FONT_ID_MAX )
    {
        cf.lStructSize    = sizeof(CHOOSEFONT);
        cf.hwndOwner      = hwnd;
        cf.hDC            = NULL;
        cf.lpLogFont      = &(fontData[id].logfont);
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
    }
    else
    {
        nop();
    }

    return rtn;
}

/********************************************************************************
 * 内容  : 論理フォント取得
 * 引数  : FONT_ID id
 * 戻り値: LOGFONT *
 ***************************************/
LOGFONT *
FontGetLogFont( FONT_ID id )
{
    LOGFONT *rtnPtr = NULL;

    if( id < FONT_ID_MAX )
    {
        rtnPtr = &(fontData[id].logfont);
    }
    else
    {
        nop();
    }

    return rtnPtr;
}
