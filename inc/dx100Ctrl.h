#ifndef DX100_CTRL_H
#define DX100_CTRL_H

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
    DX100_CTRL_SEQ_SYS_COMMON  ,
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
BOOL Dx100CtrlOnCommand( WORD code, WORD notifyCode, DX100_CTRL_MODE *modePtr );

#endif /* DX100_CTRL_H */
