#ifndef DX100_CTRL_H
#define DX100_CTRL_H

BOOL Dx100CtrlInit( HWND hWndEdit );

enum
{
    DX100_CTRL_MODE_SYSTEM     ,
    DX100_CTRL_MODE_PATCH      ,
    DX100_CTRL_MODE_ALL_VOICE  ,
    DX100_CTRL_MODE_NUM_MAX
};
typedef INT DX100_CTRL_MODE;

enum
{
    DX100_CTRL_1VOICE_SUBMODE_COMMON,
    DX100_CTRL_1VOICE_SUBMODE_NUM_MAX
};
typedef INT DX100_CTRL_1VOICE_SUBMODE;

enum
{
    DX100_CTRL_ALL_VOICE_SUBMODE_COMMON,
    DX100_CTRL_ALL_VOICE_SUBMODE_NUM_MAX
};
typedef INT DX100_CTRL_ALL_VOICE_SUBMODE;

BOOL Dx100CtrlModeSet(DX100_CTRL_MODE mode,DX100_CTRL_1VOICE_SUBMODE patchSubMode,DX100_CTRL_ALL_VOICE_SUBMODE performanceSubMode);

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
    DX100_CTRL_SEQ_SYS_COMMON  ,
    DX100_CTRL_SEQ_1VOICE      ,
    DX100_CTRL_SEQ_NUM_MAX
};
typedef INT DX100_CTRL_SEQ_ID;

BOOL Dx100CtrlSeqStart( DX100_CTRL_SEQ_METHOD method, DX100_CTRL_SEQ_ID seqIdStart, DX100_CTRL_SEQ_ID seqIdEnd );
BOOL Dx100CtrlDisplayContents( void );
void Dx100CtrlCycleProc( void );

#endif /* DX100_CTRL_H */
