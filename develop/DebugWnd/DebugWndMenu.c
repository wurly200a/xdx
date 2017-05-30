/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */
#include "DebugMenuId.h"
#include "DebugWndMenu.h"

/* 外部関数定義 */
/* 外部変数定義 */
/* 内部関数定義 */
/* 内部変数定義 */
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
 * 内容  : メニューの生成
 * 引数  : なし
 * 戻り値: HMENU
 ***************************************/
HMENU
DebugMenuCreate( void )
{
    HMENU hMenuPopup;

    hDebugMenu = CreateMenu();

    hMenuPopup = CreateMenu();
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_FILE_NEW        , TEXT("新規(&N)\tCtrl+N") );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_FILE_SAVE_AS    , TEXT("名前を付けて保存(&A)...") );
    AppendMenu( hMenuPopup, MF_SEPARATOR, 0                   , NULL );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_FILE_PAGE       , TEXT("ページ設定(&U)...") );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_FILE_PRINT      , TEXT("印刷(&P)...\tCtrl+P") );
    AppendMenu( hMenuPopup, MF_SEPARATOR, 0                   , NULL );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_FILE_EXIT       , TEXT("終了(&X)") );
    AppendMenu( hDebugMenu     , MF_POPUP    , (UINT_PTR)hMenuPopup, TEXT("ファイル(&F)") );

    EnableMenuItem( hDebugMenu, IDM_DEBUG_FILE_PAGE      , MF_GRAYED );
    EnableMenuItem( hDebugMenu, IDM_DEBUG_FILE_PRINT     , MF_GRAYED );

    hMenuPopup = CreateMenu();
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_EDIT_COPY       , TEXT("コピー(&C)\tCtrl+C") );
    AppendMenu( hMenuPopup, MF_SEPARATOR, 0                   , NULL );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_EDIT_FIND       , TEXT("検索(&F)...\tCtrl+F") );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_EDIT_FIND_NEXT  , TEXT("次を検索(&N)\tF3") );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_EDIT_GOTO_LINE  , TEXT("行へ移動(&G)...\tCtrl+G") );
    AppendMenu( hMenuPopup, MF_SEPARATOR, 0                   , NULL );
    AppendMenu( hMenuPopup, MF_STRING   , IDM_DEBUG_EDIT_SELECT_ALL , TEXT("すべて選択(&A)\tCtrl+A") );
    AppendMenu( hDebugMenu     , MF_POPUP    , (UINT_PTR)hMenuPopup, TEXT("編集(&E)") );

#if 0
    EnableMenuItem( hDebugMenu, IDM_DEBUG_EDIT_UNDO       , MF_GRAYED );
    EnableMenuItem( hDebugMenu, IDM_DEBUG_EDIT_FIND       , MF_GRAYED );
    EnableMenuItem( hDebugMenu, IDM_DEBUG_EDIT_FIND_NEXT  , MF_GRAYED );
    EnableMenuItem( hDebugMenu, IDM_DEBUG_EDIT_REPLACE    , MF_GRAYED );
#endif
    EnableMenuItem( hDebugMenu, IDM_DEBUG_EDIT_GOTO_LINE  , MF_GRAYED );

    hMenuPopup = CreateMenu();
    AppendMenu( hMenuPopup, MF_STRING, IDM_DEBUG_FORMAT_FONT     , TEXT("フォント(&F)...") );
    AppendMenu( hDebugMenu     , MF_POPUP , (UINT_PTR)hMenuPopup, TEXT("書式(&O)") );

    return hDebugMenu;
}

/********************************************************************************
 * 内容  : アクセラレータの生成
 * 引数  : なし
 * 戻り値: HACCEL
 ***************************************/
HACCEL
DebugAccelCreate( void )
{
    return CreateAcceleratorTable((LPACCEL)debugAccelTbl,(int)(sizeof(debugAccelTbl)/sizeof(ACCEL)));
}

/********************************************************************************
 * 内容  : 指定の項目がチェックされているか問い合わせ
 * 引数  : UINT itemID
 * 戻り値: BOOL
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
 * 内容  : 指定の項目のチェックを付ける
 * 引数  : UINT itemID
 * 戻り値: なし
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
 * 内容  : 指定の項目のチェックを外す
 * 引数  : UINT itemID
 * 戻り値: なし
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
 * 内容  : 指定の項目を有効にする
 * 引数  : UINT itemID
 * 戻り値: なし
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
 * 内容  : 指定の項目を無効にする
 * 引数  : UINT itemID
 * 戻り値: なし
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
