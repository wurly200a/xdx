#ifndef DX7_PARAM_CTRL_H
#define DX7_PARAM_CTRL_H

#define DX7_PARAM_CTRL_ID_OFFSET 100

typedef enum
{
    DX7_PARAM_CTRL_GROUP_NONE         ,
    DX7_PARAM_CTRL_GROUP_1VOICE       ,
    DX7_PARAM_CTRL_GROUP_ALL_VOICE    ,
    DX7_PARAM_CTRL_GROUP_MAX
}DX7_PARAM_CTRL_GROUP_ID;

typedef enum
{
    DX7_PARAM_CTRL_VOICE_00                        ,DX7_PARAM_CTRL_1VOICE_START=DX7_PARAM_CTRL_VOICE_00,
    DX7_PARAM_CTRL_VOICE_01                        ,
    DX7_PARAM_CTRL_VOICE_02                        ,
    DX7_PARAM_CTRL_VOICE_03                        ,
    DX7_PARAM_CTRL_VOICE_04                        ,
    DX7_PARAM_CTRL_VOICE_05                        ,
    DX7_PARAM_CTRL_VOICE_06                        ,
    DX7_PARAM_CTRL_VOICE_07                        ,
    DX7_PARAM_CTRL_VOICE_08                        ,
    DX7_PARAM_CTRL_VOICE_09                        ,
    DX7_PARAM_CTRL_VOICE_10                        ,
    DX7_PARAM_CTRL_VOICE_11                        ,
    DX7_PARAM_CTRL_VOICE_12                        ,
    DX7_PARAM_CTRL_VOICE_13                        ,
    DX7_PARAM_CTRL_VOICE_14                        ,
    DX7_PARAM_CTRL_VOICE_15                        ,
    DX7_PARAM_CTRL_VOICE_16                        ,
    DX7_PARAM_CTRL_VOICE_17                        ,
    DX7_PARAM_CTRL_VOICE_18                        ,
    DX7_PARAM_CTRL_VOICE_19                        ,
    DX7_PARAM_CTRL_VOICE_20                        ,
    DX7_PARAM_CTRL_VOICE_21                        ,
    DX7_PARAM_CTRL_VOICE_22                        ,
    DX7_PARAM_CTRL_VOICE_23                        ,
    DX7_PARAM_CTRL_VOICE_24                        ,
    DX7_PARAM_CTRL_VOICE_25                        ,
    DX7_PARAM_CTRL_VOICE_26                        ,
    DX7_PARAM_CTRL_VOICE_27                        ,
    DX7_PARAM_CTRL_VOICE_28                        ,
    DX7_PARAM_CTRL_VOICE_29                        ,
    DX7_PARAM_CTRL_VOICE_30                        ,
    DX7_PARAM_CTRL_VOICE_31                        ,
    DX7_PARAM_CTRL_VOICE_32                        ,
    DX7_PARAM_CTRL_VOICE_33                        ,
    DX7_PARAM_CTRL_VOICE_34                        ,
    DX7_PARAM_CTRL_VOICE_35                        ,
    DX7_PARAM_CTRL_VOICE_36                        ,
    DX7_PARAM_CTRL_VOICE_37                        ,
    DX7_PARAM_CTRL_VOICE_38                        ,
    DX7_PARAM_CTRL_VOICE_39                        ,
    DX7_PARAM_CTRL_VOICE_40                        ,
    DX7_PARAM_CTRL_VOICE_41                        ,
    DX7_PARAM_CTRL_VOICE_42                        ,
    DX7_PARAM_CTRL_VOICE_43                        ,
    DX7_PARAM_CTRL_VOICE_44                        ,
    DX7_PARAM_CTRL_VOICE_45                        ,
    DX7_PARAM_CTRL_VOICE_46                        ,
    DX7_PARAM_CTRL_VOICE_47                        ,
    DX7_PARAM_CTRL_VOICE_48                        ,
    DX7_PARAM_CTRL_VOICE_49                        ,
    DX7_PARAM_CTRL_VOICE_50                        ,
    DX7_PARAM_CTRL_VOICE_51                        ,
    DX7_PARAM_CTRL_VOICE_52                        ,
    DX7_PARAM_CTRL_VOICE_53                        ,
    DX7_PARAM_CTRL_VOICE_54                        ,
    DX7_PARAM_CTRL_VOICE_55                        ,
    DX7_PARAM_CTRL_VOICE_56                        ,
    DX7_PARAM_CTRL_VOICE_57                        ,
    DX7_PARAM_CTRL_VOICE_58                        ,
    DX7_PARAM_CTRL_VOICE_59                        ,
    DX7_PARAM_CTRL_VOICE_60                        ,
    DX7_PARAM_CTRL_VOICE_61                        ,
    DX7_PARAM_CTRL_VOICE_62                        ,
    DX7_PARAM_CTRL_VOICE_63                        ,
    DX7_PARAM_CTRL_VOICE_64                        ,
    DX7_PARAM_CTRL_VOICE_65                        ,
    DX7_PARAM_CTRL_VOICE_66                        ,
    DX7_PARAM_CTRL_VOICE_67                        ,
    DX7_PARAM_CTRL_VOICE_68                        ,
    DX7_PARAM_CTRL_VOICE_69                        ,
    DX7_PARAM_CTRL_VOICE_70                        ,
    DX7_PARAM_CTRL_VOICE_71                        ,
    DX7_PARAM_CTRL_VOICE_72                        ,
    DX7_PARAM_CTRL_VOICE_73                        ,
    DX7_PARAM_CTRL_VOICE_74                        ,
    DX7_PARAM_CTRL_VOICE_75                        ,
    DX7_PARAM_CTRL_VOICE_76                        ,
    DX7_PARAM_CTRL_VOICE_NAME/*77-86*/             ,
    DX7_PARAM_CTRL_VOICE_87                        ,
    DX7_PARAM_CTRL_VOICE_88                        ,
    DX7_PARAM_CTRL_VOICE_89                        ,
    DX7_PARAM_CTRL_VOICE_90                        ,
    DX7_PARAM_CTRL_VOICE_91                        ,
    DX7_PARAM_CTRL_VOICE_92                        ,
    DX7_PARAM_CTRL_STATIC_1                        ,
    DX7_PARAM_CTRL_STATIC_2                        ,
    DX7_PARAM_CTRL_STATIC_3                        ,
    DX7_PARAM_CTRL_STATIC_4                        ,
    DX7_PARAM_CTRL_STATIC_1_2                      ,
    DX7_PARAM_CTRL_STATIC_2_2                      ,
    DX7_PARAM_CTRL_STATIC_3_2                      ,
    DX7_PARAM_CTRL_STATIC_4_2                      ,
    DX7_PARAM_CTRL_STATIC_PATCHNAME                ,
    DX7_PARAM_CTRL_STATIC_LFO                      ,
    DX7_PARAM_CTRL_STATIC_MOD_SENS                 ,
    DX7_PARAM_CTRL_STATIC_KEY                      ,
    DX7_PARAM_CTRL_STATIC_ALGORITHM                ,
    DX7_PARAM_CTRL_STATIC_FEEDBACK                 ,
    DX7_PARAM_CTRL_STATIC_LFO_WAVE                 ,
    DX7_PARAM_CTRL_STATIC_SPEED                    ,
    DX7_PARAM_CTRL_STATIC_DELAY                    ,
    DX7_PARAM_CTRL_STATIC_PMD                      ,
    DX7_PARAM_CTRL_STATIC_AMD                      ,
    DX7_PARAM_CTRL_STATIC_LFO_SYNC                 ,
    DX7_PARAM_CTRL_STATIC_PITCH                    ,
    DX7_PARAM_CTRL_STATIC_AMPLITUDE                ,
    DX7_PARAM_CTRL_STATIC_AME                      ,
    DX7_PARAM_CTRL_STATIC_EG_BIAS                  ,
    DX7_PARAM_CTRL_STATIC_VELOCITY                 ,
    DX7_PARAM_CTRL_STATIC_OSCILLATOR               ,
    DX7_PARAM_CTRL_STATIC_ENVELOPE_GENERATOR       ,
    DX7_PARAM_CTRL_STATIC_OPERATOR                 ,
    DX7_PARAM_CTRL_STATIC_KEY_SCALING              ,
    DX7_PARAM_CTRL_STATIC_PITCH_ENVELOPE_GENERATOR ,
    DX7_PARAM_CTRL_STATIC_FREQ_RATIO               ,
    DX7_PARAM_CTRL_STATIC_DETUNE                   ,
    DX7_PARAM_CTRL_STATIC_AR                       ,
    DX7_PARAM_CTRL_STATIC_D1R                      ,
    DX7_PARAM_CTRL_STATIC_D1L                      ,
    DX7_PARAM_CTRL_STATIC_D2R                      ,
    DX7_PARAM_CTRL_STATIC_RR                       ,
    DX7_PARAM_CTRL_STATIC_OUT_LEVEL                ,
    DX7_PARAM_CTRL_STATIC_RATE                     ,
    DX7_PARAM_CTRL_STATIC_LEVEL                    ,
    DX7_PARAM_CTRL_STATIC_PR1                      ,
    DX7_PARAM_CTRL_STATIC_PL1                      ,
    DX7_PARAM_CTRL_STATIC_PR2                      ,
    DX7_PARAM_CTRL_STATIC_PL2                      ,
    DX7_PARAM_CTRL_STATIC_PR3                      ,
    DX7_PARAM_CTRL_STATIC_PL3                      ,
    DX7_PARAM_CTRL_STATIC_PITCH_BEND               ,
    DX7_PARAM_CTRL_STATIC_PORTAMENTO               ,
    DX7_PARAM_CTRL_STATIC_FOOT_CONTROL             ,
    DX7_PARAM_CTRL_STATIC_WHEEL_RANGE              ,
    DX7_PARAM_CTRL_STATIC_BREATH_CONTROLLER_RANGE  ,
    DX7_PARAM_CTRL_STATIC_POLY_MONO                ,
    DX7_PARAM_CTRL_STATIC_PB_RANGE                 ,
    DX7_PARAM_CTRL_STATIC_PORTA_MODE               ,
    DX7_PARAM_CTRL_STATIC_PORTA_TIME               ,
    DX7_PARAM_CTRL_STATIC_PORTAMENT_FOOT_SW        ,
    DX7_PARAM_CTRL_STATIC_FOOT_VOLUME_RANGE        ,
    DX7_PARAM_CTRL_STATIC_SUSTAIN_FOOT_SW          ,
    DX7_PARAM_CTRL_STATIC_WR_PITCH                 ,
    DX7_PARAM_CTRL_STATIC_WR_AMPLITUDE             ,
    DX7_PARAM_CTRL_STATIC_BC_PITCH                 ,
    DX7_PARAM_CTRL_STATIC_BC_AMPLITUDE             ,
    DX7_PARAM_CTRL_STATIC_BC_PITCH_BIAS            ,
    DX7_PARAM_CTRL_STATIC_BC_EG_BIAS               ,
    DX7_PARAM_CTRL_STATIC_CHORUS_SWITCH            ,
    DX7_PARAM_CTRL_STATIC_TRANSPOSE                ,DX7_PARAM_CTRL_1VOICE_END=DX7_PARAM_CTRL_STATIC_TRANSPOSE,

    DX7_PARAM_CTRL_ALL_VOICE_NAME_00               ,DX7_PARAM_CTRL_ALL_VOICE_START = DX7_PARAM_CTRL_ALL_VOICE_NAME_00,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_01               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_02               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_03               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_04               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_05               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_06               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_07               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_08               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_09               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_10               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_11               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_12               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_13               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_14               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_15               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_16               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_17               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_18               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_19               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_20               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_21               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_22               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_23               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_24               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_25               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_26               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_27               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_28               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_29               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_30               ,
    DX7_PARAM_CTRL_ALL_VOICE_NAME_31               ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_00       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_01       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_02       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_03       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_04       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_05       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_06       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_07       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_08       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_09       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_10       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_11       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_12       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_13       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_14       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_15       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_16       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_17       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_18       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_19       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_20       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_21       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_22       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_23       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_24       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_25       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_26       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_27       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_28       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_29       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_30       ,
    DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_31       ,DX7_PARAM_CTRL_ALL_VOICE_END = DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_31,
    DX7_PARAM_CTRL_MAX
}DX7_PARAM_CTRL_ID;

