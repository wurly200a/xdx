#ifndef DX7_CTRL_H
#define DX7_CTRL_H

enum
{
    DX7_OPERATOR1,
    DX7_OPERATOR2,
    DX7_OPERATOR3,
    DX7_OPERATOR4,
    DX7_OPERATOR5,
    DX7_OPERATOR6,
    DX7_OPERATOR_MAX,
};
typedef INT DX7_OPERATOR_NUM;

typedef struct
{
    BYTE amplitude                ;
    BYTE freq_mode                ;
    BYTE coarse                   ;
    BYTE fine                     ;
    BYTE detune                   ;
    BYTE rate1                    ;
    BYTE rate2                    ;
    BYTE rate3                    ;
    BYTE rate4                    ;
    BYTE level1                   ;
    BYTE level2                   ;
    BYTE level3                   ;
    BYTE level4                   ;
    BYTE break_point              ;
    BYTE curve_l                  ;
    BYTE curve_r                  ;
    BYTE depth_l                  ;
    BYTE depth_r                  ;
    BYTE k_rate_scaling           ;
    BYTE out_level                ;
    BYTE velo_sens                ;
} S_DX7_OPERATOR_PARAM;

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
    BYTE  oscillator_sync          ;
    BYTE  peg_rate1                ;
    BYTE  peg_rate2                ;
    BYTE  peg_rate3                ;
    BYTE  peg_rate4                ;
    BYTE  peg_level1               ;
    BYTE  peg_level2               ;
    BYTE  peg_level3               ;
    BYTE  peg_level4               ;
    BYTE  key_transpose            ;
    S_DX7_OPERATOR_PARAM operatorParam[DX7_OPERATOR_MAX];
} S_DX7_VOICE_PARAM;

BOOL Dx7CtrlInit( HINSTANCE hInst, PTSTR szAppName, HWND hwnd );

enum
{
    DX7_CTRL_MODE_NONE       ,
    DX7_CTRL_MODE_PATCH      ,
    DX7_CTRL_MODE_ALL_VOICE  ,
    DX7_CTRL_MODE_NUM_MAX
};
typedef INT DX7_CTRL_MODE;

BOOL Dx7CtrlModeSet(DX7_CTRL_MODE mode);

BOOL Dx7CtrlDisplayUpdate( void );

enum
{
    DX7_CTRL_SEQ_METHOD_GET,
    DX7_CTRL_SEQ_METHOD_SET,
    DX7_CTRL_SEQ_MEDHOD_NUM__MAX
};
typedef INT DX7_CTRL_SEQ_METHOD;

enum
{
    DX7_CTRL_SEQ_NON_EXEC    ,
    DX7_CTRL_SEQ_1VOICE      ,
    DX7_CTRL_SEQ_ALL_VOICE   ,
    DX7_CTRL_SEQ_NUM_MAX
};
typedef INT DX7_CTRL_SEQ_ID;

BOOL Dx7CtrlSeqStart( DX7_CTRL_SEQ_METHOD method, DX7_CTRL_SEQ_ID seqIdStart, DX7_CTRL_SEQ_ID seqIdEnd );
BOOL Dx7CtrlDisplayContents( void );
void Dx7CtrlCycleProc( void );

DWORD Dx7GetDataSize( DX7_CTRL_SEQ_ID seqId );
BOOL Dx7DataGet( DX7_CTRL_SEQ_ID seqId, TCHAR *dataPtr, DWORD dataSize );
BOOL Dx7DataSet( DX7_CTRL_SEQ_ID seqId, TCHAR *dataPtr, DWORD dataSize );
BOOL Dx7CtrlOnCommand( WORD code, WORD notifyCode, DX7_CTRL_MODE *modePtr );
S_DX7_VOICE_PARAM * Dx7GetVoiceParamPtr( void );
void Dx7VoiceParamCopyFromStruct( void );
BOOL Dx7voiceCopyFromOneToAll( INT voiceNum );

#endif /* DX7_CTRL_H */
