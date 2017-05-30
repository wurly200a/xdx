#ifndef DEBUG_CONFIG

/* 管理する設定のID */
typedef enum
{
   DEBUG_CONFIG_ID_WINDOW_POS_X,
   DEBUG_CONFIG_ID_WINDOW_POS_Y,
   DEBUG_CONFIG_ID_WINDOW_POS_DX,
   DEBUG_CONFIG_ID_WINDOW_POS_DY,
   DEBUG_CONFIG_ID_MAX
}DEBUG_CONFIG_ID;

/********************************************************************************
 * 内容  : 設定管理モジュールの初期化
 * 引数  : なし
 * 戻り値: なし
 ***************************************/
void DebugConfigInit( void );

/********************************************************************************
 * 内容  : DWORD設定値を保存する
 * 引数  : DEBUG_CONFIG_ID id
 * 引数  : DWORD     data
 * 戻り値: なし
 ***************************************/
void DebugConfigSaveDword(DEBUG_CONFIG_ID id, DWORD data );

/********************************************************************************
 * 内容  : DWORD設定値を読み込む
 * 引数  : DEBUG_CONFIG_ID id
 * 戻り値: なし
 ***************************************/
INT DebugConfigLoadDword(DEBUG_CONFIG_ID id );

#define DEBUG_CONFIG
#endif /* DEBUG_CONFIG */
