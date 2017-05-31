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
static BOOL seqEndProc( DX100_CTRL_SEQ_ID seqId );
static BOOL copyToParamCtrl( DX100_CTRL_SEQ_ID seqId );
static BOOL copyFromParamCtrl( DX100_CTRL_SEQ_ID seqId );
static BYTE getParamCtrlValue( PARAM_CTRL_ID id );
static BOOL displayContents( void );
static BYTE calcCheckSum( BYTE *dataPtr, INT dataSize );
static BOOL dx100CtrlPrintf( PTSTR ptstrFormat, ... );

/* 内部変数定義 */
#if 0
static BYTE seqDataTbl0[] = {0xF0,0x41,0x10,0x6A,0x11,0x10,0x02,0x12,0x00,0x00,0x00,0x00,0x19,0x43,0xF7};
#endif

static BYTE dx100CtrlSeqTxTempData[1024];
static BYTE dx100CtrlSeqRxTempData[1024];
static INT dx100CtrlSeqTempRxSize;

static BYTE dx100CtrlDataSysCommon[SYSCMN_INDEX_MAX];
static BYTE dx100CtrlDataPatchCommon[VOICE_INDEX_MAX];
static BYTE dx100CtrlDataPatchTone[4][PATCHTONE_INDEX_MAX];

typedef struct
{
    DWORD exDataAdrs  ; /* エクスクルーシブデータの内部アドレス     */
    DWORD exDataSize  ; /* エクスクルーシブデータの内部データサイズ */
    INT   rxDataSize  ; /* 受信データ格納領域のサイズ               */
    BYTE *rxDataPtr   ; /* 受信データ格納領域のポインタ             */
} S_DX100_CTRL_SEQ_DATA;

static const S_DX100_CTRL_SEQ_DATA dx100CtrlSeqDataTbl[DX100_CTRL_SEQ_NUM_MAX] =
{
    {(DWORD)0                                             ,(DWORD)0     ,(INT)0                  ,(BYTE *)NULL                   }, /* DX100_CTRL_SEQ_NON_EXEC     */
    {(DWORD)A_BLK_SYS_COMMON                              ,(DWORD)0x0062,(INT)SYSCMN_INDEX_MAX   ,&dx100CtrlDataSysCommon        }, /* DX100_CTRL_SEQ_SYS_COMMON   */
    {(DWORD)A_BLK_PATCH_MODE_TMP_PATCH|OFST_PCH_CMN       ,(DWORD)0x004A,(INT)VOICE_INDEX_MAX,&dx100CtrlDataPatchCommon      }, /* DX100_CTRL_SEQ_PATCH_COMMON */
    {(DWORD)A_BLK_PATCH_MODE_TMP_PATCH|OFST_PCH_TN1       ,(DWORD)0x0101,(INT)PATCHTONE_INDEX_MAX,&dx100CtrlDataPatchTone[0]     }, /* DX100_CTRL_SEQ_PATCH_TONE1  */
    {(DWORD)A_BLK_PATCH_MODE_TMP_PATCH|OFST_PCH_TN2       ,(DWORD)0x0101,(INT)PATCHTONE_INDEX_MAX,&dx100CtrlDataPatchTone[1]     }, /* DX100_CTRL_SEQ_PATCH_TONE2  */
    {(DWORD)A_BLK_PATCH_MODE_TMP_PATCH|OFST_PCH_TN3       ,(DWORD)0x0101,(INT)PATCHTONE_INDEX_MAX,&dx100CtrlDataPatchTone[2]     }, /* DX100_CTRL_SEQ_PATCH_TONE3  */
    {(DWORD)A_BLK_PATCH_MODE_TMP_PATCH|OFST_PCH_TN4       ,(DWORD)0x0101,(INT)PATCHTONE_INDEX_MAX,&dx100CtrlDataPatchTone[3]     }, /* DX100_CTRL_SEQ_PATCH_TONE4  */
};

typedef struct
{
    HWND                           hWndEdit             ;
    DX100_CTRL_SEQ_METHOD          nowMethod            ;
    DX100_CTRL_SEQ_ID              reqSeqIdStart        ;
    DX100_CTRL_SEQ_ID              reqSeqIdEnd          ;
    DX100_CTRL_SEQ_ID              nowExecSeqId         ;
    DX100_CTRL_MODE                nowMode              ;
    DX100_CTRL_PATCH_SUBMODE       nowPatchSubMode      ;
    DX100_CTRL_PERFORMANCE_SUBMODE nowPerformanceSubMode;
} S_DX100_CTRL_INFO;

