#ifndef CONFIG_H

/* 管理する設定のID */
typedef enum
{
    CONFIG_ID_WINDOW_POS_X    ,
    CONFIG_ID_WINDOW_POS_Y    ,
    CONFIG_ID_WINDOW_POS_DX   ,
    CONFIG_ID_WINDOW_POS_DY   ,
    CONFIG_ID_MIDI_IN_NUM     ,
    CONFIG_ID_MIDI_OUT_NUM    ,
    CONFIG_ID_MIDI_KEY_IN_NUM ,
    CONFIG_ID_MAX
}CONFIG_ID;

/********************************************************************************
 * 内容  : 設定管理モジュールの初期化
 * 引数  : HINSTANCE hInst
 * 引数  : PTSTR szAppName
 * 戻り値: なし
 ***************************************/
void ConfigInit( HINSTANCE hInst, PTSTR szAppName );

/********************************************************************************
 * 内容  : DWORD設定値を保存する
 * 引数  : CONFIG_ID id
 * 引数  : DWORD     data
 * 戻り値: なし
 ***************************************/
void ConfigSaveDword( CONFIG_ID id, DWORD data );

/********************************************************************************
 * 内容  : DWORD設定値を読み込む
 * 引数  : CONFIG_ID id
 * 戻り値: なし
 ***************************************/
INT ConfigLoadDword( CONFIG_ID id );

/**
 * ------------------------------------------------------------------------------
 * @brief  文字列設定値を保存する
 * @param  id
 * @param  ptstrValue
 * @return なし
 * @date 2017年01月04日 13時50分25秒
 **/
void ConfigSaveString( CONFIG_ID id, PTSTR ptstrValue );

/**
 * ------------------------------------------------------------------------------
 * @brief  文字列設定値を読み込む
 * @param  id
 * @return PTSTR
 * @date 2017年01月04日 13時50分25秒
 **/
PTSTR ConfigLoadString( CONFIG_ID id );

/**
 * ------------------------------------------------------------------------------
 * @brief  設定値を保存する
 * @param  start_id
 * @param  end_id
 * @return なし
 * @date 2017年01月04日 13時50分25秒
 **/
PTSTR ConfigWrite( CONFIG_ID start_id, CONFIG_ID end_id );


/**
 * ------------------------------------------------------------------------------
 * @brief  設定値が変更されていないかチェックする
 * @param  start_id
 * @param  end_id
 * @return
 * @date 2017年01月04日 13時50分25秒
 **/
BOOL ConfigIsSame( CONFIG_ID start_id, CONFIG_ID end_id );

/********************************************************************************
 * 内容  : デバッグ変数(DWORD値)を読み込む
 * 引数  : なし
 * 戻り値: INT
 ***************************************/
DWORD ConfigLoadDebugValue( void );

#define CONFIG_H
#endif /* CONFIG_H */
