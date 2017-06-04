/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */
#include "SomeCtrl.h"
#include "ParamCtrl.h"
#include "Midi.h"
#include "dx100Ctrl.h"
#include "DebugWnd.h"
#include "dx100CtrlDef.h"

/* 外部関数定義 */

/* 外部変数定義 */

/* 内部関数定義 */
static BOOL seqStart( DX100_CTRL_SEQ_METHOD method, DX100_CTRL_SEQ_ID seqId );
static INT seqStartProc( DX100_CTRL_SEQ_METHOD method, DX100_CTRL_SEQ_ID seqId, INT maxDataSize, BYTE *txDataPtr );
static BOOL seqEndProc( DX100_CTRL_SEQ_ID seqId, INT rxDataSize, BYTE *rxDataPtr );
static BOOL copyToParamCtrl( DX100_CTRL_SEQ_ID seqId );
static BOOL copyFromParamCtrl( DX100_CTRL_SEQ_ID seqId );
static BYTE getParamCtrlValue( PARAM_CTRL_ID id );
static BOOL displayContents( void );
static BYTE calcCheckSum( BYTE *dataPtr, INT dataSize );
static void debugDataArrayPrint( INT rxDataSize, BYTE *rxDataPtr, PTSTR ptstrTitle );
static BOOL dx100CtrlPrintf( PTSTR ptstrFormat, ... );

/* 内部変数定義 */
static BYTE dx100CtrlSeqTxTempData[1024*8];
static BYTE dx100CtrlSeqRxTempData[1024*8];
static INT dx100CtrlSeqTempRxSize;

static BYTE dx100CtrlDataSysCommon[SYSCMN_INDEX_MAX];
static BYTE dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_INDEX_MAX];
static BYTE dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_INDEX_MAX];

typedef struct
{
    INT   rxDataSize  ; /* 受信データ格納領域のサイズ               */
    BYTE *rxDataPtr   ; /* 受信データ格納領域のポインタ             */
} S_DX100_CTRL_SEQ_DATA;

static const S_DX100_CTRL_SEQ_DATA dx100CtrlSeqDataTbl[DX100_CTRL_SEQ_NUM_MAX] =
{
    {(INT)0                              ,(BYTE *)NULL                   }, /* DX100_CTRL_SEQ_NON_EXEC     */
    {(INT)SYSCMN_INDEX_MAX               ,&dx100CtrlDataSysCommon        }, /* DX100_CTRL_SEQ_SYS_COMMON   */
    {(INT)DX100_SYSEX_1VOICE_INDEX_MAX   ,&dx100CtrlDataOneVoice         }, /* DX100_CTRL_SEQ_1VOICE */
    {(INT)DX100_SYSEX_ALL_VOICE_INDEX_MAX,&dx100CtrlDataAllVoice         }, /* DX100_CTRL_SEQ_ALL_VOICE */
};

typedef struct
{
    HWND                           hWndEdit             ;
    DX100_CTRL_SEQ_METHOD          nowMethod            ;
    DX100_CTRL_SEQ_ID              reqSeqIdStart        ;
    DX100_CTRL_SEQ_ID              reqSeqIdEnd          ;
    DX100_CTRL_SEQ_ID              nowExecSeqId         ;
    DX100_CTRL_MODE                nowMode              ;
#if 0
    DX100_CTRL_1VOICE_SUBMODE      nowOneVoiceSubMode   ;
    DX100_CTRL_ALL_VOICE_SUBMODE   nowAllVoiceSubMode   ;
#endif
} S_DX100_CTRL_INFO;

static S_DX100_CTRL_INFO dx100CtrlInfo;


/*********************************************
 * 内容   : 初期化
 * 引数   : なし
 * 戻り値 : BOOL
 **********************************************/
BOOL
Dx100CtrlInit( HWND hWndEdit )
{
    BOOL bRtn = TRUE;

    dx100CtrlInfo.hWndEdit = hWndEdit;
    dx100CtrlInfo.nowMode            = DX100_CTRL_MODE_SYSTEM;
#if 0
    dx100CtrlInfo.nowOneVoiceSubMode = DX100_CTRL_1VOICE_SUBMODE_COMMON;
    dx100CtrlInfo.nowAllVoiceSubMode = DX100_CTRL_ALL_VOICE_SUBMODE_COMMON;
#endif

    return bRtn;
}