static S_DX100_CTRL_INFO dx100CtrlInfo;

#define DX100_CTRL_DEBUG_DATA_DISP

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
    dx100CtrlInfo.nowMode               = DX100_CTRL_MODE_SYSTEM;
    dx100CtrlInfo.nowPatchSubMode       = DX100_CTRL_PATCH_SUBMODE_COMMON;
    dx100CtrlInfo.nowPerformanceSubMode = DX100_CTRL_PERFORMANCE_SUBMODE_COMMON;

    return bRtn;
}

/*********************************************
 * 内容   : 
 * 引数   : DX100_CTRL_MODE                mode
 * 引数   : DX100_CTRL_PATCH_SUBMODE       patchSubMode
 * 引数   : DX100_CTRL_PERFORMANCE_SUBMODE performanceSubMode
 * 戻り値 : BOOL
 **********************************************/
BOOL
Dx100CtrlModeSet(DX100_CTRL_MODE mode,DX100_CTRL_PATCH_SUBMODE patchSubMode,DX100_CTRL_PERFORMANCE_SUBMODE performanceSubMode)
{
    dx100CtrlInfo.nowMode               = mode;
    dx100CtrlInfo.nowPatchSubMode       = patchSubMode;
    dx100CtrlInfo.nowPerformanceSubMode = performanceSubMode;

    return TRUE;
}

BOOL
Dx100CtrlDisplayUpdate( void )
{
    if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_SYSTEM )
    {
        copyToParamCtrl(DX100_CTRL_SEQ_SYS_COMMON);
        ParamCtrlGroupDisplay(PARAM_CTRL_GROUP_SYSTEM_COMMON);
    }
    else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_PERFORMANCE )
    {
    }
    else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_PATCH )
    {
        switch( dx100CtrlInfo.nowPatchSubMode )
        {
        case DX100_CTRL_PATCH_SUBMODE_COMMON:
            copyToParamCtrl(DX100_CTRL_SEQ_PATCH_COMMON);
            ParamCtrlGroupDisplay(PARAM_CTRL_GROUP_PATCH_COMMON);
            break;
        case DX100_CTRL_PATCH_SUBMODE_TONE1 :
            copyToParamCtrl(DX100_CTRL_SEQ_PATCH_TONE1);
            ParamCtrlGroupDisplay(PARAM_CTRL_GROUP_PATCH_TONE);
            break;
        case DX100_CTRL_PATCH_SUBMODE_TONE2 :
            copyToParamCtrl(DX100_CTRL_SEQ_PATCH_TONE2);
            ParamCtrlGroupDisplay(PARAM_CTRL_GROUP_PATCH_TONE);
            break;
        case DX100_CTRL_PATCH_SUBMODE_TONE3 :
            copyToParamCtrl(DX100_CTRL_SEQ_PATCH_TONE3);
            ParamCtrlGroupDisplay(PARAM_CTRL_GROUP_PATCH_TONE);
            break;
        case DX100_CTRL_PATCH_SUBMODE_TONE4 :
            copyToParamCtrl(DX100_CTRL_SEQ_PATCH_TONE4);
            ParamCtrlGroupDisplay(PARAM_CTRL_GROUP_PATCH_TONE);
            break;
        default:
            nop();
            break;
        }
    }
    else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_GM )
    {
    }
    else
    {
    }

    return TRUE;
}

