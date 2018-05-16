/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */
#include "SomeCtrl.h"
#include "dx7ParamCtrl.h"
#include "Midi.h"
#include "dx7Ctrl.h"
#include "DebugWnd.h"
#include "dx7CtrlDef.h"

/* 外部関数定義 */

/* 外部変数定義 */

/* 内部関数定義 */
static BOOL seqStart( DX7_CTRL_SEQ_METHOD method, DX7_CTRL_SEQ_ID seqId );
static INT seqStartProc( DX7_CTRL_SEQ_METHOD method, DX7_CTRL_SEQ_ID seqId, INT maxDataSize, BYTE *txDataPtr );
static BOOL seqEndProc( DX7_CTRL_SEQ_ID seqId, INT rxDataSize, BYTE *rxDataPtr );
static BOOL copyToParamCtrl( DX7_CTRL_SEQ_ID seqId );
static BOOL copyFromParamCtrl( DX7_CTRL_SEQ_ID seqId );
static BYTE getParamCtrlValue( DX7_PARAM_CTRL_ID id );
static BOOL displayContents( void );
static BYTE calcCheckSum( BYTE *dataPtr, INT dataSize );
static BOOL dx7voiceCopyFromAllToOne( INT voiceNum );
static void debugDataArrayPrint( INT rxDataSize, BYTE *rxDataPtr, PTSTR ptstrTitle );

/* 内部変数定義 */
static BYTE dx7CtrlSeqTxTempData[1024*8];
static BYTE dx7CtrlSeqRxTempData[1024*8];
static INT dx7CtrlSeqTempRxSize;

static BYTE dx7CtrlDataSysCommon[SYSCMN_INDEX_MAX];
static BYTE dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_INDEX_MAX];
static BYTE dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_INDEX_MAX];

typedef struct
{
    INT   rxDataSize  ; /* 受信データ格納領域のサイズ               */
    BYTE *rxDataPtr   ; /* 受信データ格納領域のポインタ             */
} S_DX7_CTRL_SEQ_DATA;

static const S_DX7_CTRL_SEQ_DATA dx7CtrlSeqDataTbl[DX7_CTRL_SEQ_NUM_MAX] =
{
    {(INT)0                              ,(BYTE *)NULL                   }, /* DX7_CTRL_SEQ_NON_EXEC     */
    {(INT)SYSCMN_INDEX_MAX               ,&dx7CtrlDataSysCommon        }, /* DX7_CTRL_SEQ_SYS_COMMON   */
    {(INT)DX7_SYSEX_1VOICE_INDEX_MAX   ,&dx7CtrlDataOneVoice         }, /* DX7_CTRL_SEQ_1VOICE */
    {(INT)DX7_SYSEX_ALL_VOICE_INDEX_MAX,&dx7CtrlDataAllVoice         }, /* DX7_CTRL_SEQ_ALL_VOICE */
};

typedef struct
{
    DX7_CTRL_SEQ_METHOD          nowMethod            ;
    DX7_CTRL_SEQ_ID              reqSeqIdStart        ;
    DX7_CTRL_SEQ_ID              reqSeqIdEnd          ;
    DX7_CTRL_SEQ_ID              nowExecSeqId         ;
    DX7_CTRL_MODE                nowMode              ;
} S_DX7_CTRL_INFO;

static S_DX7_CTRL_INFO dx7CtrlInfo;


/*********************************************
 * 内容   : 初期化
 * 引数   : HINSTANCE hInst
 * 引数   : PTSTR szAppName
 * 引数   : HWND hwnd
 * 戻り値 : BOOL
 **********************************************/
BOOL
Dx7CtrlInit( HINSTANCE hInst, PTSTR szAppName, HWND hwnd )
{
    BOOL bRtn = TRUE;

    dx7CtrlInfo.nowMode = DX7_CTRL_MODE_PATCH;

    Dx7ParamCtrlCreate( hInst, szAppName, hwnd ); /* コントロールを生成 */
    Dx7ParamCtrlGroupDisplay(DX7_PARAM_CTRL_GROUP_1VOICE);

    return bRtn;
}

/*********************************************
 * 内容   : 
 * 引数   : DX7_CTRL_MODE mode
 * 戻り値 : BOOL
 **********************************************/
BOOL
Dx7CtrlModeSet(DX7_CTRL_MODE mode)
{
    dx7CtrlInfo.nowMode = mode;

    Dx7CtrlDisplayUpdate();

    return TRUE;
}

BOOL
Dx7CtrlDisplayUpdate( void )
{
    if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_NONE )
    {
        Dx7ParamCtrlGroupDisplay(DX7_PARAM_CTRL_GROUP_NONE);
    }
    else if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_PATCH )
    {
        Dx7ParamCtrlGroupDisplay(DX7_PARAM_CTRL_GROUP_1VOICE);
    }
    else if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_ALL_VOICE )
    {
        Dx7ParamCtrlGroupDisplay(DX7_PARAM_CTRL_GROUP_ALL_VOICE);
    }
    else
    {
    }

    return TRUE;
}