/*********************************************
 * 内容   : 
 * 引数   : DX100_CTRL_MODE                mode
 * 引数   : DX100_CTRL_1VOICE_SUBMODE       patchSubMode
 * 引数   : DX100_CTRL_ALL_VOICE_SUBMODE performanceSubMode
 * 戻り値 : BOOL
 **********************************************/
BOOL
Dx100CtrlModeSet(DX100_CTRL_MODE mode/*,DX100_CTRL_1VOICE_SUBMODE patchSubMode,DX100_CTRL_ALL_VOICE_SUBMODE performanceSubMode*/)
{
    dx100CtrlInfo.nowMode            = mode;
#if 0
    dx100CtrlInfo.nowOneVoiceSubMode = patchSubMode;
    dx100CtrlInfo.nowAllVoiceSubMode = performanceSubMode;
#endif

    return TRUE;
}

BOOL
Dx100CtrlDisplayUpdate( void )
{
    if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_SYSTEM )
    {
#if 0
        copyToParamCtrl(DX100_CTRL_SEQ_SYS_COMMON);
#endif
        ParamCtrlGroupDisplay(PARAM_CTRL_GROUP_SYSTEM_COMMON);
    }
    else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_PATCH )
    {
        copyToParamCtrl(DX100_CTRL_SEQ_1VOICE);
        ParamCtrlGroupDisplay(PARAM_CTRL_GROUP_1VOICE);
    }
    else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_ALL_VOICE )
    {
        copyToParamCtrl(DX100_CTRL_SEQ_ALL_VOICE);
        ParamCtrlGroupDisplay(PARAM_CTRL_GROUP_ALL_VOICE);
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
Dx100CtrlSeqStart( DX100_CTRL_SEQ_METHOD method, DX100_CTRL_SEQ_ID seqIdStart, DX100_CTRL_SEQ_ID seqIdEnd )
{
    BOOL bRtn = TRUE;

    if( (seqIdStart && (seqIdStart < DX100_CTRL_SEQ_NUM_MAX)) &&
        (seqIdEnd   && (seqIdEnd < DX100_CTRL_SEQ_NUM_MAX)) &&
        (seqIdStart <= seqIdEnd) )
    {
        dx100CtrlInfo.nowMethod     = method;
        dx100CtrlInfo.reqSeqIdStart = seqIdStart;
        dx100CtrlInfo.reqSeqIdEnd   = seqIdEnd  ;

        seqStart(method,dx100CtrlInfo.reqSeqIdStart);
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
Dx100CtrlDisplayContents( void )
{
    return displayContents();
}

/*********************************************
 * 内容   : 
 * 引数   : 
 * 戻り値 : 
 **********************************************/
void
Dx100CtrlCycleProc( void )
{
    if( dx100CtrlInfo.nowExecSeqId )
    {
        if( MidiInqOutExec() )
        {
            nop();
        }
        else
        { /* 送信終了 */
            DebugWndPrintf("TX_END\r\n");

            if( dx100CtrlInfo.nowMethod == DX100_CTRL_SEQ_METHOD_SET )
            {
                if( dx100CtrlInfo.nowExecSeqId < dx100CtrlInfo.reqSeqIdEnd )
                {
                    seqStart(dx100CtrlInfo.nowMethod,dx100CtrlInfo.nowExecSeqId+1);
                }
                else
                {
                    dx100CtrlInfo.reqSeqIdStart = DX100_CTRL_SEQ_NON_EXEC;
                    dx100CtrlInfo.reqSeqIdEnd   = DX100_CTRL_SEQ_NON_EXEC;
                    dx100CtrlInfo.nowExecSeqId  = DX100_CTRL_SEQ_NON_EXEC;
                }
            }
            else
            {
                nop();
            }
        }

        if( MidiInqInExec() )
        {
            nop();
        }
        else
        { /* 受信終了 */
            if( dx100CtrlInfo.nowMethod == DX100_CTRL_SEQ_METHOD_GET )
            {
                DebugWndPrintf("RX_END\r\n");
                seqEndProc(dx100CtrlInfo.nowExecSeqId,dx100CtrlSeqTempRxSize,dx100CtrlSeqRxTempData);
            }
            else
            {
                DebugWndPrintf("RX_NOT_EXEC\r\n");
            }

            if( dx100CtrlInfo.nowExecSeqId < dx100CtrlInfo.reqSeqIdEnd )
            {
                seqStart(dx100CtrlInfo.nowMethod,dx100CtrlInfo.nowExecSeqId+1);
            }
            else
            {
                /* 全てのシーケンス終了 */
                dx100CtrlInfo.reqSeqIdStart = DX100_CTRL_SEQ_NON_EXEC;
                dx100CtrlInfo.reqSeqIdEnd   = DX100_CTRL_SEQ_NON_EXEC;
                dx100CtrlInfo.nowExecSeqId  = DX100_CTRL_SEQ_NON_EXEC;

                if( dx100CtrlInfo.nowMethod == DX100_CTRL_SEQ_METHOD_GET )
                {
                    Dx100CtrlDisplayUpdate();
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
seqStart( DX100_CTRL_SEQ_METHOD method, DX100_CTRL_SEQ_ID seqId )
{
    INT txSize;

    if( seqId && (seqId < DX100_CTRL_SEQ_NUM_MAX) )
    {
        DebugWndPrintf("..............\r\n");
        DebugWndPrintf("DX100_CTRL_SEQ_ID:%d\r\n",seqId);

        txSize = seqStartProc(method,seqId,1024,dx100CtrlSeqTxTempData);

        debugDataArrayPrint(txSize,dx100CtrlSeqTxTempData,"TX");

        dx100CtrlInfo.nowExecSeqId = seqId;

        if( method == DX100_CTRL_SEQ_METHOD_SET )
        {
            nop();
        }
        else
        {
            MidiInExclusiveData( &dx100CtrlSeqRxTempData, &dx100CtrlSeqTempRxSize );
        }

        MidiOutExclusiveData( &dx100CtrlSeqTxTempData, txSize );
    }
    else
    {
        nop();
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : DX100_CTRL_SEQ_ID seqId
 * 戻り値 : INT
 **********************************************/
static INT
seqStartProc( DX100_CTRL_SEQ_METHOD method, DX100_CTRL_SEQ_ID seqId, INT maxDataSize, BYTE *txDataPtr )
{
    S_DX100_CTRL_SEQ_DATA *tblPtr;
    INT txSize = (INT)0;
    INT i;
    BYTE checkSum;

    tblPtr = &(dx100CtrlSeqDataTbl[seqId]);

    switch( seqId )
    {
    case DX100_CTRL_SEQ_1VOICE:
        if( method == DX100_CTRL_SEQ_METHOD_SET )
        {
            copyFromParamCtrl( seqId );
            checkSum = calcCheckSum(tblPtr->rxDataPtr+DX100_SYSEX_1VOICE_DATA,DX100_SYSEX_VCED_MAX);
            dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_FOOTER_CHECKSUM] = checkSum;
            memcpy((void *)txDataPtr,(void *)tblPtr->rxDataPtr,tblPtr->rxDataSize);
            txSize = DX100_SYSEX_1VOICE_INDEX_MAX;
        }
        else
        { /* データ要求しデータ受信する*/
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_STATUS    ) = EX_STATUS                   ;
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_ID_NO     ) = EX_ID_NUMBER_YAMAHA         ;
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_SUB_STATUS) = 0x20               | 0x00/* MIDI CH*/;
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_FORMAT_NO ) = DX100_DUMP_REQ_FORMAT_1VOICE;
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_EOX       ) = EX_ETX                      ;
            txSize = DX100_SYSEX_BULK_DUMP_REQUEST_INDEX_MAX;
        }
        break;
    case DX100_CTRL_SEQ_ALL_VOICE:
        if( method == DX100_CTRL_SEQ_METHOD_SET )
        {
            memcpy((void *)txDataPtr,(void *)tblPtr->rxDataPtr,tblPtr->rxDataSize);
            txSize = DX100_SYSEX_ALL_VOICE_INDEX_MAX;
        }
        else
        { /* データ要求しデータ受信する*/
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_STATUS    ) = EX_STATUS                    ;
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_ID_NO     ) = EX_ID_NUMBER_YAMAHA          ;
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_SUB_STATUS) = 0x20               | 0x00/* MIDI CH*/;
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_FORMAT_NO ) = DX100_DUMP_REQ_FORMAT_32VOICE;
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_EOX       ) = EX_ETX                       ;
            txSize = DX100_SYSEX_BULK_DUMP_REQUEST_INDEX_MAX;
        }
        break;
    }

    DebugWndPrintf("checkSum:0x%02X\r\n",checkSum);

    return txSize;
}

