/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */
#include "SomeCtrl.h"
#include "dx100ParamCtrl.h"
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
static BYTE getParamCtrlValue( DX100_PARAM_CTRL_ID id );
static BOOL displayContents( void );
static BYTE calcCheckSum( BYTE *dataPtr, INT dataSize );
static BOOL dx100voiceCopyFromAllToOne( INT voiceNum );
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
 * ����   : HINSTANCE hInst
 * ����   : PTSTR szAppName
 * ����   : HWND hwnd
 * �߂�l : BOOL
 **********************************************/
BOOL
Dx100CtrlInit( HINSTANCE hInst, PTSTR szAppName, HWND hwnd )
{
    BOOL bRtn = TRUE;

    dx100CtrlInfo.nowMode = DX100_CTRL_MODE_PATCH;

    Dx100ParamCtrlCreate( hInst, szAppName, hwnd ); /* �R���g���[���𐶐� */
    Dx100ParamCtrlGroupDisplay(DX100_PARAM_CTRL_GROUP_1VOICE);

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

    Dx100CtrlDisplayUpdate();

    return TRUE;
}

BOOL
Dx100CtrlDisplayUpdate( void )
{
    if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_NONE )
    {
        Dx100ParamCtrlGroupDisplay(DX100_PARAM_CTRL_GROUP_NONE);
    }
    else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_PATCH )
    {
        Dx100ParamCtrlGroupDisplay(DX100_PARAM_CTRL_GROUP_1VOICE);
    }
    else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_ALL_VOICE )
    {
        Dx100ParamCtrlGroupDisplay(DX100_PARAM_CTRL_GROUP_ALL_VOICE);
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
                { /* ���̃V�[�P���X������ */
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
            { /* GET�̎� */
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
            { /* SET�̎� */
                DebugWndPrintf("RX_NOT_EXEC\r\n");
            }

            if( dx100CtrlInfo.nowExecSeqId < dx100CtrlInfo.reqSeqIdEnd )
            { /* ���̃V�[�P���X������ */
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
                    if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_NONE )
                    {
                    }
                    else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_PATCH )
                    {
                        copyToParamCtrl(DX100_CTRL_SEQ_1VOICE);
                    }
                    else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_ALL_VOICE )
                    {
                        copyToParamCtrl(DX100_CTRL_SEQ_ALL_VOICE);
                    }
                    else
                    {
                    }

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

    tblPtr = &(dx100CtrlSeqDataTbl[seqId]);

    switch( seqId )
    {
    case DX100_CTRL_SEQ_1VOICE:
        if( method == DX100_CTRL_SEQ_METHOD_SET )
        {
            copyFromParamCtrl( seqId );
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
                paramCtrlIndex = DX100_PARAM_CTRL_VOICE_00 + i/*DX100_PARAM_CTRL_VOICE_00...DX100_PARAM_CTRL_VOICE_76*/;

                SendMessage( Dx100ParamCtrlGetHWND(paramCtrlIndex), CB_SETCURSEL, dx100CtrlDataOneVoice[dataIndex], (LPARAM)0 );
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
                    SetWindowText( Dx100ParamCtrlGetHWND(DX100_PARAM_CTRL_VOICE_NAME),&patchName[0]);
                }
                else
                {
                }
            }
            else
            {
                paramCtrlIndex = DX100_PARAM_CTRL_VOICE_87 + (i-DX100_SYSEX_VCED_87)/*DX100_PARAM_CTRL_VOICE_87...DX100_PARAM_CTRL_VOICE_92*/;

                SendMessage( Dx100ParamCtrlGetHWND(paramCtrlIndex), CB_SETCURSEL, dx100CtrlDataOneVoice[dataIndex], (LPARAM)0 );
            }

        }
        break;
    case DX100_CTRL_SEQ_ALL_VOICE:
        for( i=0; i<32; i++ )
        {
            INT j;
            memset(&patchName[0],0,10+1);
            strncpy(&patchName[0],&dx100CtrlDataAllVoice[ DX100_SYSEX_ALL_VOICE_DATA + (i*DX100_SYSEX_VMEM_MAX) + DX100_SYSEX_VMEM_57],10);
            SetWindowText( Dx100ParamCtrlGetHWND(DX100_PARAM_CTRL_ALL_VOICE_NAME_00+i),&patchName[0]);
#if 0 /* �p�b�`�����o�� */
            DebugWndPrintf("%s\r\n",&patchName[0]);
#endif
        }

        for( i=0; i<32; i++ )
        {
            INT j;
            memset(&szBuffer[0],0,128*2+1);
            for( j=0;j<128; j++ )
            {
                sprintf(&szBuffer[j*2],"%02X",dx100CtrlDataAllVoice[ DX100_SYSEX_ALL_VOICE_DATA + (i*DX100_SYSEX_VMEM_MAX) + j]);
            }
//            SetWindowText( Dx100ParamCtrlGetHWND(DX100_PARAM_CTRL_ALL_VOICE_BULK_00+i),&szBuffer[0]);

#if 0 /* �o���N�_���v�̃e�L�X�g���o�� */
            DebugWndPrintf("%s\r\n",&szBuffer[0]);
#endif
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
    BYTE checkSum;

    switch( seqId )
    {
    case DX100_CTRL_SEQ_1VOICE:
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_REQUEST_STATUS    ] = EX_STATUS;
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_REQUEST_ID_NO     ] = EX_ID_NUMBER_YAMAHA;
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_REQUEST_SUB_STATUS] = 0x00;
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_REQUEST_FORMAT_NO ] = DX100_DUMP_REQ_FORMAT_1VOICE;
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_BYTE_COUNT_HIGH   ] = 0x00;
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_BYTE_COUNT_LOW    ] = 0x5D;

        for( i=0; i<DX100_SYSEX_VCED_MAX; i++ )
        {
            INT dataIndex = DX100_SYSEX_1VOICE_DATA+i/*DX100_SYSEX_VCED_00...DX100_SYSEX_VCED_92*/;
            INT paramCtrlIndex;

            if( i < DX100_SYSEX_VCED_77 )
            {
                paramCtrlIndex = DX100_PARAM_CTRL_VOICE_00 + i/*DX100_PARAM_CTRL_VOICE_00...DX100_PARAM_CTRL_VOICE_76*/;

                dx100CtrlDataOneVoice[dataIndex] = getParamCtrlValue(paramCtrlIndex);
            }
            else if( (DX100_SYSEX_VCED_77 <= i) && (i <= DX100_SYSEX_VCED_86) )
            {
                if( DX100_SYSEX_VCED_77 == i )
                {
                    memset(&patchName[0],0,10+1);
                    Dx100ParamCtrlGetText(DX100_PARAM_CTRL_VOICE_NAME,patchName);
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
                paramCtrlIndex = DX100_PARAM_CTRL_VOICE_87 + (i-DX100_SYSEX_VCED_87)/*DX100_PARAM_CTRL_VOICE_87...DX100_PARAM_CTRL_VOICE_92*/;

                dx100CtrlDataOneVoice[dataIndex] = getParamCtrlValue(paramCtrlIndex);
            }

        }
        checkSum = calcCheckSum(&(dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA]),DX100_SYSEX_VCED_MAX);
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_FOOTER_CHECKSUM] = checkSum;
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_FOOTER_ETX] = EX_ETX;

        DebugWndPrintf("checkSum:0x%02X\r\n",checkSum);
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
getParamCtrlValue( DX100_PARAM_CTRL_ID id )
{
    BYTE value = (BYTE)0x00;
    int iCbNum;
    HWND hComboBox;

    hComboBox = Dx100ParamCtrlGetHWND(id);
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
        copyFromParamCtrl( seqId );

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

        if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_NONE )
        {
        }
        else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_PATCH )
        {
            copyToParamCtrl(DX100_CTRL_SEQ_1VOICE);
        }
        else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_ALL_VOICE )
        {
            copyToParamCtrl(DX100_CTRL_SEQ_ALL_VOICE);
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
 * ���e   : 
 * ����   : 
 * �߂�l : 
 **********************************************/
BOOL
Dx100CtrlOnCommand( WORD code )
{
    BOOL bRtn = (BOOL)FALSE;

    switch( code )
    {
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_00+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_01+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_02+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_03+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_04+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_05+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_06+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_07+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_08+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_09+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_10+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_11+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_12+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_13+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_14+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_15+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_16+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_17+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_18+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_19+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_20+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_21+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_22+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_23+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_24+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_25+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_26+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_27+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_28+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_29+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_30+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_31+DX100_PARAM_CTRL_ID_OFFSET):
        DebugWndPrintf("TO 1VOICE,%d\r\n",code-(DX100_PARAM_CTRL_ID_OFFSET+DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_00));
        dx100voiceCopyFromAllToOne(code-(DX100_PARAM_CTRL_ID_OFFSET+DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_00));
        bRtn = (BOOL)TRUE;
        break;
    default:
        break;
    }

    return bRtn;
}

