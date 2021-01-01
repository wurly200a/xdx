/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */

/* �O���֐���` */

/* �O���ϐ���` */
/* �����֐���` */
#include "Config.h"
/* �����ϐ���` */

typedef struct
{
    HINSTANCE hInstance;
    PTSTR     szAppName;
    BOOL      bInitConfig;
    TCHAR     szIniFileName[512];
} S_CONFIG__DATA;

static S_CONFIG__DATA configData;

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
    { TEXT("bSaveConfirm"         ),TEXT("0x00000000"), CONFIG_DATA_DWORD  },
    { TEXT("iWindowPosX"          ),TEXT("0x00000000"), CONFIG_DATA_DWORD  },
    { TEXT("iWindowPosY"          ),TEXT("0x00000000"), CONFIG_DATA_DWORD  },
    { TEXT("iWindowPosDX"         ),TEXT("0x000004C5"), CONFIG_DATA_DWORD  },
    { TEXT("iWindowPosDY"         ),TEXT("0x000002A0"), CONFIG_DATA_DWORD  },
    { TEXT("iMidiInNum"           ),TEXT("0x00000000"), CONFIG_DATA_DWORD  },
    { TEXT("iMidiOutNum"          ),TEXT("0x00000000"), CONFIG_DATA_DWORD  },
    { TEXT("iMidiKeyInNum"        ),TEXT("0x00000000"), CONFIG_DATA_DWORD  },
    { TEXT("Dx7OneVoiceDataDir"   ),TEXT("")          , CONFIG_DATA_STRING },
    { TEXT("Dx7AllVoiceDataDir"   ),TEXT("")          , CONFIG_DATA_STRING },
    { TEXT("Dx100OneVoiceDataDir" ),TEXT("")          , CONFIG_DATA_STRING },
    { TEXT("Dx100AllVoiceDataDir" ),TEXT("")          , CONFIG_DATA_STRING },
};

#define CONFIG_DATA_STRING_LENGTH_MAX 512
typedef struct
{
    TCHAR szValue[CONFIG_DATA_STRING_LENGTH_MAX];
} S_CONFIG_PARAM;

S_CONFIG_PARAM configParam[CONFIG_ID_MAX];

/********************************************************************************
 * ���e  : �ݒ�Ǘ����W���[���̏�����
 * ����  : HINSTANCE hInst
 * ����  : PTSTR szAppName
 * �߂�l: �Ȃ�
 ***************************************/
void
ConfigInit( HINSTANCE hInst, PTSTR szAppName )
{
    DWORD length;
    int i;

    configData.bInitConfig = TRUE;
    configData.hInstance   = hInst;
    configData.szAppName   = szAppName;

    length = GetModuleFileName(NULL,configData.szIniFileName,512);
    configData.szIniFileName[length-1] = 'i';
    configData.szIniFileName[length-2] = 'n';
    configData.szIniFileName[length-3] = 'i';

    for( i=0; i<CONFIG_ID_MAX; i++ )
    {
        GetPrivateProfileString( szAppName, configInfoTbl[i].pKeyName, configInfoTbl[i].pInitValue, configParam[i].szValue, CONFIG_DATA_STRING_LENGTH_MAX, configData.szIniFileName );
    }
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
    if( configData.bInitConfig )
    {
        if( (id < CONFIG_ID_MAX) &&
            (configInfoTbl[id].dataType == CONFIG_DATA_DWORD) )
        {
            wsprintf( configParam[id].szValue, "0x%08lX", data );
#ifdef OLD_CONFIG_SAVE_LOAD
            WritePrivateProfileString( configData.szAppName, configInfoTbl[id].pKeyName, configParam[id].szValue, configData.szIniFileName );
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

    if( configData.bInitConfig )
    {
        if( (id < CONFIG_ID_MAX) &&
            (configInfoTbl[id].dataType == CONFIG_DATA_DWORD) )
        {
#ifdef OLD_CONFIG_SAVE_LOAD
            GetPrivateProfileString( configData.szAppName, configInfoTbl[id].pKeyName, configInfoTbl[id].pInitValue, configInfoTbl[id].szValue, CONFIG_DATA_STRING_LENGTH_MAX, configData.szIniFileName );
#endif
            rtn = strtoul( &(configParam[id].szValue[2]),NULL,16 );
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
    if( configData.bInitConfig )
    {
        if( (id < CONFIG_ID_MAX) &&
            (configInfoTbl[id].dataType == CONFIG_DATA_STRING) )
        {
            strcpy( configParam[id].szValue, ptstrValue );
#ifdef OLD_CONFIG_SAVE_LOAD
            WritePrivateProfileString( configData.szAppName, configInfoTbl[id].pKeyName, configParam[id].szValue, configData.szIniFileName );
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

    if( configData.bInitConfig )
    {
        if( (id < CONFIG_ID_MAX) &&
            (configInfoTbl[id].dataType == CONFIG_DATA_STRING) )
        {
#ifdef OLD_CONFIG_SAVE_LOAD
            GetPrivateProfileString( configData.szAppName, configInfoTbl[id].pKeyName, configInfoTbl[id].pInitValue, configInfoTbl[id].szValue, CONFIG_DATA_STRING_LENGTH_MAX, configData.szIniFileName );
#endif
            rtn = configParam[id].szValue;
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
        WritePrivateProfileString( configData.szAppName, configInfoTbl[i].pKeyName, configParam[i].szValue, configData.szIniFileName );
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

        GetPrivateProfileString( configData.szAppName, configInfoTbl[i].pKeyName, configInfoTbl[i].pInitValue, szValueTemp, CONFIG_DATA_STRING_LENGTH_MAX, configData.szIniFileName );
        if( !strcmp( szValueTemp, configParam[i].szValue ) )
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

    if( configData.bInitConfig )
    {
        GetPrivateProfileString( configData.szAppName, TEXT("DEBUG"), TEXT("0x00000000"), szDword, 11, configData.szIniFileName );
        rtn = strtoul( szDword+2,NULL,16 );
    }
    else
    {
        nop();
    }

    return (DWORD)rtn;
}