/**
 * ------------------------------------------------------------------------------
 * @brief  シーケンス開始
 *         開始IDから終了IDまで連続して実行する
 * @param  method
 * @param  seqIdStart
 * @param  seqIdEnd
 * @return TRUE: 正常に実行開始
 * @author wurly @date 2017年06月03日 21時16分31秒
 */
BOOL
Dx7CtrlSeqStart( DX7_CTRL_SEQ_METHOD method, DX7_CTRL_SEQ_ID seqIdStart, DX7_CTRL_SEQ_ID seqIdEnd )
{
    BOOL bRtn = TRUE;

    if( (seqIdStart && (seqIdStart < DX7_CTRL_SEQ_NUM_MAX)) &&
        (seqIdEnd   && (seqIdEnd < DX7_CTRL_SEQ_NUM_MAX)) &&
        (seqIdStart <= seqIdEnd) )
    {
        dx7CtrlInfo.nowMethod     = method;
        dx7CtrlInfo.reqSeqIdStart = seqIdStart;
        dx7CtrlInfo.reqSeqIdEnd   = seqIdEnd  ;

        seqStart(method,dx7CtrlInfo.reqSeqIdStart);
    }
    else
    {
        nop();
    }

    return bRtn;
}

/*********************************************
 * 内容   : コンテンツ表示
 * 引数   : なし
 * 戻り値 : BOOL
 **********************************************/
BOOL
Dx7CtrlDisplayContents( void )
{
    return displayContents();
}

/*********************************************
 * 内容   : 
 * 引数   : 
 * 戻り値 : 
 **********************************************/
void
Dx7CtrlCycleProc( void )
{
    if( dx7CtrlInfo.nowExecSeqId )
    {
        if( MidiInqOutExec() )
        {
            nop();
        }
        else
        { /* 送信終了 */
            DebugWndPrintf("TX_END\r\n");

            if( dx7CtrlInfo.nowMethod == DX7_CTRL_SEQ_METHOD_SET )
            {
                if( dx7CtrlInfo.nowExecSeqId < dx7CtrlInfo.reqSeqIdEnd )
                { /* 次のシーケンスがある */
                    seqStart(dx7CtrlInfo.nowMethod,dx7CtrlInfo.nowExecSeqId+1);
                }
                else
                {
                    dx7CtrlInfo.reqSeqIdStart = DX7_CTRL_SEQ_NON_EXEC;
                    dx7CtrlInfo.reqSeqIdEnd   = DX7_CTRL_SEQ_NON_EXEC;
                    dx7CtrlInfo.nowExecSeqId  = DX7_CTRL_SEQ_NON_EXEC;
                }
            }
            else
            { /* GETの時 */
                nop();
            }
        }

        if( MidiInqInExec() )
        {
            nop();
        }
        else
        { /* 受信終了 */
            if( dx7CtrlInfo.nowMethod == DX7_CTRL_SEQ_METHOD_GET )
            {
                DebugWndPrintf("RX_END\r\n");
                seqEndProc(dx7CtrlInfo.nowExecSeqId,dx7CtrlSeqTempRxSize,dx7CtrlSeqRxTempData);
            }
            else
            { /* SETの時 */
                DebugWndPrintf("RX_NOT_EXEC\r\n");
            }

            if( dx7CtrlInfo.nowExecSeqId < dx7CtrlInfo.reqSeqIdEnd )
            { /* 次のシーケンスがある */
                seqStart(dx7CtrlInfo.nowMethod,dx7CtrlInfo.nowExecSeqId+1);
            }
            else
            {
                /* 全てのシーケンス終了 */
                dx7CtrlInfo.reqSeqIdStart = DX7_CTRL_SEQ_NON_EXEC;
                dx7CtrlInfo.reqSeqIdEnd   = DX7_CTRL_SEQ_NON_EXEC;
                dx7CtrlInfo.nowExecSeqId  = DX7_CTRL_SEQ_NON_EXEC;

                if( dx7CtrlInfo.nowMethod == DX7_CTRL_SEQ_METHOD_GET )
                {
                    if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_NONE )
                    {
                    }
                    else if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_PATCH )
                    {
                        copyToParamCtrl(DX7_CTRL_SEQ_1VOICE);
                    }
                    else if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_ALL_VOICE )
                    {
                        copyToParamCtrl(DX7_CTRL_SEQ_ALL_VOICE);
                    }
                    else
                    {
                    }

                    Dx7CtrlDisplayUpdate();
                }
                else
                {
                    nop();
                }
            }
        }
    }
    else
    {
        nop();
    }
}

/*********************************************
 * 内容   : シーケンス開始
 * 引数   : 
 * 戻り値 : BOOL
 **********************************************/
