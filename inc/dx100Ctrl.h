#ifndef DX100_CTRL_H
#define DX100_CTRL_H

enum
{
    DX100_OPERATOR1,
    DX100_OPERATOR2,
    DX100_OPERATOR3,
    DX100_OPERATOR4,
    DX100_OPERATOR_MAX,
};
typedef INT DX100_OPERATOR_NUM;

enum
{
    DX100_ALGORITHM1,
    DX100_ALGORITHM2,
    DX100_ALGORITHM3,
    DX100_ALGORITHM4,
    DX100_ALGORITHM5,
    DX100_ALGORITHM6,
    DX100_ALGORITHM7,
    DX100_ALGORITHM8,
    DX100_ALGORITHM_MAX,
};
typedef INT DX100_ALGORITHM_NUM;

enum
{
    DX100_MINUS3,
    DX100_MINUS2,
    DX100_MINUS1,
    DX100_ZERO  ,
    DX100_PLUS1 ,
    DX100_PLUS2 ,
    DX100_PLUS3 ,
    DX100_DETUNE_MAX,
};
typedef INT DX100_DETUNE;

enum
{
    DX100_LFO_WAVE_SAW,
    DX100_LFO_WAVE_SQU,
    DX100_LFO_WAVE_TRI,
    DX100_LFO_WAVE_S_H,
    DX100_LFO_WAVE_MAX,
};
typedef INT DX100_LFO_WAVE;


typedef struct
{
    BYTE ame       ;
    BYTE eg_bias   ;
    BYTE velocity  ;
    BYTE freq_ratio;
    BYTE detune    ;
    BYTE ar        ;
    BYTE d1r       ;
    BYTE d1l       ;
    BYTE d2r       ;
    BYTE rr        ;
    BYTE out_level ;
    BYTE rate      ;
    BYTE level     ;
} S_DX100_OPERATOR_PARAM;

typedef INT DX100_VOICE_NUM;
#define DX100_VOICE_NUM_MAX 32

typedef struct
{
    TCHAR patchname[11]            ;
    BYTE  algorithm                ;
    BYTE  feedback                 ;
    BYTE  lfo_wave                 ;
    BYTE  speed                    ;
    BYTE  delay                    ;
    BYTE  pmd                      ;
    BYTE  amd                      ;
    BYTE  lfo_sync                 ;
    BYTE  pitch                    ;
    BYTE  amplitude                ;
    BYTE  pr1                      ;
    BYTE  pl1                      ;
    BYTE  pr2                      ;
    BYTE  pl2                      ;
    BYTE  pr3                      ;
    BYTE  pl3                      ;
    BYTE  poly_mono                ;
    BYTE  pb_range                 ;
    BYTE  porta_mode               ;
    BYTE  porta_time               ;
    BYTE  portament_foot_sw        ;
    BYTE  foot_volume_range        ;
    BYTE  sustain_foot_sw          ;
    BYTE  wr_pitch                 ;
    BYTE  wr_amplitude             ;
    BYTE  bc_pitch                 ;
    BYTE  bc_amplitude             ;
    BYTE  bc_pitch_bias            ;
    BYTE  bc_eg_bias               ;
    BYTE  chorus_switch            ;
    BYTE  transpose                ;
    S_DX100_OPERATOR_PARAM operatorParam[DX100_OPERATOR_MAX];
} S_DX100_VOICE_PARAM;

BOOL Dx100CtrlInit( HINSTANCE hInst, PTSTR szAppName, HWND hwnd );

enum
{
    DX100_CTRL_MODE_NONE       ,
    DX100_CTRL_MODE_PATCH      ,
    DX100_CTRL_MODE_ALL_VOICE  ,
    DX100_CTRL_MODE_NUM_MAX
};
typedef INT DX100_CTRL_MODE;

BOOL Dx100CtrlModeSet(DX100_CTRL_MODE mode);

BOOL Dx100CtrlDisplayUpdate( void );

enum
{
    DX100_CTRL_SEQ_METHOD_GET,
    DX100_CTRL_SEQ_METHOD_SET,
    DX100_CTRL_SEQ_MEDHOD_NUM__MAX
};
typedef INT DX100_CTRL_SEQ_METHOD;

enum
{
    DX100_CTRL_SEQ_NON_EXEC    ,
    DX100_CTRL_SEQ_1VOICE      ,
    DX100_CTRL_SEQ_ALL_VOICE   ,
    DX100_CTRL_SEQ_NUM_MAX
};
typedef INT DX100_CTRL_SEQ_ID;

BOOL Dx100CtrlSeqStart( DX100_CTRL_SEQ_METHOD method, DX100_CTRL_SEQ_ID seqIdStart, DX100_CTRL_SEQ_ID seqIdEnd );
BOOL Dx100CtrlDisplayContents( void );
void Dx100CtrlCycleProc( void );

DWORD Dx100GetDataSize( DX100_CTRL_SEQ_ID seqId );
BOOL Dx100DataGet( DX100_CTRL_SEQ_ID seqId, TCHAR *dataPtr, DWORD dataSize );
BOOL Dx100DataSet( DX100_CTRL_SEQ_ID seqId, TCHAR *dataPtr, DWORD dataSize );
BOOL Dx100CtrlOnCommand( WORD code, WORD notifyCode, DX100_CTRL_MODE *modePtr, BOOL *extraModePtr );

S_DX100_VOICE_PARAM * Dx100GetVoiceParamPtr( DX100_VOICE_NUM voiceNum );

#endif /* DX100_CTRL_H */
