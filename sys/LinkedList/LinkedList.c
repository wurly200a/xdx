/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */

/* 外部関数定義 */
#include "WinMain.h"

/* 外部変数定義 */

/* 内部関数定義 */
#include "LinkedList.h"

/********************************************************************************
 * 内容  : 連結リストにデータを追加する
 * 引数  : S_LIST_HEADER **topPtr 先頭データをつなぐポインタ
 * 引数  : S_LIST_HEADER **topPtr 最終データをつなぐポインタ
 * 引数  : S_LIST_HEADER *dataPtr つなぐデータ
 * 戻り値: なし
 ***************************************/
void
AddLinkedList( S_LIST_HEADER **topPtr, S_LIST_HEADER **endPtr, S_LIST_HEADER *dataPtr )
{
    if( *topPtr == NULL )
    {
        dataPtr->prevPtr = NULL;
        dataPtr->nextPtr = NULL;
        *topPtr = dataPtr;
        *endPtr = dataPtr;
    }
    else
    {
        if( *endPtr != NULL )
        {
            dataPtr->prevPtr   = *endPtr;
            dataPtr->nextPtr   = NULL;
            (*endPtr)->nextPtr = dataPtr;
            *endPtr            = dataPtr;
        }
        else
        {
            nop(); /* 異常 */
        }
    }
}

/********************************************************************************
 * 内容  : 連結リストのデータを置き換える
 * 引数  : S_LIST_HEADER **topPtr 先頭データをつなぐポインタ
 * 引数  : S_LIST_HEADER **topPtr 最終データをつなぐポインタ
 * 引数  : S_LIST_HEADER *nowPtr  置き換え対象のデータ
 * 引数  : S_LIST_HEADER *dataPtr 置き換えるデータ
 * 戻り値: 置き換えたデータ
 ***************************************/
S_LIST_HEADER *
ReplaceLinkedList( S_LIST_HEADER **topPtr, S_LIST_HEADER **endPtr, S_LIST_HEADER *nowPtr, S_LIST_HEADER *dataPtr )
{
    if( (topPtr != NULL) && (endPtr != NULL) && (nowPtr != NULL) && (dataPtr != NULL) )
    {
        dataPtr->prevPtr = nowPtr->prevPtr;
        dataPtr->nextPtr = nowPtr->nextPtr;

        if( dataPtr->prevPtr != NULL )
        {
            (dataPtr->prevPtr)->nextPtr = dataPtr;
        }
        else
        {
            *(topPtr) = dataPtr;
        }
        if( dataPtr->nextPtr != NULL )
        {
            (dataPtr->nextPtr)->prevPtr = dataPtr;
        }
        else
        {
            *(endPtr) = dataPtr;
        }
    }
    else
    {
        nop();
    }

    return dataPtr;
}

/********************************************************************************
 * 内容  : 連結リストからデータを削除する
 * 引数  : S_LIST_HEADER **topPtr 先頭データがつないであるポインタ
 * 引数  : S_LIST_HEADER **topPtr 最終データをつないであるポインタ
 * 引数  : S_LIST_HEADER *dataPtr 削除するデータ
 * 戻り値: S_LIST_HEADER *        削除した次のデータ
 ***************************************/
S_LIST_HEADER *
RemoveLinkedList( S_LIST_HEADER **topPtr, S_LIST_HEADER **endPtr, S_LIST_HEADER *dataPtr )
{
    S_LIST_HEADER *nextPtr = NULL;

    if( (topPtr != NULL) && (endPtr != NULL) && (dataPtr != NULL) )
    {
        nextPtr = dataPtr->nextPtr; /* 戻り値 */

        if( dataPtr->prevPtr != NULL )
        { /* 前データ有り */
            (dataPtr->prevPtr)->nextPtr = dataPtr->nextPtr; /* 次データを、前データの次データとする */
            if( (dataPtr->prevPtr)->nextPtr != NULL )
            { /* 次データがあった場合 */
                ((dataPtr->prevPtr)->nextPtr)->prevPtr = dataPtr->prevPtr; /* 次データの前データは、前データ */
            }
            else
            { /* 次データ無し */
                (*endPtr) = dataPtr->prevPtr; /* 前データが最後のデータとなる */
            }
        }
        else
        { /* 前データ無し */
            *topPtr = dataPtr->nextPtr;
            if( *topPtr != NULL )
            { /* 次データがあった場合 */
                (*topPtr)->prevPtr = NULL;
            }
            else
            { /* 次データ無し */
                (*endPtr) = NULL; /* データは全部無くなった */
            }
        }
    }
    else
    {
        nop();
    }

    return nextPtr;
}

/********************************************************************************
 * 内容  : 連結リストへの挿入
 * 引数  : S_LIST_HEADER **topPtr       先頭データをつなぐポインタ
 * 引数  : S_LIST_HEADER **topPtr       最終データをつなぐポインタ
 * 引数  : S_LIST_HEADER *nowPtr        挿入位置
 * 引数  : S_LIST_HEADER **insertTopPtr 挿入する連結リストの先頭
 * 引数  : S_LIST_HEADER **insertEndPtr 挿入する連結リストの最後
 * 戻り値: void
 ***************************************/
void
InsertLinkedList( S_LIST_HEADER **topPtr, S_LIST_HEADER **endPtr, S_LIST_HEADER *nowPtr, S_LIST_HEADER **insertTopPtr, S_LIST_HEADER **insertEndPtr )
{
    if( (topPtr != NULL) && (endPtr != NULL) && (nowPtr != NULL) && (insertTopPtr != NULL) && (insertEndPtr != NULL) )
    {
        (*insertTopPtr)->prevPtr = nowPtr->prevPtr;

        if( (*insertTopPtr)->prevPtr != NULL )
        {
            ((*insertTopPtr)->prevPtr)->nextPtr = (*insertTopPtr);
        }
        else
        {
            (*topPtr) = (*insertTopPtr);
        }

        (*insertEndPtr)->nextPtr = nowPtr;
        nowPtr->prevPtr = (*insertEndPtr);
    }
    else
    {
        nop();
    }
}
