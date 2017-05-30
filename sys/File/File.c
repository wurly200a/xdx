/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */

/* 外部関数定義 */
/* 外部変数定義 */
/* 内部関数定義 */
#include "File.h"
/* 内部変数定義 */
static TCHAR nullStr = '\0';


typedef struct
{
    BOOL          init;       /* 初期化済かどうか         */
    DWORD         size;       /* ファイルサイズ           */
    PTSTR         pFileName;  /* ファイル名(フルパス)     */
    PTSTR         pTitleName; /* ファイル名のみ           */
    PTSTR         pDirPath;   /* ディレクトリパス         */
    OPENFILENAME* pOfn;
    PTSTR         pFilter;
    PTSTR         pExt;
    PBYTE         pByte;      /* ファイルを読み込んだ領域 */
} S_FILE_LIST;

#define CUSTOM_FILTER_SIZE 256

static TCHAR szFileName [FILE_ID_MAX][1024];
static TCHAR szTitleName[FILE_ID_MAX][1024];
static TCHAR szDirPath  [FILE_ID_MAX][1024];
static OPENFILENAME ofns[FILE_ID_MAX];
static TCHAR szCustFilter[FILE_ID_MAX][CUSTOM_FILTER_SIZE];

static S_FILE_LIST fileList[FILE_ID_MAX] =
{  /* exist, size, file, title, dir ,  ofn  ,filter                            , ext        ,pByte */
    { FALSE,    0, NULL, NULL , NULL,  NULL ,TEXT("テキスト文書 (*.txt)\0*.txt\0")TEXT("すべてのファイル\0*.*\0"), TEXT("txt"),NULL  },
};

/********************************************************************************
 * 内容  : モジュール初期化
 * 引数  : HWND ウィンドウハンドラ
 * 戻り値: なし
 ***************************************/
void
FileInitialize( HWND hwnd )
{
    INT i;

    for( i=0; i<FILE_ID_MAX; i++ )
    {
        fileList[i].init = TRUE;

        fileList[i].pFileName  = szFileName[i];
        fileList[i].pTitleName = szTitleName[i];
        fileList[i].pDirPath   = szDirPath[i];
        fileList[i].pOfn       = &(ofns[i]);

        fileList[i].pOfn->lStructSize       = sizeof(OPENFILENAME);
        fileList[i].pOfn->hwndOwner         = hwnd;
        fileList[i].pOfn->hInstance         = NULL;
        fileList[i].pOfn->lpstrFilter       = fileList[i].pFilter;
        fileList[i].pOfn->lpstrCustomFilter = szCustFilter[i];
        fileList[i].pOfn->nMaxCustFilter    = CUSTOM_FILTER_SIZE;
        fileList[i].pOfn->nFilterIndex      = 0;
        fileList[i].pOfn->lpstrFile         = NULL;
        fileList[i].pOfn->nMaxFile          = MAX_PATH;
        fileList[i].pOfn->lpstrFileTitle    = NULL;
        fileList[i].pOfn->nMaxFileTitle     = MAX_PATH;
        fileList[i].pOfn->lpstrInitialDir   = NULL;
        fileList[i].pOfn->lpstrTitle        = NULL;
        fileList[i].pOfn->Flags             = 0;
        fileList[i].pOfn->nFileOffset       = 0;
        fileList[i].pOfn->nFileExtension    = 0;
/*      fileList[i].pOfn->lpstrDefExt       = TEXT ("txt") ; */
        fileList[i].pOfn->lpstrDefExt       = fileList[i].pExt;
        fileList[i].pOfn->lCustData         = 0L;
        fileList[i].pOfn->lpfnHook          = NULL;
        fileList[i].pOfn->lpTemplateName    = NULL;
    }
}


/********************************************************************************
 * 内容  : モジュール終了
 * 引数  : なし
 * 戻り値: なし
 ***************************************/
void
FileEnd( void )
{
    INT i;

    for( i=0; i<FILE_ID_MAX; i++ )
    {
        if( fileList[i].init == TRUE )
        {
            if( fileList[i].pByte != NULL )
            {
                free( fileList[i].pByte );
                fileList[i].pByte = NULL;
            }
            else
            {
                /* do nothing */
            }
        }
        else
        {
            /* do nothing */
        }
    }
}

/********************************************************************************
 * 内容  : コモンダイアログにより、ファイルのフルパス、名前を取得する
 * 引数  : HWND    hwnd
 * 引数  : FILE_ID id
 * 戻り値: BOOL
 **********************************************/