/*********************************************
 * 内容   : 
 * 引数   : DX100_CTRL_SEQ_ID seqId
 * 戻り値 : BOOL
 **********************************************/
static BOOL
seqEndProc( DX100_CTRL_SEQ_ID seqId, INT rxDataSize, BYTE *rxDataPtr )
{
    S_DX100_CTRL_SEQ_DATA *tblPtr = &(dx100CtrlSeqDataTbl[seqId]);

//    DebugWndPrintf("rxDataSize,%d\r\n",rxDataSize);

    debugDataArrayPrint(rxDataSize,rxDataPtr,"RX");

    if( rxDataSize == tblPtr->rxDataSize )
    {
        memcpy((void *)tblPtr->rxDataPtr,(void *)rxDataPtr,tblPtr->rxDataSize);

        switch( seqId )
        {
        case DX100_CTRL_SEQ_1VOICE:
            debugDataArrayPrint(DX100_SYSEX_VCED_MAX,tblPtr->rxDataPtr+DX100_SYSEX_1VOICE_DATA,"CONTENTS");
            break;
        case DX100_CTRL_SEQ_ALL_VOICE:
            debugDataArrayPrint(4096,tblPtr->rxDataPtr+DX100_SYSEX_ALL_VOICE_DATA,"CONTENTS");
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
#if 0
    case DX100_CTRL_SEQ_SYS_COMMON  :
        break;
#endif
    case DX100_CTRL_SEQ_1VOICE:
        break;
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : DX100_CTRL_SEQ_ID seqId
 * 戻り値 : BOOL
 **********************************************/
static BOOL
copyToParamCtrl( DX100_CTRL_SEQ_ID seqId )
{
    TCHAR patchName[12+1];
    TCHAR szBuffer[1024];
    INT toneNum;
    INT i;

    switch( seqId )
    {
    case DX100_CTRL_SEQ_SYS_COMMON  :
        break;
    case DX100_CTRL_SEQ_1VOICE:
        for( i=0; i<DX100_SYSEX_VCED_MAX; i++ )
        {
            INT dataIndex = DX100_SYSEX_1VOICE_DATA+i/*DX100_SYSEX_VCED_00...DX100_SYSEX_VCED_92*/;
            INT paramCtrlIndex;

            if( i < DX100_SYSEX_VCED_77 )
            {
                paramCtrlIndex = PARAM_CTRL_VOICE_00 + i/*PARAM_CTRL_VOICE_00...PARAM_CTRL_VOICE_76*/;

                SendMessage( ParamCtrlGetHWND(paramCtrlIndex), CB_SETCURSEL, dx100CtrlDataOneVoice[dataIndex], (LPARAM)0 );
            }
            else if( (DX100_SYSEX_VCED_77 <= i) && (i <= DX100_SYSEX_VCED_86) )
            {
                if( DX100_SYSEX_VCED_77 == i )
                {
                    memset(&patchName[0],0,10+1);
                    strncpy(&patchName[0],&dx100CtrlDataOneVoice[dataIndex],10);
                    SetWindowText( ParamCtrlGetHWND(PARAM_CTRL_VOICE_NAME),&patchName[0]);
                }
                else
                {
                }
            }
            else
            {
                paramCtrlIndex = PARAM_CTRL_VOICE_87 + (i-DX100_SYSEX_VCED_87)/*PARAM_CTRL_VOICE_87...PARAM_CTRL_VOICE_92*/;

                SendMessage( ParamCtrlGetHWND(paramCtrlIndex), CB_SETCURSEL, dx100CtrlDataOneVoice[dataIndex], (LPARAM)0 );
            }

        }
    case DX100_CTRL_SEQ_ALL_VOICE:
        for( i=0; i<24; i++ )
        {
            INT j;
            memset(&patchName[0],0,10+1);
            strncpy(&patchName[0],&dx100CtrlDataAllVoice[ DX100_SYSEX_ALL_VOICE_DATA + (i*DX100_SYSEX_VMEM_MAX) + DX100_SYSEX_VMEM_57],10);
            SetWindowText( ParamCtrlGetHWND(PARAM_CTRL_ALL_VOICE_NAME_00+i),&patchName[0]);

//            for( j=0; j<10; j++ )
//            {
//                dx100CtrlPrintf( "%c(%02X) ",dx100CtrlDataAllVoice[ DX100_SYSEX_ALL_VOICE_DATA + (i*DX100_SYSEX_VMEM_MAX) + DX100_SYSEX_VMEM_57 + j],dx100CtrlDataAllVoice[ DX100_SYSEX_ALL_VOICE_DATA + (i*DX100_SYSEX_VMEM_MAX) + DX100_SYSEX_VMEM_57 + j]);
//            }
//            dx100CtrlPrintf( "%02X\r\n" );
        }
        break;
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : DX100_CTRL_SEQ_ID seqId
 * 戻り値 : BOOL
 **********************************************/
static BOOL
copyFromParamCtrl( DX100_CTRL_SEQ_ID seqId )
{
    TCHAR patchName[12+1];
    TCHAR szBuffer[1024];
    INT toneNum;
    INT i;

    switch( seqId )
    {
#if 0
    case DX100_CTRL_SEQ_SYS_COMMON  :
        dx100CtrlDataSysCommon[SYSCMN_SOUNDMODE                ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_SOUNDMODE                   );
        dx100CtrlDataSysCommon[SYSCMN_PERFORMANCENUMBER        ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PERFORMANCENUMBER           );
        dx100CtrlDataSysCommon[SYSCMN_PATCHGROUPTYPE           ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PATCHGROUPTYPE              );
        dx100CtrlDataSysCommon[SYSCMN_PATCHGROUPID             ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PATCHGROUPID                );
        dx100CtrlDataSysCommon[SYSCMN_PATCHNUMBER_H            ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PATCHNUMBER_H               );
        dx100CtrlDataSysCommon[SYSCMN_PATCHNUMBER_L            ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PATCHNUMBER_L               );
        dx100CtrlDataSysCommon[SYSCMN_MASTERTUNE               ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_MASTERTUNE                  );
        dx100CtrlDataSysCommon[SYSCMN_SCALETUNESWITCH          ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_SCALETUNESWITCH             );
        dx100CtrlDataSysCommon[SYSCMN_EFXSWITCH                ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_EFXSWITCH                   );
        dx100CtrlDataSysCommon[SYSCMN_CHORUSSWITCH             ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_CHORUSSWITCH                );
        dx100CtrlDataSysCommon[SYSCMN_REVERBSWITCH             ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_REVERBSWITCH                );
        dx100CtrlDataSysCommon[SYSCMN_PATCHREMAIN              ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PATCHREMAIN                 );
        dx100CtrlDataSysCommon[SYSCMN_CLOCKSOURCE              ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_CLOCKSOURCE                 );
        dx100CtrlDataSysCommon[SYSCMN_TAPCONTROLSOURC          ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_TAPCONTROLSOURC             );
        dx100CtrlDataSysCommon[SYSCMN_HOLDCONTROLSOURCE        ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_HOLDCONTROLSOURCE           );
        dx100CtrlDataSysCommon[SYSCMN_PEAKCONTROLSOURCE        ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PEAKCONTROLSOURCE           );
        dx100CtrlDataSysCommon[SYSCMN_VOLUMECONTROLSOURCE      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_VOLUMECONTROLSOURCE         );
        dx100CtrlDataSysCommon[SYSCMN_AFTERTOUCHSOURCE         ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_AFTERTOUCHSOURCE            );
        dx100CtrlDataSysCommon[SYSCMN_SYSTEMCONTROLSOURCE11_97 ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_SYSTEMCONTROLSOURCE11_97    );
        dx100CtrlDataSysCommon[SYSCMN_SYSTEMCONTROLSOURCE21_97 ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_SYSTEMCONTROLSOURCE21_97    );
        dx100CtrlDataSysCommon[SYSCMN_RECEIVEPROGRAMCHANGE     ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_RECEIVEPROGRAMCHANGE        );
        dx100CtrlDataSysCommon[SYSCMN_RECEIVEBANKSELECT        ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_RECEIVEBANKSELECT           );
        dx100CtrlDataSysCommon[SYSCMN_RECEIVECONTROLCHANGE     ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_RECEIVECONTROLCHANGE        );
        dx100CtrlDataSysCommon[SYSCMN_RECEIVEMODULATION        ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_RECEIVEMODULATION           );
        dx100CtrlDataSysCommon[SYSCMN_RECEIVEVOLUME            ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_RECEIVEVOLUME               );
        dx100CtrlDataSysCommon[SYSCMN_RECEIVEHOLD              ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_RECEIVEHOLD                 );
        dx100CtrlDataSysCommon[SYSCMN_RECEIVEPITCHBEND         ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_RECEIVEPITCHBEND            );
        dx100CtrlDataSysCommon[SYSCMN_RECEIVEAFTERTOUCH        ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_RECEIVEAFTERTOUCH           );
        dx100CtrlDataSysCommon[SYSCMN_CONTROLCHANNEL           ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_CONTROLCHANNEL              );
        dx100CtrlDataSysCommon[SYSCMN_PATCHRECEIVECHANNEL      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PATCHRECEIVECHANNEL         );
        dx100CtrlDataSysCommon[SYSCMN_RHYTHMEDITSOURCE         ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_RHYTHMEDITSOURCE            );
        dx100CtrlDataSysCommon[SYSCMN_PREVIEWSOUNDMODE         ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PREVIEWSOUNDMODE            );
        dx100CtrlDataSysCommon[SYSCMN_PREVIEWNOTESET10_127     ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PREVIEWNOTESET10_127        );
        dx100CtrlDataSysCommon[SYSCMN_PREVIEWVELOCITYSET10_127 ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PREVIEWVELOCITYSET10_127    );
        dx100CtrlDataSysCommon[SYSCMN_PREVIEWNOTESET20_127     ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PREVIEWNOTESET20_127        );
        dx100CtrlDataSysCommon[SYSCMN_PREVIEWVELOCITYSET20_127 ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PREVIEWVELOCITYSET20_127    );
        dx100CtrlDataSysCommon[SYSCMN_PREVIEWNOTESET30_127     ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PREVIEWNOTESET30_127        );
        dx100CtrlDataSysCommon[SYSCMN_PREVIEWVELOCITYSET30_127 ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PREVIEWVELOCITYSET30_127    );
        dx100CtrlDataSysCommon[SYSCMN_PREVIEWNOTESET40_127     ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PREVIEWNOTESET40_127        );
        dx100CtrlDataSysCommon[SYSCMN_PREVIEWVELOCITYSET40_127 ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PREVIEWVELOCITYSET40_127    );
        dx100CtrlDataSysCommon[SYSCMN_TRANSMITPROGRAMCHANGE    ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_TRANSMITPROGRAMCHANGE       );
        dx100CtrlDataSysCommon[SYSCMN_TRANSMITBANKSELECT       ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_TRANSMITBANKSELECT          );
        dx100CtrlDataSysCommon[SYSCMN_PATCHTRANSMITCHANNEL     ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PATCHTRANSMITCHANNEL        );
        dx100CtrlDataSysCommon[SYSCMN_TRANSPOSESWITCH          ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_TRANSPOSESWITCH             );
        dx100CtrlDataSysCommon[SYSCMN_TRANSPOSEVALUE           ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_TRANSPOSEVALUE              );
        dx100CtrlDataSysCommon[SYSCMN_OCTAVESHIFT              ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_OCTAVESHIFT                 );
        dx100CtrlDataSysCommon[SYSCMN_KEYBOARDVELOCITY         ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_KEYBOARDVELOCITY            );
        dx100CtrlDataSysCommon[SYSCMN_KEYBOARDSENS             ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_KEYBOARDSENS                );
        dx100CtrlDataSysCommon[SYSCMN_AFTERTOUCHSENS           ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_AFTERTOUCHSENS              );
        dx100CtrlDataSysCommon[SYSCMN_PEDAL_1_ASSIGN           ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PEDAL_1_ASSIGN              );
        dx100CtrlDataSysCommon[SYSCMN_PEDAL_1_OUTPUTMODE       ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PEDAL_1_OUTPUTMODE          );
        dx100CtrlDataSysCommon[SYSCMN_PEDAL_1_POLARITY         ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PEDAL_1_POLARITY            );
        dx100CtrlDataSysCommon[SYSCMN_PEDAL2ASSIGN             ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PEDAL2ASSIGN                );
        dx100CtrlDataSysCommon[SYSCMN_PEDAL2OUTPUTMODE         ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PEDAL2OUTPUTMODE            );
        dx100CtrlDataSysCommon[SYSCMN_PEDAL2POLARITY           ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PEDAL2POLARITY              );
        dx100CtrlDataSysCommon[SYSCMN_C1ASSIGN                 ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_C1ASSIGN                    );
        dx100CtrlDataSysCommon[SYSCMN_C1OUTPUTMODE             ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_C1OUTPUTMODE                );
        dx100CtrlDataSysCommon[SYSCMN_C2ASSIGN                 ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_C2ASSIGN                    );
        dx100CtrlDataSysCommon[SYSCMN_C2OUTPUTMODE             ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_C2OUTPUTMODE                );
        dx100CtrlDataSysCommon[SYSCMN_HOLDPEDALOUTPUTMODE      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_HOLDPEDALOUTPUTMODE         );
        dx100CtrlDataSysCommon[SYSCMN_HOLDPEDALPOLARITY        ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_HOLDPEDALPOLARITY           );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP1SWITCH   ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP1SWITCH      );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP1MSB      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP1MSB         );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP1LSB      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP1LSB         );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP2SWITCH   ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP2SWITCH      );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP2MSB0     ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP2MSB0        );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP2LSB0     ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP2LSB0        );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP3SWITCH   ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP3SWITCH      );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP3MSB      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP3MSB         );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP3LSB      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP3LSB         );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP4SWITCH   ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP4SWITCH      );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP4MSB      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP4MSB         );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP4LSB      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP4LSB         );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP5SWITCH   ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP5SWITCH      );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP5MSB      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP5MSB         );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP5LSB      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP5LSB         );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP6SWITCH   ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP6SWITCH      );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP6MSB      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP6MSB         );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP6LSB      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP6LSB         );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP7SWITCH   ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP7SWITCH      );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP7MSB      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP7MSB         );
        dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP7LSB      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_BANKSELECTGROUP7LSB         );
        dx100CtrlDataSysCommon[SYSCMN_PEDAL3ASSIGN             ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PEDAL3ASSIGN                );
        dx100CtrlDataSysCommon[SYSCMN_PEDAL3OUTPUTMODE         ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PEDAL3OUTPUTMODE            );
        dx100CtrlDataSysCommon[SYSCMN_PEDAL3POLARITY           ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PEDAL3POLARITY              );
        dx100CtrlDataSysCommon[SYSCMN_PEDAL4ASSIGN             ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PEDAL4ASSIGN                );
        dx100CtrlDataSysCommon[SYSCMN_PEDAL4OUTPUTMODE         ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PEDAL4OUTPUTMODE            );
        dx100CtrlDataSysCommon[SYSCMN_PEDAL4POLARITY           ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_PEDAL4POLARITY              );
        dx100CtrlDataSysCommon[SYSCMN_ARPEGGIOSTYLE            ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_ARPEGGIOSTYLE               );
        dx100CtrlDataSysCommon[SYSCMN_ARPEGGIOMOTIF            ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_ARPEGGIOMOTIF               );
        dx100CtrlDataSysCommon[SYSCMN_ARPEGGIOBEATPATTERN      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_ARPEGGIOBEATPATTERN         );
        dx100CtrlDataSysCommon[SYSCMN_ARPEGGIOACCENTRATE       ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_ARPEGGIOACCENTRATE          );
        dx100CtrlDataSysCommon[SYSCMN_ARPEGGIOSHUFFLERATE      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_ARPEGGIOSHUFFLERATE         );
        dx100CtrlDataSysCommon[SYSCMN_ARPEGGIOKEYBOARDVELOCITY ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_ARPEGGIOKEYBOARDVELOCITY    );
        dx100CtrlDataSysCommon[SYSCMN_ARPEGGIOOCTAVERANGE      ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_ARPEGGIOOCTAVERANGE         );
        dx100CtrlDataSysCommon[SYSCMN_ARPEGGIOPARTNUMBER       ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_ARPEGGIOPARTNUMBER          );
        dx100CtrlDataSysCommon[SYSCMN_SYSTEMTEMPO_H            ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_SYSTEMTEMPO_H               );
        dx100CtrlDataSysCommon[SYSCMN_SYSTEMTEMPO_L            ] = getParamCtrlValue(PARAM_CTRL_SYSCMN_SYSTEMTEMPO_L               );
        break;
