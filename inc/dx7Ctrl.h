#ifndef DX7_CTRL_H
#define DX7_CTRL_H

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
    DX7_CTRL_SEQ_SYS_COMMON  ,
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
BOOL Dx7CtrlOnCommand( WORD code );

#endif /* DX7_CTRL_H */
