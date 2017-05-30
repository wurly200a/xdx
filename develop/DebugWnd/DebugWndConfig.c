/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */

/* 外部関数定義 */

/* 外部変数定義 */
/* 内部関数定義 */
#include "DebugWndConfig.h"
/* 内部変数定義 */
static BOOL bInitDebugConfig;
static TCHAR szIniFileName[512];

typedef struct
{
    PTSTR         pKeyName;    /* キー名 */
    PTSTR         pInitValue;  /* 初期値 */
} S_DEBUG_CONFIG_INFO;

static S_DEBUG_CONFIG_INFO configInfoTbl[DEBUG_CONFIG_ID_MAX] =
{
    { TEXT("iWindowPosX" ),TEXT("0x00000000") },
    { TEXT("iWindowPosY" ),TEXT("0x00000000") },
    { TEXT("iWindowPosDX"),TEXT("0x000001F4") }, /* 500 */
    { TEXT("iWindowPosDY"),TEXT("0x0000012C") }, /* 300 */
};

static const TCHAR szDebugProfileName[] = TEXT("Debug");

/********************************************************************************
 * 内容  : 設定管理モジュールの初期化
 * 引数  : なし
 * 戻り値: なし
 ***************************************/
void
DebugConfigInit( void )
{
    DWORD length;

    bInitDebugConfig = TRUE;
    length = GetModuleFileName(NULL,szIniFileName,512);

    szIniFileName[length-4] = '_';
    szIniFileName[length-3] = 'd';
    szIniFileName[length-2] = 'e';
    szIniFileName[length-1] = 'b';
    szIniFileName[length-0] = 'u';
    szIniFileName[length+1] = 'g';
    szIniFileName[length+2] = '.';
    szIniFileName[length+3] = 'i';
    szIniFileName[length+4] = 'n';
    szIniFileName[length+5] = 'i';

}

/********************************************************************************
 * 内容  : DWORD設定値を保存する
 * 引数  : DEBUG_CONFIG_ID id
 * 引数  : DWORD     data
 * 戻り値: なし
 ***************************************/
void
DebugConfigSaveDword( DEBUG_CONFIG_ID id, DWORD data )
{
    TCHAR szDword[11];

    if( bInitDebugConfig )
    {
        if( id < DEBUG_CONFIG_ID_MAX )
        {
            wsprintf( szDword, "0x%08lX", data );
            WritePrivateProfileString( szDebugProfileName, configInfoTbl[id].pKeyName, szDword, szIniFileName );
        }
        else
        {
            nop();
        }
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * 内容  : DWORD設定値を読み込む
 * 引数  : DEBUG_CONFIG_ID id
 * 戻り値: なし
 ***************************************/
INT
DebugConfigLoadDword( DEBUG_CONFIG_ID id )
{
    TCHAR szDword[11];
    DWORD rtn = (DWORD)0;

    if( bInitDebugConfig )
    {
        if( id < DEBUG_CONFIG_ID_MAX )
        {
            GetPrivateProfileString( szDebugProfileName, configInfoTbl[id].pKeyName, configInfoTbl[id].pInitValue, szDword, 11, szIniFileName );
            rtn = strtol( szDword+2,NULL,16 );
        }
        else
        {
            nop();
        }
    }
    else
    {
        nop();
    }

    return (DWORD)rtn;
}
