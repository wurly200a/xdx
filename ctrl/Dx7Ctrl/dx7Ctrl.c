/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */
#include <math.h>
#include "SomeCtrl.h"
#include "dx7ParamCtrl.h"
#include "Midi.h"
#include "dx7Ctrl.h"
#include "DebugWnd.h"
#include "dx7CtrlDef.h"

/* 外部関数定義 */

/* 外部変数定義 */

/* 内部関数定義 */
static void dx7OneVoiceDataInit( void );
static void dx7AllVoiceDataInit( void );
static BOOL seqStart( DX7_CTRL_SEQ_METHOD method, DX7_CTRL_SEQ_ID seqId );
static INT seqStartProc( DX7_CTRL_SEQ_METHOD method, DX7_CTRL_SEQ_ID seqId, INT maxDataSize, BYTE *txDataPtr );
static BOOL seqEndProc( DX7_CTRL_SEQ_ID seqId, INT rxDataSize, BYTE *rxDataPtr );
static BOOL copyToParamCtrl( DX7_CTRL_SEQ_ID seqId );
static BOOL copyFromParamCtrl( DX7_CTRL_SEQ_ID seqId );
static BYTE getParamCtrlValue( DX7_PARAM_CTRL_ID id );
static BOOL displayContents( void );
static BYTE calcCheckSum( BYTE *dataPtr, INT dataSize );
static BOOL dx7voiceCopyFromAllToOne( INT voiceNum );
static BOOL dx7voiceCopyFromOneToOneOfAll( INT voiceNum );
static void dx7SetFrequencyText( DX7_PARAM_CTRL_ID modeId, DX7_PARAM_CTRL_ID coarseId, DX7_PARAM_CTRL_ID fineId, DX7_PARAM_CTRL_ID valueId);
static void debugDataArrayPrint( INT rxDataSize, BYTE *rxDataPtr, PTSTR ptstrTitle );

/* 内部変数定義 */
static BYTE dx7CtrlSeqTxTempData[1024*8];
static BYTE dx7CtrlSeqRxTempData[1024*8];
static INT dx7CtrlSeqTempRxSize;

static BYTE dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_INDEX_MAX];
static BYTE dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_INDEX_MAX];

static S_DX7_VOICE_PARAM dx7voiceParam;

typedef struct
{
    INT   rxDataSize  ; /* 受信データ格納領域のサイズ               */
    BYTE *rxDataPtr   ; /* 受信データ格納領域のポインタ             */
} S_DX7_CTRL_SEQ_DATA;

static const S_DX7_CTRL_SEQ_DATA dx7CtrlSeqDataTbl[DX7_CTRL_SEQ_NUM_MAX] =
{
    {(INT)0                            ,(BYTE *)NULL                 }, /* DX7_CTRL_SEQ_NON_EXEC     */
    {(INT)DX7_SYSEX_1VOICE_INDEX_MAX   ,&dx7CtrlDataOneVoice         }, /* DX7_CTRL_SEQ_1VOICE */
    {(INT)DX7_SYSEX_ALL_VOICE_INDEX_MAX,&dx7CtrlDataAllVoice         }, /* DX7_CTRL_SEQ_ALL_VOICE */
};

#define DX7_ALGORITHM_MAX 32

typedef struct
{
    HINSTANCE hInst;
    HWND      hwnd ;
    HWND      hctrl[DX7_ALGORITHM_MAX];
    DX7_CTRL_SEQ_METHOD          nowMethod            ;
    DX7_CTRL_SEQ_ID              reqSeqIdStart        ;
    DX7_CTRL_SEQ_ID              reqSeqIdEnd          ;
    DX7_CTRL_SEQ_ID              nowExecSeqId         ;
    DX7_CTRL_MODE                nowMode              ;
} S_DX7_CTRL_INFO;

static S_DX7_CTRL_INFO dx7CtrlInfo;


/*********************************************
 * 内容   : 初期化
 * 引数   : HINSTANCE hInst
 * 引数   : PTSTR szAppName
 * 引数   : HWND hwnd
 * 戻り値 : BOOL
 **********************************************/
BOOL
Dx7CtrlInit( HINSTANCE hInst, PTSTR szAppName, HWND hwnd )
{
    BOOL bRtn = TRUE;

    dx7CtrlInfo.hInst = hInst;
    dx7CtrlInfo.hwnd  = hwnd ;
    dx7CtrlInfo.nowMode = DX7_CTRL_MODE_PATCH;

    {
        static HBITMAP hBitmap[DX7_ALGORITHM_MAX];
        INT i;
        for( i=0; i<DX7_ALGORITHM_MAX; i++ )
        {
            TCHAR szBuff[128];

            sprintf(szBuff,"DX7_%02d",i+1);
            dx7CtrlInfo.hctrl[i] = CreateWindow( TEXT("Static"), TEXT(""), WS_CHILD|WS_VISIBLE|SS_BITMAP,900,0,0,0, dx7CtrlInfo.hwnd, (HMENU)-1, dx7CtrlInfo.hInst, NULL);
            hBitmap[i] = (HBITMAP)LoadImage(dx7CtrlInfo.hInst, szBuff, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR );
            SendMessage(dx7CtrlInfo.hctrl[i], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[i]);
            ShowWindow(dx7CtrlInfo.hctrl[i], SW_HIDE);
        }
    }

    Dx7ParamCtrlCreate( hInst, szAppName, hwnd ); /* コントロールを生成 */

    /* 1VOICE 初期値 */
    dx7OneVoiceDataInit();
    copyToParamCtrl(DX7_CTRL_SEQ_1VOICE);

    /* ALL VOICE 初期値 */
    dx7AllVoiceDataInit();
    copyToParamCtrl(DX7_CTRL_SEQ_ALL_VOICE);

    return bRtn;
}

/*********************************************
 * 内容   : 1VOICEデータ初期化
 * 引数   : なし
 * 戻り値 : なし
 **********************************************/
static void
dx7OneVoiceDataInit( void )
{
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_REQUEST_STATUS    ] = EX_STATUS                 ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_REQUEST_ID_NO     ] = EX_ID_NUMBER_YAMAHA       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_REQUEST_SUB_STATUS] = 0x00                      ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_REQUEST_FORMAT_NO ] = DX7_DUMP_REQ_FORMAT_1VOICE;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_BYTE_COUNT_HIGH   ] = 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_BYTE_COUNT_LOW    ] = 0x9B; /* 155*/
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_00 ] /*R1    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_01 ] /*R2    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_02 ] /*R3    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_03 ] /*R4    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_04 ] /*L1    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_05 ] /*L2    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_06 ] /*L3    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_07 ] /*L4    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_08 ] /*BP    */= 0x27;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_09 ] /*LD    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_10 ] /*RD    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_11 ] /*LC    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_12 ] /*RC    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_13 ] /*RS    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_14 ] /*AMS   */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_15 ] /*TS    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_16 ] /*TL    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_17 ] /*PM    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_18 ] /*PC    */= 0x01;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_19 ] /*PF    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_20 ] /*PD    */= 0x07;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_21 ] /*R1    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_22 ] /*R2    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_23 ] /*R3    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_24 ] /*R4    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_25 ] /*L1    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_26 ] /*L2    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_27 ] /*L3    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_28 ] /*L4    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_29 ] /*BP    */= 0x27;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_30 ] /*LD    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_31 ] /*RD    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_32 ] /*LC    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_33 ] /*RC    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_34 ] /*RS    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_35 ] /*AMS   */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_36 ] /*TS    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_37 ] /*TL    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_38 ] /*PM    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_39 ] /*PC    */= 0x01;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_40 ] /*PF    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_41 ] /*PD    */= 0x07;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_42 ] /*R1    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_43 ] /*R2    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_44 ] /*R3    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_45 ] /*R4    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_46 ] /*L1    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_47 ] /*L2    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_48 ] /*L3    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_49 ] /*L4    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_50 ] /*BP    */= 0x27;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_51 ] /*LD    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_52 ] /*RD    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_53 ] /*LC    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_54 ] /*RC    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_55 ] /*RS    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_56 ] /*AMS   */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_57 ] /*TS    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_58 ] /*TL    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_59 ] /*PM    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_60 ] /*PC    */= 0x01;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_61 ] /*PF    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_62 ] /*PD    */= 0x07;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_63 ] /*R1    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_64 ] /*R2    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_65 ] /*R3    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_66 ] /*R4    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_67 ] /*L1    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_68 ] /*L2    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_69 ] /*L3    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_70 ] /*L4    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_71 ] /*BP    */= 0x27;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_72 ] /*LD    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_73 ] /*RD    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_74 ] /*LC    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_75 ] /*RC    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_76 ] /*RS    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_77 ] /*AMS   */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_78 ] /*TS    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_79 ] /*TL    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_80 ] /*PM    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_81 ] /*PC    */= 0x01;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_82 ] /*PF    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_83 ] /*PD    */= 0x07;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_84 ] /*R1    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_85 ] /*R2    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_86 ] /*R3    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_87 ] /*R4    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_88 ] /*L1    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_89 ] /*L2    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_90 ] /*L3    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_91 ] /*L4    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_92 ] /*BP    */= 0x27;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_93 ] /*LD    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_94 ] /*RD    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_95 ] /*LC    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_96 ] /*RC    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_97 ] /*RS    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_98 ] /*AMS   */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_99 ] /*TS    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_100] /*TL    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_101] /*PM    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_102] /*PC    */= 0x01;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_103] /*PF    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_104] /*PD    */= 0x07;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_105] /*R1    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_106] /*R2    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_107] /*R3    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_108] /*R4    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_109] /*L1    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_110] /*L2    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_111] /*L3    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_112] /*L4    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_113] /*BP    */= 0x27;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_114] /*LD    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_115] /*RD    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_116] /*LC    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_117] /*RC    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_118] /*RS    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_119] /*AMS   */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_120] /*TS    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_121] /*TL    */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_122] /*PM    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_123] /*PC    */= 0x01;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_124] /*PF    */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_125] /*PD    */= 0x07;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_126] /*PR1   */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_127] /*PR2   */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_128] /*PR3   */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_129] /*PR4   */= 0x63;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_130] /*PL1   */= 0x32;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_131] /*PL2   */= 0x32;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_132] /*PL3   */= 0x32;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_133] /*PL4   */= 0x32;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_134] /*ALS   */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_135] /*FBL   */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_136] /*OPI   */= 0x01;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_137] /*LFS   */= 0x23;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_138] /*LFD   */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_139] /*LPMD  */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_140] /*LAMD  */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_141] /*LFKS  */= 0x01;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_142] /*LFW   */= 0x00;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_143] /*LPMS  */= 0x03;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_144] /*TRNP  */= 0x18;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_145] /*VNAM1 */= 0x49;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_146] /*VNAM2 */= 0x4E;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_147] /*VNAM3 */= 0x49;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_148] /*VNAM4 */= 0x54;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_149] /*VNAM5 */= 0x20;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_150] /*VNAM6 */= 0x56;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_151] /*VNAM7 */= 0x4F;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_152] /*VNAM8 */= 0x49;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_153] /*VNAM9 */= 0x43;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_154] /*VNAM10*/= 0x45;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_FOOTER_CHECKSUM]    = 0x67;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_FOOTER_ETX     ]    = EX_ETX;
}

/*********************************************
 * 内容   : ALL VOICEデータ初期化
 * 引数   : なし
 * 戻り値 : なし
 **********************************************/
