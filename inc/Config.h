#ifndef CONFIG_H

/* �Ǘ�����ݒ��ID */
typedef enum
{
    CONFIG_ID_WINDOW_POS_X    ,
    CONFIG_ID_WINDOW_POS_Y    ,
    CONFIG_ID_WINDOW_POS_DX   ,
    CONFIG_ID_WINDOW_POS_DY   ,
    CONFIG_ID_MIDI_IN_NUM     ,
    CONFIG_ID_MIDI_OUT_NUM    ,
    CONFIG_ID_MIDI_KEY_IN_NUM ,
    CONFIG_ID_MAX
}CONFIG_ID;

/********************************************************************************
 * ���e  : �ݒ�Ǘ����W���[���̏�����
 * ����  : HINSTANCE hInst
 * ����  : PTSTR szAppName
 * �߂�l: �Ȃ�
 ***************************************/
void ConfigInit( HINSTANCE hInst, PTSTR szAppName );

/********************************************************************************
 * ���e  : DWORD�ݒ�l��ۑ�����
 * ����  : CONFIG_ID id
 * ����  : DWORD     data
 * �߂�l: �Ȃ�
 ***************************************/
void ConfigSaveDword( CONFIG_ID id, DWORD data );

/********************************************************************************
 * ���e  : DWORD�ݒ�l��ǂݍ���
 * ����  : CONFIG_ID id
 * �߂�l: �Ȃ�
 ***************************************/
INT ConfigLoadDword( CONFIG_ID id );

/**
 * ------------------------------------------------------------------------------
 * @brief  ������ݒ�l��ۑ�����
 * @param  id
 * @param  ptstrValue
 * @return �Ȃ�
 * @date 2017�N01��04�� 13��50��25�b
 **/
void ConfigSaveString( CONFIG_ID id, PTSTR ptstrValue );

/**
 * ------------------------------------------------------------------------------
 * @brief  ������ݒ�l��ǂݍ���
 * @param  id
 * @return PTSTR
 * @date 2017�N01��04�� 13��50��25�b
 **/
PTSTR ConfigLoadString( CONFIG_ID id );

/**
 * ------------------------------------------------------------------------------
 * @brief  �ݒ�l��ۑ�����
 * @param  start_id
 * @param  end_id
 * @return �Ȃ�
 * @date 2017�N01��04�� 13��50��25�b
 **/
PTSTR ConfigWrite( CONFIG_ID start_id, CONFIG_ID end_id );


/**
 * ------------------------------------------------------------------------------
 * @brief  �ݒ�l���ύX����Ă��Ȃ����`�F�b�N����
 * @param  start_id
 * @param  end_id
 * @return
 * @date 2017�N01��04�� 13��50��25�b
 **/
BOOL ConfigIsSame( CONFIG_ID start_id, CONFIG_ID end_id );

/********************************************************************************
 * ���e  : �f�o�b�O�ϐ�(DWORD�l)��ǂݍ���
 * ����  : �Ȃ�
 * �߂�l: INT
 ***************************************/
DWORD ConfigLoadDebugValue( void );

#define CONFIG_H
#endif /* CONFIG_H */
