#ifndef EDITWND_BUFFER_DATA_TYPE_H

typedef struct tag_buffer_line_data
{
    S_LIST_HEADER header         ;
    DWORD         lineNum        ; /* Y�ʒu            */
    DWORD         caretDataPos   ; /* X�ʒu            */
    DWORD         dataSize       ; /* �f�[�^�T�C�Y     */
    INT           newLineCodeSize; /* ���s�R�[�h�T�C�Y */
    TCHAR         data[]         ;
} S_BUFF_LINE_DATA;

/********************************************************************************
 * ���e  : �s�f�[�^��ǉ�����
 * ����  : S_BUFF_LINE_DATA **topPtrPtr �擪�f�[�^���Ȃ��|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA **topPtrPtr �ŏI�f�[�^���Ȃ��|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA *dataPtr �Ȃ��f�[�^
 * �߂�l: �Ȃ�
 ***************************************/
void EditWndBufferAddLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr, S_BUFF_LINE_DATA *dataPtr );

/********************************************************************************
 * ���e  : �s�f�[�^���폜����
 * ����  : S_BUFF_LINE_DATA **topPtrPtr �擪�f�[�^���Ȃ��ł���|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA **endPtrPtr �ŏI�f�[�^���Ȃ��ł���|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA *dataPtr    �폜����f�[�^
 * �߂�l: S_BUFF_LINE_DATA *           �폜�������̃f�[�^
 ***************************************/
void EditWndBufferRemoveLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr, S_BUFF_LINE_DATA *dataPtr );

/********************************************************************************
 * ���e  : �s�f�[�^�̑}��
 * ����  : S_BUFF_LINE_DATA **topPtrPtr       �擪�f�[�^���Ȃ��|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA **endPtrPtr       �ŏI�f�[�^���Ȃ��|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA *nowPtr           �}���ʒu
 * ����  : S_BUFF_LINE_DATA **insertTopPtrPtr
 * ����  : S_BUFF_LINE_DATA **insertEndPtrPtr
 * �߂�l: void
 ***************************************/
void EditWndBufferInsertLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr, S_BUFF_LINE_DATA *nowPtr, S_BUFF_LINE_DATA **insertTopPtrPtr, S_BUFF_LINE_DATA **insertEndPtrPtr );

/********************************************************************************
 * ���e  : �s�f�[�^��u��������
 * ����  : S_BUFF_LINE_DATA **topPtrPtr �擪�f�[�^���Ȃ��|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA **endPtrPtr �ŏI�f�[�^���Ȃ��|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA *nowPtr     �u�������Ώۂ̃f�[�^
 * ����  : S_BUFF_LINE_DATA *dataPtr    �u��������f�[�^
 * �߂�l: �u���������f�[�^
 ***************************************/
S_BUFF_LINE_DATA * EditWndBufferReplaceLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr, S_BUFF_LINE_DATA *nowPtr, S_BUFF_LINE_DATA *dataPtr );

/********************************************************************************
 * ���e  : �o�b�t�@�f�[�^�̃N���A
 * ����  : S_BUFF_LINE_DATA **topPtrPtr �擪�f�[�^���Ȃ��|�C���^(�̃|�C���^)
 * ����  : S_BUFF_LINE_DATA **endPtrPtr �ŏI�f�[�^���Ȃ��|�C���^(�̃|�C���^)
 * �߂�l: �Ȃ�
 ***************************************/
void EditWndBufferAllRemoveLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr );

/********************************************************************************
 * ���e  : �s�f�[�^�̐���
 * ����  : DWORD size          �f�[�^�T�C�Y(���s�R�[�h�܂�)
 * ����  : INT newLineCodeSize ���s�R�[�h�T�C�Y
 * ����  : TCHAR *dataPtr      �f�[�^(�|�C���^)
 * ����  : DWORD lineNum       �s�ԍ�
 * ����  : DWORD caretPos      �L�����b�g�ʒu
 * �߂�l: S_BUFF_LINE_DATA *
 ***************************************/
S_BUFF_LINE_DATA * BuffLineDataCreate( DWORD size, INT newLineCodeSize, TCHAR *dataPtr, DWORD lineNum, DWORD caretPos );

/********************************************************************************
 * ���e  : �s�f�[�^�̉��
 * ����  : S_BUFF_LINE_DATA *
 * �߂�l: �Ȃ�
 ***************************************/
void BuffLineDataDestroy( S_BUFF_LINE_DATA *dataPtr );

/********************************************************************************
 * ���e  : �s�f�[�^�̌���
 * ����  : S_BUFF_LINE_DATA *data1Ptr (���s�R�[�h�͍폜�����)
 * ����  : S_BUFF_LINE_DATA *data2Ptr
 * �߂�l: S_BUFF_LINE_DATA *
 ***************************************/
S_BUFF_LINE_DATA * BuffLineDataJoin( S_BUFF_LINE_DATA *data1Ptr, S_BUFF_LINE_DATA *data2Ptr );

/********************************************************************************
 * ���e  : �s�f�[�^�̕���
 * ����  : S_BUFF_LINE_DATA *dataPtr  ��������f�[�^
 * ����  : S_BUFF_LINE_DATA **new1Ptr ������̃f�[�^1(�̃|�C���^) (�������m�ۂ���)
 * ����  : S_BUFF_LINE_DATA **new2Ptr ������̃f�[�^2(�̃|�C���^) (�������m�ۂ���)
 * �߂�l: �Ȃ�
 ***************************************/
void BuffLineDataDivide( S_BUFF_LINE_DATA *dataPtr, S_BUFF_LINE_DATA **new1PtrPtr, S_BUFF_LINE_DATA **new2PtrPtr );

/********************************************************************************
 * ���e  : �s�f�[�^��Z������
 * ����  : S_BUFF_LINE_DATA *dataPtr
 * ����  : DWORD size
 * �߂�l: S_BUFF_LINE_DATA *
 ***************************************/
S_BUFF_LINE_DATA * BuffLineDataShorten( S_BUFF_LINE_DATA *dataPtr, DWORD size );

/********************************************************************************
 * ���e  : �s�ԍ��̍X�V
 * ����  : S_BUFF_LINE_DATA *dataPtr
 * �߂�l: �Ȃ�
 ***************************************/
void BuffLineDataUpdateLineNumAfter( S_BUFF_LINE_DATA *dataPtr );

#define EDITWND_BUFFER_DATA_TYPE_H
#endif /* EDITWND_BUFFER_DATA_TYPE_H */