static void
dx7AllVoiceDataInit( void )
{
    INT topNum,i;

    dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_HEADER_REQUEST_STATUS    ] = EX_STATUS;
    dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_HEADER_REQUEST_ID_NO     ] = EX_ID_NUMBER_YAMAHA;
    dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_HEADER_REQUEST_SUB_STATUS] = 0x00;
    dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_HEADER_REQUEST_FORMAT_NO ] = DX7_DUMP_REQ_FORMAT_32VOICE;
    dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_HEADER_BYTE_COUNT_HIGH   ] = 0x20;
    dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_HEADER_BYTE_COUNT_LOW    ] = 0x00;
    for( i=0; i<32; i++ )
    {
        INT topNum = DX7_SYSEX_ALL_VOICE_DATA + (i*DX7_SYSEX_VMEM_MAX);

        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_00 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_01 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_02 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_03 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_04 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_05 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_06 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_07 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_08 ] = 0x27;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_09 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_10 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_11 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_12 ] = 0x38;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_13 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_14 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_15 ] = 0x02;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_16 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_17 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_18 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_19 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_20 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_21 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_22 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_23 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_24 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_25 ] = 0x27;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_26 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_27 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_28 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_29 ] = 0x38;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_30 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_31 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_32 ] = 0x02;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_33 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_34 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_35 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_36 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_37 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_38 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_39 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_40 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_41 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_42 ] = 0x27;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_43 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_44 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_45 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_46 ] = 0x38;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_47 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_48 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_49 ] = 0x02;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_50 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_51 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_52 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_53 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_54 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_55 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_56 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_57 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_58 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_59 ] = 0x27;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_60 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_61 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_62 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_63 ] = 0x38;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_64 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_65 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_66 ] = 0x02;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_67 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_68 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_69 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_70 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_71 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_72 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_73 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_74 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_75 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_76 ] = 0x27;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_77 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_78 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_79 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_80 ] = 0x38;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_81 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_82 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_83 ] = 0x02;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_84 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_85 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_86 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_87 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_88 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_89 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_90 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_91 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_92 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_93 ] = 0x27;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_94 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_95 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_96 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_97 ] = 0x38;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_98 ] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_99 ] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_100] = 0x02;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_101] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_102] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_103] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_104] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_105] = 0x63;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_106] = 0x32;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_107] = 0x32;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_108] = 0x32;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_109] = 0x32;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_110] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_111] = 0x08;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_112] = 0x23;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_113] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_114] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_115] = 0x00;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_116] = 0x31;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_117] = 0x18;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_118] = 0x49;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_119] = 0x4E;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_120] = 0x49;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_121] = 0x54;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_122] = 0x20;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_123] = 0x56;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_124] = 0x4F;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_125] = 0x49;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_126] = 0x43;
        dx7CtrlDataAllVoice[topNum+DX7_SYSEX_VMEM_127] = 0x45;
    }
    dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_FOOTER_CHECKSUM] = 0x60;
    dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_FOOTER_ETX     ] = EX_ETX;
}

/*********************************************
 * 内容   : 
 * 引数   : DX7_CTRL_MODE mode
 * 戻り値 : BOOL
 **********************************************/
BOOL
Dx7CtrlModeSet(DX7_CTRL_MODE mode)
{
    dx7CtrlInfo.nowMode = mode;

    Dx7CtrlDisplayUpdate();

    return TRUE;
}

BOOL
Dx7CtrlDisplayUpdate( void )
{
    INT i;

    if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_NONE )
    {
        Dx7ParamCtrlGroupDisplay(DX7_PARAM_CTRL_GROUP_NONE);
    }
    else if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_PATCH )
    {
        dx7SetFrequencyText(DX7_PARAM_CTRL_VOICE_17,DX7_PARAM_CTRL_VOICE_18,DX7_PARAM_CTRL_VOICE_19,DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE6);
        dx7SetFrequencyText(DX7_PARAM_CTRL_VOICE_38,DX7_PARAM_CTRL_VOICE_39,DX7_PARAM_CTRL_VOICE_40,DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE5);
        dx7SetFrequencyText(DX7_PARAM_CTRL_VOICE_59,DX7_PARAM_CTRL_VOICE_60,DX7_PARAM_CTRL_VOICE_61,DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE4);
        dx7SetFrequencyText(DX7_PARAM_CTRL_VOICE_80,DX7_PARAM_CTRL_VOICE_81,DX7_PARAM_CTRL_VOICE_82,DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE3);
        dx7SetFrequencyText(DX7_PARAM_CTRL_VOICE_101,DX7_PARAM_CTRL_VOICE_102,DX7_PARAM_CTRL_VOICE_103,DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE2);
        dx7SetFrequencyText(DX7_PARAM_CTRL_VOICE_122,DX7_PARAM_CTRL_VOICE_123,DX7_PARAM_CTRL_VOICE_124,DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE1);

        Dx7ParamCtrlGroupDisplay(DX7_PARAM_CTRL_GROUP_1VOICE);
    }
    else if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_ALL_VOICE )
    {
        Dx7ParamCtrlGroupDisplay(DX7_PARAM_CTRL_GROUP_ALL_VOICE);
    }
    else
    {
    }

    for( i=0; i<DX7_ALGORITHM_MAX; i++ )
    {
        if( (dx7CtrlInfo.nowMode == DX7_CTRL_MODE_PATCH) &&
            (i == getParamCtrlValue(DX7_PARAM_CTRL_VOICE_134)) )
        {
            ShowWindow(dx7CtrlInfo.hctrl[i], SW_SHOW);
        }
        else
        {
            ShowWindow(dx7CtrlInfo.hctrl[i], SW_HIDE);
        }
    }

    return TRUE;
}

/**
 * ------------------------------------------------------------------------------
 * @brief  シーケンス開始
 *         開始IDから終了IDまで連続して実行する
 * @param  method
 * @param  seqIdStart
 * @param  seqIdEnd
 * @return TRUE: 正常に実行開始
 * @author wurly @date 2017年06月03日 21時16分31秒
 */
BOOL
Dx7CtrlSeqStart( DX7_CTRL_SEQ_METHOD method, DX7_CTRL_SEQ_ID seqIdStart, DX7_CTRL_SEQ_ID seqIdEnd )
{
    BOOL bRtn = TRUE;

    if( (seqIdStart && (seqIdStart < DX7_CTRL_SEQ_NUM_MAX)) &&
        (seqIdEnd   && (seqIdEnd < DX7_CTRL_SEQ_NUM_MAX)) &&
        (seqIdStart <= seqIdEnd) )
    {
        dx7CtrlInfo.nowMethod     = method;
        dx7CtrlInfo.reqSeqIdStart = seqIdStart;
        dx7CtrlInfo.reqSeqIdEnd   = seqIdEnd  ;

        seqStart(method,dx7CtrlInfo.reqSeqIdStart);
    }
    else
    {
        nop();
    }

    return bRtn;
}

/*********************************************
 * 内容   : コンテンツ表示
 * 引数   : なし
 * 戻り値 : BOOL
 **********************************************/
BOOL
Dx7CtrlDisplayContents( void )
{
    return displayContents();
}

/*********************************************
 * 内容   : 
 * 引数   : 
 * 戻り値 : 
 **********************************************/
void
Dx7CtrlCycleProc( void )
{
    if( dx7CtrlInfo.nowExecSeqId )
    {
        if( MidiInqOutExec() )
        {
            nop();
        }
        else
        { /* 送信終了 */
            DebugWndPrintf("TX_END\r\n");

            if( dx7CtrlInfo.nowMethod == DX7_CTRL_SEQ_METHOD_SET )
            {
                if( dx7CtrlInfo.nowExecSeqId < dx7CtrlInfo.reqSeqIdEnd )
                { /* 次のシーケンスがある */
                    seqStart(dx7CtrlInfo.nowMethod,dx7CtrlInfo.nowExecSeqId+1);
                }
                else
                {
                    dx7CtrlInfo.reqSeqIdStart = DX7_CTRL_SEQ_NON_EXEC;
                    dx7CtrlInfo.reqSeqIdEnd   = DX7_CTRL_SEQ_NON_EXEC;
                    dx7CtrlInfo.nowExecSeqId  = DX7_CTRL_SEQ_NON_EXEC;
                }
            }
            else
            { /* GETの時 */
                nop();
            }
        }

        if( MidiInqInExec() )
        {
            nop();
        }
        else
        { /* 受信終了 */
            if( dx7CtrlInfo.nowMethod == DX7_CTRL_SEQ_METHOD_GET )
            {
                DebugWndPrintf("RX_END\r\n");
                seqEndProc(dx7CtrlInfo.nowExecSeqId,dx7CtrlSeqTempRxSize,dx7CtrlSeqRxTempData);
            }
            else
            { /* SETの時 */
                DebugWndPrintf("RX_NOT_EXEC\r\n");
            }

            if( dx7CtrlInfo.nowExecSeqId < dx7CtrlInfo.reqSeqIdEnd )
            { /* 次のシーケンスがある */
                seqStart(dx7CtrlInfo.nowMethod,dx7CtrlInfo.nowExecSeqId+1);
            }
            else
            {
                /* 全てのシーケンス終了 */
                dx7CtrlInfo.reqSeqIdStart = DX7_CTRL_SEQ_NON_EXEC;
                dx7CtrlInfo.reqSeqIdEnd   = DX7_CTRL_SEQ_NON_EXEC;
                dx7CtrlInfo.nowExecSeqId  = DX7_CTRL_SEQ_NON_EXEC;

                if( dx7CtrlInfo.nowMethod == DX7_CTRL_SEQ_METHOD_GET )
                {
                    if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_NONE )
                    {
                    }
                    else if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_PATCH )
                    {
                        copyToParamCtrl(DX7_CTRL_SEQ_1VOICE);
                    }
                    else if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_ALL_VOICE )
                    {
                        copyToParamCtrl(DX7_CTRL_SEQ_ALL_VOICE);
                    }
                    else
                    {
                    }

                    Dx7CtrlDisplayUpdate();
                }
                else
                {
                    nop();
                }
            }
        }
    }
    else
    {
        nop();
    }
}

/*********************************************
 * 内容   : シーケンス開始
 * 引数   : 
 * 戻り値 : BOOL
 **********************************************/
static BOOL
seqStart( DX7_CTRL_SEQ_METHOD method, DX7_CTRL_SEQ_ID seqId )
{
    INT txSize;

    if( seqId && (seqId < DX7_CTRL_SEQ_NUM_MAX) )
    {
        DebugWndPrintf("..............\r\n");
        DebugWndPrintf("DX7_CTRL_SEQ_ID:%d\r\n",seqId);

        txSize = seqStartProc(method,seqId,1024,dx7CtrlSeqTxTempData);

        debugDataArrayPrint(txSize,dx7CtrlSeqTxTempData,"TX");

        dx7CtrlInfo.nowExecSeqId = seqId;

        if( method == DX7_CTRL_SEQ_METHOD_SET )
        {
            nop();
        }
        else
        {
            MidiInExclusiveData( &dx7CtrlSeqRxTempData, &dx7CtrlSeqTempRxSize );
        }

        MidiOutExclusiveData( &dx7CtrlSeqTxTempData, txSize );
    }
    else
    {
        nop();
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : DX7_CTRL_SEQ_ID seqId
 * 戻り値 : INT
 **********************************************/
static INT
seqStartProc( DX7_CTRL_SEQ_METHOD method, DX7_CTRL_SEQ_ID seqId, INT maxDataSize, BYTE *txDataPtr )
{
    S_DX7_CTRL_SEQ_DATA *tblPtr;
    INT txSize = (INT)0;
    INT i;

    tblPtr = &(dx7CtrlSeqDataTbl[seqId]);

    switch( seqId )
    {
    case DX7_CTRL_SEQ_1VOICE:
        if( method == DX7_CTRL_SEQ_METHOD_SET )
        {
            copyFromParamCtrl( seqId );
            memcpy((void *)txDataPtr,(void *)tblPtr->rxDataPtr,tblPtr->rxDataSize);
            txSize = DX7_SYSEX_1VOICE_INDEX_MAX;
        }
        else
        { /* データ要求しデータ受信する*/
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_STATUS    ) = EX_STATUS                   ;
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_ID_NO     ) = EX_ID_NUMBER_YAMAHA         ;
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_SUB_STATUS) = 0x20               | 0x00/* MIDI CH*/;
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_FORMAT_NO ) = DX7_DUMP_REQ_FORMAT_1VOICE;
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_EOX       ) = EX_ETX                      ;
            txSize = DX7_SYSEX_BULK_DUMP_REQUEST_INDEX_MAX;
        }
        break;
    case DX7_CTRL_SEQ_ALL_VOICE:
        if( method == DX7_CTRL_SEQ_METHOD_SET )
        {
            memcpy((void *)txDataPtr,(void *)tblPtr->rxDataPtr,tblPtr->rxDataSize);
            txSize = DX7_SYSEX_ALL_VOICE_INDEX_MAX;
        }
        else
        { /* データ要求しデータ受信する*/
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_STATUS    ) = EX_STATUS                    ;
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_ID_NO     ) = EX_ID_NUMBER_YAMAHA          ;
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_SUB_STATUS) = 0x20               | 0x00/* MIDI CH*/;
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_FORMAT_NO ) = DX7_DUMP_REQ_FORMAT_32VOICE;
            *(txDataPtr+DX7_SYSEX_BULK_DUMP_REQUEST_EOX       ) = EX_ETX                       ;
            txSize = DX7_SYSEX_BULK_DUMP_REQUEST_INDEX_MAX;
        }
        break;
    }

    return txSize;
}

