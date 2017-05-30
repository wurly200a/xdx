/* 共通インクルードファイル */
#include "common.h"

/* 個別インクルードファイル */
#include "LinkedList.h"
#include "EditWndBufferDataType.h"

/* 外部関数定義 */

/* 外部変数定義 */

/* 内部関数定義 */

/********************************************************************************
 * 内容  : 行データを追加する
 * 引数  : S_BUFF_LINE_DATA **topPtrPtr 先頭データをつなぐポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA **topPtrPtr 最終データをつなぐポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA *dataPtr つなぐデータ
 * 戻り値: なし
 ***************************************/
void
EditWndBufferAddLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr, S_BUFF_LINE_DATA *dataPtr )
{
    if( *topPtrPtr == NULL )
    {
        dataPtr->lineNum = 0;
    }
    else
    {
        if( *endPtrPtr != NULL )
        {
            dataPtr->lineNum = (*endPtrPtr)->lineNum+1;
        }
        else
        {
            nop(); /* 異常 */
        }
    }

    AddLinkedList( (S_LIST_HEADER **)topPtrPtr, (S_LIST_HEADER **)endPtrPtr, (S_LIST_HEADER *)dataPtr );
}

/********************************************************************************
 * 内容  : 行データを削除する
 * 引数  : S_BUFF_LINE_DATA **topPtrPtr 先頭データがつないであるポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA **endPtrPtr 最終データをつないであるポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA *dataPtr    削除するデータ
 * 戻り値: S_BUFF_LINE_DATA *           削除した次のデータ
 ***************************************/