static BOOL
seqStart( DX7_CTRL_SEQ_METHOD method, DX7_CTRL_SEQ_ID seqId )
{
    INT txSize;

    if( seqId && (seqId < DX7_CTRL_SEQ_NUM_MAX) )
    {
        DebugWndPrintf("..............\r\n");
        DebugWndPrintf("DX7_CTRL_SEQ_ID:%d\r\n",seqId);

        txSize = seqStartProc(method,seqId,1024,dx7CtrlSeqTxTempData);

        debugDataArrayPrint(txSize,dx7CtrlSeqTxTempData,"TX");

        dx7CtrlInfo.nowExecSeqId = seqId;

        if( method == DX7_CTRL_SEQ_METHOD_SET )
        {
            nop();
        }
        else
        {
            MidiInExclusiveData( &dx7CtrlSeqRxTempData, &dx7CtrlSeqTempRxSize );
        }

        MidiOutExclusiveData( &dx7CtrlSeqTxTempData, txSize );
    }
    else
    {
        nop();
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : DX7_CTRL_SEQ_ID seqId
 * 戻り値 : INT
 **********************************************/
static INT
seqStartProc( DX7_CTRL_SEQ_METHOD method, DX7_CTRL_SEQ_ID seqId, INT maxDataSize, BYTE *txDataPtr )
{
    S_DX7_CTRL_SEQ_DATA *tblPtr;
    INT txSize = (INT)0;
    INT i;

    tblPtr = &(dx7CtrlSeqDataTbl[seqId]);

    switch( seqId )
    {
    case DX7_CTRL_SEQ_1VOICE:
        if( method == DX7_CTRL_SEQ_METHOD_SET )
        {
            copyFromParamCtrl( seqId );
            memcpy((void *)txDataPtr,(void *)tblPtr->rxDataPtr,tblPtr->rxDataSize);
            txSize = DX7_SYSEX_1VOICE_INDEX_MAX;
        }
        else
        { /* データ要求しデータ受信する*/
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_STATUS    ) = EX_STATUS                   ;
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_ID_NO     ) = EX_ID_NUMBER_YAMAHA         ;
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_SUB_STATUS) = 0x20               | 0x00/* MIDI CH*/;
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_FORMAT_NO ) = DX7_DUMP_REQ_FORMAT_1VOICE;
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_EOX       ) = EX_ETX                      ;
            txSize = DX7_SYSEX_BULK_DUMP_REQUEST_INDEX_MAX;
        }
        break;
    case DX7_CTRL_SEQ_ALL_VOICE:
        if( method == DX7_CTRL_SEQ_METHOD_SET )
        {
            memcpy((void *)txDataPtr,(void *)tblPtr->rxDataPtr,tblPtr->rxDataSize);
            txSize = DX7_SYSEX_ALL_VOICE_INDEX_MAX;
        }
        else
        { /* データ要求しデータ受信する*/
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_STATUS    ) = EX_STATUS                    ;
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_ID_NO     ) = EX_ID_NUMBER_YAMAHA          ;
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_SUB_STATUS) = 0x20               | 0x00/* MIDI CH*/;
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_FORMAT_NO ) = DX7_DUMP_REQ_FORMAT_32VOICE;
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_EOX       ) = EX_ETX                       ;
            txSize = DX7_SYSEX_BULK_DUMP_REQUEST_INDEX_MAX;
        }
        break;
    }

    return txSize;
}

/*********************************************
 * 内容   : 
 * 引数   : DX7_CTRL_SEQ_ID seqId
 * 戻り値 : BOOL
 **********************************************/
