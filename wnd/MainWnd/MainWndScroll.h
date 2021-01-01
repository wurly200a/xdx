#ifndef MAINWND_SCROLL_H

/********************************************************************************
 * ���e  : �X�N���[�����̃Z�b�g
 * ����  : HWND hwnd
 * �߂�l: �Ȃ�
 ***************************************/
void InitMainWndScrollInfo( HWND hwnd );

/********************************************************************************
 * ���e  : �X�N���[��
 * ����  : HWND hwnd
 * �߂�l: �Ȃ�
 ***************************************/
void MainWndHscroll( HWND hwnd, WPARAM wParam );

/********************************************************************************
 * ���e  : �X�N���[��
 * ����  : HWND hwnd
 * �߂�l: �Ȃ�
 ***************************************/
void MainWndVscroll( HWND hwnd, WPARAM wParam );

/********************************************************************************
 * ���e  : �X�N���[�����̃Z�b�g
 * ����  : HWND hwnd
 * ����  : cxClient
 * ����  : cyClient
 * �߂�l: �Ȃ�
 ***************************************/
void SetMainWndAllScrollInfo( HWND hwnd, int cxClient, int cyClient );

/********************************************************************************
 * ���e  : �X�N���[�����̎擾
 * ����  : HWND hwnd
 * ����  : *iHorzPosPtr
 * ����  : *iVertPosPtr
 * �߂�l: �Ȃ�
 ***************************************/
void GetMainWndAllScrollInfo( HWND hwnd, int *iHorzPosPtr, int *iVertPosPtr );

/********************************************************************************
 * ���e  : �X�N���[���ʒu�̃Z�b�g
 * ����  : HWND hwnd
 * ����  : int   fnBar �X�N���[���o�[�̃^�C�v
 * ����  : DWORD nPos  �ʒu
 * �߂�l: �Ȃ�
 ***************************************/
void SetMainWndScrollPos( HWND hwnd, int fnBar, DWORD nPos );

#define MAINWND_SCROLL_H
#endif /* MAINWND_SCROLL_H */
