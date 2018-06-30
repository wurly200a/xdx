/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */
#include "dx100Ctrl.h"
#include "dx7Ctrl.h"

/* 外部関数定義 */

/* 外部変数定義 */

/* 内部関数定義 */
#include "DxConvert.h"

/* 内部変数定義 */

typedef struct
{
    char *str;
    BYTE coarse;
    BYTE fine  ;
} S_DX_FREQ_OP4_TO_OP6_CONVERT_INFO;

static S_DX_FREQ_OP4_TO_OP6_CONVERT_INFO convertTblFreq[64] =
{ /* str    ,coarse,fine */
    {"0.50" , 0, 0},
    {"0.71" , 0,42},
    {"0.78" , 0,56},
    {"0.87" , 0,74},
    {"1.00" , 1, 0},
    {"1.41" , 1,41},
    {"1.57" , 1,57},
    {"1.73" , 1,73},
    {"2.00" , 2, 0},
    {"2.82" , 2,41},
    {"3.00" , 3, 0},
    {"3.14" , 2,57},
    {"3.46" , 2,73},
    {"4.00" , 4, 0},
    {"4.24" , 4, 6},
    {"4.71" , 3,57},
    {"5.00" , 5, 0},
    {"5.19" , 3,73},
    {"5.65" , 5,13},
    {"6.00" , 6, 0},
    {"6.28" , 4,57},
    {"6.92" , 4,73},
    {"7.00" , 7, 0},
    {"7.07" , 7, 1},
    {"7.85" , 5,57},
    {"8.00" , 8, 0},
    {"8.48" , 8, 6},
    {"8.65" , 5,73},
    {"9.00" , 9, 0},
    {"9.42" , 6,57},
    {"9.89" , 7,41}, /* 9.870 */
    {"10.00",10, 0},
    {"10.38", 6,73},
    {"10.99", 7,57},
    {"11.00",11, 0},
    {"11.30",10,13},
    {"12.00",12, 0},
    {"12.11", 7,73},
    {"12.56", 8,57},
    {"12.72", 8,59},
    {"13.00",13, 0},
    {"13.84", 8,73},
    {"14.00",14, 0},
    {"14.10",10,41},
    {"14.13", 9,57},
    {"15.00",15, 0},
    {"15.55",14,11}, /* 14.540 */
    {"15.57", 9,73},
    {"15.70",10,57},
    {"16.96",16, 6},
    {"17.27",11,57},
    {"17.30",10,73},
    {"18.37",11,67},
    {"18.84",12,57},
    {"19.03",11,73},
    {"19.78",18,10}, /* 19.800 */
    {"20.41",13,57},
    {"20.76",12,73},
    {"21.20",20, 6},
    {"21.98",14,57},
    {"22.49",13,73},
    {"23.55",15,57},
    {"24.22",14,73},
    {"25.95",15,73},
};

typedef struct
{
    DX7_ALGORITHM_NUM dx7algorighm;
    DX7_OPERATOR_NUM  dx7OperatorNum[DX100_OPERATOR_MAX];
} S_DX_OPERATOR_OP4_TO_OP6_CONVERT_INFO;

static S_DX_OPERATOR_OP4_TO_OP6_CONVERT_INFO convertTblOperator[DX100_ALGORITHM_MAX] =
{
    {DX7_ALGORITHM01,{DX7_ALGORITHM03,DX7_ALGORITHM04,DX7_ALGORITHM05,DX7_ALGORITHM06}},
    {DX7_ALGORITHM14,{DX7_ALGORITHM04,DX7_ALGORITHM05,DX7_ALGORITHM03,DX7_ALGORITHM06}},
    {DX7_ALGORITHM08,{DX7_ALGORITHM03,DX7_ALGORITHM05,DX7_ALGORITHM06,DX7_ALGORITHM04}},
    {DX7_ALGORITHM07,{DX7_ALGORITHM03,DX7_ALGORITHM04,DX7_ALGORITHM05,DX7_ALGORITHM06}},
    {DX7_ALGORITHM05,{DX7_ALGORITHM03,DX7_ALGORITHM04,DX7_ALGORITHM05,DX7_ALGORITHM06}},
    {DX7_ALGORITHM22,{DX7_ALGORITHM03,DX7_ALGORITHM04,DX7_ALGORITHM05,DX7_ALGORITHM06}},
    {DX7_ALGORITHM31,{DX7_ALGORITHM03,DX7_ALGORITHM04,DX7_ALGORITHM05,DX7_ALGORITHM06}},
    {DX7_ALGORITHM32,{DX7_ALGORITHM03,DX7_ALGORITHM04,DX7_ALGORITHM05,DX7_ALGORITHM06}},
};