/*********************************************
 * 内容   :
 * 引数   : なし
 * 戻り値 : BOOL
 **********************************************/
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
    S_DX100_CTRL_SEQ_DATA *tblPtr;
    INT i,contentSize;

    if( dx100CtrlInfo.nowExecSeqId )
    {
        tblPtr = &(dx100CtrlSeqDataTbl[dx100CtrlInfo.nowExecSeqId]);

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
            DebugWndPrintf("rxSize:%d\r\n",dx100CtrlSeqTempRxSize);
            DebugWndPrintf("RX:\r\n");
#ifdef DX100_CTRL_DEBUG_DATA_DISP
            for( i=0; i<dx100CtrlSeqTempRxSize; i++ )
            {
                DebugWndPrintf("0x%02X ",dx100CtrlSeqRxTempData[i]);
                if( !((i+1)%16) )
                {
                    DebugWndPrintf("\r\n");
                }
            }
            DebugWndPrintf("\r\n");
#endif

            contentSize = (dx100CtrlSeqTempRxSize-(MIDI_EX_HEADER_DATA + 2 + 1 + EX_FOOTER_SIZE));

            if( contentSize == tblPtr->rxDataSize )
            {
                memcpy((void *)tblPtr->rxDataPtr,(void *)(&dx100CtrlSeqRxTempData[0] + MIDI_EX_HEADER_DATA + 2),tblPtr->rxDataSize);
#ifdef DX100_CTRL_DEBUG_DATA_DISP
                DebugWndPrintf("CONTENTS:\r\n");
                for( i=0; i<tblPtr->rxDataSize; i++ )
                {
                    DebugWndPrintf("0x%02X ",*(tblPtr->rxDataPtr+i));
                    if( !((i+1)%16) )
                    {
                        DebugWndPrintf("\r\n");
                    }
                }
                DebugWndPrintf("\r\n");
#endif
            }
            else
            {
                DebugWndPrintf("Check NG,%d,%d\r\n",contentSize,tblPtr->rxDataSize);
            }

            seqEndProc(dx100CtrlInfo.nowExecSeqId);

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
    S_DX100_CTRL_SEQ_DATA *tblPtr;
    INT i;
    INT txSize;
    BYTE checkSum;

    if( seqId && (seqId < DX100_CTRL_SEQ_NUM_MAX) )
    {
        tblPtr = &(dx100CtrlSeqDataTbl[seqId]);

        DebugWndPrintf("..............\r\n");
        DebugWndPrintf("DX100_CTRL_SEQ_ID:%d\r\n",seqId);

        dx100CtrlSeqTxTempData[MIDI_EX_HEADER_STATUS    ] = EX_STATUS          ;
        dx100CtrlSeqTxTempData[MIDI_EX_HEADER_ID_NUMBER ] = EX_ID_NUMBER_YAMAHA; /* ID      : 0x43 YAMAHA  */
#if 1
        dx100CtrlSeqTxTempData[MIDI_EX_HEADER_SUB_STATUS] = 0x20; /* dump request 0x2n */
        dx100CtrlSeqTxTempData[MIDI_EX_HEADER_PARAM     ] = 0x03; /* 3=1音色bulk data */
#else
        dx100CtrlSeqTxTempData[MIDI_EX_HEADER_DEVICE_ID ] = EX_DEVICE_ID       ; /* deviceID: 0x10         */
        dx100CtrlSeqTxTempData[MIDI_EX_HEADER_MODEL_ID  ] = EX_MODEL_ID_DX100  ; /* modelID : 0x6A DX100   */

        if( method == DX100_CTRL_SEQ_METHOD_SET )
        {
            dx100CtrlSeqTxTempData[MIDI_EX_HEADER_COMMAND_ID] = DT1;
        }
        else
        {
            dx100CtrlSeqTxTempData[MIDI_EX_HEADER_COMMAND_ID] = RQ1;
        }

        dx100CtrlSeqTxTempData[MIDI_EX_HEADER_ADRS0     ] = HIBYTE(HIWORD(tblPtr->exDataAdrs));
        dx100CtrlSeqTxTempData[MIDI_EX_HEADER_ADRS1     ] = LOBYTE(HIWORD(tblPtr->exDataAdrs));
        dx100CtrlSeqTxTempData[MIDI_EX_HEADER_ADRS2     ] = HIBYTE(LOWORD(tblPtr->exDataAdrs));
        dx100CtrlSeqTxTempData[MIDI_EX_HEADER_ADRS3     ] = LOBYTE(LOWORD(tblPtr->exDataAdrs));
#endif

        if( method == DX100_CTRL_SEQ_METHOD_SET )
        {
            copyFromParamCtrl( seqId );

            memcpy( (void *)&dx100CtrlSeqTxTempData[MIDI_EX_HEADER_DATA], (void *)tblPtr->rxDataPtr, tblPtr->rxDataSize );
#if 0
            dx100CtrlSeqTxTempData[MIDI_EX_HEADER_DATA+tblPtr->rxDataSize]   = calcCheckSum( &dx100CtrlSeqTxTempData[MIDI_EX_HEADER_ADRS0], 4+tblPtr->rxDataSize );
            dx100CtrlSeqTxTempData[MIDI_EX_HEADER_DATA+tblPtr->rxDataSize+1] = EX_ETX;
#endif
            txSize = MIDI_EX_HEADER_DATA + tblPtr->rxDataSize + EX_FOOTER_SIZE;
        }
        else
        {
            /* データ要求しデータ受信する*/
#if 1
            dx100CtrlSeqTxTempData[MIDI_EX_HEADER_DATA]       = EX_ETX;
            txSize = 5;
#else
            dx100CtrlSeqTxTempData[MIDI_EX_HEADER_DATA    +0] = HIBYTE(HIWORD(tblPtr->exDataSize));
            dx100CtrlSeqTxTempData[MIDI_EX_HEADER_DATA    +1] = LOBYTE(HIWORD(tblPtr->exDataSize));
            dx100CtrlSeqTxTempData[MIDI_EX_HEADER_DATA    +2] = HIBYTE(LOWORD(tblPtr->exDataSize));
            dx100CtrlSeqTxTempData[MIDI_EX_HEADER_DATA    +3] = LOBYTE(LOWORD(tblPtr->exDataSize));
            dx100CtrlSeqTxTempData[13]                        = calcCheckSum( &dx100CtrlSeqTxTempData[MIDI_EX_HEADER_ADRS0], 8 );
            dx100CtrlSeqTxTempData[14]                        = EX_ETX;
            txSize = 15;
#endif
        }

        DebugWndPrintf("txDataSize:%d\r\n",txSize);
        DebugWndPrintf("checkSum:0x%02X\r\n",checkSum);
        DebugWndPrintf("TX:");
#ifdef DX100_CTRL_DEBUG_DATA_DISP
        for( i=0; i<txSize; i++ )
        {
            DebugWndPrintf("0x%02X ",dx100CtrlSeqTxTempData[i]);
            if( !((i+1)%16) )
            {
                DebugWndPrintf("\r\n");
            }
        }
        DebugWndPrintf("\r\n");
#endif
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
 * 戻り値 : BOOL
 **********************************************/
static BOOL
seqEndProc( DX100_CTRL_SEQ_ID seqId )
{
    TCHAR patchName[12+1];
    TCHAR szBuffer[1024];

    switch( seqId )
    {
    case DX100_CTRL_SEQ_SYS_COMMON  :
        break;
    case DX100_CTRL_SEQ_PATCH_COMMON:
        break;
    case DX100_CTRL_SEQ_PATCH_TONE1 :
        break;
    case DX100_CTRL_SEQ_PATCH_TONE2 :
        break;
    case DX100_CTRL_SEQ_PATCH_TONE3 :
        break;
    case DX100_CTRL_SEQ_PATCH_TONE4 :
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

    switch( seqId )
    {
#if 0
    case DX100_CTRL_SEQ_SYS_COMMON  :
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_SOUNDMODE                   ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_SOUNDMODE                ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PERFORMANCENUMBER           ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PERFORMANCENUMBER        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PATCHGROUPTYPE              ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PATCHGROUPTYPE           ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PATCHGROUPID                ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PATCHGROUPID             ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PATCHNUMBER_H               ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PATCHNUMBER_H            ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PATCHNUMBER_L               ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PATCHNUMBER_L            ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_MASTERTUNE                  ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_MASTERTUNE               ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_SCALETUNESWITCH             ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_SCALETUNESWITCH          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_EFXSWITCH                   ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_EFXSWITCH                ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_CHORUSSWITCH                ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_CHORUSSWITCH             ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_REVERBSWITCH                ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_REVERBSWITCH             ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PATCHREMAIN                 ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PATCHREMAIN              ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_CLOCKSOURCE                 ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_CLOCKSOURCE              ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_TAPCONTROLSOURC             ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_TAPCONTROLSOURC          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_HOLDCONTROLSOURCE           ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_HOLDCONTROLSOURCE        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PEAKCONTROLSOURCE           ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PEAKCONTROLSOURCE        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_VOLUMECONTROLSOURCE         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_VOLUMECONTROLSOURCE      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_AFTERTOUCHSOURCE            ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_AFTERTOUCHSOURCE         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_SYSTEMCONTROLSOURCE11_97    ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_SYSTEMCONTROLSOURCE11_97 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_SYSTEMCONTROLSOURCE21_97    ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_SYSTEMCONTROLSOURCE21_97 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_RECEIVEPROGRAMCHANGE        ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_RECEIVEPROGRAMCHANGE     ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_RECEIVEBANKSELECT           ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_RECEIVEBANKSELECT        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_RECEIVECONTROLCHANGE        ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_RECEIVECONTROLCHANGE     ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_RECEIVEMODULATION           ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_RECEIVEMODULATION        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_RECEIVEVOLUME               ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_RECEIVEVOLUME            ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_RECEIVEHOLD                 ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_RECEIVEHOLD              ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_RECEIVEPITCHBEND            ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_RECEIVEPITCHBEND         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_RECEIVEAFTERTOUCH           ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_RECEIVEAFTERTOUCH        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_CONTROLCHANNEL              ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_CONTROLCHANNEL           ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PATCHRECEIVECHANNEL         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PATCHRECEIVECHANNEL      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_RHYTHMEDITSOURCE            ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_RHYTHMEDITSOURCE         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PREVIEWSOUNDMODE            ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PREVIEWSOUNDMODE         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PREVIEWNOTESET10_127        ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PREVIEWNOTESET10_127     ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PREVIEWVELOCITYSET10_127    ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PREVIEWVELOCITYSET10_127 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PREVIEWNOTESET20_127        ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PREVIEWNOTESET20_127     ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PREVIEWVELOCITYSET20_127    ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PREVIEWVELOCITYSET20_127 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PREVIEWNOTESET30_127        ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PREVIEWNOTESET30_127     ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PREVIEWVELOCITYSET30_127    ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PREVIEWVELOCITYSET30_127 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PREVIEWNOTESET40_127        ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PREVIEWNOTESET40_127     ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PREVIEWVELOCITYSET40_127    ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PREVIEWVELOCITYSET40_127 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_TRANSMITPROGRAMCHANGE       ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_TRANSMITPROGRAMCHANGE    ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_TRANSMITBANKSELECT          ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_TRANSMITBANKSELECT       ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PATCHTRANSMITCHANNEL        ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PATCHTRANSMITCHANNEL     ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_TRANSPOSESWITCH             ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_TRANSPOSESWITCH          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_TRANSPOSEVALUE              ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_TRANSPOSEVALUE           ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_OCTAVESHIFT                 ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_OCTAVESHIFT              ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_KEYBOARDVELOCITY            ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_KEYBOARDVELOCITY         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_KEYBOARDSENS                ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_KEYBOARDSENS             ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_AFTERTOUCHSENS              ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_AFTERTOUCHSENS           ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PEDAL_1_ASSIGN              ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PEDAL_1_ASSIGN           ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PEDAL_1_OUTPUTMODE          ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PEDAL_1_OUTPUTMODE       ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PEDAL_1_POLARITY            ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PEDAL_1_POLARITY         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PEDAL2ASSIGN                ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PEDAL2ASSIGN             ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PEDAL2OUTPUTMODE            ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PEDAL2OUTPUTMODE         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PEDAL2POLARITY              ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PEDAL2POLARITY           ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_C1ASSIGN                    ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_C1ASSIGN                 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_C1OUTPUTMODE                ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_C1OUTPUTMODE             ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_C2ASSIGN                    ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_C2ASSIGN                 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_C2OUTPUTMODE                ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_C2OUTPUTMODE             ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_HOLDPEDALOUTPUTMODE         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_HOLDPEDALOUTPUTMODE      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_HOLDPEDALPOLARITY           ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_HOLDPEDALPOLARITY        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP1SWITCH      ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP1SWITCH   ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP1MSB         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP1MSB      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP1LSB         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP1LSB      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP2SWITCH      ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP2SWITCH   ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP2MSB0        ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP2MSB0     ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP2LSB0        ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP2LSB0     ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP3SWITCH      ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP3SWITCH   ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP3MSB         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP3MSB      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP3LSB         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP3LSB      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP4SWITCH      ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP4SWITCH   ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP4MSB         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP4MSB      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP4LSB         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP4LSB      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP5SWITCH      ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP5SWITCH   ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP5MSB         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP5MSB      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP5LSB         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP5LSB      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP6SWITCH      ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP6SWITCH   ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP6MSB         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP6MSB      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP6LSB         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP6LSB      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP7SWITCH      ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP7SWITCH   ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP7MSB         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP7MSB      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_BANKSELECTGROUP7LSB         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_BANKSELECTGROUP7LSB      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PEDAL3ASSIGN                ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PEDAL3ASSIGN             ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PEDAL3OUTPUTMODE            ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PEDAL3OUTPUTMODE         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PEDAL3POLARITY              ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PEDAL3POLARITY           ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PEDAL4ASSIGN                ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PEDAL4ASSIGN             ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PEDAL4OUTPUTMODE            ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PEDAL4OUTPUTMODE         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_PEDAL4POLARITY              ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_PEDAL4POLARITY           ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_ARPEGGIOSTYLE               ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_ARPEGGIOSTYLE            ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_ARPEGGIOMOTIF               ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_ARPEGGIOMOTIF            ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_ARPEGGIOBEATPATTERN         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_ARPEGGIOBEATPATTERN      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_ARPEGGIOACCENTRATE          ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_ARPEGGIOACCENTRATE       ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_ARPEGGIOSHUFFLERATE         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_ARPEGGIOSHUFFLERATE      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_ARPEGGIOKEYBOARDVELOCITY    ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_ARPEGGIOKEYBOARDVELOCITY ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_ARPEGGIOOCTAVERANGE         ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_ARPEGGIOOCTAVERANGE      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_ARPEGGIOPARTNUMBER          ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_ARPEGGIOPARTNUMBER       ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_SYSTEMTEMPO_H               ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_SYSTEMTEMPO_H            ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_SYSCMN_SYSTEMTEMPO_L               ), CB_SETCURSEL, dx100CtrlDataSysCommon[SYSCMN_SYSTEMTEMPO_L            ], (LPARAM)0 );
        break;
#endif
    case DX100_CTRL_SEQ_PATCH_COMMON:
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_00                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_00  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_01                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_01  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_02                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_02  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_03                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_03  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_04                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_04  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_05                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_05  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_06                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_06  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_07                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_07  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_08                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_08  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_09                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_09  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_10                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_10  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_11                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_11  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_12                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_12  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_13                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_13  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_14                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_14  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_15                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_15  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_16                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_16  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_17                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_17  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_18                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_18  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_19                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_19  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_20                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_20  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_21                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_21  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_22                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_22  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_23                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_23  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_24                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_24  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_25                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_25  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_26                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_26  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_27                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_27  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_28                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_28  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_29                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_29  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_30                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_30  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_31                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_31  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_32                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_32  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_33                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_33  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_34                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_34  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_35                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_35  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_36                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_36  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_37                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_37  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_38                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_38  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_39                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_39  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_40                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_40  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_41                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_41  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_42                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_42  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_43                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_43  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_44                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_44  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_45                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_45  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_46                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_46  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_47                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_47  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_48                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_48  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_49                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_49  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_50                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_50  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_51                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_51  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_52                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_52  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_53                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_53  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_54                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_54  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_55                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_55  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_56                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_56  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_57                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_57  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_58                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_58  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_59                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_59  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_60                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_60  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_61                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_61  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_62                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_62  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_63                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_63  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_64                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_64  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_65                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_65  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_66                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_66  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_67                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_67  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_68                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_68  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_69                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_69  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_70                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_70  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_71                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_71  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_72                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_72  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_73                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_73  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_74                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_74  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_75                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_75  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_76                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_76  ], (LPARAM)0 );
#if 1
        memset(&patchName[0],0,10+1);
        strncpy(&patchName[0],&dx100CtrlDataPatchCommon[VOICE_77],10);
        SetWindowText( ParamCtrlGetHWND(PARAM_CTRL_VOICE_NAME),&patchName[0]);
#else
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_77                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_77  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_78                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_78  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_79                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_79  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_80                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_80  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_81                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_81  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_82                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_82  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_83                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_83  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_84                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_84  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_85                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_85  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_86                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_86  ], (LPARAM)0 );
#endif
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_87                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_87  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_88                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_88  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_89                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_89  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_90                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_90  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_91                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_91  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_VOICE_92                  ), CB_SETCURSEL, dx100CtrlDataPatchCommon[VOICE_92  ], (LPARAM)0 );
        break;
