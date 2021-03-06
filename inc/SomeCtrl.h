#ifndef SOME_CTRL_H

typedef enum
{
    SOME_CTRL_GROUP_DISABLE  ,
    SOME_CTRL_GROUP_ALWAYS   ,
    SOME_CTRL_GROUP_1VOICE   ,
    SOME_CTRL_GROUP_ALL_VOICE,
    SOME_CTRL_GROUP_SYSTEM   ,
    SOME_CTRL_GROUP_MAX
}SOME_CTRL_GROUP_ID;

typedef enum
{
    SOME_CTRL_DEVICE_SELECT_STATIC   ,
    SOME_CTRL_DEVICE_SELECT          ,
    SOME_CTRL_GROUP_MIDI             ,
    SOME_CTRL_MIDI_IN_STATIC         ,
    SOME_CTRL_MIDI_IN                ,
    SOME_CTRL_MIDI_IN_OPEN_BUTTON    ,
    SOME_CTRL_MIDI_OUT_STATIC        ,
    SOME_CTRL_MIDI_OUT               ,
    SOME_CTRL_MIDI_OUT_OPEN_BUTTON   ,
    SOME_CTRL_MIDI_KEY_IN_STATIC     ,
    SOME_CTRL_MIDI_KEY_IN            ,
    SOME_CTRL_MIDI_KEY_IN_OPEN_BUTTON,
    SOME_CTRL_DEBUG_BUTTON           ,

    SOME_CTRL_GROUP_BOX_MODE         ,
    SOME_CTRL_MODE_1VOICE            ,
    SOME_CTRL_MODE_ALL_VOICE         ,
    SOME_CTRL_MODE_SYSTEM            ,

    SOME_CTRL_GROUP_BOX_VOICE        ,
    SOME_CTRL_VOICE_LOAD_BUTTON      ,
    SOME_CTRL_VOICE_SAVE_BUTTON      ,
    SOME_CTRL_VOICE_GET_BUTTON       ,
    SOME_CTRL_VOICE_SET_BUTTON       ,

    SOME_CTRL_GROUP_BOX_ALL_VOICE    ,
    SOME_CTRL_ALL_VOICE_LOAD_BUTTON  ,
    SOME_CTRL_ALL_VOICE_SAVE_BUTTON  ,
    SOME_CTRL_ALL_VOICE_GET_BUTTON   ,
    SOME_CTRL_ALL_VOICE_SET_BUTTON   ,

    SOME_CTRL_GROUP_BOX_SYSTEM       ,
    SOME_CTRL_SYSTEM_GET_BUTTON      ,
    SOME_CTRL_SYSTEM_SET_BUTTON      ,

    SOME_CTRL_MAX
}SOME_CTRL_ID;

#define SOME_CTRL_ID_OFFSET 10
#define MY_EX_STYLE (WS_EX_OVERLAPPEDWINDOW)

/********************************************************************************
 * 内容  : 登録された全てのコントロールを生成する
 * 引数  : HINSTANCE hInst
 * 引数  : PTSTR szAppName
 * 引数  : HWND hwnd 親ウィンドウのハンドラ
 * 戻り値: BOOL
 ***************************************/
BOOL SomeCtrlCreate( HINSTANCE hInst, PTSTR szAppName, HWND hwnd );

/********************************************************************************
 * 内容  : 指定のグループのコントロールを表示する
 * 引数  : SOME_CTRL_GROUP_ID groupId
 * 戻り値: BOOL
 ***************************************/
BOOL SomeCtrlGroupDisplay( SOME_CTRL_GROUP_ID groupId );

/********************************************************************************
 * 内容  : グループIDの取得
 * 引数  : なし
 * 戻り値: SOME_CTRL_GROUP_ID
 ***************************************/
SOME_CTRL_GROUP_ID SomeCtrlGetNowGroupId( void );

/********************************************************************************
 * 内容  : 登録された全てのコントロールのサイズを調整する
 * 引数  : int cxClient
 * 引数  : int cyClient
 * 戻り値: LONG コントロールを配置する矩形の縦方向のサイズ
 ***************************************/
LONG SomeCtrlSize( int cxClient,int cyClient );

/********************************************************************************
 * 内容  : コントロールのウィンドウハンドラを取得する
 * 引数  : SOME_CTRL_ID id
 * 戻り値: HWND
 ***************************************/
HWND SomeCtrlGetHWND( SOME_CTRL_ID id );

/********************************************************************************
 * 内容  : コントロールを有効にする
 * 引数  : SOME_CTRL_ID id
 * 戻り値: なし
 ***************************************/
void SomeCtrlEnable( SOME_CTRL_ID id );

/********************************************************************************
 * 内容  : コントロールを無効にする
 * 引数  : SOME_CTRL_ID id
 * 戻り値: なし
 ***************************************/
void SomeCtrlDisable( SOME_CTRL_ID id );

/********************************************************************************
 * 内容  : コントロールからテキストを取得する
 * 引数  : SOME_CTRL_ID id
 * 引数  : PTSTR ptstrText
 * 戻り値: BOOL
 ***************************************/
BOOL SomeCtrlGetText( SOME_CTRL_ID id, PTSTR ptstrText );

#define SOME_CTRL_H
#endif /* SOME_CTRL_H */
