#ifndef EDITWND_H

/* [���b�Z�[�W]                                                                 */
/* WM_CUT                        ��                                             */
/* WM_COPY                       ��                                             */
/* WM_PASTE                      ��                                             */
/* WM_CLEAR                      ��                                             */
/* WM_SETFONT                    ��                                             */
/* EM_GETSEL                     �~                                             */
/* EM_SETSEL                     ��(wParam==0,lParam=-1�̎��̂�)                */
/* EM_GETRECT                    �~                                             */
/* EM_SETRECT                    �~                                             */
/* EM_SETRECTNP                  �~                                             */
/* EM_SCROLL                     �~                                             */
/* EM_LINESCROLL                 �~                                             */
/* EM_SCROLLCARET                �~                                             */
/* EM_GETMODIFY                  �~                                             */
/* EM_SETMODIFY                  �~                                             */
/* EM_GETLINECOUNT               ��                                             */
/* EM_LINEINDEX                  �~                                             */
/* EM_SETHANDLE                  �~                                             */
/* EM_GETHANDLE                  �~                                             */
/* EM_GETTHUMB                   �~                                             */
/* EM_LINELENGTH                 �~                                             */
/* EM_REPLACESEL                 �~                                             */
/* EM_GETLINE                    �~                                             */
/* EM_CANUNDO                    �~                                             */
/* EM_UNDO                       �~                                             */
/* EM_FMTLINES                   �~                                             */
/* EM_LINEFROMCHAR               ��(wParam==-1�̎��̂�)                         */
/* EM_SETTABSTOPS                �~                                             */
/* EM_SETPASSWORDCHAR            �~                                             */
/* EM_EMPTYUNDOBUFFER            �~                                             */
/* EM_GETFIRSTVISIBLELINE        �~                                             */
/* EM_SETREADONLY                �~                                             */
/* EM_SETWORDBREAKPROC           �~                                             */
/* EM_GETWORDBREAKPROC           �~                                             */
/* EM_GETPASSWORDCHAR            �~                                             */
/* EM_SETMARGINS                 �~                                             */
/* EM_GETMARGINS                 �~                                             */
/* EM_SETLIMITTEXT (EM_LIMITTEXT)�~                                             */
/* EM_GETLIMITTEXT               �~                                             */
/* EM_POSFROMCHAR                �~                                             */
/* EM_CHARFROMPOS                �~                                             */
/* EM_SETIMESTATUS               �~                                             */
/* EM_GETIMESTATUS               �~                                             */
/*                                                                              */
/* [�֐�]                                                                       */
/* GetWindowTextLength(HWND hWnd)                           �~                  */
/* GetWindowText(HWND hWnd,LPTSTR lpString,int nMaxCount)   �~                  */
/* SetWindowText(HWND hWnd,LPCTSTR lpString)                �~                  */
/*                                                                              */

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�N���X�̓o�^
 * ����  : HINSTANCE hInst
 * �߂�l: BOOL
 ***************************************/
BOOL EditWndRegisterClass( HINSTANCE hInst );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�̏�����
 * ����  : HWND hwnd
 * �߂�l: �Ȃ�
 ***************************************/
void EditWndDataInit( HWND hwnd );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�̃f�[�^�Z�b�g
 * ����  : HWND hwnd
 * ����  : TCHAR* dataPtr
 * ����  : DWORD  length
 * ����  : BOOL   bInit (TRUE:�����f�[�^���N���A,FALSE:�N���A���Ȃ�)
 * �߂�l: �Ȃ�
 ***************************************/
void EditWndDataSet( HWND hwnd, TCHAR* dataPtr, DWORD length, BOOL bInit );

typedef enum
{
    EDITWND_ALL,
    EDITWND_SELECTED
} EDITWND_REGION;

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�̃f�[�^�擾
 * ����  : HWND hwnd
 * ����  : TCHAR *dataPtr
 * ����  : DWORD dataSize
 * ����  : EDITWND_REGION region
 * �߂�l: BOOL
 ***************************************/
BOOL EditWndDataGet( HWND hwnd, TCHAR *dataPtr, DWORD dataSize, EDITWND_REGION region );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�̃f�[�^�T�C�Y�擾
 * ����  : HWND hwnd
 * ����  : EDITWND_REGION region
 * �߂�l: DWORD
 ***************************************/
DWORD EditWndGetDataSize( HWND hwnd, EDITWND_REGION region );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̍ő啶���T�C�Y�擾
 * ����  : HWND hwnd
 * �߂�l: DWORD
 ***************************************/
DWORD EditWndGetColumnMaxSize( HWND hwnd );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃L�����b�gX�ʒu�擾
 * ����  : HWND hwnd
 * �߂�l: DWORD
 ***************************************/
DWORD EditWndGetCaretXpos( HWND hwnd );

enum
{
    NEWLINECODE_CRLF = 0,
    NEWLINECODE_LF   ,
    NEWLINECODE_CR   ,
    NEWLINECODE_NONE ,
};
typedef UINT NEWLINECODE_TYPE;
/********************************************************************************
 * ���e  : EDIT�E�B���h�E�̉��s�R�[�h�Z�b�g
 * ����  : HWND hwnd
 * ����  : NEWLINECODE_TYPE newLineCodeType
 * �߂�l: BOOL (TRUE:�\�����������K�v)
 ***************************************/
BOOL EditWndNewLineCodeSet( HWND hwnd, NEWLINECODE_TYPE newLineCodeType );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�̌���������f�[�^�Z�b�g
 * ����  : HWND hwnd
 * ����  : TCHAR* dataPtr
 * ����  : DWORD  length
 * ����  : BOOL bDirectionUp
 * �߂�l: BOOL
 ***************************************/
BOOL EditWndFindDataSet( HWND hwnd, TCHAR* dataPtr, DWORD length, BOOL bDirectionUp );

#define EDITWND_H
#endif /* EDITWND_H */
