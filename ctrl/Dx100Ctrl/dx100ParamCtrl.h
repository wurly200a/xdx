#ifndef DX100_PARAM_CTRL_H
#define DX100_PARAM_CTRL_H

#define DX100_PARAM_CTRL_ID_OFFSET 100

typedef enum
{
    DX100_PARAM_CTRL_GROUP_SYSTEM_COMMON,
    DX100_PARAM_CTRL_GROUP_1VOICE       ,
    DX100_PARAM_CTRL_GROUP_ALL_VOICE    ,
    DX100_PARAM_CTRL_GROUP_MAX
}DX100_PARAM_CTRL_GROUP_ID;

typedef enum
{
    DX100_PARAM_CTRL_SYSCMN_SOUNDMODE                ,

    DX100_PARAM_CTRL_VOICE_00                        ,DX100_PARAM_CTRL_1VOICE_START=DX100_PARAM_CTRL_VOICE_00,
    DX100_PARAM_CTRL_VOICE_01                        ,
    DX100_PARAM_CTRL_VOICE_02                        ,
    DX100_PARAM_CTRL_VOICE_03                        ,
    DX100_PARAM_CTRL_VOICE_04                        ,
    DX100_PARAM_CTRL_VOICE_05                        ,
    DX100_PARAM_CTRL_VOICE_06                        ,
    DX100_PARAM_CTRL_VOICE_07                        ,
    DX100_PARAM_CTRL_VOICE_08                        ,
    DX100_PARAM_CTRL_VOICE_09                        ,
    DX100_PARAM_CTRL_VOICE_10                        ,
    DX100_PARAM_CTRL_VOICE_11                        ,
    DX100_PARAM_CTRL_VOICE_12                        ,
    DX100_PARAM_CTRL_VOICE_13                        ,
    DX100_PARAM_CTRL_VOICE_14                        ,
    DX100_PARAM_CTRL_VOICE_15                        ,
    DX100_PARAM_CTRL_VOICE_16                        ,
    DX100_PARAM_CTRL_VOICE_17                        ,
    DX100_PARAM_CTRL_VOICE_18                        ,
    DX100_PARAM_CTRL_VOICE_19                        ,
    DX100_PARAM_CTRL_VOICE_20                        ,
    DX100_PARAM_CTRL_VOICE_21                        ,
    DX100_PARAM_CTRL_VOICE_22                        ,
    DX100_PARAM_CTRL_VOICE_23                        ,
    DX100_PARAM_CTRL_VOICE_24                        ,
    DX100_PARAM_CTRL_VOICE_25                        ,
    DX100_PARAM_CTRL_VOICE_26                        ,
    DX100_PARAM_CTRL_VOICE_27                        ,
    DX100_PARAM_CTRL_VOICE_28                        ,
    DX100_PARAM_CTRL_VOICE_29                        ,
    DX100_PARAM_CTRL_VOICE_30                        ,
    DX100_PARAM_CTRL_VOICE_31                        ,
    DX100_PARAM_CTRL_VOICE_32                        ,
    DX100_PARAM_CTRL_VOICE_33                        ,
    DX100_PARAM_CTRL_VOICE_34                        ,
    DX100_PARAM_CTRL_VOICE_35                        ,
    DX100_PARAM_CTRL_VOICE_36                        ,
    DX100_PARAM_CTRL_VOICE_37                        ,
    DX100_PARAM_CTRL_VOICE_38                        ,
    DX100_PARAM_CTRL_VOICE_39                        ,
    DX100_PARAM_CTRL_VOICE_40                        ,
    DX100_PARAM_CTRL_VOICE_41                        ,
    DX100_PARAM_CTRL_VOICE_42                        ,
    DX100_PARAM_CTRL_VOICE_43                        ,
    DX100_PARAM_CTRL_VOICE_44                        ,
    DX100_PARAM_CTRL_VOICE_45                        ,
    DX100_PARAM_CTRL_VOICE_46                        ,
    DX100_PARAM_CTRL_VOICE_47                        ,
    DX100_PARAM_CTRL_VOICE_48                        ,
    DX100_PARAM_CTRL_VOICE_49                        ,
    DX100_PARAM_CTRL_VOICE_50                        ,
    DX100_PARAM_CTRL_VOICE_51                        ,
    DX100_PARAM_CTRL_VOICE_52                        ,
    DX100_PARAM_CTRL_VOICE_53                        ,
    DX100_PARAM_CTRL_VOICE_54                        ,
    DX100_PARAM_CTRL_VOICE_55                        ,
    DX100_PARAM_CTRL_VOICE_56                        ,
    DX100_PARAM_CTRL_VOICE_57                        ,
    DX100_PARAM_CTRL_VOICE_58                        ,
    DX100_PARAM_CTRL_VOICE_59                        ,
    DX100_PARAM_CTRL_VOICE_60                        ,
    DX100_PARAM_CTRL_VOICE_61                        ,
    DX100_PARAM_CTRL_VOICE_62                        ,
    DX100_PARAM_CTRL_VOICE_63                        ,
    DX100_PARAM_CTRL_VOICE_64                        ,
    DX100_PARAM_CTRL_VOICE_65                        ,
    DX100_PARAM_CTRL_VOICE_66                        ,
    DX100_PARAM_CTRL_VOICE_67                        ,
    DX100_PARAM_CTRL_VOICE_68                        ,
    DX100_PARAM_CTRL_VOICE_69                        ,
    DX100_PARAM_CTRL_VOICE_70                        ,
    DX100_PARAM_CTRL_VOICE_71                        ,
    DX100_PARAM_CTRL_VOICE_72                        ,
    DX100_PARAM_CTRL_VOICE_73                        ,
    DX100_PARAM_CTRL_VOICE_74                        ,
    DX100_PARAM_CTRL_VOICE_75                        ,
    DX100_PARAM_CTRL_VOICE_76                        ,
    DX100_PARAM_CTRL_VOICE_NAME/*77-86*/             ,
    DX100_PARAM_CTRL_VOICE_87                        ,
    DX100_PARAM_CTRL_VOICE_88                        ,
    DX100_PARAM_CTRL_VOICE_89                        ,
    DX100_PARAM_CTRL_VOICE_90                        ,
    DX100_PARAM_CTRL_VOICE_91                        ,
    DX100_PARAM_CTRL_VOICE_92                        ,
    DX100_PARAM_CTRL_STATIC_1                        ,
    DX100_PARAM_CTRL_STATIC_2                        ,
    DX100_PARAM_CTRL_STATIC_3                        ,
    DX100_PARAM_CTRL_STATIC_4                        ,
    DX100_PARAM_CTRL_STATIC_1_2                      ,
    DX100_PARAM_CTRL_STATIC_2_2                      ,
    DX100_PARAM_CTRL_STATIC_3_2                      ,
    DX100_PARAM_CTRL_STATIC_4_2                      ,
    DX100_PARAM_CTRL_STATIC_PATCHNAME                ,
    DX100_PARAM_CTRL_STATIC_LFO                      ,
    DX100_PARAM_CTRL_STATIC_MOD_SENS                 ,
    DX100_PARAM_CTRL_STATIC_KEY                      ,
    DX100_PARAM_CTRL_STATIC_ALGORITHM                ,
    DX100_PARAM_CTRL_STATIC_FEEDBACK                 ,
    DX100_PARAM_CTRL_STATIC_LFO_WAVE                 ,
    DX100_PARAM_CTRL_STATIC_SPEED                    ,
    DX100_PARAM_CTRL_STATIC_DELAY                    ,
    DX100_PARAM_CTRL_STATIC_PMD                      ,
    DX100_PARAM_CTRL_STATIC_AMD                      ,
    DX100_PARAM_CTRL_STATIC_LFO_SYNC                 ,
    DX100_PARAM_CTRL_STATIC_PITCH                    ,
    DX100_PARAM_CTRL_STATIC_AMPLITUDE                ,
    DX100_PARAM_CTRL_STATIC_AME                      ,
    DX100_PARAM_CTRL_STATIC_EG_BIAS                  ,
    DX100_PARAM_CTRL_STATIC_VELOCITY                 ,
    DX100_PARAM_CTRL_STATIC_OSCILLATOR               ,
    DX100_PARAM_CTRL_STATIC_ENVELOPE_GENERATOR       ,
    DX100_PARAM_CTRL_STATIC_OPERATOR                 ,
    DX100_PARAM_CTRL_STATIC_KEY_SCALING              ,
    DX100_PARAM_CTRL_STATIC_PITCH_ENVELOPE_GENERATOR ,
    DX100_PARAM_CTRL_STATIC_FREQ_RATIO               ,
    DX100_PARAM_CTRL_STATIC_DETUNE                   ,
    DX100_PARAM_CTRL_STATIC_AR                       ,
    DX100_PARAM_CTRL_STATIC_D1R                      ,
    DX100_PARAM_CTRL_STATIC_D1L                      ,
    DX100_PARAM_CTRL_STATIC_D2R                      ,
    DX100_PARAM_CTRL_STATIC_RR                       ,
    DX100_PARAM_CTRL_STATIC_OUT_LEVEL                ,
    DX100_PARAM_CTRL_STATIC_RATE                     ,
    DX100_PARAM_CTRL_STATIC_LEVEL                    ,
    DX100_PARAM_CTRL_STATIC_PR1                      ,
    DX100_PARAM_CTRL_STATIC_PL1                      ,
    DX100_PARAM_CTRL_STATIC_PR2                      ,
    DX100_PARAM_CTRL_STATIC_PL2                      ,
    DX100_PARAM_CTRL_STATIC_PR3                      ,
    DX100_PARAM_CTRL_STATIC_PL3                      ,
    DX100_PARAM_CTRL_STATIC_PITCH_BEND               ,
    DX100_PARAM_CTRL_STATIC_PORTAMENTO               ,
    DX100_PARAM_CTRL_STATIC_FOOT_CONTROL             ,
    DX100_PARAM_CTRL_STATIC_WHEEL_RANGE              ,
    DX100_PARAM_CTRL_STATIC_BREATH_CONTROLLER_RANGE  ,
    DX100_PARAM_CTRL_STATIC_POLY_MONO                ,
    DX100_PARAM_CTRL_STATIC_PB_RANGE                 ,
    DX100_PARAM_CTRL_STATIC_PORTA_MODE               ,
    DX100_PARAM_CTRL_STATIC_PORTA_TIME               ,
    DX100_PARAM_CTRL_STATIC_PORTAMENT_FOOT_SW        ,
    DX100_PARAM_CTRL_STATIC_FOOT_VOLUME_RANGE        ,
    DX100_PARAM_CTRL_STATIC_SUSTAIN_FOOT_SW          ,
    DX100_PARAM_CTRL_STATIC_WR_PITCH                 ,
    DX100_PARAM_CTRL_STATIC_WR_AMPLITUDE             ,
    DX100_PARAM_CTRL_STATIC_BC_PITCH                 ,
    DX100_PARAM_CTRL_STATIC_BC_AMPLITUDE             ,
    DX100_PARAM_CTRL_STATIC_BC_PITCH_BIAS            ,
    DX100_PARAM_CTRL_STATIC_BC_EG_BIAS               ,
    DX100_PARAM_CTRL_STATIC_CHORUS_SWITCH            ,
    DX100_PARAM_CTRL_STATIC_TRANSPOSE                ,DX100_PARAM_CTRL_1VOICE_END=DX100_PARAM_CTRL_STATIC_TRANSPOSE,

    DX100_PARAM_CTRL_ALL_VOICE_NAME_00               ,DX100_PARAM_CTRL_ALL_VOICE_START = DX100_PARAM_CTRL_ALL_VOICE_NAME_00,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_01               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_02               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_03               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_04               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_05               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_06               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_07               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_08               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_09               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_10               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_11               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_12               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_13               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_14               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_15               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_16               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_17               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_18               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_19               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_20               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_21               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_22               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_23               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_24               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_25               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_26               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_27               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_28               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_29               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_30               ,
    DX100_PARAM_CTRL_ALL_VOICE_NAME_31               ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_00       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_01       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_02       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_03       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_04       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_05       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_06       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_07       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_08       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_09       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_10       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_11       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_12       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_13       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_14       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_15       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_16       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_17       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_18       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_19       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_20       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_21       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_22       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_23       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_24       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_25       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_26       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_27       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_28       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_29       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_30       ,
    DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_31       ,DX100_PARAM_CTRL_ALL_VOICE_END = DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_31,
    DX100_PARAM_CTRL_MAX
}DX100_PARAM_CTRL_ID;

