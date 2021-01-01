#ifndef LINKEDLIST_H

typedef struct tagS_LIST
{
    struct tagS_LIST *prevPtr;
    struct tagS_LIST *nextPtr;
} S_LIST_HEADER;

/********************************************************************************
 * ���e  : �A�����X�g�Ƀf�[�^��ǉ�����
 * ����  : S_LIST_HEADER **topPtr �擪�f�[�^���Ȃ��|�C���^
 * ����  : S_LIST_HEADER **topPtr �ŏI�f�[�^���Ȃ��|�C���^
 * ����  : S_LIST_HEADER *dataPtr �Ȃ��f�[�^
 * �߂�l: �Ȃ�
 ***************************************/
void AddLinkedList( S_LIST_HEADER **topPtr, S_LIST_HEADER **endPtr, S_LIST_HEADER *dataPtr );

/********************************************************************************
 * ���e  : �A�����X�g�̃f�[�^��u��������
 * ����  : S_LIST_HEADER **topPtr �擪�f�[�^���Ȃ��|�C���^
 * ����  : S_LIST_HEADER **topPtr �ŏI�f�[�^���Ȃ��|�C���^
 * ����  : S_LIST_HEADER *nowPtr  �u�������Ώۂ̃f�[�^
 * ����  : S_LIST_HEADER *dataPtr �u��������f�[�^
 * �߂�l: �u���������f�[�^
 ***************************************/
S_LIST_HEADER *ReplaceLinkedList( S_LIST_HEADER **topPtr, S_LIST_HEADER **endPtr, S_LIST_HEADER *nowPtr, S_LIST_HEADER *dataPtr );

/********************************************************************************
 * ���e  : �A�����X�g����f�[�^���폜����
 * ����  : S_LIST_HEADER **topPtr �擪�f�[�^���Ȃ��ł���|�C���^
 * ����  : S_LIST_HEADER **topPtr �ŏI�f�[�^���Ȃ��ł���|�C���^
 * ����  : S_LIST_HEADER *dataPtr �폜����f�[�^
 * �߂�l: S_LIST_HEADER *        �폜�������̃f�[�^
 ***************************************/
S_LIST_HEADER *RemoveLinkedList( S_LIST_HEADER **topPtr, S_LIST_HEADER **endPtr, S_LIST_HEADER *dataPtr );

/********************************************************************************
 * ���e  : �A�����X�g�ւ̑}��
 * ����  : S_LIST_HEADER **topPtr       �擪�f�[�^���Ȃ��|�C���^
 * ����  : S_LIST_HEADER **topPtr       �ŏI�f�[�^���Ȃ��|�C���^
 * ����  : S_LIST_HEADER *nowPtr        �}���ʒu
 * ����  : S_LIST_HEADER **insertTopPtr �}������A�����X�g�̐擪
 * ����  : S_LIST_HEADER **insertEndPtr �}������A�����X�g�̍Ō�
 * �߂�l: void
 ***************************************/
void InsertLinkedList( S_LIST_HEADER **topPtr, S_LIST_HEADER **endPtr, S_LIST_HEADER *nowPtr, S_LIST_HEADER **insertTopPtr, S_LIST_HEADER **insertEndPtr );

#define LINKEDLIST_H
#endif /* LINKEDLIST_H */

