/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */
#include "SomeCtrl.h"
#include "ParamCtrl.h"
#include "Midi.h"
#include "dx100Ctrl.h"
#include "DebugWnd.h"
#include "dx100CtrlDef.h"

/* �O���֐���` */

/* �O���ϐ���` */

/* �����֐���` */
static BOOL seqStart( DX100_CTRL_SEQ_METHOD method, DX100_CTRL_SEQ_ID seqId );
static INT seqStartProc( DX100_CTRL_SEQ_METHOD method, DX100_CTRL_SEQ_ID seqId, INT maxDataSize, BYTE *txDataPtr );
static BOOL seqEndProc( DX100_CTRL_SEQ_ID seqId, INT rxDataSize, BYTE *rxDataPtr );
static BOOL copyToParamCtrl( DX100_CTRL_SEQ_ID seqId );
static BOOL copyFromParamCtrl( DX100_CTRL_SEQ_ID seqId );
static BYTE getParamCtrlValue( PARAM_CTRL_ID id );
static BOOL displayContents( void );
static BYTE calcCheckSum( BYTE *dataPtr, INT dataSize );
static void debugDataArrayPrint( INT rxDataSize, BYTE *rxDataPtr, PTSTR ptstrTitle );

/* �����ϐ���` */
static BYTE dx100CtrlSeqTxTempData[1024*8];
static BYTE dx100CtrlSeqRxTempData[1024*8];
static INT dx100CtrlSeqTempRxSize;

static BYTE dx100CtrlDataSysCommon[SYSCMN_INDEX_MAX];
static BYTE dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_INDEX_MAX];
static BYTE dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_INDEX_MAX];

typedef struct
{
    INT   rxDataSize  ; /* ��M�f�[�^�i�[�̈�̃T�C�Y               */
    BYTE *rxDataPtr   ; /* ��M�f�[�^�i�[�̈�̃|�C���^             */
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
    DX100_CTRL_SEQ_METHOD          nowMethod            ;
    DX100_CTRL_SEQ_ID              reqSeqIdStart        ;
    DX100_CTRL_SEQ_ID              reqSeqIdEnd          ;
    DX100_CTRL_SEQ_ID              nowExecSeqId         ;
    DX100_CTRL_MODE                nowMode              ;
} S_DX100_CTRL_INFO;

static S_DX100_CTRL_INFO dx100CtrlInfo;


/*********************************************
 * ���e   : ������
 * ����   : �Ȃ�
 * �߂�l : BOOL
 **********************************************/
BOOL
Dx100CtrlInit( void )
{
    BOOL bRtn = TRUE;

    dx100CtrlInfo.nowMode = DX100_CTRL_MODE_PATCH;

    return bRtn;
}

/*********************************************
 * ���e   : 
 * ����   : DX100_CTRL_MODE mode
 * �߂�l : BOOL
 **********************************************/
BOOL
Dx100CtrlModeSet(DX100_CTRL_MODE mode)
{
    dx100CtrlInfo.nowMode = mode;

    return TRUE;
}

BOOL
Dx100CtrlDisplayUpdate( void )
{
    if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_SYSTEM )
    {
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
 * @brief  �V�[�P���X�J�n
 *         �J�nID����I��ID�܂ŘA�����Ď��s����
 * @param  method
 * @param  seqIdStart
 * @param  seqIdEnd
 * @return TRUE: ����Ɏ��s�J�n
 * @author wurly @date 2017�N06��03�� 21��16��31�b
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
 * ���e   : �R���e���c�\��
 * ����   : �Ȃ�
 * �߂�l : BOOL
 **********************************************/
BOOL
Dx100CtrlDisplayContents( void )
{
    return displayContents();
}

/*********************************************
 * ���e   : 
 * ����   : 
 * �߂�l : 
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
        { /* ���M�I�� */
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
        { /* ��M�I�� */
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
                /* �S�ẴV�[�P���X�I�� */
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
 * ���e   : �V�[�P���X�J�n
 * ����   : 
 * �߂�l : BOOL
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
 * ���e   : 
 * ����   : DX100_CTRL_SEQ_ID seqId
 * �߂�l : INT
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
        { /* �f�[�^�v�����f�[�^��M����*/
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
        { /* �f�[�^�v�����f�[�^��M����*/
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
 * ���e   : 
 * ����   : DX100_CTRL_SEQ_ID seqId
 * �߂�l : BOOL
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
    case DX100_CTRL_SEQ_1VOICE:
        break;
    }

    return TRUE;
}

/*********************************************
 * ���e   : 
 * ����   : DX100_CTRL_SEQ_ID seqId
 * �߂�l : BOOL
 **********************************************/
static BOOL
copyToParamCtrl( DX100_CTRL_SEQ_ID seqId )
{
    TCHAR patchName[12+1];
    TCHAR szBuffer[1024];
    INT toneNum;
    INT i,j;

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
        }
        break;
    }

    return TRUE;
}

/*********************************************
 * ���e   : 
 * ����   : DX100_CTRL_SEQ_ID seqId
 * �߂�l : BOOL
 **********************************************/
static BOOL
copyFromParamCtrl( DX100_CTRL_SEQ_ID seqId )
{
    TCHAR patchName[12+1];
    TCHAR szBuffer[1024];
    INT toneNum;
    INT i,j;

    switch( seqId )
    {
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
                    memset(&dx100CtrlDataOneVoice[dataIndex],0,10+1);
                    strncpy(&dx100CtrlDataOneVoice[dataIndex],&patchName[0],10);

                    for( j=0; j<10; j++ )
                    {
                        if( dx100CtrlDataOneVoice[dataIndex+j] == '\0' )
                        {
                            dx100CtrlDataOneVoice[dataIndex+j] = ' ';
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
                paramCtrlIndex = PARAM_CTRL_VOICE_87 + (i-DX100_SYSEX_VCED_87)/*PARAM_CTRL_VOICE_87...PARAM_CTRL_VOICE_92*/;

                dx100CtrlDataOneVoice[dataIndex] = getParamCtrlValue(paramCtrlIndex);
            }

        }
        break;
    case DX100_CTRL_SEQ_ALL_VOICE:
        break;
    }

    return TRUE;
}

/*********************************************
 * ���e   : 
 * ����   : DX100_CTRL_SEQ_ID seqId
 * �߂�l : BOOL
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
 * ���e   : 
 * ����   : 
 * �߂�l : 
 **********************************************/
static BOOL
displayContents( void )
{
    INT i;

    return TRUE;
}

/*********************************************
 * ���e   : 
 * ����   : 
 * �߂�l : 
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
 * ���e  : �f�[�^�T�C�Y�擾
 * ����  : seqId
 * �߂�l: DWORD
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
 * ���e  : �f�[�^�擾
 * ����  : seqId
 * ����  : TCHAR *dataPtr
 * ����  : DWORD dataSize
 * �߂�l: BOOL
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
 * ���e  : �f�[�^�Z�b�g
 * ����  : seqId
 * ����  : TCHAR *dataPtr
 * ����  : DWORD dataSize
 * �߂�l: BOOL
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
 * ���e   : 
 * ����   : DX100_CTRL_SEQ_ID seqId
 * �߂�l : �Ȃ�
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
