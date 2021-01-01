/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */
#include "MenuId.h"
#include "MainWndMenu.h"

/* �O���֐���` */
/* �O���ϐ���` */
/* �����֐���` */
/* �����ϐ���` */
HMENU hMenu;

static const ACCEL accelTbl[] =
{
    { (FVIRTKEY|FNOINVERT|FCONTROL),'N'       ,IDM_FILE_NEW        },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'O'       ,IDM_FILE_OPEN       },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'S'       ,IDM_FILE_SAVE       },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'P'       ,IDM_FILE_PRINT      },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'Z'       ,IDM_EDIT_UNDO       },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'X'       ,IDM_EDIT_CUT        },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'C'       ,IDM_EDIT_COPY       },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'V'       ,IDM_EDIT_PASTE      },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'F'       ,IDM_EDIT_FIND       },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'H'       ,IDM_EDIT_REPLACE    },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'G'       ,IDM_EDIT_GOTO_LINE  },
    { (FVIRTKEY|FNOINVERT|FCONTROL),'A'       ,IDM_EDIT_SELECT_ALL },
    { (FVIRTKEY|FNOINVERT)         ,VK_F5     ,IDM_EDIT_DATETIME   },
};

/********************************************************************************
 * ���e  : ���j���[�̐���
 * ����  : �Ȃ�
 * �߂�l: HMENU
 ***************************************/
HMENU
MenuCreate( void )
{
    HMENU hMenuPopup,hMenuPopup2;

    hMenu = CreateMenu();

    hMenuPopup = CreateMenu();
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_FILE_NEW        , TEXT("�V�K(&N)\tCtrl+N") );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_FILE_OPEN       , TEXT("�J��(&O)...\tCtrl+O") );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_FILE_SAVE       , TEXT("�㏑���ۑ�(&S)\tCtrl+S") );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_FILE_SAVE_AS    , TEXT("���O��t���ĕۑ�(&A)...") );
//    AppendMenu( hMenuPopup, MF_SEPARATOR, 0                   , NULL );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_FILE_PAGE       , TEXT("�y�[�W�ݒ�(&U)...") );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_FILE_PRINT      , TEXT("���(&P)...\tCtrl+P") );
//    AppendMenu( hMenuPopup, MF_SEPARATOR, 0                   , NULL );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_FILE_EXIT       , TEXT("�I��(&X)") );
//    AppendMenu( hMenu     , MF_POPUP    , (UINT_PTR)hMenuPopup, TEXT("�t�@�C��(&F)") );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_FILE_EXIT       , TEXT("E&xit") );
    AppendMenu( hMenu     , MF_POPUP    , (UINT_PTR)hMenuPopup, TEXT("&File") );

//    EnableMenuItem( hMenu, IDM_FILE_PAGE      , MF_GRAYED );
//    EnableMenuItem( hMenu, IDM_FILE_PRINT     , MF_GRAYED );

//    hMenuPopup = CreateMenu();
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_EDIT_UNDO       , TEXT("���ɖ߂�(&U)\tCtrl+Z") );
//    AppendMenu( hMenuPopup, MF_SEPARATOR, 0                   , NULL );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_EDIT_CUT        , TEXT("�؂���(&T)\tCtrl+X") );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_EDIT_COPY       , TEXT("�R�s�[(&C)\tCtrl+C") );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_EDIT_PASTE      , TEXT("�\��t��(&P)\tCtrl+V") );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_EDIT_DELETE     , TEXT("�폜(&L)\tDel") );
//    AppendMenu( hMenuPopup, MF_SEPARATOR, 0                   , NULL );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_EDIT_FIND       , TEXT("����(&F)...\tCtrl+F") );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_EDIT_FIND_NEXT  , TEXT("��������(&N)\tF3") );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_EDIT_REPLACE    , TEXT("�u��(&R)...\tCtrl+H") );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_EDIT_GOTO_LINE  , TEXT("�s�ֈړ�(&G)...\tCtrl+G") );
//    AppendMenu( hMenuPopup, MF_SEPARATOR, 0                   , NULL );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_EDIT_SELECT_ALL , TEXT("���ׂđI��(&A)\tCtrl+A") );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_EDIT_DATETIME   , TEXT("���t�Ǝ���(&D)\tF5") );
//    AppendMenu( hMenu     , MF_POPUP    , (UINT_PTR)hMenuPopup, TEXT("�ҏW(&E)") );

