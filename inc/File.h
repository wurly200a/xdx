#ifndef FILE_H

/* 管理するファイルのID */
typedef enum
{
    FILE_ID_DX7_1VOICE_DATA   ,
    FILE_ID_DX7_ALL_VOICE_DATA,
    FILE_ID_DX100_1VOICE_DATA   ,
    FILE_ID_DX100_ALL_VOICE_DATA,
    FILE_ID_MAX
}FILE_ID;

/********************************************************************************
 * 内容  : モジュール初期化
 * 引数  : HWND ウィンドウハンドラ
 * 戻り値: なし
 ***************************************/
void FileInitialize( HWND hwnd );

/********************************************************************************
 * 内容  : モジュール終了
 * 引数  : なし
 * 戻り値: なし
 ***************************************/
void FileEnd( void );

/********************************************************************************
 * 内容  : コモンダイアログにより、ファイルのフルパス、名前を取得する
 * 引数  : HWND    hwnd
 * 引数  : FILE_ID id
 * 戻り値: BOOL
 **********************************************/
BOOL FileOpenDlg( HWND hwnd, FILE_ID id );

/********************************************************************************
 * 内容  : ファイル名称(フルパス)を取得する
 * 引数  : FILE_ID id
 * 戻り値: PTSTR
 ***************************************/
PTSTR FileGetName( FILE_ID id );

/********************************************************************************
 * 内容  : ファイル名称を取得する
 * 引数  : FILE_ID id
 * 戻り値: PTSTR
 ***************************************/
PTSTR FileGetTitleName( FILE_ID id );

/********************************************************************************
 * 内容  : ディレクトリパスを取得する
 * 引数  : FILE_ID id
 * 戻り値: PTSTR
 ***************************************/
PTSTR FileGetDir( FILE_ID id );

/********************************************************************************
 * 内容  : ファイル名称をセットする
 * 引数  : FILE_ID id
 * 引数  : PTSTR   ptstrFileName
 * 引数  : BOOL    bCat (TRUE:連結する,FALSE:上書き)
 * 戻り値: PTSTR
 ***************************************/
BOOL FileSetName( FILE_ID id, PTSTR ptstrFileName, BOOL bCat );

/********************************************************************************
 * 内容  : ファイルから1byteずつデータを読み込む。領域はmallocで取得する
 * 引数  : FILE_ID id    ファイルID
 *       : LPDWORD pSize データサイズを格納する変数のポインタ
 * 戻り値: PBYTE         読み込んだデータの先頭ポインタ
 ***************************************/
PBYTE FileReadByte( FILE_ID id, LPDWORD pSize );

/********************************************************************************
 * 内容  : ファイルの長さを取得する(Max: 4GByte)
 * 引数  : PTSTR pstrFileName ファイルのフルパス
 * 戻り値: DWORD
 ***************************************/
DWORD FileGetLength( PTSTR pstrFileName );

typedef enum
{
    FILE_OK,
    FILE_NAME_NOT_SET,
    FILE_SIZE_ERR,
    FILE_DATA_ERR,
    FILE_HANDLE_ERR,
    FILE_NOT_INITIALIZE,
} FILE_RESULT;

/********************************************************************************
 * 内容  : ファイルに書き込む
 * 引数  : FILE_ID id
 * 引数  : TCHAR *dataPtr
 * 引数  : DWORD dataSize
 * 戻り値: FILE_RESULT
 ***************************************/
FILE_RESULT FileWrite( FILE_ID id, TCHAR *dataPtr, DWORD dataSize );

/********************************************************************************
 * 内容  : コモンダイアログにより、ファイルを保存するフルパス、名前を取得する
 * 引数  : HWND    hwnd
 * 引数  : FILE_ID id
 * 戻り値: BOOL
 **********************************************/
BOOL FileSaveDlg( HWND hwnd, FILE_ID id );

#define FILE_H
#endif /* FILE_H */
