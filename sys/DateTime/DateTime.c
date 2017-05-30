/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */

/* 外部関数定義 */
#include "WinMain.h"

/* 外部変数定義 */
/* 内部関数定義 */
#include "DateTime.h"
/* 内部変数定義 */
static TCHAR szDateTime[64];

/********************************************************************************
 * 内容  : 現在の日付時刻の文字列を取得する
 * 引数  : なし
 * 戻り値: PTSTR
 ***************************************/
PTSTR
DateTimeGetString( void )
{
    SYSTEMTIME systime;

    GetLocalTime(&systime);
    wsprintf( szDateTime, "%d:%02d %04d/%02d/%02d",systime.wHour,systime.wMinute,systime.wYear,systime.wMonth,systime.wDay );

    return szDateTime;
}
