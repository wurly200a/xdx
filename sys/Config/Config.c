/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */

/* �O���֐���` */
#include "WinMain.h"

/* �O���ϐ���` */
/* �����֐���` */
#include "Config.h"
/* �����ϐ���` */
static BOOL bInitConfig;
static TCHAR szIniFileName[512];

typedef enum
{
    CONFIG_DATA_DWORD,
    CONFIG_DATA_STRING,
} CONFIG_DATA_TYPE;

typedef struct
{
    PTSTR            pKeyName;    /* �L�[�� */
    PTSTR            pInitValue;  /* �����l */
    CONFIG_DATA_TYPE dataType;
} S_CONFIG_INFO;

static S_CONFIG_INFO configInfoTbl[CONFIG_ID_MAX] =
{
    { TEXT("iWindowPosX"   ),TEXT("0x00000000"), CONFIG_DATA_DWORD },
    { TEXT("iWindowPosY"   ),TEXT("0x00000000"), CONFIG_DATA_DWORD },
    { TEXT("iWindowPosDX"  ),TEXT("0x000001F4"), CONFIG_DATA_DWORD }, /* 500 */
    { TEXT("iWindowPosDY"  ),TEXT("0x0000012C"), CONFIG_DATA_DWORD }, /* 300 */
    { TEXT("iMidiInNum"    ),TEXT("0x00000000"), CONFIG_DATA_DWORD },
    { TEXT("iMidiOutNum"   ),TEXT("0x00000000"), CONFIG_DATA_DWORD },
    { TEXT("iMidiKeyInNum" ),TEXT("0x00000000"), CONFIG_DATA_DWORD },
};

#define CONFIG_DATA_STRING_LENGTH_MAX 512
typedef struct
{
    TCHAR szValue[CONFIG_DATA_STRING_LENGTH_MAX];
} S_CONFIG_DATA;

S_CONFIG_DATA configData[CONFIG_ID_MAX];

/********************************************************************************
 * ���e  : �ݒ�Ǘ����W���[���̏�����
 * ����  : �Ȃ�
 * �߂�l: �Ȃ�
 ***************************************/
void
ConfigInit( void )
{
    DWORD length;
    int i;

    length = GetModuleFileName(NULL,szIniFileName,512);
    szIniFileName[length-1] = 'i';
    szIniFileName[length-2] = 'n';
    szIniFileName[length-3] = 'i';

    for( i=0; i<CONFIG_ID_MAX; i++ )
    {
        GetPrivateProfileString( GetAppName(), configInfoTbl[i].pKeyName, configInfoTbl[i].pInitValue, configData[i].szValue, CONFIG_DATA_STRING_LENGTH_MAX, szIniFileName );
    }
    bInitConfig = TRUE;
}

/********************************************************************************
 * ���e  : DWORD�ݒ�l��ۑ�����
 * ����  : CONFIG_ID id
 * ����  : DWORD     data
 * �߂�l: �Ȃ�
 ***************************************/
