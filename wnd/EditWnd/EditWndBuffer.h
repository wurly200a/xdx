#ifndef EDITWND_BUFFER_H

typedef struct tagS_EDITWND_BUFF
{
    int unused;
} S_EDITWND_BUFF;
typedef S_EDITWND_BUFF * H_EDITWND_BUFF;

typedef struct tagS_EDITWND_BUFF_LINE
{
    int unused;
} S_EDITWND_BUFF_LINE;
typedef S_EDITWND_BUFF_LINE * H_EDITWND_BUFF_LINE;

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃I�u�W�F�N�g����
 * ����  : �Ȃ�
 * �߂�l: H_EDITWND_BUFF
 ***************************************/
H_EDITWND_BUFF EditWndBuffCreate( void );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃I�u�W�F�N�g�j��
 * ����  : H_EDITWND_BUFF
 * �߂�l: BOOL
 ***************************************/
BOOL EditWndBuffDestroy( H_EDITWND_BUFF hEditWndBuff );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̏�����
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: �Ȃ�
 ***************************************/
void EditWndBuffInit( H_EDITWND_BUFF hEditWndBuff );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̏I��
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: �Ȃ�
 ***************************************/
void EditWndBuffEnd( H_EDITWND_BUFF hEditWndBuff );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃f�[�^�Z�b�g
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : TCHAR* dataPtr
 * ����  : DWORD  length
 * ����  : BOOL   bInit  (TRUE:�����f�[�^���N���A,FALSE:�N���A���Ȃ�)
 * �߂�l: �Ȃ�
 ***************************************/
void EditWndBuffDataSet( H_EDITWND_BUFF hEditWndBuff, TCHAR* dataPtr, DWORD length, BOOL bInit );

typedef enum
{
    BUFF_ALL,
    BUFF_SELECTED
} EDITWND_BUFF_REGION;

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃f�[�^�擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : TCHAR *dataPtr
 * ����  : DWORD dataSize
 * ����  : EDITWND_BUFF_REGION region
 * �߂�l: BOOL
 ***************************************/
BOOL EditWndBuffDataGet( H_EDITWND_BUFF hEditWndBuff, TCHAR *dataPtr, DWORD dataSize, EDITWND_BUFF_REGION region );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃f�[�^�T�C�Y�擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : EDITWND_BUFF_REGION region
 * �߂�l: DWORD
 ***************************************/
DWORD EditWndBuffGetDataSize( H_EDITWND_BUFF hEditWndBuff, EDITWND_BUFF_REGION region );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̍ő�s�T�C�Y�擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: DWORD
 ***************************************/
DWORD EditWndBuffGetLineMaxSize( H_EDITWND_BUFF hEditWndBuff );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̍ő啶���T�C�Y�擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: DWORD
 ***************************************/
DWORD EditWndBuffGetColumnMaxSize( H_EDITWND_BUFF hEditWndBuff);

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃L�����b�gX�ʒu�擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: DWORD
 ***************************************/
DWORD EditWndBuffGetCaretXpos( H_EDITWND_BUFF hEditWndBuff );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃L�����b�gY�ʒu�擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: DWORD
 ***************************************/
DWORD EditWndBuffGetCaretYpos( H_EDITWND_BUFF hEditWndBuff );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃L�����b�g�ʒu�Z�b�g
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : DWORD xPos
 * ����  : DWORD lineNum
 * �߂�l: BOOL (TRUE:�L�����b�g�ʒu���ς����,FALSE:�ς��Ȃ�)
 ***************************************/
BOOL EditWndBuffSetCaretPos( H_EDITWND_BUFF hEditWndBuff, DWORD xPos, DWORD lineNum );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃L�����b�gX�ʒu���Z
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: �Ȃ�
 ***************************************/
void EditWndBuffIncCaretXpos( H_EDITWND_BUFF hEditWndBuff );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃L�����b�gX�ʒu���Z
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: �Ȃ�
 ***************************************/
void EditWndBuffDecCaretXpos( H_EDITWND_BUFF hEditWndBuff );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃L�����b�gY�ʒu���Z
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: �Ȃ�
 ***************************************/
void EditWndBuffIncCaretYpos( H_EDITWND_BUFF hEditWndBuff );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃L�����b�gY�ʒu���Z
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: �Ȃ�
 ***************************************/
void EditWndBuffDecCaretYpos( H_EDITWND_BUFF hEditWndBuff );

/********************************************************************************
 * ���e  : �w��s�f�[�^�̃n���h���擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : DWORD lineNum
 * �߂�l: H_EDITWND_BUFF_LINE
 ***************************************/
H_EDITWND_BUFF_LINE EditWndBuffGetLinePtr( H_EDITWND_BUFF hEditWndBuff, DWORD lineNum );

/********************************************************************************
 * ���e  : ���̍s�f�[�^�̃n���h���擾
 * ����  : H_EDITWND_BUFF_LINE hLineData
 * �߂�l: H_EDITWND_BUFF_LINE
 ***************************************/
H_EDITWND_BUFF_LINE EditWndBuffGetLineNextPtr( H_EDITWND_BUFF_LINE hLineData );

enum
{
    SINGLE_CHAR     ,
    DOUBLE_CHAR_HIGH,
    DOUBLE_CHAR_LOW ,
    TAB_CHAR        ,
    END_CHAR        ,
};
typedef int CHARSET_TYPE;

typedef struct
{
    TCHAR        data[8] ; /* �f�[�^(���p,�S�p,�^�u)                   */
    INT          offset  ; /* ���p�ȊO�̏ꍇ�A�擪����̃I�t�Z�b�g�ʒu */
    INT          size    ; /* �f�[�^�T�C�Y                             */
    CHARSET_TYPE type    ;
    BOOL         bSelect ; /* �I������Ă��邩�ۂ�                     */
    DWORD        dataPos ; /* �f�[�^�ʒu                               */
    DWORD        caretPos; /* �L�����b�g�ʒu(��؂�̂����ʒu)         */
} S_BUFF_DISP_DATA;

/********************************************************************************
 * ���e  : �w��s�A�w���̃f�[�^���擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : H_EDITWND_BUFF_LINE hLineData
 * ����  : DWORD             dispPos     �\���ʒu
 * ����  : S_BUFF_DISP_DATA *dataPtr
 * �߂�l: BOOL
 ***************************************/
BOOL EditWndBuffGetDispData( H_EDITWND_BUFF hEditWndBuff, H_EDITWND_BUFF_LINE hLineData, DWORD dispPos, S_BUFF_DISP_DATA *dataPtr );

enum
{
    EDITWND_BUFF_NEWLINE_CRLF= 0,
    EDITWND_BUFF_NEWLINE_LF  ,
    EDITWND_BUFF_NEWLINE_CR  ,
    EDITWND_BUFF_NEWLINE_NONE,
};
/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̉��s�R�[�h�Z�b�g
 * ����  : UINT newLineType
 * �߂�l: �Ȃ�
 ***************************************/
void EditWndBuffSetNewLineCode( H_EDITWND_BUFF hEditWndBuff, UINT newLineType );

/********************************************************************************
 * ���e  : �f�[�^�폜
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : BOOL bBackSpace
 * �߂�l: �Ȃ�
 ***************************************/
void EditWndBuffRemoveData( H_EDITWND_BUFF hEditWndBuff, BOOL bBackSpace );

/********************************************************************************
 * ���e  : ���s�f�[�^�擾
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : PTSTR dataPtr
 * �߂�l: INT
 ***************************************/
INT EditWndBuffGetNewLineData( H_EDITWND_BUFF hEditWndBuff, PTSTR dataPtr );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̃^�u�T�C�Y�Z�b�g
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : INT tabSize
 * �߂�l: BOOL (TRUE:�ύX���ꂽ)
 ***************************************/
BOOL EditWndBuffSetTabSize( H_EDITWND_BUFF hEditWndBuff, INT tabSize );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�͈̔͑I��ON
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: �Ȃ�
 ***************************************/
void EditWndBuffSelectOn( H_EDITWND_BUFF hEditWndBuff );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�͈̔͑I��OFF
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: BOOL (TRUE: �I��͈͂��������ꂽ,FALSE: �I��͈͖���)
 ***************************************/
BOOL EditWndBuffSelectOff( H_EDITWND_BUFF hEditWndBuff );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̑S�͈͑I��
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: BOOL
 ***************************************/
BOOL EditWndBuffSelectAll( H_EDITWND_BUFF hEditWndBuff );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@�̌����f�[�^�Z�b�g
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * ����  : TCHAR* dataPtr
 * ����  : DWORD  length
 * ����  : BOOL bDirectionUp
 * �߂�l: BOOL
 ***************************************/
BOOL EditWndBuffFindDataSet( H_EDITWND_BUFF hEditWndBuff, TCHAR* dataPtr, DWORD length, BOOL bDirectionUp );

/********************************************************************************
 * ���e  : EDIT�E�B���h�E�o�b�t�@��Undo
 * ����  : H_EDITWND_BUFF hEditWndBuff
 * �߂�l: BOOL
 ***************************************/
BOOL EditWndBuffUndo( H_EDITWND_BUFF hEditWndBuff );

#define EDITWND_BUFFER_H
#endif /* EDITWND_BUFFER_H */
