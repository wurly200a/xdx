#ifndef MODAL_DLG_H

/* 管理する設定のID */
typedef enum
{
    MODAL_DLG_ID_ABOUT,
    MODAL_DLG_ID_MAX
}MODAL_DLG_ID;

typedef struct
{
    DWORD dwData1;
} S_MODAL_DLG_DATA;

/********************************************************************************
 * 内容  : モーダルダイアログボックス初期化
 * 引数  : なし
 * 戻り値: BOOL
 ***************************************/
BOOL ModalDlgInit( void );

/********************************************************************************
 * 内容  : モーダルダイアログボックス
 * 引数  : MODAL_DLG_ID      id
 * 引数  : S_MODAL_DLG_DATA *dataPtr
 * 引数  : HWND              hwnd
 * 引数  : int               x
 * 引数  : int               y
 * 戻り値: BOOL
 ***************************************/
BOOL ModalDlg( MODAL_DLG_ID id, S_MODAL_DLG_DATA *dataPtr, HWND hwnd, int x, int y );

#define MODAL_DLG_H
#endif /* MODAL_DLG_H */