/*********************************************
 * 内容   : 
 * 引数   : DX7_CTRL_SEQ_ID seqId
 * 戻り値 : BOOL
 **********************************************/
static BOOL
seqEndProc( DX7_CTRL_SEQ_ID seqId, INT rxDataSize, BYTE *rxDataPtr )
{
    S_DX7_CTRL_SEQ_DATA *tblPtr = &(dx7CtrlSeqDataTbl[seqId]);

//    DebugWndPrintf("rxDataSize,%d\r\n",rxDataSize);

    debugDataArrayPrint(rxDataSize,rxDataPtr,"RX");

    if( rxDataSize == tblPtr->rxDataSize )
    {
        memcpy((void *)tblPtr->rxDataPtr,(void *)rxDataPtr,tblPtr->rxDataSize);

        switch( seqId )
        {
        case DX7_CTRL_SEQ_1VOICE:
            debugDataArrayPrint(DX7_SYSEX_VCED_MAX,tblPtr->rxDataPtr+DX7_SYSEX_1VOICE_DATA,"CONTENTS");
            break;
        case DX7_CTRL_SEQ_ALL_VOICE:
            debugDataArrayPrint(4096,tblPtr->rxDataPtr+DX7_SYSEX_ALL_VOICE_DATA,"CONTENTS");
            break;
        default:
            break;
        }
    }
    else
    {
        DebugWndPrintf("Check NG,%d,%d\r\n",rxDataSize,tblPtr->rxDataSize);
    }

    switch( seqId )
    {
    case DX7_CTRL_SEQ_1VOICE:
        break;
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : DX7_CTRL_SEQ_ID seqId
 * 戻り値 : BOOL
 **********************************************/
static BOOL
copyToParamCtrl( DX7_CTRL_SEQ_ID seqId )
{
    TCHAR patchName[12+1];
    TCHAR szBuffer[1024];
    INT toneNum;
    INT i,j;

    switch( seqId )
    {
    case DX7_CTRL_SEQ_1VOICE:
        for( i=0; i<DX7_SYSEX_VCED_MAX; i++ )
        {
            INT dataIndex = DX7_SYSEX_1VOICE_DATA+i/*DX7_SYSEX_VCED_00...DX7_SYSEX_VCED_155*/;
            INT paramCtrlIndex;

            if( i < DX7_SYSEX_VCED_145 )
            {
                paramCtrlIndex = DX7_PARAM_CTRL_VOICE_00 + i/*DX7_PARAM_CTRL_VOICE_00...DX7_PARAM_CTRL_VOICE_144*/;

                SendMessage( Dx7ParamCtrlGetHWND(paramCtrlIndex), CB_SETCURSEL, dx7CtrlDataOneVoice[dataIndex], (LPARAM)0 );
            }
            else if( (DX7_SYSEX_VCED_145 <= i) && (i <= DX7_SYSEX_VCED_154) )
            {
                if( DX7_SYSEX_VCED_145 == i )
                {
                    memset(&patchName[0],0,10+1);
                    strncpy(&patchName[0],&dx7CtrlDataOneVoice[dataIndex],10);

                    for( j=9; 0<=j; j++ )
                    {
                        if( patchName[j] == ' ' )
                        {
                            patchName[j] = '\0';
                        }
                        else
                        {
                            break;
                        }
                    }
                    SetWindowText( Dx7ParamCtrlGetHWND(DX7_PARAM_CTRL_VOICE_NAME),&patchName[0]);
                }
                else
                {
                }
            }
            else
            {
#if 0
                paramCtrlIndex = DX7_PARAM_CTRL_VOICE_155 + (i-DX7_SYSEX_VCED_155)/*DX7_PARAM_CTRL_VOICE_155 */;

                SendMessage( Dx7ParamCtrlGetHWND(paramCtrlIndex), CB_SETCURSEL, dx7CtrlDataOneVoice[dataIndex], (LPARAM)0 );
#endif
            }

        }
        break;
    case DX7_CTRL_SEQ_ALL_VOICE:
        for( i=0; i<32; i++ )
        {
            INT j;
            memset(&patchName[0],0,10+1);
            strncpy(&patchName[0],&dx7CtrlDataAllVoice[ DX7_SYSEX_ALL_VOICE_DATA + (i*DX7_SYSEX_VMEM_MAX) + DX7_SYSEX_VMEM_118],10);
            SetWindowText( Dx7ParamCtrlGetHWND(DX7_PARAM_CTRL_ALL_VOICE_NAME_00+i),&patchName[0]);
#if 0 /* パッチ名を出力 */
            DebugWndPrintf("%s\r\n",&patchName[0]);
#endif
        }

        for( i=0; i<32; i++ )
        {
            if( dx7CtrlDataAllVoice[ DX7_SYSEX_ALL_VOICE_DATA + (i*DX7_SYSEX_VMEM_MAX) + DX7_SYSEX_VMEM_102] == 99 &&
                dx7CtrlDataAllVoice[ DX7_SYSEX_ALL_VOICE_DATA + (i*DX7_SYSEX_VMEM_MAX) + DX7_SYSEX_VMEM_103] == 99 &&
                dx7CtrlDataAllVoice[ DX7_SYSEX_ALL_VOICE_DATA + (i*DX7_SYSEX_VMEM_MAX) + DX7_SYSEX_VMEM_104] == 99 &&
                dx7CtrlDataAllVoice[ DX7_SYSEX_ALL_VOICE_DATA + (i*DX7_SYSEX_VMEM_MAX) + DX7_SYSEX_VMEM_105] == 99 &&
                dx7CtrlDataAllVoice[ DX7_SYSEX_ALL_VOICE_DATA + (i*DX7_SYSEX_VMEM_MAX) + DX7_SYSEX_VMEM_106] == 50 &&
                dx7CtrlDataAllVoice[ DX7_SYSEX_ALL_VOICE_DATA + (i*DX7_SYSEX_VMEM_MAX) + DX7_SYSEX_VMEM_107] == 50 &&
                dx7CtrlDataAllVoice[ DX7_SYSEX_ALL_VOICE_DATA + (i*DX7_SYSEX_VMEM_MAX) + DX7_SYSEX_VMEM_108] == 50 &&
                dx7CtrlDataAllVoice[ DX7_SYSEX_ALL_VOICE_DATA + (i*DX7_SYSEX_VMEM_MAX) + DX7_SYSEX_VMEM_109] == 50 )
            {
                SetWindowText( Dx7ParamCtrlGetHWND(DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_00+i),TEXT(""));
            }
            else
            {
                SetWindowText( Dx7ParamCtrlGetHWND(DX7_PARAM_CTRL_ALL_VOICE_EX1_PARAM_00+i),TEXT("PEG"));
            }
        }

        for( i=0; i<32; i++ )
        {
            INT j;
            memset(&szBuffer[0],0,128*2+1);
            for( j=0;j<128; j++ )
            {
                sprintf(&szBuffer[j*2],"%02X",dx7CtrlDataAllVoice[ DX7_SYSEX_ALL_VOICE_DATA + (i*DX7_SYSEX_VMEM_MAX) + j]);
            }
//            SetWindowText( Dx7ParamCtrlGetHWND(DX7_PARAM_CTRL_ALL_VOICE_BULK_00+i),&szBuffer[0]);

#if 0 /* バルクダンプのテキストを出力 */
            DebugWndPrintf("%s\r\n",&szBuffer[0]);
#endif
        }
        break;
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : DX7_CTRL_SEQ_ID seqId
 * 戻り値 : BOOL
 **********************************************/
static BOOL
copyFromParamCtrl( DX7_CTRL_SEQ_ID seqId )
{
    TCHAR patchName[12+1];
    TCHAR szBuffer[1024];
    INT toneNum;
    INT i,j;
    BYTE checkSum;

    switch( seqId )
    {
    case DX7_CTRL_SEQ_1VOICE:
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_REQUEST_STATUS    ] = EX_STATUS;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_REQUEST_ID_NO     ] = EX_ID_NUMBER_YAMAHA;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_REQUEST_SUB_STATUS] = 0x00;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_REQUEST_FORMAT_NO ] = DX7_DUMP_REQ_FORMAT_1VOICE;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_BYTE_COUNT_HIGH   ] = 0x00;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_HEADER_BYTE_COUNT_LOW    ] = 0x9B; /* 155 */

        for( i=0; i<DX7_SYSEX_VCED_MAX; i++ )
        {
            INT dataIndex = DX7_SYSEX_1VOICE_DATA+i/*DX7_SYSEX_VCED_00...DX7_SYSEX_VCED_155*/;
            INT paramCtrlIndex;

            if( i < DX7_SYSEX_VCED_145 )
            {
                paramCtrlIndex = DX7_PARAM_CTRL_VOICE_00 + i/*DX7_PARAM_CTRL_VOICE_00...DX7_PARAM_CTRL_VOICE_118*/;

                dx7CtrlDataOneVoice[dataIndex] = getParamCtrlValue(paramCtrlIndex);
            }
            else if( (DX7_SYSEX_VCED_145 <= i) && (i <= DX7_SYSEX_VCED_154) )
            {
                if( DX7_SYSEX_VCED_145 == i )
                {
                    memset(&patchName[0],0,10+1);
                    Dx7ParamCtrlGetText(DX7_PARAM_CTRL_VOICE_NAME,patchName);
                    memset(&dx7CtrlDataOneVoice[dataIndex],0,10+1);
                    strncpy(&dx7CtrlDataOneVoice[dataIndex],&patchName[0],10);

                    for( j=0; j<10; j++ )
                    {
                        if( dx7CtrlDataOneVoice[dataIndex+j] == '\0' )
                        {
                            dx7CtrlDataOneVoice[dataIndex+j] = ' ';
                        }
                        else
                        {
                            nop();
                        }
                    }
                }
                else
                {
                }
            }
            else
            {
#if 0
                paramCtrlIndex = DX7_PARAM_CTRL_VOICE_87 + (i-DX7_SYSEX_VCED_87)/*DX7_PARAM_CTRL_VOICE_87...DX7_PARAM_CTRL_VOICE_92*/;

                dx7CtrlDataOneVoice[dataIndex] = getParamCtrlValue(paramCtrlIndex);
#endif
            }

        }
        checkSum = calcCheckSum(&(dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA]),DX7_SYSEX_VCED_MAX);
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_FOOTER_CHECKSUM] = checkSum;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_FOOTER_ETX] = EX_ETX;

        DebugWndPrintf("checkSum:0x%02X\r\n",checkSum);
        break;
    case DX7_CTRL_SEQ_ALL_VOICE:
        dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_HEADER_REQUEST_STATUS    ] = EX_STATUS;
        dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_HEADER_REQUEST_ID_NO     ] = EX_ID_NUMBER_YAMAHA;
        dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_HEADER_REQUEST_SUB_STATUS] = 0x00;
        dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_HEADER_REQUEST_FORMAT_NO ] = DX7_DUMP_REQ_FORMAT_32VOICE;
        dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_HEADER_BYTE_COUNT_HIGH   ] = 0x20;
        dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_HEADER_BYTE_COUNT_LOW    ] = 0x00;

        checkSum = calcCheckSum(&(dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_DATA]),DX7_SYSEX_VMEM_MAX*32);

        dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_FOOTER_CHECKSUM] = checkSum;
        dx7CtrlDataAllVoice[DX7_SYSEX_ALL_VOICE_FOOTER_ETX     ] = EX_ETX;

        DebugWndPrintf("checkSum:0x%02X\r\n",checkSum);
        break;
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : DX7_CTRL_SEQ_ID seqId
 * 戻り値 : BOOL
 **********************************************/
