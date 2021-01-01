#ifndef STS_BAR_H

typedef enum
{
    STS_BAR_0,
    STS_BAR_1,
    STS_BAR_MAX
}STS_BAR_ID;

/********************************************************************************
 * ���e  : �X�e�[�^�X�o�[����
 * ����  : HWND hwnd �e�E�B���h�E�̃n���h��
 * ����  : BOOL fShow �f�t�H���g�\�����邩�ۂ�
 * �߂�l: HWND
 ***************************************/
HWND StsBarCreate( HWND hwnd, BOOL fShow );

/********************************************************************************
 * ���e  : �X�e�[�^�X�o�[�̃T�C�Y����
 * ����  : int cxClient
 * ����  : int cyClient
 * �߂�l: LONG �c�����̃T�C�Y
 ***************************************/
LONG StsBarSize( int cxClient,int cyClient );

/********************************************************************************
 * ���e  : �X�e�[�^�X�o�[�ւ̕�����Z�b�g
 * ����  : STS_BAR_ID id
 * ����  : PTSTR ptstrFormat, ...
 * �߂�l: �Ȃ�
 ***************************************/
void StsBarSetText( STS_BAR_ID id, PTSTR ptstrFormat, ... );

/********************************************************************************
 * ���e  : �X�e�[�^�X�o�[�̕\��
 * ����  : BOOL fShow
 * �߂�l: BOOL
 ***************************************/
BOOL StsBarShowWindow( BOOL fShow );

#define STS_BAR_H
#endif /* STS_BAR_H */