BOOL
FileOpenDlg( HWND hwnd, FILE_ID id )
{
    BOOL rtn = FALSE;

    if( (id < FILE_ID_MAX) && (fileList[id].init == TRUE) )
    {
        fileList[id].pOfn->hwndOwner      = hwnd;
        fileList[id].pOfn->lpstrFile      = fileList[id].pFileName;
        fileList[id].pOfn->lpstrFileTitle = fileList[id].pTitleName;
        fileList[id].pOfn->Flags          = OFN_HIDEREADONLY | OFN_CREATEPROMPT;

        rtn = GetOpenFileName( fileList[id].pOfn );

        if( rtn == TRUE )
        {
            lstrcpy( (PTSTR)fileList[id].pDirPath , (PTSTR) fileList[id].pFileName);
            *(fileList[id].pDirPath + fileList[id].pOfn->nFileOffset) = '\0';
        }
        else
        {
            *(fileList[id].pDirPath) = '\0';
        }
    }
    else
    {
        /* error */
    }

    return rtn;
}

/********************************************************************************
 * 内容  : ファイル名称(フルパス)を取得する
 * 引数  : FILE_ID id
 * 戻り値: PTSTR
 ***************************************/
PTSTR
FileGetName( FILE_ID id )
{
    PTSTR rtn;

    if( (id < FILE_ID_MAX) && (fileList[id].init == TRUE) )
    {
        rtn = fileList[id].pFileName;
    }
    else
    {
        rtn = &nullStr;
    }

    return rtn;
}

/********************************************************************************
 * 内容  : ファイル名称を取得する
 * 引数  : FILE_ID id
 * 戻り値: PTSTR
 ***************************************/
PTSTR
FileGetTitleName( FILE_ID id )
{
    PTSTR rtn;

    if( (id < FILE_ID_MAX) && (fileList[id].init == TRUE) )
    {
        rtn = fileList[id].pTitleName;
    }
    else
    {
        rtn = &nullStr;
    }

    return rtn;
}

/********************************************************************************
 * 内容  : ディレクトリパスを取得する
 * 引数  : FILE_ID id
 * 戻り値: PTSTR
 ***************************************/
PTSTR
FileGetDir( FILE_ID id )
{
    PTSTR rtn;

    if( (id < FILE_ID_MAX) && (fileList[id].init == TRUE) )
    {
        rtn = fileList[id].pDirPath;
    }
    else
    {
        rtn = &nullStr;
    }

    return rtn;
}

/********************************************************************************
 * 内容  : ファイル名称をセットする
 * 引数  : FILE_ID id
 * 引数  : PTSTR   ptstrFileName
 * 引数  : BOOL    bCat (TRUE:連結する,FALSE:上書き)
 * 戻り値: PTSTR
 ***************************************/
BOOL
FileSetName( FILE_ID id, PTSTR ptstrFileName, BOOL bCat )
{
    BOOL rtn = FALSE;

    if( (id < FILE_ID_MAX) && (fileList[id].init == TRUE) )
    {
        if( bCat )
        { /* 連結する */
            lstrcat( fileList[id].pFileName, ptstrFileName );
        }
        else
        {
            if( *((TCHAR *)ptstrFileName) == '"' )
            {
                strncpy( fileList[id].pFileName, (ptstrFileName)+1, lstrlen(ptstrFileName)-2 );
            }
            else
            {
                lstrcpy( fileList[id].pFileName, ptstrFileName );
            }
        }

        if( ((strrchr(fileList[id].pFileName,'\\')) != NULL) ) /* ファイル名に ￥が付いていることを確認 */
        {
            lstrcpy( (PTSTR)fileList[id].pDirPath , (PTSTR) fileList[id].pFileName);
            *( (strrchr(fileList[id].pDirPath,'\\')) + 1 ) = '\0';

            lstrcpy( (PTSTR)fileList[id].pTitleName , (PTSTR)(strrchr(fileList[id].pFileName,'\\'))+1 );
        }
        else
        {
            /* null */
        }

        rtn = TRUE;
    }
    else
    {
        /* error */
    }

    return rtn;
}

/********************************************************************************
 * 内容  : ファイルから1byteずつデータを読み込む。領域はmallocで取得する
 * 引数  : FILE_ID id    ファイルID
 *       : LPDWORD pSize データサイズを格納する変数のポインタ
 * 戻り値: PBYTE         読み込んだデータの先頭ポインタ
 ***************************************/