static BYTE
getParamCtrlValue( DX7_PARAM_CTRL_ID id )
{
    BYTE value = (BYTE)0x00;
    int iCbNum;
    HWND hComboBox;

    hComboBox = Dx7ParamCtrlGetHWND(id);
    if( hComboBox )
    {
        value = SendMessage(hComboBox,CB_GETCURSEL,0,0);
    }
    else
    {
        nop();
    }

    return value;
}

/*********************************************
 * 内容   : 
 * 引数   : 
 * 戻り値 : 
 **********************************************/
static BOOL
displayContents( void )
{
    INT i;

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : 
 * 戻り値 : 
 **********************************************/
static BYTE
calcCheckSum( BYTE *dataPtr, INT dataSize )
{
    BYTE checkSum = (BYTE)0;
    INT i;

    for( i=0,checkSum=0; i<dataSize; i++ )
    {
        checkSum += *(dataPtr+i);
    }
    checkSum & 0x7F;
    checkSum = (0x80 - checkSum) & 0x7F;

    return checkSum;
}

/********************************************************************************
 * 内容  : データサイズ取得
 * 引数  : seqId
 * 戻り値: DWORD
 ***************************************/
DWORD
Dx7GetDataSize( DX7_CTRL_SEQ_ID seqId )
{
    S_DX7_CTRL_SEQ_DATA *tblPtr;
    DWORD dwSize = (DWORD)0;

    if( seqId < DX7_CTRL_SEQ_NUM_MAX )
    {
        tblPtr = &(dx7CtrlSeqDataTbl[seqId]);
        dwSize = tblPtr->rxDataSize;
    }
    else
    {
    }

    return dwSize;
}

/********************************************************************************
 * 内容  : データ取得
 * 引数  : seqId
 * 引数  : TCHAR *dataPtr
 * 引数  : DWORD dataSize
 * 戻り値: BOOL
 ***************************************/
BOOL
Dx7DataGet( DX7_CTRL_SEQ_ID seqId, TCHAR *dataPtr, DWORD dataSize )
{
    S_DX7_CTRL_SEQ_DATA *tblPtr;
    DWORD dwSize = (DWORD)0;

    if( seqId < DX7_CTRL_SEQ_NUM_MAX )
    {
        copyFromParamCtrl( seqId );

        tblPtr = &(dx7CtrlSeqDataTbl[seqId]);
        dwSize = tblPtr->rxDataSize;

        if( dwSize <= dataSize )
        {
            memcpy(dataPtr,tblPtr->rxDataPtr,tblPtr->rxDataSize);
        }
        else
        {
        }
    }
    else
    {
    }

    return TRUE;
}

/********************************************************************************
 * 内容  : データセット
 * 引数  : seqId
 * 引数  : TCHAR *dataPtr
 * 引数  : DWORD dataSize
 * 戻り値: BOOL
 ***************************************/
BOOL
Dx7DataSet( DX7_CTRL_SEQ_ID seqId, TCHAR *dataPtr, DWORD dataSize )
{
    S_DX7_CTRL_SEQ_DATA *tblPtr;
    DWORD dwSize = (DWORD)0;

    if( seqId < DX7_CTRL_SEQ_NUM_MAX )
    {
        tblPtr = &(dx7CtrlSeqDataTbl[seqId]);
        dwSize = tblPtr->rxDataSize;

        DebugWndPrintf("[DX7] DataSet,%d,%d\r\n",dwSize,dataSize);

        if( dwSize <= dataSize )
        {
            memcpy(tblPtr->rxDataPtr,dataPtr,tblPtr->rxDataSize);
        }
        else
        {
        }

        if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_NONE )
        {
        }
        else if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_PATCH )
        {
            copyToParamCtrl(DX7_CTRL_SEQ_1VOICE);
        }
        else if( dx7CtrlInfo.nowMode == DX7_CTRL_MODE_ALL_VOICE )
        {
            copyToParamCtrl(DX7_CTRL_SEQ_ALL_VOICE);
        }
        else
        {
        }
    }
    else
    {
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : 
 * 戻り値 : 
 **********************************************/
BOOL
Dx7CtrlOnCommand( WORD code, WORD notifyCode, DX7_CTRL_MODE *modePtr )
{
    BOOL bRtn = (BOOL)FALSE;
    DX7_CTRL_MODE mode = (DX7_CTRL_MODE)DX7_CTRL_MODE_NUM_MAX;

    switch( code )
    {
    case (DX7_PARAM_CTRL_VOICE_134+DX7_PARAM_CTRL_ID_OFFSET): /* ALGORITHMの変更*/
        Dx7CtrlDisplayUpdate();
        break;
    case (DX7_PARAM_CTRL_VOICE_17                 +DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_VOICE_18                 +DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_VOICE_19                 +DX7_PARAM_CTRL_ID_OFFSET):
        if( notifyCode == 1 )
        {
            dx7SetFrequencyText(DX7_PARAM_CTRL_VOICE_17,DX7_PARAM_CTRL_VOICE_18,DX7_PARAM_CTRL_VOICE_19,DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE6);
            bRtn = (BOOL)TRUE;
        }
        else
        {
            nop();
        }
        break;
    case (DX7_PARAM_CTRL_VOICE_38                 +DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_VOICE_39                 +DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_VOICE_40                 +DX7_PARAM_CTRL_ID_OFFSET):
        if( notifyCode == 1 )
        {
            dx7SetFrequencyText(DX7_PARAM_CTRL_VOICE_38,DX7_PARAM_CTRL_VOICE_39,DX7_PARAM_CTRL_VOICE_40,DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE5);
            bRtn = (BOOL)TRUE;
        }
        else
        {
            nop();
        }
        break;
    case (DX7_PARAM_CTRL_VOICE_59                 +DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_VOICE_60                 +DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_VOICE_61                 +DX7_PARAM_CTRL_ID_OFFSET):
        if( notifyCode == 1 )
        {
            dx7SetFrequencyText(DX7_PARAM_CTRL_VOICE_59,DX7_PARAM_CTRL_VOICE_60,DX7_PARAM_CTRL_VOICE_61,DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE4);
            bRtn = (BOOL)TRUE;
        }
        else
        {
            nop();
        }
        break;
    case (DX7_PARAM_CTRL_VOICE_80                 +DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_VOICE_81                 +DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_VOICE_82                 +DX7_PARAM_CTRL_ID_OFFSET):
        if( notifyCode == 1 )
        {
            dx7SetFrequencyText(DX7_PARAM_CTRL_VOICE_80,DX7_PARAM_CTRL_VOICE_81,DX7_PARAM_CTRL_VOICE_82,DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE3);
            bRtn = (BOOL)TRUE;
        }
        else
        {
            nop();
        }
        break;
    case (DX7_PARAM_CTRL_VOICE_101                +DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_VOICE_102                +DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_VOICE_103                +DX7_PARAM_CTRL_ID_OFFSET):
        if( notifyCode == 1 )
        {
            dx7SetFrequencyText(DX7_PARAM_CTRL_VOICE_101,DX7_PARAM_CTRL_VOICE_102,DX7_PARAM_CTRL_VOICE_103,DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE2);
            bRtn = (BOOL)TRUE;
        }
        else
        {
            nop();
        }
        break;
    case (DX7_PARAM_CTRL_VOICE_122                +DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_VOICE_123                +DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_VOICE_124                +DX7_PARAM_CTRL_ID_OFFSET):
        if( notifyCode == 1 )
        {
            dx7SetFrequencyText(DX7_PARAM_CTRL_VOICE_122,DX7_PARAM_CTRL_VOICE_123,DX7_PARAM_CTRL_VOICE_124,DX7_PARAM_CTRL_STATIC_FREQUENCY_VALUE1);
            bRtn = (BOOL)TRUE;
        }
        else
        {
            nop();
        }
        break;
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_00+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_01+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_02+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_03+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_04+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_05+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_06+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_07+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_08+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_09+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_10+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_11+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_12+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_13+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_14+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_15+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_16+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_17+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_18+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_19+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_20+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_21+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_22+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_23+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_24+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_25+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_26+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_27+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_28+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_29+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_30+DX7_PARAM_CTRL_ID_OFFSET):
    case (DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_31+DX7_PARAM_CTRL_ID_OFFSET):
        DebugWndPrintf("[DX7] TO 1VOICE,%d=0x%04X\r\n",code-(DX7_PARAM_CTRL_ID_OFFSET+DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_00),notifyCode);
        if( notifyCode == 0 )
        {
            dx7voiceCopyFromAllToOne(code-(DX7_PARAM_CTRL_ID_OFFSET+DX7_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_00));
            bRtn = (BOOL)TRUE;
            mode = DX7_CTRL_MODE_PATCH;
        }
        else
        {
            nop();
        }
        break;
    case (DX7_PARAM_CTRL_BUTTON_TO_ALL_VOICE+DX7_PARAM_CTRL_ID_OFFSET):
        DebugWndPrintf("[DX7] TO ALL VOICE,%d=0x%04X\r\n",code,notifyCode);
        copyFromParamCtrl( DX7_CTRL_SEQ_1VOICE );
        dx7voiceCopyFromOneToOneOfAll(getParamCtrlValue(DX7_PARAM_CTRL_COMBOBOX_TO_ALL_VOICE_NUM));
        bRtn = (BOOL)TRUE;
        mode = DX7_CTRL_MODE_ALL_VOICE;
        break;
    default:
        break;
    }

    if( modePtr != NULL )
    {
        if( bRtn && (mode != (DX7_CTRL_MODE)DX7_CTRL_MODE_NUM_MAX) )
        {
            *modePtr = mode;
        }
        else
        {
            *modePtr = dx7CtrlInfo.nowMode;
        }
    }
    else
    {
        nop();
    }

    return bRtn;
}

/********************************************************************************
 * 内容  :
 * 引数  : なし
 * 戻り値: S_DX7_VOICE_PARAM *
 ***************************************/