void
ConfigSaveDword( CONFIG_ID id, DWORD data )
{
    if( bInitConfig )
    {
        if( (id < CONFIG_ID_MAX) &&
            (configInfoTbl[id].dataType == CONFIG_DATA_DWORD) )
        {
            wsprintf( configData[id].szValue, "0x%08lX", data );
#ifdef OLD_CONFIG_SAVE_LOAD
            WritePrivateProfileString( GetAppName(), configInfoTbl[id].pKeyName, configData[id].szValue, szIniFileName );
#endif
        }
        else
        {
            nop();
        }
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * ���e  : DWORD�ݒ�l��ǂݍ���
 * ����  : CONFIG_ID id
 * �߂�l: �Ȃ�
 ***************************************/
INT
ConfigLoadDword( CONFIG_ID id )
{
    DWORD rtn = (DWORD)0;

    if( bInitConfig )
    {
        if( (id < CONFIG_ID_MAX) &&
            (configInfoTbl[id].dataType == CONFIG_DATA_DWORD) )
        {
#ifdef OLD_CONFIG_SAVE_LOAD
            GetPrivateProfileString( GetAppName(), configInfoTbl[id].pKeyName, configInfoTbl[id].pInitValue, configInfoTbl[id].szValue, CONFIG_DATA_STRING_LENGTH_MAX, szIniFileName );
#endif
            rtn = strtoul( &(configData[id].szValue[2]),NULL,16 );
        }
        else
        {
            nop();
        }
    }
    else
    {
        nop();
    }

    return (DWORD)rtn;
}

/**
 * ------------------------------------------------------------------------------
 * @brief  ������ݒ�l��ۑ�����
 * @param  id
 * @param  ptstrValue
 * @return �Ȃ�
 * @date 2017�N01��04�� 13��50��25�b
 **/
void
ConfigSaveString( CONFIG_ID id, PTSTR ptstrValue )
{
    if( bInitConfig )
    {
        if( (id < CONFIG_ID_MAX) &&
            (configInfoTbl[id].dataType == CONFIG_DATA_STRING) )
        {
            strcpy( configData[id].szValue, ptstrValue );
#ifdef OLD_CONFIG_SAVE_LOAD
            WritePrivateProfileString( GetAppName(), configInfoTbl[id].pKeyName, configData[id].szValue, szIniFileName );
#endif
        }
        else
        {
            nop();
        }
    }
    else
    {
        nop();
    }
}

/**
 * ------------------------------------------------------------------------------
 * @brief  ������ݒ�l��ǂݍ���
 * @param  id
 * @return PTSTR
 * @date 2017�N01��04�� 13��50��25�b
 **/
PTSTR
ConfigLoadString( CONFIG_ID id )
{
    PTSTR rtn = "\0";

    if( bInitConfig )
    {
        if( (id < CONFIG_ID_MAX) &&
            (configInfoTbl[id].dataType == CONFIG_DATA_STRING) )
        {
#ifdef OLD_CONFIG_SAVE_LOAD
            GetPrivateProfileString( GetAppName(), configInfoTbl[id].pKeyName, configInfoTbl[id].pInitValue, configInfoTbl[id].szValue, CONFIG_DATA_STRING_LENGTH_MAX, szIniFileName );
#endif
            rtn = configData[id].szValue;
        }
        else
        {
            nop();
        }
    }
    else
    {
        nop();
    }

    return (DWORD)rtn;
}

/**
 * ------------------------------------------------------------------------------
 * @brief  �ݒ�l��ۑ�����
 * @param  start_id
 * @param  end_id
 * @return �Ȃ�
 * @date 2017�N01��04�� 13��50��25�b
 **/
PTSTR
ConfigWrite( CONFIG_ID start_id, CONFIG_ID end_id )
{
    CONFIG_ID i;

    if(((0<=start_id)&&(start_id<CONFIG_ID_MAX)) &&
       ((0<=end_id  )&&(end_id  <CONFIG_ID_MAX)) )
    {
        end_id = end_id + 1;
    }
    else
    {
        start_id = (CONFIG_ID)0;
        end_id   = (CONFIG_ID)CONFIG_ID_MAX;
    }

    for( i=start_id; i<end_id; i++ )
    {
        WritePrivateProfileString( GetAppName(), configInfoTbl[i].pKeyName, configData[i].szValue, szIniFileName );
    }
}

/**
 * ------------------------------------------------------------------------------
 * @brief  �ݒ�l���ύX����Ă��Ȃ����`�F�b�N����
 * @param  start_id
 * @param  end_id
 * @return
 * @date 2017�N01��04�� 13��50��25�b
 **/
BOOL
ConfigIsSame( CONFIG_ID start_id, CONFIG_ID end_id )
{
    CONFIG_ID i;
    BOOL bRtn = (BOOL)TRUE;

    if(((0<=start_id)&&(start_id<CONFIG_ID_MAX)) &&
       ((0<=end_id  )&&(end_id  <CONFIG_ID_MAX)) )
    {
        end_id = end_id + 1;
    }
    else
    {
        start_id = (CONFIG_ID)0;
        end_id   = (CONFIG_ID)CONFIG_ID_MAX;
    }

    for( i=start_id; i<end_id; i++ )
    {
        TCHAR szValueTemp[CONFIG_DATA_STRING_LENGTH_MAX];

        GetPrivateProfileString( GetAppName(), configInfoTbl[i].pKeyName, configInfoTbl[i].pInitValue, szValueTemp, CONFIG_DATA_STRING_LENGTH_MAX, szIniFileName );
        if( !strcmp( szValueTemp, configData[i].szValue ) )
        {
        }
        else
        {
            bRtn = (BOOL)FALSE;
        }
    }

    return bRtn;
}

/********************************************************************************
 * ���e  : �f�o�b�O�ϐ�(DWORD�l)��ǂݍ���
 * ����  : �Ȃ�
 * �߂�l: INT
 ***************************************/
DWORD
ConfigLoadDebugValue( void )
{
    TCHAR szDword[11];
    DWORD rtn = (DWORD)0;

    if( bInitConfig )
    {
        GetPrivateProfileString( GetAppName(), TEXT("DEBUG"), TEXT("0x00000000"), szDword, 11, szIniFileName );
        rtn = strtoul( szDword+2,NULL,16 );
    }
    else
    {
        nop();
    }

    return (DWORD)rtn;
}