/********************************************************************************
 * ���e  : �{�C�X�R�s�[(ALL VOICE����1VOICE��)
 * ����  : INT voiceNum
 * �߂�l: BOOL
 ***************************************/
static BOOL
dx100voiceCopyFromAllToOne( INT voiceNum )
{
    if( voiceNum < 32 )
    {
        INT topNum = DX100_SYSEX_ALL_VOICE_DATA + (voiceNum*DX100_SYSEX_VMEM_MAX);

        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_00]/* OP4 ATTACK RATE                                  */ = dx100CtrlDataAllVoice[topNum+ 0];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_01]/* OP4 DECAY 1 RATE                                 */ = dx100CtrlDataAllVoice[topNum+ 1];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_02]/* OP4 DECAY 2 RATE                                 */ = dx100CtrlDataAllVoice[topNum+ 2];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_03]/* OP4 RELEASE RATE                                 */ = dx100CtrlDataAllVoice[topNum+ 3];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_04]/* OP4 DECAY 1 LEVEL                                */ = dx100CtrlDataAllVoice[topNum+ 4];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_05]/* OP4 KEYBOARD SCALING LEVEL                       */ = dx100CtrlDataAllVoice[topNum+ 5];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_06]/* OP4 KEYBOARD SCALING RATE                        */ = (dx100CtrlDataAllVoice[topNum+ 9])>>3; /* BIT4-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_07]/* OP4 EG BIAS SENSITIVITY                          */ = ((dx100CtrlDataAllVoice[topNum+ 6])>>3)&0x7; /* BIT5-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_08]/* OP4 AMPLITUDE MODULATION ENABLE                  */ = ((dx100CtrlDataAllVoice[topNum+ 6])>>6)&0x1; /* BIT6   */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_09]/* OP4 KEY VELOCITY                                 */ = ((dx100CtrlDataAllVoice[topNum+ 6])   )&0x7; /* BIT2-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_10]/* OP4 OUTPUT LEVEL                                 */ = dx100CtrlDataAllVoice[topNum+ 7];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_11]/* OP4 OSCILLATOR FREQUENCY                         */ = dx100CtrlDataAllVoice[topNum+ 8];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_12]/* OP4 DETUNE                                       */ = (dx100CtrlDataAllVoice[topNum+ 9])&0x7; /* BIT2-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_13]/* OP2 ATTACK RATE                                  */ = dx100CtrlDataAllVoice[topNum+10];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_14]/* OP2 DECAY 1 RATE                                 */ = dx100CtrlDataAllVoice[topNum+11];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_15]/* OP2 DECAY 2 RATE                                 */ = dx100CtrlDataAllVoice[topNum+12];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_16]/* OP2 RELEASE RATE                                 */ = dx100CtrlDataAllVoice[topNum+13];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_17]/* OP2 DECAY 1 LEVEL                                */ = dx100CtrlDataAllVoice[topNum+14];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_18]/* OP2 KEYBOARD SCALING LEVEL                       */ = dx100CtrlDataAllVoice[topNum+15];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_19]/* OP2 KEYBOARD SCALING RATE                        */ = (dx100CtrlDataAllVoice[topNum+19])>>3; /* BIT4-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_20]/* OP2 EG BIAS SENSITIVITY                          */ = ((dx100CtrlDataAllVoice[topNum+16])>>3)&0x7; /* BIT5-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_21]/* OP2 AMPLITUDE MODULATION ENABLE                  */ = ((dx100CtrlDataAllVoice[topNum+16])>>6)&0x1; /* BIT6 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_22]/* OP2 KEY VELOCITY                                 */ = ((dx100CtrlDataAllVoice[topNum+16])   )&0x7; /* BIT2-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_23]/* OP2 OUTPUT LEVEL                                 */ = dx100CtrlDataAllVoice[topNum+17];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_24]/* OP2 OSCILLATOR FREQUENCY                         */ = dx100CtrlDataAllVoice[topNum+18];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_25]/* OP2 DETUNE                                       */ = (dx100CtrlDataAllVoice[topNum+19])&0x7; /* BIT2-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_26]/* OP3 ATTACK RATE                                  */ = dx100CtrlDataAllVoice[topNum+20];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_27]/* OP3 DECAY 1 RATE                                 */ = dx100CtrlDataAllVoice[topNum+21];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_28]/* OP3 DECAY 2 RATE                                 */ = dx100CtrlDataAllVoice[topNum+22];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_29]/* OP3 RELEASE RATE                                 */ = dx100CtrlDataAllVoice[topNum+23];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_30]/* OP3 DECAY 1 LEVEL                                */ = dx100CtrlDataAllVoice[topNum+24];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_31]/* OP3 KEYBOARD SCALING LEVEL                       */ = dx100CtrlDataAllVoice[topNum+25];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_32]/* OP3 KEYBOARD SCALING RATE                        */ = (dx100CtrlDataAllVoice[topNum+29])>>3; /* BIT4-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_33]/* OP3 EG BIAS SENSITIVITY                          */ = ((dx100CtrlDataAllVoice[topNum+26])>>3)&0x7; /* BIT5-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_34]/* OP3 AMPLITUDE MODULATION ENABLE                  */ = ((dx100CtrlDataAllVoice[topNum+26])>>6)&0x1; /* BIT6   */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_35]/* OP3 KEY VELOCITY                                 */ = ((dx100CtrlDataAllVoice[topNum+26])   )&0x7; /* BIT2-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_36]/* OP3 OUTPUT LEVEL                                 */ = dx100CtrlDataAllVoice[topNum+27];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_37]/* OP3 OSCILLATOR FREQUENCY                         */ = dx100CtrlDataAllVoice[topNum+28];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_38]/* OP3 DETUNE                                       */ = (dx100CtrlDataAllVoice[topNum+29])&0x7; /*BIT2-0*/
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_39]/* OP1 ATTACK RATE                                  */ = dx100CtrlDataAllVoice[topNum+30];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_40]/* OP1 DECAY 1 RATE                                 */ = dx100CtrlDataAllVoice[topNum+31];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_41]/* OP1 DECAY 2 RATE                                 */ = dx100CtrlDataAllVoice[topNum+32];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_42]/* OP1 RELEASE RATE                                 */ = dx100CtrlDataAllVoice[topNum+33];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_43]/* OP1 DECAY 1 LEVEL                                */ = dx100CtrlDataAllVoice[topNum+34];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_44]/* OP1 KEYBOARD SCALING LEVEL                       */ = dx100CtrlDataAllVoice[topNum+35];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_45]/* OP1 KEYBOARD SCALING RATE                        */ = (dx100CtrlDataAllVoice[topNum+39])>>3; /* BIT4-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_46]/* OP1 EG BIAS SENSITIVITY                          */ = ((dx100CtrlDataAllVoice[topNum+36])>>3)&0x7; /* BIT5-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_47]/* OP1 AMPLITUDE MODULATION ENABLE                  */ = ((dx100CtrlDataAllVoice[topNum+36])>>6)&0x1; /* BIT6   */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_48]/* OP1 KEY VELOCITY                                 */ = ((dx100CtrlDataAllVoice[topNum+36])   )&0x7; /* BIT2-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_49]/* OP1 OUTPUT LEVEL                                 */ = dx100CtrlDataAllVoice[topNum+37];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_50]/* OP1 OSCILLATOR FREQUENCY                         */ = dx100CtrlDataAllVoice[topNum+38];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_51]/* OP1 DETUNE                                       */ = (dx100CtrlDataAllVoice[topNum+39])&0x7; /* BIT2-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_52]/* ALGORITHM                                        */ = (dx100CtrlDataAllVoice[topNum+40])&0x7; /* BIT2-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_53]/* FEEDBACK LEVEL                                   */ = ((dx100CtrlDataAllVoice[topNum+40])>>3)&0x7; /* BIT5-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_54]/* LFO SPEED                                        */ = dx100CtrlDataAllVoice[topNum+41];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_55]/* LFO DELAY                                        */ = dx100CtrlDataAllVoice[topNum+42];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_56]/* PITCH MODULATION DEPTH                           */ = dx100CtrlDataAllVoice[topNum+43];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_57]/* amplitude MODULATION depth                       */ = dx100CtrlDataAllVoice[topNum+44];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_58]/* LFO SYNC                                         */ = (dx100CtrlDataAllVoice[topNum+40])>>6; /* BIT7-6 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_59]/* lfo wave                                         */ = (dx100CtrlDataAllVoice[topNum+45])&0x3; /* BIT1-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_60]/* pitch modulation sensitivity                     */ = ((dx100CtrlDataAllVoice[topNum+45])>>4)&0x7; /* BIT6-4 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_61]/* amplitude modulation sensitivity                 */ = ((dx100CtrlDataAllVoice[topNum+45])>>2)&0x3; /* BIT3-2 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_62]/* transpose                                        */ = dx100CtrlDataAllVoice[topNum+46];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_63]/* play mode poly/mono                              */ = ((dx100CtrlDataAllVoice[topNum+48])>>3)&0x1; /* BIT3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_64]/* pitch bend range                                 */ = dx100CtrlDataAllVoice[topNum+47];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_65]/* portamento mode                                  */ = (dx100CtrlDataAllVoice[topNum+48])&0x1; /* BIT0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_66]/* portamento time                                  */ = dx100CtrlDataAllVoice[topNum+49];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_67]/* foot volume range                                */ = dx100CtrlDataAllVoice[topNum+50];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_68]/* sustain foot switch                              */ = ((dx100CtrlDataAllVoice[topNum+48])>>2)&0x1; /* BIT2 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_69]/* portamento foot switch                           */ = ((dx100CtrlDataAllVoice[topNum+48])>>1)&0x1; /* BIT1 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_70]/* chorus switch                                    */ = ((dx100CtrlDataAllVoice[topNum+48])>>4)&0x1; /* BIT4 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_71]/* modulation wheel pitch modulation range          */ = dx100CtrlDataAllVoice[topNum+51];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_72]/* modulation wheel amplitude modulation range      */ = dx100CtrlDataAllVoice[topNum+52];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_73]/* breath control pitch modulation range            */ = dx100CtrlDataAllVoice[topNum+53];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_74]/* breath control amplitude modulation range        */ = dx100CtrlDataAllVoice[topNum+54];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_75]/* breath control pitch bias range                  */ = dx100CtrlDataAllVoice[topNum+55];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_76]/* breath control eg bias range                     */ = dx100CtrlDataAllVoice[topNum+56];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_77]/* voice name 1                                     */ = dx100CtrlDataAllVoice[topNum+57];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_78]/* voice name 2                                     */ = dx100CtrlDataAllVoice[topNum+58];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_79]/* voice name 3                                     */ = dx100CtrlDataAllVoice[topNum+59];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_80]/* voice name 4                                     */ = dx100CtrlDataAllVoice[topNum+60];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_81]/* voice name 5                                     */ = dx100CtrlDataAllVoice[topNum+61];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_82]/* voice name 6                                     */ = dx100CtrlDataAllVoice[topNum+62];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_83]/* voice name 7                                     */ = dx100CtrlDataAllVoice[topNum+63];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_84]/* voice name 8                                     */ = dx100CtrlDataAllVoice[topNum+64];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_85]/* voice name 9                                     */ = dx100CtrlDataAllVoice[topNum+65];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_86]/* voice name 10                                    */ = dx100CtrlDataAllVoice[topNum+66];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_87]/* pitch eg rate 1                                  */ = dx100CtrlDataAllVoice[topNum+67];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_88]/* pitch eg rate 2                                  */ = dx100CtrlDataAllVoice[topNum+68];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_89]/* pitch eg rate 3                                  */ = dx100CtrlDataAllVoice[topNum+69];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_90]/* pitch eg level 1                                 */ = dx100CtrlDataAllVoice[topNum+70];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_91]/* pitch eg level 2                                 */ = dx100CtrlDataAllVoice[topNum+71];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_92]/* pitch eg level 3                                 */ = dx100CtrlDataAllVoice[topNum+72];

        copyToParamCtrl(DX100_CTRL_SEQ_1VOICE);
    }
    else
    {
        nop();
    }
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