S_DX7_VOICE_PARAM *
Dx7GetVoiceParamPtr( void )
{
    dx7OneVoiceDataInit();

    dx7voiceParam.operatorParam[DX7_OPERATOR6].rate1         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_00 ] /*R1    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].rate2         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_01 ] /*R2    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].rate3         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_02 ] /*R3    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].rate4         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_03 ] /*R4    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].level1        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_04 ] /*L1    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].level2        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_05 ] /*L2    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].level3        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_06 ] /*L3    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].level4        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_07 ] /*L4    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].break_point   = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_08 ] /*BP    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].depth_l       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_09 ] /*LD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].depth_r       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_10 ] /*RD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].curve_l       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_11 ] /*LC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].curve_r       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_12 ] /*RC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].k_rate_scaling= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_13 ] /*RS    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].amplitude     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_14 ] /*AMS   */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].velo_sens     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_15 ] /*TS    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].out_level     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_16 ] /*TL    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].freq_mode     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_17 ] /*PM    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].coarse        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_18 ] /*PC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].fine          = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_19 ] /*PF    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR6].detune        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_20 ] /*PD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].rate1         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_21 ] /*R1    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].rate2         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_22 ] /*R2    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].rate3         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_23 ] /*R3    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].rate4         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_24 ] /*R4    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].level1        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_25 ] /*L1    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].level2        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_26 ] /*L2    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].level3        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_27 ] /*L3    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].level4        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_28 ] /*L4    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].break_point   = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_29 ] /*BP    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].depth_l       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_30 ] /*LD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].depth_r       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_31 ] /*RD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].curve_l       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_32 ] /*LC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].curve_r       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_33 ] /*RC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].k_rate_scaling= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_34 ] /*RS    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].amplitude     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_35 ] /*AMS   */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].velo_sens     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_36 ] /*TS    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].out_level     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_37 ] /*TL    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].freq_mode     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_38 ] /*PM    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].coarse        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_39 ] /*PC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].fine          = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_40 ] /*PF    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR5].detune        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_41 ] /*PD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].rate1         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_42 ] /*R1    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].rate2         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_43 ] /*R2    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].rate3         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_44 ] /*R3    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].rate4         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_45 ] /*R4    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].level1        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_46 ] /*L1    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].level2        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_47 ] /*L2    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].level3        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_48 ] /*L3    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].level4        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_49 ] /*L4    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].break_point   = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_50 ] /*BP    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].depth_l       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_51 ] /*LD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].depth_r       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_52 ] /*RD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].curve_l       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_53 ] /*LC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].curve_r       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_54 ] /*RC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].k_rate_scaling= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_55 ] /*RS    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].amplitude     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_56 ] /*AMS   */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].velo_sens     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_57 ] /*TS    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].out_level     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_58 ] /*TL    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].freq_mode     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_59 ] /*PM    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].coarse        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_60 ] /*PC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].fine          = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_61 ] /*PF    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR4].detune        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_62 ] /*PD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].rate1         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_63 ] /*R1    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].rate2         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_64 ] /*R2    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].rate3         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_65 ] /*R3    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].rate4         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_66 ] /*R4    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].level1        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_67 ] /*L1    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].level2        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_68 ] /*L2    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].level3        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_69 ] /*L3    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].level4        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_70 ] /*L4    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].break_point   = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_71 ] /*BP    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].depth_l       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_72 ] /*LD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].depth_r       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_73 ] /*RD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].curve_l       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_74 ] /*LC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].curve_r       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_75 ] /*RC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].k_rate_scaling= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_76 ] /*RS    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].amplitude     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_77 ] /*AMS   */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].velo_sens     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_78 ] /*TS    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].out_level     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_79 ] /*TL    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].freq_mode     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_80 ] /*PM    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].coarse        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_81 ] /*PC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].fine          = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_82 ] /*PF    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR3].detune        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_83 ] /*PD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].rate1         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_84 ] /*R1    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].rate2         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_85 ] /*R2    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].rate3         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_86 ] /*R3    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].rate4         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_87 ] /*R4    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].level1        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_88 ] /*L1    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].level2        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_89 ] /*L2    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].level3        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_90 ] /*L3    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].level4        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_91 ] /*L4    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].break_point   = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_92 ] /*BP    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].depth_l       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_93 ] /*LD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].depth_r       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_94 ] /*RD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].curve_l       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_95 ] /*LC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].curve_r       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_96 ] /*RC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].k_rate_scaling= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_97 ] /*RS    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].amplitude     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_98 ] /*AMS   */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].velo_sens     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_99 ] /*TS    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].out_level     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_100] /*TL    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].freq_mode     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_101] /*PM    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].coarse        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_102] /*PC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].fine          = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_103] /*PF    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR2].detune        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_104] /*PD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].rate1         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_105] /*R1    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].rate2         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_106] /*R2    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].rate3         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_107] /*R3    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].rate4         = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_108] /*R4    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].level1        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_109] /*L1    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].level2        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_110] /*L2    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].level3        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_111] /*L3    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].level4        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_112] /*L4    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].break_point   = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_113] /*BP    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].depth_l       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_114] /*LD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].depth_r       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_115] /*RD    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].curve_l       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_116] /*LC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].curve_r       = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_117] /*RC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].k_rate_scaling= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_118] /*RS    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].amplitude     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_119] /*AMS   */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].velo_sens     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_120] /*TS    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].out_level     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_121] /*TL    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].freq_mode     = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_122] /*PM    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].coarse        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_123] /*PC    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].fine          = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_124] /*PF    */;
    dx7voiceParam.operatorParam[DX7_OPERATOR1].detune        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_125] /*PD    */;
    dx7voiceParam.peg_rate1                                  = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_126] /*PR1   */;
    dx7voiceParam.peg_rate2                                  = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_127] /*PR2   */;
    dx7voiceParam.peg_rate3                                  = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_128] /*PR3   */;
    dx7voiceParam.peg_rate4                                  = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_129] /*PR4   */;
    dx7voiceParam.peg_level1                                 = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_130] /*PL1   */;
    dx7voiceParam.peg_level2                                 = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_131] /*PL2   */;
    dx7voiceParam.peg_level3                                 = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_132] /*PL3   */;
    dx7voiceParam.peg_level4                                 = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_133] /*PL4   */;
    dx7voiceParam.algorithm                                  = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_134] /*ALS   */;
    dx7voiceParam.feedback                                   = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_135] /*FBL   */;
    dx7voiceParam.oscillator_sync                            = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_136] /*OPI   */;
    dx7voiceParam.speed                                      = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_137] /*LFS   */;
    dx7voiceParam.delay                                      = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_138] /*LFD   */;
    dx7voiceParam.pmd                                        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_139] /*LPMD  */;
    dx7voiceParam.amd                                        = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_140] /*LAMD  */;
    dx7voiceParam.lfo_sync                                   = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_141] /*LFKS  */;
    dx7voiceParam.lfo_wave                                   = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_142] /*LFW   */;
    dx7voiceParam.pitch                                      = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_143] /*LPMS  */;
    dx7voiceParam.key_transpose                              = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_144] /*TRNP  */;
    dx7voiceParam.patchname[0]                               = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_145] /*VNAM1 */;
    dx7voiceParam.patchname[1]                               = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_146] /*VNAM2 */;
    dx7voiceParam.patchname[2]                               = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_147] /*VNAM3 */;
    dx7voiceParam.patchname[3]                               = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_148] /*VNAM4 */;
    dx7voiceParam.patchname[4]                               = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_149] /*VNAM5 */;
    dx7voiceParam.patchname[5]                               = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_150] /*VNAM6 */;
    dx7voiceParam.patchname[6]                               = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_151] /*VNAM7 */;
    dx7voiceParam.patchname[7]                               = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_152] /*VNAM8 */;
    dx7voiceParam.patchname[8]                               = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_153] /*VNAM9 */;
    dx7voiceParam.patchname[9]                               = dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_154] /*VNAM10*/;

    return &dx7voiceParam;
}

/********************************************************************************
 * 内容  : 構造体から1VOICEデータにコピー
 * 引数  : なし
 * 戻り値: なし
 ***************************************/
void
Dx7VoiceParamCopyFromStruct( void )
{
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_00 ] /*R1    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].rate1         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_01 ] /*R2    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].rate2         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_02 ] /*R3    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].rate3         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_03 ] /*R4    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].rate4         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_04 ] /*L1    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].level1        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_05 ] /*L2    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].level2        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_06 ] /*L3    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].level3        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_07 ] /*L4    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].level4        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_08 ] /*BP    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].break_point   ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_09 ] /*LD    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].depth_l       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_10 ] /*RD    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].depth_r       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_11 ] /*LC    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].curve_l       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_12 ] /*RC    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].curve_r       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_13 ] /*RS    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].k_rate_scaling;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_14 ] /*AMS   */= dx7voiceParam.operatorParam[DX7_OPERATOR6].amplitude     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_15 ] /*TS    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].velo_sens     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_16 ] /*TL    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].out_level     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_17 ] /*PM    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].freq_mode     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_18 ] /*PC    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].coarse        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_19 ] /*PF    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].fine          ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_20 ] /*PD    */= dx7voiceParam.operatorParam[DX7_OPERATOR6].detune        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_21 ] /*R1    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].rate1         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_22 ] /*R2    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].rate2         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_23 ] /*R3    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].rate3         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_24 ] /*R4    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].rate4         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_25 ] /*L1    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].level1        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_26 ] /*L2    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].level2        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_27 ] /*L3    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].level3        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_28 ] /*L4    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].level4        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_29 ] /*BP    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].break_point   ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_30 ] /*LD    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].depth_l       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_31 ] /*RD    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].depth_r       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_32 ] /*LC    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].curve_l       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_33 ] /*RC    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].curve_r       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_34 ] /*RS    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].k_rate_scaling;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_35 ] /*AMS   */= dx7voiceParam.operatorParam[DX7_OPERATOR5].amplitude     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_36 ] /*TS    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].velo_sens     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_37 ] /*TL    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].out_level     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_38 ] /*PM    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].freq_mode     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_39 ] /*PC    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].coarse        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_40 ] /*PF    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].fine          ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_41 ] /*PD    */= dx7voiceParam.operatorParam[DX7_OPERATOR5].detune        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_42 ] /*R1    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].rate1         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_43 ] /*R2    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].rate2         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_44 ] /*R3    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].rate3         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_45 ] /*R4    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].rate4         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_46 ] /*L1    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].level1        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_47 ] /*L2    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].level2        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_48 ] /*L3    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].level3        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_49 ] /*L4    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].level4        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_50 ] /*BP    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].break_point   ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_51 ] /*LD    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].depth_l       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_52 ] /*RD    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].depth_r       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_53 ] /*LC    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].curve_l       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_54 ] /*RC    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].curve_r       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_55 ] /*RS    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].k_rate_scaling;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_56 ] /*AMS   */= dx7voiceParam.operatorParam[DX7_OPERATOR4].amplitude     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_57 ] /*TS    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].velo_sens     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_58 ] /*TL    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].out_level     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_59 ] /*PM    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].freq_mode     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_60 ] /*PC    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].coarse        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_61 ] /*PF    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].fine          ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_62 ] /*PD    */= dx7voiceParam.operatorParam[DX7_OPERATOR4].detune        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_63 ] /*R1    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].rate1         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_64 ] /*R2    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].rate2         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_65 ] /*R3    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].rate3         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_66 ] /*R4    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].rate4         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_67 ] /*L1    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].level1        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_68 ] /*L2    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].level2        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_69 ] /*L3    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].level3        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_70 ] /*L4    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].level4        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_71 ] /*BP    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].break_point   ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_72 ] /*LD    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].depth_l       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_73 ] /*RD    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].depth_r       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_74 ] /*LC    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].curve_l       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_75 ] /*RC    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].curve_r       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_76 ] /*RS    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].k_rate_scaling;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_77 ] /*AMS   */= dx7voiceParam.operatorParam[DX7_OPERATOR3].amplitude     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_78 ] /*TS    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].velo_sens     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_79 ] /*TL    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].out_level     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_80 ] /*PM    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].freq_mode     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_81 ] /*PC    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].coarse        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_82 ] /*PF    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].fine          ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_83 ] /*PD    */= dx7voiceParam.operatorParam[DX7_OPERATOR3].detune        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_84 ] /*R1    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].rate1         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_85 ] /*R2    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].rate2         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_86 ] /*R3    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].rate3         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_87 ] /*R4    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].rate4         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_88 ] /*L1    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].level1        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_89 ] /*L2    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].level2        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_90 ] /*L3    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].level3        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_91 ] /*L4    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].level4        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_92 ] /*BP    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].break_point   ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_93 ] /*LD    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].depth_l       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_94 ] /*RD    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].depth_r       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_95 ] /*LC    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].curve_l       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_96 ] /*RC    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].curve_r       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_97 ] /*RS    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].k_rate_scaling;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_98 ] /*AMS   */= dx7voiceParam.operatorParam[DX7_OPERATOR2].amplitude     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_99 ] /*TS    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].velo_sens     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_100] /*TL    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].out_level     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_101] /*PM    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].freq_mode     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_102] /*PC    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].coarse        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_103] /*PF    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].fine          ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_104] /*PD    */= dx7voiceParam.operatorParam[DX7_OPERATOR2].detune        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_105] /*R1    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].rate1         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_106] /*R2    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].rate2         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_107] /*R3    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].rate3         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_108] /*R4    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].rate4         ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_109] /*L1    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].level1        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_110] /*L2    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].level2        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_111] /*L3    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].level3        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_112] /*L4    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].level4        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_113] /*BP    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].break_point   ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_114] /*LD    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].depth_l       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_115] /*RD    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].depth_r       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_116] /*LC    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].curve_l       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_117] /*RC    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].curve_r       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_118] /*RS    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].k_rate_scaling;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_119] /*AMS   */= dx7voiceParam.operatorParam[DX7_OPERATOR1].amplitude     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_120] /*TS    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].velo_sens     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_121] /*TL    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].out_level     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_122] /*PM    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].freq_mode     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_123] /*PC    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].coarse        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_124] /*PF    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].fine          ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_125] /*PD    */= dx7voiceParam.operatorParam[DX7_OPERATOR1].detune        ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_126] /*PR1   */= dx7voiceParam.peg_rate1      ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_127] /*PR2   */= dx7voiceParam.peg_rate2      ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_128] /*PR3   */= dx7voiceParam.peg_rate3      ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_129] /*PR4   */= dx7voiceParam.peg_rate4      ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_130] /*PL1   */= dx7voiceParam.peg_level1     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_131] /*PL2   */= dx7voiceParam.peg_level2     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_132] /*PL3   */= dx7voiceParam.peg_level3     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_133] /*PL4   */= dx7voiceParam.peg_level4     ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_134] /*ALS   */= dx7voiceParam.algorithm      ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_135] /*FBL   */= dx7voiceParam.feedback       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_136] /*OPI   */= dx7voiceParam.oscillator_sync;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_137] /*LFS   */= dx7voiceParam.speed          ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_138] /*LFD   */= dx7voiceParam.delay          ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_139] /*LPMD  */= dx7voiceParam.pmd            ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_140] /*LAMD  */= dx7voiceParam.amd            ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_141] /*LFKS  */= dx7voiceParam.lfo_sync       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_142] /*LFW   */= dx7voiceParam.lfo_wave       ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_143] /*LPMS  */= dx7voiceParam.pitch          ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_144] /*TRNP  */= dx7voiceParam.key_transpose  ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_145] /*VNAM1 */= dx7voiceParam.patchname[0]   ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_146] /*VNAM2 */= dx7voiceParam.patchname[1]   ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_147] /*VNAM3 */= dx7voiceParam.patchname[2]   ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_148] /*VNAM4 */= dx7voiceParam.patchname[3]   ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_149] /*VNAM5 */= dx7voiceParam.patchname[4]   ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_150] /*VNAM6 */= dx7voiceParam.patchname[5]   ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_151] /*VNAM7 */= dx7voiceParam.patchname[6]   ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_152] /*VNAM8 */= dx7voiceParam.patchname[7]   ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_153] /*VNAM9 */= dx7voiceParam.patchname[8]   ;
    dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_154] /*VNAM10*/= dx7voiceParam.patchname[9]   ;

    copyToParamCtrl(DX7_CTRL_SEQ_1VOICE);
}

