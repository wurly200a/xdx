#ifndef DEBUGWND_DEF_H

typedef enum
{
    DEBUGWND_ON_CREATE          , /* WM_CREATE                     */
    DEBUGWND_ON_PAINT           , /* WM_PAINT                      */
    DEBUGWND_ON_SIZE            , /* WM_SIZE                       */
    DEBUGWND_ON_MOVE            , /* WM_MOVE                       */
    DEBUGWND_ON_WINDOWPOSCHANGED, /* WM_WINDOWPOSCHANGED           */
    DEBUGWND_ON_CLOSE           , /* WM_CLOSE                      */
    DEBUGWND_ON_DESTROY         , /* WM_DESTROY                    */
    DEBUGWND_ON_COMMAND         , /* WM_COMMAND                    */
    DEBUGWND_ON_KEYUP           , /* WM_KEYUP                      */
    DEBUGWND_ON_KEYDOWN         , /* WM_KEYDOWN                    */
    DEBUGWND_ON_CHAR            , /* WM_CHAR                       */
    DEBUGWND_ON_HSCROLL         , /* WM_HSCROLL                    */
    DEBUGWND_ON_VSCROLL         , /* WM_VSCROLL                    */
    DEBUGWND_ON_MOUSEWHEEL      , /* WM_MOUSEWHEEL                 */
    DEBUGWND_ON_SETFOCUS        , /* WM_SETFOCUS                   */
    DEBUGWND_ON_KILLFOCUS       , /* WM_KILLFOCUS                  */
    DEBUGWND_ON_INITMENUPOPUP   , /* WM_INITMENUPOPUP              */
    DEBUGWND_ON_FINDMSGSTRING   , /* FINDMSGSTRINGÇÃìoò^ÉÅÉbÉZÅ[ÉW */
    DEBUGWND_ON_APP             , /* WM_APP                        */
    DEBUGWND_ON_DEFAULT         , /* default                       */
    DEBUGWND_MAX
} DEBUGWND_INDEX;

typedef struct
{
    HINSTANCE hInstance;
    PTSTR     szAppName;
    BOOL execute;
    BOOL bAppExit;
    int  xWindowPos;
    int  yWindowPos;
    int  xPos;
    int  yPos;
    int  cxWindow;
    int  cyWindow;
    int  cxClient;
    int  cyClient;
    int  cxChar;
    int  cyChar;
    int  cxCaps;
    HWND hWndEdit;
    HFONT hFontIo;
    HACCEL hAccel;
    UINT messageFindReplace;
    HWND hDlgModeless;
} S_DEBUGWND_DATA;

#define DEBUGWND_DEF_H
#endif /* DEBUGWND_DEF_H */
