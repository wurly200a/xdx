#ifndef PARAM_CTRL_H
#define PARAM_CTRL_H

#define PARAM_CTRL_ID_OFFSET 100

enum
{
    PARAM_CTRL_TARGET_NAME,
    PARAM_CTRL_TARGET_DATA,
    PARAM_CTRL_TARGET_MAX
};
typedef BYTE PARAM_CTRL_TARGET;

typedef enum
{
    PARAM_CTRL_GROUP_SYSTEM_COMMON,
    PARAM_CTRL_GROUP_PATCH_COMMON ,
    PARAM_CTRL_GROUP_PATCH_TONE   ,
    PARAM_CTRL_GROUP_MAX
}PARAM_CTRL_GROUP_ID;

typedef enum
{
    PARAM_CTRL_SYSCMN_SOUNDMODE                   ,
    PARAM_CTRL_PATCHCMN_PATCHNAME             ,
    PARAM_CTRL_PATCHTN_TONESWITCH                 ,

    PARAM_CTRL_MAX
}PARAM_CTRL_ID;

/********************************************************************************
 * ���e  : �o�^���ꂽ�S�Ẵp�����[�^�R���g���[���𐶐�����
 * ����  : HWND hwnd �e�E�B���h�E�̃n���h��
 * �߂�l: BOOL
 ***************************************/
BOOL ParamCtrlCreate( HWND hwnd );

/********************************************************************************
 * ���e  : �w��̃O���[�v�̃p�����[�^�R���g���[����\��
 * ����  : PARAM_CTRL_GROUP_ID groupId
 * �߂�l: BOOL
 ***************************************/
BOOL ParamCtrlGroupDisplay( PARAM_CTRL_GROUP_ID groupId );

/********************************************************************************
 * ���e  : �E�B���h�E�n���h�����擾����
 * ����  : PARAM_CTRL_ID id
 * �߂�l: HWND
 ***************************************/
HWND ParamCtrlGetHWND( PARAM_CTRL_ID id );

#endif /* PARAM_CTRL_H */