/********************************************************************************
 * 内容  : ボイスコピー(1VOICEからALL VOICEへ)
 * 引数  : INT voiceNum
 * 戻り値: BOOL
 ***************************************/
BOOL
Dx7voiceCopyFromOneToAll( INT voiceNum )
{
    dx7voiceCopyFromOneToOneOfAll(voiceNum);
}

/********************************************************************************
 * 内容  : ボイスコピー(ALL VOICEから1VOICEへ)
 * 引数  : INT voiceNum
 * 戻り値: BOOL
 ***************************************/
static BOOL
dx7voiceCopyFromAllToOne( INT voiceNum )
{
    if( voiceNum < 32 )
    {
        INT topNum = DX7_SYSEX_ALL_VOICE_DATA + (voiceNum*DX7_SYSEX_VMEM_MAX);

        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_00 ] /*R1    */= dx7CtrlDataAllVoice[topNum+0  ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_01 ] /*R2    */= dx7CtrlDataAllVoice[topNum+1  ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_02 ] /*R3    */= dx7CtrlDataAllVoice[topNum+2  ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_03 ] /*R4    */= dx7CtrlDataAllVoice[topNum+3  ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_04 ] /*L1    */= dx7CtrlDataAllVoice[topNum+4  ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_05 ] /*L2    */= dx7CtrlDataAllVoice[topNum+5  ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_06 ] /*L3    */= dx7CtrlDataAllVoice[topNum+6  ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_07 ] /*L4    */= dx7CtrlDataAllVoice[topNum+7  ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_08 ] /*BP    */= dx7CtrlDataAllVoice[topNum+8  ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_09 ] /*LD    */= dx7CtrlDataAllVoice[topNum+9  ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_10 ] /*RD    */= dx7CtrlDataAllVoice[topNum+10 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_11 ] /*LC    */=(dx7CtrlDataAllVoice[topNum+11 ])&0x3    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_12 ] /*RC    */=(dx7CtrlDataAllVoice[topNum+11 ]>>2)&0x3 ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_13 ] /*RS    */=(dx7CtrlDataAllVoice[topNum+12 ])&0x7    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_14 ] /*AMS   */=(dx7CtrlDataAllVoice[topNum+13 ])&0x3    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_15 ] /*TS    */=(dx7CtrlDataAllVoice[topNum+13 ]>>2)&0x7 ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_16 ] /*TL    */= dx7CtrlDataAllVoice[topNum+14 ]         ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_17 ] /*PM    */= dx7CtrlDataAllVoice[topNum+15 ]&0x1     ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_18 ] /*PC    */=(dx7CtrlDataAllVoice[topNum+15 ]>>1)&0x1F;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_19 ] /*PF    */= dx7CtrlDataAllVoice[topNum+16 ]         ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_20 ] /*PD    */=(dx7CtrlDataAllVoice[topNum+12 ]>>3)&0xF ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_21 ] /*R1    */= dx7CtrlDataAllVoice[topNum+17 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_22 ] /*R2    */= dx7CtrlDataAllVoice[topNum+18 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_23 ] /*R3    */= dx7CtrlDataAllVoice[topNum+19 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_24 ] /*R4    */= dx7CtrlDataAllVoice[topNum+20 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_25 ] /*L1    */= dx7CtrlDataAllVoice[topNum+21 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_26 ] /*L2    */= dx7CtrlDataAllVoice[topNum+22 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_27 ] /*L3    */= dx7CtrlDataAllVoice[topNum+23 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_28 ] /*L4    */= dx7CtrlDataAllVoice[topNum+24 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_29 ] /*BP    */= dx7CtrlDataAllVoice[topNum+25 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_30 ] /*LD    */= dx7CtrlDataAllVoice[topNum+26 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_31 ] /*RD    */= dx7CtrlDataAllVoice[topNum+27 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_32 ] /*LC    */=(dx7CtrlDataAllVoice[topNum+28 ])&0x3    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_33 ] /*RC    */=(dx7CtrlDataAllVoice[topNum+28 ]>>2)&0x3 ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_34 ] /*RS    */=(dx7CtrlDataAllVoice[topNum+29 ])&0x7    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_35 ] /*AMS   */=(dx7CtrlDataAllVoice[topNum+30 ])&0x3    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_36 ] /*TS    */=(dx7CtrlDataAllVoice[topNum+30 ]>>2)&0x7 ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_37 ] /*TL    */= dx7CtrlDataAllVoice[topNum+31 ]         ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_38 ] /*PM    */= dx7CtrlDataAllVoice[topNum+32 ]&0x1     ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_39 ] /*PC    */=(dx7CtrlDataAllVoice[topNum+32 ]>>1)&0x1F;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_40 ] /*PF    */= dx7CtrlDataAllVoice[topNum+33 ]         ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_41 ] /*PD    */=(dx7CtrlDataAllVoice[topNum+29 ]>>3)&0xF ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_42 ] /*R1    */= dx7CtrlDataAllVoice[topNum+34 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_43 ] /*R2    */= dx7CtrlDataAllVoice[topNum+35 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_44 ] /*R3    */= dx7CtrlDataAllVoice[topNum+36 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_45 ] /*R4    */= dx7CtrlDataAllVoice[topNum+37 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_46 ] /*L1    */= dx7CtrlDataAllVoice[topNum+38 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_47 ] /*L2    */= dx7CtrlDataAllVoice[topNum+39 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_48 ] /*L3    */= dx7CtrlDataAllVoice[topNum+40 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_49 ] /*L4    */= dx7CtrlDataAllVoice[topNum+41 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_50 ] /*BP    */= dx7CtrlDataAllVoice[topNum+42 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_51 ] /*LD    */= dx7CtrlDataAllVoice[topNum+43 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_52 ] /*RD    */= dx7CtrlDataAllVoice[topNum+44 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_53 ] /*LC    */=(dx7CtrlDataAllVoice[topNum+45 ])&0x3    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_54 ] /*RC    */=(dx7CtrlDataAllVoice[topNum+45 ]>>2)&0x3 ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_55 ] /*RS    */=(dx7CtrlDataAllVoice[topNum+46 ])&0x7    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_56 ] /*AMS   */=(dx7CtrlDataAllVoice[topNum+47 ])&0x3    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_57 ] /*TS    */=(dx7CtrlDataAllVoice[topNum+47 ]>>2)&0x7 ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_58 ] /*TL    */= dx7CtrlDataAllVoice[topNum+48 ]         ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_59 ] /*PM    */= dx7CtrlDataAllVoice[topNum+49 ]&0x1     ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_60 ] /*PC    */=(dx7CtrlDataAllVoice[topNum+49 ]>>1)&0x1F;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_61 ] /*PF    */= dx7CtrlDataAllVoice[topNum+50 ]         ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_62 ] /*PD    */=(dx7CtrlDataAllVoice[topNum+46 ]>>3)&0xF ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_63 ] /*R1    */= dx7CtrlDataAllVoice[topNum+51 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_64 ] /*R2    */= dx7CtrlDataAllVoice[topNum+52 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_65 ] /*R3    */= dx7CtrlDataAllVoice[topNum+53 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_66 ] /*R4    */= dx7CtrlDataAllVoice[topNum+54 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_67 ] /*L1    */= dx7CtrlDataAllVoice[topNum+55 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_68 ] /*L2    */= dx7CtrlDataAllVoice[topNum+56 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_69 ] /*L3    */= dx7CtrlDataAllVoice[topNum+57 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_70 ] /*L4    */= dx7CtrlDataAllVoice[topNum+58 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_71 ] /*BP    */= dx7CtrlDataAllVoice[topNum+59 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_72 ] /*LD    */= dx7CtrlDataAllVoice[topNum+60 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_73 ] /*RD    */= dx7CtrlDataAllVoice[topNum+61 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_74 ] /*LC    */=(dx7CtrlDataAllVoice[topNum+62 ])&0x3    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_75 ] /*RC    */=(dx7CtrlDataAllVoice[topNum+62 ]>>2)&0x3 ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_76 ] /*RS    */=(dx7CtrlDataAllVoice[topNum+63 ])&0x7    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_77 ] /*AMS   */=(dx7CtrlDataAllVoice[topNum+64 ])&0x3    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_78 ] /*TS    */=(dx7CtrlDataAllVoice[topNum+64 ]>>2)&0x7 ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_79 ] /*TL    */= dx7CtrlDataAllVoice[topNum+65 ]         ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_80 ] /*PM    */= dx7CtrlDataAllVoice[topNum+66 ]&0x1     ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_81 ] /*PC    */=(dx7CtrlDataAllVoice[topNum+66 ]>>1)&0x1F;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_82 ] /*PF    */= dx7CtrlDataAllVoice[topNum+67 ]         ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_83 ] /*PD    */=(dx7CtrlDataAllVoice[topNum+63 ]>>3)&0xF ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_84 ] /*R1    */= dx7CtrlDataAllVoice[topNum+68 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_85 ] /*R2    */= dx7CtrlDataAllVoice[topNum+69 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_86 ] /*R3    */= dx7CtrlDataAllVoice[topNum+70 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_87 ] /*R4    */= dx7CtrlDataAllVoice[topNum+71 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_88 ] /*L1    */= dx7CtrlDataAllVoice[topNum+72 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_89 ] /*L2    */= dx7CtrlDataAllVoice[topNum+73 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_90 ] /*L3    */= dx7CtrlDataAllVoice[topNum+74 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_91 ] /*L4    */= dx7CtrlDataAllVoice[topNum+75 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_92 ] /*BP    */= dx7CtrlDataAllVoice[topNum+76 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_93 ] /*LD    */= dx7CtrlDataAllVoice[topNum+77 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_94 ] /*RD    */= dx7CtrlDataAllVoice[topNum+78 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_95 ] /*LC    */=(dx7CtrlDataAllVoice[topNum+79 ])&0x3    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_96 ] /*RC    */=(dx7CtrlDataAllVoice[topNum+79 ]>>2)&0x3 ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_97 ] /*RS    */=(dx7CtrlDataAllVoice[topNum+80 ])&0x7    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_98 ] /*AMS   */=(dx7CtrlDataAllVoice[topNum+81 ])&0x3    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_99 ] /*TS    */=(dx7CtrlDataAllVoice[topNum+81 ]>>2)&0x7 ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_100] /*TL    */= dx7CtrlDataAllVoice[topNum+82 ]         ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_101] /*PM    */= dx7CtrlDataAllVoice[topNum+83 ]&0x1     ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_102] /*PC    */=(dx7CtrlDataAllVoice[topNum+83 ]>>1)&0x1F;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_103] /*PF    */= dx7CtrlDataAllVoice[topNum+84 ]         ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_104] /*PD    */=(dx7CtrlDataAllVoice[topNum+80 ]>>3)&0xF ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_105] /*R1    */= dx7CtrlDataAllVoice[topNum+85 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_106] /*R2    */= dx7CtrlDataAllVoice[topNum+86 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_107] /*R3    */= dx7CtrlDataAllVoice[topNum+87 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_108] /*R4    */= dx7CtrlDataAllVoice[topNum+88 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_109] /*L1    */= dx7CtrlDataAllVoice[topNum+89 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_110] /*L2    */= dx7CtrlDataAllVoice[topNum+90 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_111] /*L3    */= dx7CtrlDataAllVoice[topNum+91 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_112] /*L4    */= dx7CtrlDataAllVoice[topNum+92 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_113] /*BP    */= dx7CtrlDataAllVoice[topNum+93 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_114] /*LD    */= dx7CtrlDataAllVoice[topNum+94 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_115] /*RD    */= dx7CtrlDataAllVoice[topNum+95 ];         
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_116] /*LC    */=(dx7CtrlDataAllVoice[topNum+96 ])&0x3    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_117] /*RC    */=(dx7CtrlDataAllVoice[topNum+96 ]>>2)&0x3 ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_118] /*RS    */=(dx7CtrlDataAllVoice[topNum+97 ])&0x7    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_119] /*AMS   */=(dx7CtrlDataAllVoice[topNum+98 ])&0x3    ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_120] /*TS    */=(dx7CtrlDataAllVoice[topNum+98 ]>>2)&0x7 ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_121] /*TL    */= dx7CtrlDataAllVoice[topNum+99 ]         ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_122] /*PM    */= dx7CtrlDataAllVoice[topNum+100]&0x1     ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_123] /*PC    */=(dx7CtrlDataAllVoice[topNum+100]>>1)&0x1F;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_124] /*PF    */= dx7CtrlDataAllVoice[topNum+101]         ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_125] /*PD    */=(dx7CtrlDataAllVoice[topNum+97 ]>>3)&0xF ;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_126] /*PR1   */= dx7CtrlDataAllVoice[topNum+102];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_127] /*PR2   */= dx7CtrlDataAllVoice[topNum+103];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_128] /*PR3   */= dx7CtrlDataAllVoice[topNum+104];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_129] /*PR4   */= dx7CtrlDataAllVoice[topNum+105];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_130] /*PL1   */= dx7CtrlDataAllVoice[topNum+106];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_131] /*PL2   */= dx7CtrlDataAllVoice[topNum+107];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_132] /*PL3   */= dx7CtrlDataAllVoice[topNum+108];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_133] /*PL4   */= dx7CtrlDataAllVoice[topNum+109];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_134] /*ALS   */= dx7CtrlDataAllVoice[topNum+110];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_135] /*FBL   */= dx7CtrlDataAllVoice[topNum+111]&0x7;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_136] /*OPI   */=(dx7CtrlDataAllVoice[topNum+111]>>3)&0x1;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_137] /*LFS   */= dx7CtrlDataAllVoice[topNum+112];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_138] /*LFD   */= dx7CtrlDataAllVoice[topNum+113];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_139] /*LPMD  */= dx7CtrlDataAllVoice[topNum+114];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_140] /*LAMD  */= dx7CtrlDataAllVoice[topNum+115];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_141] /*LFKS  */= dx7CtrlDataAllVoice[topNum+116]&0x1;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_142] /*LFW   */=(dx7CtrlDataAllVoice[topNum+116]>>1)&0x7;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_143] /*LPMS  */=(dx7CtrlDataAllVoice[topNum+116]>>4)&0x7;
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_144] /*TRNP  */= dx7CtrlDataAllVoice[topNum+117];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_145] /*VNAM1 */= dx7CtrlDataAllVoice[topNum+118];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_146] /*VNAM2 */= dx7CtrlDataAllVoice[topNum+119];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_147] /*VNAM3 */= dx7CtrlDataAllVoice[topNum+120];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_148] /*VNAM4 */= dx7CtrlDataAllVoice[topNum+121];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_149] /*VNAM5 */= dx7CtrlDataAllVoice[topNum+122];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_150] /*VNAM6 */= dx7CtrlDataAllVoice[topNum+123];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_151] /*VNAM7 */= dx7CtrlDataAllVoice[topNum+124];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_152] /*VNAM8 */= dx7CtrlDataAllVoice[topNum+125];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_153] /*VNAM9 */= dx7CtrlDataAllVoice[topNum+126];
        dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_154] /*VNAM10*/= dx7CtrlDataAllVoice[topNum+127];

        copyToParamCtrl(DX7_CTRL_SEQ_1VOICE);
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * 内容  : ボイスコピー(1VOICEからALL VOICEへ)
 * 引数  : INT voiceNum
 * 戻り値: BOOL
 ***************************************/