/********************************************************************************
 * 内容  : 登録された全てのパラメータコントロールを生成する
 * 引数  : HINSTANCE hInst
 * 引数  : PTSTR szAppName
 * 引数  : HWND hwnd 親ウィンドウのハンドラ
 * 戻り値: BOOL
 ***************************************/
BOOL Dx100ParamCtrlCreate( HINSTANCE hInst, PTSTR szAppName, HWND hwnd );

/********************************************************************************
 * 内容  : 指定のグループのパラメータコントロールを表示
 * 引数  : DX100_PARAM_CTRL_GROUP_ID groupId
 * 戻り値: BOOL
 ***************************************/
BOOL Dx100ParamCtrlGroupDisplay( DX100_PARAM_CTRL_GROUP_ID groupId );

/********************************************************************************
 * 内容  : ウィンドウハンドラを取得する
 * 引数  : DX100_PARAM_CTRL_ID id
 * 戻り値: HWND
 ***************************************/
HWND Dx100ParamCtrlGetHWND( DX100_PARAM_CTRL_ID id );

/********************************************************************************
 * 内容  : ウィンドウからテキストを取得する
 * 引数  : DX100_PARAM_CTRL_ID id
 * 引数  : PTSTR ptstrText
 * 戻り値: BOOL
 ***************************************/
BOOL Dx100ParamCtrlGetText( DX100_PARAM_CTRL_ID id, PTSTR ptstrText );

/********************************************************************************
 * 内容  : コントロールのサイズを調整する
 * 引数  : int xPos
 * 引数  : int yPos
 * 戻り値: なし
 ***************************************/
void Dx100ParamCtrlSetSize( int xPos, int yPos );

#endif /* DX100_PARAM_CTRL_H */