#if 0
    EnableMenuItem( hMenu, IDM_EDIT_UNDO       , MF_GRAYED );
    EnableMenuItem( hMenu, IDM_EDIT_FIND       , MF_GRAYED );
    EnableMenuItem( hMenu, IDM_EDIT_FIND_NEXT  , MF_GRAYED );
    EnableMenuItem( hMenu, IDM_EDIT_REPLACE    , MF_GRAYED );
#endif
//    EnableMenuItem( hMenu, IDM_EDIT_GOTO_LINE  , MF_GRAYED );
//
//    hMenuPopup = CreateMenu();
//    AppendMenu( hMenuPopup, MF_STRING, IDM_FORMAT_WRAP     , TEXT("�E�[�Ő܂�Ԃ�(&W)") );
//    AppendMenu( hMenuPopup, MF_STRING, IDM_FORMAT_FONT     , TEXT("�t�H���g(&F)...") );
//    AppendMenu( hMenu     , MF_POPUP , (UINT_PTR)hMenuPopup, TEXT("����(&O)") );
//
//    EnableMenuItem( hMenu, IDM_FORMAT_WRAP , MF_GRAYED );

    hMenuPopup2 = CreateMenu();
    AppendMenu( hMenuPopup2, MF_STRING, IDM_ORDER_DESCENDING, TEXT("Descending(4,3,2,1 or 6,5,4,3,2,1)") );
    AppendMenu( hMenuPopup2, MF_STRING, IDM_ORDER_ASCENDING , TEXT("Ascending(1,2,3,4 or 1,2,3,4,5,6)") );
    hMenuPopup = CreateMenu();
    AppendMenu( hMenuPopup, MF_POPUP, (UINT_PTR)hMenuPopup2, TEXT("Operator Display Order") );
    AppendMenu( hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, TEXT("&Settings") );

//    hMenuPopup2 = CreateMenu();
//    AppendMenu( hMenuPopup2, MF_STRING, IDM_EXTEND_NEWLINE_CRLF, TEXT("CR+LF") );
//    AppendMenu( hMenuPopup2, MF_STRING, IDM_EXTEND_NEWLINE_LF  , TEXT("LF") );
//    AppendMenu( hMenuPopup2, MF_STRING, IDM_EXTEND_NEWLINE_CR  , TEXT("CR") );
//    hMenuPopup = CreateMenu();
//    AppendMenu( hMenuPopup, MF_POPUP, (UINT_PTR)hMenuPopup2, TEXT("���s�R�[�h(&N)") );
//    AppendMenu( hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, TEXT("�g��(&X)") );

    hMenuPopup = CreateMenu();
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_HELP_HELP  , TEXT("�g�s�b�N�̌���(&H)") );
//    AppendMenu( hMenuPopup, MF_SEPARATOR, 0              , NULL );
//    AppendMenu( hMenuPopup, MF_STRING   , IDM_HELP_ABOUT , TEXT("�o�[�W�������(&A)") );
//    AppendMenu( hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, TEXT("�w���v(&H)") );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_HELP_ABOUT , TEXT("&About...") );
    AppendMenu( hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, TEXT("&Help") );

    EnableMenuItem( hMenu, IDM_HELP_HELP  , MF_GRAYED );

    return hMenu;
}

/********************************************************************************
 * ���e  : �A�N�Z�����[�^�̐���
 * ����  : �Ȃ�
 * �߂�l: HACCEL
 ***************************************/
HACCEL
AccelCreate( void )
{
    return CreateAcceleratorTable((LPACCEL)accelTbl,(int)(sizeof(accelTbl)/sizeof(ACCEL)));
}

/********************************************************************************
 * ���e  : �w��̍��ڂ��`�F�b�N����Ă��邩�₢���킹
 * ����  : UINT itemID
 * �߂�l: BOOL
 ***************************************/
BOOL
MenuInqItemChecked( UINT itemID )
{
    INT iMenuFlags;
    BOOL rtn = FALSE;

    if( hMenu != NULL )
    {
        iMenuFlags = GetMenuState( hMenu, itemID, MF_BYCOMMAND );
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
MenuCheckItem( UINT itemID )
{
    if( hMenu != NULL )
    {
        CheckMenuItem( hMenu, itemID, MF_CHECKED );
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
MenuUnCheckItem( UINT itemID )
{
    if( hMenu != NULL )
    {
        CheckMenuItem( hMenu, itemID, MF_UNCHECKED );
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
MenuEnableItem( UINT itemID )
{
    if( hMenu != NULL )
    {
        EnableMenuItem( hMenu, itemID, MF_ENABLED );
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
MenuUnEnableItem( UINT itemID )
{
    if( hMenu != NULL )
    {
        EnableMenuItem( hMenu, itemID, MF_GRAYED );
    }
    else
    {
        nop();
    }
}