void
EditWndBufferRemoveLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr, S_BUFF_LINE_DATA *dataPtr )
{
    S_BUFF_LINE_DATA *removeNextPtr;

    removeNextPtr = (S_BUFF_LINE_DATA *)RemoveLinkedList( (S_LIST_HEADER **)topPtrPtr,(S_LIST_HEADER **)endPtrPtr,(S_LIST_HEADER *)dataPtr );
    if( removeNextPtr != NULL )
    {
        if( removeNextPtr->header.prevPtr != NULL )
        {
            BuffLineDataUpdateLineNumAfter( (S_BUFF_LINE_DATA *)removeNextPtr->header.prevPtr );
        }
        else
        {
            removeNextPtr->lineNum = 0;
            BuffLineDataUpdateLineNumAfter( removeNextPtr );
        }
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * 内容  : 行データの挿入
 * 引数  : S_BUFF_LINE_DATA **topPtrPtr       先頭データをつなぐポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA **endPtrPtr       最終データをつなぐポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA *nowPtr           挿入位置
 * 引数  : S_BUFF_LINE_DATA **insertTopPtrPtr
 * 引数  : S_BUFF_LINE_DATA **insertEndPtrPtr
 * 戻り値: void
 ***************************************/
void
EditWndBufferInsertLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr, S_BUFF_LINE_DATA *nowPtr, S_BUFF_LINE_DATA **insertTopPtrPtr, S_BUFF_LINE_DATA **insertEndPtrPtr )
{
    if( (nowPtr != NULL) && (insertTopPtrPtr != NULL) )
    {
        (*insertTopPtrPtr)->lineNum = nowPtr->lineNum;

        InsertLinkedList((S_LIST_HEADER **)topPtrPtr,(S_LIST_HEADER **)endPtrPtr,(S_LIST_HEADER *)nowPtr,(S_LIST_HEADER **)insertTopPtrPtr,(S_LIST_HEADER **)insertEndPtrPtr);
        BuffLineDataUpdateLineNumAfter( (*insertTopPtrPtr) );
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * 内容  : 行データを置き換える
 * 引数  : S_BUFF_LINE_DATA **topPtrPtr 先頭データをつなぐポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA **endPtrPtr 最終データをつなぐポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA *nowPtr     置き換え対象のデータ
 * 引数  : S_BUFF_LINE_DATA *dataPtr    置き換えるデータ
 * 戻り値: 置き換えたデータ
 ***************************************/
S_BUFF_LINE_DATA *
EditWndBufferReplaceLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr, S_BUFF_LINE_DATA *nowPtr, S_BUFF_LINE_DATA *dataPtr )
{
    if( (topPtrPtr != NULL) && (endPtrPtr != NULL) && (nowPtr != NULL) && (dataPtr != NULL) )
    {
        dataPtr->lineNum = nowPtr->lineNum;
        dataPtr = (S_BUFF_LINE_DATA *)ReplaceLinkedList((S_LIST_HEADER **)topPtrPtr,(S_LIST_HEADER **)endPtrPtr,(S_LIST_HEADER *)nowPtr,(S_LIST_HEADER *)dataPtr);
    }
    else
    {
        nop();
    }

    return dataPtr;
}

/********************************************************************************
 * 内容  : バッファデータのクリア
 * 引数  : S_BUFF_LINE_DATA **topPtrPtr 先頭データをつなぐポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA **endPtrPtr 最終データをつなぐポインタ(のポインタ)
 * 戻り値: なし
 ***************************************/
void
EditWndBufferAllRemoveLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr )
{
    S_BUFF_LINE_DATA *lineDataPtr,*nextPtr;

    lineDataPtr = *topPtrPtr;
    while( lineDataPtr != NULL )
    {
        nextPtr = (S_BUFF_LINE_DATA *)RemoveLinkedList((S_LIST_HEADER **)topPtrPtr,(S_LIST_HEADER **)endPtrPtr,(S_LIST_HEADER *)lineDataPtr);
        BuffLineDataDestroy(lineDataPtr);
        lineDataPtr = nextPtr;
    }
}

/********************************************************************************
 * 内容  : 行データの生成
 * 引数  : DWORD size          データサイズ(改行コード含む)
 * 引数  : INT newLineCodeSize 改行コードサイズ
 * 引数  : TCHAR *dataPtr      データ(ポインタ)
 * 引数  : DWORD lineNum       行番号
 * 引数  : DWORD caretPos      キャレット位置
 * 戻り値: S_BUFF_LINE_DATA *
 ***************************************/
S_BUFF_LINE_DATA *
BuffLineDataCreate( DWORD size, INT newLineCodeSize, TCHAR *dataPtr, DWORD lineNum, DWORD caretPos )
{
    S_BUFF_LINE_DATA *newPtr = NULL;

    newPtr = (S_BUFF_LINE_DATA *)malloc( sizeof(S_BUFF_LINE_DATA) + (size * sizeof(TCHAR)) + 1 );
    if( newPtr != NULL )
    {
        memset( newPtr, sizeof(S_BUFF_LINE_DATA), 0 );
        newPtr->lineNum         = lineNum;
        newPtr->caretDataPos    = caretPos;
        newPtr->dataSize        = size;
        newPtr->newLineCodeSize = newLineCodeSize;

        if( dataPtr != NULL )
        {
            memcpy( newPtr->data, dataPtr, size );
        }
        else
        {
            nop();
        }
        newPtr->data[size] = '\0';
    }
    else
    {
        nop();
    }

    return newPtr;
}

/********************************************************************************
 * 内容  : 行データの解放
 * 引数  : S_BUFF_LINE_DATA *
 * 戻り値: なし
 ***************************************/
void
BuffLineDataDestroy( S_BUFF_LINE_DATA *dataPtr )
{
    free( dataPtr );
}

/********************************************************************************
 * 内容  : 行データの結合
 * 引数  : S_BUFF_LINE_DATA *data1Ptr (改行コードは削除される)
 * 引数  : S_BUFF_LINE_DATA *data2Ptr
 * 戻り値: S_BUFF_LINE_DATA *
 ***************************************/
S_BUFF_LINE_DATA *
BuffLineDataJoin( S_BUFF_LINE_DATA *data1Ptr, S_BUFF_LINE_DATA *data2Ptr )
{
    S_BUFF_LINE_DATA *newPtr = NULL;

    if( (data1Ptr != NULL) && (data2Ptr != NULL) )
    {
        newPtr = BuffLineDataCreate( data1Ptr->dataSize-data1Ptr->newLineCodeSize+data2Ptr->dataSize, data2Ptr->newLineCodeSize, data1Ptr->data, data1Ptr->lineNum, 0 );
        if( newPtr != NULL )
        {
            memcpy( newPtr->data + data1Ptr->dataSize - data1Ptr->newLineCodeSize, data2Ptr->data, data2Ptr->dataSize );
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

    return newPtr;
}

/********************************************************************************
 * 内容  : 行データの分割
 * 引数  : S_BUFF_LINE_DATA *dataPtr  分割するデータ
 * 引数  : S_BUFF_LINE_DATA **new1Ptr 分割後のデータ1(のポインタ) (メモリ確保する)
 * 引数  : S_BUFF_LINE_DATA **new2Ptr 分割後のデータ2(のポインタ) (メモリ確保する)
 * 戻り値: なし
 ***************************************/
void
BuffLineDataDivide( S_BUFF_LINE_DATA *dataPtr, S_BUFF_LINE_DATA **new1PtrPtr, S_BUFF_LINE_DATA **new2PtrPtr )
{
    if( (dataPtr != NULL) && (new1PtrPtr != NULL) && (new2PtrPtr != NULL) )
    {
        *new1PtrPtr = BuffLineDataCreate( dataPtr->caretDataPos                  , 0                       , dataPtr->data                  , dataPtr->lineNum  , dataPtr->caretDataPos ); /* 改行より前 */
        *new2PtrPtr = BuffLineDataCreate( dataPtr->dataSize-dataPtr->caretDataPos, dataPtr->newLineCodeSize, dataPtr->data+dataPtr->caretDataPos, dataPtr->lineNum+1, 0                 ); /* 改行より後 */
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * 内容  : 行データを短くする
 * 引数  : S_BUFF_LINE_DATA *dataPtr
 * 引数  : DWORD size
 * 戻り値: S_BUFF_LINE_DATA *
 ***************************************/
S_BUFF_LINE_DATA *
BuffLineDataShorten( S_BUFF_LINE_DATA *dataPtr, DWORD size )
{
    S_BUFF_LINE_DATA *newPtr = NULL;

    if( (dataPtr != NULL) &&
        (dataPtr->dataSize >= size) )
    {
        newPtr = BuffLineDataCreate( dataPtr->dataSize-size, dataPtr->newLineCodeSize, dataPtr->data, dataPtr->lineNum, dataPtr->caretDataPos );
        if( newPtr != NULL )
        {
            /* 改行コードのコピー */
            memcpy( newPtr->data + newPtr->dataSize - newPtr->newLineCodeSize, dataPtr->data + dataPtr->dataSize - dataPtr->newLineCodeSize, dataPtr->newLineCodeSize );
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

    return newPtr;
}

/********************************************************************************
 * 内容  : 行番号の更新
 * 引数  : S_BUFF_LINE_DATA *dataPtr
 * 戻り値: なし
 ***************************************/
void
BuffLineDataUpdateLineNumAfter( S_BUFF_LINE_DATA *dataPtr )
{
    S_BUFF_LINE_DATA *nowPtr;
    DWORD i;

    if( dataPtr != NULL )
    {
        i = dataPtr->lineNum;

        for( nowPtr=dataPtr; (nowPtr != NULL); nowPtr=(S_BUFF_LINE_DATA *)nowPtr->header.nextPtr,i++ )
        {
            nowPtr->lineNum = i;
        }
    }
    else
    {
        nop();
    }
}
