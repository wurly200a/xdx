#ifndef MODAL_DLG_H

/* �Ǘ�����ݒ��ID */
typedef enum
{
    MODAL_DLG_ID_ABOUT,
    MODAL_DLG_ID_MAX
}MODAL_DLG_ID;

typedef struct
{
    DWORD dwData1;
} S_MODAL_DLG_DATA;

/********************************************************************************
 * ���e  : ���[�_���_�C�A���O�{�b�N�X������
 * ����  : �Ȃ�
 * �߂�l: BOOL
 ***************************************/
BOOL ModalDlgInit( void );

/********************************************************************************
 * ���e  : ���[�_���_�C�A���O�{�b�N�X
 * ����  : MODAL_DLG_ID      id
 * ����  : S_MODAL_DLG_DATA *dataPtr
 * ����  : HWND              hwnd
 * ����  : int               x
 * ����  : int               y
 * �߂�l: BOOL
 ***************************************/
BOOL ModalDlg( MODAL_DLG_ID id, S_MODAL_DLG_DATA *dataPtr, HWND hwnd, int x, int y );

#define MODAL_DLG_H
#endif /* MODAL_DLG_H */
