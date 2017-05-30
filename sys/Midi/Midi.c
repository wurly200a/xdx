/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */
#include <mmsystem.h>
#include "SomeCtrl.h"
#include "Config.h"

/* 外部関数定義 */

/* 外部変数定義 */

/* 内部関数定義 */
#include "Midi.h"
#include "DebugWnd.h"

static DWORD myMidiOutMessage( HMIDIOUT hMidi, int iStatus, int iChannel, int iData1,  int iData2 );

DWORD WINAPI ThreadSciReceive( void *data );

void CALLBACK MidiInProc(HMIDIIN hMidiIn,UINT wMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2);

static void receiveStartMidiExData( BYTE *dataPtr, INT *dataSizePtr );
static void receiveEndMidiExData( void );
static void transmitStartMidiExData( BYTE *dataPtr, INT dataSize );
static void transmitEndMidiExData( void );

/* 内部変数定義 */

typedef struct
{
    MIDIOUTCAPS moc;
    MIDIINCAPS  mic;
    HMIDIOUT hMidiOut;
    HMIDIIN  hMidiIn ;
    HMIDISTRM hMidiStream;
    BOOL     bMidiInReq;
    BOOL     bMidiOutReq;
    UINT     uDeviceId;
    INT      *midiInDataSizePtr;
} S_MIDI_INFO;

static S_MIDI_INFO midiInfo;

#define MIDI_EX_BUFFER_SIZE 1024
static MIDIHDR midiInExHeader;
static MIDIHDR midiOutExHeader;

#if 0
#define MIDI_DRV_DEBUG_PRINTF DebugWndPrintf
#else
#define MIDI_DRV_DEBUG_PRINTF
#endif

/*********************************************
 * 内容   : MIDIの初期化
 * 引数   : なし
 * 戻り値 : BOOL
 **********************************************/
BOOL
MidiInit( void )
{
    BOOL bRtn = TRUE;
    int i,iNumDevs,iCbNum;

    /* MIDI IN */
    if( 0 != ( iNumDevs = midiInGetNumDevs() ) )
    {
        for(i=0; i<iNumDevs; i++)
        {
            midiInGetDevCaps(i, &midiInfo.mic, sizeof(midiInfo.mic));
            iCbNum = SendMessage( SomeCtrlGetHWND(SOME_CTRL_MIDI_IN) , CB_ADDSTRING, 0, (LPARAM) midiInfo.mic.szPname );
            SendMessage( SomeCtrlGetHWND(SOME_CTRL_MIDI_IN)    , CB_SETITEMDATA, iCbNum, (LPARAM)i );

            iCbNum = SendMessage( SomeCtrlGetHWND(SOME_CTRL_MIDI_KEY_IN) , CB_ADDSTRING, 0, (LPARAM) midiInfo.mic.szPname );
            SendMessage( SomeCtrlGetHWND(SOME_CTRL_MIDI_KEY_IN), CB_SETITEMDATA, iCbNum, (LPARAM)i );
        }

        SendMessage( SomeCtrlGetHWND(SOME_CTRL_MIDI_IN)    , CB_SETCURSEL, ConfigLoadDword(CONFIG_ID_MIDI_IN_NUM)    , (LPARAM)0 );
        SendMessage( SomeCtrlGetHWND(SOME_CTRL_MIDI_KEY_IN), CB_SETCURSEL, ConfigLoadDword(CONFIG_ID_MIDI_KEY_IN_NUM), (LPARAM)0 );
    }
    else
    { /* error */
    }

    /* MIDI OUT */
    if( 0 != ( iNumDevs = midiOutGetNumDevs() ) )
    {
        if(!midiOutGetDevCaps(MIDIMAPPER, &midiInfo.moc, sizeof(midiInfo.moc)))
        {
            iCbNum = SendMessage( SomeCtrlGetHWND(SOME_CTRL_MIDI_OUT) , CB_ADDSTRING, 0, (LPARAM) midiInfo.moc.szPname );
            SendMessage( SomeCtrlGetHWND(SOME_CTRL_MIDI_OUT) , CB_SETITEMDATA, iCbNum, (LPARAM)MIDIMAPPER );
        }
        else
        {
            /* do nothing */
        }

        for(i=0; i<iNumDevs; i++)
        {
            midiOutGetDevCaps(i, &midiInfo.moc, sizeof(midiInfo.moc));
            iCbNum = SendMessage( SomeCtrlGetHWND(SOME_CTRL_MIDI_OUT) , CB_ADDSTRING, 0, (LPARAM) midiInfo.moc.szPname );
            SendMessage( SomeCtrlGetHWND(SOME_CTRL_MIDI_OUT) , CB_SETITEMDATA, iCbNum, (LPARAM)i );
        }

        SendMessage( SomeCtrlGetHWND(SOME_CTRL_MIDI_OUT) , CB_SETCURSEL, ConfigLoadDword(CONFIG_ID_MIDI_OUT_NUM), (LPARAM)0 );
    }
    else
    { /* error */
    }

    return bRtn;
}


