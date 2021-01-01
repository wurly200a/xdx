#ifndef SOME_CTRL_H

typedef enum
{
    SOME_CTRL_GROUP_ALWAYS   ,
    SOME_CTRL_GROUP_SYSTEM   ,
    SOME_CTRL_GROUP_1VOICE   ,
    SOME_CTRL_GROUP_ALL_VOICE,
    SOME_CTRL_GROUP_MAX
}SOME_CTRL_GROUP_ID;

typedef enum
{
    SOME_CTRL_MIDI_IN_STATIC         ,
    SOME_CTRL_MIDI_IN                ,
    SOME_CTRL_MIDI_IN_OPEN_BUTTON    ,
    SOME_CTRL_MIDI_OUT_STATIC        ,
    SOME_CTRL_MIDI_OUT               ,
    SOME_CTRL_MIDI_OUT_OPEN_BUTTON   ,
    SOME_CTRL_MIDI_KEY_IN_STATIC     ,
    SOME_CTRL_MIDI_KEY_IN            ,
    SOME_CTRL_MIDI_KEY_IN_OPEN_BUTTON,
    SOME_CTRL_DEBUG_BUTTON           ,

    SOME_CTRL_GROUP_BOX1             ,
    SOME_CTRL_MODE_SYSTEM            ,
    SOME_CTRL_MODE_1VOICE            ,
    SOME_CTRL_MODE_ALL_VOICE         ,

    SOME_CTRL_GROUP_BOX2             ,
    SOME_CTRL_SYSTEM_GET_BUTTON      ,
    SOME_CTRL_SYSTEM_SET_BUTTON      ,

    SOME_CTRL_GROUP_BOX3             ,
    SOME_CTRL_VOICE_LOAD_BUTTON      ,
    SOME_CTRL_VOICE_SAVE_BUTTON      ,
    SOME_CTRL_VOICE_GET_BUTTON       ,
    SOME_CTRL_VOICE_SET_BUTTON       ,

    SOME_CTRL_GROUP_BOX4             ,
    SOME_CTRL_ALL_VOICE_LOAD_BUTTON  ,
    SOME_CTRL_ALL_VOICE_SAVE_BUTTON  ,
    SOME_CTRL_ALL_VOICE_GET_BUTTON   ,
    SOME_CTRL_ALL_VOICE_SET_BUTTON   ,

    SOME_CTRL_MAX
}SOME_CTRL_ID;

#define SOME_CTRL_ID_OFFSET 10
#define MY_EX_STYLE (WS_EX_OVERLAPPEDWINDOW)

/********************************************************************************
 * ���e  : �o�^���ꂽ�S�ẴR���g���[���𐶐�����
 * ����  : HWND hwnd �e�E�B���h�E�̃n���h��
 * �߂�l: BOOL
 ***************************************/
BOOL SomeCtrlCreate( HWND hwnd );

/********************************************************************************
 * ���e  : �w��̃O���[�v�̃R���g���[����\������
 * ����  : SOME_CTRL_GROUP_ID groupId
 * �߂�l: BOOL
 ***************************************/
BOOL SomeCtrlGroupDisplay( SOME_CTRL_GROUP_ID groupId );

/********************************************************************************
 * ���e  : �o�^���ꂽ�S�ẴR���g���[���̃T�C�Y�𒲐�����
 * ����  : int cxClient
 * ����  : int cyClient
 * �߂�l: LONG �R���g���[����z�u�����`�̏c�����̃T�C�Y
 ***************************************/
LONG SomeCtrlSize( int cxClient,int cyClient );

/********************************************************************************
 * ���e  : �R���g���[���̃E�B���h�E�n���h�����擾����
 * ����  : SOME_CTRL_ID id
 * �߂�l: HWND
 ***************************************/
HWND SomeCtrlGetHWND( SOME_CTRL_ID id );

/********************************************************************************
 * ���e  : �R���g���[����L���ɂ���
 * ����  : SOME_CTRL_ID id
 * �߂�l: �Ȃ�
 ***************************************/
void SomeCtrlEnable( SOME_CTRL_ID id );

/********************************************************************************
 * ���e  : �R���g���[���𖳌��ɂ���
 * ����  : SOME_CTRL_ID id
 * �߂�l: �Ȃ�
 ***************************************/
void SomeCtrlDisable( SOME_CTRL_ID id );

/********************************************************************************
 * ���e  : �R���g���[������e�L�X�g���擾����
 * ����  : SOME_CTRL_ID id
 * ����  : PTSTR ptstrText
 * �߂�l: BOOL
 ***************************************/
BOOL SomeCtrlGetText( SOME_CTRL_ID id, PTSTR ptstrText );

#define SOME_CTRL_H
#endif /* SOME_CTRL_H */
