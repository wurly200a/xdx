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
    PARAM_CTRL_GROUP_1VOICE       ,
    PARAM_CTRL_GROUP_ALL_VOICE    ,
    PARAM_CTRL_GROUP_MAX
}PARAM_CTRL_GROUP_ID;

typedef enum
{
    PARAM_CTRL_SYSCMN_SOUNDMODE                   ,

    PARAM_CTRL_VOICE_00                        ,
    PARAM_CTRL_VOICE_01                        ,
    PARAM_CTRL_VOICE_02                        ,
    PARAM_CTRL_VOICE_03                        ,
    PARAM_CTRL_VOICE_04                        ,
    PARAM_CTRL_VOICE_05                        ,
    PARAM_CTRL_VOICE_06                        ,
    PARAM_CTRL_VOICE_07                        ,
    PARAM_CTRL_VOICE_08                        ,
    PARAM_CTRL_VOICE_09                        ,
    PARAM_CTRL_VOICE_10                        ,
    PARAM_CTRL_VOICE_11                        ,
    PARAM_CTRL_VOICE_12                        ,
    PARAM_CTRL_VOICE_13                        ,
    PARAM_CTRL_VOICE_14                        ,
    PARAM_CTRL_VOICE_15                        ,
    PARAM_CTRL_VOICE_16                        ,
    PARAM_CTRL_VOICE_17                        ,
    PARAM_CTRL_VOICE_18                        ,
    PARAM_CTRL_VOICE_19                        ,
    PARAM_CTRL_VOICE_20                        ,
    PARAM_CTRL_VOICE_21                        ,
    PARAM_CTRL_VOICE_22                        ,
    PARAM_CTRL_VOICE_23                        ,
    PARAM_CTRL_VOICE_24                        ,
    PARAM_CTRL_VOICE_25                        ,
    PARAM_CTRL_VOICE_26                        ,
    PARAM_CTRL_VOICE_27                        ,
    PARAM_CTRL_VOICE_28                        ,
    PARAM_CTRL_VOICE_29                        ,
    PARAM_CTRL_VOICE_30                        ,
    PARAM_CTRL_VOICE_31                        ,
    PARAM_CTRL_VOICE_32                        ,
    PARAM_CTRL_VOICE_33                        ,
    PARAM_CTRL_VOICE_34                        ,
    PARAM_CTRL_VOICE_35                        ,
    PARAM_CTRL_VOICE_36                        ,
    PARAM_CTRL_VOICE_37                        ,
    PARAM_CTRL_VOICE_38                        ,
    PARAM_CTRL_VOICE_39                        ,
    PARAM_CTRL_VOICE_40                        ,
    PARAM_CTRL_VOICE_41                        ,
    PARAM_CTRL_VOICE_42                        ,
    PARAM_CTRL_VOICE_43                        ,
    PARAM_CTRL_VOICE_44                        ,
    PARAM_CTRL_VOICE_45                        ,
    PARAM_CTRL_VOICE_46                        ,
    PARAM_CTRL_VOICE_47                        ,
    PARAM_CTRL_VOICE_48                        ,
    PARAM_CTRL_VOICE_49                        ,
    PARAM_CTRL_VOICE_50                        ,
    PARAM_CTRL_VOICE_51                        ,
    PARAM_CTRL_VOICE_52                        ,
    PARAM_CTRL_VOICE_53                        ,
    PARAM_CTRL_VOICE_54                        ,
    PARAM_CTRL_VOICE_55                        ,
    PARAM_CTRL_VOICE_56                        ,
    PARAM_CTRL_VOICE_57                        ,
    PARAM_CTRL_VOICE_58                        ,
    PARAM_CTRL_VOICE_59                        ,
    PARAM_CTRL_VOICE_60                        ,
    PARAM_CTRL_VOICE_61                        ,
    PARAM_CTRL_VOICE_62                        ,
    PARAM_CTRL_VOICE_63                        ,
    PARAM_CTRL_VOICE_64                        ,
    PARAM_CTRL_VOICE_65                        ,
    PARAM_CTRL_VOICE_66                        ,
    PARAM_CTRL_VOICE_67                        ,
    PARAM_CTRL_VOICE_68                        ,
    PARAM_CTRL_VOICE_69                        ,
    PARAM_CTRL_VOICE_70                        ,
    PARAM_CTRL_VOICE_71                        ,
    PARAM_CTRL_VOICE_72                        ,
    PARAM_CTRL_VOICE_73                        ,
    PARAM_CTRL_VOICE_74                        ,
    PARAM_CTRL_VOICE_75                        ,
    PARAM_CTRL_VOICE_76                        ,
    PARAM_CTRL_VOICE_NAME/*77-86*/             ,
    PARAM_CTRL_VOICE_87                        ,
    PARAM_CTRL_VOICE_88                        ,
    PARAM_CTRL_VOICE_89                        ,
    PARAM_CTRL_VOICE_90                        ,
    PARAM_CTRL_VOICE_91                        ,
    PARAM_CTRL_VOICE_92                        ,

    PARAM_CTRL_ALL_VOICE_NAME_00               ,
    PARAM_CTRL_ALL_VOICE_NAME_01               ,
    PARAM_CTRL_ALL_VOICE_NAME_02               ,
    PARAM_CTRL_ALL_VOICE_NAME_03               ,
    PARAM_CTRL_ALL_VOICE_NAME_04               ,
    PARAM_CTRL_ALL_VOICE_NAME_05               ,
    PARAM_CTRL_ALL_VOICE_NAME_06               ,
    PARAM_CTRL_ALL_VOICE_NAME_07               ,
    PARAM_CTRL_ALL_VOICE_NAME_08               ,
    PARAM_CTRL_ALL_VOICE_NAME_09               ,
    PARAM_CTRL_ALL_VOICE_NAME_10               ,
    PARAM_CTRL_ALL_VOICE_NAME_11               ,
    PARAM_CTRL_ALL_VOICE_NAME_12               ,
    PARAM_CTRL_ALL_VOICE_NAME_13               ,
    PARAM_CTRL_ALL_VOICE_NAME_14               ,
    PARAM_CTRL_ALL_VOICE_NAME_15               ,
    PARAM_CTRL_ALL_VOICE_NAME_16               ,
    PARAM_CTRL_ALL_VOICE_NAME_17               ,
    PARAM_CTRL_ALL_VOICE_NAME_18               ,
    PARAM_CTRL_ALL_VOICE_NAME_19               ,
    PARAM_CTRL_ALL_VOICE_NAME_20               ,
    PARAM_CTRL_ALL_VOICE_NAME_21               ,
    PARAM_CTRL_ALL_VOICE_NAME_22               ,
    PARAM_CTRL_ALL_VOICE_NAME_23               ,

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

/********************************************************************************
 * 内容  : ウィンドウからテキストを取得する
 * 引数  : PARAM_CTRL_ID id
 * 引数  : PTSTR ptstrText
 * 戻り値: BOOL
 ***************************************/
BOOL ParamCtrlGetText( PARAM_CTRL_ID id, PTSTR ptstrText );

#endif /* PARAM_CTRL_H */