/*********************************************
 * 内容   : MIDIのオープン
 * 引数   : UINT uDeviceId
 * 戻り値 : HMIDIOUT
 **********************************************/
HMIDIOUT
MidiOutOpen( UINT uDeviceId )
{
    MMRESULT result;
    static TCHAR szMidiDev[50];
    HWND hComboBox;

    if( midiInfo.hMidiOut )
    {
        nop();
    }
    else
    {
        result = midiOutOpen(&midiInfo.hMidiOut,uDeviceId,0,0,0);
        MIDI_DRV_DEBUG_PRINTF("open result:%d\r\n",result);

    }

    return midiInfo.hMidiOut;
}

/*********************************************
 * 内容   : MIDIのクローズ
 * 引数   : なし
 * 戻り値 : BOOL
 **********************************************/
BOOL
MidiOutClose( void )
{
    MMRESULT result;

    if( midiInfo.hMidiOut )
    {
        transmitEndMidiExData();

        result = midiOutReset(midiInfo.hMidiOut);
        MIDI_DRV_DEBUG_PRINTF("reset result:%d\r\n",result);

        result = midiOutClose(midiInfo.hMidiOut);
        MIDI_DRV_DEBUG_PRINTF("close result:%d\r\n",result);
        midiInfo.hMidiOut = NULL;
    }
    else
    {
        nop();
    }

    return TRUE;
}

/*********************************************
 * 内容   : MIDIのオープン
 * 引数   : UINT uDeviceId
 * 戻り値 : HMIDIIN
 **********************************************/
HMIDIIN
MidiInOpen( UINT uDeviceId )
{
    MMRESULT result;
    static TCHAR szMidiDev[50];
    HWND hComboBox;

    if( midiInfo.hMidiIn )
    {
        nop();
    }
    else
    {
        result = midiInOpen(&midiInfo.hMidiIn,uDeviceId,(DWORD_PTR)MidiInProc,uDeviceId,CALLBACK_FUNCTION);
        MIDI_DRV_DEBUG_PRINTF("open result:%d\r\n",result);
        if( !result )
        {
            midiInStart(midiInfo.hMidiIn);
        }
    }

    return midiInfo.hMidiIn;
}

/*********************************************
 * 内容   : MIDIのクローズ
 * 引数   : なし
 * 戻り値 : BOOL
 **********************************************/
BOOL
MidiInClose( void )
{
    MMRESULT result;

    if( midiInfo.hMidiIn )
    {
        receiveEndMidiExData();

        midiInStop(midiInfo.hMidiIn);

        result = midiInReset(midiInfo.hMidiIn);
        MIDI_DRV_DEBUG_PRINTF("reset result:%d\r\n",result);

        result = midiInClose(midiInfo.hMidiIn);
        MIDI_DRV_DEBUG_PRINTF("close result:%d\r\n",result);
        midiInfo.hMidiIn = NULL;
    }
    else
    {
        nop();
    }

    return TRUE;
}

