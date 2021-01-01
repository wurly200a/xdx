#include "common.h"

/* �O���֐���` */
#include "MainWnd.h"

/* �O���ϐ���` */
/* �����֐���` */
static HWND initializeApp( HINSTANCE hInst, LPSTR szCmdLine, int nCmdShow, HACCEL *hAccelPtr );

/* �����ϐ���` */
static HINSTANCE hInstance;    /* �C���X�^���X�̃n���h��     */
static TCHAR szAppName[] = TEXT("DX7/100Editor"); /* �A�v���P�[�V�����̖��� */

/********************************************************************************
 * ���e  : WINDOWS�v���O�����̃G���g���|�C���g
 * ����  : HINSTANCE hInstance     �C���X�^���X�̃n���h��
 * ����  : HINSTANCE hPrevInstance ���̃v���O���������O�Ɏ��s����Ă����C���X�^���X�̃n���h��
 * ����  : LPSTR szCmdLine         �v���O�����̋N���̂��߂Ɏg��ꂽ�R�}���h�s
 * ����  : int iCmdShow            �v���O�����̏����\���̌`�Ԃ̎w��
 * �߂�l: int
 ***************************************/
int WINAPI
WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow )
{
    MSG msg;
    HACCEL hAccel;
    HWND hWnd;

    hWnd = initializeApp( hInstance, szCmdLine, nCmdShow, &hAccel );
    if( hWnd == NULL )
    {
        MessageBox(NULL, "couldn't start!", NULL, MB_OK);
        return 0;
    }

    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        if( !IsMainWndMessage( &msg ) )
        {
            if( !TranslateAccelerator(hWnd,hAccel,&msg) )
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
            else
            {
                nop();
            }
        }
    }

    return (int)msg.wParam; /* PostQuitMessage()�̈��� */
}


/********************************************************************************
 * ���e   : �A�v���P�[�V�����̏�����
 * ����   : HINSTANCE hInst
 * ����   : LPSTR szCmdLine
 * ����   : int nCmdShow
 * ����   : HACCEL *hAccelPtr
 * �߂�l : HWND
 ***************************************/
static HWND
initializeApp( HINSTANCE hInst, LPSTR szCmdLine, int nCmdShow, HACCEL *hAccelPtr )
{
    hInstance = hInst;
    return MainWndCreate( hInst, szAppName, szCmdLine, nCmdShow, hAccelPtr ); /* ���C���E�B���h�E���� */
}
