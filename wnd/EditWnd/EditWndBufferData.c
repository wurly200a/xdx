/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"

/* �ʃC���N���[�h�t�@�C�� */
#include "LinkedList.h"
#include "EditWndBufferDataType.h"

/* �O���֐���` */

/* �O���ϐ���` */

/* �����֐���` */

/********************************************************************************
 * ���e  : �s�f�[�^��ǉ�����
 * ����  : S_BUFF_LINE_DATA **topPtrPtr �擪�f�[�^���Ȃ��|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA **topPtrPtr �ŏI�f�[�^���Ȃ��|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA *dataPtr �Ȃ��f�[�^
 * �߂�l: �Ȃ�
 ***************************************/
void
EditWndBufferAddLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr, S_BUFF_LINE_DATA *dataPtr )
{
    if( *topPtrPtr == NULL )
    {
        dataPtr->lineNum = 0;
    }
    else
    {
        if( *endPtrPtr != NULL )
        {
            dataPtr->lineNum = (*endPtrPtr)->lineNum+1;
        }
        else
        {
            nop(); /* �ُ� */
        }
    }

    AddLinkedList( (S_LIST_HEADER **)topPtrPtr, (S_LIST_HEADER **)endPtrPtr, (S_LIST_HEADER *)dataPtr );
}

/********************************************************************************
 * ���e  : �s�f�[�^���폜����
 * ����  : S_BUFF_LINE_DATA **topPtrPtr �擪�f�[�^���Ȃ��ł���|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA **endPtrPtr �ŏI�f�[�^���Ȃ��ł���|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA *dataPtr    �폜����f�[�^
 * �߂�l: S_BUFF_LINE_DATA *           �폜�������̃f�[�^
 ***************************************/
