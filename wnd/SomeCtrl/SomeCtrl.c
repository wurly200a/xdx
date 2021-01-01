/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */

/* �O���֐���` */

/* �O���ϐ���` */

/* �����֐���` */
#include "SomeCtrl.h"
/* �����ϐ���` */

typedef struct
{
    SOME_CTRL_GROUP_ID groupId;
    PTSTR              class;       /* �E�B���h�E�N���X��                         */
    PTSTR              value;       /* �E�B���h�E�e�L�X�g                         */
    int                x;           /* �N���C�A���g�̈�� x���W                   */
    int                y;           /* �N���C�A���g�̈�� y���W                   */
    int                width;       /* ������                                     */
    int                height;      /* ��������                                   */
    BOOL               bSizeAdj;    /* �T�C�Y�������s����                         */
    int                widthOfst;   /* �N���C�A���g�̈�̕��T�C�Y����̃I�t�Z�b�g */
    int                heightOfst;  /* 1�������̍�������̃I�t�Z�b�g              */
    DWORD              exStyle;     /* �g���X�^�C��                               */
    DWORD              style;       /* �X�^�C��                                   */
} S_SOME_CTRL;

static S_SOME_CTRL ctrlListTbl[SOME_CTRL_MAX] =
{
    /*                        , class            , value              , x , y  , w  , h ,bSAdj  ,wOfst, hOfst,exStyle,style  */
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("static")   , TEXT("MIDI IN")    ,365, 15,  80 , 15 ,TRUE  ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("combobox") , TEXT("")           ,430, 10, 200 , 300,FALSE ,0    , 0    ,0                ,(WS_CHILD|CBS_SORT|CBS_DROPDOWNLIST)    },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("button")   , TEXT("Open")       ,645, 10,  40 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("static")   , TEXT("MIDI OUT")   ,365, 35,  80 , 15 ,TRUE  ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("combobox") , TEXT("")           ,430, 30, 200 , 300,FALSE ,0    , 0    ,0                ,(WS_CHILD|CBS_SORT|CBS_DROPDOWNLIST)    },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("button")   , TEXT("Open")       ,645, 30,  40 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_DISABLE  , TEXT("static")   , TEXT("MIDI KEY IN"),365, 55,  80 , 15 ,TRUE  ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_DISABLE  , TEXT("combobox") , TEXT("")           ,430, 50, 200 , 300,FALSE ,0    , 0    ,0                ,(WS_CHILD|CBS_SORT|CBS_DROPDOWNLIST)    },
    {SOME_CTRL_GROUP_DISABLE  , TEXT("button")   , TEXT("Open")       ,645, 50,  40 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_DISABLE  , TEXT("button")   , TEXT("DEBUG")      ,720, 30,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },

    {SOME_CTRL_GROUP_ALWAYS   , TEXT("button")   , TEXT("MODE")       , 10,  5, 200 , 45 ,FALSE ,0    , 0    ,WS_EX_TRANSPARENT,(WS_CHILD|BS_GROUPBOX)                  },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("button")   , TEXT("1VOICE")     , 20, 20,  70 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_ALWAYS   , TEXT("button")   , TEXT("ALL VOICE")  ,120, 20,  70 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_DISABLE  , TEXT("button")   , TEXT("SYSTEM")     ,240, 20,  70 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },

    {SOME_CTRL_GROUP_1VOICE   , TEXT("button")   , TEXT("ACTION")     , 10, 60, 350 , 45 ,FALSE ,0    , 0    ,WS_EX_TRANSPARENT,(WS_CHILD|BS_GROUPBOX)                  },
    {SOME_CTRL_GROUP_1VOICE   , TEXT("button")   , TEXT("LOAD")       , 20, 75,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_1VOICE   , TEXT("button")   , TEXT("SAVE")       ,100, 75,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_1VOICE   , TEXT("button")   , TEXT("GET")        ,180, 75,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_1VOICE   , TEXT("button")   , TEXT("SET")        ,260, 75,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
#if 0
    {SOME_CTRL_GROUP_1VOICE   , TEXT("button")   , TEXT("OP4")        ,  5,135, 300 ,280 ,FALSE ,0    , 0    ,WS_EX_TRANSPARENT,(WS_CHILD|BS_GROUPBOX)                  },
    {SOME_CTRL_GROUP_1VOICE   , TEXT("button")   , TEXT("OP2")        ,  5,415, 300 ,280 ,FALSE ,0    , 0    ,WS_EX_TRANSPARENT,(WS_CHILD|BS_GROUPBOX)                  },
    {SOME_CTRL_GROUP_1VOICE   , TEXT("button")   , TEXT("OP3")        ,315,135, 300 ,280 ,FALSE ,0    , 0    ,WS_EX_TRANSPARENT,(WS_CHILD|BS_GROUPBOX)                  },
    {SOME_CTRL_GROUP_1VOICE   , TEXT("button")   , TEXT("OP1")        ,315,415, 300 ,280 ,FALSE ,0    , 0    ,WS_EX_TRANSPARENT,(WS_CHILD|BS_GROUPBOX)                  },
#endif
    {SOME_CTRL_GROUP_ALL_VOICE, TEXT("button")   , TEXT("ACTION")     , 10, 60, 350 , 45 ,FALSE ,0    , 0    ,WS_EX_TRANSPARENT,(WS_CHILD|BS_GROUPBOX)                  },
    {SOME_CTRL_GROUP_ALL_VOICE, TEXT("button")   , TEXT("LOAD")       , 20, 75,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_ALL_VOICE, TEXT("button")   , TEXT("SAVE")       ,100, 75,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_ALL_VOICE, TEXT("button")   , TEXT("GET")        ,180, 75,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_ALL_VOICE, TEXT("button")   , TEXT("SET")        ,260, 75,  50 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },

    {SOME_CTRL_GROUP_SYSTEM   , TEXT("button")   , TEXT("ACTION")     , 10, 60, 350 , 45 ,FALSE ,0    , 0    ,WS_EX_TRANSPARENT,(WS_CHILD|BS_GROUPBOX)                  },
    {SOME_CTRL_GROUP_SYSTEM   , TEXT("button")   , TEXT("GET")        , 50, 75,  40 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
    {SOME_CTRL_GROUP_SYSTEM   , TEXT("button")   , TEXT("SET")        ,150, 75,  40 , 20 ,FALSE ,0    , 0    ,0                ,(WS_CHILD|ES_LEFT)                      },
};

typedef struct
{
    BOOL               exist                        ; /* �E�B���h�E����������Ă��邩�ǂ��� */
    HWND               hwnd                         ;
} S_SOME_CTRL_INFO;
static S_SOME_CTRL_INFO ctrlInfo[SOME_CTRL_MAX];

/********************************************************************************
 * ���e  : �o�^���ꂽ�S�ẴR���g���[���𐶐�����
 * ����  : HINSTANCE hInst
 * ����  : PTSTR szAppName
 * ����  : HWND hwnd �e�E�B���h�E�̃n���h��
 * �߂�l: BOOL
 ***************************************/
BOOL
SomeCtrlCreate( HINSTANCE hInst, PTSTR szAppName, HWND hwnd )
{
    int i,j,iCbNum;
    TCHAR szBuff[128];

    for(i=0; i<SOME_CTRL_MAX; i++)
    {
        S_SOME_CTRL *tblPtr = &(ctrlListTbl[i]);
        S_SOME_CTRL_INFO *infoPtr = &(ctrlInfo[i]);

        infoPtr->hwnd = CreateWindowEx( tblPtr->exStyle,                  /* �g���X�^�C��       */
                                       tblPtr->class,                    /* �N���X��           */
                                       tblPtr->value,                    /* �E�B���h�E�e�L�X�g */
                                       tblPtr->style,                    /* �X�^�C��           */
                                       tblPtr->x,tblPtr->y,       /* x���W,y���W        */
                                       tblPtr->width,                    /* ��                 */
                                       tblPtr->height,                   /* ����               */
                                       hwnd,(HMENU)(SOME_CTRL_ID_OFFSET+i),     /* �e�E�B���h�E,�q�E�B���h�EID */
                                       hInst,NULL );                       /* �C���X�^���X�n���h��,�⏕���� */
        if( infoPtr->hwnd != NULL )
        {
            SendMessage(infoPtr->hwnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));

            /* �ʂɏ���������� */
#if 0
            switch( i )
            {
            case SOME_CTRL_PATCH_NUM_SELECT:
                iCbNum = SendMessage(infoPtr->hwnd , CB_ADDSTRING, 0, (LPARAM)"Temporary");
                SendMessage( infoPtr->hwnd , CB_SETITEMDATA, iCbNum, (LPARAM)0 );
                for(j=1; j<=128; j++ )
                {
                    wsprintf(szBuff,TEXT("%d"),j);
                    iCbNum = SendMessage(infoPtr->hwnd , CB_ADDSTRING, 0, (LPARAM)&szBuff[0]);
                    SendMessage( infoPtr->hwnd , CB_SETITEMDATA, iCbNum, (LPARAM)j );
                }
                SendMessage( infoPtr->hwnd , CB_SETCURSEL, 0, (LPARAM)0 );
                break;
            default:
                break;
            }
#endif

            if( tblPtr->groupId == SOME_CTRL_GROUP_ALWAYS )
            {
                ShowWindow(infoPtr->hwnd, SW_SHOW);
            }
            else
            {
                nop();
            }
            infoPtr->exist = TRUE;
        }
        else
        {
            /* do nothing */
        }
    }

    return TRUE;
}

