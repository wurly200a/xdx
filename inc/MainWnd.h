#ifndef MAINWND_H

/********************************************************************************
 * ���e  : ���C���E�B���h�E�N���X�̓o�^�A�E�B���h�E�̐���
 * ����  : LPSTR szCmdLine
 * ����  : int nCmdShow
 * ����  : HACCEL *hAccelPtr
 * �߂�l: HWND
 ***************************************/
HWND MainWndCreate( LPSTR szCmdLine, int nCmdShow, HACCEL *hAccelPtr );

/********************************************************************************
 * ���e  : ���C���E�B���h�E���ŏ������郁�b�Z�[�W�𔻒肷��
 * ����  : MSG *msgPtr
 * �߂�l: BOOL
 ***************************************/
BOOL IsMainWndMessage( MSG *msgPtr );

#define MAINWND_H
#endif /* MAINWND_H */
