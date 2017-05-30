#ifndef EDITWND_BUFFER_DATA_TYPE_H

typedef struct tag_buffer_line_data
{
    S_LIST_HEADER header         ;
    DWORD         lineNum        ; /* Y位置            */
    DWORD         caretDataPos   ; /* X位置            */
    DWORD         dataSize       ; /* データサイズ     */
    INT           newLineCodeSize; /* 改行コードサイズ */
    TCHAR         data[]         ;
} S_BUFF_LINE_DATA;

/********************************************************************************
 * 内容  : 行データを追加する
 * 引数  : S_BUFF_LINE_DATA **topPtrPtr 先頭データをつなぐポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA **topPtrPtr 最終データをつなぐポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA *dataPtr つなぐデータ
 * 戻り値: なし
 ***************************************/
void EditWndBufferAddLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr, S_BUFF_LINE_DATA *dataPtr );

/********************************************************************************
 * 内容  : 行データを削除する
 * 引数  : S_BUFF_LINE_DATA **topPtrPtr 先頭データがつないであるポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA **endPtrPtr 最終データをつないであるポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA *dataPtr    削除するデータ
 * 戻り値: S_BUFF_LINE_DATA *           削除した次のデータ
 ***************************************/
void EditWndBufferRemoveLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr, S_BUFF_LINE_DATA *dataPtr );

/********************************************************************************
 * 内容  : 行データの挿入
 * 引数  : S_BUFF_LINE_DATA **topPtrPtr       先頭データをつなぐポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA **endPtrPtr       最終データをつなぐポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA *nowPtr           挿入位置
 * 引数  : S_BUFF_LINE_DATA **insertTopPtrPtr
 * 引数  : S_BUFF_LINE_DATA **insertEndPtrPtr
 * 戻り値: void
 ***************************************/
void EditWndBufferInsertLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr, S_BUFF_LINE_DATA *nowPtr, S_BUFF_LINE_DATA **insertTopPtrPtr, S_BUFF_LINE_DATA **insertEndPtrPtr );

/********************************************************************************
 * 内容  : 行データを置き換える
 * 引数  : S_BUFF_LINE_DATA **topPtrPtr 先頭データをつなぐポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA **endPtrPtr 最終データをつなぐポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA *nowPtr     置き換え対象のデータ
 * 引数  : S_BUFF_LINE_DATA *dataPtr    置き換えるデータ
 * 戻り値: 置き換えたデータ
 ***************************************/
S_BUFF_LINE_DATA * EditWndBufferReplaceLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr, S_BUFF_LINE_DATA *nowPtr, S_BUFF_LINE_DATA *dataPtr );

/********************************************************************************
 * 内容  : バッファデータのクリア
 * 引数  : S_BUFF_LINE_DATA **topPtrPtr 先頭データをつなぐポインタ(のポインタ)
 * 引数  : S_BUFF_LINE_DATA **endPtrPtr 最終データをつなぐポインタ(のポインタ)
 * 戻り値: なし
 ***************************************/
void EditWndBufferAllRemoveLinkedList( S_BUFF_LINE_DATA **topPtrPtr, S_BUFF_LINE_DATA **endPtrPtr );

/********************************************************************************
 * 内容  : 行データの生成
 * 引数  : DWORD size          データサイズ(改行コード含む)
 * 引数  : INT newLineCodeSize 改行コードサイズ
 * 引数  : TCHAR *dataPtr      データ(ポインタ)
 * 引数  : DWORD lineNum       行番号
 * 引数  : DWORD caretPos      キャレット位置
 * 戻り値: S_BUFF_LINE_DATA *
 ***************************************/
S_BUFF_LINE_DATA * BuffLineDataCreate( DWORD size, INT newLineCodeSize, TCHAR *dataPtr, DWORD lineNum, DWORD caretPos );

/********************************************************************************
 * 内容  : 行データの解放
 * 引数  : S_BUFF_LINE_DATA *
 * 戻り値: なし
 ***************************************/
void BuffLineDataDestroy( S_BUFF_LINE_DATA *dataPtr );

/********************************************************************************
 * 内容  : 行データの結合
 * 引数  : S_BUFF_LINE_DATA *data1Ptr (改行コードは削除される)
 * 引数  : S_BUFF_LINE_DATA *data2Ptr
 * 戻り値: S_BUFF_LINE_DATA *
 ***************************************/
S_BUFF_LINE_DATA * BuffLineDataJoin( S_BUFF_LINE_DATA *data1Ptr, S_BUFF_LINE_DATA *data2Ptr );

/********************************************************************************
 * 内容  : 行データの分割
 * 引数  : S_BUFF_LINE_DATA *dataPtr  分割するデータ
 * 引数  : S_BUFF_LINE_DATA **new1Ptr 分割後のデータ1(のポインタ) (メモリ確保する)
 * 引数  : S_BUFF_LINE_DATA **new2Ptr 分割後のデータ2(のポインタ) (メモリ確保する)
 * 戻り値: なし
 ***************************************/
void BuffLineDataDivide( S_BUFF_LINE_DATA *dataPtr, S_BUFF_LINE_DATA **new1PtrPtr, S_BUFF_LINE_DATA **new2PtrPtr );

/********************************************************************************
 * 内容  : 行データを短くする
 * 引数  : S_BUFF_LINE_DATA *dataPtr
 * 引数  : DWORD size
 * 戻り値: S_BUFF_LINE_DATA *
 ***************************************/
S_BUFF_LINE_DATA * BuffLineDataShorten( S_BUFF_LINE_DATA *dataPtr, DWORD size );

/********************************************************************************
 * 内容  : 行番号の更新
 * 引数  : S_BUFF_LINE_DATA *dataPtr
 * 戻り値: なし
 ***************************************/
void BuffLineDataUpdateLineNumAfter( S_BUFF_LINE_DATA *dataPtr );

#define EDITWND_BUFFER_DATA_TYPE_H
#endif /* EDITWND_BUFFER_DATA_TYPE_H */
