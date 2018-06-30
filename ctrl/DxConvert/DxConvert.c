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

/********************************************************************************
 * 内容  :
 * 引数  : S_DX100_VOICE_PARAM *dx100voiceParamPtr
 * 引数  : S_DX7_VOICE_PARAM *dx7voiceParamPtr
 * 戻り値: なし
 ***************************************/
void
DxConvertVoiceParamDx100toDx7( S_DX100_VOICE_PARAM *dx100voiceParamPtr,S_DX7_VOICE_PARAM *dx7voiceParamPtr )
{
    dx7voiceParamPtr->patchname[0]    = dx100voiceParamPtr->patchname[0];
    dx7voiceParamPtr->patchname[1]    = dx100voiceParamPtr->patchname[1];
    dx7voiceParamPtr->patchname[2]    = dx100voiceParamPtr->patchname[2];
    dx7voiceParamPtr->patchname[3]    = dx100voiceParamPtr->patchname[3];
    dx7voiceParamPtr->patchname[4]    = dx100voiceParamPtr->patchname[4];
    dx7voiceParamPtr->patchname[5]    = dx100voiceParamPtr->patchname[5];
    dx7voiceParamPtr->patchname[6]    = dx100voiceParamPtr->patchname[6];
    dx7voiceParamPtr->patchname[7]    = dx100voiceParamPtr->patchname[7];
    dx7voiceParamPtr->patchname[8]    = dx100voiceParamPtr->patchname[8];
    dx7voiceParamPtr->patchname[9]    = dx100voiceParamPtr->patchname[9];
    dx7voiceParamPtr->algorithm       = dx100voiceParamPtr->algorithm   ;
    dx7voiceParamPtr->feedback        = dx100voiceParamPtr->feedback    ;
#if 0
    dx7voiceParamPtr->lfo_wave        = ;
    dx7voiceParamPtr->speed           = ;
    dx7voiceParamPtr->delay           = ;
    dx7voiceParamPtr->pmd             = ;
    dx7voiceParamPtr->amd             = ;
    dx7voiceParamPtr->lfo_sync        = ;
    dx7voiceParamPtr->pitch           = ;
    dx7voiceParamPtr->oscillator_sync = ;
    dx7voiceParamPtr->peg_rate1       = ;
    dx7voiceParamPtr->peg_rate2       = ;
    dx7voiceParamPtr->peg_rate3       = ;
    dx7voiceParamPtr->peg_rate4       = ;
    dx7voiceParamPtr->peg_level1      = ;
    dx7voiceParamPtr->peg_level2      = ;
    dx7voiceParamPtr->peg_level3      = ;
    dx7voiceParamPtr->peg_level4      = ;
    dx7voiceParamPtr->key_transpose   = ;
#endif

#if 0
 = dx100voiceParamPtr->lfo_wave                 ;
 = dx100voiceParamPtr->speed                    ;
 = dx100voiceParamPtr->delay                    ;
 = dx100voiceParamPtr->pmd                      ;
 = dx100voiceParamPtr->amd                      ;
 = dx100voiceParamPtr->lfo_sync                 ;
 = dx100voiceParamPtr->pitch                    ;
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
 = dx100voiceParamPtr->transpose                ;
#endif
}
