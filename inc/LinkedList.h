#ifndef LINKEDLIST_H

typedef struct tagS_LIST
{
    struct tagS_LIST *prevPtr;
    struct tagS_LIST *nextPtr;
} S_LIST_HEADER;

/********************************************************************************
 * 内容  : 連結リストにデータを追加する
 * 引数  : S_LIST_HEADER **topPtr 先頭データをつなぐポインタ
 * 引数  : S_LIST_HEADER **topPtr 最終データをつなぐポインタ
 * 引数  : S_LIST_HEADER *dataPtr つなぐデータ
 * 戻り値: なし
 ***************************************/
void AddLinkedList( S_LIST_HEADER **topPtr, S_LIST_HEADER **endPtr, S_LIST_HEADER *dataPtr );

/********************************************************************************
 * 内容  : 連結リストのデータを置き換える
 * 引数  : S_LIST_HEADER **topPtr 先頭データをつなぐポインタ
 * 引数  : S_LIST_HEADER **topPtr 最終データをつなぐポインタ
 * 引数  : S_LIST_HEADER *nowPtr  置き換え対象のデータ
 * 引数  : S_LIST_HEADER *dataPtr 置き換えるデータ
 * 戻り値: 置き換えたデータ
 ***************************************/
S_LIST_HEADER *ReplaceLinkedList( S_LIST_HEADER **topPtr, S_LIST_HEADER **endPtr, S_LIST_HEADER *nowPtr, S_LIST_HEADER *dataPtr );

/********************************************************************************
 * 内容  : 連結リストからデータを削除する
 * 引数  : S_LIST_HEADER **topPtr 先頭データがつないであるポインタ
 * 引数  : S_LIST_HEADER **topPtr 最終データをつないであるポインタ
 * 引数  : S_LIST_HEADER *dataPtr 削除するデータ
 * 戻り値: S_LIST_HEADER *        削除した次のデータ
 ***************************************/
S_LIST_HEADER *RemoveLinkedList( S_LIST_HEADER **topPtr, S_LIST_HEADER **endPtr, S_LIST_HEADER *dataPtr );

/********************************************************************************
 * 内容  : 連結リストへの挿入
 * 引数  : S_LIST_HEADER **topPtr       先頭データをつなぐポインタ
 * 引数  : S_LIST_HEADER **topPtr       最終データをつなぐポインタ
 * 引数  : S_LIST_HEADER *nowPtr        挿入位置
 * 引数  : S_LIST_HEADER **insertTopPtr 挿入する連結リストの先頭
 * 引数  : S_LIST_HEADER **insertEndPtr 挿入する連結リストの最後
 * 戻り値: void
 ***************************************/
void InsertLinkedList( S_LIST_HEADER **topPtr, S_LIST_HEADER **endPtr, S_LIST_HEADER *nowPtr, S_LIST_HEADER **insertTopPtr, S_LIST_HEADER **insertEndPtr );

#define LINKEDLIST_H
#endif /* LINKEDLIST_H */

