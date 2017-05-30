#ifndef PARAM_CTRL_H
#define PARAM_CTRL_H

#define PARAM_CTRL_ID_OFFSET 100

enum
{
    PARAM_CTRL_TARGET_NAME,
    PARAM_CTRL_TARGET_DATA,
    PARAM_CTRL_TARGET_MAX
};
typedef BYTE PARAM_CTRL_TARGET;

typedef enum
{
    PARAM_CTRL_GROUP_SYSTEM_COMMON,
    PARAM_CTRL_GROUP_PATCH_COMMON ,
    PARAM_CTRL_GROUP_PATCH_TONE   ,
    PARAM_CTRL_GROUP_MAX
}PARAM_CTRL_GROUP_ID;

typedef enum
{
    PARAM_CTRL_SYSCMN_SOUNDMODE                   ,
    PARAM_CTRL_PATCHCMN_PATCHNAME             ,
    PARAM_CTRL_PATCHTN_TONESWITCH                 ,

    PARAM_CTRL_MAX
}PARAM_CTRL_ID;

/********************************************************************************
 * 内容  : 登録された全てのパラメータコントロールを生成する
 * 引数  : HWND hwnd 親ウィンドウのハンドラ
 * 戻り値: BOOL
 ***************************************/
BOOL ParamCtrlCreate( HWND hwnd );

/********************************************************************************
 * 内容  : 指定のグループのパラメータコントロールを表示
 * 引数  : PARAM_CTRL_GROUP_ID groupId
 * 戻り値: BOOL
 ***************************************/
BOOL ParamCtrlGroupDisplay( PARAM_CTRL_GROUP_ID groupId );

/********************************************************************************
 * 内容  : ウィンドウハンドラを取得する
 * 引数  : PARAM_CTRL_ID id
 * 戻り値: HWND
 ***************************************/
HWND ParamCtrlGetHWND( PARAM_CTRL_ID id );

#endif /* PARAM_CTRL_H */
