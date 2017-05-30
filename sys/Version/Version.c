/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */

/* 外部関数定義 */
#include "WinMain.h"

/* 外部変数定義 */
/* 内部関数定義 */
#include "Version.h"
/* 内部変数定義 */
static WORD versionData = 0x0001;
static TCHAR szVersion[64];

/********************************************************************************
 * 内容  : バージョン文字列取得
 * 引数  : なし
 * 戻り値: PTSTR
 ***************************************/
PTSTR
VersionStringGet( void )
{
    wsprintf( szVersion, TEXT("%01x.%02x (%s)"),HIBYTE(versionData),LOBYTE(versionData),__DATE__ );

    return szVersion;
}
