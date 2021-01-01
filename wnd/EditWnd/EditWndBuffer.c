/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */
#include "DebugWnd.h"
#include "LinkedList.h"
#include "EditWndBufferDataType.h"

/* �O���֐���` */

/* �O���ϐ���` */

/* �����֐���` */
#include "EditWndBuffer.h"

typedef struct tagS_EDITWND_BUFF_LOCAL
{
    struct
    {
        S_BUFF_LINE_DATA *topPtr       ; /* �擪�̃f�[�^                           */
        S_BUFF_LINE_DATA *endPtr       ; /* �Ō�̃f�[�^                           */
        S_BUFF_LINE_DATA *nowPtr       ; /* �L�����b�g������f�[�^                 */
        S_BUFF_LINE_DATA *selectPtr    ; /* �I��͈͂̐擪�̃f�[�^                 */
        DWORD            selectCaretPos; /* �I��͈͂̐擪�̃f�[�^�̃L�����b�g�ʒu */
    } lineData;
    UINT  NewLineType;
    DWORD xCaret     ;
    DWORD yCaret     ;
    INT   tabSize    ;
} S_EDITWND_BUFF_LOCAL;
typedef S_EDITWND_BUFF_LOCAL * H_EDITWND_BUFF_LOCAL;

static S_BUFF_LINE_DATA *getBuffLine( H_EDITWND_BUFF_LOCAL h, TCHAR *dataPtr, DWORD maxLength );
static CHARSET_TYPE detectCharSet( S_BUFF_LINE_DATA *dataPtr, DWORD offset );
static DWORD getCaretDispXpos( H_EDITWND_BUFF_LOCAL h, S_BUFF_LINE_DATA *linePtr, DWORD dataPos );
static BOOL getDispCharData( H_EDITWND_BUFF_LOCAL h, S_BUFF_LINE_DATA *linePtr, DWORD dispPos, S_BUFF_DISP_DATA *dataPtr );
static void setSelectPosNowPosToFar( H_EDITWND_BUFF_LOCAL h, BOOL bMinus, DWORD offset );

/* �����ϐ���` */

#if 0
#define EDITWNDBUFF_DEBUG_PRINTF DebugWndPrintf
#else
#define EDITWNDBUFF_DEBUG_PRINTF
#endif

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃I�u�W�F�N�g����
 * ����  : �Ȃ�
 * �߂�l: H_EDITWND_BUFF
 ***************************************/
H_EDITWND_BUFF
EditWndBuffCreate( void )
{
    H_EDITWND_BUFF_LOCAL h;

    h = (H_EDITWND_BUFF_LOCAL)malloc( sizeof(S_EDITWND_BUFF_LOCAL) );
    if( h != NULL )
    {
        h->lineData.topPtr         = NULL;
        h->lineData.endPtr         = NULL;
        h->lineData.nowPtr         = NULL;
        h->lineData.selectPtr      = NULL;
        h->lineData.selectCaretPos = 0;
        h->NewLineType = EDITWND_BUFF_NEWLINE_CRLF;
        h->xCaret      = 0;
        h->yCaret      = 0;
        h->tabSize     = 8;
    }
    else
    {
        nop();
    }

    return (H_EDITWND_BUFF)h;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃I�u�W�F�N�g�j��
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: BOOL
 ***************************************/
BOOL
EditWndBuffDestroy( H_EDITWND_BUFF hEditWndBuff )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( (h != NULL) )
    {
        free( h );
    }
    else
    {
        nop();
    }

    return TRUE;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̏�����
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: �Ȃ�
 ***************************************/
void
EditWndBuffInit( H_EDITWND_BUFF hEditWndBuff )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;
    S_BUFF_LINE_DATA *lineDataPtr;

    EditWndBufferAllRemoveLinkedList(&(h->lineData.topPtr),&(h->lineData.endPtr));

    /* ��f�[�^��ǉ� */
    lineDataPtr = BuffLineDataCreate( 0, 0, NULL, 0, 0 );
    if( lineDataPtr != NULL )
    {
        EditWndBufferAddLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),lineDataPtr );
        (h->lineData.nowPtr) = lineDataPtr;
    }
    else
    {
        nop();
    }

    (h->lineData.selectPtr) = NULL;
    (h->lineData.selectCaretPos) = 0;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̏I��
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: �Ȃ�
 ***************************************/