/*********************************************
 * 内容   : MIDI エクスクルーシブデータ受信
 * 引数   : BYTE *dataPtr
 * 引数   : INT  *dataSizePtr
 * 戻り値 : BOOL
 **********************************************/
BOOL
MidiInExclusiveData( BYTE *dataPtr, INT *dataSizePtr )
{
    MMRESULT result;

    if( midiInfo.hMidiIn )
    {
        receiveStartMidiExData(dataPtr,dataSizePtr);
    }
    else
    {
        nop();
    }

    return TRUE;
}

/*********************************************
 * 内容   : MIDI エクスクルーシブデータ送信
 * 引数   : BYTE *dataPtr
 * 引数   : INT  dataSize
 * 戻り値 : BOOL
 **********************************************/
BOOL
MidiOutExclusiveData( BYTE *dataPtr, INT dataSize )
{
    MMRESULT result;

    if( midiInfo.hMidiOut )
    {
        transmitStartMidiExData(dataPtr,dataSize);
    }
    else
    {
        nop();
    }

    return TRUE;
}

static DWORD
myMidiOutMessage( HMIDIOUT hMidi, int iStatus, int iChannel, int iData1,  int iData2 )
{
     DWORD dwMessage;

     dwMessage = iStatus | iChannel | (iData1 << 8) | (iData2 << 16);

     return midiOutShortMsg( hMidi, dwMessage );
}

/*********************************************
 * 内容   :
 * 引数   : なし
 * 戻り値 : BOOL
 **********************************************/
BOOL
MidiNoteOn( void )
{
    MMRESULT result;
    DWORD dwMessage;

    if( midiInfo.hMidiOut )
    {
//        result = midiOutShortMsg(midiInfo.hMidiOut,dwMessage);
        result = myMidiOutMessage(midiInfo.hMidiOut,0x90,0,64,127);
    }
    else
    {
        nop();
    }

    return TRUE;
}

/*********************************************
 * 内容   :
 * 引数   : なし
 * 戻り値 : BOOL
 **********************************************/
