#ifndef EDITWND_DEF_H

typedef enum
{
    EDITWND_ON_CREATE              , /* WM_CREATE               */
    EDITWND_ON_PAINT               , /* WM_PAINT                */
    EDITWND_ON_SIZE                , /* WM_SIZE                 */
    EDITWND_ON_CLOSE               , /* WM_CLOSE                */
    EDITWND_ON_DESTROY             , /* WM_DESTROY              */
    EDITWND_ON_COMMAND             , /* WM_COMMAND              */
    EDITWND_ON_KEYUP               , /* WM_KEYUP                */
    EDITWND_ON_KEYDOWN             , /* WM_KEYDOWN              */
    EDITWND_ON_CHAR                , /* WM_CHAR                 */
    EDITWND_ON_HSCROLL             , /* WM_HSCROLL              */
    EDITWND_ON_VSCROLL             , /* WM_VSCROLL              */
    EDITWND_ON_SETFOCUS            , /* WM_SETFOCUS             */
    EDITWND_ON_KILLFOCUS           , /* WM_KILLFOCUS            */
    EDITWND_ON_MOUSEACTIVATE       , /* WM_MOUSEACTIVATE        */
    EDITWND_ON_MOUSEWHEEL          , /* WM_MOUSEWHEEL           */
    EDITWND_ON_MOUSEMOVE           , /* WM_MOUSEMOVE            */
    EDITWND_ON_LBUTTONDOWN         , /* WM_LBUTTONDOWN          */
    EDITWND_ON_MBUTTONDOWN         , /* WM_MBUTTONDOWN          */
    EDITWND_ON_RBUTTONDOWN         , /* WM_RBUTTONDOWN          */
    EDITWND_ON_LBUTTONUP           , /* WM_LBUTTONUP            */
    EDITWND_ON_MBUTTONUP           , /* WM_MBUTTONUP            */
    EDITWND_ON_RBUTTONUP           , /* WM_RBUTTONUP            */
    EDITWND_ON_IME_STARTCOMPOSITION, /* WM_IME_STARTCOMPOSITION */
    EDITWND_ON_CUT                 , /* WM_CUT                  */
    EDITWND_ON_COPY                , /* WM_COPY                 */
    EDITWND_ON_PASTE               , /* WM_PASTE                */
    EDITWND_ON_CLEAR               , /* WM_CLEAR                */
    EDITWND_ON_UNDO                , /* WM_UNDO                 */
    EDITWND_ON_SETSEL              , /* EM_SETSEL               */
    EDITWND_ON_SETFONT             , /* WM_SETFONT              */
    EDITWND_ON_GETLINECOUNT        , /* EM_GETLINECOUNT         */
    EDITWND_ON_LINEFROMCHAR        , /* EM_LINEFROMCHAR         */
    EDITWND_ON_DEFAULT             , /* default                 */
    EDITWND_MAX
} EDITWND_INDEX;

#if 1
#define BG_COLOR_RGB          RGB(0xFF,0xFF,0xFF)
#define TAB_BK_COLOR_RGB      RGB(0xE0,0xFF,0xFF) /* êÖêF */
#define BK_COLOR_RGB          RGB(0xFF,0xFF,0xFF)
#define TEXT_COLOR_RGB        RGB(0x00,0x00,0x00)
#define SELECT_BK_COLOR_RGB   RGB(0x00,0x0D,0x7F) /* îZÇ¢ê¬ */
#define SELECT_TEXT_COLOR_RGB RGB(0xFF,0xFF,0xFF)
#else
#define BG_COLOR_RGB          RGB(0x2F,0x4F,0x4F)
#define TAB_BK_COLOR_RGB      RGB(0x37,0x57,0x57)
#define BK_COLOR_RGB          RGB(0x2F,0x4F,0x4F)
#define TEXT_COLOR_RGB        RGB(0xFF,0xDA,0xB9)
#define SELECT_BK_COLOR_RGB   RGB(0x00,0x0D,0x7F) /* îZÇ¢ê¬ */
#define SELECT_TEXT_COLOR_RGB TEXT_COLOR_RGB
#endif

#define IDM_EDIT_UNDO                   40011
#define IDM_EDIT_CUT                    40012
#define IDM_EDIT_COPY                   40013
#define IDM_EDIT_PASTE                  40014
#define IDM_EDIT_DELETE                 40015
#define IDM_EDIT_SELECT_ALL             40020

#define EDITWND_DEF_H
#endif /* EDITWND_DEF_H */
