/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */
#include "DebugWnd.h"
#include "LinkedList.h"
#include "EditWndBufferDataType.h"

/* 外部関数定義 */

/* 外部変数定義 */

/* 内部関数定義 */
#include "EditWndBuffer.h"

typedef struct tagS_EDITWND_BUFF_LOCAL
{
    struct
    {
        S_BUFF_LINE_DATA *topPtr       ; /* 先頭のデータ                           */
        S_BUFF_LINE_DATA *endPtr       ; /* 最後のデータ                           */
        S_BUFF_LINE_DATA *nowPtr       ; /* キャレットがあるデータ                 */
        S_BUFF_LINE_DATA *selectPtr    ; /* 選択範囲の先頭のデータ                 */
        DWORD            selectCaretPos; /* 選択範囲の先頭のデータのキャレット位置 */
    } lineData;
    UINT  NewLineType;
    DWORD xCaret     ;
    DWORD yCaret     ;
    INT   tabSize    ;
} S_EDITWND_BUFF_LOCAL;
typedef S_EDITWND_BUFF_LOCAL * H_EDITWND_BUFF_LOCAL;

static S_BUFF_LINE_DATA *getBuffLine( H_EDITWND_BUFF_LOCAL h, TCHAR *dataPtr, DWORD maxLength );
static CHARSET_TYPE detectCharSet( S_BUFF_LINE_DATA *dataPtr, DWORD offset );
static DWORD getCaretDispXpos( H_EDITWND_BUFF_LOCAL h, S_BUFF_LINE_DATA *linePtr, DWORD dataPos );
static BOOL getDispCharData( H_EDITWND_BUFF_LOCAL h, S_BUFF_LINE_DATA *linePtr, DWORD dispPos, S_BUFF_DISP_DATA *dataPtr );
static void setSelectPosNowPosToFar( H_EDITWND_BUFF_LOCAL h, BOOL bMinus, DWORD offset );

/* 内部変数定義 */

#if 0
#define EDITWNDBUFF_DEBUG_PRINTF DebugWndPrintf
#else
#define EDITWNDBUFF_DEBUG_PRINTF
#endif

/********************************************************************************
 * 内容  : EDITウィンドウバッファのオブジェクト生成
 * 引数  : なし
 * 戻り値: H_EDITWND_BUFF
 ***************************************/
H_EDITWND_BUFF
EditWndBuffCreate( void )
{
    H_EDITWND_BUFF_LOCAL h;

    h = (H_EDITWND_BUFF_LOCAL)malloc( sizeof(S_EDITWND_BUFF_LOCAL) );
    if( h != NULL )
    {
        h->lineData.topPtr         = NULL;
        h->lineData.endPtr         = NULL;
        h->lineData.nowPtr         = NULL;
        h->lineData.selectPtr      = NULL;
        h->lineData.selectCaretPos = 0;
        h->NewLineType = EDITWND_BUFF_NEWLINE_CRLF;
        h->xCaret      = 0;
        h->yCaret      = 0;
        h->tabSize     = 8;
    }
    else
    {
        nop();
    }

    return (H_EDITWND_BUFF)h;
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファのオブジェクト破棄
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 戻り値: BOOL
 ***************************************/
BOOL
EditWndBuffDestroy( H_EDITWND_BUFF hEditWndBuff )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( (h != NULL) )
    {
        free( h );
    }
    else
    {
        nop();
    }

    return TRUE;
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファの初期化
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 戻り値: なし
 ***************************************/
void
EditWndBuffInit( H_EDITWND_BUFF hEditWndBuff )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;
    S_BUFF_LINE_DATA *lineDataPtr;

    EditWndBufferAllRemoveLinkedList(&(h->lineData.topPtr),&(h->lineData.endPtr));

    /* 空データを追加 */
    lineDataPtr = BuffLineDataCreate( 0, 0, NULL, 0, 0 );
    if( lineDataPtr != NULL )
    {
        EditWndBufferAddLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),lineDataPtr );
        (h->lineData.nowPtr) = lineDataPtr;
    }
    else
    {
        nop();
    }

    (h->lineData.selectPtr) = NULL;
    (h->lineData.selectCaretPos) = 0;
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファの終了
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 戻り値: なし
 ***************************************/
void
EditWndBuffEnd( H_EDITWND_BUFF hEditWndBuff )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    EditWndBufferAllRemoveLinkedList(&(h->lineData.topPtr),&(h->lineData.endPtr));
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファのデータセット
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 引数  : TCHAR* dataPtr
 * 引数  : DWORD  length
 * 引数  : BOOL   bInit  (TRUE:既存データをクリア,FALSE:クリアしない)
 * 戻り値: なし
 ***************************************/