static BOOL
dx7voiceCopyFromOneToOneOfAll( INT voiceNum )
{
    if( voiceNum < 32 )
    {
        INT topNum = DX7_SYSEX_ALL_VOICE_DATA + (voiceNum*DX7_SYSEX_VMEM_MAX);

        /* */dx7CtrlDataAllVoice[topNum+0  ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_00 ]; /*R1    */
        /* */dx7CtrlDataAllVoice[topNum+1  ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_01 ]; /*R2    */
        /* */dx7CtrlDataAllVoice[topNum+2  ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_02 ]; /*R3    */
        /* */dx7CtrlDataAllVoice[topNum+3  ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_03 ]; /*R4    */
        /* */dx7CtrlDataAllVoice[topNum+4  ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_04 ]; /*L1    */
        /* */dx7CtrlDataAllVoice[topNum+5  ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_05 ]; /*L2    */
        /* */dx7CtrlDataAllVoice[topNum+6  ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_06 ]; /*L3    */
        /* */dx7CtrlDataAllVoice[topNum+7  ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_07 ]; /*L4    */
        /* */dx7CtrlDataAllVoice[topNum+8  ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_08 ]; /*BP    */
        /* */dx7CtrlDataAllVoice[topNum+9  ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_09 ]; /*LD    */
        /* */dx7CtrlDataAllVoice[topNum+10 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_10 ]; /*RD    */
        /* */dx7CtrlDataAllVoice[topNum+11 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_12 ]&0x3)<<2) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_11 ]&0x3)  ;/*RC    *//*LC    */
        /* */dx7CtrlDataAllVoice[topNum+12 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_20 ]&0xF)<<3) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_13 ]&0x7)  ;/*PD    *//*RS    */
        /* */dx7CtrlDataAllVoice[topNum+13 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_15 ]&0x7)<<2) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_14 ]&0x3)  ;/*TS    *//*AMS   */
        /* */dx7CtrlDataAllVoice[topNum+14 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_16 ]; /*TL    */
        /* */dx7CtrlDataAllVoice[topNum+15 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_18 ]&0x1F)<<1)| ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_17 ])&0x1);/*PC    *//*PM    */
        /* */dx7CtrlDataAllVoice[topNum+16 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_19 ]; /*PF    */
        /* */dx7CtrlDataAllVoice[topNum+17 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_21 ]; /*R1    */
        /* */dx7CtrlDataAllVoice[topNum+18 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_22 ]; /*R2    */
        /* */dx7CtrlDataAllVoice[topNum+19 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_23 ]; /*R3    */
        /* */dx7CtrlDataAllVoice[topNum+20 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_24 ]; /*R4    */
        /* */dx7CtrlDataAllVoice[topNum+21 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_25 ]; /*L1    */
        /* */dx7CtrlDataAllVoice[topNum+22 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_26 ]; /*L2    */
        /* */dx7CtrlDataAllVoice[topNum+23 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_27 ]; /*L3    */
        /* */dx7CtrlDataAllVoice[topNum+24 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_28 ]; /*L4    */
        /* */dx7CtrlDataAllVoice[topNum+25 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_29 ]; /*BP    */
        /* */dx7CtrlDataAllVoice[topNum+26 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_30 ]; /*LD    */
        /* */dx7CtrlDataAllVoice[topNum+27 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_31 ]; /*RD    */
        /* */dx7CtrlDataAllVoice[topNum+28 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_33 ]&0x3)<<2) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_32 ]&0x3)  ;/*RC    *//*LC    */
        /* */dx7CtrlDataAllVoice[topNum+29 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_41 ]&0xF)<<3) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_34 ]&0x7)  ;/*PD    *//*RS    */
        /* */dx7CtrlDataAllVoice[topNum+30 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_36 ]&0x7)<<2) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_35 ]&0x3)  ;/*TS    *//*AMS   */
        /* */dx7CtrlDataAllVoice[topNum+31 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_37 ]; /*TL    */
        /* */dx7CtrlDataAllVoice[topNum+32 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_39 ]&0x1F)<<1)| ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_38 ])&0x1);/*PC    *//*PM    */
        /* */dx7CtrlDataAllVoice[topNum+33 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_40 ]; /*PF    */
        /* */dx7CtrlDataAllVoice[topNum+34 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_42 ]; /*R1    */
        /* */dx7CtrlDataAllVoice[topNum+35 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_43 ]; /*R2    */
        /* */dx7CtrlDataAllVoice[topNum+36 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_44 ]; /*R3    */
        /* */dx7CtrlDataAllVoice[topNum+37 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_45 ]; /*R4    */
        /* */dx7CtrlDataAllVoice[topNum+38 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_46 ]; /*L1    */
        /* */dx7CtrlDataAllVoice[topNum+39 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_47 ]; /*L2    */
        /* */dx7CtrlDataAllVoice[topNum+40 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_48 ]; /*L3    */
        /* */dx7CtrlDataAllVoice[topNum+41 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_49 ]; /*L4    */
        /* */dx7CtrlDataAllVoice[topNum+42 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_50 ]; /*BP    */
        /* */dx7CtrlDataAllVoice[topNum+43 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_51 ]; /*LD    */
        /* */dx7CtrlDataAllVoice[topNum+44 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_52 ]; /*RD    */
        /* */dx7CtrlDataAllVoice[topNum+45 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_54 ]&0x3)<<2) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_53 ]&0x3)  ;/*RC    *//*LC    */
        /* */dx7CtrlDataAllVoice[topNum+46 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_62 ]&0xF)<<3) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_55 ]&0x7)  ;/*PD    *//*RS    */
        /* */dx7CtrlDataAllVoice[topNum+47 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_57 ]&0x7)<<2) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_56 ]&0x3)  ;/*TS    *//*AMS   */
        /* */dx7CtrlDataAllVoice[topNum+48 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_58 ]; /*TL    */
        /* */dx7CtrlDataAllVoice[topNum+49 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_60 ]&0x1F)<<1)| ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_59 ])&0x1);/*PC    *//*PM    */
        /* */dx7CtrlDataAllVoice[topNum+50 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_61 ]; /*PF    */
        /* */dx7CtrlDataAllVoice[topNum+51 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_63 ]; /*R1    */
        /* */dx7CtrlDataAllVoice[topNum+52 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_64 ]; /*R2    */
        /* */dx7CtrlDataAllVoice[topNum+53 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_65 ]; /*R3    */
        /* */dx7CtrlDataAllVoice[topNum+54 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_66 ]; /*R4    */
        /* */dx7CtrlDataAllVoice[topNum+55 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_67 ]; /*L1    */
        /* */dx7CtrlDataAllVoice[topNum+56 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_68 ]; /*L2    */
        /* */dx7CtrlDataAllVoice[topNum+57 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_69 ]; /*L3    */
        /* */dx7CtrlDataAllVoice[topNum+58 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_70 ]; /*L4    */
        /* */dx7CtrlDataAllVoice[topNum+59 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_71 ]; /*BP    */
        /* */dx7CtrlDataAllVoice[topNum+60 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_72 ]; /*LD    */
        /* */dx7CtrlDataAllVoice[topNum+61 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_73 ]; /*RD    */
        /* */dx7CtrlDataAllVoice[topNum+62 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_75 ]&0x3)<<2) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_74 ]&0x3)  ;/*RC    *//*LC    */
        /* */dx7CtrlDataAllVoice[topNum+63 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_83 ]&0xF)<<3) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_76 ]&0x7)  ;/*PD    *//*RS    */
        /* */dx7CtrlDataAllVoice[topNum+64 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_78 ]&0x7)<<2) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_77 ]&0x3)  ;/*TS    *//*AMS   */
        /* */dx7CtrlDataAllVoice[topNum+65 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_79 ]; /*TL    */
        /* */dx7CtrlDataAllVoice[topNum+66 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_81 ]&0x1F)<<1)| ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_80 ])&0x1);/*PC    *//*PM    */
        /* */dx7CtrlDataAllVoice[topNum+67 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_82 ]; /*PF    */
        /* */dx7CtrlDataAllVoice[topNum+68 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_84 ]; /*R1    */
        /* */dx7CtrlDataAllVoice[topNum+69 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_85 ]; /*R2    */
        /* */dx7CtrlDataAllVoice[topNum+70 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_86 ]; /*R3    */
        /* */dx7CtrlDataAllVoice[topNum+71 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_87 ]; /*R4    */
        /* */dx7CtrlDataAllVoice[topNum+72 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_88 ]; /*L1    */
        /* */dx7CtrlDataAllVoice[topNum+73 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_89 ]; /*L2    */
        /* */dx7CtrlDataAllVoice[topNum+74 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_90 ]; /*L3    */
        /* */dx7CtrlDataAllVoice[topNum+75 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_91 ]; /*L4    */
        /* */dx7CtrlDataAllVoice[topNum+76 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_92 ]; /*BP    */
        /* */dx7CtrlDataAllVoice[topNum+77 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_93 ]; /*LD    */
        /* */dx7CtrlDataAllVoice[topNum+78 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_94 ]; /*RD    */
        /* */dx7CtrlDataAllVoice[topNum+79 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_96 ]&0x3)<<2) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_95 ]&0x3)  ;/*RC    *//*LC    */
        /* */dx7CtrlDataAllVoice[topNum+80 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_104]&0xF)<<3) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_97 ]&0x7)  ;/*PD    *//*RS    */
        /* */dx7CtrlDataAllVoice[topNum+81 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_99 ]&0x7)<<2) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_98 ]&0x3)  ;/*TS    *//*AMS   */
        /* */dx7CtrlDataAllVoice[topNum+82 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_100]; /*TL    */
        /* */dx7CtrlDataAllVoice[topNum+83 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_102]&0x1F)<<1)| ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_101])&0x1);/*PC    *//*PM    */
        /* */dx7CtrlDataAllVoice[topNum+84 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_103]; /*PF    */
        /* */dx7CtrlDataAllVoice[topNum+85 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_105]; /*R1    */
        /* */dx7CtrlDataAllVoice[topNum+86 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_106]; /*R2    */
        /* */dx7CtrlDataAllVoice[topNum+87 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_107]; /*R3    */
        /* */dx7CtrlDataAllVoice[topNum+88 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_108]; /*R4    */
        /* */dx7CtrlDataAllVoice[topNum+89 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_109]; /*L1    */
        /* */dx7CtrlDataAllVoice[topNum+90 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_110]; /*L2    */
        /* */dx7CtrlDataAllVoice[topNum+91 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_111]; /*L3    */
        /* */dx7CtrlDataAllVoice[topNum+92 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_112]; /*L4    */
        /* */dx7CtrlDataAllVoice[topNum+93 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_113]; /*BP    */
        /* */dx7CtrlDataAllVoice[topNum+94 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_114]; /*LD    */
        /* */dx7CtrlDataAllVoice[topNum+95 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_115]; /*RD    */
        /* */dx7CtrlDataAllVoice[topNum+96 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_117]&0x3)<<2) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_116]&0x3)  ;/*RC    *//*LC    */
        /* */dx7CtrlDataAllVoice[topNum+97 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_125]&0xF)<<3) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_118]&0x7)  ;/*PD    *//*RS    */
        /* */dx7CtrlDataAllVoice[topNum+98 ]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_120]&0x7)<<2) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_119]&0x3)  ;/*TS    *//*AMS   */
        /* */dx7CtrlDataAllVoice[topNum+99 ]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_121]; /*TL    */
        /* */dx7CtrlDataAllVoice[topNum+100]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_123]&0x1F)<<1)| ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_122])&0x1);/*PC    *//*PM    */
        /* */dx7CtrlDataAllVoice[topNum+101]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_124]; /*PF    */
        /* */dx7CtrlDataAllVoice[topNum+102]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_126]; /*PR1   */
        /* */dx7CtrlDataAllVoice[topNum+103]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_127]; /*PR2   */
        /* */dx7CtrlDataAllVoice[topNum+104]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_128]; /*PR3   */
        /* */dx7CtrlDataAllVoice[topNum+105]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_129]; /*PR4   */
        /* */dx7CtrlDataAllVoice[topNum+106]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_130]; /*PL1   */
        /* */dx7CtrlDataAllVoice[topNum+107]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_131]; /*PL2   */
        /* */dx7CtrlDataAllVoice[topNum+108]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_132]; /*PL3   */
        /* */dx7CtrlDataAllVoice[topNum+109]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_133]; /*PL4   */
        /* */dx7CtrlDataAllVoice[topNum+110]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_134]; /*ALS   */
        /* */dx7CtrlDataAllVoice[topNum+111]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_136]&0x1)<<3) | (dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_135]&0x7); /*OPI   *//*FBL   */
        /* */dx7CtrlDataAllVoice[topNum+112]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_137]; /*LFS   */
        /* */dx7CtrlDataAllVoice[topNum+113]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_138]; /*LFD   */
        /* */dx7CtrlDataAllVoice[topNum+114]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_139]; /*LPMD  */
        /* */dx7CtrlDataAllVoice[topNum+115]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_140]; /*LAMD  */
        /* */dx7CtrlDataAllVoice[topNum+116]/*            */= ((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_143]&0x7)<<4)|((dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_142]&0x7)<<1)|(dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_141]&0x1); /*LPMS  *//*LFW   *//*LFKS  */
        /* */dx7CtrlDataAllVoice[topNum+117]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_144]; /*TRNP  */
        /* */dx7CtrlDataAllVoice[topNum+118]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_145]; /*VNAM1 */
        /* */dx7CtrlDataAllVoice[topNum+119]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_146]; /*VNAM2 */
        /* */dx7CtrlDataAllVoice[topNum+120]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_147]; /*VNAM3 */
        /* */dx7CtrlDataAllVoice[topNum+121]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_148]; /*VNAM4 */
        /* */dx7CtrlDataAllVoice[topNum+122]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_149]; /*VNAM5 */
        /* */dx7CtrlDataAllVoice[topNum+123]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_150]; /*VNAM6 */
        /* */dx7CtrlDataAllVoice[topNum+124]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_151]; /*VNAM7 */
        /* */dx7CtrlDataAllVoice[topNum+125]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_152]; /*VNAM8 */
        /* */dx7CtrlDataAllVoice[topNum+126]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_153]; /*VNAM9 */
        /* */dx7CtrlDataAllVoice[topNum+127]/*            */= dx7CtrlDataOneVoice[DX7_SYSEX_1VOICE_DATA_154]; /*VNAM10*/

        copyToParamCtrl(DX7_CTRL_SEQ_ALL_VOICE);
    }
    else
    {
        nop();
    }
}

