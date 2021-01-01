/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */
#include "DebugMenuId.h"
#include "DebugWndMenu.h"

/* �O���֐���` */
/* �O���ϐ���` */
/* �����֐���` */
/* �����ϐ���` */
HMENU hDebugMenu;

static const ACCEL debugAccelTbl[] =
{
    { (FVIRTKEY|FNOINVERT|FCONTROL),'N'       ,IDM_DEBUG_FILE_NEW        },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'S'       ,IDM_DEBUG_FILE_SAVE_AS    },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'P'       ,IDM_DEBUG_FILE_PRINT      },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'C'       ,IDM_DEBUG_EDIT_COPY       },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'F'       ,IDM_DEBUG_EDIT_FIND       },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'G'       ,IDM_DEBUG_EDIT_GOTO_LINE  },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'A'       ,IDM_DEBUG_EDIT_SELECT_ALL },
};

/********************************************************************************
 * ���e  : ���j���[�̐���
 * ����  : �Ȃ�
 * �߂�l: HMENU
 ***************************************/
HMENU
DebugMenuCreate( void )
{
    HMENU hMenuPopup;

    hDebugMenu = CreateMenu();

    hMenuPopup = CreateMenu();
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_FILE_NEW        , TEXT("�V�K(&N)\tCtrl+N") );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_FILE_SAVE_AS    , TEXT("���O��t���ĕۑ�(&A)...") );
    AppendMenu( hMenuPopup, MF_SEPARATOR, 0                   , NULL );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_FILE_PAGE       , TEXT("�y�[�W�ݒ�(&U)...") );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_FILE_PRINT      , TEXT("���(&P)...\tCtrl+P") );
    AppendMenu( hMenuPopup, MF_SEPARATOR, 0                   , NULL );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_FILE_EXIT       , TEXT("�I��(&X)") );
    AppendMenu( hDebugMenu     , MF_POPUP    , (UINT_PTR)hMenuPopup, TEXT("�t�@�C��(&F)") );

    EnableMenuItem( hDebugMenu, IDM_DEBUG_FILE_PAGE      , MF_GRAYED );
    EnableMenuItem( hDebugMenu, IDM_DEBUG_FILE_PRINT     , MF_GRAYED );

    hMenuPopup = CreateMenu();
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_EDIT_COPY       , TEXT("�R�s�[(&C)\tCtrl+C") );
    AppendMenu( hMenuPopup, MF_SEPARATOR, 0                   , NULL );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_EDIT_FIND       , TEXT("����(&F)...\tCtrl+F") );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_EDIT_FIND_NEXT  , TEXT("��������(&N)\tF3") );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_EDIT_GOTO_LINE  , TEXT("�s�ֈړ�(&G)...\tCtrl+G") );
    AppendMenu( hMenuPopup, MF_SEPARATOR, 0                   , NULL );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_EDIT_SELECT_ALL , TEXT("���ׂđI��(&A)\tCtrl+A") );
    AppendMenu( hDebugMenu     , MF_POPUP    , (UINT_PTR)hMenuPopup, TEXT("�ҏW(&E)") );

#if 0
    EnableMenuItem( hDebugMenu, IDM_DEBUG_EDIT_UNDO       , MF_GRAYED );
    EnableMenuItem( hDebugMenu, IDM_DEBUG_EDIT_FIND       , MF_GRAYED );
    EnableMenuItem( hDebugMenu, IDM_DEBUG_EDIT_FIND_NEXT  , MF_GRAYED );
    EnableMenuItem( hDebugMenu, IDM_DEBUG_EDIT_REPLACE    , MF_GRAYED );
#endif
    EnableMenuItem( hDebugMenu, IDM_DEBUG_EDIT_GOTO_LINE  , MF_GRAYED );

    hMenuPopup = CreateMenu();
    AppendMenu( hMenuPopup, MF_STRING, IDM_DEBUG_FORMAT_FONT     , TEXT("�t�H���g(&F)...") );
    AppendMenu( hDebugMenu     , MF_POPUP , (UINT_PTR)hMenuPopup, TEXT("����(&O)") );

    return hDebugMenu;
}

/********************************************************************************
 * ���e  : �A�N�Z�����[�^�̐���
 * ����  : �Ȃ�
 * �߂�l: HACCEL
 ***************************************/
HACCEL
DebugAccelCreate( void )
{
    return CreateAcceleratorTable((LPACCEL)debugAccelTbl,(int)(sizeof(debugAccelTbl)/sizeof(ACCEL)));
}

/********************************************************************************
 * ���e  : �w��̍��ڂ��`�F�b�N����Ă��邩�₢���킹
 * ����  : UINT itemID
 * �߂�l: BOOL
 ***************************************/
BOOL
DebugMenuInqItemChecked( UINT itemID )
{
    INT iMenuFlags;
    BOOL rtn = FALSE;

    if( hDebugMenu != NULL )
    {
        iMenuFlags = GetMenuState( hDebugMenu, itemID, MF_BYCOMMAND );
        if( iMenuFlags & MF_CHECKED )
        {
            rtn = TRUE;
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

    return rtn;
}

/********************************************************************************
 * ���e  : �w��̍��ڂ̃`�F�b�N��t����
 * ����  : UINT itemID
 * �߂�l: �Ȃ�
 ***************************************/
void
DebugMenuCheckItem( UINT itemID )
{
    if( hDebugMenu != NULL )
    {
        CheckMenuItem( hDebugMenu, itemID, MF_CHECKED );
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * ���e  : �w��̍��ڂ̃`�F�b�N���O��
 * ����  : UINT itemID
 * �߂�l: �Ȃ�
 ***************************************/
void
DebugMenuUnCheckItem( UINT itemID )
{
    if( hDebugMenu != NULL )
    {
        CheckMenuItem( hDebugMenu, itemID, MF_UNCHECKED );
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * ���e  : �w��̍��ڂ�L���ɂ���
 * ����  : UINT itemID
 * �߂�l: �Ȃ�
 ***************************************/
void
DebugMenuEnableItem( UINT itemID )
{
    if( hDebugMenu != NULL )
    {
        EnableMenuItem( hDebugMenu, itemID, MF_ENABLED );
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * ���e  : �w��̍��ڂ𖳌��ɂ���
 * ����  : UINT itemID
 * �߂�l: �Ȃ�
 ***************************************/
void
DebugMenuUnEnableItem( UINT itemID )
{
    if( hDebugMenu != NULL )
    {
        EnableMenuItem( hDebugMenu, itemID, MF_GRAYED );
    }
    else
    {
        nop();
    }
}