static BOOL
seqEndProc( DX7_CTRL_SEQ_ID seqId, INT rxDataSize, BYTE *rxDataPtr )
{
    S_DX7_CTRL_SEQ_DATA *tblPtr = &(dx7CtrlSeqDataTbl[seqId]);

//    DebugWndPrintf("rxDataSize,%d\r\n",rxDataSize);

    debugDataArrayPrint(rxDataSize,rxDataPtr,"RX");

    if( rxDataSize == tblPtr->rxDataSize )
    {
        memcpy((void *)tblPtr->rxDataPtr,(void *)rxDataPtr,tblPtr->rxDataSize);

        switch( seqId )
        {
        case DX7_CTRL_SEQ_1VOICE:
            debugDataArrayPrint(DX7_SYSEX_VCED_MAX,tblPtr->rxDataPtr+DX7_SYSEX_1VOICE_DATA,"CONTENTS");
            break;
        case DX7_CTRL_SEQ_ALL_VOICE:
            debugDataArrayPrint(4096,tblPtr->rxDataPtr+DX7_SYSEX_ALL_VOICE_DATA,"CONTENTS");
            break;
        default:
            break;
        }
    }
    else
    {
        DebugWndPrintf("Check NG,%d,%d\r\n",rxDataSize,tblPtr->rxDataSize);
    }

    switch( seqId )
    {
    case DX7_CTRL_SEQ_1VOICE:
        break;
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : DX7_CTRL_SEQ_ID seqId
 * 戻り値 : BOOL
 **********************************************/
static BOOL
copyToParamCtrl( DX7_CTRL_SEQ_ID seqId )
{
    TCHAR patchName[12+1];
    TCHAR szBuffer[1024];
    INT toneNum;
    INT i,j;

    switch( seqId )
    {
    case DX7_CTRL_SEQ_SYS_COMMON  :
        break;
    case DX7_CTRL_SEQ_1VOICE:
        for( i=0; i<DX7_SYSEX_VCED_MAX; i++ )
        {
            INT dataIndex = DX7_SYSEX_1VOICE_DATA+i/*DX7_SYSEX_VCED_00...DX7_SYSEX_VCED_92*/;
            INT paramCtrlIndex;

            if( i < DX7_SYSEX_VCED_77 )
            {
                paramCtrlIndex = DX7_PARAM_CTRL_VOICE_00 + i/*DX7_PARAM_CTRL_VOICE_00...DX7_PARAM_CTRL_VOICE_76*/;

                SendMessage( Dx7ParamCtrlGetHWND(paramCtrlIndex), CB_SETCURSEL, dx7CtrlDataOneVoice[dataIndex], (LPARAM)0 );
            }
            else if( (DX7_SYSEX_VCED_77 <= i) && (i <= DX7_SYSEX_VCED_86) )
            {
                if( DX7_SYSEX_VCED_77 == i )
                {
                    memset(&patchName[0],0,10+1);
                    strncpy(&patchName[0],&dx7CtrlDataOneVoice[dataIndex],10);

                    for( j=9; 0<=j; j++ )
                    {
                        if( patchName[j] == ' ' )
                        {
                            patchName[j] = '\0';
                        }
                        else
                        {
                            break;
                        }
                    }
                    SetWindowText( Dx7ParamCtrlGetHWND(DX7_PARAM_CTRL_VOICE_NAME),&patchName[0]);
                }
                else
                {
                }
            }
            else
            {
                paramCtrlIndex = DX7_PARAM_CTRL_VOICE_87 + (i-DX7_SYSEX_VCED_87)/*DX7_PARAM_CTRL_VOICE_87...DX7_PARAM_CTRL_VOICE_92*/;

                SendMessage( Dx7ParamCtrlGetHWND(paramCtrlIndex), CB_SETCURSEL, dx7CtrlDataOneVoice[dataIndex], (LPARAM)0 );
            }

        }
        break;
    case DX7_CTRL_SEQ_ALL_VOICE:
        for( i=0; i<32; i++ )
        {
            INT j;
            memset(&patchName[0],0,10+1);
            strncpy(&patchName[0],&dx7CtrlDataAllVoice[ DX7_SYSEX_ALL_VOICE_DATA + (i*DX7_SYSEX_VMEM_MAX) + DX7_SYSEX_VMEM_57],10);
            SetWindowText( Dx7ParamCtrlGetHWND(DX7_PARAM_CTRL_ALL_VOICE_NAME_00+i),&patchName[0]);
#if 0 /* パッチ名を出力 */
            DebugWndPrintf("%s\r\n",&patchName[0]);
#endif
        }

        for( i=0; i<32; i++ )
        {
            INT j;
            memset(&szBuffer[0],0,128*2+1);
            for( j=0;j<128; j++ )
            {
                sprintf(&szBuffer[j*2],"%02X",dx7CtrlDataAllVoice[ DX7_SYSEX_ALL_VOICE_DATA + (i*DX7_SYSEX_VMEM_MAX) + j]);
            }
//            SetWindowText( Dx7ParamCtrlGetHWND(DX7_PARAM_CTRL_ALL_VOICE_BULK_00+i),&szBuffer[0]);

#if 0 /* バルクダンプのテキストを出力 */
            DebugWndPrintf("%s\r\n",&szBuffer[0]);
#endif
        }
        break;
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : DX7_CTRL_SEQ_ID seqId
 * 戻り値 : BOOL
 **********************************************/
static BOOL
copyFromParamCtrl( DX7_CTRL_SEQ_ID seqId )
{
    TCHAR patchName[12+1];
    TCHAR szBuffer[1024];
    INT toneNum;
    INT i,j;
    BYTE checkSum;

    switch( seqId )
    {
    case DX7_CTRL_SEQ_1VOICE:
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_REQUEST_STATUS    ] = EX_STATUS;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_REQUEST_ID_NO     ] = EX_ID_NUMBER_YAMAHA;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_REQUEST_SUB_STATUS] = 0x00;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_REQUEST_FORMAT_NO ] = DX7_DUMP_REQ_FORMAT_1VOICE;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_BYTE_COUNT_HIGH   ] = 0x00;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_BYTE_COUNT_LOW    ] = 0x5D;

        for( i=0; i<DX7_SYSEX_VCED_MAX; i++ )
        {
            INT dataIndex = DX7_SYSEX_1VOICE_DATA+i/*DX7_SYSEX_VCED_00...DX7_SYSEX_VCED_92*/;
            INT paramCtrlIndex;

            if( i < DX7_SYSEX_VCED_77 )
            {
                paramCtrlIndex = DX7_PARAM_CTRL_VOICE_00 + i/*DX7_PARAM_CTRL_VOICE_00...DX7_PARAM_CTRL_VOICE_76*/;

                dx7CtrlDataOneVoice[dataIndex] = getParamCtrlValue(paramCtrlIndex);
            }
            else if( (DX7_SYSEX_VCED_77 <= i) && (i <= DX7_SYSEX_VCED_86) )
            {
                if( DX7_SYSEX_VCED_77 == i )
                {
                    memset(&patchName[0],0,10+1);
                    Dx7ParamCtrlGetText(DX7_PARAM_CTRL_VOICE_NAME,patchName);
                    memset(&dx7CtrlDataOneVoice[dataIndex],0,10+1);
                    strncpy(&dx7CtrlDataOneVoice[dataIndex],&patchName[0],10);

                    for( j=0; j<10; j++ )
                    {
                        if( dx7CtrlDataOneVoice[dataIndex+j] == '\0' )
                        {
                            dx7CtrlDataOneVoice[dataIndex+j] = ' ';
                        }
                        else
                        {
                            nop();
                        }
                    }
                }
                else
                {
                }
            }
            else
            {
                paramCtrlIndex = DX7_PARAM_CTRL_VOICE_87 + (i-DX7_SYSEX_VCED_87)/*DX7_PARAM_CTRL_VOICE_87...DX7_PARAM_CTRL_VOICE_92*/;

                dx7CtrlDataOneVoice[dataIndex] = getParamCtrlValue(paramCtrlIndex);
            }

        }
        checkSum = calcCheckSum(&(dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA]),DX7_SYSEX_VCED_MAX);
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_FOOTER_CHECKSUM] = checkSum;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_FOOTER_ETX] = EX_ETX;

        DebugWndPrintf("checkSum:0x%02X\r\n",checkSum);
        break;
    case DX7_CTRL_SEQ_ALL_VOICE:
        break;
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : DX7_CTRL_SEQ_ID seqId
 * 戻り値 : BOOL
 **********************************************/