/********************************************************************************
 * 内容  :
 * 引数  : DX7_PARAM_CTRL_ID modId
 * 引数  : DX7_PARAM_CTRL_ID coarseId
 * 引数  : DX7_PARAM_CTRL_ID fineId
 * 引数  : DX7_PARAM_CTRL_ID valueId
 * 戻り値: なし
 ***************************************/
static void
dx7SetFrequencyText( DX7_PARAM_CTRL_ID modeId, DX7_PARAM_CTRL_ID coarseId, DX7_PARAM_CTRL_ID fineId, DX7_PARAM_CTRL_ID valueId)
{
    INT coarse,fine;
    float freq;
    TCHAR szBuff[128];

    coarse = getParamCtrlValue(coarseId);
    fine = getParamCtrlValue(fineId);

    if( getParamCtrlValue(modeId) == 1 )
    { /* FIXED */
        coarse &= 0x3;
        freq = pow(10,((float)1+(fine-1))/100) * pow(10,coarse);
        sprintf(szBuff,"%.*f",3,freq);
    }
    else
    { /* RATIO */
        freq = ((float)1.00 + (float)0.01*fine) * (coarse ? coarse : 0.5);
        sprintf(szBuff,"%.*f",3,freq);
    }

    SetWindowText( Dx7ParamCtrlGetHWND(valueId),szBuff);
}


//#define DX7_CTRL_DEBUG_DATA_DISP
/*********************************************
 * 内容   : 
 * 引数   : DX7_CTRL_SEQ_ID seqId
 * 戻り値 : なし
 **********************************************/
static void
debugDataArrayPrint( INT rxDataSize, BYTE *rxDataPtr, PTSTR ptstrTitle )
{
    INT i;

#ifdef DX7_CTRL_DEBUG_DATA_DISP
    DebugWndPrintf("---------------------------------------------------\r\n");
    DebugWndPrintf("%s:\r\n",ptstrTitle);
    DebugWndPrintf("Size:%d\r\n",rxDataSize);

    for( i=0; i<rxDataSize; i++ )
    {
        DebugWndPrintf("0x%02X ",*(rxDataPtr+i));
        if( !((i+1)%16) )
        {
            DebugWndPrintf("\r\n");
        }
    }
    DebugWndPrintf("\r\n");
    DebugWndPrintf("---------------------------------------------------\r\n");
#endif
}