/********************************************************************************
 * ���e  : �w��̃O���[�v�̃R���g���[����\������
 * ����  : SOME_CTRL_GROUP_ID groupId
 * �߂�l: BOOL
 ***************************************/
BOOL
SomeCtrlGroupDisplay( SOME_CTRL_GROUP_ID groupId )
{
    SOME_CTRL_ID id;

    for(id=0; id<SOME_CTRL_MAX; id++)
    {
        S_SOME_CTRL *tblPtr = &(ctrlListTbl[id]);
        S_SOME_CTRL_INFO *infoPtr = &(ctrlInfo[id]);

        if( (tblPtr->groupId == SOME_CTRL_GROUP_ALWAYS) ||
            (tblPtr->groupId == groupId) )
        {
            ShowWindow(infoPtr->hwnd, SW_SHOW);
        }
        else
        {
            ShowWindow(infoPtr->hwnd, SW_HIDE);
        }
    }

    switch( groupId )
    {
    case SOME_CTRL_GROUP_1VOICE   :
        SomeCtrlDisable( SOME_CTRL_MODE_1VOICE    );
        SomeCtrlEnable ( SOME_CTRL_MODE_ALL_VOICE );
        break;
    case SOME_CTRL_GROUP_ALL_VOICE:
        SomeCtrlEnable ( SOME_CTRL_MODE_1VOICE    );
        SomeCtrlDisable( SOME_CTRL_MODE_ALL_VOICE );
        break;
    default:
        nop();
        break;
    }

    return TRUE;
}