static BYTE
getParamCtrlValue( DX7_PARAM_CTRL_ID id )
{
    BYTE value = (BYTE)0x00;
    int iCbNum;
    HWND hComboBox;

    hComboBox = Dx7ParamCtrlGetHWND(id);
    if( hComboBox )
    {
        value = SendMessage(hComboBox,CB_GETCURSEL,0,0);
    }
    else
    {
        nop();
    }

    return value;
}

/*********************************************
 * 内容   : 
 * 引数   : 
 * 戻り値 : 
 **********************************************/
static BOOL
displayContents( void )
{
    INT i;

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : 
 * 戻り値 : 
 **********************************************/
static BYTE
calcCheckSum( BYTE *dataPtr, INT dataSize )
{
    BYTE checkSum = (BYTE)0;
    INT i;

    for( i=0,checkSum=0; i<dataSize; i++ )
    {
        checkSum += *(dataPtr+i);
    }
    checkSum & 0x7F;
    checkSum = (0x80 - checkSum) & 0x7F;

    return checkSum;
}

/********************************************************************************
 * 内容  : データサイズ取得
 * 引数  : seqId
 * 戻り値: DWORD
 ***************************************/
DWORD
Dx7GetDataSize( DX7_CTRL_SEQ_ID seqId )
{
    S_DX7_CTRL_SEQ_DATA *tblPtr;
    DWORD dwSize = (DWORD)0;

    if( seqId < DX7_CTRL_SEQ_NUM_MAX )
    {
        tblPtr = &(dx7CtrlSeqDataTbl[seqId]);
        dwSize = tblPtr->rxDataSize;
    }
    else
    {
    }

    return dwSize;
}

/********************************************************************************
 * 内容  : データ取得
 * 引数  : seqId
 * 引数  : TCHAR *dataPtr
 * 引数  : DWORD dataSize
 * 戻り値: BOOL
 ***************************************/
BOOL
Dx7DataGet( DX7_CTRL_SEQ_ID seqId, TCHAR *dataPtr, DWORD dataSize )
{
    S_DX7_CTRL_SEQ_DATA *tblPtr;
    DWORD dwSize = (DWORD)0;

    if( seqId < DX7_CTRL_SEQ_NUM_MAX )
    {
        copyFromParamCtrl( seqId );

        tblPtr = &(dx7CtrlSeqDataTbl[seqId]);
        dwSize = tblPtr->rxDataSize;

        if( dwSize <= dataSize )
        {
            memcpy(dataPtr,tblPtr->rxDataPtr,tblPtr->rxDataSize);
        }
        else
        {
        }
    }
    else
    {
    }

    return TRUE;
}

/********************************************************************************
 * 内容  : データセット
 * 引数  : seqId
 * 引数  : TCHAR *dataPtr
 * 引数  : DWORD dataSize
 * 戻り値: BOOL
 ***************************************/
BOOL
Dx7DataSet( DX7_CTRL_SEQ_ID seqId, TCHAR *dataPtr, DWORD dataSize )
{
    S_DX7_CTRL_SEQ_DATA *tblPtr;
    DWORD dwSize = (DWORD)0;

    if( seqId < DX7_CTRL_SEQ_NUM_MAX )
    {
        tblPtr = &(dx7CtrlSeqDataTbl[seqId]);
        dwSize = tblPtr->rxDataSize;

        if( dwSize <= dataSize )
        {
            memcpy(tblPtr->rxDataPtr,dataPtr,tblPtr->rxDataSize);
        }
        else
        {
        }

        if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_NONE )
        {
        }
        else if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_PATCH )
        {
            copyToParamCtrl(DX7_CTRL_SEQ_1VOICE);
        }
        else if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_ALL_VOICE )
        {
            copyToParamCtrl(DX7_CTRL_SEQ_ALL_VOICE);
        }
        else
        {
        }
    }
    else
    {
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : 
 * 戻り値 : 
 **********************************************/
BOOL
Dx7CtrlOnCommand( WORD code )
{
    BOOL bRtn = (BOOL)FALSE;

    switch( code )
    {
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_00+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_01+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_02+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_03+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_04+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_05+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_06+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_07+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_08+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_09+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_10+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_11+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_12+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_13+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_14+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_15+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_16+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_17+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_18+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_19+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_20+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_21+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_22+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_23+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_24+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_25+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_26+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_27+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_28+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_29+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_30+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_31+DX7_PARAM_CTRL_ID_OFFSET):
        DebugWndPrintf("TO 1VOICE,%d\r\n",code-(DX7_PARAM_CTRL_ID_OFFSET+DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_00));
        dx7voiceCopyFromAllToOne(code-(DX7_PARAM_CTRL_ID_OFFSET+DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_00));
        bRtn = (BOOL)TRUE;
        break;
    default:
        break;
    }

    return bRtn;
}