void
EditWndBuffEnd( H_EDITWND_BUFF hEditWndBuff )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    EditWndBufferAllRemoveLinkedList(&(h->lineData.topPtr),&(h->lineData.endPtr));
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃f�[�^�Z�b�g
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : TCHAR* dataPtr
 * ����  : DWORD  length
 * ����  : BOOL   bInit  (TRUE:�����f�[�^���N���A,FALSE:�N���A���Ȃ�)
 * �߂�l: �Ȃ�
 ***************************************/
void
EditWndBuffDataSet( H_EDITWND_BUFF hEditWndBuff, TCHAR* dataPtr, DWORD length, BOOL bInit )
{
    S_BUFF_LINE_DATA *lineDataPtr;
    S_BUFF_LINE_DATA *tempTopPtr = NULL;
    S_BUFF_LINE_DATA *tempEndPtr = NULL;
    S_BUFF_LINE_DATA *newPtr,*targetPtr;
    S_BUFF_LINE_DATA *dividePrePtr,*dividePostPtr;
    DWORD lineLengthSum = 0;
    DWORD caretPos = 0;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( bInit )
    {
        EditWndBuffInit(hEditWndBuff);
    }
    else
    {
        nop();
    }

    if( (dataPtr != NULL) && (length > 0) )
    { /* �f�[�^�L��̏ꍇ */
        EDITWNDBUFF_DEBUG_PRINTF("EditWndBuffDataSet,");

        /* ���s�ŕ��������f�[�^�����A�����X�g(tempTopPtr�`tempEndPtr)�ɓo�^(��������) */
        while( lineLengthSum < length )
        {
            lineDataPtr = (S_BUFF_LINE_DATA *)getBuffLine( h, dataPtr+lineLengthSum, length - lineLengthSum );

            if( lineDataPtr != NULL )
            {
                EditWndBufferAddLinkedList( &tempTopPtr,&tempEndPtr,lineDataPtr );
                lineLengthSum += lineDataPtr->dataSize;
            }
            else
            {
                break;
            }
        }
        /* ���s�ŕ��������f�[�^�����A�����X�g(tempTopPtr�`tempEndPtr)�ɓo�^(�����܂�) */

        if( (h->lineData.nowPtr)->caretDataPos == 0 )
        { /* �s�̐擪�ɑ}�� */
            EDITWNDBUFF_DEBUG_PRINTF("Top,");
            EditWndBufferInsertLinkedList(&(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),&tempTopPtr,&tempEndPtr);

            if( tempEndPtr->newLineCodeSize == 0 )
            { /* �}���f�[�^�̍Ō�ɉ��s���Ȃ���� */
                newPtr = BuffLineDataJoin(tempEndPtr,(h->lineData.nowPtr)); /* �ǉ��f�[�^�̍ŏI�s�ƌ��݂̍s�ƌ��������V���ȃf�[�^���� */
                if( newPtr != NULL )
                {
                    targetPtr = (h->lineData.nowPtr);
                    (h->lineData.nowPtr) = EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),newPtr ); /* ���݂̍s��u������ */
                    BuffLineDataDestroy( targetPtr );                                                       /* ���݂̍s�̃f�[�^�����                   */

                    caretPos = tempEndPtr->dataSize - tempEndPtr->newLineCodeSize;           /* �L�����b�g�ʒu�͒ǉ��f�[�^�̍ŏI�s�̍Ō� */
                    EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),tempEndPtr); /* �ŏI�s���͕̂s�v�Ȃ̂ŘA�����X�g����O�� */
                    BuffLineDataDestroy( tempEndPtr );

                    (h->lineData.nowPtr) = newPtr;                                            /* �����������̂����ݍs�Ƃ��� */
                    (h->lineData.nowPtr)->caretDataPos = caretPos;                                /* �L�����b�g�ʒu�͌����ʒu     */
                }
                else
                {
                    nop();
                }
            }
            else
            { /* �}���f�[�^�̍Ō�ɉ��s�������� */
                (h->lineData.nowPtr)->caretDataPos = 0;
            }
        }
        else
        { /* �s�̓r���ɑ}�� */
            /* ���݂̍s���L�����b�g�ʒu�ŕ���(��������) */
            BuffLineDataDivide( (h->lineData.nowPtr), &dividePrePtr, &dividePostPtr );
            if( dividePrePtr != NULL )
            {
                if( dividePostPtr != NULL )
                {
                    EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),dividePostPtr ); /* �܂� �Â��f�[�^�����s�ȍ~�̃f�[�^�ɒu������ */
                    EditWndBufferInsertLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),dividePostPtr,&dividePrePtr,&dividePrePtr ); /* ���̑O�ɉ��s�O�̃f�[�^��}�� */

                    BuffLineDataDestroy( (h->lineData.nowPtr) );
                    (h->lineData.nowPtr) = dividePostPtr;
                    (h->lineData.nowPtr)->caretDataPos = 0;
                    /* ���݂̍s���L�����b�g�ʒu�ŕ���(�����܂�) */

                    if( tempTopPtr == tempEndPtr )
                    { /* 1�s�}���̏ꍇ */
                        EditWndBufferInsertLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),&tempEndPtr,&tempEndPtr );

                        /* ���������O���f�[�^�ƁA�}���f�[�^�̐擪������(��������) */
                        newPtr = BuffLineDataJoin( dividePrePtr, tempTopPtr );
                        if( newPtr != NULL )
                        {
                            targetPtr = tempTopPtr;
                            EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),tempTopPtr,newPtr );
                            BuffLineDataDestroy( targetPtr );
                            EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),dividePrePtr);
                            BuffLineDataDestroy( dividePrePtr );
                        }
                        else
                        {
                            nop();
                        }
                        tempTopPtr = tempEndPtr = newPtr;

                        if( tempEndPtr->newLineCodeSize == 0 )
                        { /* �}���f�[�^�̍Ō�ɉ��s���Ȃ���� */
                            newPtr = BuffLineDataJoin(tempEndPtr,dividePostPtr); /* �ǉ��f�[�^�̍ŏI�s�Ɖ��s�ȍ~�̍s�ƌ��� */
                            if( newPtr != NULL )
                            {
                                newPtr->caretDataPos = tempEndPtr->dataSize - tempEndPtr->newLineCodeSize;
                                EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),dividePostPtr,newPtr );
                                BuffLineDataDestroy( dividePostPtr );
                                (h->lineData.nowPtr) = newPtr;
                                EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),tempEndPtr);
                                BuffLineDataDestroy( tempEndPtr );
                            }
                            else
                            {
                                nop();
                            }
                        }
                        else
                        { /* �}���f�[�^�̍Ō�ɉ��s�L�� */
                            nop();
                        }
                    }
                    else
                    { /* �����s�}���̏ꍇ */
                        /* ���������ʒu�Ƀf�[�^��}��(��������) */
                        EditWndBufferInsertLinkedList(&(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),&tempTopPtr,&tempEndPtr);

                        if( tempEndPtr->newLineCodeSize == 0 )
                        { /* �}���f�[�^�̍Ō�ɉ��s���Ȃ���� */
                            newPtr = BuffLineDataJoin(tempEndPtr,dividePostPtr); /* �ǉ��f�[�^�̍ŏI�s�Ɖ��s�ȍ~�̍s�ƌ��� */
                            if( newPtr != NULL )
                            {
                                newPtr->caretDataPos = tempEndPtr->dataSize - tempEndPtr->newLineCodeSize;
                                EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),dividePostPtr,newPtr );
                                BuffLineDataDestroy( dividePostPtr );
                                (h->lineData.nowPtr) = newPtr;
                                EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),tempEndPtr);
                                BuffLineDataDestroy( tempEndPtr );
                            }
                            else
                            {
                                nop();
                            }
                        }
                        else
                        { /* �}���f�[�^�̍Ō�ɉ��s�L�� */
                            nop();
                        }

                        /* ���������O���f�[�^�ƁA�}���f�[�^�̐擪������(��������) */
                        newPtr = BuffLineDataJoin( dividePrePtr, tempTopPtr );
                        if( newPtr != NULL )
                        {
                            targetPtr = tempTopPtr;
                            EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),tempTopPtr,newPtr );
                            BuffLineDataDestroy( targetPtr );
                            EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),dividePrePtr);
                            BuffLineDataDestroy( dividePrePtr );
                        }
                        else
                        {
                            nop();
                        }
                    }
                }
                else
                {
                    BuffLineDataDestroy( dividePrePtr );
                }
            }
            else
            {
                nop();
            }
        }

        EDITWNDBUFF_DEBUG_PRINTF("0x%02X,%d\r\n",(BYTE)*dataPtr,length);
    }
    else
    { /* �f�[�^�����̏ꍇ */
        nop();
    }

    if( bInit )
    {
        if( (h->lineData.topPtr) != NULL )
        {
            (h->lineData.nowPtr) = (h->lineData.topPtr);
            (h->lineData.nowPtr)->caretDataPos = 0;
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

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃f�[�^�擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : TCHAR *dataPtr
 * ����  : DWORD dataSize
 * ����  : EDITWND_BUFF_REGION region
 * �߂�l: BOOL
 ***************************************/
BOOL
EditWndBuffDataGet( H_EDITWND_BUFF hEditWndBuff, TCHAR *dataPtr, DWORD dataSize, EDITWND_BUFF_REGION region )
{
    BOOL rtn = FALSE;
    S_BUFF_LINE_DATA *nowPtr,*prevPtr;
    DWORD size;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( region == BUFF_ALL )
    {
        if( (h->lineData.topPtr) == NULL )
        {
            nop();
        }
        else
        {
            if( dataPtr != NULL )
            {
                for( nowPtr=(h->lineData.topPtr); nowPtr != NULL; nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr )
                {
                    memcpy( dataPtr, nowPtr->data, nowPtr->dataSize );
                    dataPtr += nowPtr->dataSize;
                }
            }
            else
            {
                nop();
            }
            rtn = TRUE;
        }
    }
    else
    {
        if( dataPtr != NULL )
        {
            if( (h->lineData.selectPtr) != NULL )
            { /* �I���J�n�ʒu�L�� */
                if( ((h->lineData.selectPtr)->lineNum) < ((h->lineData.nowPtr)->lineNum) )
                { /* �������ɑI�� */
                    for( nowPtr = (h->lineData.selectPtr),prevPtr = NULL; prevPtr != (h->lineData.nowPtr); nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr )
                    {
                        if( nowPtr == (h->lineData.selectPtr) )
                        {
                            size = (nowPtr->dataSize - (h->lineData.selectCaretPos));
                            memcpy( dataPtr, (nowPtr->data + (h->lineData.selectCaretPos)),size );
                            dataPtr += size;
                        }
                        else if( nowPtr == (h->lineData.nowPtr) )
                        {
                            size = nowPtr->caretDataPos;
                            memcpy( dataPtr,nowPtr->data,size );
                            dataPtr += size;
                        }
                        else
                        {
                            size = nowPtr->dataSize;
                            memcpy( dataPtr, nowPtr->data,size );
                            dataPtr += size;
                        }
                        prevPtr = nowPtr;
                    }
                }
                else if( ((h->lineData.selectPtr)->lineNum) > ((h->lineData.nowPtr)->lineNum) )
                { /* �������ɑI�� */
                    for( nowPtr = (h->lineData.nowPtr),prevPtr = NULL; prevPtr != (h->lineData.selectPtr); nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr )
                    {
                        if( nowPtr == (h->lineData.nowPtr) )
                        {
                            size = (nowPtr->dataSize - nowPtr->caretDataPos);
                            memcpy( dataPtr, (nowPtr->data + nowPtr->caretDataPos),size );
                            dataPtr += size;
                        }
                        else if( nowPtr == (h->lineData.selectPtr) )
                        {
                            size = (h->lineData.selectCaretPos);
                            memcpy( dataPtr,nowPtr->data,size );
                            dataPtr += size;
                        }
                        else
                        {
                            size = nowPtr->dataSize;
                            memcpy( dataPtr, nowPtr->data,size );
                            dataPtr += size;
                        }
                        prevPtr = nowPtr;
                    }
                }
                else
                { /* ����s���őI�� */
                    if( (h->lineData.nowPtr) == (h->lineData.selectPtr) )
                    {
                        if( (h->lineData.nowPtr)->caretDataPos < (h->lineData.selectCaretPos) )
                        {
                            size = ((h->lineData.selectCaretPos) - (h->lineData.nowPtr)->caretDataPos);
                            memcpy( dataPtr, ((h->lineData.nowPtr)->data + (h->lineData.nowPtr)->caretDataPos),size );
                            dataPtr += size;
                        }
                        else if( (h->lineData.nowPtr)->caretDataPos > (h->lineData.selectCaretPos) )
                        {
                            size = ((h->lineData.nowPtr)->caretDataPos - (h->lineData.selectCaretPos));
                            memcpy( dataPtr, ((h->lineData.nowPtr)->data + (h->lineData.selectCaretPos)),size );
                            dataPtr += size;
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

    return rtn;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃f�[�^�T�C�Y�擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : EDITWND_BUFF_REGION region
 * �߂�l: DWORD
 ***************************************/
DWORD
EditWndBuffGetDataSize( H_EDITWND_BUFF hEditWndBuff, EDITWND_BUFF_REGION region )
{
    DWORD dataSize = 0;
    S_BUFF_LINE_DATA *nowPtr,*prevPtr;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( region == BUFF_ALL )
    {
        for( nowPtr = (h->lineData.topPtr); nowPtr != NULL; nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr )
        {
            dataSize += nowPtr->dataSize;
        }
    }
    else
    {
        if( (h->lineData.selectPtr) != NULL )
        { /* �I���J�n�ʒu�L�� */
            if( ((h->lineData.selectPtr)->lineNum) < ((h->lineData.nowPtr)->lineNum) )
            { /* �������ɑI�� */
                for( nowPtr = (h->lineData.selectPtr),prevPtr = NULL; prevPtr != (h->lineData.nowPtr); nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr )
                {
                    if( nowPtr == (h->lineData.selectPtr) )
                    {
                        dataSize += (nowPtr->dataSize - (h->lineData.selectCaretPos));
                    }
                    else if( nowPtr == (h->lineData.nowPtr) )
                    {
                        dataSize += nowPtr->caretDataPos;
                    }
                    else
                    {
                        dataSize += nowPtr->dataSize;
                    }
                    prevPtr = nowPtr;
                }
            }
            else if( ((h->lineData.selectPtr)->lineNum) > ((h->lineData.nowPtr)->lineNum) )
            { /* �������ɑI�� */
                for( nowPtr = (h->lineData.nowPtr),prevPtr = NULL; prevPtr != (h->lineData.selectPtr); nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr )
                {
                    if( nowPtr == (h->lineData.nowPtr) )
                    {
                        dataSize += (nowPtr->dataSize - nowPtr->caretDataPos);
                    }
                    else if( nowPtr == (h->lineData.selectPtr) )
                    {
                        dataSize += (h->lineData.selectCaretPos);
                    }
                    else
                    {
                        dataSize += nowPtr->dataSize;
                    }
                    prevPtr = nowPtr;
                }
            }
            else
            { /* ����s���őI�� */
                if( (h->lineData.nowPtr) == (h->lineData.selectPtr) )
                {
                    if( (h->lineData.nowPtr)->caretDataPos < (h->lineData.selectCaretPos) )
                    {
                        dataSize += ((h->lineData.selectCaretPos) - (h->lineData.nowPtr)->caretDataPos);
                    }
                    else if( (h->lineData.nowPtr)->caretDataPos > (h->lineData.selectCaretPos) )
                    {
                        dataSize += ((h->lineData.nowPtr)->caretDataPos - (h->lineData.selectCaretPos));
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
        }
        else
        {
            nop();
        }
    }

    return dataSize;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̍ő�s�T�C�Y�擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: DWORD
 ***************************************/
DWORD
EditWndBuffGetLineMaxSize( H_EDITWND_BUFF hEditWndBuff )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    return (h->lineData.endPtr)->lineNum;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̍ő啶���T�C�Y�擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: DWORD
 ***************************************/
DWORD
EditWndBuffGetColumnMaxSize( H_EDITWND_BUFF hEditWndBuff)
{
    DWORD columnMaxSize = 0;
    S_BUFF_LINE_DATA *nowPtr;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    for( nowPtr = (h->lineData.topPtr); nowPtr != NULL; nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr )
    {
        columnMaxSize = max( columnMaxSize, (nowPtr->dataSize - nowPtr->newLineCodeSize) );
    }

    return columnMaxSize;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃L�����b�gX�ʒu�擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: DWORD
 ***************************************/
DWORD
EditWndBuffGetCaretXpos( H_EDITWND_BUFF hEditWndBuff )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    return getCaretDispXpos(h,(h->lineData.nowPtr),(h->lineData.nowPtr)->caretDataPos);
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃L�����b�gY�ʒu�擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: DWORD
 ***************************************/
DWORD
EditWndBuffGetCaretYpos( H_EDITWND_BUFF hEditWndBuff )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    return (h->lineData.nowPtr)->lineNum;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃L�����b�g�ʒu�Z�b�g
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : DWORD xPos
 * ����  : DWORD lineNum
 * �߂�l: BOOL (TRUE:�L�����b�g�ʒu���ς����,FALSE:�ς��Ȃ�)
 ***************************************/
BOOL
EditWndBuffSetCaretPos( H_EDITWND_BUFF hEditWndBuff, DWORD xPos, DWORD lineNum )
{
    S_BUFF_LINE_DATA *nowPtr = NULL;
    S_BUFF_DISP_DATA dispData;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;
    BOOL bCaretPosChange = FALSE;

    nowPtr = (S_BUFF_LINE_DATA *)EditWndBuffGetLinePtr( hEditWndBuff, lineNum );

    if( nowPtr != NULL )
    {
        if( (h->lineData.nowPtr) != nowPtr )
        {
            (h->lineData.nowPtr) = nowPtr;
            bCaretPosChange = TRUE;
        }
        else
        {
            nop();
        }

        getDispCharData( h,(h->lineData.nowPtr), xPos, &dispData );

        if( (h->lineData.nowPtr)->caretDataPos != dispData.caretPos )
        {
            (h->lineData.nowPtr)->caretDataPos = dispData.caretPos;
            bCaretPosChange = TRUE;
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

    return bCaretPosChange;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃L�����b�gX�ʒu���Z
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: �Ȃ�
 ***************************************/
void
EditWndBuffIncCaretXpos( H_EDITWND_BUFF hEditWndBuff )
{
    int moveAmount = 0;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( (h->lineData.nowPtr) != NULL )
    {
        if( detectCharSet((h->lineData.nowPtr),(h->lineData.nowPtr)->caretDataPos) == DOUBLE_CHAR_HIGH )
        { /* ���̕����ňړ��ʂ𔻒f */
            moveAmount = 2;
        }
        else
        {
            moveAmount = 1;
        }

        if( ((h->lineData.nowPtr)->caretDataPos + moveAmount) > ((h->lineData.nowPtr)->dataSize - (h->lineData.nowPtr)->newLineCodeSize) )
        { /* �L�����b�g�͂��łɍs�̍ŉE�[ */
            if( ((h->lineData.nowPtr)->header.nextPtr != NULL) )
            { /* ���s�L�� */
                (h->lineData.nowPtr) = (S_BUFF_LINE_DATA *)(h->lineData.nowPtr)->header.nextPtr;
                (h->lineData.nowPtr)->caretDataPos = 0;
            }
            else
            { /* ���s���� */
                nop();
            }
        }
        else
        { /* �܂��E�ֈړ��\ */
            (h->lineData.nowPtr)->caretDataPos += moveAmount;
        }
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃L�����b�gX�ʒu���Z
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: �Ȃ�
 ***************************************/
void
EditWndBuffDecCaretXpos( H_EDITWND_BUFF hEditWndBuff )
{
    int moveAmount = 0;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( (h->lineData.nowPtr) != NULL )
    {
        if( ((h->lineData.nowPtr)->caretDataPos>0) &&
            (detectCharSet((h->lineData.nowPtr),(h->lineData.nowPtr)->caretDataPos-1) == DOUBLE_CHAR_LOW) )
        { /* �O�̕����ňړ��ʂ𔻒f */
            moveAmount = 2;
        }
        else
        {
            moveAmount = 1;
        }

        if( (h->lineData.nowPtr)->caretDataPos == 0 )
        { /* �L�����b�g�͂��łɍs�̍ō��[ */
            if( ((h->lineData.nowPtr)->header.prevPtr != NULL) )
            { /* �O�s�L�� */
                (h->lineData.nowPtr) = (S_BUFF_LINE_DATA *)(h->lineData.nowPtr)->header.prevPtr;
                (h->lineData.nowPtr)->caretDataPos = (h->lineData.nowPtr)->dataSize - (h->lineData.nowPtr)->newLineCodeSize;
            }
            else
            { /* �O�s���� */
                nop();
            }
        }
        else
        { /* �܂����ֈړ��\ */
            (h->lineData.nowPtr)->caretDataPos = max( (h->lineData.nowPtr)->caretDataPos - moveAmount, 0 );
        }
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃L�����b�gY�ʒu���Z
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: �Ȃ�
 ***************************************/
void
EditWndBuffIncCaretYpos( H_EDITWND_BUFF hEditWndBuff )
{
    DWORD preDispPos = 0;
    S_BUFF_LINE_DATA *nextPtr;
    S_BUFF_DISP_DATA dispData;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    nextPtr = (S_BUFF_LINE_DATA *)((h->lineData.nowPtr)->header.nextPtr);

    if( ((h->lineData.nowPtr) != NULL) && (nextPtr != NULL) )
    {
        preDispPos = getCaretDispXpos(h,(h->lineData.nowPtr),(h->lineData.nowPtr)->caretDataPos);
        getDispCharData( h, nextPtr, preDispPos, &dispData );
        nextPtr->caretDataPos = dispData.caretPos;
        (h->lineData.nowPtr) = nextPtr;
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃L�����b�gY�ʒu���Z
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: �Ȃ�
 ***************************************/
void
EditWndBuffDecCaretYpos( H_EDITWND_BUFF hEditWndBuff )
{
    DWORD preDispPos = 0;
    S_BUFF_LINE_DATA *prevPtr;
    S_BUFF_DISP_DATA dispData;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    prevPtr = (S_BUFF_LINE_DATA *)(h->lineData.nowPtr)->header.prevPtr;

    if( ((h->lineData.nowPtr) != NULL) && (prevPtr != NULL) )
    {
        preDispPos = getCaretDispXpos(h,(h->lineData.nowPtr),(h->lineData.nowPtr)->caretDataPos);
        getDispCharData( h, prevPtr, preDispPos, &dispData );
        prevPtr->caretDataPos = dispData.caretPos;
        (h->lineData.nowPtr) = prevPtr;
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * ���e  : �w��s�f�[�^�̃n���h���擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : DWORD lineNum
 * �߂�l: H_EDITWND_BUFF_LINE
 ***************************************/
H_EDITWND_BUFF_LINE
EditWndBuffGetLinePtr( H_EDITWND_BUFF hEditWndBuff, DWORD lineNum )
{
    S_BUFF_LINE_DATA *nowPtr,*nextPtr,*rtnPtr = NULL;
    DWORD i;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( (h->lineData.topPtr) == NULL )
    {
        nop();
    }
    else
    {
        for( i=0,nowPtr = nextPtr = (h->lineData.topPtr); (nowPtr != NULL) && (i<=lineNum); nowPtr=nextPtr,i++ )
        {
            nextPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr;

            if( i == lineNum )
            {
                rtnPtr = nowPtr;
                break;
            }
            else
            {
                nop();
            }
        }
    }

    return (H_EDITWND_BUFF_LINE)rtnPtr;
}

/********************************************************************************
 * ���e  : ���̍s�f�[�^�̃n���h���擾
 * ����  : H_EDITWND_BUFF_LINE hLineData
 * �߂�l: H_EDITWND_BUFF_LINE
 ***************************************/
H_EDITWND_BUFF_LINE
EditWndBuffGetLineNextPtr( H_EDITWND_BUFF_LINE hLineData )
{
    return (H_EDITWND_BUFF_LINE)(((S_BUFF_LINE_DATA *)hLineData)->header.nextPtr);
}

/********************************************************************************
 * ���e  : �w��s�A�w���̃f�[�^���擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : H_EDITWND_BUFF_LINE hLineData
 * ����  : DWORD             dispPos     �\���ʒu
 * ����  : S_BUFF_DISP_DATA *dataPtr
 * �߂�l: BOOL
 ***************************************/
BOOL
EditWndBuffGetDispData( H_EDITWND_BUFF hEditWndBuff, H_EDITWND_BUFF_LINE hLineData, DWORD dispPos, S_BUFF_DISP_DATA *dataPtr )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    getDispCharData( h, (S_BUFF_LINE_DATA *)hLineData, dispPos, dataPtr );

    return TRUE;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̉��s�R�[�h�Z�b�g
 * ����  : UINT newLineType
 * �߂�l: �Ȃ�
 ***************************************/
void
EditWndBuffSetNewLineCode( H_EDITWND_BUFF hEditWndBuff, UINT newLineType )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    h->NewLineType = newLineType;
}

/********************************************************************************
 * ���e  : �f�[�^�폜
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : BOOL bBackSpace
 * �߂�l: �Ȃ�
 ***************************************/
void
EditWndBuffRemoveData( H_EDITWND_BUFF hEditWndBuff, BOOL bBackSpace )
{
    DWORD removeSize = 0,saveCaretPos;
    S_BUFF_LINE_DATA *newPtr = NULL,*prevPtr = NULL,*nextPtr = NULL,*nowPtr = NULL,*savePtr;
    S_BUFF_LINE_DATA *dividePrePtr,*dividePostPtr;
    BOOL bNormalDelete = FALSE;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    EDITWNDBUFF_DEBUG_PRINTF("EditWndBuffRemoveData,");

    if( (h->lineData.nowPtr) != NULL )
    {
        if( (h->lineData.selectPtr) != NULL )
        { /* �I���J�n�ʒu�L�� */
            if( ((h->lineData.nowPtr) == (h->lineData.selectPtr)) &&
                (((h->lineData.nowPtr)->lineNum) == ((h->lineData.selectPtr)->lineNum)) )
            { /* �I��͈͓͂���s�� */
                if( (h->lineData.nowPtr)->caretDataPos < (h->lineData.selectCaretPos) )
                { /* �L�����b�g�ʒu���I���J�n�ʒu */
                    removeSize += ((h->lineData.selectCaretPos) - (h->lineData.nowPtr)->caretDataPos);
                    saveCaretPos = (h->lineData.nowPtr)->caretDataPos;
                    (h->lineData.nowPtr)->caretDataPos = (h->lineData.selectCaretPos);
                    (h->lineData.selectCaretPos) = saveCaretPos;
                }
                else
                { /* �ʏ�(�I���J�n�ʒu���L�����b�g�ʒu) */
                    removeSize += ((h->lineData.nowPtr)->caretDataPos - (h->lineData.selectCaretPos));
                }

                if( removeSize )
                {
                    EDITWNDBUFF_DEBUG_PRINTF("0x%02X,%d\r\n",(BYTE)(h->lineData.nowPtr)->data[h->lineData.selectCaretPos],removeSize);

                    BuffLineDataDivide( (h->lineData.nowPtr), &dividePrePtr, &dividePostPtr ); /* �L�����b�g�ʒu�ŕ��� */
                    newPtr = BuffLineDataShorten( dividePrePtr, removeSize );                 /* ������̑O���f�[�^�̖������珊��ʍ폜 */
                    BuffLineDataDestroy( dividePrePtr );                                 /* �O���f�[�^�� */
                    dividePrePtr = newPtr;                                            /* ��������     */

                    newPtr = BuffLineDataJoin( dividePrePtr, dividePostPtr );                 /* �Č��� */
                    newPtr->lineNum = (h->lineData.nowPtr)->lineNum;                   /* �s�ԍ��͓��� */
                    newPtr->caretDataPos = (h->lineData.nowPtr)->caretDataPos - removeSize;    /* �L�����b�g�ʒu�͍폜�ʕ��O���ɂȂ� */
                    BuffLineDataDestroy( dividePrePtr );                                 /* ��ƃf�[�^���폜 */
                    BuffLineDataDestroy( dividePostPtr );                                /* ��ƃf�[�^���폜 */

                    /* �Â��s�f�[�^�ƒu�������� */
                    EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),newPtr );
                    BuffLineDataDestroy( (h->lineData.nowPtr) );
                    (h->lineData.nowPtr) = newPtr;
                }
                else
                {
                    nop();
                }
            }
            else
            { /* �I��͈͂������s�ɂ܂������Ă��� */
                if( ((h->lineData.selectPtr)->lineNum) < ((h->lineData.nowPtr)->lineNum) )
                { /* �������ɑI�� */
                    nop();
                }
                else
                { /* �������ɑI�� */
                    savePtr = (h->lineData.selectPtr);
                    saveCaretPos = (h->lineData.selectCaretPos);
                    (h->lineData.selectPtr) = (h->lineData.nowPtr);
                    (h->lineData.selectCaretPos) = (h->lineData.nowPtr)->caretDataPos;
                    (h->lineData.nowPtr) = savePtr;
                    (h->lineData.nowPtr)->caretDataPos = saveCaretPos;
                }

                /* �擪����Ō�܂őI�����ꂽ�s���폜 */
                for( nowPtr = (S_BUFF_LINE_DATA *)(h->lineData.selectPtr)->header.nextPtr; nowPtr != (h->lineData.nowPtr); nowPtr = nextPtr )
                {
                    nextPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr;
                    EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),nowPtr);
                    BuffLineDataDestroy( nowPtr );
                }
                /* �擪�s�̂����A�I���ʒu����O���c�� */
                if( (h->lineData.selectCaretPos) == 0 )
                { /* �I���ʒu���擪��������A1�s�ہX�폜 */
                    EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.selectPtr));
                    BuffLineDataDestroy( (h->lineData.selectPtr) );
                }
                else
                {
                    (h->lineData.selectPtr)->caretDataPos = (h->lineData.selectCaretPos);
                    BuffLineDataDivide( (h->lineData.selectPtr), &dividePrePtr, &dividePostPtr ); /* �L�����b�g�ʒu�ŕ��� */
                    EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.selectPtr),dividePrePtr );
                    BuffLineDataDestroy( (h->lineData.selectPtr) );
                }

                /* �ŏI�s�̂����A�I���ʒu�������c�� */
                BuffLineDataDivide( (h->lineData.nowPtr), &dividePrePtr, &dividePostPtr ); /* �L�����b�g�ʒu�ŕ��� */
                EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),dividePostPtr );
                BuffLineDataDestroy( (h->lineData.nowPtr) );
                (h->lineData.nowPtr) = dividePostPtr;
                (h->lineData.nowPtr)->caretDataPos = 0;
                BuffLineDataUpdateLineNumAfter( (h->lineData.nowPtr) );
            }

            (h->lineData.selectPtr) = NULL;
            (h->lineData.selectCaretPos) = 0;
        }
        else
        { /* �I�𖳂� */
            if( bBackSpace )
            { /* BS�L�[ */
                if( (h->lineData.nowPtr)->caretDataPos > 0  )
                {
                    bNormalDelete = TRUE;

                    if( detectCharSet((h->lineData.nowPtr),(h->lineData.nowPtr)->caretDataPos-1) == DOUBLE_CHAR_LOW )
                    { /* ���̕����ō폜�ʂ𔻒f */
                        removeSize = 2;
                    }
                    else
                    {
                        removeSize = 1;
                    }
                }
                else
                { /* �L�����b�g���s�̐擪�ʒu�B�܂�A�O�s�Ƃ̌����B*/
                    if( (h->lineData.nowPtr)->header.prevPtr != NULL )
                    {
                        prevPtr = (S_BUFF_LINE_DATA *)(h->lineData.nowPtr)->header.prevPtr;
                        newPtr = BuffLineDataJoin( prevPtr,(h->lineData.nowPtr) ); /* �O�s�Ɩ{�s�����������V�����s�f�[�^�𐶐� */

                        if( newPtr != NULL )
                        {
                            /* �s�ԍ��A�L�����b�g�ʒu��O�s�f�[�^�ɂ���Č��܂� */
                            newPtr->lineNum  = prevPtr->lineNum;
                            newPtr->caretDataPos = prevPtr->dataSize - prevPtr->newLineCodeSize;

                            EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr) ); /* �{�s�͍폜 */
                            EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),prevPtr,newPtr );    /* �O�s�͐V�����s�f�[�^�ɒu�������� */

                            BuffLineDataDestroy( (h->lineData.nowPtr) );
                            BuffLineDataDestroy( prevPtr );
                            (h->lineData.nowPtr) = newPtr;

                            BuffLineDataUpdateLineNumAfter( (h->lineData.nowPtr) );
                        }
                        else
                        {
                            EDITWNDBUFF_DEBUG_PRINTF("Bug!!!\r\n");
                        }
                    }
                    else
                    {
                        EDITWNDBUFF_DEBUG_PRINTF("No Action(Position 0,0)\r\n");
                    }
                }
            }
            else
            { /* DEL�L�[ */
                if( (h->lineData.nowPtr)->caretDataPos != ((h->lineData.nowPtr)->dataSize - (h->lineData.nowPtr)->newLineCodeSize) )
                { /* �L�����b�g���s�̍ŏI�ʒu�łȂ� */
                    bNormalDelete = TRUE;

                    if( detectCharSet((h->lineData.nowPtr),(h->lineData.nowPtr)->caretDataPos) == DOUBLE_CHAR_HIGH )
                    { /* �L�����b�g�ʒu�̕����ō폜�ʂ𔻒f */
                        removeSize = 2;
                    }
                    else
                    {
                        removeSize = 1;
                    }
                    (h->lineData.nowPtr)->caretDataPos += removeSize;
                }
                else
                { /* �L�����b�g���s�̍ŏI�ʒu�B�܂�A���s�Ƃ̌����B*/
                    if( (h->lineData.nowPtr)->header.nextPtr != NULL )
                    { /* ���s�L�� */
                        nextPtr = (S_BUFF_LINE_DATA *)(h->lineData.nowPtr)->header.nextPtr;
                        newPtr = BuffLineDataJoin( (h->lineData.nowPtr),nextPtr ); /* �{�s�Ǝ��s�����������V�����s�f�[�^�𐶐� */

                        if( newPtr != NULL )
                        {
                            /* �s�ԍ��A�L�����b�g�ʒu�͖{�s�f�[�^�ɂ���Č��܂� */
                            newPtr->lineNum  = (h->lineData.nowPtr)->lineNum;
                            newPtr->caretDataPos = (h->lineData.nowPtr)->caretDataPos;

                            EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),nextPtr );         /* ���s�͍폜 */
                            EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),newPtr ); /* �{�s�͐V�����s�f�[�^�ɒu�������� */

                            BuffLineDataDestroy( (h->lineData.nowPtr) );
                            BuffLineDataDestroy( nextPtr );
                            (h->lineData.nowPtr) = newPtr;

                            BuffLineDataUpdateLineNumAfter( (h->lineData.nowPtr) );
                        }
                        else
                        {
                            nop();
                        }
                    }
                    else
                    { /* ���s���� */
                        EDITWNDBUFF_DEBUG_PRINTF("No Action(DEL at LastPos)\r\n");
                    }
                }
            }

            if( bNormalDelete )
            { /* �폜�L�� */
                EDITWNDBUFF_DEBUG_PRINTF("0x%02X,%d\r\n",(BYTE)(h->lineData.nowPtr)->data[(h->lineData.nowPtr)->caretDataPos-removeSize],removeSize);

                BuffLineDataDivide( (h->lineData.nowPtr), &dividePrePtr, &dividePostPtr ); /* �L�����b�g�ʒu�ŕ��� */
                newPtr = BuffLineDataShorten( dividePrePtr, removeSize );                 /* ������̑O���f�[�^�̖������珊��ʍ폜 */
                BuffLineDataDestroy( dividePrePtr );                                 /* �O���f�[�^�� */
                dividePrePtr = newPtr;                                            /* ��������     */

                newPtr = BuffLineDataJoin( dividePrePtr, dividePostPtr );                 /* �Č��� */
                newPtr->lineNum = (h->lineData.nowPtr)->lineNum;                   /* �s�ԍ��͓��� */
                newPtr->caretDataPos = (h->lineData.nowPtr)->caretDataPos - removeSize;    /* �L�����b�g�ʒu�͍폜�ʕ��O���ɂȂ� */
                BuffLineDataDestroy( dividePrePtr );                                 /* ��ƃf�[�^���폜 */
                BuffLineDataDestroy( dividePostPtr );                                /* ��ƃf�[�^���폜 */

                /* �Â��s�f�[�^�ƒu�������� */
                EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),newPtr );
                BuffLineDataDestroy( (h->lineData.nowPtr) );
                (h->lineData.nowPtr) = newPtr;
            }
            else
            {
                nop();
            }
        }
    }
    else
    {
        EDITWNDBUFF_DEBUG_PRINTF("No Action(Empty)\r\n");
    }
}

/********************************************************************************
 * ���e  : ���s�f�[�^�擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : PTSTR dataPtr
 * �߂�l: INT
 ***************************************/
INT
EditWndBuffGetNewLineData( H_EDITWND_BUFF hEditWndBuff, PTSTR dataPtr )
{
    INT size = 0;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    switch( h->NewLineType )
    {
    case EDITWND_BUFF_NEWLINE_CRLF:
        *dataPtr     = '\r';
        *(dataPtr+1) = '\n';
        size = 2;
        break;
    case EDITWND_BUFF_NEWLINE_LF  :
        *dataPtr = '\n';
        size = 1;
        break;
    case EDITWND_BUFF_NEWLINE_CR  :
        *dataPtr = '\r';
        size = 1;
        break;
    case EDITWND_BUFF_NEWLINE_NONE:
    default:
        break;
    }

    return size;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃^�u�T�C�Y�Z�b�g
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : INT tabSize
 * �߂�l: BOOL (TRUE:�ύX���ꂽ)
 ***************************************/
BOOL
EditWndBuffSetTabSize( H_EDITWND_BUFF hEditWndBuff, INT tabSize )
{
    BOOL bRtn = FALSE;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( tabSize != h->tabSize )
    {
        h->tabSize = tabSize;
        bRtn = TRUE;
    }
    else
    {
        nop();
    }

    return bRtn;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�͈̔͑I��ON
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: �Ȃ�
 ***************************************/
void
EditWndBuffSelectOn( H_EDITWND_BUFF hEditWndBuff )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( (h->lineData.selectPtr) )
    { /* ���ɑI���ς݂̏ꍇ */
        nop(); /* �ύX���Ȃ� */
    }
    else
    {
        (h->lineData.selectPtr) = (h->lineData.nowPtr);
        (h->lineData.selectCaretPos) = (h->lineData.nowPtr)->caretDataPos;
    }
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�͈̔͑I��OFF
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: BOOL (TRUE: �I��͈͂��������ꂽ,FALSE: �I��͈͖���)
 ***************************************/
BOOL
EditWndBuffSelectOff( H_EDITWND_BUFF hEditWndBuff )
{
    BOOL bRtn = FALSE;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( !(h->lineData.selectPtr) && !(h->lineData.selectCaretPos) )
    { /* �I������Ă��Ȃ����� */
        nop();
    }
    else if( (h->lineData.selectPtr == h->lineData.nowPtr) &&
        (h->lineData.selectCaretPos == (h->lineData.nowPtr)->caretDataPos) )
    { /* �I������Ă��Ȃ����� */
        nop();
    }
    else
    {
        bRtn = TRUE;
    }

    (h->lineData.selectPtr) = NULL;
    (h->lineData.selectCaretPos) = 0;

    return bRtn;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̑S�͈͑I��
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: BOOL
 ***************************************/
BOOL
EditWndBuffSelectAll( H_EDITWND_BUFF hEditWndBuff )
{
    BOOL bRtn = FALSE;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( (h->lineData.topPtr) != NULL )
    {
        if( ((h->lineData.selectPtr) == (h->lineData.topPtr)) &&
            ((h->lineData.selectCaretPos) == 0) &&
            ((h->lineData.nowPtr) == (h->lineData.endPtr)) &&
            ((h->lineData.nowPtr)->caretDataPos == ((h->lineData.nowPtr)->dataSize - (h->lineData.nowPtr)->newLineCodeSize) ) )
        {
            nop();
        }
        else
        {
            (h->lineData.selectPtr) = (h->lineData.topPtr);
            (h->lineData.selectCaretPos) = 0;
            (h->lineData.nowPtr) = (h->lineData.endPtr);
            (h->lineData.nowPtr)->caretDataPos = (h->lineData.nowPtr)->dataSize - (h->lineData.nowPtr)->newLineCodeSize;
            bRtn = TRUE;
        }
    }
    else
    {
        nop();
    }

    return bRtn;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̌����f�[�^�Z�b�g
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : TCHAR* dataPtr
 * ����  : DWORD  length
 * ����  : BOOL bDirectionUp
 * �߂�l: BOOL
 ***************************************/
BOOL
EditWndBuffFindDataSet( H_EDITWND_BUFF hEditWndBuff, TCHAR* dataPtr, DWORD length, BOOL bDirectionUp )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;
    BOOL rtn = (BOOL)FALSE;

    if( (dataPtr != NULL) && (length > 0) )
    { /* �f�[�^�L��̏ꍇ */
        S_BUFF_LINE_DATA *nowPtr;
        DWORD nowDataPos;
        DWORD findDataLen;

        findDataLen = min(length,strlen(dataPtr));
        EDITWNDBUFF_DEBUG_PRINTF("EditWndBuffFindDataSet,Target:%s,%d,%d\r\n",dataPtr,findDataLen,bDirectionUp);

        if( !bDirectionUp )
        { /* ���� */
            for( nowPtr = (h->lineData.nowPtr),nowDataPos=(h->lineData.nowPtr)->caretDataPos; nowPtr != NULL; nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr,nowDataPos=0 )
            {
                TCHAR *ptr;
#if 0
                EDITWNDBUFF_DEBUG_PRINTF("EditWndBuffFindDataSet:%s\r\n",nowPtr->data);
#endif
                ptr = (TCHAR *)strstr(nowPtr->data+nowDataPos,dataPtr);

                if( ptr )
                { /* �������� */
                    h->lineData.nowPtr = nowPtr;
                    (h->lineData.nowPtr)->caretDataPos = (DWORD)(ptr - nowPtr->data) + findDataLen;
                    h->lineData.selectPtr = nowPtr;
                    h->lineData.selectCaretPos = (DWORD)(ptr - nowPtr->data);
                    EDITWNDBUFF_DEBUG_PRINTF("EditWndBuffFindDataSet,Found:%s\r\n",(TCHAR *)ptr);
                    rtn = (BOOL)TRUE;
                    break;
                }
            }
        }
        else
        { /* ��� */
            for( nowPtr = (h->lineData.nowPtr),nowDataPos=(h->lineData.nowPtr)->caretDataPos; nowPtr != NULL; nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.prevPtr,nowDataPos=0 )
            {
                TCHAR *ptr;
#if 0
                EDITWNDBUFF_DEBUG_PRINTF("EditWndBuffFindDataSet:%s\r\n",nowPtr->data);
#endif
                ptr = (TCHAR *)strstr(nowPtr->data,dataPtr); /* �b��A�e�s�A�t����T�[�`���Ȃ���΂Ȃ�Ȃ� */

                if( ptr )
                { /* �������� */
                    h->lineData.nowPtr = nowPtr;
                    (h->lineData.nowPtr)->caretDataPos = (DWORD)(ptr - nowPtr->data) + findDataLen;
                    h->lineData.selectPtr = nowPtr;
                    h->lineData.selectCaretPos = (DWORD)(ptr - nowPtr->data);
                    EDITWNDBUFF_DEBUG_PRINTF("EditWndBuffFindDataSet,Found:%s\r\n",(TCHAR *)ptr);
                    rtn = (BOOL)TRUE;
                    break;
                }
            }
        }
    }
    else
    { /* �f�[�^�����̏ꍇ */
        nop();
    }

    return rtn;
}

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@��Undo
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: BOOL
 ***************************************/
BOOL
EditWndBuffUndo( H_EDITWND_BUFF hEditWndBuff )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;
    BOOL rtn = (BOOL)FALSE;

    EDITWNDBUFF_DEBUG_PRINTF("EditWndBuffUndo,");
//    setSelectPosNowPosToFar(h,TRUE,3);
    setSelectPosNowPosToFar(h,FALSE,3);
    rtn = (BOOL)TRUE;

    return (BOOL)rtn;
}

/********************************************************************************
 * ���e  : �o�b�t�@�s�f�[�^���擾
 * ����  : H_EDITWND_BUFF_LOCAL h
 * ����  : TCHAR *dataPtr  �f�[�^�̐擪
 * ����  : DWORD maxLength �ő咷��
 * �߂�l: S_BUFF_LINE_DATA *
 ***************************************/
static S_BUFF_LINE_DATA *
getBuffLine( H_EDITWND_BUFF_LOCAL h, TCHAR *dataPtr, DWORD maxLength )
{
    S_BUFF_LINE_DATA *lineDataPtr = NULL;
    DWORD i;
    INT   newLineCodeSize = 0;

    for( i=0; i<maxLength; i++ )
    {
        if( h->NewLineType == EDITWND_BUFF_NEWLINE_CRLF )
        {
            if( (i>0) &&
                (*(dataPtr+i-1) == '\r') &&
                (*(dataPtr+i)   == '\n') )
            {
                i++;
                newLineCodeSize = 2;
                break;
            }
            else
            {
                nop();
            }
        }
        else if( h->NewLineType == EDITWND_BUFF_NEWLINE_LF )
        {
            if( (*(dataPtr+i) == '\n') )
            {
                i++;
                newLineCodeSize = 1;
                break;
            }
            else
            {
                nop();
            }
        }
        else if( h->NewLineType == EDITWND_BUFF_NEWLINE_CR )
        {
            if( (*(dataPtr+i) == '\r') )
            {
                i++;
                newLineCodeSize = 1;
                break;
            }
            else
            {
                nop();
            }
        }
        else
        { /* EDITWND_BUFF_NEWLINE_NONE */
            nop();
        }
    }

    lineDataPtr = BuffLineDataCreate( i, newLineCodeSize, dataPtr, 0, 0 );

    return lineDataPtr;
}

/********************************************************************************
 * ���e  : �L�����N�^�Z�b�g�̔��f(Shift_JIS�̏ꍇ)
 * ����  : S_BUFF_LINE_DATA *dataPtr
 * ����  : DWORD            offset   ���f�����������̐擪����̃I�t�Z�b�g(0 origin)
 * �߂�l: CHARSET_TYPE
 ***************************************/
static CHARSET_TYPE
detectCharSet( S_BUFF_LINE_DATA *dataPtr, DWORD offset )
{
    DWORD i;
    int charType = SINGLE_CHAR;

    if( dataPtr != NULL )
    {
        for( i=0; i<(dataPtr->dataSize-dataPtr->newLineCodeSize); i++ )
        {
            if( charType == DOUBLE_CHAR_HIGH )
            {
                charType = DOUBLE_CHAR_LOW;
            }
            else
            {
                if( ( (BYTE)(*(dataPtr->data+i)) <= (BYTE)0x80) || (((BYTE)0xA0 <= (BYTE)(*(dataPtr->data+i))) && ((BYTE)(*(dataPtr->data+i)) <= (BYTE)0xDF)) )
                {
                    charType = SINGLE_CHAR;
                }
                else
                {
                    charType = DOUBLE_CHAR_HIGH;
                }
            }

            if( i==offset )
            {
                break;
            }
            else
            {
                nop();
            }
        }

        if( i==(dataPtr->dataSize-dataPtr->newLineCodeSize) )
        {
            charType = SINGLE_CHAR;
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

    return charType;
}

/********************************************************************************
 * ���e  : �L�����b�gX�\���ʒu�擾
 * ����  : H_EDITWND_BUFF_LOCAL h
 * ����  : S_BUFF_LINE_DATA *linePtr   �s�f�[�^
 * ����  : DWORD             dataPos   ��ʒu(�f�[�^���)
 * �߂�l: DWORD
 ***************************************/
static DWORD
getCaretDispXpos( H_EDITWND_BUFF_LOCAL h, S_BUFF_LINE_DATA *linePtr, DWORD dataPos )
{
    DWORD i,j=0;
    DWORD literalMaxSize;
    int tab_offset;
    CHARSET_TYPE charType;

    if( linePtr != NULL )
    {
        charType = SINGLE_CHAR;
        literalMaxSize = (linePtr->dataSize-linePtr->newLineCodeSize);

        for( i=0,j=0; (i<dataPos)&&(i<literalMaxSize); i++ )
        { /* 1�s���̃f�[�^��1���������� */
            if( charType == DOUBLE_CHAR_HIGH )
            { /* �O������2byte�����̏��byte�������� */
                charType = DOUBLE_CHAR_LOW;
                j++;
            }
            else
            { /* �O������2byte�����̏��byte�ȊO */
                if( (*(linePtr->data+i)) == '\0' )
                {
                    charType = SINGLE_CHAR;
                    j++;
                }
                else if( (*(linePtr->data+i)) == '\t' )
                { /* �������̕�����TAB */
                    charType = TAB_CHAR;

                    tab_offset = h->tabSize - (j % h->tabSize);
                    j+=tab_offset;
                }
                else if( ( (BYTE)(*(linePtr->data+i)) <= (BYTE)0x80) ||
                         (((BYTE)0xA0 <= (BYTE)(*(linePtr->data+i))) && ((BYTE)(*(linePtr->data+i)) <= (BYTE)0xDF)) ||
                         ((BYTE)0xF0 <= (BYTE)(*(linePtr->data+i))) )
                { /* �������̕�����1byte���� */
                    charType = SINGLE_CHAR;
                    j++;
                }
                else
                { /* �������̕�����2byte�����̏��byte */
                    charType = DOUBLE_CHAR_HIGH;
                    j++;
                }
            }
        }
    }
    else
    {
        nop();
    }

    return j;
}

/********************************************************************************
 * ���e  : �w��ʒu�̕\���f�[�^���擾����
 * ����  : H_EDITWND_BUFF_LOCAL h
 * ����  : S_BUFF_LINE_DATA *linePtr   �s�f�[�^
 * ����  : DWORD             dispPos   ��ʒu(�\�����)
 * ����  : S_BUFF_DISP_DATA *dataPtr
 * �߂�l: BOOL
 ***************************************/
static BOOL
getDispCharData( H_EDITWND_BUFF_LOCAL h, S_BUFF_LINE_DATA *linePtr, DWORD dispPos, S_BUFF_DISP_DATA *dataPtr )
{
    DWORD dataPos,j,k;
    DWORD literalMaxSize;
    int tab_offset;

    if( (linePtr != NULL) && (dataPtr != NULL) )
    {
        dataPtr->size    = 0;
        dataPtr->type    = SINGLE_CHAR;
        dataPtr->bSelect = FALSE;
        dataPtr->dataPos = 0;
        dataPtr->caretPos= 0;

        literalMaxSize = (linePtr->dataSize-linePtr->newLineCodeSize);

        for( dataPos=0,j=0; dataPos<literalMaxSize; dataPos++ )
        { /* 1�s���̃f�[�^��1���������� */
            if( dataPtr->type == DOUBLE_CHAR_HIGH )
            { /* �O������2byte�����̏��byte�������� */
                dataPtr->type = DOUBLE_CHAR_LOW;
                *(dataPtr->data+1) = *(linePtr->data+dataPos);
            }
            else
            { /* �O������2byte�����̏��byte�ȊO */
                if( (*(linePtr->data+dataPos)) == '\0' )
                {
                    dataPtr->type = SINGLE_CHAR;
                    *(dataPtr->data) = ' ';
                }
                else if( (*(linePtr->data+dataPos)) == '\t' )
                { /* �������̕�����TAB */
                    dataPtr->type = TAB_CHAR;

                    tab_offset = h->tabSize - (j % h->tabSize);

                    if( dispPos < (j+tab_offset) )
                    {
                        for(k=0;k<tab_offset;k++)
                        {
                            *(dataPtr->data+k) = ' ';
                        }
                        dataPtr->size = tab_offset;
                        dataPtr->offset = dispPos - j;
                        break;
                    }
                    else
                    {
                        j+=tab_offset;
                        continue;
                    }
                }
                else if( ( (BYTE)(*(linePtr->data+dataPos)) <= (BYTE)0x80) ||
                         (((BYTE)0xA0 <= (BYTE)(*(linePtr->data+dataPos))) && ((BYTE)(*(linePtr->data+dataPos)) <= (BYTE)0xDF)) ||
                         ((BYTE)0xF0 <= (BYTE)(*(linePtr->data+dataPos))) )
                { /* �������̕�����1byte���� */
                    dataPtr->type = SINGLE_CHAR;
                    *(dataPtr->data) = *(linePtr->data+dataPos);
                }
                else
                { /* �������̕�����2byte�����̏��byte */
                    dataPtr->type = DOUBLE_CHAR_HIGH;
                    *(dataPtr->data)   = *(linePtr->data+dataPos);
                    *(dataPtr->data+1) = *(linePtr->data+dataPos+1);
                }
            }

            if( j == dispPos )
            { /* �������̕������w�肳�ꂽ�\���ʒu */
                break;
            }
            else
            {
                nop();
            }

            j++;
        }

        if( dataPos == literalMaxSize )
        {
            dataPtr->type = END_CHAR;
        }
        else
        {
            nop();
        }

        dataPtr->dataPos = dataPos;

        if( dataPos >= literalMaxSize )
        {
            dataPtr->dataPos = literalMaxSize;
        }
        else
        {
            switch( dataPtr->type )
            {
            case SINGLE_CHAR:
                dataPtr->size = 1;
                dataPtr->offset = 0;
                break;
            case DOUBLE_CHAR_HIGH:
                dataPtr->size = 2;
                dataPtr->offset = 0;
                break;
            case DOUBLE_CHAR_LOW:
                dataPtr->size = 2;
                dataPtr->offset = 1;
                break;
            default:
                break;
            }

            if( (h->lineData.selectPtr) != NULL )
            { /* �I���J�n�ʒu�L�� */
                if( ((h->lineData.selectPtr)->lineNum) < ((h->lineData.nowPtr)->lineNum) )
                { /* �������ɑI�� */
                    if( ((h->lineData.selectPtr)->lineNum < linePtr->lineNum) && (linePtr->lineNum < (h->lineData.nowPtr)->lineNum) )
                    {
                        dataPtr->bSelect = TRUE;
                    }
                    else if( ((h->lineData.selectPtr)->lineNum == linePtr->lineNum) && ((h->lineData.selectCaretPos) <= dataPos) )
                    {
                        dataPtr->bSelect = TRUE;
                    }
                    else if( ((h->lineData.nowPtr)->lineNum == linePtr->lineNum) && (dataPos < linePtr->caretDataPos) )
                    {
                        dataPtr->bSelect = TRUE;
                    }
                    else
                    {
                        nop();
                    }
                }
                else if( ((h->lineData.selectPtr)->lineNum) > ((h->lineData.nowPtr)->lineNum) )
                { /* �������ɑI�� */
                    if( ((h->lineData.nowPtr)->lineNum < linePtr->lineNum) && (linePtr->lineNum < (h->lineData.selectPtr)->lineNum) )
                    {
                        dataPtr->bSelect = TRUE;
                    }
                    else if( ((h->lineData.nowPtr)->lineNum == linePtr->lineNum) && (linePtr->caretDataPos <= dataPos) )
                    {
                        dataPtr->bSelect = TRUE;
                    }
                    else if( ((h->lineData.selectPtr)->lineNum == linePtr->lineNum) && (dataPos < (h->lineData.selectCaretPos)) )
                    {
                        dataPtr->bSelect = TRUE;
                    }
                    else
                    {
                        nop();
                    }
                }
                else
                { /* ����s���őI�� */
                    if( (h->lineData.nowPtr) == linePtr )
                    {
                        if( linePtr->caretDataPos < (h->lineData.selectCaretPos) )
                        {
                            if( (linePtr->caretDataPos <= dataPos) && (dataPos < (h->lineData.selectCaretPos)) )
                            {
                                dataPtr->bSelect = TRUE;
                            }
                            else
                            {
                                nop();
                            }
                        }
                        else if( linePtr->caretDataPos > (h->lineData.selectCaretPos) )
                        {
                            if( ((h->lineData.selectCaretPos) <= dataPos) && (dataPos < linePtr->caretDataPos) )
                            {
                                dataPtr->bSelect = TRUE;
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
                    else
                    {
                        nop();
                    }
                }
            }
            else
            {
                nop();
            }
        }

        if( dataPtr->type == DOUBLE_CHAR_LOW )
        {
            dataPtr->caretPos = dataPtr->dataPos + 1;
        }
        else if( (dataPtr->type == TAB_CHAR) && (dataPtr->offset != 0)  )
        {
            dataPtr->caretPos = dataPtr->dataPos + 1;
        }
        else
        {
            dataPtr->caretPos = dataPtr->dataPos;
        }
    }
    else
    {
        nop();
    }

    return TRUE;
}

/********************************************************************************
 * ���e  : ���݂̃L�����b�g�̈ʒu���珊��ʈړ������ʒu��I������
 * ����  : H_EDITWND_BUFF_LOCAL h
 * ����  : BOOL                 bMinus
 * ����  : DWORD                offset
 * �߂�l: �Ȃ�
 ***************************************/
static void
setSelectPosNowPosToFar( H_EDITWND_BUFF_LOCAL h, BOOL bMinus, DWORD offset )
{
    S_BUFF_LINE_DATA *nowPtr;
    DWORD nowOffset;

    if( (h->lineData.nowPtr) != NULL )
    {
        nowOffset = offset;

        if( bMinus )
        { /* ����� */
            if( nowOffset <= (h->lineData.nowPtr)->caretDataPos )
            { /* ����s�Ɋ܂܂��*/
                (h->lineData.selectPtr) = (h->lineData.nowPtr);
                h->lineData.selectCaretPos = (h->lineData.nowPtr)->caretDataPos - nowOffset;
            }
            else
            { /* ����s�Ɋ܂܂�Ȃ� */
                nowOffset -= (h->lineData.nowPtr)->caretDataPos;

                for( nowPtr = (h->lineData.nowPtr)->header.prevPtr; nowPtr != NULL; nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.prevPtr )
                {
                    if( nowOffset <= (nowPtr->dataSize-nowPtr->newLineCodeSize) )
                    { /* ���̍s�Ɋ܂܂��*/
                        (h->lineData.selectPtr) = nowPtr;
                        h->lineData.selectCaretPos = (nowPtr->dataSize-nowPtr->newLineCodeSize) - nowOffset;
                        break;
                    }
                    else
                    {
                        nowOffset -= (nowPtr->dataSize-nowPtr->newLineCodeSize);
                    }
                }
            }
        }
        else
        { /* ������ */
            if( (h->lineData.nowPtr)->caretDataPos + nowOffset <= ((h->lineData.nowPtr)->dataSize-(h->lineData.nowPtr)->newLineCodeSize)  )
            { /* ����s�Ɋ܂܂��*/
                (h->lineData.selectPtr) = (h->lineData.nowPtr);
                h->lineData.selectCaretPos = (h->lineData.nowPtr)->caretDataPos + nowOffset;
            }
            else
            { /* ����s�Ɋ܂܂�Ȃ� */
                nowOffset -= ((h->lineData.nowPtr)->dataSize-(h->lineData.nowPtr)->newLineCodeSize) - (h->lineData.nowPtr)->caretDataPos;

                for( nowPtr = (h->lineData.nowPtr)->header.nextPtr; nowPtr != NULL; nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr )
                {
                    if( nowOffset <= (nowPtr->dataSize-nowPtr->newLineCodeSize) )
                    { /* ���̍s�Ɋ܂܂��*/
                        (h->lineData.selectPtr) = nowPtr;
                        h->lineData.selectCaretPos = nowOffset;
                        break;
                    }
                    else
                    {
                        nowOffset -= (nowPtr->dataSize-nowPtr->newLineCodeSize);
                    }
                }
            }
        }
    }
    else
    {
        nop(); /* �ُ� */
    }
}