#endif
    case DX100_CTRL_SEQ_1VOICE:
        for( i=0; i<DX100_SYSEX_VCED_MAX; i++ )
        {
            INT dataIndex = DX100_SYSEX_1VOICE_DATA+i/*DX100_SYSEX_VCED_00...DX100_SYSEX_VCED_92*/;
            INT paramCtrlIndex;

            if( i < DX100_SYSEX_VCED_77 )
            {
                paramCtrlIndex = PARAM_CTRL_VOICE_00 + i/*PARAM_CTRL_VOICE_00...PARAM_CTRL_VOICE_76*/;

                dx100CtrlDataOneVoice[dataIndex] = getParamCtrlValue(paramCtrlIndex);
            }
            else if( (DX100_SYSEX_VCED_77 <= i) && (i <= DX100_SYSEX_VCED_86) )
            {
                if( DX100_SYSEX_VCED_77 == i )
                {
                    memset(&patchName[0],0,10+1);
                    ParamCtrlGetText(PARAM_CTRL_VOICE_NAME,patchName);
                    strncpy(&dx100CtrlDataOneVoice[dataIndex],&patchName[0],10);
                }
                else
                {
                }
            }
            else
            {
                paramCtrlIndex = PARAM_CTRL_VOICE_87 + (i-DX100_SYSEX_VCED_87)/*PARAM_CTRL_VOICE_87...PARAM_CTRL_VOICE_92*/;

                dx100CtrlDataOneVoice[dataIndex] = getParamCtrlValue(paramCtrlIndex);
            }

        }
        break;
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : DX100_CTRL_SEQ_ID seqId
 * 戻り値 : BOOL
 **********************************************/