#if 0
    case DX100_CTRL_SEQ_PATCH_TONE1 :
    case DX100_CTRL_SEQ_PATCH_TONE2 :
    case DX100_CTRL_SEQ_PATCH_TONE3 :
    case DX100_CTRL_SEQ_PATCH_TONE4 :
        toneNum = seqId - DX100_CTRL_SEQ_PATCH_TONE1;
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_TONESWITCH                 ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_TONESWITCH                 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_WAVEGROUPTYPE              ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_WAVEGROUPTYPE              ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_WAVEGROUPID                ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_WAVEGROUPID                ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_WAVENUMBERH                ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_WAVENUMBERH                ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_WAVENUMBERL                ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_WAVENUMBERL                ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_WAVEGAIN                   ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_WAVEGAIN                   ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FXMSWITCH                  ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FXMSWITCH                  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FXMCOLOR                   ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FXMCOLOR                   ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FXMDEPTH                   ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FXMDEPTH                   ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_TONEDELAYMODE              ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_TONEDELAYMODE              ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_TONEDELAYTIME              ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_TONEDELAYTIME              ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_VELOCITYCROSSFADE          ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_VELOCITYCROSSFADE          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_VELOCITYRANGELOWER         ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_VELOCITYRANGELOWER         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_VELOCITYRANGEUPPER         ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_VELOCITYRANGEUPPER         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_KEYBOARDRANGELOWER         ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_KEYBOARDRANGELOWER         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_KEYBOARDRANGEUPPER         ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_KEYBOARDRANGEUPPER         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_REDAMPERCONTROLSWITCH      ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_REDAMPERCONTROLSWITCH      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_VOLUMECONTROLSWITCH        ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_VOLUMECONTROLSWITCH        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_HOLD_1CONTROLSWITCH        ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_HOLD_1CONTROLSWITCH        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHBENDCONTROLSWITCH     ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHBENDCONTROLSWITCH     ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PANCONTROLSWITCH           ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PANCONTROLSWITCH           ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER1DESTINATION1    ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER1DESTINATION1    ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER1DEPTH1          ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER1DEPTH1          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER1DESTINATION2    ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER1DESTINATION2    ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER1DEPTH2          ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER1DEPTH2          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER1DESTINATION3    ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER1DESTINATION3    ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER1DEPTH3          ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER1DEPTH3          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER1DESTINATION4    ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER1DESTINATION4    ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER1DEPTH4          ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER1DEPTH4          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER2DESTINATION1    ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER2DESTINATION1    ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER2DEPTH1          ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER2DEPTH1          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER2DESTINATION2    ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER2DESTINATION2    ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER2DEPTH2          ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER2DEPTH2          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER2DESTINATION3    ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER2DESTINATION3    ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER2DEPTH3          ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER2DEPTH3          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER2DESTINATION4    ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER2DESTINATION4    ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER2DEPTH4          ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER2DEPTH4          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER3DESTINATION1    ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER3DESTINATION1    ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER3DEPTH1          ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER3DEPTH1          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER3DESTINATION2    ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER3DESTINATION2    ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER3DEPTH2          ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER3DEPTH2          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER3DESTINATION3    ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER3DESTINATION3    ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER3DEPTH3          ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER3DEPTH3          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER3DESTINATION4    ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER3DESTINATION4    ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CONTROLLER3DEPTH4          ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CONTROLLER3DEPTH4          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LFO1WAVEFORM               ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LFO1WAVEFORM               ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LFO1KEYSYNC                ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LFO1KEYSYNC                ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LFO1RATE                   ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LFO1RATE                   ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LFO1OFFSET                 ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LFO1OFFSET                 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LFO1DELAYTIME              ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LFO1DELAYTIME              ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LFO1FADEMODE               ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LFO1FADEMODE               ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LFO1FADETIME               ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LFO1FADETIME               ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LFO1EXTERNALSYNC           ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LFO1EXTERNALSYNC           ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LFO2WAVEFORM               ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LFO2WAVEFORM               ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LFO2KEYSYNC                ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LFO2KEYSYNC                ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LFO2RATE                   ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LFO2RATE                   ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LFO2OFFSET                 ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LFO2OFFSET                 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LFO2DELAYTIME              ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LFO2DELAYTIME              ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LFO2FADEMODE               ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LFO2FADEMODE               ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LFO2FADETIME               ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LFO2FADETIME               ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LFO2EXTERNALSYNC           ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LFO2EXTERNALSYNC           ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_COARSETUNE                 ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_COARSETUNE                 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FINETUNE                   ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FINETUNE                   ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_RANDOMPITCHDEPTH           ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_RANDOMPITCHDEPTH           ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHKEYFOLLOW             ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHKEYFOLLOW             ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHENVELOPEDEPTH         ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHENVELOPEDEPTH         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHENVELOPEVELOCITYSENS  ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHENVELOPEVELOCITYSENS  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHENVELOPEVELOCITYTIME1 ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHENVELOPEVELOCITYTIME1 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHENVELOPEVELOCITYTIME4 ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHENVELOPEVELOCITYTIME4 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHENVELOPETIMEKEYFOLLOW ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHENVELOPETIMEKEYFOLLOW ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHENVELOPETIME1         ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHENVELOPETIME1         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHENVELOPETIME2         ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHENVELOPETIME2         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHENVELOPETIME3         ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHENVELOPETIME3         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHENVELOPETIME4         ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHENVELOPETIME4         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHENVELOPELEVEL1        ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHENVELOPELEVEL1        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHENVELOPELEVEL2        ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHENVELOPELEVEL2        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHENVELOPELEVEL3        ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHENVELOPELEVEL3        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHENVELOPELEVEL4        ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHENVELOPELEVEL4        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHLFO1DEPTH             ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHLFO1DEPTH             ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PITCHLFO2DEPTH             ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PITCHLFO2DEPTH             ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERTYPE                 ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERTYPE                 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CUTOFFFREQUENCY            ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CUTOFFFREQUENCY            ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CUTOFFKEYFOLLOW            ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CUTOFFKEYFOLLOW            ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_RESONANCE                  ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_RESONANCE                  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_RESONANCEVELOCITYSENS      ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_RESONANCEVELOCITYSENS      ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERENVELOPEDEPTH        ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERENVELOPEDEPTH        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERENVELOPEVELOCITYCURVE), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERENVELOPEVELOCITYCURVE], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERENVELOPEVELOCITYSENS ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERENVELOPEVELOCITYSENS ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERENVELOPEVELOCITYTIME1), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERENVELOPEVELOCITYTIME1], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERENVELOPEVELOCITYTIME4), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERENVELOPEVELOCITYTIME4], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERENVELOPETIMEKEYFOLLOW), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERENVELOPETIMEKEYFOLLOW], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERENVELOPETIME1        ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERENVELOPETIME1        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERENVELOPETIME2        ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERENVELOPETIME2        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERENVELOPETIME3        ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERENVELOPETIME3        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERENVELOPETIME4        ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERENVELOPETIME4        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERENVELOPELEVEL1       ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERENVELOPELEVEL1       ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERENVELOPELEVEL2       ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERENVELOPELEVEL2       ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERENVELOPELEVEL3       ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERENVELOPELEVEL3       ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERENVELOPELEVEL4       ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERENVELOPELEVEL4       ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERLFO1DEPTH            ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERLFO1DEPTH            ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_FILTERLFO2DEPTH            ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_FILTERLFO2DEPTH            ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_TONELEVEL                  ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_TONELEVEL                  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_BIASDIRECTION              ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_BIASDIRECTION              ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_BIASPOSITION               ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_BIASPOSITION               ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_BIASLEVEL                  ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_BIASLEVEL                  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LEVELENVELOPEVELOCITYCURVE ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LEVELENVELOPEVELOCITYCURVE ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LEVELENVELOPEVELOCITYSENS  ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LEVELENVELOPEVELOCITYSENS  ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LEVELENVELOPEVELOCITYTIME1 ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LEVELENVELOPEVELOCITYTIME1 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LEVELENVELOPEVELOCITYTIME4 ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LEVELENVELOPEVELOCITYTIME4 ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LEVELENVELOPETIMEKEYFOLLOW ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LEVELENVELOPETIMEKEYFOLLOW ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LEVELENVELOPETIME1         ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LEVELENVELOPETIME1         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LEVELENVELOPETIME2         ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LEVELENVELOPETIME2         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LEVELENVELOPETIME3         ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LEVELENVELOPETIME3         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LEVELENVELOPETIME4         ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LEVELENVELOPETIME4         ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LEVELENVELOPELEVEL1        ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LEVELENVELOPELEVEL1        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LEVELENVELOPELEVEL2        ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LEVELENVELOPELEVEL2        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LEVELENVELOPELEVEL3        ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LEVELENVELOPELEVEL3        ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LEVELLFO1DEPTH             ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LEVELLFO1DEPTH             ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_LEVELLFO2DEPTH             ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_LEVELLFO2DEPTH             ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_TONEPAN                    ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_TONEPAN                    ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PANKEYFOLLOW               ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PANKEYFOLLOW               ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_RANDOMPANDEPTH             ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_RANDOMPANDEPTH             ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_ALTERNATEPANDEPTH          ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_ALTERNATEPANDEPTH          ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PANLFO1DEPTH               ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PANLFO1DEPTH               ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_PANLFO2DEPTH               ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_PANLFO2DEPTH               ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_OUTPUTASSIGN               ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_OUTPUTASSIGN               ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_MIX_EFXSENDLEVEL           ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_MIX_EFXSENDLEVEL           ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_CHORUSSENDLEVEL            ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_CHORUSSENDLEVEL            ], (LPARAM)0 );
        SendMessage( ParamCtrlGetHWND(PARAM_CTRL_PATCHTN_REVERBSENDLEVEL            ), CB_SETCURSEL, dx100CtrlDataPatchTone[toneNum][PATCHTONE_REVERBSENDLEVEL            ], (LPARAM)0 );
        break;
#endif
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

#if 0
    switch( seqId )
    {
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
    case DX100_CTRL_SEQ_PATCH_COMMON:
        break;
    case DX100_CTRL_SEQ_PATCH_TONE1 :
    case DX100_CTRL_SEQ_PATCH_TONE2 :
    case DX100_CTRL_SEQ_PATCH_TONE3 :
    case DX100_CTRL_SEQ_PATCH_TONE4 :
        toneNum = seqId - DX100_CTRL_SEQ_PATCH_TONE1;
        break;
    }
#endif

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