void
EditWndBufferRemoveLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr, S_BUFF_LINE_DATA *dataPtr )
{
    S_BUFF_LINE_DATA *removeNextPtr;

    removeNextPtr = (S_BUFF_LINE_DATA *)RemoveLinkedList( (S_LIST_HEADER **)topPtrPtr,(S_LIST_HEADER **)endPtrPtr,(S_LIST_HEADER *)dataPtr );
    if( removeNextPtr != NULL )
    {
        if( removeNextPtr->header.prevPtr != NULL )
        {
            BuffLineDataUpdateLineNumAfter( (S_BUFF_LINE_DATA *)removeNextPtr->header.prevPtr );
        }
        else
        {
            removeNextPtr->lineNum = 0;
            BuffLineDataUpdateLineNumAfter( removeNextPtr );
        }
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * ���e  : �s�f�[�^�̑}��
 * ����  : S_BUFF_LINE_DATA **topPtrPtr       �擪�f�[�^���Ȃ��|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA **endPtrPtr       �ŏI�f�[�^���Ȃ��|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA *nowPtr           �}���ʒu
 * ����  : S_BUFF_LINE_DATA **insertTopPtrPtr
 * ����  : S_BUFF_LINE_DATA **insertEndPtrPtr
 * �߂�l: void
 ***************************************/
void
EditWndBufferInsertLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr, S_BUFF_LINE_DATA *nowPtr, S_BUFF_LINE_DATA **insertTopPtrPtr, S_BUFF_LINE_DATA **insertEndPtrPtr )
{
    if( (nowPtr != NULL) && (insertTopPtrPtr != NULL) )
    {
        (*insertTopPtrPtr)->lineNum = nowPtr->lineNum;

        InsertLinkedList((S_LIST_HEADER **)topPtrPtr,(S_LIST_HEADER **)endPtrPtr,(S_LIST_HEADER *)nowPtr,(S_LIST_HEADER **)insertTopPtrPtr,(S_LIST_HEADER **)insertEndPtrPtr);
        BuffLineDataUpdateLineNumAfter( (*insertTopPtrPtr) );
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * ���e  : �s�f�[�^��u��������
 * ����  : S_BUFF_LINE_DATA **topPtrPtr �擪�f�[�^���Ȃ��|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA **endPtrPtr �ŏI�f�[�^���Ȃ��|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA *nowPtr     �u�������Ώۂ̃f�[�^
 * ����  : S_BUFF_LINE_DATA *dataPtr    �u��������f�[�^
 * �߂�l: �u���������f�[�^
 ***************************************/
S_BUFF_LINE_DATA *
EditWndBufferReplaceLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr, S_BUFF_LINE_DATA *nowPtr, S_BUFF_LINE_DATA *dataPtr )
{
    if( (topPtrPtr != NULL) && (endPtrPtr != NULL) && (nowPtr != NULL) && (dataPtr != NULL) )
    {
        dataPtr->lineNum = nowPtr->lineNum;
        dataPtr = (S_BUFF_LINE_DATA *)ReplaceLinkedList((S_LIST_HEADER **)topPtrPtr,(S_LIST_HEADER **)endPtrPtr,(S_LIST_HEADER *)nowPtr,(S_LIST_HEADER *)dataPtr);
    }
    else
    {
        nop();
    }

    return dataPtr;
}

/********************************************************************************
 * ���e  : �o�b�t�@�f�[�^�̃N���A
 * ����  : S_BUFF_LINE_DATA **topPtrPtr �擪�f�[�^���Ȃ��|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA **endPtrPtr �ŏI�f�[�^���Ȃ��|�C���^(�̃|�C���^)
 * �߂�l: �Ȃ�
 ***************************************/
void
EditWndBufferAllRemoveLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr )
{
    S_BUFF_LINE_DATA *lineDataPtr,*nextPtr;

    lineDataPtr = *topPtrPtr;
    while( lineDataPtr != NULL )
    {
        nextPtr = (S_BUFF_LINE_DATA *)RemoveLinkedList((S_LIST_HEADER **)topPtrPtr,(S_LIST_HEADER **)endPtrPtr,(S_LIST_HEADER *)lineDataPtr);
        BuffLineDataDestroy(lineDataPtr);
        lineDataPtr = nextPtr;
    }
}

/********************************************************************************
 * ���e  : �s�f�[�^�̐���
 * ����  : DWORD size          �f�[�^�T�C�Y(���s�R�[�h�܂�)
 * ����  : INT newLineCodeSize ���s�R�[�h�T�C�Y
 * ����  : TCHAR *dataPtr      �f�[�^(�|�C���^)
 * ����  : DWORD lineNum       �s�ԍ�
 * ����  : DWORD caretPos      �L�����b�g�ʒu
 * �߂�l: S_BUFF_LINE_DATA *
 ***************************************/
S_BUFF_LINE_DATA *
BuffLineDataCreate( DWORD size, INT newLineCodeSize, TCHAR *dataPtr, DWORD lineNum, DWORD caretPos )
{
    S_BUFF_LINE_DATA *newPtr = NULL;

    newPtr = (S_BUFF_LINE_DATA *)malloc( sizeof(S_BUFF_LINE_DATA) + (size * sizeof(TCHAR)) + 1 );
    if( newPtr != NULL )
    {
        memset( newPtr, sizeof(S_BUFF_LINE_DATA), 0 );
        newPtr->lineNum         = lineNum;
        newPtr->caretDataPos    = caretPos;
        newPtr->dataSize        = size;
        newPtr->newLineCodeSize = newLineCodeSize;

        if( dataPtr != NULL )
        {
            memcpy( newPtr->data, dataPtr, size );
        }
        else
        {
            nop();
        }
        newPtr->data[size] = '\0';
    }
    else
    {
        nop();
    }

    return newPtr;
}

/********************************************************************************
 * ���e  : �s�f�[�^�̉��
 * ����  : S_BUFF_LINE_DATA *
 * �߂�l: �Ȃ�
 ***************************************/
void
BuffLineDataDestroy( S_BUFF_LINE_DATA *dataPtr )
{
    free( dataPtr );
}

/********************************************************************************
 * ���e  : �s�f�[�^�̌���
 * ����  : S_BUFF_LINE_DATA *data1Ptr (���s�R�[�h�͍폜�����)
 * ����  : S_BUFF_LINE_DATA *data2Ptr
 * �߂�l: S_BUFF_LINE_DATA *
 ***************************************/
S_BUFF_LINE_DATA *
BuffLineDataJoin( S_BUFF_LINE_DATA *data1Ptr, S_BUFF_LINE_DATA *data2Ptr )
{
    S_BUFF_LINE_DATA *newPtr = NULL;

    if( (data1Ptr != NULL) && (data2Ptr != NULL) )
    {
        newPtr = BuffLineDataCreate( data1Ptr->dataSize-data1Ptr->newLineCodeSize+data2Ptr->dataSize, data2Ptr->newLineCodeSize, data1Ptr->data, data1Ptr->lineNum, 0 );
        if( newPtr != NULL )
        {
            memcpy( newPtr->data + data1Ptr->dataSize - data1Ptr->newLineCodeSize, data2Ptr->data, data2Ptr->dataSize );
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

    return newPtr;
}

/********************************************************************************
 * ���e  : �s�f�[�^�̕���
 * ����  : S_BUFF_LINE_DATA *dataPtr  ��������f�[�^
 * ����  : S_BUFF_LINE_DATA **new1Ptr ������̃f�[�^1(�̃|�C���^) (�������m�ۂ���)
 * ����  : S_BUFF_LINE_DATA **new2Ptr ������̃f�[�^2(�̃|�C���^) (�������m�ۂ���)
 * �߂�l: �Ȃ�
 ***************************************/
void
BuffLineDataDivide( S_BUFF_LINE_DATA *dataPtr, S_BUFF_LINE_DATA **new1PtrPtr, S_BUFF_LINE_DATA **new2PtrPtr )
{
    if( (dataPtr != NULL) && (new1PtrPtr != NULL) && (new2PtrPtr != NULL) )
    {
        *new1PtrPtr = BuffLineDataCreate( dataPtr->caretDataPos                  , 0                       , dataPtr->data                  , dataPtr->lineNum  , dataPtr->caretDataPos ); /* ���s���O */
        *new2PtrPtr = BuffLineDataCreate( dataPtr->dataSize-dataPtr->caretDataPos, dataPtr->newLineCodeSize, dataPtr->data+dataPtr->caretDataPos, dataPtr->lineNum+1, 0                 ); /* ���s���� */
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * ���e  : �s�f�[�^��Z������
 * ����  : S_BUFF_LINE_DATA *dataPtr
 * ����  : DWORD size
 * �߂�l: S_BUFF_LINE_DATA *
 ***************************************/
S_BUFF_LINE_DATA *
BuffLineDataShorten( S_BUFF_LINE_DATA *dataPtr, DWORD size )
{
    S_BUFF_LINE_DATA *newPtr = NULL;

    if( (dataPtr != NULL) &&
        (dataPtr->dataSize >= size) )
    {
        newPtr = BuffLineDataCreate( dataPtr->dataSize-size, dataPtr->newLineCodeSize, dataPtr->data, dataPtr->lineNum, dataPtr->caretDataPos );
        if( newPtr != NULL )
        {
            /* ���s�R�[�h�̃R�s�[ */
            memcpy( newPtr->data + newPtr->dataSize - newPtr->newLineCodeSize, dataPtr->data + dataPtr->dataSize - dataPtr->newLineCodeSize, dataPtr->newLineCodeSize );
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

    return newPtr;
}

/********************************************************************************
 * ���e  : �s�ԍ��̍X�V
 * ����  : S_BUFF_LINE_DATA *dataPtr
 * �߂�l: �Ȃ�
 ***************************************/
void
BuffLineDataUpdateLineNumAfter( S_BUFF_LINE_DATA *dataPtr )
{
    S_BUFF_LINE_DATA *nowPtr;
    DWORD i;

    if( dataPtr != NULL )
    {
        i = dataPtr->lineNum;

        for( nowPtr=dataPtr; (nowPtr != NULL); nowPtr=(S_BUFF_LINE_DATA *)nowPtr->header.nextPtr,i++ )
        {
            nowPtr->lineNum = i;
        }
    }
    else
    {
        nop();
    }
}