/********************************************************************************
 * 内容  : ボイスコピー(ALL VOICEから1VOICEへ)
 * 引数  : INT voiceNum
 * 戻り値: BOOL
 ***************************************/
static BOOL
dx7voiceCopyFromAllToOne( INT voiceNum )
{
    if( voiceNum < 32 )
    {
        INT topNum = DX7_SYSEX_ALL_VOICE_DATA + (voiceNum*DX7_SYSEX_VMEM_MAX);

        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_00]/* OP4 ATTACK RATE                                  */ = dx7CtrlDataAllVoice[topNum+ 0];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_01]/* OP4 DECAY 1 RATE                                 */ = dx7CtrlDataAllVoice[topNum+ 1];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_02]/* OP4 DECAY 2 RATE                                 */ = dx7CtrlDataAllVoice[topNum+ 2];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_03]/* OP4 RELEASE RATE                                 */ = dx7CtrlDataAllVoice[topNum+ 3];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_04]/* OP4 DECAY 1 LEVEL                                */ = dx7CtrlDataAllVoice[topNum+ 4];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_05]/* OP4 KEYBOARD SCALING LEVEL                       */ = dx7CtrlDataAllVoice[topNum+ 5];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_06]/* OP4 KEYBOARD SCALING RATE                        */ = (dx7CtrlDataAllVoice[topNum+ 9])>>3; /* BIT4-3 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_07]/* OP4 EG BIAS SENSITIVITY                          */ = ((dx7CtrlDataAllVoice[topNum+ 6])>>3)&0x7; /* BIT5-3 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_08]/* OP4 AMPLITUDE MODULATION ENABLE                  */ = ((dx7CtrlDataAllVoice[topNum+ 6])>>6)&0x1; /* BIT6   */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_09]/* OP4 KEY VELOCITY                                 */ = ((dx7CtrlDataAllVoice[topNum+ 6])   )&0x7; /* BIT2-0 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_10]/* OP4 OUTPUT LEVEL                                 */ = dx7CtrlDataAllVoice[topNum+ 7];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_11]/* OP4 OSCILLATOR FREQUENCY                         */ = dx7CtrlDataAllVoice[topNum+ 8];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_12]/* OP4 DETUNE                                       */ = (dx7CtrlDataAllVoice[topNum+ 9])&0x7; /* BIT2-0 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_13]/* OP2 ATTACK RATE                                  */ = dx7CtrlDataAllVoice[topNum+10];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_14]/* OP2 DECAY 1 RATE                                 */ = dx7CtrlDataAllVoice[topNum+11];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_15]/* OP2 DECAY 2 RATE                                 */ = dx7CtrlDataAllVoice[topNum+12];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_16]/* OP2 RELEASE RATE                                 */ = dx7CtrlDataAllVoice[topNum+13];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_17]/* OP2 DECAY 1 LEVEL                                */ = dx7CtrlDataAllVoice[topNum+14];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_18]/* OP2 KEYBOARD SCALING LEVEL                       */ = dx7CtrlDataAllVoice[topNum+15];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_19]/* OP2 KEYBOARD SCALING RATE                        */ = (dx7CtrlDataAllVoice[topNum+19])>>3; /* BIT4-3 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_20]/* OP2 EG BIAS SENSITIVITY                          */ = ((dx7CtrlDataAllVoice[topNum+16])>>3)&0x7; /* BIT5-3 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_21]/* OP2 AMPLITUDE MODULATION ENABLE                  */ = ((dx7CtrlDataAllVoice[topNum+16])>>6)&0x1; /* BIT6 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_22]/* OP2 KEY VELOCITY                                 */ = ((dx7CtrlDataAllVoice[topNum+16])   )&0x7; /* BIT2-0 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_23]/* OP2 OUTPUT LEVEL                                 */ = dx7CtrlDataAllVoice[topNum+17];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_24]/* OP2 OSCILLATOR FREQUENCY                         */ = dx7CtrlDataAllVoice[topNum+18];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_25]/* OP2 DETUNE                                       */ = (dx7CtrlDataAllVoice[topNum+19])&0x7; /* BIT2-0 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_26]/* OP3 ATTACK RATE                                  */ = dx7CtrlDataAllVoice[topNum+20];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_27]/* OP3 DECAY 1 RATE                                 */ = dx7CtrlDataAllVoice[topNum+21];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_28]/* OP3 DECAY 2 RATE                                 */ = dx7CtrlDataAllVoice[topNum+22];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_29]/* OP3 RELEASE RATE                                 */ = dx7CtrlDataAllVoice[topNum+23];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_30]/* OP3 DECAY 1 LEVEL                                */ = dx7CtrlDataAllVoice[topNum+24];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_31]/* OP3 KEYBOARD SCALING LEVEL                       */ = dx7CtrlDataAllVoice[topNum+25];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_32]/* OP3 KEYBOARD SCALING RATE                        */ = (dx7CtrlDataAllVoice[topNum+29])>>3; /* BIT4-3 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_33]/* OP3 EG BIAS SENSITIVITY                          */ = ((dx7CtrlDataAllVoice[topNum+26])>>3)&0x7; /* BIT5-3 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_34]/* OP3 AMPLITUDE MODULATION ENABLE                  */ = ((dx7CtrlDataAllVoice[topNum+26])>>6)&0x1; /* BIT6   */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_35]/* OP3 KEY VELOCITY                                 */ = ((dx7CtrlDataAllVoice[topNum+26])   )&0x7; /* BIT2-0 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_36]/* OP3 OUTPUT LEVEL                                 */ = dx7CtrlDataAllVoice[topNum+27];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_37]/* OP3 OSCILLATOR FREQUENCY                         */ = dx7CtrlDataAllVoice[topNum+28];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_38]/* OP3 DETUNE                                       */ = (dx7CtrlDataAllVoice[topNum+29])&0x7; /*BIT2-0*/
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_39]/* OP1 ATTACK RATE                                  */ = dx7CtrlDataAllVoice[topNum+30];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_40]/* OP1 DECAY 1 RATE                                 */ = dx7CtrlDataAllVoice[topNum+31];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_41]/* OP1 DECAY 2 RATE                                 */ = dx7CtrlDataAllVoice[topNum+32];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_42]/* OP1 RELEASE RATE                                 */ = dx7CtrlDataAllVoice[topNum+33];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_43]/* OP1 DECAY 1 LEVEL                                */ = dx7CtrlDataAllVoice[topNum+34];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_44]/* OP1 KEYBOARD SCALING LEVEL                       */ = dx7CtrlDataAllVoice[topNum+35];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_45]/* OP1 KEYBOARD SCALING RATE                        */ = (dx7CtrlDataAllVoice[topNum+39])>>3; /* BIT4-3 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_46]/* OP1 EG BIAS SENSITIVITY                          */ = ((dx7CtrlDataAllVoice[topNum+36])>>3)&0x7; /* BIT5-3 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_47]/* OP1 AMPLITUDE MODULATION ENABLE                  */ = ((dx7CtrlDataAllVoice[topNum+36])>>6)&0x1; /* BIT6   */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_48]/* OP1 KEY VELOCITY                                 */ = ((dx7CtrlDataAllVoice[topNum+36])   )&0x7; /* BIT2-0 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_49]/* OP1 OUTPUT LEVEL                                 */ = dx7CtrlDataAllVoice[topNum+37];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_50]/* OP1 OSCILLATOR FREQUENCY                         */ = dx7CtrlDataAllVoice[topNum+38];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_51]/* OP1 DETUNE                                       */ = (dx7CtrlDataAllVoice[topNum+39])&0x7; /* BIT2-0 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_52]/* ALGORITHM                                        */ = (dx7CtrlDataAllVoice[topNum+40])&0x7; /* BIT2-0 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_53]/* FEEDBACK LEVEL                                   */ = ((dx7CtrlDataAllVoice[topNum+40])>>3)&0x7; /* BIT5-3 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_54]/* LFO SPEED                                        */ = dx7CtrlDataAllVoice[topNum+41];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_55]/* LFO DELAY                                        */ = dx7CtrlDataAllVoice[topNum+42];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_56]/* PITCH MODULATION DEPTH                           */ = dx7CtrlDataAllVoice[topNum+43];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_57]/* amplitude MODULATION depth                       */ = dx7CtrlDataAllVoice[topNum+44];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_58]/* LFO SYNC                                         */ = (dx7CtrlDataAllVoice[topNum+40])>>6; /* BIT7-6 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_59]/* lfo wave                                         */ = (dx7CtrlDataAllVoice[topNum+45])&0x3; /* BIT1-0 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_60]/* pitch modulation sensitivity                     */ = ((dx7CtrlDataAllVoice[topNum+45])>>4)&0x7; /* BIT6-4 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_61]/* amplitude modulation sensitivity                 */ = ((dx7CtrlDataAllVoice[topNum+45])>>2)&0x3; /* BIT3-2 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_62]/* transpose                                        */ = dx7CtrlDataAllVoice[topNum+46];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_63]/* play mode poly/mono                              */ = ((dx7CtrlDataAllVoice[topNum+48])>>3)&0x1; /* BIT3 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_64]/* pitch bend range                                 */ = dx7CtrlDataAllVoice[topNum+47];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_65]/* portamento mode                                  */ = (dx7CtrlDataAllVoice[topNum+48])&0x1; /* BIT0 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_66]/* portamento time                                  */ = dx7CtrlDataAllVoice[topNum+49];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_67]/* foot volume range                                */ = dx7CtrlDataAllVoice[topNum+50];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_68]/* sustain foot switch                              */ = ((dx7CtrlDataAllVoice[topNum+48])>>2)&0x1; /* BIT2 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_69]/* portamento foot switch                           */ = ((dx7CtrlDataAllVoice[topNum+48])>>1)&0x1; /* BIT1 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_70]/* chorus switch                                    */ = ((dx7CtrlDataAllVoice[topNum+48])>>4)&0x1; /* BIT4 */
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_71]/* modulation wheel pitch modulation range          */ = dx7CtrlDataAllVoice[topNum+51];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_72]/* modulation wheel amplitude modulation range      */ = dx7CtrlDataAllVoice[topNum+52];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_73]/* breath control pitch modulation range            */ = dx7CtrlDataAllVoice[topNum+53];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_74]/* breath control amplitude modulation range        */ = dx7CtrlDataAllVoice[topNum+54];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_75]/* breath control pitch bias range                  */ = dx7CtrlDataAllVoice[topNum+55];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_76]/* breath control eg bias range                     */ = dx7CtrlDataAllVoice[topNum+56];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_77]/* voice name 1                                     */ = dx7CtrlDataAllVoice[topNum+57];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_78]/* voice name 2                                     */ = dx7CtrlDataAllVoice[topNum+58];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_79]/* voice name 3                                     */ = dx7CtrlDataAllVoice[topNum+59];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_80]/* voice name 4                                     */ = dx7CtrlDataAllVoice[topNum+60];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_81]/* voice name 5                                     */ = dx7CtrlDataAllVoice[topNum+61];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_82]/* voice name 6                                     */ = dx7CtrlDataAllVoice[topNum+62];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_83]/* voice name 7                                     */ = dx7CtrlDataAllVoice[topNum+63];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_84]/* voice name 8                                     */ = dx7CtrlDataAllVoice[topNum+64];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_85]/* voice name 9                                     */ = dx7CtrlDataAllVoice[topNum+65];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_86]/* voice name 10                                    */ = dx7CtrlDataAllVoice[topNum+66];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_87]/* pitch eg rate 1                                  */ = dx7CtrlDataAllVoice[topNum+67];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_88]/* pitch eg rate 2                                  */ = dx7CtrlDataAllVoice[topNum+68];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_89]/* pitch eg rate 3                                  */ = dx7CtrlDataAllVoice[topNum+69];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_90]/* pitch eg level 1                                 */ = dx7CtrlDataAllVoice[topNum+70];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_91]/* pitch eg level 2                                 */ = dx7CtrlDataAllVoice[topNum+71];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_92]/* pitch eg level 3                                 */ = dx7CtrlDataAllVoice[topNum+72];

        copyToParamCtrl(DX7_CTRL_SEQ_1VOICE);
    }
    else
    {
        nop();
    }
}

//#define DX7_CTRL_DEBUG_DATA_DISP
/*********************************************
 * 内容   : 
 * 引数   : DX7_CTRL_SEQ_ID seqId
 * 戻り値 : なし
 **********************************************/
static void
debugDataArrayPrint( INT rxDataSize, BYTE *rxDataPtr, PTSTR ptstrTitle )
{
    INT i;

#ifdef DX7_CTRL_DEBUG_DATA_DISP
    DebugWndPrintf("---------------------------------------------------\r\n");
    DebugWndPrintf("%s:\r\n",ptstrTitle);
    DebugWndPrintf("Size:%d\r\n",rxDataSize);

    for( i=0; i<rxDataSize; i++ )
    {
        DebugWndPrintf("0x%02X ",*(rxDataPtr+i));
        if( !((i+1)%16) )
        {
            DebugWndPrintf("\r\n");
        }
    }
    DebugWndPrintf("\r\n");
    DebugWndPrintf("---------------------------------------------------\r\n");
#endif
}