void
EditWndBuffDataSet( H_EDITWND_BUFF hEditWndBuff, TCHAR* dataPtr, DWORD length, BOOL bInit )
{
    S_BUFF_LINE_DATA *lineDataPtr;
    S_BUFF_LINE_DATA *tempTopPtr = NULL;
    S_BUFF_LINE_DATA *tempEndPtr = NULL;
    S_BUFF_LINE_DATA *newPtr,*targetPtr;
    S_BUFF_LINE_DATA *dividePrePtr,*dividePostPtr;
    DWORD lineLengthSum = 0;
    DWORD caretPos = 0;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( bInit )
    {
        EditWndBuffInit(hEditWndBuff);
    }
    else
    {
        nop();
    }

    if( (dataPtr != NULL) && (length > 0) )
    { /* データ有りの場合 */
        EDITWNDBUFF_DEBUG_PRINTF("EditWndBuffDataSet,");

        /* 改行で分割したデータを仮連結リスト(tempTopPtr～tempEndPtr)に登録(ここから) */
        while( lineLengthSum < length )
        {
            lineDataPtr = (S_BUFF_LINE_DATA *)getBuffLine( h, dataPtr+lineLengthSum, length - lineLengthSum );

            if( lineDataPtr != NULL )
            {
                EditWndBufferAddLinkedList( &tempTopPtr,&tempEndPtr,lineDataPtr );
                lineLengthSum += lineDataPtr->dataSize;
            }
            else
            {
                break;
            }
        }
        /* 改行で分割したデータを仮連結リスト(tempTopPtr～tempEndPtr)に登録(ここまで) */

        if( (h->lineData.nowPtr)->caretDataPos == 0 )
        { /* 行の先頭に挿入 */
            EDITWNDBUFF_DEBUG_PRINTF("Top,");
            EditWndBufferInsertLinkedList(&(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),&tempTopPtr,&tempEndPtr);

            if( tempEndPtr->newLineCodeSize == 0 )
            { /* 挿入データの最後に改行がなければ */
                newPtr = BuffLineDataJoin(tempEndPtr,(h->lineData.nowPtr)); /* 追加データの最終行と現在の行と結合した新たなデータ生成 */
                if( newPtr != NULL )
                {
                    targetPtr = (h->lineData.nowPtr);
                    (h->lineData.nowPtr) = EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),newPtr ); /* 現在の行を置き換え */
                    BuffLineDataDestroy( targetPtr );                                                       /* 現在の行のデータを解放                   */

                    caretPos = tempEndPtr->dataSize - tempEndPtr->newLineCodeSize;           /* キャレット位置は追加データの最終行の最後 */
                    EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),tempEndPtr); /* 最終行自体は不要なので連結リストから外す */
                    BuffLineDataDestroy( tempEndPtr );

                    (h->lineData.nowPtr) = newPtr;                                            /* 結合したものを現在行とする */
                    (h->lineData.nowPtr)->caretDataPos = caretPos;                                /* キャレット位置は結合位置     */
                }
                else
                {
                    nop();
                }
            }
            else
            { /* 挿入データの最後に改行があった */
                (h->lineData.nowPtr)->caretDataPos = 0;
            }
        }
        else
        { /* 行の途中に挿入 */
            /* 現在の行をキャレット位置で分割(ここから) */
            BuffLineDataDivide( (h->lineData.nowPtr), &dividePrePtr, &dividePostPtr );
            if( dividePrePtr != NULL )
            {
                if( dividePostPtr != NULL )
                {
                    EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),dividePostPtr ); /* まず 古いデータを改行以降のデータに置き換え */
                    EditWndBufferInsertLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),dividePostPtr,&dividePrePtr,&dividePrePtr ); /* その前に改行前のデータを挿入 */

                    BuffLineDataDestroy( (h->lineData.nowPtr) );
                    (h->lineData.nowPtr) = dividePostPtr;
                    (h->lineData.nowPtr)->caretDataPos = 0;
                    /* 現在の行をキャレット位置で分割(ここまで) */

                    if( tempTopPtr == tempEndPtr )
                    { /* 1行挿入の場合 */
                        EditWndBufferInsertLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),&tempEndPtr,&tempEndPtr );

                        /* 分割した前方データと、挿入データの先頭を結合(ここから) */
                        newPtr = BuffLineDataJoin( dividePrePtr, tempTopPtr );
                        if( newPtr != NULL )
                        {
                            targetPtr = tempTopPtr;
                            EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),tempTopPtr,newPtr );
                            BuffLineDataDestroy( targetPtr );
                            EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),dividePrePtr);
                            BuffLineDataDestroy( dividePrePtr );
                        }
                        else
                        {
                            nop();
                        }
                        tempTopPtr = tempEndPtr = newPtr;

                        if( tempEndPtr->newLineCodeSize == 0 )
                        { /* 挿入データの最後に改行がなければ */
                            newPtr = BuffLineDataJoin(tempEndPtr,dividePostPtr); /* 追加データの最終行と改行以降の行と結合 */
                            if( newPtr != NULL )
                            {
                                newPtr->caretDataPos = tempEndPtr->dataSize - tempEndPtr->newLineCodeSize;
                                EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),dividePostPtr,newPtr );
                                BuffLineDataDestroy( dividePostPtr );
                                (h->lineData.nowPtr) = newPtr;
                                EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),tempEndPtr);
                                BuffLineDataDestroy( tempEndPtr );
                            }
                            else
                            {
                                nop();
                            }
                        }
                        else
                        { /* 挿入データの最後に改行有り */
                            nop();
                        }
                    }
                    else
                    { /* 複数行挿入の場合 */
                        /* 分割した位置にデータを挿入(ここから) */
                        EditWndBufferInsertLinkedList(&(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),&tempTopPtr,&tempEndPtr);

                        if( tempEndPtr->newLineCodeSize == 0 )
                        { /* 挿入データの最後に改行がなければ */
                            newPtr = BuffLineDataJoin(tempEndPtr,dividePostPtr); /* 追加データの最終行と改行以降の行と結合 */
                            if( newPtr != NULL )
                            {
                                newPtr->caretDataPos = tempEndPtr->dataSize - tempEndPtr->newLineCodeSize;
                                EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),dividePostPtr,newPtr );
                                BuffLineDataDestroy( dividePostPtr );
                                (h->lineData.nowPtr) = newPtr;
                                EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),tempEndPtr);
                                BuffLineDataDestroy( tempEndPtr );
                            }
                            else
                            {
                                nop();
                            }
                        }
                        else
                        { /* 挿入データの最後に改行有り */
                            nop();
                        }

                        /* 分割した前方データと、挿入データの先頭を結合(ここから) */
                        newPtr = BuffLineDataJoin( dividePrePtr, tempTopPtr );
                        if( newPtr != NULL )
                        {
                            targetPtr = tempTopPtr;
                            EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),tempTopPtr,newPtr );
                            BuffLineDataDestroy( targetPtr );
                            EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),dividePrePtr);
                            BuffLineDataDestroy( dividePrePtr );
                        }
                        else
                        {
                            nop();
                        }
                    }
                }
                else
                {
                    BuffLineDataDestroy( dividePrePtr );
                }
            }
            else
            {
                nop();
            }
        }

        EDITWNDBUFF_DEBUG_PRINTF("0x%02X,%d\r\n",(BYTE)*dataPtr,length);
    }
    else
    { /* データ無しの場合 */
        nop();
    }

    if( bInit )
    {
        if( (h->lineData.topPtr) != NULL )
        {
            (h->lineData.nowPtr) = (h->lineData.topPtr);
            (h->lineData.nowPtr)->caretDataPos = 0;
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
 * 内容  : EDITウィンドウバッファのデータ取得
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 引数  : TCHAR *dataPtr
 * 引数  : DWORD dataSize
 * 引数  : EDITWND_BUFF_REGION region
 * 戻り値: BOOL
 ***************************************/
BOOL
EditWndBuffDataGet( H_EDITWND_BUFF hEditWndBuff, TCHAR *dataPtr, DWORD dataSize, EDITWND_BUFF_REGION region )
{
    BOOL rtn = FALSE;
    S_BUFF_LINE_DATA *nowPtr,*prevPtr;
    DWORD size;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( region == BUFF_ALL )
    {
        if( (h->lineData.topPtr) == NULL )
        {
            nop();
        }
        else
        {
            if( dataPtr != NULL )
            {
                for( nowPtr=(h->lineData.topPtr); nowPtr != NULL; nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr )
                {
                    memcpy( dataPtr, nowPtr->data, nowPtr->dataSize );
                    dataPtr += nowPtr->dataSize;
                }
            }
            else
            {
                nop();
            }
            rtn = TRUE;
        }
    }
    else
    {
        if( dataPtr != NULL )
        {
            if( (h->lineData.selectPtr) != NULL )
            { /* 選択開始位置有り */
                if( ((h->lineData.selectPtr)->lineNum) < ((h->lineData.nowPtr)->lineNum) )
                { /* 正方向に選択 */
                    for( nowPtr = (h->lineData.selectPtr),prevPtr = NULL; prevPtr != (h->lineData.nowPtr); nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr )
                    {
                        if( nowPtr == (h->lineData.selectPtr) )
                        {
                            size = (nowPtr->dataSize - (h->lineData.selectCaretPos));
                            memcpy( dataPtr, (nowPtr->data + (h->lineData.selectCaretPos)),size );
                            dataPtr += size;
                        }
                        else if( nowPtr == (h->lineData.nowPtr) )
                        {
                            size = nowPtr->caretDataPos;
                            memcpy( dataPtr,nowPtr->data,size );
                            dataPtr += size;
                        }
                        else
                        {
                            size = nowPtr->dataSize;
                            memcpy( dataPtr, nowPtr->data,size );
                            dataPtr += size;
                        }
                        prevPtr = nowPtr;
                    }
                }
                else if( ((h->lineData.selectPtr)->lineNum) > ((h->lineData.nowPtr)->lineNum) )
                { /* 負方向に選択 */
                    for( nowPtr = (h->lineData.nowPtr),prevPtr = NULL; prevPtr != (h->lineData.selectPtr); nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr )
                    {
                        if( nowPtr == (h->lineData.nowPtr) )
                        {
                            size = (nowPtr->dataSize - nowPtr->caretDataPos);
                            memcpy( dataPtr, (nowPtr->data + nowPtr->caretDataPos),size );
                            dataPtr += size;
                        }
                        else if( nowPtr == (h->lineData.selectPtr) )
                        {
                            size = (h->lineData.selectCaretPos);
                            memcpy( dataPtr,nowPtr->data,size );
                            dataPtr += size;
                        }
                        else
                        {
                            size = nowPtr->dataSize;
                            memcpy( dataPtr, nowPtr->data,size );
                            dataPtr += size;
                        }
                        prevPtr = nowPtr;
                    }
                }
                else
                { /* 同一行内で選択 */
                    if( (h->lineData.nowPtr) == (h->lineData.selectPtr) )
                    {
                        if( (h->lineData.nowPtr)->caretDataPos < (h->lineData.selectCaretPos) )
                        {
                            size = ((h->lineData.selectCaretPos) - (h->lineData.nowPtr)->caretDataPos);
                            memcpy( dataPtr, ((h->lineData.nowPtr)->data + (h->lineData.nowPtr)->caretDataPos),size );
                            dataPtr += size;
                        }
                        else if( (h->lineData.nowPtr)->caretDataPos > (h->lineData.selectCaretPos) )
                        {
                            size = ((h->lineData.nowPtr)->caretDataPos - (h->lineData.selectCaretPos));
                            memcpy( dataPtr, ((h->lineData.nowPtr)->data + (h->lineData.selectCaretPos)),size );
                            dataPtr += size;
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

    return rtn;
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファのデータサイズ取得
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 引数  : EDITWND_BUFF_REGION region
 * 戻り値: DWORD
 ***************************************/
DWORD
EditWndBuffGetDataSize( H_EDITWND_BUFF hEditWndBuff, EDITWND_BUFF_REGION region )
{
    DWORD dataSize = 0;
    S_BUFF_LINE_DATA *nowPtr,*prevPtr;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( region == BUFF_ALL )
    {
        for( nowPtr = (h->lineData.topPtr); nowPtr != NULL; nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr )
        {
            dataSize += nowPtr->dataSize;
        }
    }
    else
    {
        if( (h->lineData.selectPtr) != NULL )
        { /* 選択開始位置有り */
            if( ((h->lineData.selectPtr)->lineNum) < ((h->lineData.nowPtr)->lineNum) )
            { /* 正方向に選択 */
                for( nowPtr = (h->lineData.selectPtr),prevPtr = NULL; prevPtr != (h->lineData.nowPtr); nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr )
                {
                    if( nowPtr == (h->lineData.selectPtr) )
                    {
                        dataSize += (nowPtr->dataSize - (h->lineData.selectCaretPos));
                    }
                    else if( nowPtr == (h->lineData.nowPtr) )
                    {
                        dataSize += nowPtr->caretDataPos;
                    }
                    else
                    {
                        dataSize += nowPtr->dataSize;
                    }
                    prevPtr = nowPtr;
                }
            }
            else if( ((h->lineData.selectPtr)->lineNum) > ((h->lineData.nowPtr)->lineNum) )
            { /* 負方向に選択 */
                for( nowPtr = (h->lineData.nowPtr),prevPtr = NULL; prevPtr != (h->lineData.selectPtr); nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr )
                {
                    if( nowPtr == (h->lineData.nowPtr) )
                    {
                        dataSize += (nowPtr->dataSize - nowPtr->caretDataPos);
                    }
                    else if( nowPtr == (h->lineData.selectPtr) )
                    {
                        dataSize += (h->lineData.selectCaretPos);
                    }
                    else
                    {
                        dataSize += nowPtr->dataSize;
                    }
                    prevPtr = nowPtr;
                }
            }
            else
            { /* 同一行内で選択 */
                if( (h->lineData.nowPtr) == (h->lineData.selectPtr) )
                {
                    if( (h->lineData.nowPtr)->caretDataPos < (h->lineData.selectCaretPos) )
                    {
                        dataSize += ((h->lineData.selectCaretPos) - (h->lineData.nowPtr)->caretDataPos);
                    }
                    else if( (h->lineData.nowPtr)->caretDataPos > (h->lineData.selectCaretPos) )
                    {
                        dataSize += ((h->lineData.nowPtr)->caretDataPos - (h->lineData.selectCaretPos));
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
        }
        else
        {
            nop();
        }
    }

    return dataSize;
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファの最大行サイズ取得
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 戻り値: DWORD
 ***************************************/
DWORD
EditWndBuffGetLineMaxSize( H_EDITWND_BUFF hEditWndBuff )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    return (h->lineData.endPtr)->lineNum;
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファの最大文字サイズ取得
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 戻り値: DWORD
 ***************************************/
DWORD
EditWndBuffGetColumnMaxSize( H_EDITWND_BUFF hEditWndBuff)
{
    DWORD columnMaxSize = 0;
    S_BUFF_LINE_DATA *nowPtr;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    for( nowPtr = (h->lineData.topPtr); nowPtr != NULL; nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr )
    {
        columnMaxSize = max( columnMaxSize, (nowPtr->dataSize - nowPtr->newLineCodeSize) );
    }

    return columnMaxSize;
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファのキャレットX位置取得
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 戻り値: DWORD
 ***************************************/
DWORD
EditWndBuffGetCaretXpos( H_EDITWND_BUFF hEditWndBuff )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    return getCaretDispXpos(h,(h->lineData.nowPtr),(h->lineData.nowPtr)->caretDataPos);
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファのキャレットY位置取得
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 戻り値: DWORD
 ***************************************/
DWORD
EditWndBuffGetCaretYpos( H_EDITWND_BUFF hEditWndBuff )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    return (h->lineData.nowPtr)->lineNum;
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファのキャレット位置セット
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 引数  : DWORD xPos
 * 引数  : DWORD lineNum
 * 戻り値: BOOL (TRUE:キャレット位置が変わった,FALSE:変わらない)
 ***************************************/
BOOL
EditWndBuffSetCaretPos( H_EDITWND_BUFF hEditWndBuff, DWORD xPos, DWORD lineNum )
{
    S_BUFF_LINE_DATA *nowPtr = NULL;
    S_BUFF_DISP_DATA dispData;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;
    BOOL bCaretPosChange = FALSE;

    nowPtr = (S_BUFF_LINE_DATA *)EditWndBuffGetLinePtr( hEditWndBuff, lineNum );

    if( nowPtr != NULL )
    {
        if( (h->lineData.nowPtr) != nowPtr )
        {
            (h->lineData.nowPtr) = nowPtr;
            bCaretPosChange = TRUE;
        }
        else
        {
            nop();
        }

        getDispCharData( h,(h->lineData.nowPtr), xPos, &dispData );

        if( (h->lineData.nowPtr)->caretDataPos != dispData.caretPos )
        {
            (h->lineData.nowPtr)->caretDataPos = dispData.caretPos;
            bCaretPosChange = TRUE;
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

    return bCaretPosChange;
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファのキャレットX位置加算
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 戻り値: なし
 ***************************************/
void
EditWndBuffIncCaretXpos( H_EDITWND_BUFF hEditWndBuff )
{
    int moveAmount = 0;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( (h->lineData.nowPtr) != NULL )
    {
        if( detectCharSet((h->lineData.nowPtr),(h->lineData.nowPtr)->caretDataPos) == DOUBLE_CHAR_HIGH )
        { /* 次の文字で移動量を判断 */
            moveAmount = 2;
        }
        else
        {
            moveAmount = 1;
        }

        if( ((h->lineData.nowPtr)->caretDataPos + moveAmount) > ((h->lineData.nowPtr)->dataSize - (h->lineData.nowPtr)->newLineCodeSize) )
        { /* キャレットはすでに行の最右端 */
            if( ((h->lineData.nowPtr)->header.nextPtr != NULL) )
            { /* 次行有り */
                (h->lineData.nowPtr) = (S_BUFF_LINE_DATA *)(h->lineData.nowPtr)->header.nextPtr;
                (h->lineData.nowPtr)->caretDataPos = 0;
            }
            else
            { /* 次行無し */
                nop();
            }
        }
        else
        { /* まだ右へ移動可能 */
            (h->lineData.nowPtr)->caretDataPos += moveAmount;
        }
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファのキャレットX位置減算
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 戻り値: なし
 ***************************************/
void
EditWndBuffDecCaretXpos( H_EDITWND_BUFF hEditWndBuff )
{
    int moveAmount = 0;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( (h->lineData.nowPtr) != NULL )
    {
        if( ((h->lineData.nowPtr)->caretDataPos>0) &&
            (detectCharSet((h->lineData.nowPtr),(h->lineData.nowPtr)->caretDataPos-1) == DOUBLE_CHAR_LOW) )
        { /* 前の文字で移動量を判断 */
            moveAmount = 2;
        }
        else
        {
            moveAmount = 1;
        }

        if( (h->lineData.nowPtr)->caretDataPos == 0 )
        { /* キャレットはすでに行の最左端 */
            if( ((h->lineData.nowPtr)->header.prevPtr != NULL) )
            { /* 前行有り */
                (h->lineData.nowPtr) = (S_BUFF_LINE_DATA *)(h->lineData.nowPtr)->header.prevPtr;
                (h->lineData.nowPtr)->caretDataPos = (h->lineData.nowPtr)->dataSize - (h->lineData.nowPtr)->newLineCodeSize;
            }
            else
            { /* 前行無し */
                nop();
            }
        }
        else
        { /* まだ左へ移動可能 */
            (h->lineData.nowPtr)->caretDataPos = max( (h->lineData.nowPtr)->caretDataPos - moveAmount, 0 );
        }
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファのキャレットY位置加算
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 戻り値: なし
 ***************************************/
void
EditWndBuffIncCaretYpos( H_EDITWND_BUFF hEditWndBuff )
{
    DWORD preDispPos = 0;
    S_BUFF_LINE_DATA *nextPtr;
    S_BUFF_DISP_DATA dispData;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    nextPtr = (S_BUFF_LINE_DATA *)((h->lineData.nowPtr)->header.nextPtr);

    if( ((h->lineData.nowPtr) != NULL) && (nextPtr != NULL) )
    {
        preDispPos = getCaretDispXpos(h,(h->lineData.nowPtr),(h->lineData.nowPtr)->caretDataPos);
        getDispCharData( h, nextPtr, preDispPos, &dispData );
        nextPtr->caretDataPos = dispData.caretPos;
        (h->lineData.nowPtr) = nextPtr;
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファのキャレットY位置減算
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 戻り値: なし
 ***************************************/
void
EditWndBuffDecCaretYpos( H_EDITWND_BUFF hEditWndBuff )
{
    DWORD preDispPos = 0;
    S_BUFF_LINE_DATA *prevPtr;
    S_BUFF_DISP_DATA dispData;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    prevPtr = (S_BUFF_LINE_DATA *)(h->lineData.nowPtr)->header.prevPtr;

    if( ((h->lineData.nowPtr) != NULL) && (prevPtr != NULL) )
    {
        preDispPos = getCaretDispXpos(h,(h->lineData.nowPtr),(h->lineData.nowPtr)->caretDataPos);
        getDispCharData( h, prevPtr, preDispPos, &dispData );
        prevPtr->caretDataPos = dispData.caretPos;
        (h->lineData.nowPtr) = prevPtr;
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * 内容  : 指定行データのハンドル取得
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 引数  : DWORD lineNum
 * 戻り値: H_EDITWND_BUFF_LINE
 ***************************************/
H_EDITWND_BUFF_LINE
EditWndBuffGetLinePtr( H_EDITWND_BUFF hEditWndBuff, DWORD lineNum )
{
    S_BUFF_LINE_DATA *nowPtr,*nextPtr,*rtnPtr = NULL;
    DWORD i;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( (h->lineData.topPtr) == NULL )
    {
        nop();
    }
    else
    {
        for( i=0,nowPtr = nextPtr = (h->lineData.topPtr); (nowPtr != NULL) && (i<=lineNum); nowPtr=nextPtr,i++ )
        {
            nextPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr;

            if( i == lineNum )
            {
                rtnPtr = nowPtr;
                break;
            }
            else
            {
                nop();
            }
        }
    }

    return (H_EDITWND_BUFF_LINE)rtnPtr;
}

/********************************************************************************
 * 内容  : 次の行データのハンドル取得
 * 引数  : H_EDITWND_BUFF_LINE hLineData
 * 戻り値: H_EDITWND_BUFF_LINE
 ***************************************/
H_EDITWND_BUFF_LINE
EditWndBuffGetLineNextPtr( H_EDITWND_BUFF_LINE hLineData )
{
    return (H_EDITWND_BUFF_LINE)(((S_BUFF_LINE_DATA *)hLineData)->header.nextPtr);
}

/********************************************************************************
 * 内容  : 指定行、指定列のデータを取得
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 引数  : H_EDITWND_BUFF_LINE hLineData
 * 引数  : DWORD             dispPos     表示位置
 * 引数  : S_BUFF_DISP_DATA *dataPtr
 * 戻り値: BOOL
 ***************************************/
BOOL
EditWndBuffGetDispData( H_EDITWND_BUFF hEditWndBuff, H_EDITWND_BUFF_LINE hLineData, DWORD dispPos, S_BUFF_DISP_DATA *dataPtr )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    getDispCharData( h, (S_BUFF_LINE_DATA *)hLineData, dispPos, dataPtr );

    return TRUE;
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファの改行コードセット
 * 引数  : UINT newLineType
 * 戻り値: なし
 ***************************************/
void
EditWndBuffSetNewLineCode( H_EDITWND_BUFF hEditWndBuff, UINT newLineType )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    h->NewLineType = newLineType;
}

/********************************************************************************
 * 内容  : データ削除
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 引数  : BOOL bBackSpace
 * 戻り値: なし
 ***************************************/
void
EditWndBuffRemoveData( H_EDITWND_BUFF hEditWndBuff, BOOL bBackSpace )
{
    DWORD removeSize = 0,saveCaretPos;
    S_BUFF_LINE_DATA *newPtr = NULL,*prevPtr = NULL,*nextPtr = NULL,*nowPtr = NULL,*savePtr;
    S_BUFF_LINE_DATA *dividePrePtr,*dividePostPtr;
    BOOL bNormalDelete = FALSE;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    EDITWNDBUFF_DEBUG_PRINTF("EditWndBuffRemoveData,");

    if( (h->lineData.nowPtr) != NULL )
    {
        if( (h->lineData.selectPtr) != NULL )
        { /* 選択開始位置有り */
            if( ((h->lineData.nowPtr) == (h->lineData.selectPtr)) &&
                (((h->lineData.nowPtr)->lineNum) == ((h->lineData.selectPtr)->lineNum)) )
            { /* 選択範囲は同一行内 */
                if( (h->lineData.nowPtr)->caretDataPos < (h->lineData.selectCaretPos) )
                { /* キャレット位置＜選択開始位置 */
                    removeSize += ((h->lineData.selectCaretPos) - (h->lineData.nowPtr)->caretDataPos);
                    saveCaretPos = (h->lineData.nowPtr)->caretDataPos;
                    (h->lineData.nowPtr)->caretDataPos = (h->lineData.selectCaretPos);
                    (h->lineData.selectCaretPos) = saveCaretPos;
                }
                else
                { /* 通常(選択開始位置≦キャレット位置) */
                    removeSize += ((h->lineData.nowPtr)->caretDataPos - (h->lineData.selectCaretPos));
                }

                if( removeSize )
                {
                    EDITWNDBUFF_DEBUG_PRINTF("0x%02X,%d\r\n",(BYTE)(h->lineData.nowPtr)->data[h->lineData.selectCaretPos],removeSize);

                    BuffLineDataDivide( (h->lineData.nowPtr), &dividePrePtr, &dividePostPtr ); /* キャレット位置で分割 */
                    newPtr = BuffLineDataShorten( dividePrePtr, removeSize );                 /* 分割後の前方データの末尾から所定量削除 */
                    BuffLineDataDestroy( dividePrePtr );                                 /* 前方データを */
                    dividePrePtr = newPtr;                                            /* 書き換え     */

                    newPtr = BuffLineDataJoin( dividePrePtr, dividePostPtr );                 /* 再結合 */
                    newPtr->lineNum = (h->lineData.nowPtr)->lineNum;                   /* 行番号は同じ */
                    newPtr->caretDataPos = (h->lineData.nowPtr)->caretDataPos - removeSize;    /* キャレット位置は削除量分前方になる */
                    BuffLineDataDestroy( dividePrePtr );                                 /* 作業データを削除 */
                    BuffLineDataDestroy( dividePostPtr );                                /* 作業データを削除 */

                    /* 古い行データと置き換える */
                    EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),newPtr );
                    BuffLineDataDestroy( (h->lineData.nowPtr) );
                    (h->lineData.nowPtr) = newPtr;
                }
                else
                {
                    nop();
                }
            }
            else
            { /* 選択範囲が複数行にまたがっている */
                if( ((h->lineData.selectPtr)->lineNum) < ((h->lineData.nowPtr)->lineNum) )
                { /* 正方向に選択 */
                    nop();
                }
                else
                { /* 負方向に選択 */
                    savePtr = (h->lineData.selectPtr);
                    saveCaretPos = (h->lineData.selectCaretPos);
                    (h->lineData.selectPtr) = (h->lineData.nowPtr);
                    (h->lineData.selectCaretPos) = (h->lineData.nowPtr)->caretDataPos;
                    (h->lineData.nowPtr) = savePtr;
                    (h->lineData.nowPtr)->caretDataPos = saveCaretPos;
                }

                /* 先頭から最後まで選択された行を削除 */
                for( nowPtr = (S_BUFF_LINE_DATA *)(h->lineData.selectPtr)->header.nextPtr; nowPtr != (h->lineData.nowPtr); nowPtr = nextPtr )
                {
                    nextPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr;
                    EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),nowPtr);
                    BuffLineDataDestroy( nowPtr );
                }
                /* 先頭行のうち、選択位置から前を残す */
                if( (h->lineData.selectCaretPos) == 0 )
                { /* 選択位置が先頭だったら、1行丸々削除 */
                    EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.selectPtr));
                    BuffLineDataDestroy( (h->lineData.selectPtr) );
                }
                else
                {
                    (h->lineData.selectPtr)->caretDataPos = (h->lineData.selectCaretPos);
                    BuffLineDataDivide( (h->lineData.selectPtr), &dividePrePtr, &dividePostPtr ); /* キャレット位置で分割 */
                    EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.selectPtr),dividePrePtr );
                    BuffLineDataDestroy( (h->lineData.selectPtr) );
                }

                /* 最終行のうち、選択位置から後を残す */
                BuffLineDataDivide( (h->lineData.nowPtr), &dividePrePtr, &dividePostPtr ); /* キャレット位置で分割 */
                EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),dividePostPtr );
                BuffLineDataDestroy( (h->lineData.nowPtr) );
                (h->lineData.nowPtr) = dividePostPtr;
                (h->lineData.nowPtr)->caretDataPos = 0;
                BuffLineDataUpdateLineNumAfter( (h->lineData.nowPtr) );
            }

            (h->lineData.selectPtr) = NULL;
            (h->lineData.selectCaretPos) = 0;
        }
        else
        { /* 選択無し */
            if( bBackSpace )
            { /* BSキー */
                if( (h->lineData.nowPtr)->caretDataPos > 0  )
                {
                    bNormalDelete = TRUE;

                    if( detectCharSet((h->lineData.nowPtr),(h->lineData.nowPtr)->caretDataPos-1) == DOUBLE_CHAR_LOW )
                    { /* 次の文字で削除量を判断 */
                        removeSize = 2;
                    }
                    else
                    {
                        removeSize = 1;
                    }
                }
                else
                { /* キャレットが行の先頭位置。つまり、前行との結合。*/
                    if( (h->lineData.nowPtr)->header.prevPtr != NULL )
                    {
                        prevPtr = (S_BUFF_LINE_DATA *)(h->lineData.nowPtr)->header.prevPtr;
                        newPtr = BuffLineDataJoin( prevPtr,(h->lineData.nowPtr) ); /* 前行と本行を結合した新しい行データを生成 */

                        if( newPtr != NULL )
                        {
                            /* 行番号、キャレット位置を前行データによって決まる */
                            newPtr->lineNum  = prevPtr->lineNum;
                            newPtr->caretDataPos = prevPtr->dataSize - prevPtr->newLineCodeSize;

                            EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr) ); /* 本行は削除 */
                            EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),prevPtr,newPtr );    /* 前行は新しい行データに置き換える */

                            BuffLineDataDestroy( (h->lineData.nowPtr) );
                            BuffLineDataDestroy( prevPtr );
                            (h->lineData.nowPtr) = newPtr;

                            BuffLineDataUpdateLineNumAfter( (h->lineData.nowPtr) );
                        }
                        else
                        {
                            EDITWNDBUFF_DEBUG_PRINTF("Bug!!!\r\n");
                        }
                    }
                    else
                    {
                        EDITWNDBUFF_DEBUG_PRINTF("No Action(Position 0,0)\r\n");
                    }
                }
            }
            else
            { /* DELキー */
                if( (h->lineData.nowPtr)->caretDataPos != ((h->lineData.nowPtr)->dataSize - (h->lineData.nowPtr)->newLineCodeSize) )
                { /* キャレットが行の最終位置でない */
                    bNormalDelete = TRUE;

                    if( detectCharSet((h->lineData.nowPtr),(h->lineData.nowPtr)->caretDataPos) == DOUBLE_CHAR_HIGH )
                    { /* キャレット位置の文字で削除量を判断 */
                        removeSize = 2;
                    }
                    else
                    {
                        removeSize = 1;
                    }
                    (h->lineData.nowPtr)->caretDataPos += removeSize;
                }
                else
                { /* キャレットが行の最終位置。つまり、次行との結合。*/
                    if( (h->lineData.nowPtr)->header.nextPtr != NULL )
                    { /* 次行有り */
                        nextPtr = (S_BUFF_LINE_DATA *)(h->lineData.nowPtr)->header.nextPtr;
                        newPtr = BuffLineDataJoin( (h->lineData.nowPtr),nextPtr ); /* 本行と次行を結合した新しい行データを生成 */

                        if( newPtr != NULL )
                        {
                            /* 行番号、キャレット位置は本行データによって決まる */
                            newPtr->lineNum  = (h->lineData.nowPtr)->lineNum;
                            newPtr->caretDataPos = (h->lineData.nowPtr)->caretDataPos;

                            EditWndBufferRemoveLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),nextPtr );         /* 次行は削除 */
                            EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),newPtr ); /* 本行は新しい行データに置き換える */

                            BuffLineDataDestroy( (h->lineData.nowPtr) );
                            BuffLineDataDestroy( nextPtr );
                            (h->lineData.nowPtr) = newPtr;

                            BuffLineDataUpdateLineNumAfter( (h->lineData.nowPtr) );
                        }
                        else
                        {
                            nop();
                        }
                    }
                    else
                    { /* 次行無し */
                        EDITWNDBUFF_DEBUG_PRINTF("No Action(DEL at LastPos)\r\n");
                    }
                }
            }

            if( bNormalDelete )
            { /* 削除有効 */
                EDITWNDBUFF_DEBUG_PRINTF("0x%02X,%d\r\n",(BYTE)(h->lineData.nowPtr)->data[(h->lineData.nowPtr)->caretDataPos-removeSize],removeSize);

                BuffLineDataDivide( (h->lineData.nowPtr), &dividePrePtr, &dividePostPtr ); /* キャレット位置で分割 */
                newPtr = BuffLineDataShorten( dividePrePtr, removeSize );                 /* 分割後の前方データの末尾から所定量削除 */
                BuffLineDataDestroy( dividePrePtr );                                 /* 前方データを */
                dividePrePtr = newPtr;                                            /* 書き換え     */

                newPtr = BuffLineDataJoin( dividePrePtr, dividePostPtr );                 /* 再結合 */
                newPtr->lineNum = (h->lineData.nowPtr)->lineNum;                   /* 行番号は同じ */
                newPtr->caretDataPos = (h->lineData.nowPtr)->caretDataPos - removeSize;    /* キャレット位置は削除量分前方になる */
                BuffLineDataDestroy( dividePrePtr );                                 /* 作業データを削除 */
                BuffLineDataDestroy( dividePostPtr );                                /* 作業データを削除 */

                /* 古い行データと置き換える */
                EditWndBufferReplaceLinkedList( &(h->lineData.topPtr),&(h->lineData.endPtr),(h->lineData.nowPtr),newPtr );
                BuffLineDataDestroy( (h->lineData.nowPtr) );
                (h->lineData.nowPtr) = newPtr;
            }
            else
            {
                nop();
            }
        }
    }
    else
    {
        EDITWNDBUFF_DEBUG_PRINTF("No Action(Empty)\r\n");
    }
}

/********************************************************************************
 * 内容  : 改行データ取得
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 引数  : PTSTR dataPtr
 * 戻り値: INT
 ***************************************/
INT
EditWndBuffGetNewLineData( H_EDITWND_BUFF hEditWndBuff, PTSTR dataPtr )
{
    INT size = 0;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    switch( h->NewLineType )
    {
    case EDITWND_BUFF_NEWLINE_CRLF:
        *dataPtr     = '\r';
        *(dataPtr+1) = '\n';
        size = 2;
        break;
    case EDITWND_BUFF_NEWLINE_LF  :
        *dataPtr = '\n';
        size = 1;
        break;
    case EDITWND_BUFF_NEWLINE_CR  :
        *dataPtr = '\r';
        size = 1;
        break;
    case EDITWND_BUFF_NEWLINE_NONE:
    default:
        break;
    }

    return size;
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファのタブサイズセット
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 引数  : INT tabSize
 * 戻り値: BOOL (TRUE:変更された)
 ***************************************/
BOOL
EditWndBuffSetTabSize( H_EDITWND_BUFF hEditWndBuff, INT tabSize )
{
    BOOL bRtn = FALSE;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( tabSize != h->tabSize )
    {
        h->tabSize = tabSize;
        bRtn = TRUE;
    }
    else
    {
        nop();
    }

    return bRtn;
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファの範囲選択ON
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 戻り値: なし
 ***************************************/
void
EditWndBuffSelectOn( H_EDITWND_BUFF hEditWndBuff )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( (h->lineData.selectPtr) )
    { /* 既に選択済みの場合 */
        nop(); /* 変更しない */
    }
    else
    {
        (h->lineData.selectPtr) = (h->lineData.nowPtr);
        (h->lineData.selectCaretPos) = (h->lineData.nowPtr)->caretDataPos;
    }
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファの範囲選択OFF
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 戻り値: BOOL (TRUE: 選択範囲が解除された,FALSE: 選択範囲無し)
 ***************************************/
BOOL
EditWndBuffSelectOff( H_EDITWND_BUFF hEditWndBuff )
{
    BOOL bRtn = FALSE;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( !(h->lineData.selectPtr) && !(h->lineData.selectCaretPos) )
    { /* 選択されていなかった */
        nop();
    }
    else if( (h->lineData.selectPtr == h->lineData.nowPtr) &&
        (h->lineData.selectCaretPos == (h->lineData.nowPtr)->caretDataPos) )
    { /* 選択されていなかった */
        nop();
    }
    else
    {
        bRtn = TRUE;
    }

    (h->lineData.selectPtr) = NULL;
    (h->lineData.selectCaretPos) = 0;

    return bRtn;
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファの全範囲選択
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 戻り値: BOOL
 ***************************************/
BOOL
EditWndBuffSelectAll( H_EDITWND_BUFF hEditWndBuff )
{
    BOOL bRtn = FALSE;
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;

    if( (h->lineData.topPtr) != NULL )
    {
        if( ((h->lineData.selectPtr) == (h->lineData.topPtr)) &&
            ((h->lineData.selectCaretPos) == 0) &&
            ((h->lineData.nowPtr) == (h->lineData.endPtr)) &&
            ((h->lineData.nowPtr)->caretDataPos == ((h->lineData.nowPtr)->dataSize - (h->lineData.nowPtr)->newLineCodeSize) ) )
        {
            nop();
        }
        else
        {
            (h->lineData.selectPtr) = (h->lineData.topPtr);
            (h->lineData.selectCaretPos) = 0;
            (h->lineData.nowPtr) = (h->lineData.endPtr);
            (h->lineData.nowPtr)->caretDataPos = (h->lineData.nowPtr)->dataSize - (h->lineData.nowPtr)->newLineCodeSize;
            bRtn = TRUE;
        }
    }
    else
    {
        nop();
    }

    return bRtn;
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファの検索データセット
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 引数  : TCHAR* dataPtr
 * 引数  : DWORD  length
 * 引数  : BOOL bDirectionUp
 * 戻り値: BOOL
 ***************************************/
BOOL
EditWndBuffFindDataSet( H_EDITWND_BUFF hEditWndBuff, TCHAR* dataPtr, DWORD length, BOOL bDirectionUp )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;
    BOOL rtn = (BOOL)FALSE;

    if( (dataPtr != NULL) && (length > 0) )
    { /* データ有りの場合 */
        S_BUFF_LINE_DATA *nowPtr;
        DWORD nowDataPos;
        DWORD findDataLen;

        findDataLen = min(length,strlen(dataPtr));
        EDITWNDBUFF_DEBUG_PRINTF("EditWndBuffFindDataSet,Target:%s,%d,%d\r\n",dataPtr,findDataLen,bDirectionUp);

        if( !bDirectionUp )
        { /* 下へ */
            for( nowPtr = (h->lineData.nowPtr),nowDataPos=(h->lineData.nowPtr)->caretDataPos; nowPtr != NULL; nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr,nowDataPos=0 )
            {
                TCHAR *ptr;
#if 0
                EDITWNDBUFF_DEBUG_PRINTF("EditWndBuffFindDataSet:%s\r\n",nowPtr->data);
#endif
                ptr = (TCHAR *)strstr(nowPtr->data+nowDataPos,dataPtr);

                if( ptr )
                { /* 見つかった */
                    h->lineData.nowPtr = nowPtr;
                    (h->lineData.nowPtr)->caretDataPos = (DWORD)(ptr - nowPtr->data) + findDataLen;
                    h->lineData.selectPtr = nowPtr;
                    h->lineData.selectCaretPos = (DWORD)(ptr - nowPtr->data);
                    EDITWNDBUFF_DEBUG_PRINTF("EditWndBuffFindDataSet,Found:%s\r\n",(TCHAR *)ptr);
                    rtn = (BOOL)TRUE;
                    break;
                }
            }
        }
        else
        { /* 上へ */
            for( nowPtr = (h->lineData.nowPtr),nowDataPos=(h->lineData.nowPtr)->caretDataPos; nowPtr != NULL; nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.prevPtr,nowDataPos=0 )
            {
                TCHAR *ptr;
#if 0
                EDITWNDBUFF_DEBUG_PRINTF("EditWndBuffFindDataSet:%s\r\n",nowPtr->data);
#endif
                ptr = (TCHAR *)strstr(nowPtr->data,dataPtr); /* 暫定、各行、逆からサーチしなければならない */

                if( ptr )
                { /* 見つかった */
                    h->lineData.nowPtr = nowPtr;
                    (h->lineData.nowPtr)->caretDataPos = (DWORD)(ptr - nowPtr->data) + findDataLen;
                    h->lineData.selectPtr = nowPtr;
                    h->lineData.selectCaretPos = (DWORD)(ptr - nowPtr->data);
                    EDITWNDBUFF_DEBUG_PRINTF("EditWndBuffFindDataSet,Found:%s\r\n",(TCHAR *)ptr);
                    rtn = (BOOL)TRUE;
                    break;
                }
            }
        }
    }
    else
    { /* データ無しの場合 */
        nop();
    }

    return rtn;
}

/********************************************************************************
 * 内容  : EDITウィンドウバッファのUndo
 * 引数  : H_EDITWND_BUFF hEditWndBuff
 * 戻り値: BOOL
 ***************************************/
BOOL
EditWndBuffUndo( H_EDITWND_BUFF hEditWndBuff )
{
    H_EDITWND_BUFF_LOCAL h = (H_EDITWND_BUFF_LOCAL)hEditWndBuff;
    BOOL rtn = (BOOL)FALSE;

    EDITWNDBUFF_DEBUG_PRINTF("EditWndBuffUndo,");
//    setSelectPosNowPosToFar(h,TRUE,3);
    setSelectPosNowPosToFar(h,FALSE,3);
    rtn = (BOOL)TRUE;

    return (BOOL)rtn;
}

/********************************************************************************
 * 内容  : バッファ行データを取得
 * 引数  : H_EDITWND_BUFF_LOCAL h
 * 引数  : TCHAR *dataPtr  データの先頭
 * 引数  : DWORD maxLength 最大長さ
 * 戻り値: S_BUFF_LINE_DATA *
 ***************************************/
static S_BUFF_LINE_DATA *
getBuffLine( H_EDITWND_BUFF_LOCAL h, TCHAR *dataPtr, DWORD maxLength )
{
    S_BUFF_LINE_DATA *lineDataPtr = NULL;
    DWORD i;
    INT   newLineCodeSize = 0;

    for( i=0; i<maxLength; i++ )
    {
        if( h->NewLineType == EDITWND_BUFF_NEWLINE_CRLF )
        {
            if( (i>0) &&
                (*(dataPtr+i-1) == '\r') &&
                (*(dataPtr+i)   == '\n') )
            {
                i++;
                newLineCodeSize = 2;
                break;
            }
            else
            {
                nop();
            }
        }
        else if( h->NewLineType == EDITWND_BUFF_NEWLINE_LF )
        {
            if( (*(dataPtr+i) == '\n') )
            {
                i++;
                newLineCodeSize = 1;
                break;
            }
            else
            {
                nop();
            }
        }
        else if( h->NewLineType == EDITWND_BUFF_NEWLINE_CR )
        {
            if( (*(dataPtr+i) == '\r') )
            {
                i++;
                newLineCodeSize = 1;
                break;
            }
            else
            {
                nop();
            }
        }
        else
        { /* EDITWND_BUFF_NEWLINE_NONE */
            nop();
        }
    }

    lineDataPtr = BuffLineDataCreate( i, newLineCodeSize, dataPtr, 0, 0 );

    return lineDataPtr;
}

/********************************************************************************
 * 内容  : キャラクタセットの判断(Shift_JISの場合)
 * 引数  : S_BUFF_LINE_DATA *dataPtr
 * 引数  : DWORD            offset   判断したい文字の先頭からのオフセット(0 origin)
 * 戻り値: CHARSET_TYPE
 ***************************************/
static CHARSET_TYPE
detectCharSet( S_BUFF_LINE_DATA *dataPtr, DWORD offset )
{
    DWORD i;
    int charType = SINGLE_CHAR;

    if( dataPtr != NULL )
    {
        for( i=0; i<(dataPtr->dataSize-dataPtr->newLineCodeSize); i++ )
        {
            if( charType == DOUBLE_CHAR_HIGH )
            {
                charType = DOUBLE_CHAR_LOW;
            }
            else
            {
                if( ( (BYTE)(*(dataPtr->data+i)) <= (BYTE)0x80) || (((BYTE)0xA0 <= (BYTE)(*(dataPtr->data+i))) && ((BYTE)(*(dataPtr->data+i)) <= (BYTE)0xDF)) )
                {
                    charType = SINGLE_CHAR;
                }
                else
                {
                    charType = DOUBLE_CHAR_HIGH;
                }
            }

            if( i==offset )
            {
                break;
            }
            else
            {
                nop();
            }
        }

        if( i==(dataPtr->dataSize-dataPtr->newLineCodeSize) )
        {
            charType = SINGLE_CHAR;
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

    return charType;
}

/********************************************************************************
 * 内容  : キャレットX表示位置取得
 * 引数  : H_EDITWND_BUFF_LOCAL h
 * 引数  : S_BUFF_LINE_DATA *linePtr   行データ
 * 引数  : DWORD             dataPos   列位置(データ上の)
 * 戻り値: DWORD
 ***************************************/
static DWORD
getCaretDispXpos( H_EDITWND_BUFF_LOCAL h, S_BUFF_LINE_DATA *linePtr, DWORD dataPos )
{
    DWORD i,j=0;
    DWORD literalMaxSize;
    int tab_offset;
    CHARSET_TYPE charType;

    if( linePtr != NULL )
    {
        charType = SINGLE_CHAR;
        literalMaxSize = (linePtr->dataSize-linePtr->newLineCodeSize);

        for( i=0,j=0; (i<dataPos)&&(i<literalMaxSize); i++ )
        { /* 1行中のデータを1文字ずつ処理 */
            if( charType == DOUBLE_CHAR_HIGH )
            { /* 前文字が2byte文字の上位byteだったら */
                charType = DOUBLE_CHAR_LOW;
                j++;
            }
            else
            { /* 前文字が2byte文字の上位byte以外 */
                if( (*(linePtr->data+i)) == '\0' )
                {
                    charType = SINGLE_CHAR;
                    j++;
                }
                else if( (*(linePtr->data+i)) == '\t' )
                { /* 処理中の文字はTAB */
                    charType = TAB_CHAR;

                    tab_offset = h->tabSize - (j % h->tabSize);
                    j+=tab_offset;
                }
                else if( ( (BYTE)(*(linePtr->data+i)) <= (BYTE)0x80) ||
                         (((BYTE)0xA0 <= (BYTE)(*(linePtr->data+i))) && ((BYTE)(*(linePtr->data+i)) <= (BYTE)0xDF)) ||
                         ((BYTE)0xF0 <= (BYTE)(*(linePtr->data+i))) )
                { /* 処理中の文字は1byte文字 */
                    charType = SINGLE_CHAR;
                    j++;
                }
                else
                { /* 処理中の文字は2byte文字の上位byte */
                    charType = DOUBLE_CHAR_HIGH;
                    j++;
                }
            }
        }
    }
    else
    {
        nop();
    }

    return j;
}

/********************************************************************************
 * 内容  : 指定位置の表示データを取得する
 * 引数  : H_EDITWND_BUFF_LOCAL h
 * 引数  : S_BUFF_LINE_DATA *linePtr   行データ
 * 引数  : DWORD             dispPos   列位置(表示上の)
 * 引数  : S_BUFF_DISP_DATA *dataPtr
 * 戻り値: BOOL
 ***************************************/
static BOOL
getDispCharData( H_EDITWND_BUFF_LOCAL h, S_BUFF_LINE_DATA *linePtr, DWORD dispPos, S_BUFF_DISP_DATA *dataPtr )
{
    DWORD dataPos,j,k;
    DWORD literalMaxSize;
    int tab_offset;

    if( (linePtr != NULL) && (dataPtr != NULL) )
    {
        dataPtr->size    = 0;
        dataPtr->type    = SINGLE_CHAR;
        dataPtr->bSelect = FALSE;
        dataPtr->dataPos = 0;
        dataPtr->caretPos= 0;

        literalMaxSize = (linePtr->dataSize-linePtr->newLineCodeSize);

        for( dataPos=0,j=0; dataPos<literalMaxSize; dataPos++ )
        { /* 1行中のデータを1文字ずつ処理 */
            if( dataPtr->type == DOUBLE_CHAR_HIGH )
            { /* 前文字が2byte文字の上位byteだったら */
                dataPtr->type = DOUBLE_CHAR_LOW;
                *(dataPtr->data+1) = *(linePtr->data+dataPos);
            }
            else
            { /* 前文字が2byte文字の上位byte以外 */
                if( (*(linePtr->data+dataPos)) == '\0' )
                {
                    dataPtr->type = SINGLE_CHAR;
                    *(dataPtr->data) = ' ';
                }
                else if( (*(linePtr->data+dataPos)) == '\t' )
                { /* 処理中の文字はTAB */
                    dataPtr->type = TAB_CHAR;

                    tab_offset = h->tabSize - (j % h->tabSize);

                    if( dispPos < (j+tab_offset) )
                    {
                        for(k=0;k<tab_offset;k++)
                        {
                            *(dataPtr->data+k) = ' ';
                        }
                        dataPtr->size = tab_offset;
                        dataPtr->offset = dispPos - j;
                        break;
                    }
                    else
                    {
                        j+=tab_offset;
                        continue;
                    }
                }
                else if( ( (BYTE)(*(linePtr->data+dataPos)) <= (BYTE)0x80) ||
                         (((BYTE)0xA0 <= (BYTE)(*(linePtr->data+dataPos))) && ((BYTE)(*(linePtr->data+dataPos)) <= (BYTE)0xDF)) ||
                         ((BYTE)0xF0 <= (BYTE)(*(linePtr->data+dataPos))) )
                { /* 処理中の文字は1byte文字 */
                    dataPtr->type = SINGLE_CHAR;
                    *(dataPtr->data) = *(linePtr->data+dataPos);
                }
                else
                { /* 処理中の文字は2byte文字の上位byte */
                    dataPtr->type = DOUBLE_CHAR_HIGH;
                    *(dataPtr->data)   = *(linePtr->data+dataPos);
                    *(dataPtr->data+1) = *(linePtr->data+dataPos+1);
                }
            }

            if( j == dispPos )
            { /* 処理中の文字が指定された表示位置 */
                break;
            }
            else
            {
                nop();
            }

            j++;
        }

        if( dataPos == literalMaxSize )
        {
            dataPtr->type = END_CHAR;
        }
        else
        {
            nop();
        }

        dataPtr->dataPos = dataPos;

        if( dataPos >= literalMaxSize )
        {
            dataPtr->dataPos = literalMaxSize;
        }
        else
        {
            switch( dataPtr->type )
            {
            case SINGLE_CHAR:
                dataPtr->size = 1;
                dataPtr->offset = 0;
                break;
            case DOUBLE_CHAR_HIGH:
                dataPtr->size = 2;
                dataPtr->offset = 0;
                break;
            case DOUBLE_CHAR_LOW:
                dataPtr->size = 2;
                dataPtr->offset = 1;
                break;
            default:
                break;
            }

            if( (h->lineData.selectPtr) != NULL )
            { /* 選択開始位置有り */
                if( ((h->lineData.selectPtr)->lineNum) < ((h->lineData.nowPtr)->lineNum) )
                { /* 正方向に選択 */
                    if( ((h->lineData.selectPtr)->lineNum < linePtr->lineNum) && (linePtr->lineNum < (h->lineData.nowPtr)->lineNum) )
                    {
                        dataPtr->bSelect = TRUE;
                    }
                    else if( ((h->lineData.selectPtr)->lineNum == linePtr->lineNum) && ((h->lineData.selectCaretPos) <= dataPos) )
                    {
                        dataPtr->bSelect = TRUE;
                    }
                    else if( ((h->lineData.nowPtr)->lineNum == linePtr->lineNum) && (dataPos < linePtr->caretDataPos) )
                    {
                        dataPtr->bSelect = TRUE;
                    }
                    else
                    {
                        nop();
                    }
                }
                else if( ((h->lineData.selectPtr)->lineNum) > ((h->lineData.nowPtr)->lineNum) )
                { /* 負方向に選択 */
                    if( ((h->lineData.nowPtr)->lineNum < linePtr->lineNum) && (linePtr->lineNum < (h->lineData.selectPtr)->lineNum) )
                    {
                        dataPtr->bSelect = TRUE;
                    }
                    else if( ((h->lineData.nowPtr)->lineNum == linePtr->lineNum) && (linePtr->caretDataPos <= dataPos) )
                    {
                        dataPtr->bSelect = TRUE;
                    }
                    else if( ((h->lineData.selectPtr)->lineNum == linePtr->lineNum) && (dataPos < (h->lineData.selectCaretPos)) )
                    {
                        dataPtr->bSelect = TRUE;
                    }
                    else
                    {
                        nop();
                    }
                }
                else
                { /* 同一行内で選択 */
                    if( (h->lineData.nowPtr) == linePtr )
                    {
                        if( linePtr->caretDataPos < (h->lineData.selectCaretPos) )
                        {
                            if( (linePtr->caretDataPos <= dataPos) && (dataPos < (h->lineData.selectCaretPos)) )
                            {
                                dataPtr->bSelect = TRUE;
                            }
                            else
                            {
                                nop();
                            }
                        }
                        else if( linePtr->caretDataPos > (h->lineData.selectCaretPos) )
                        {
                            if( ((h->lineData.selectCaretPos) <= dataPos) && (dataPos < linePtr->caretDataPos) )
                            {
                                dataPtr->bSelect = TRUE;
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
                    else
                    {
                        nop();
                    }
                }
            }
            else
            {
                nop();
            }
        }

        if( dataPtr->type == DOUBLE_CHAR_LOW )
        {
            dataPtr->caretPos = dataPtr->dataPos + 1;
        }
        else if( (dataPtr->type == TAB_CHAR) && (dataPtr->offset != 0)  )
        {
            dataPtr->caretPos = dataPtr->dataPos + 1;
        }
        else
        {
            dataPtr->caretPos = dataPtr->dataPos;
        }
    }
    else
    {
        nop();
    }

    return TRUE;
}

/********************************************************************************
 * 内容  : 現在のキャレットの位置から所定量移動した位置を選択する
 * 引数  : H_EDITWND_BUFF_LOCAL h
 * 引数  : BOOL                 bMinus
 * 引数  : DWORD                offset
 * 戻り値: なし
 ***************************************/
static void
setSelectPosNowPosToFar( H_EDITWND_BUFF_LOCAL h, BOOL bMinus, DWORD offset )
{
    S_BUFF_LINE_DATA *nowPtr;
    DWORD nowOffset;

    if( (h->lineData.nowPtr) != NULL )
    {
        nowOffset = offset;

        if( bMinus )
        { /* 上方向 */
            if( nowOffset <= (h->lineData.nowPtr)->caretDataPos )
            { /* 同一行に含まれる*/
                (h->lineData.selectPtr) = (h->lineData.nowPtr);
                h->lineData.selectCaretPos = (h->lineData.nowPtr)->caretDataPos - nowOffset;
            }
            else
            { /* 同一行に含まれない */
                nowOffset -= (h->lineData.nowPtr)->caretDataPos;

                for( nowPtr = (h->lineData.nowPtr)->header.prevPtr; nowPtr != NULL; nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.prevPtr )
                {
                    if( nowOffset <= (nowPtr->dataSize-nowPtr->newLineCodeSize) )
                    { /* この行に含まれる*/
                        (h->lineData.selectPtr) = nowPtr;
                        h->lineData.selectCaretPos = (nowPtr->dataSize-nowPtr->newLineCodeSize) - nowOffset;
                        break;
                    }
                    else
                    {
                        nowOffset -= (nowPtr->dataSize-nowPtr->newLineCodeSize);
                    }
                }
            }
        }
        else
        { /* 下方向 */
            if( (h->lineData.nowPtr)->caretDataPos + nowOffset <= ((h->lineData.nowPtr)->dataSize-(h->lineData.nowPtr)->newLineCodeSize)  )
            { /* 同一行に含まれる*/
                (h->lineData.selectPtr) = (h->lineData.nowPtr);
                h->lineData.selectCaretPos = (h->lineData.nowPtr)->caretDataPos + nowOffset;
            }
            else
            { /* 同一行に含まれない */
                nowOffset -= ((h->lineData.nowPtr)->dataSize-(h->lineData.nowPtr)->newLineCodeSize) - (h->lineData.nowPtr)->caretDataPos;

                for( nowPtr = (h->lineData.nowPtr)->header.nextPtr; nowPtr != NULL; nowPtr = (S_BUFF_LINE_DATA *)nowPtr->header.nextPtr )
                {
                    if( nowOffset <= (nowPtr->dataSize-nowPtr->newLineCodeSize) )
                    { /* この行に含まれる*/
                        (h->lineData.selectPtr) = nowPtr;
                        h->lineData.selectCaretPos = nowOffset;
                        break;
                    }
                    else
                    {
                        nowOffset -= (nowPtr->dataSize-nowPtr->newLineCodeSize);
                    }
                }
            }
        }
    }
    else
    {
        nop(); /* 異常 */
    }
}
