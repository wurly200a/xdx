/* ���ʃC���N���[�h�t�@�C�� */
#include "common.h"
/* �ʃC���N���[�h�t�@�C�� */

/* �O���֐���` */
#include "WinMain.h"

/* �O���ϐ���` */

/* �����֐���` */
#include "ParamCtrl.h"

static BOOL paramCtrlCreate( HWND hwnd, PARAM_CTRL_GROUP_ID groupId, PARAM_CTRL_ID startId, PARAM_CTRL_ID endId );
/* �����ϐ���` */

enum
{
   PCT_COMBO,
   PCT_EDIT ,
   PCT_MAX
};
typedef BYTE PARAM_CTRL_TYPE;


typedef struct
{
    PARAM_CTRL_TYPE type          ;
    PTSTR           strText       ; /* �E�B���h�E�e�L�X�g                         */
    INT             minValue      ;
    INT             maxValue      ;
    INT             xBasePosOffset;
} S_PARAM_CTRL;

#define X_BASE_POS_OFFSET 250
S_PARAM_CTRL paramListTbl[PARAM_CTRL_MAX] =
{
    {PCT_COMBO,TEXT("SOUNDMODE                "),0,  2,170},

    {PCT_COMBO,TEXT("00 OP4 ATTACK RATE"                            ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("01 OP4 DECAY 1 RATE"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("02 OP4 DECAY 2 RATE"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("03 OP4 RELEASE RATE"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("04 OP4 DECAY 1 LEVEL"                          ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("05 OP4 KEYBOARD SCALING LEVEL"                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("06 OP4 KEYBOARD SCALING RATE"                  ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("07 OP4 EG BIAS SENSITIVITY"                    ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("08 OP4 AMPLITUDE MODULATION ENABLE"            ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("09 OP4 KEY VELOCITY"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("10 OP4 OUTPUT LEVEL"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("11 OP4 OSCILLATOR FREQUENCY"                   ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("12 OP4 DETUNE"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("13 OP3 ATTACK RATE"                            ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("14 OP3 DECAY 1 RATE"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("15 OP3 DECAY 2 RATE"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("16 OP3 RELEASE RATE"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("17 OP3 DECAY 1 LEVEL"                          ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("18 OP3 KEYBOARD SCALING LEVEL"                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("19 OP3 KEYBOARD SCALING RATE"                  ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("20 OP3 EG BIAS SENSITIVITY"                    ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("21 OP3 AMPLITUDE MODULATION ENABLE"            ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("22 OP3 KEY VELOCITY"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("23 OP3 OUTPUT LEVEL"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("24 OP3 OSCILLATOR FREQUENCY"                   ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("25 OP3 DETUNE"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("26 OP2 ATTACK RATE"                            ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("27 OP2 DECAY 1 RATE"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("28 OP2 DECAY 2 RATE"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("29 OP2 RELEASE RATE"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("30 OP2 DECAY 1 LEVEL"                          ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("31 OP2 KEYBOARD SCALING LEVEL"                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("32 OP2 KEYBOARD SCALING RATE"                  ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("33 OP2 EG BIAS SENSITIVITY"                    ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("34 OP2 AMPLITUDE MODULATION ENABLE"            ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("35 OP2 KEY VELOCITY"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("36 OP2 OUTPUT LEVEL"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("37 OP2 OSCILLATOR FREQUENCY"                   ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("38 OP2 DETUNE"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("39 OP1 ATTACK RATE"                            ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("40 OP1 DECAY 1 RATE"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("41 OP1 DECAY 2 RATE"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("42 OP1 RELEASE RATE"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("43 OP1 DECAY 1 LEVEL"                          ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("44 OP1 KEYBOARD SCALING LEVEL"                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("45 OP1 KEYBOARD SCALING RATE"                  ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("46 OP1 EG BIAS SENSITIVITY"                    ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("47 OP1 AMPLITUDE MODULATION ENABLE"            ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("48 OP1 KEY VELOCITY"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("49 OP1 OUTPUT LEVEL"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("50 OP1 OSCILLATOR FREQUENCY"                   ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("51 OP1 DETUNE"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("52 ALGORITHM"                                  ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("53 FEEDBACK LEVEL"                             ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("54 LFO SPEED"                                  ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("55 LFO DELAY"                                  ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("56 PITCH MODULATION DEPTH"                     ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("57 AMPLITUDE MODULATION DEPTH"                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("58 LFO SYNC"                                   ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("59 LFO WAVE"                                   ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("60 PITCH MODULATION SENSITIVITY"               ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("61 AMPLITUDE MODULATION SENSITIVITY"           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("62 TRANSPOSE"                                  ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("63 PLAY MODE POLY/MONO"                        ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("64 PITCH BEND RANGE"                           ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("65 PORTAMENTO MODE"                            ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("66 PORTAMENTO TIME"                            ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("67 FOOT VOLUME RANGE"                          ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("68 SUSTAIN FOOT SWITCH"                        ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("69 PORTAMENT FOOT SWITCH"                      ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("70 CHORUS SWITCH"                              ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("71 MODULATION WHEEL PITCH MODULATION RANGE"    ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("72 MODULATION WHEEL AMPLITUDE MODULATION RANGE"),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("73 BREATH CONTROL PITCH MODULATION RANGE"      ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("74 BREATH CONTROL AMPLITUDE MODULATION RANGE"  ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("75 BREATH CONTROL PITCH BIAS RANGE"            ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("76 BREATH CONTROL EG BIAS RANGE"               ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("PATCHNAME"                                     ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("PITCH EG RATE 1"                               ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("PITCH EG RATE 2"                               ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("PITCH EG RATE 3"                               ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("PITCH EG LEVEL 1"                              ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("PITCH EG LEVEL 2"                              ),0,127,X_BASE_POS_OFFSET},
    {PCT_COMBO,TEXT("PITCH EG LEVEL 3"                              ),0,127,X_BASE_POS_OFFSET},

    {PCT_EDIT ,TEXT("VOICE_NAME_00"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_01"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_02"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_03"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_04"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_05"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_06"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_07"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_08"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_09"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_10"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_11"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_12"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_13"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_14"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_15"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_16"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_17"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_18"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_19"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_20"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_21"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_22"                                 ),0,127,X_BASE_POS_OFFSET},
    {PCT_EDIT ,TEXT("VOICE_NAME_23"                                 ),0,127,X_BASE_POS_OFFSET},
};

typedef struct
{
    INT xPos  ;
    INT yPos  ;
    INT width ;
    INT height;
} S_PARAM_WINDOW_INFO;

typedef struct
{
    BOOL                exist                        ; /* �E�B���h�E����������Ă��邩�ǂ��� */
    HWND                hwnd_name                    ; /* �E�B���h�E�̃n���h��               */
    HWND                hwnd_data                    ; /* �E�B���h�E�̃n���h��               */
    S_PARAM_WINDOW_INFO wtInfo[PARAM_CTRL_TARGET_MAX];
    PARAM_CTRL_GROUP_ID groupId                      ;
} S_PARAM_INFO;

S_PARAM_INFO ctrlParamInfo[PARAM_CTRL_MAX];

#define PARAM_STATIC_X_BASE_POS  10
#define PARAM_STATIC_Y_BASE_POS 150
#define PARAM_STATIC_WIDTH      150
#define PARAM_STATIC_HEIGHT      15

#define PARAM_STATIC_Y_UNIT      20

#define PARAM_DATA_Y_BASE_POS  150
#define PARAM_DATA_WIDTH        40
#define PARAM_DATA_HEIGHT      300

#define PARAM_DATA_Y_UNIT       20

#define PARAM_WRAP_NUM      26 /* 1�񂠂���̍s�� */
#define PARAM_WRAP_UNIT    /*220*/ 300      /* ��̃s�b�` */

/********************************************************************************
 * ���e  : �o�^���ꂽ�S�Ẵp�����[�^�R���g���[���𐶐�����
 * ����  : HWND hwnd �e�E�B���h�E�̃n���h��
 * �߂�l: BOOL
 ***************************************/
BOOL
ParamCtrlCreate( HWND hwnd )
{
    paramCtrlCreate(hwnd,PARAM_CTRL_GROUP_SYSTEM_COMMON,PARAM_CTRL_SYSCMN_SOUNDMODE  ,PARAM_CTRL_SYSCMN_SOUNDMODE  );
    paramCtrlCreate(hwnd,PARAM_CTRL_GROUP_1VOICE       ,PARAM_CTRL_VOICE_00          ,PARAM_CTRL_VOICE_92          );
    paramCtrlCreate(hwnd,PARAM_CTRL_GROUP_ALL_VOICE    ,PARAM_CTRL_ALL_VOICE_NAME_00 ,PARAM_CTRL_ALL_VOICE_NAME_23 );

    ParamCtrlGroupDisplay(PARAM_CTRL_GROUP_SYSTEM_COMMON);

    return TRUE;
}

/********************************************************************************
 * ���e  : �o�^���ꂽ�S�Ẵp�����[�^�R���g���[���𐶐�����
 * ����  : HWND hwnd �e�E�B���h�E�̃n���h��
 * ����  : PARAM_CTRL_GROUP_ID groupId
 * ����  : PARAM_CTRL_ID       startId
 * ����  : PARAM_CTRL_ID       endId
 * �߂�l: BOOL
 ***************************************/
static BOOL
paramCtrlCreate( HWND hwnd, PARAM_CTRL_GROUP_ID groupId, PARAM_CTRL_ID startId, PARAM_CTRL_ID endId )
{
    int nowId,numForDisp;
    INT columnNum = 0;

    if( (startId <= endId) && (endId < PARAM_CTRL_MAX) )
    {
        for(nowId=startId,numForDisp=0; nowId<=endId; nowId++,numForDisp++)
        {
            PTSTR class;
            DWORD style;
            DWORD exStyle;
            DWORD height;
            INT   width;
            INT xPos;
            S_PARAM_INFO *infoPtr = &ctrlParamInfo[nowId];
            S_PARAM_CTRL *tblPtr = &paramListTbl[nowId];

            if( !numForDisp || numForDisp%PARAM_WRAP_NUM )
            {
                nop();
            }
            else
            {
                columnNum++;
            }

            if( tblPtr->type == PCT_COMBO )
            {
                class = (PTSTR)TEXT("combobox");
                style = (WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
                exStyle = 0;
                height = PARAM_DATA_HEIGHT;
                width  = PARAM_DATA_WIDTH;
            }
            else
            {
                class = (PTSTR)TEXT("edit");
                style = (WS_CHILD|ES_LEFT);
                exStyle = WS_EX_OVERLAPPEDWINDOW;
                height = 20;
                width  = 100;
            }

            /* �X�^�e�B�b�N�R���g���[��(��������) */
            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].xPos   = PARAM_STATIC_X_BASE_POS+(PARAM_WRAP_UNIT*columnNum);
            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].yPos   = PARAM_STATIC_Y_BASE_POS+(PARAM_STATIC_Y_UNIT*(numForDisp-(PARAM_WRAP_NUM*columnNum)));

//        infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].width  = PARAM_STATIC_WIDTH;
            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].width  = strlen(tblPtr->strText)*8;

            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].height = PARAM_STATIC_HEIGHT;
            infoPtr->hwnd_name = CreateWindowEx( 0,                                              /* �g���X�^�C��       */
                                                TEXT("static"),                                 /* �N���X��           */
                                                tblPtr->strText,                        /* �E�B���h�E�e�L�X�g */
                                                (WS_CHILD|ES_LEFT),                             /* �X�^�C��           */
                                                infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].xPos,   /* x���W              */
                                                infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].yPos,   /* y���W              */
                                                infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].width,  /* ��                 */
                                                infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].height, /* ����               */
                                                hwnd,(HMENU)(PARAM_CTRL_ID_OFFSET+nowId*2),         /* �e�E�B���h�E,�q�E�B���h�EID */
                                                GetHinst(),NULL );                              /* �C���X�^���X�n���h��,�⏕���� */
            if( infoPtr->hwnd_name != NULL )
            {
                SendMessage(infoPtr->hwnd_name, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));

                /* �ʂɏ���������� */
                switch( nowId )
                {
                default:
                    break;
                }

                ShowWindow(infoPtr->hwnd_name, SW_HIDE);
            }
            else
            {
                /* do nothing */
            }
            /* �X�^�e�B�b�N�R���g���[��(�����܂�) */

            /* �p�����[�^���Ǘ�����R���g���[��(��������) */
            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].xPos   = tblPtr->xBasePosOffset+(PARAM_WRAP_UNIT*columnNum);
            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].yPos   = PARAM_DATA_Y_BASE_POS+(PARAM_DATA_Y_UNIT*(numForDisp-(PARAM_WRAP_NUM*columnNum)));
            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].width  = width;
            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].height = height;

            infoPtr->hwnd_data = CreateWindowEx( exStyle,                                    /* �g���X�^�C��       */
                                                class,                                      /* �N���X��           */
                                                0,                                          /* �E�B���h�E�e�L�X�g */
                                                style,                                      /* �X�^�C��           */
                                                infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].xPos,   /* x���W              */
                                                infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].yPos,   /* y���W              */
                                                infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].width,  /* ��                 */
                                                infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].height, /* ����               */
                                                hwnd,(HMENU)(PARAM_CTRL_ID_OFFSET+(nowId*2)+1),/* �e�E�B���h�E,�q�E�B���h�EID */
                                                GetHinst(),NULL );                        /* �C���X�^���X�n���h��,�⏕���� */
            if( infoPtr->hwnd_data != NULL )
            {
                INT j,iCbNum;
                TCHAR szBuff[128];

                SendMessage(infoPtr->hwnd_data, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));

                /* �ʂɏ���������� */
                switch( nowId )
                {
                default:
                    break;
                }

                ShowWindow(infoPtr->hwnd_data, SW_HIDE);

                if( tblPtr->type == PCT_COMBO )
                {
                    for(j=tblPtr->minValue; j<=tblPtr->maxValue; j++ )
                    {
                        wsprintf(szBuff,TEXT("%d"),j);
                        iCbNum = SendMessage(infoPtr->hwnd_data , CB_ADDSTRING, 0, (LPARAM)&szBuff[0]);
                        SendMessage( infoPtr->hwnd_data , CB_SETITEMDATA, iCbNum, (LPARAM)j );
                    }
                    SendMessage( infoPtr->hwnd_data , CB_SETCURSEL, 0, (LPARAM)0 );
                }
                else
                {
                    nop();
                }
            }
            else
            {
                /* do nothing */
            }
            infoPtr->exist = TRUE;
            infoPtr->groupId = groupId;
        }
    }
    else
    {
        nop();
    }

    return TRUE;
}

/********************************************************************************
 * ���e  : �w��̃O���[�v�̃p�����[�^�R���g���[����\��
 * ����  : PARAM_CTRL_GROUP_ID groupId
 * �߂�l: BOOL
 ***************************************/
BOOL
ParamCtrlGroupDisplay( PARAM_CTRL_GROUP_ID groupId )
{
    int nowId;

    for(nowId=0; nowId<PARAM_CTRL_MAX; nowId++ )
    {
        S_PARAM_INFO *infoPtr = &ctrlParamInfo[nowId];

        if( infoPtr->groupId == groupId )
        {
            ShowWindow(infoPtr->hwnd_name, SW_SHOW);
            ShowWindow(infoPtr->hwnd_data, SW_SHOW);
        }
        else
        {
            ShowWindow(infoPtr->hwnd_name, SW_HIDE);
            ShowWindow(infoPtr->hwnd_data, SW_HIDE);
        }
    }

    return TRUE;
}


/********************************************************************************
 * ���e  : �E�B���h�E�n���h�����擾����
 * ����  : PARAM_CTRL_ID id
 * �߂�l: HWND
 ***************************************/
HWND
ParamCtrlGetHWND( PARAM_CTRL_ID id )
{
    HWND rtn = (HWND)0;

    if( id < PARAM_CTRL_MAX )
    {
        S_PARAM_INFO *infoPtr = &ctrlParamInfo[id];

        if( infoPtr->exist == TRUE )
        {
            rtn = infoPtr->hwnd_data;
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

    return rtn;
}

/********************************************************************************
 * ���e  : �R���g���[���̃T�C�Y�𒲐�����
 * ����  : PARAM_CTRL_ID id
 * ����  : PARAM_CTRL_TARGET target
 * ����  : INT           width
 * �߂�l: �Ȃ�
 ***************************************/
void
ParamCtrlSetSize( PARAM_CTRL_ID id, PARAM_CTRL_TARGET target, INT width )
{
    int i;

    if( (id < PARAM_CTRL_MAX) && (target < PARAM_CTRL_TARGET_MAX) )
    {
        for(i=0; i<PARAM_CTRL_MAX; i++)
        {
            S_PARAM_INFO *infoPtr = &ctrlParamInfo[i];

            if( target == PARAM_CTRL_TARGET_NAME )
            {
                MoveWindow( infoPtr->hwnd_name,
                            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].xPos,   /* x���W              */
                            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].yPos,   /* y���W              */
                            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].width,  /* ��                 */
                            infoPtr->wtInfo[PARAM_CTRL_TARGET_NAME].height, /* ����               */
                            TRUE);
            }
            else
            {
                MoveWindow( infoPtr->hwnd_data,
                            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].xPos,   /* x���W              */
                            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].yPos,   /* y���W              */
                            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].width,  /* ��                 */
                            infoPtr->wtInfo[PARAM_CTRL_TARGET_DATA].height, /* ����               */
                            TRUE);
            }
        }
    }
    else
    {
        nop();
    }
}
