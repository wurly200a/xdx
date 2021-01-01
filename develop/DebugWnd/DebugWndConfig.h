#ifndef DEBUG_CONFIG

/* �Ǘ�����ݒ��ID */
typedef enum
{
   DEBUG_CONFIG_ID_WINDOW_POS_X,
   DEBUG_CONFIG_ID_WINDOW_POS_Y,
   DEBUG_CONFIG_ID_WINDOW_POS_DX,
   DEBUG_CONFIG_ID_WINDOW_POS_DY,
   DEBUG_CONFIG_ID_MAX
}DEBUG_CONFIG_ID;

/********************************************************************************
 * ���e  : �ݒ�Ǘ����W���[���̏�����
 * ����  : �Ȃ�
 * �߂�l: �Ȃ�
 ***************************************/
void DebugConfigInit( void );

/********************************************************************************
 * ���e  : DWORD�ݒ�l��ۑ�����
 * ����  : DEBUG_CONFIG_ID id
 * ����  : DWORD     data
 * �߂�l: �Ȃ�
 ***************************************/
void DebugConfigSaveDword(DEBUG_CONFIG_ID id, DWORD data );

/********************************************************************************
 * ���e  : DWORD�ݒ�l��ǂݍ���
 * ����  : DEBUG_CONFIG_ID id
 * �߂�l: �Ȃ�
 ***************************************/
INT DebugConfigLoadDword(DEBUG_CONFIG_ID id );

#define DEBUG_CONFIG
#endif /* DEBUG_CONFIG */