static DX7_DETUNE convetTblDetune[DX100_DETUNE_MAX] =
{
    DX7_MINUS3,
    DX7_MINUS2,
    DX7_MINUS1,
    DX7_ZERO  ,
    DX7_PLUS1 ,
    DX7_PLUS2 ,
    DX7_PLUS3 ,
};

/********************************************************************************
 * 内容  :
 * 引数  : S_DX100_VOICE_PARAM *dx100voiceParamPtr
 * 引数  : S_DX7_VOICE_PARAM *dx7voiceParamPtr
 * 戻り値: なし
 ***************************************/
void
DxConvertVoiceParamDx100toDx7( S_DX100_VOICE_PARAM *dx100voiceParamPtr,S_DX7_VOICE_PARAM *dx7voiceParamPtr )
{
    DX7_OPERATOR_NUM dx7opNum;
    DX100_OPERATOR_NUM dx100opNum;
    S_DX_OPERATOR_OP4_TO_OP6_CONVERT_INFO *infoTblPtr;

    dx7voiceParamPtr->patchname[0] = dx100voiceParamPtr->patchname[0];
    dx7voiceParamPtr->patchname[1] = dx100voiceParamPtr->patchname[1];
    dx7voiceParamPtr->patchname[2] = dx100voiceParamPtr->patchname[2];
    dx7voiceParamPtr->patchname[3] = dx100voiceParamPtr->patchname[3];
    dx7voiceParamPtr->patchname[4] = dx100voiceParamPtr->patchname[4];
    dx7voiceParamPtr->patchname[5] = dx100voiceParamPtr->patchname[5];
    dx7voiceParamPtr->patchname[6] = dx100voiceParamPtr->patchname[6];
    dx7voiceParamPtr->patchname[7] = dx100voiceParamPtr->patchname[7];
    dx7voiceParamPtr->patchname[8] = dx100voiceParamPtr->patchname[8];
    dx7voiceParamPtr->patchname[9] = dx100voiceParamPtr->patchname[9];

    infoTblPtr = &convertTblOperator[dx100voiceParamPtr->algorithm];

    dx7voiceParamPtr->algorithm = infoTblPtr->dx7algorighm;

    dx7voiceParamPtr->feedback = dx100voiceParamPtr->feedback    ;
    dx7voiceParamPtr->lfo_wave        = dx100voiceParamPtr->lfo_wave                 ;
    dx7voiceParamPtr->speed           = dx100voiceParamPtr->speed                    ;
    dx7voiceParamPtr->delay           = dx100voiceParamPtr->delay                    ;
    dx7voiceParamPtr->pmd             = dx100voiceParamPtr->pmd                      ;
    dx7voiceParamPtr->amd             = dx100voiceParamPtr->amd                      ;
    dx7voiceParamPtr->lfo_sync        = dx100voiceParamPtr->lfo_sync                 ;
    dx7voiceParamPtr->pitch           = dx100voiceParamPtr->pitch                    ;
#if 0
    dx7voiceParamPtr->oscillator_sync = ;
    dx7voiceParamPtr->peg_rate1       = ;
    dx7voiceParamPtr->peg_rate2       = ;
    dx7voiceParamPtr->peg_rate3       = ;
    dx7voiceParamPtr->peg_rate4       = ;
    dx7voiceParamPtr->peg_level1      = ;
    dx7voiceParamPtr->peg_level2      = ;
    dx7voiceParamPtr->peg_level3      = ;
    dx7voiceParamPtr->peg_level4      = ;
#endif

#if 0
 = dx100voiceParamPtr->amplitude                ;
 = dx100voiceParamPtr->pr1                      ;
 = dx100voiceParamPtr->pl1                      ;
 = dx100voiceParamPtr->pr2                      ;
 = dx100voiceParamPtr->pl2                      ;
 = dx100voiceParamPtr->pr3                      ;
 = dx100voiceParamPtr->pl3                      ;
 = dx100voiceParamPtr->poly_mono                ;
 = dx100voiceParamPtr->pb_range                 ;
 = dx100voiceParamPtr->porta_mode               ;
 = dx100voiceParamPtr->porta_time               ;
 = dx100voiceParamPtr->portament_foot_sw        ;
 = dx100voiceParamPtr->foot_volume_range        ;
 = dx100voiceParamPtr->sustain_foot_sw          ;
 = dx100voiceParamPtr->wr_pitch                 ;
 = dx100voiceParamPtr->wr_amplitude             ;
 = dx100voiceParamPtr->bc_pitch                 ;
 = dx100voiceParamPtr->bc_amplitude             ;
 = dx100voiceParamPtr->bc_pitch_bias            ;
 = dx100voiceParamPtr->bc_eg_bias               ;
 = dx100voiceParamPtr->chorus_switch            ;
#endif
    dx7voiceParamPtr->key_transpose = dx100voiceParamPtr->transpose;

    for( dx7opNum=0; dx7opNum<DX7_OPERATOR_MAX; dx7opNum++ )
    {
        dx7voiceParamPtr->operatorParam[dx7opNum].out_level = 0;
    }

    for( dx100opNum=0; dx100opNum<DX100_OPERATOR_MAX; dx100opNum++ )
    {
        dx7opNum = infoTblPtr->dx7OperatorNum[dx100opNum];

        dx7voiceParamPtr->operatorParam[dx7opNum].amplitude      = dx100voiceParamPtr->operatorParam[dx100opNum].ame       ;
        dx7voiceParamPtr->operatorParam[dx7opNum].freq_mode      = 0;
        dx7voiceParamPtr->operatorParam[dx7opNum].coarse         = convertTblFreq[dx100voiceParamPtr->operatorParam[dx100opNum].freq_ratio].coarse;
        dx7voiceParamPtr->operatorParam[dx7opNum].fine           = convertTblFreq[dx100voiceParamPtr->operatorParam[dx100opNum].freq_ratio].fine  ;
        dx7voiceParamPtr->operatorParam[dx7opNum].detune         = convetTblDetune[dx100voiceParamPtr->operatorParam[dx100opNum].detune];
#if 0
        dx7voiceParamPtr->operatorParam[dx7opNum].rate1          = dx100voiceParamPtr->operatorParam[dx100opNum].ar        ;
        dx7voiceParamPtr->operatorParam[dx7opNum].rate2          = dx100voiceParamPtr->operatorParam[dx100opNum].d1r       ;
        dx7voiceParamPtr->operatorParam[dx7opNum].rate3          = dx100voiceParamPtr->operatorParam[dx100opNum].d2r       ;
        dx7voiceParamPtr->operatorParam[dx7opNum].rate4          = dx100voiceParamPtr->operatorParam[dx100opNum].rr        ;
        dx7voiceParamPtr->operatorParam[dx7opNum].level1         = dx100voiceParamPtr->operatorParam[dx100opNum].d1l       ;
        dx7voiceParamPtr->operatorParam[dx7opNum].level2         = 90;
        dx7voiceParamPtr->operatorParam[dx7opNum].level3         = 90;
        dx7voiceParamPtr->operatorParam[dx7opNum].level4         = 90;
#endif

#if 0
          = dx100voiceParamPtr->operatorParam[dx100opNum].velocity  
#endif
#if 0
        dx7voiceParamPtr->operatorParam[dx7opNum].break_point    = 0;
        dx7voiceParamPtr->operatorParam[dx7opNum].curve_l        = 0;
        dx7voiceParamPtr->operatorParam[dx7opNum].curve_r        = 0;
        dx7voiceParamPtr->operatorParam[dx7opNum].depth_l        = 0;
        dx7voiceParamPtr->operatorParam[dx7opNum].depth_r        = 0;
#endif
        dx7voiceParamPtr->operatorParam[dx7opNum].k_rate_scaling = dx100voiceParamPtr->operatorParam[dx100opNum].rate      ;
        dx7voiceParamPtr->operatorParam[dx7opNum].out_level      = dx100voiceParamPtr->operatorParam[dx100opNum].out_level ;
//        dx7voiceParamPtr->operatorParam[dx7opNum].velo_sens      = 0;
    }

}