BOOL
MidiNoteOff( void )
{
    MMRESULT result;
    DWORD dwMessage;

    if( midiInfo.hMidiOut )
    {
//        result = midiOutShortMsg(midiInfo.hMidiOut,dwMessage);
        result = myMidiOutMessage(midiInfo.hMidiOut,0x80,0,64,127);
    }
    else
    {
        nop();
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : 
 * 戻り値 : 
 **********************************************/
void
MidiCycleProc( void )
{
    DWORD i;

    if( midiInfo.bMidiInReq )
    {
        if( midiInExHeader.dwFlags & MHDR_DONE )
        {
            receiveEndMidiExData();
        }
        else
        {
            nop();
        }
    }
    else
    {
        nop();
    }

    if( midiInfo.bMidiOutReq )
    {
        if( midiOutExHeader.dwFlags & MHDR_DONE )
        {
            MIDI_DRV_DEBUG_PRINTF("midiOutExHeader.dwFlags & MHDR_DONE\r\n");

            transmitEndMidiExData();

            midiOutExHeader.dwFlags = 0;
        }
        else
        {
            nop();
        }
    }
    else
    {
        nop();
    }
}

/*********************************************
 * 内容   : 受信中チェック
 * 引数   : なし
 * 戻り値 : BOOL
 **********************************************/
BOOL
MidiInqInExec( void )
{
    return midiInfo.bMidiInReq;
}


/*********************************************
 * 内容   : 送信中チェック
 * 引数   : なし
 * 戻り値 : BOOL
 **********************************************/
BOOL
MidiInqOutExec( void )
{
    return midiInfo.bMidiOutReq;
}

/*********************************************
 * func   : 指定バイト数 受信
 * param  : BYTE   *dataPtr   データの格納領域
 * param  : WORD   size       データの格納サイズ
 * param  : DWORD  dwTimeout  タイムアウト時間(msec)
 * param  : HANDLE hExitEvent 終了イベントのハンドラ
 * return : BOOL
 *********************/
BOOL
MidiRcvByte( BYTE *dataPtr, WORD size, DWORD dwTimeout, HANDLE hExitEvent )
{
    return TRUE;
}

/*********************************************
 * 内容   : 指定バイト数 送信
 *
 * 引数   :  BYTE *dataPtr データの格納領域
 * 引数   :  WORD  size    データの格納サイズ
 *        :
 * 戻り値 : BOOL
 **********************************************/
BOOL
MidiSndByte( BYTE *dataPtr, WORD size )
{
    return TRUE;
}

/*********************************************
 * 内容   : シリアル受信処理を行うスレッド
 * 引数   : void *data (未使用)
 * 戻り値 : DWORD
 **********************************************/
DWORD WINAPI
ThreadMidiReceive( void *data )
{
    return (DWORD)0;
}

void CALLBACK
MidiInProc( HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 )
{
    MIDI_DRV_DEBUG_PRINTF("MidiInProc\r\n");
}

static void
receiveStartMidiExData( BYTE *dataPtr, INT *dataSizePtr )
{
    MMRESULT result;

    if( midiInfo.hMidiIn && !midiInfo.bMidiInReq )
    {
        midiInfo.midiInDataSizePtr = dataSizePtr;
        midiInfo.bMidiInReq = (BOOL)TRUE;

        memset((void *)&midiInExHeader,0,sizeof(MIDIHDR));
        midiInExHeader.lpData = (LPSTR)dataPtr;
        midiInExHeader.dwBufferLength = MIDI_EX_BUFFER_SIZE;

        result = midiInPrepareHeader(midiInfo.hMidiIn,&midiInExHeader,sizeof(MIDIHDR));
        MIDI_DRV_DEBUG_PRINTF("in prepare result:%d\r\n",result);

        result = midiInAddBuffer(midiInfo.hMidiIn,&midiInExHeader,sizeof(MIDIHDR));
        MIDI_DRV_DEBUG_PRINTF("addbufer result:%d\r\n",result);
    }
    else
    {
        nop();
    }
}

static void
receiveEndMidiExData( void )
{
    MMRESULT result;

    if( midiInfo.bMidiInReq )
    {
        if( midiInfo.midiInDataSizePtr != NULL )
        {
            *(midiInfo.midiInDataSizePtr) = midiInExHeader.dwBytesRecorded;
        }
        else
        {
            nop();
        }

        midiInfo.midiInDataSizePtr = (INT *)NULL;
        midiInfo.bMidiInReq = (BOOL)FALSE;
    }
    else
    {
        nop();
    }
}

static void
transmitStartMidiExData( BYTE *dataPtr, INT dataSize )
{
    MMRESULT result;

    if( midiInfo.hMidiOut && !midiInfo.bMidiOutReq )
    {
        midiInfo.bMidiOutReq = (BOOL)TRUE;

        memset((void *)&midiOutExHeader,0,sizeof(MIDIHDR));
        midiOutExHeader.lpData = (LPSTR)dataPtr;
        midiOutExHeader.dwBufferLength = MIDI_EX_BUFFER_SIZE;
        midiOutExHeader.dwBytesRecorded = dataSize;

        result = midiOutPrepareHeader(midiInfo.hMidiOut,&midiOutExHeader,sizeof(MIDIHDR));
        MIDI_DRV_DEBUG_PRINTF("out prepare result:%d\r\n",result);

        result = midiOutLongMsg(midiInfo.hMidiOut,&midiOutExHeader,sizeof(MIDIHDR));
        MIDI_DRV_DEBUG_PRINTF("midiOutLongMsg result:%d\r\n",result);
    }
    else
    {
        nop();
    }
}

static void
transmitEndMidiExData( void )
{
    MMRESULT result;

    if( midiInfo.bMidiOutReq )
    {
        result = midiOutUnprepareHeader(midiInfo.hMidiOut,&midiOutExHeader,sizeof(MIDIHDR));
        MIDI_DRV_DEBUG_PRINTF("out unprepare result:%d\r\n",result);

        midiInfo.bMidiOutReq = (BOOL)FALSE;
    }
    else
    {
        nop();
    }
}