PBYTE
FileReadByte( FILE_ID id, LPDWORD pSize )
{
    HANDLE hFile;
    DWORD  dwBytesRead;
    PBYTE  rtn = NULL;

    if( (id < FILE_ID_MAX) && (fileList[id].init == TRUE) )
    {
        if( fileList[id].pByte != NULL )
        {
            /* 既に入っていたら解放してから */
            free( fileList[id].pByte );
            fileList[id].pByte = NULL;
        }
        else
        {
            /* do nothing */
        }

        hFile = CreateFile( fileList[id].pFileName, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, 0, NULL );
        if( hFile == INVALID_HANDLE_VALUE )
        {
            nop();
        }
        else
        {
            fileList[id].size  = GetFileSize( hFile, NULL );
            fileList[id].pByte = malloc( fileList[id].size + 2 );
            ReadFile( hFile, fileList[id].pByte, fileList[id].size, &dwBytesRead, NULL );
            CloseHandle( hFile );
            *(fileList[id].pByte + fileList[id].size)     = '\0'; /* 保護 */
            *(fileList[id].pByte + fileList[id].size + 1) = '\0'; /* 保護 */
            *pSize = fileList[id].size;
            rtn = fileList[id].pByte;
        }
    }
    else
    {
        nop(); /* error */
    }

    return rtn;
}


/********************************************************************************
 * 内容  : ファイルの長さを取得する(Max: 4GByte)
 * 引数  : PTSTR pstrFileName ファイルのフルパス
 * 戻り値: DWORD
 ***************************************/
DWORD
FileGetLength( PTSTR pstrFileName )
{
    HANDLE hFile;
    DWORD  dwFileLength;

    hFile = CreateFile(pstrFileName, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, 0, NULL);
    if( hFile == INVALID_HANDLE_VALUE )
    {
        dwFileLength = 0;
    }
    else
    {
        dwFileLength = GetFileSize(hFile, NULL); /* ファイルの長さ取得 */
    }

    return dwFileLength;
}

/********************************************************************************
 * 内容  : ファイルに書き込む
 * 引数  : FILE_ID id
 * 引数  : TCHAR *dataPtr
 * 引数  : DWORD dataSize
 * 戻り値: FILE_RESULT
 ***************************************/
FILE_RESULT
FileWrite( FILE_ID id, TCHAR *dataPtr, DWORD dataSize )
{
    DWORD  dwBytesWritten;
    HANDLE hFile;
    FILE_RESULT result = FILE_OK;

    if( (id < FILE_ID_MAX) && (fileList[id].init == TRUE) )
    {
        if( fileList[id].pFileName[0] != '\0' )
        {
            hFile = CreateFile( fileList[id].pFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );
            if( hFile == INVALID_HANDLE_VALUE )
            {
                result = FILE_HANDLE_ERR;
            }
            else
            {
                if( dataPtr != NULL )
                {
                    WriteFile( hFile, dataPtr, dataSize * sizeof (TCHAR), &dwBytesWritten, NULL );

                    if( (dataSize * sizeof (TCHAR)) != dwBytesWritten )
                    {
                        result = FILE_SIZE_ERR;
                    }
                    else
                    {
                        nop();
                    }
                }
                else
                {
                    result = FILE_DATA_ERR;
                }
                CloseHandle( hFile );
            }
        }
        else
        {
            result = FILE_NAME_NOT_SET;
        }
    }
    else
    {
        result = FILE_NOT_INITIALIZE;
    }

    return result;
}

/********************************************************************************
 * 内容  : コモンダイアログにより、ファイルを保存するフルパス、名前を取得する
 * 引数  : HWND    hwnd
 * 引数  : FILE_ID id
 * 戻り値: BOOL
 **********************************************/
BOOL
FileSaveDlg( HWND hwnd, FILE_ID id )
{
    BOOL rtn = FALSE;

    if( (id < FILE_ID_MAX) && (fileList[id].init == TRUE) )
    {
        fileList[id].pOfn->hwndOwner      = hwnd;
        fileList[id].pOfn->lpstrFile      = fileList[id].pFileName;
        fileList[id].pOfn->lpstrFileTitle = fileList[id].pTitleName;
        fileList[id].pOfn->Flags          = OFN_OVERWRITEPROMPT | OFN_EXPLORER;

        rtn = GetSaveFileName( fileList[id].pOfn );

        if( rtn == TRUE )
        {
            lstrcpy( (PTSTR)fileList[id].pDirPath , (PTSTR) fileList[id].pFileName);
            *(fileList[id].pDirPath + fileList[id].pOfn->nFileOffset) = '\0';
        }
        else
        {
            *(fileList[id].pDirPath) = '\0';
        }
    }
    else
    {
        /* error */
    }

    return rtn;
}