/********************************************************************************
 * 内容  : 登録された全てのパラメータコントロールを生成する
 * 引数  : HINSTANCE hInst
 * 引数  : PTSTR szAppName
 * 引数  : HWND hwnd 親ウィンドウのハンドラ
 * 戻り値: BOOL
 ***************************************/
BOOL Dx7ParamCtrlCreate( HINSTANCE hInst, PTSTR szAppName, HWND hwnd );

/********************************************************************************
 * 内容  : 指定のグループのパラメータコントロールを表示
 * 引数  : DX7_PARAM_CTRL_GROUP_ID groupId
 * 戻り値: BOOL
 ***************************************/
BOOL Dx7ParamCtrlGroupDisplay( DX7_PARAM_CTRL_GROUP_ID groupId );

/********************************************************************************
 * 内容  : ウィンドウハンドラを取得する
 * 引数  : DX7_PARAM_CTRL_ID id
 * 戻り値: HWND
 ***************************************/
HWND Dx7ParamCtrlGetHWND( DX7_PARAM_CTRL_ID id );

/********************************************************************************
 * 内容  : ウィンドウからテキストを取得する
 * 引数  : DX7_PARAM_CTRL_ID id
 * 引数  : PTSTR ptstrText
 * 戻り値: BOOL
 ***************************************/
BOOL Dx7ParamCtrlGetText( DX7_PARAM_CTRL_ID id, PTSTR ptstrText );

/********************************************************************************
 * 内容  : コントロールのサイズを調整する
 * 引数  : int xPos
 * 引数  : int yPos
 * 戻り値: なし
 ***************************************/
void Dx7ParamCtrlSetSize( int xPos, int yPos );

#endif /* DX7_PARAM_CTRL_H */
