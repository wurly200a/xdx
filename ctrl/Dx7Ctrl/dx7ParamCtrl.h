#ifndef DX7_PARAM_CTRL_H
#define DX7_PARAM_CTRL_H

#define DX7_PARAM_CTRL_ID_OFFSET 2000

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
    DX7_PARAM_CTRL_VOICE_77                        ,
    DX7_PARAM_CTRL_VOICE_78                        ,
    DX7_PARAM_CTRL_VOICE_79                        ,
    DX7_PARAM_CTRL_VOICE_80                        ,
    DX7_PARAM_CTRL_VOICE_81                        ,
    DX7_PARAM_CTRL_VOICE_82                        ,
    DX7_PARAM_CTRL_VOICE_83                        ,
    DX7_PARAM_CTRL_VOICE_84                        ,
    DX7_PARAM_CTRL_VOICE_85                        ,
    DX7_PARAM_CTRL_VOICE_86                        ,
    DX7_PARAM_CTRL_VOICE_87                        ,
    DX7_PARAM_CTRL_VOICE_88                        ,
    DX7_PARAM_CTRL_VOICE_89                        ,
    DX7_PARAM_CTRL_VOICE_90                        ,
    DX7_PARAM_CTRL_VOICE_91                        ,
    DX7_PARAM_CTRL_VOICE_92                        ,
    DX7_PARAM_CTRL_VOICE_93                        ,
    DX7_PARAM_CTRL_VOICE_94                        ,
    DX7_PARAM_CTRL_VOICE_95                        ,
    DX7_PARAM_CTRL_VOICE_96                        ,
    DX7_PARAM_CTRL_VOICE_97                        ,
    DX7_PARAM_CTRL_VOICE_98                        ,
    DX7_PARAM_CTRL_VOICE_99                        ,
    DX7_PARAM_CTRL_VOICE_100                       ,
    DX7_PARAM_CTRL_VOICE_101                       ,
    DX7_PARAM_CTRL_VOICE_102                       ,
    DX7_PARAM_CTRL_VOICE_103                       ,
    DX7_PARAM_CTRL_VOICE_104                       ,
    DX7_PARAM_CTRL_VOICE_105                       ,
    DX7_PARAM_CTRL_VOICE_106                       ,
    DX7_PARAM_CTRL_VOICE_107                       ,
    DX7_PARAM_CTRL_VOICE_108                       ,
    DX7_PARAM_CTRL_VOICE_109                       ,
    DX7_PARAM_CTRL_VOICE_110                       ,
    DX7_PARAM_CTRL_VOICE_111                       ,
    DX7_PARAM_CTRL_VOICE_112                       ,
    DX7_PARAM_CTRL_VOICE_113                       ,
    DX7_PARAM_CTRL_VOICE_114                       ,
    DX7_PARAM_CTRL_VOICE_115                       ,
    DX7_PARAM_CTRL_VOICE_116                       ,
    DX7_PARAM_CTRL_VOICE_117                       ,
    DX7_PARAM_CTRL_VOICE_118                       ,
    DX7_PARAM_CTRL_VOICE_119                       ,
    DX7_PARAM_CTRL_VOICE_120                       ,
    DX7_PARAM_CTRL_VOICE_121                       ,
    DX7_PARAM_CTRL_VOICE_122                       ,
    DX7_PARAM_CTRL_VOICE_123                       ,
    DX7_PARAM_CTRL_VOICE_124                       ,
    DX7_PARAM_CTRL_VOICE_125                       ,
    DX7_PARAM_CTRL_VOICE_126                       ,
    DX7_PARAM_CTRL_VOICE_127                       ,
    DX7_PARAM_CTRL_VOICE_128                       ,
    DX7_PARAM_CTRL_VOICE_129                       ,
    DX7_PARAM_CTRL_VOICE_130                       ,
    DX7_PARAM_CTRL_VOICE_131                       ,
    DX7_PARAM_CTRL_VOICE_132                       ,
    DX7_PARAM_CTRL_VOICE_133                       ,
    DX7_PARAM_CTRL_VOICE_134                       ,
    DX7_PARAM_CTRL_VOICE_135                       ,
    DX7_PARAM_CTRL_VOICE_136                       ,
    DX7_PARAM_CTRL_VOICE_137                       ,
    DX7_PARAM_CTRL_VOICE_138                       ,
    DX7_PARAM_CTRL_VOICE_139                       ,
    DX7_PARAM_CTRL_VOICE_140                       ,
    DX7_PARAM_CTRL_VOICE_141                       ,
    DX7_PARAM_CTRL_VOICE_142                       ,
    DX7_PARAM_CTRL_VOICE_143                       ,
    DX7_PARAM_CTRL_VOICE_144                       ,
    DX7_PARAM_CTRL_VOICE_NAME/*145-154*/           ,
    /*DX7_PARAM_CTRL_VOICE_155                  ,*/
    DX7_PARAM_CTRL_BUTTON_TO_ALL_VOICE             ,
    DX7_PARAM_CTRL_COMBOBOX_TO_ALL_VOICE_NUM       ,
    DX7_PARAM_CTRL_STATIC_PATCHNAME                ,
    DX7_PARAM_CTRL_STATIC_LFO                      ,
    DX7_PARAM_CTRL_STATIC_MODULATION_SENSITVITY    ,
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
    DX7_PARAM_CTRL_STATIC_1_2                      ,/*OPERATOR1*/
    DX7_PARAM_CTRL_STATIC_2_2                      ,/*OPERATOR2*/
    DX7_PARAM_CTRL_STATIC_3_2                      ,/*OPERATOR3*/
    DX7_PARAM_CTRL_STATIC_4_2                      ,/*OPERATOR4*/
    DX7_PARAM_CTRL_STATIC_5_2                      ,/*OPERATOR5*/
    DX7_PARAM_CTRL_STATIC_6_2                      ,/*OPERATOR6*/
    DX7_PARAM_CTRL_STATIC_1_3                      ,/*OPERATOR1*/
    DX7_PARAM_CTRL_STATIC_2_3                      ,/*OPERATOR2*/
    DX7_PARAM_CTRL_STATIC_3_3                      ,/*OPERATOR3*/
    DX7_PARAM_CTRL_STATIC_4_3                      ,/*OPERATOR4*/
    DX7_PARAM_CTRL_STATIC_5_3                      ,/*OPERATOR5*/
    DX7_PARAM_CTRL_STATIC_6_3                      ,/*OPERATOR6*/
    DX7_PARAM_CTRL_STATIC_OSCILLATOR               ,
    DX7_PARAM_CTRL_STATIC_FREQ_MODE                ,
    DX7_PARAM_CTRL_STATIC_SYNC                     ,
    DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE1         ,
    DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE2         ,
    DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE3         ,
    DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE4         ,
    DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE5         ,
    DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE6         ,
    DX7_PARAM_CTRL_STATIC_FREQUENCY                ,
    DX7_PARAM_CTRL_STATIC_COARSE                   ,
    DX7_PARAM_CTRL_STATIC_FINE                     ,
    DX7_PARAM_CTRL_STATIC_DETUNE                   ,
    DX7_PARAM_CTRL_STATIC_EG                       ,
    DX7_PARAM_CTRL_STATIC_RATE1                    ,
    DX7_PARAM_CTRL_STATIC_RATE2                    ,
    DX7_PARAM_CTRL_STATIC_RATE3                    ,
    DX7_PARAM_CTRL_STATIC_RATE4                    ,
    DX7_PARAM_CTRL_STATIC_LEVEL1                   ,
    DX7_PARAM_CTRL_STATIC_LEVEL2                   ,
    DX7_PARAM_CTRL_STATIC_LEVEL3                   ,
    DX7_PARAM_CTRL_STATIC_LEVEL4                   ,
    DX7_PARAM_CTRL_STATIC_K_LEVEL_SCALING          ,
    DX7_PARAM_CTRL_STATIC_BREAK                    ,
    DX7_PARAM_CTRL_STATIC_CURVE_L                  ,
    DX7_PARAM_CTRL_STATIC_CURVE_R                  ,
    DX7_PARAM_CTRL_STATIC_DEPTH_L                  ,
    DX7_PARAM_CTRL_STATIC_DEPTH_R                  ,
    DX7_PARAM_CTRL_STATIC_K_RATE_SCALING1          ,
    DX7_PARAM_CTRL_STATIC_K_RATE_SCALING2          ,
    DX7_PARAM_CTRL_STATIC_OPERATOR                 ,
    DX7_PARAM_CTRL_STATIC_OUT_LEVEL                ,
    DX7_PARAM_CTRL_STATIC_VELO_SENS                ,
    DX7_PARAM_CTRL_STATIC_PEG                      ,
    DX7_PARAM_CTRL_STATIC_PEG_RATE1                ,
    DX7_PARAM_CTRL_STATIC_PEG_RATE2                ,
    DX7_PARAM_CTRL_STATIC_PEG_RATE3                ,
    DX7_PARAM_CTRL_STATIC_PEG_RATE4                ,
    DX7_PARAM_CTRL_STATIC_PEG_LEVEL1               ,
    DX7_PARAM_CTRL_STATIC_PEG_LEVEL2               ,
    DX7_PARAM_CTRL_STATIC_PEG_LEVEL3               ,
    DX7_PARAM_CTRL_STATIC_PEG_LEVEL4               ,
    DX7_PARAM_CTRL_STATIC_KEY_TRANSPOSE            ,DX7_PARAM_CTRL_1VOICE_END=DX7_PARAM_CTRL_STATIC_KEY_TRANSPOSE,
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
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_00          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_01          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_02          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_03          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_04          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_05          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_06          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_07          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_08          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_09          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_10          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_11          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_12          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_13          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_14          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_15          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_16          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_17          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_18          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_19          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_20          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_21          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_22          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_23          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_24          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_25          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_26          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_27          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_28          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_29          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_30          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_31          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_00          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_01          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_02          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_03          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_04          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_05          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_06          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_07          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_08          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_09          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_10          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_11          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_12          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_13          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_14          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_15          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_16          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_17          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_18          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_19          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_20          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_21          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_22          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_23          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_24          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_25          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_26          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_27          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_28          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_29          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_30          ,
    DX7_PARAM_CTRL_ALL_VOICE_EX2_PARAM_31          ,
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