/********************************************************************************
 * ���e  : �o�^���ꂽ�S�ẴR���g���[���̃T�C�Y�𒲐�����
 * ����  : int cxClient
 * ����  : int cyClient
 * �߂�l: LONG �R���g���[����z�u�����`�̏c�����̃T�C�Y
 ***************************************/
LONG
SomeCtrlSize( int cxClient,int cyClient )
{
    int i;

    for(i=0; i<SOME_CTRL_MAX; i++)
    {
        S_SOME_CTRL *tblPtr = &(ctrlListTbl[i]);
        S_SOME_CTRL_INFO *infoPtr = &(ctrlInfo[i]);

        if( tblPtr->bSizeAdj )
        {
            MoveWindow( infoPtr->hwnd, tblPtr->x,tblPtr->y, (cxClient + tblPtr->widthOfst),cyClient + tblPtr->heightOfst, TRUE);
        }
        else
        {
            /* do nothing */
        }
    }

    return (LONG)700;
}

/********************************************************************************
 * ���e  : �R���g���[���̃E�B���h�E�n���h�����擾����
 * ����  : SOME_CTRL_ID id
 * �߂�l: HWND
 ***************************************/
HWND
SomeCtrlGetHWND( SOME_CTRL_ID id )
{
    HWND rtn;

    if( ctrlInfo[id].exist == TRUE )
    {
        rtn = ctrlInfo[id].hwnd;
    }
    else
    {
        rtn = (HWND)0;
    }

    return rtn;
}

/********************************************************************************
 * ���e  : �R���g���[����L���ɂ���
 * ����  : SOME_CTRL_ID id
 * �߂�l: �Ȃ�
 ***************************************/
void
SomeCtrlEnable( SOME_CTRL_ID id )
{
    EnableWindow( ctrlInfo[id].hwnd, TRUE);
}

/********************************************************************************
 * ���e  : �R���g���[���𖳌��ɂ���
 * ����  : SOME_CTRL_ID id
 * �߂�l: �Ȃ�
 ***************************************/
void
SomeCtrlDisable( SOME_CTRL_ID id )
{
    EnableWindow( ctrlInfo[id].hwnd, FALSE);
}

/********************************************************************************
 * ���e  : �E�B���h�E����e�L�X�g���擾����
 * ����  : SOME_CTRL_ID id
 * ����  : PTSTR ptstrText
 * �߂�l: BOOL
 ***************************************/
BOOL
SomeCtrlGetText( SOME_CTRL_ID id, PTSTR ptstrText )
{
    BOOL rtn = FALSE;
    INT iLength;

    if( ctrlInfo[id].exist == TRUE )
    {
        iLength = GetWindowTextLength( ctrlInfo[id].hwnd );
        GetWindowText( ctrlInfo[id].hwnd, ptstrText, iLength + 1 );
        rtn = TRUE;
    }
    else
    {
        /* error */
    }

    return rtn;
}
