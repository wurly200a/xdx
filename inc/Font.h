#ifndef FONT_H

/* 管理するフォントのID */
typedef enum
{
    FONT_ID_MAIN,
    FONT_ID_IO  ,
    FONT_ID_MAX
}FONT_ID;

/********************************************************************************
 * 内容  : フォント初期化
 * 引数  : なし
 * 戻り値: なし
 ***************************************/
void FontInit( void );

/********************************************************************************
 * 内容  : フォント選択
 * 引数  : HWND hwnd
 * 引数  : FONT_ID id
 * 戻り値: BOOL (TRUE:OKが押された、FALSE:キャンセルされた)
 ***************************************/
BOOL FontChooseFont( HWND hwnd, FONT_ID id );

/********************************************************************************
 * 内容  : 論理フォント取得
 * 引数  : FONT_ID id
 * 戻り値: LOGFONT *
 ***************************************/
LOGFONT *FontGetLogFont( FONT_ID id );

#define FONT_H
#endif /* FONT_H */