static BYTE
getParamCtrlValue( PARAM_CTRL_ID id )
{
    BYTE value = (BYTE)0x00;
    int iCbNum;
    HWND hComboBox;

    hComboBox = ParamCtrlGetHWND(id);
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

    EditWndDataSet( dx100CtrlInfo.hWndEdit,"\r\n",2,TRUE );

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
Dx100GetDataSize( DX100_CTRL_SEQ_ID seqId )
{
    S_DX100_CTRL_SEQ_DATA *tblPtr;
    DWORD dwSize = (DWORD)0;

    if( seqId < DX100_CTRL_SEQ_NUM_MAX )
    {
        tblPtr = &(dx100CtrlSeqDataTbl[seqId]);
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
Dx100DataGet( DX100_CTRL_SEQ_ID seqId, TCHAR *dataPtr, DWORD dataSize )
{
    S_DX100_CTRL_SEQ_DATA *tblPtr;
    DWORD dwSize = (DWORD)0;

    if( seqId < DX100_CTRL_SEQ_NUM_MAX )
    {
        tblPtr = &(dx100CtrlSeqDataTbl[seqId]);
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
Dx100DataSet( DX100_CTRL_SEQ_ID seqId, TCHAR *dataPtr, DWORD dataSize )
{
    S_DX100_CTRL_SEQ_DATA *tblPtr;
    DWORD dwSize = (DWORD)0;

    if( seqId < DX100_CTRL_SEQ_NUM_MAX )
    {
        tblPtr = &(dx100CtrlSeqDataTbl[seqId]);
        dwSize = tblPtr->rxDataSize;

        if( dwSize <= dataSize )
        {
            memcpy(tblPtr->rxDataPtr,dataPtr,tblPtr->rxDataSize);
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

//#define DX100_CTRL_DEBUG_DATA_DISP
/*********************************************
 * 内容   : 
 * 引数   : DX100_CTRL_SEQ_ID seqId
 * 戻り値 : なし
 **********************************************/
static void
debugDataArrayPrint( INT rxDataSize, BYTE *rxDataPtr, PTSTR ptstrTitle )
{
    INT i;

#ifdef DX100_CTRL_DEBUG_DATA_DISP
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

/********************************************************************************
 * 内容   : デバッグウィンドウへの printf
 * 引数   : PTSTR ptstrFormat, ...
 * 戻り値 : BOOL
 ********************************************************************************/
BOOL
dx100CtrlPrintf( PTSTR ptstrFormat, ...)
{
    va_list vaArgs;
    static TCHAR szBuf[1024];

    if( dx100CtrlInfo.hWndEdit != NULL )
    {
        va_start(vaArgs, ptstrFormat);
        if( wvsprintf(szBuf, ptstrFormat, vaArgs) != EOF )
        {
            EditWndDataSet( dx100CtrlInfo.hWndEdit,szBuf,strlen(szBuf),FALSE );
        }
        else
        {
            nop();
        }
        va_end(vaArgs);
    }
    else
    {
        nop();
    }

    return TRUE;
}
