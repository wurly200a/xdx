/* 共通インクルードファイル */
#include "common.h"
/* 個別インクルードファイル */
#include "SomeCtrl.h"
#include "dx100ParamCtrl.h"
#include "Midi.h"
#include "dx100Ctrl.h"
#include "DebugWnd.h"
#include "dx100CtrlDef.h"

/* 外部関数定義 */

/* 外部変数定義 */

/* 内部関数定義 */
static BOOL seqStart( DX100_CTRL_SEQ_METHOD method, DX100_CTRL_SEQ_ID seqId );
static INT seqStartProc( DX100_CTRL_SEQ_METHOD method, DX100_CTRL_SEQ_ID seqId, INT maxDataSize, BYTE *txDataPtr );
static BOOL seqEndProc( DX100_CTRL_SEQ_ID seqId, INT rxDataSize, BYTE *rxDataPtr );
static BOOL copyToParamCtrl( DX100_CTRL_SEQ_ID seqId );
static BOOL copyFromParamCtrl( DX100_CTRL_SEQ_ID seqId );
static BYTE getParamCtrlValue( DX100_PARAM_CTRL_ID id );
static BOOL displayContents( void );
static BYTE calcCheckSum( BYTE *dataPtr, INT dataSize );
static BOOL dx100voiceCopyFromAllToOne( INT voiceNum );
static BOOL dx100voiceCopyFromOneToOneOfAll( INT voiceNum );
static void debugDataArrayPrint( INT rxDataSize, BYTE *rxDataPtr, PTSTR ptstrTitle );

/* 内部変数定義 */
static BYTE dx100CtrlSeqTxTempData[1024*8];
static BYTE dx100CtrlSeqRxTempData[1024*8];
static INT dx100CtrlSeqTempRxSize;

static BYTE dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_INDEX_MAX];
static BYTE dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_INDEX_MAX];

typedef struct
{
    INT   rxDataSize  ; /* 受信データ格納領域のサイズ               */
    BYTE *rxDataPtr   ; /* 受信データ格納領域のポインタ             */
} S_DX100_CTRL_SEQ_DATA;

static const S_DX100_CTRL_SEQ_DATA dx100CtrlSeqDataTbl[DX100_CTRL_SEQ_NUM_MAX] =
{
    {(INT)0                              ,(BYTE *)NULL                   }, /* DX100_CTRL_SEQ_NON_EXEC     */
    {(INT)DX100_SYSEX_1VOICE_INDEX_MAX   ,&dx100CtrlDataOneVoice         }, /* DX100_CTRL_SEQ_1VOICE */
    {(INT)DX100_SYSEX_ALL_VOICE_INDEX_MAX,&dx100CtrlDataAllVoice         }, /* DX100_CTRL_SEQ_ALL_VOICE */
};

typedef struct
{
    DX100_CTRL_SEQ_METHOD          nowMethod            ;
    DX100_CTRL_SEQ_ID              reqSeqIdStart        ;
    DX100_CTRL_SEQ_ID              reqSeqIdEnd          ;
    DX100_CTRL_SEQ_ID              nowExecSeqId         ;
    DX100_CTRL_MODE                nowMode              ;
} S_DX100_CTRL_INFO;

static S_DX100_CTRL_INFO dx100CtrlInfo;


/*********************************************
 * 内容   : 初期化
 * 引数   : HINSTANCE hInst
 * 引数   : PTSTR szAppName
 * 引数   : HWND hwnd
 * 戻り値 : BOOL
 **********************************************/
BOOL
Dx100CtrlInit( HINSTANCE hInst, PTSTR szAppName, HWND hwnd )
{
    BOOL bRtn = TRUE;
    INT i;

    dx100CtrlInfo.nowMode = DX100_CTRL_MODE_PATCH;

    Dx100ParamCtrlCreate( hInst, szAppName, hwnd ); /* コントロールを生成 */

    /* 1VOICE 初期値 */
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_REQUEST_STATUS    ] = EX_STATUS;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_REQUEST_ID_NO     ] = EX_ID_NUMBER_YAMAHA;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_REQUEST_SUB_STATUS] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_REQUEST_FORMAT_NO ] = DX100_DUMP_REQ_FORMAT_1VOICE;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_BYTE_COUNT_HIGH   ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_BYTE_COUNT_LOW    ] = 0x5D;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_00                  ] = 0x1F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_01                  ] = 0x1F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_02                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_03                  ] = 0x0F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_04                  ] = 0x0F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_05                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_06                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_07                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_08                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_09                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_10                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_11                  ] = 0x04;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_12                  ] = 0x03;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_13                  ] = 0x1F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_14                  ] = 0x1F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_15                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_16                  ] = 0x0F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_17                  ] = 0x0F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_18                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_19                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_20                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_21                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_22                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_23                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_24                  ] = 0x04;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_25                  ] = 0x03;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_26                  ] = 0x1F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_27                  ] = 0x1F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_28                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_29                  ] = 0x0F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_30                  ] = 0x0F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_31                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_32                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_33                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_34                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_35                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_36                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_37                  ] = 0x04;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_38                  ] = 0x03;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_39                  ] = 0x1F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_40                  ] = 0x1F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_41                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_42                  ] = 0x0F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_43                  ] = 0x0F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_44                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_45                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_46                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_47                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_48                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_49                  ] = 0x5A;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_50                  ] = 0x04;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_51                  ] = 0x03;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_52                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_53                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_54                  ] = 0x23;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_55                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_56                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_57                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_58                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_59                  ] = 0x02;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_60                  ] = 0x06;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_61                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_62                  ] = 0x18;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_63                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_64                  ] = 0x04;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_65                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_66                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_67                  ] = 0x63;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_68                  ] = 0x01;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_69                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_70                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_71                  ] = 0x32;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_72                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_73                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_74                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_75                  ] = 0x32;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_76                  ] = 0x00;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_77                  ] = 0x49;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_78                  ] = 0x4E;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_79                  ] = 0x49;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_80                  ] = 0x54;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_81                  ] = 0x20;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_82                  ] = 0x56;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_83                  ] = 0x4F;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_84                  ] = 0x49;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_85                  ] = 0x43;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_86                  ] = 0x45;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_87                  ] = 0x63;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_88                  ] = 0x63;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_89                  ] = 0x63;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_90                  ] = 0x32;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_91                  ] = 0x32;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_92                  ] = 0x32;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_FOOTER_CHECKSUM          ] = 0x02;
    dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_FOOTER_ETX               ] = EX_ETX;
    copyToParamCtrl(DX100_CTRL_SEQ_1VOICE);

    /* ALL VOICE 初期値 */
    dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_HEADER_REQUEST_STATUS    ] = EX_STATUS;
    dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_HEADER_REQUEST_ID_NO     ] = EX_ID_NUMBER_YAMAHA;
    dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_HEADER_REQUEST_SUB_STATUS] = 0x00;
    dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_HEADER_REQUEST_FORMAT_NO ] = DX100_DUMP_REQ_FORMAT_32VOICE;
    dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_HEADER_BYTE_COUNT_HIGH   ] = 0x20;
    dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_HEADER_BYTE_COUNT_LOW    ] = 0x00;

    for( i=0; i<32; i++ )
    {
        INT topNum = DX100_SYSEX_ALL_VOICE_DATA + (i*DX100_SYSEX_VMEM_MAX);

        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_00 ] = 0x1F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_01 ] = 0x1F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_02 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_03 ] = 0x0F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_04 ] = 0x0F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_05 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_06 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_07 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_08 ] = 0x04;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_09 ] = 0x03;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_10 ] = 0x1F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_11 ] = 0x1F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_12 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_13 ] = 0x0F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_14 ] = 0x0F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_15 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_16 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_17 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_18 ] = 0x04;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_19 ] = 0x03;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_20 ] = 0x1F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_21 ] = 0x1F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_22 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_23 ] = 0x0F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_24 ] = 0x0F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_25 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_26 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_27 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_28 ] = 0x04;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_29 ] = 0x03;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_30 ] = 0x1F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_31 ] = 0x1F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_32 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_33 ] = 0x0F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_34 ] = 0x0F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_35 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_36 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_37 ] = 0x5A;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_38 ] = 0x04;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_39 ] = 0x03;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_40 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_41 ] = 0x23;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_42 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_43 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_44 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_45 ] = 0x62;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_46 ] = 0x18;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_47 ] = 0x04;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_48 ] = 0x04;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_49 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_50 ] = 0x63;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_51 ] = 0x32;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_52 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_53 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_54 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_55 ] = 0x32;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_56 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_57 ] = 0x49;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_58 ] = 0x4E;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_59 ] = 0x49;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_60 ] = 0x54;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_61 ] = 0x20;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_62 ] = 0x56;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_63 ] = 0x4F;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_64 ] = 0x49;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_65 ] = 0x43;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_66 ] = 0x45;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_67 ] = 0x63;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_68 ] = 0x63;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_69 ] = 0x63;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_70 ] = 0x32;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_71 ] = 0x32;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_72 ] = 0x32;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_73 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_74 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_75 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_76 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_77 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_78 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_79 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_80 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_81 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_82 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_83 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_84 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_85 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_86 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_87 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_88 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_89 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_90 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_91 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_92 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_93 ] = 0x20;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_94 ] = 0xF7;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_95 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_96 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_97 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_98 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_99 ] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_100] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_101] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_102] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_103] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_104] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_105] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_106] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_107] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_108] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_109] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_110] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_111] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_112] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_113] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_114] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_115] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_116] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_117] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_118] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_119] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_120] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_121] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_122] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_123] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_124] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_125] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_126] = 0x00;
        dx100CtrlDataAllVoice[topNum+DX100_SYSEX_VMEM_127] = 0x00;
    }
    dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_FOOTER_CHECKSUM] = 0x40;
    dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_FOOTER_ETX     ] = EX_ETX;

    copyToParamCtrl(DX100_CTRL_SEQ_ALL_VOICE);

    return bRtn;
}

/*********************************************
 * 内容   : 
 * 引数   : DX100_CTRL_MODE mode
 * 戻り値 : BOOL
 **********************************************/
BOOL
Dx100CtrlModeSet(DX100_CTRL_MODE mode)
{
    dx100CtrlInfo.nowMode = mode;

    Dx100CtrlDisplayUpdate();

    return TRUE;
}

BOOL
Dx100CtrlDisplayUpdate( void )
{
    if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_NONE )
    {
        Dx100ParamCtrlGroupDisplay(DX100_PARAM_CTRL_GROUP_NONE);
    }
    else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_PATCH )
    {
        Dx100ParamCtrlGroupDisplay(DX100_PARAM_CTRL_GROUP_1VOICE);
    }
    else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_ALL_VOICE )
    {
        Dx100ParamCtrlGroupDisplay(DX100_PARAM_CTRL_GROUP_ALL_VOICE);
    }
    else
    {
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
Dx100CtrlSeqStart( DX100_CTRL_SEQ_METHOD method, DX100_CTRL_SEQ_ID seqIdStart, DX100_CTRL_SEQ_ID seqIdEnd )
{
    BOOL bRtn = TRUE;

    if( (seqIdStart && (seqIdStart < DX100_CTRL_SEQ_NUM_MAX)) &&
        (seqIdEnd   && (seqIdEnd < DX100_CTRL_SEQ_NUM_MAX)) &&
        (seqIdStart <= seqIdEnd) )
    {
        dx100CtrlInfo.nowMethod     = method;
        dx100CtrlInfo.reqSeqIdStart = seqIdStart;
        dx100CtrlInfo.reqSeqIdEnd   = seqIdEnd  ;

        seqStart(method,dx100CtrlInfo.reqSeqIdStart);
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
Dx100CtrlDisplayContents( void )
{
    return displayContents();
}

/*********************************************
 * 内容   : 
 * 引数   : 
 * 戻り値 : 
 **********************************************/
void
Dx100CtrlCycleProc( void )
{
    if( dx100CtrlInfo.nowExecSeqId )
    {
        if( MidiInqOutExec() )
        {
            nop();
        }
        else
        { /* 送信終了 */
            DebugWndPrintf("TX_END\r\n");

            if( dx100CtrlInfo.nowMethod == DX100_CTRL_SEQ_METHOD_SET )
            {
                if( dx100CtrlInfo.nowExecSeqId < dx100CtrlInfo.reqSeqIdEnd )
                { /* 次のシーケンスがある */
                    seqStart(dx100CtrlInfo.nowMethod,dx100CtrlInfo.nowExecSeqId+1);
                }
                else
                {
                    dx100CtrlInfo.reqSeqIdStart = DX100_CTRL_SEQ_NON_EXEC;
                    dx100CtrlInfo.reqSeqIdEnd   = DX100_CTRL_SEQ_NON_EXEC;
                    dx100CtrlInfo.nowExecSeqId  = DX100_CTRL_SEQ_NON_EXEC;
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
            if( dx100CtrlInfo.nowMethod == DX100_CTRL_SEQ_METHOD_GET )
            {
                DebugWndPrintf("RX_END\r\n");
                seqEndProc(dx100CtrlInfo.nowExecSeqId,dx100CtrlSeqTempRxSize,dx100CtrlSeqRxTempData);
            }
            else
            { /* SETの時 */
                DebugWndPrintf("RX_NOT_EXEC\r\n");
            }

            if( dx100CtrlInfo.nowExecSeqId < dx100CtrlInfo.reqSeqIdEnd )
            { /* 次のシーケンスがある */
                seqStart(dx100CtrlInfo.nowMethod,dx100CtrlInfo.nowExecSeqId+1);
            }
            else
            {
                /* 全てのシーケンス終了 */
                dx100CtrlInfo.reqSeqIdStart = DX100_CTRL_SEQ_NON_EXEC;
                dx100CtrlInfo.reqSeqIdEnd   = DX100_CTRL_SEQ_NON_EXEC;
                dx100CtrlInfo.nowExecSeqId  = DX100_CTRL_SEQ_NON_EXEC;

                if( dx100CtrlInfo.nowMethod == DX100_CTRL_SEQ_METHOD_GET )
                {
                    if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_NONE )
                    {
                    }
                    else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_PATCH )
                    {
                        copyToParamCtrl(DX100_CTRL_SEQ_1VOICE);
                    }
                    else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_ALL_VOICE )
                    {
                        copyToParamCtrl(DX100_CTRL_SEQ_ALL_VOICE);
                    }
                    else
                    {
                    }

                    Dx100CtrlDisplayUpdate();
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
seqStart( DX100_CTRL_SEQ_METHOD method, DX100_CTRL_SEQ_ID seqId )
{
    INT txSize;

    if( seqId && (seqId < DX100_CTRL_SEQ_NUM_MAX) )
    {
        DebugWndPrintf("..............\r\n");
        DebugWndPrintf("DX100_CTRL_SEQ_ID:%d\r\n",seqId);

        txSize = seqStartProc(method,seqId,1024,dx100CtrlSeqTxTempData);

        debugDataArrayPrint(txSize,dx100CtrlSeqTxTempData,"TX");

        dx100CtrlInfo.nowExecSeqId = seqId;

        if( method == DX100_CTRL_SEQ_METHOD_SET )
        {
            nop();
        }
        else
        {
            MidiInExclusiveData( &dx100CtrlSeqRxTempData, &dx100CtrlSeqTempRxSize );
        }

        MidiOutExclusiveData( &dx100CtrlSeqTxTempData, txSize );
    }
    else
    {
        nop();
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : DX100_CTRL_SEQ_ID seqId
 * 戻り値 : INT
 **********************************************/
static INT
seqStartProc( DX100_CTRL_SEQ_METHOD method, DX100_CTRL_SEQ_ID seqId, INT maxDataSize, BYTE *txDataPtr )
{
    S_DX100_CTRL_SEQ_DATA *tblPtr;
    INT txSize = (INT)0;
    INT i;

    tblPtr = &(dx100CtrlSeqDataTbl[seqId]);

    switch( seqId )
    {
    case DX100_CTRL_SEQ_1VOICE:
        if( method == DX100_CTRL_SEQ_METHOD_SET )
        {
            copyFromParamCtrl( seqId );
            memcpy((void *)txDataPtr,(void *)tblPtr->rxDataPtr,tblPtr->rxDataSize);
            txSize = DX100_SYSEX_1VOICE_INDEX_MAX;
        }
        else
        { /* データ要求しデータ受信する*/
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_STATUS    ) = EX_STATUS                   ;
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_ID_NO     ) = EX_ID_NUMBER_YAMAHA         ;
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_SUB_STATUS) = 0x20               | 0x00/* MIDI CH*/;
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_FORMAT_NO ) = DX100_DUMP_REQ_FORMAT_1VOICE;
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_EOX       ) = EX_ETX                      ;
            txSize = DX100_SYSEX_BULK_DUMP_REQUEST_INDEX_MAX;
        }
        break;
    case DX100_CTRL_SEQ_ALL_VOICE:
        if( method == DX100_CTRL_SEQ_METHOD_SET )
        {
            memcpy((void *)txDataPtr,(void *)tblPtr->rxDataPtr,tblPtr->rxDataSize);
            txSize = DX100_SYSEX_ALL_VOICE_INDEX_MAX;
        }
        else
        { /* データ要求しデータ受信する*/
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_STATUS    ) = EX_STATUS                    ;
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_ID_NO     ) = EX_ID_NUMBER_YAMAHA          ;
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_SUB_STATUS) = 0x20               | 0x00/* MIDI CH*/;
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_FORMAT_NO ) = DX100_DUMP_REQ_FORMAT_32VOICE;
            *(txDataPtr+DX100_SYSEX_BULK_DUMP_REQUEST_EOX       ) = EX_ETX                       ;
            txSize = DX100_SYSEX_BULK_DUMP_REQUEST_INDEX_MAX;
        }
        break;
    }

    return txSize;
}

/*********************************************
 * 内容   : 
 * 引数   : DX100_CTRL_SEQ_ID seqId
 * 戻り値 : BOOL
 **********************************************/
static BOOL
seqEndProc( DX100_CTRL_SEQ_ID seqId, INT rxDataSize, BYTE *rxDataPtr )
{
    S_DX100_CTRL_SEQ_DATA *tblPtr = &(dx100CtrlSeqDataTbl[seqId]);

//    DebugWndPrintf("rxDataSize,%d\r\n",rxDataSize);

    debugDataArrayPrint(rxDataSize,rxDataPtr,"RX");

    if( rxDataSize == tblPtr->rxDataSize )
    {
        memcpy((void *)tblPtr->rxDataPtr,(void *)rxDataPtr,tblPtr->rxDataSize);

        switch( seqId )
        {
        case DX100_CTRL_SEQ_1VOICE:
            debugDataArrayPrint(DX100_SYSEX_VCED_MAX,tblPtr->rxDataPtr+DX100_SYSEX_1VOICE_DATA,"CONTENTS");
            break;
        case DX100_CTRL_SEQ_ALL_VOICE:
            debugDataArrayPrint(4096,tblPtr->rxDataPtr+DX100_SYSEX_ALL_VOICE_DATA,"CONTENTS");
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
    case DX100_CTRL_SEQ_1VOICE:
        break;
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : DX100_CTRL_SEQ_ID seqId
 * 戻り値 : BOOL
 **********************************************/
static BOOL
copyToParamCtrl( DX100_CTRL_SEQ_ID seqId )
{
    TCHAR patchName[12+1];
    TCHAR szBuffer[1024];
    INT toneNum;
    INT i,j;

    switch( seqId )
    {
    case DX100_CTRL_SEQ_1VOICE:
        for( i=0; i<DX100_SYSEX_VCED_MAX; i++ )
        {
            INT dataIndex = DX100_SYSEX_1VOICE_DATA+i/*DX100_SYSEX_VCED_00...DX100_SYSEX_VCED_92*/;
            INT paramCtrlIndex;

            if( i < DX100_SYSEX_VCED_77 )
            {
                paramCtrlIndex = DX100_PARAM_CTRL_VOICE_00 + i/*DX100_PARAM_CTRL_VOICE_00...DX100_PARAM_CTRL_VOICE_76*/;

                SendMessage( Dx100ParamCtrlGetHWND(paramCtrlIndex), CB_SETCURSEL, dx100CtrlDataOneVoice[dataIndex], (LPARAM)0 );
            }
            else if( (DX100_SYSEX_VCED_77 <= i) && (i <= DX100_SYSEX_VCED_86) )
            {
                if( DX100_SYSEX_VCED_77 == i )
                {
                    memset(&patchName[0],0,10+1);
                    strncpy(&patchName[0],&dx100CtrlDataOneVoice[dataIndex],10);

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
                    SetWindowText( Dx100ParamCtrlGetHWND(DX100_PARAM_CTRL_VOICE_NAME),&patchName[0]);
                }
                else
                {
                }
            }
            else
            {
                paramCtrlIndex = DX100_PARAM_CTRL_VOICE_87 + (i-DX100_SYSEX_VCED_87)/*DX100_PARAM_CTRL_VOICE_87...DX100_PARAM_CTRL_VOICE_92*/;

                SendMessage( Dx100ParamCtrlGetHWND(paramCtrlIndex), CB_SETCURSEL, dx100CtrlDataOneVoice[dataIndex], (LPARAM)0 );
            }

        }
        break;
    case DX100_CTRL_SEQ_ALL_VOICE:
        for( i=0; i<32; i++ )
        {
            INT j;
            memset(&patchName[0],0,10+1);
            strncpy(&patchName[0],&dx100CtrlDataAllVoice[ DX100_SYSEX_ALL_VOICE_DATA + (i*DX100_SYSEX_VMEM_MAX) + DX100_SYSEX_VMEM_57],10);
            SetWindowText( Dx100ParamCtrlGetHWND(DX100_PARAM_CTRL_ALL_VOICE_NAME_00+i),&patchName[0]);
#if 0 /* パッチ名を出力 */
            DebugWndPrintf("%s\r\n",&patchName[0]);
#endif
        }

        for( i=0; i<32; i++ )
        {
            INT j;
            memset(&szBuffer[0],0,128*2+1);
            for( j=0;j<128; j++ )
            {
                sprintf(&szBuffer[j*2],"%02X",dx100CtrlDataAllVoice[ DX100_SYSEX_ALL_VOICE_DATA + (i*DX100_SYSEX_VMEM_MAX) + j]);
            }
//            SetWindowText( Dx100ParamCtrlGetHWND(DX100_PARAM_CTRL_ALL_VOICE_BULK_00+i),&szBuffer[0]);

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
 * 引数   : DX100_CTRL_SEQ_ID seqId
 * 戻り値 : BOOL
 **********************************************/
static BOOL
copyFromParamCtrl( DX100_CTRL_SEQ_ID seqId )
{
    TCHAR patchName[12+1];
    TCHAR szBuffer[1024];
    INT toneNum;
    INT i,j;
    BYTE checkSum;

    switch( seqId )
    {
    case DX100_CTRL_SEQ_1VOICE:
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_REQUEST_STATUS    ] = EX_STATUS;
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_REQUEST_ID_NO     ] = EX_ID_NUMBER_YAMAHA;
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_REQUEST_SUB_STATUS] = 0x00;
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_REQUEST_FORMAT_NO ] = DX100_DUMP_REQ_FORMAT_1VOICE;
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_BYTE_COUNT_HIGH   ] = 0x00;
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_HEADER_BYTE_COUNT_LOW    ] = 0x5D;

        for( i=0; i<DX100_SYSEX_VCED_MAX; i++ )
        {
            INT dataIndex = DX100_SYSEX_1VOICE_DATA+i/*DX100_SYSEX_VCED_00...DX100_SYSEX_VCED_92*/;
            INT paramCtrlIndex;

            if( i < DX100_SYSEX_VCED_77 )
            {
                paramCtrlIndex = DX100_PARAM_CTRL_VOICE_00 + i/*DX100_PARAM_CTRL_VOICE_00...DX100_PARAM_CTRL_VOICE_76*/;

                dx100CtrlDataOneVoice[dataIndex] = getParamCtrlValue(paramCtrlIndex);
            }
            else if( (DX100_SYSEX_VCED_77 <= i) && (i <= DX100_SYSEX_VCED_86) )
            {
                if( DX100_SYSEX_VCED_77 == i )
                {
                    memset(&patchName[0],0,10+1);
                    Dx100ParamCtrlGetText(DX100_PARAM_CTRL_VOICE_NAME,patchName);
                    memset(&dx100CtrlDataOneVoice[dataIndex],0,10+1);
                    strncpy(&dx100CtrlDataOneVoice[dataIndex],&patchName[0],10);

                    for( j=0; j<10; j++ )
                    {
                        if( dx100CtrlDataOneVoice[dataIndex+j] == '\0' )
                        {
                            dx100CtrlDataOneVoice[dataIndex+j] = ' ';
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
                paramCtrlIndex = DX100_PARAM_CTRL_VOICE_87 + (i-DX100_SYSEX_VCED_87)/*DX100_PARAM_CTRL_VOICE_87...DX100_PARAM_CTRL_VOICE_92*/;

                dx100CtrlDataOneVoice[dataIndex] = getParamCtrlValue(paramCtrlIndex);
            }

        }
        checkSum = calcCheckSum(&(dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA]),DX100_SYSEX_VCED_MAX);
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_FOOTER_CHECKSUM] = checkSum;
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_FOOTER_ETX] = EX_ETX;

        DebugWndPrintf("checkSum:0x%02X\r\n",checkSum);
        break;
    case DX100_CTRL_SEQ_ALL_VOICE:
        dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_HEADER_REQUEST_STATUS    ] = EX_STATUS;
        dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_HEADER_REQUEST_ID_NO     ] = EX_ID_NUMBER_YAMAHA;
        dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_HEADER_REQUEST_SUB_STATUS] = 0x00;
        dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_HEADER_REQUEST_FORMAT_NO ] = DX100_DUMP_REQ_FORMAT_32VOICE;
        dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_HEADER_BYTE_COUNT_HIGH   ] = 0x20;
        dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_HEADER_BYTE_COUNT_LOW    ] = 0x00;

        checkSum = calcCheckSum(&(dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_DATA]),DX100_SYSEX_VMEM_MAX*32);

        dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_FOOTER_CHECKSUM] = checkSum;
        dx100CtrlDataAllVoice[DX100_SYSEX_ALL_VOICE_FOOTER_ETX     ] = EX_ETX;

        DebugWndPrintf("checkSum:0x%02X\r\n",checkSum);
        break;
    }

    return TRUE;
}

/*********************************************
 * 内容   : 
 * 引数   : DX100_CTRL_SEQ_ID seqId
 * 戻り値 : BOOL
 **********************************************/
static BYTE
getParamCtrlValue( DX100_PARAM_CTRL_ID id )
{
    BYTE value = (BYTE)0x00;
    int iCbNum;
    HWND hComboBox;

    hComboBox = Dx100ParamCtrlGetHWND(id);
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
Dx100GetDataSize( DX100_CTRL_SEQ_ID seqId )
{
    S_DX100_CTRL_SEQ_DATA *tblPtr;
    DWORD dwSize = (DWORD)0;

    if( seqId < DX100_CTRL_SEQ_NUM_MAX )
    {
        tblPtr = &(dx100CtrlSeqDataTbl[seqId]);
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
Dx100DataGet( DX100_CTRL_SEQ_ID seqId, TCHAR *dataPtr, DWORD dataSize )
{
    S_DX100_CTRL_SEQ_DATA *tblPtr;
    DWORD dwSize = (DWORD)0;

    if( seqId < DX100_CTRL_SEQ_NUM_MAX )
    {
        copyFromParamCtrl( seqId );

        tblPtr = &(dx100CtrlSeqDataTbl[seqId]);
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
Dx100DataSet( DX100_CTRL_SEQ_ID seqId, TCHAR *dataPtr, DWORD dataSize )
{
    S_DX100_CTRL_SEQ_DATA *tblPtr;
    DWORD dwSize = (DWORD)0;

    if( seqId < DX100_CTRL_SEQ_NUM_MAX )
    {
        tblPtr = &(dx100CtrlSeqDataTbl[seqId]);
        dwSize = tblPtr->rxDataSize;

        if( dwSize <= dataSize )
        {
            memcpy(tblPtr->rxDataPtr,dataPtr,tblPtr->rxDataSize);
        }
        else
        {
        }

        if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_NONE )
        {
        }
        else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_PATCH )
        {
            copyToParamCtrl(DX100_CTRL_SEQ_1VOICE);
        }
        else if( dx100CtrlInfo.nowMode == DX100_CTRL_MODE_ALL_VOICE )
        {
            copyToParamCtrl(DX100_CTRL_SEQ_ALL_VOICE);
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
Dx100CtrlOnCommand( WORD code, WORD notifyCode, DX100_CTRL_MODE *modePtr )
{
    BOOL bRtn = (BOOL)FALSE;
    DX100_CTRL_MODE mode = (DX100_CTRL_MODE)DX100_CTRL_MODE_NUM_MAX;

    switch( code )
    {
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_00+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_01+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_02+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_03+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_04+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_05+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_06+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_07+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_08+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_09+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_10+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_11+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_12+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_13+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_14+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_15+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_16+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_17+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_18+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_19+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_20+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_21+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_22+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_23+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_24+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_25+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_26+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_27+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_28+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_29+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_30+DX100_PARAM_CTRL_ID_OFFSET):
    case (DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_31+DX100_PARAM_CTRL_ID_OFFSET):
        DebugWndPrintf("[DX100] TO 1VOICE,%d=0x%04X\r\n",code-(DX100_PARAM_CTRL_ID_OFFSET+DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_00),notifyCode);
        if( notifyCode == 0 )
        {
            dx100voiceCopyFromAllToOne(code-(DX100_PARAM_CTRL_ID_OFFSET+DX100_PARAM_CTRL_ALL_VOICE_TO_ONE_VOICE_00));
            bRtn = (BOOL)TRUE;
            mode = DX100_CTRL_MODE_PATCH;
        }
        else
        {
            nop();
        }
        break;
    case (DX100_PARAM_CTRL_BUTTON_TO_ALL_VOICE+DX100_PARAM_CTRL_ID_OFFSET):
        DebugWndPrintf("[DX100] TO ALL VOICE,%d=0x%04X\r\n",code,notifyCode);
        dx100voiceCopyFromOneToOneOfAll(getParamCtrlValue(DX100_PARAM_CTRL_COMBOBOX_TO_ALL_VOICE_NUM));
        bRtn = (BOOL)TRUE;
        mode = DX100_CTRL_MODE_ALL_VOICE;
        break;
    default:
        break;
    }

    if( modePtr != NULL )
    {
        if( bRtn && (mode != (DX100_CTRL_MODE)DX100_CTRL_MODE_NUM_MAX) )
        {
            *modePtr = mode;
        }
        else
        {
            *modePtr = dx100CtrlInfo.nowMode;
        }
    }
    else
    {
        nop();
    }

    return bRtn;
}

/********************************************************************************
 * 内容  : ボイスコピー(ALL VOICEから1VOICEへ)
 * 引数  : INT voiceNum
 * 戻り値: BOOL
 ***************************************/
static BOOL
dx100voiceCopyFromAllToOne( INT voiceNum )
{
    if( voiceNum < 32 )
    {
        INT topNum = DX100_SYSEX_ALL_VOICE_DATA + (voiceNum*DX100_SYSEX_VMEM_MAX);

        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_00]/* OP4 ATTACK RATE                                  */ = dx100CtrlDataAllVoice[topNum+ 0];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_01]/* OP4 DECAY 1 RATE                                 */ = dx100CtrlDataAllVoice[topNum+ 1];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_02]/* OP4 DECAY 2 RATE                                 */ = dx100CtrlDataAllVoice[topNum+ 2];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_03]/* OP4 RELEASE RATE                                 */ = dx100CtrlDataAllVoice[topNum+ 3];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_04]/* OP4 DECAY 1 LEVEL                                */ = dx100CtrlDataAllVoice[topNum+ 4];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_05]/* OP4 KEYBOARD SCALING LEVEL                       */ = dx100CtrlDataAllVoice[topNum+ 5];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_06]/* OP4 KEYBOARD SCALING RATE                        */ = (dx100CtrlDataAllVoice[topNum+ 9])>>3; /* BIT4-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_07]/* OP4 EG BIAS SENSITIVITY                          */ = ((dx100CtrlDataAllVoice[topNum+ 6])>>3)&0x7; /* BIT5-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_08]/* OP4 AMPLITUDE MODULATION ENABLE                  */ = ((dx100CtrlDataAllVoice[topNum+ 6])>>6)&0x1; /* BIT6   */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_09]/* OP4 KEY VELOCITY                                 */ = ((dx100CtrlDataAllVoice[topNum+ 6])   )&0x7; /* BIT2-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_10]/* OP4 OUTPUT LEVEL                                 */ = dx100CtrlDataAllVoice[topNum+ 7];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_11]/* OP4 OSCILLATOR FREQUENCY                         */ = dx100CtrlDataAllVoice[topNum+ 8];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_12]/* OP4 DETUNE                                       */ = (dx100CtrlDataAllVoice[topNum+ 9])&0x7; /* BIT2-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_13]/* OP2 ATTACK RATE                                  */ = dx100CtrlDataAllVoice[topNum+10];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_14]/* OP2 DECAY 1 RATE                                 */ = dx100CtrlDataAllVoice[topNum+11];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_15]/* OP2 DECAY 2 RATE                                 */ = dx100CtrlDataAllVoice[topNum+12];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_16]/* OP2 RELEASE RATE                                 */ = dx100CtrlDataAllVoice[topNum+13];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_17]/* OP2 DECAY 1 LEVEL                                */ = dx100CtrlDataAllVoice[topNum+14];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_18]/* OP2 KEYBOARD SCALING LEVEL                       */ = dx100CtrlDataAllVoice[topNum+15];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_19]/* OP2 KEYBOARD SCALING RATE                        */ = (dx100CtrlDataAllVoice[topNum+19])>>3; /* BIT4-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_20]/* OP2 EG BIAS SENSITIVITY                          */ = ((dx100CtrlDataAllVoice[topNum+16])>>3)&0x7; /* BIT5-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_21]/* OP2 AMPLITUDE MODULATION ENABLE                  */ = ((dx100CtrlDataAllVoice[topNum+16])>>6)&0x1; /* BIT6 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_22]/* OP2 KEY VELOCITY                                 */ = ((dx100CtrlDataAllVoice[topNum+16])   )&0x7; /* BIT2-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_23]/* OP2 OUTPUT LEVEL                                 */ = dx100CtrlDataAllVoice[topNum+17];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_24]/* OP2 OSCILLATOR FREQUENCY                         */ = dx100CtrlDataAllVoice[topNum+18];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_25]/* OP2 DETUNE                                       */ = (dx100CtrlDataAllVoice[topNum+19])&0x7; /* BIT2-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_26]/* OP3 ATTACK RATE                                  */ = dx100CtrlDataAllVoice[topNum+20];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_27]/* OP3 DECAY 1 RATE                                 */ = dx100CtrlDataAllVoice[topNum+21];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_28]/* OP3 DECAY 2 RATE                                 */ = dx100CtrlDataAllVoice[topNum+22];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_29]/* OP3 RELEASE RATE                                 */ = dx100CtrlDataAllVoice[topNum+23];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_30]/* OP3 DECAY 1 LEVEL                                */ = dx100CtrlDataAllVoice[topNum+24];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_31]/* OP3 KEYBOARD SCALING LEVEL                       */ = dx100CtrlDataAllVoice[topNum+25];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_32]/* OP3 KEYBOARD SCALING RATE                        */ = (dx100CtrlDataAllVoice[topNum+29])>>3; /* BIT4-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_33]/* OP3 EG BIAS SENSITIVITY                          */ = ((dx100CtrlDataAllVoice[topNum+26])>>3)&0x7; /* BIT5-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_34]/* OP3 AMPLITUDE MODULATION ENABLE                  */ = ((dx100CtrlDataAllVoice[topNum+26])>>6)&0x1; /* BIT6   */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_35]/* OP3 KEY VELOCITY                                 */ = ((dx100CtrlDataAllVoice[topNum+26])   )&0x7; /* BIT2-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_36]/* OP3 OUTPUT LEVEL                                 */ = dx100CtrlDataAllVoice[topNum+27];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_37]/* OP3 OSCILLATOR FREQUENCY                         */ = dx100CtrlDataAllVoice[topNum+28];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_38]/* OP3 DETUNE                                       */ = (dx100CtrlDataAllVoice[topNum+29])&0x7; /*BIT2-0*/
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_39]/* OP1 ATTACK RATE                                  */ = dx100CtrlDataAllVoice[topNum+30];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_40]/* OP1 DECAY 1 RATE                                 */ = dx100CtrlDataAllVoice[topNum+31];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_41]/* OP1 DECAY 2 RATE                                 */ = dx100CtrlDataAllVoice[topNum+32];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_42]/* OP1 RELEASE RATE                                 */ = dx100CtrlDataAllVoice[topNum+33];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_43]/* OP1 DECAY 1 LEVEL                                */ = dx100CtrlDataAllVoice[topNum+34];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_44]/* OP1 KEYBOARD SCALING LEVEL                       */ = dx100CtrlDataAllVoice[topNum+35];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_45]/* OP1 KEYBOARD SCALING RATE                        */ = (dx100CtrlDataAllVoice[topNum+39])>>3; /* BIT4-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_46]/* OP1 EG BIAS SENSITIVITY                          */ = ((dx100CtrlDataAllVoice[topNum+36])>>3)&0x7; /* BIT5-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_47]/* OP1 AMPLITUDE MODULATION ENABLE                  */ = ((dx100CtrlDataAllVoice[topNum+36])>>6)&0x1; /* BIT6   */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_48]/* OP1 KEY VELOCITY                                 */ = ((dx100CtrlDataAllVoice[topNum+36])   )&0x7; /* BIT2-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_49]/* OP1 OUTPUT LEVEL                                 */ = dx100CtrlDataAllVoice[topNum+37];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_50]/* OP1 OSCILLATOR FREQUENCY                         */ = dx100CtrlDataAllVoice[topNum+38];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_51]/* OP1 DETUNE                                       */ = (dx100CtrlDataAllVoice[topNum+39])&0x7; /* BIT2-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_52]/* ALGORITHM                                        */ = (dx100CtrlDataAllVoice[topNum+40])&0x7; /* BIT2-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_53]/* FEEDBACK LEVEL                                   */ = ((dx100CtrlDataAllVoice[topNum+40])>>3)&0x7; /* BIT5-3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_54]/* LFO SPEED                                        */ = dx100CtrlDataAllVoice[topNum+41];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_55]/* LFO DELAY                                        */ = dx100CtrlDataAllVoice[topNum+42];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_56]/* PITCH MODULATION DEPTH                           */ = dx100CtrlDataAllVoice[topNum+43];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_57]/* amplitude MODULATION depth                       */ = dx100CtrlDataAllVoice[topNum+44];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_58]/* LFO SYNC                                         */ = (dx100CtrlDataAllVoice[topNum+40])>>6; /* BIT7-6 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_59]/* lfo wave                                         */ = (dx100CtrlDataAllVoice[topNum+45])&0x3; /* BIT1-0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_60]/* pitch modulation sensitivity                     */ = ((dx100CtrlDataAllVoice[topNum+45])>>4)&0x7; /* BIT6-4 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_61]/* amplitude modulation sensitivity                 */ = ((dx100CtrlDataAllVoice[topNum+45])>>2)&0x3; /* BIT3-2 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_62]/* transpose                                        */ = dx100CtrlDataAllVoice[topNum+46];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_63]/* play mode poly/mono                              */ = ((dx100CtrlDataAllVoice[topNum+48])>>3)&0x1; /* BIT3 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_64]/* pitch bend range                                 */ = dx100CtrlDataAllVoice[topNum+47];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_65]/* portamento mode                                  */ = (dx100CtrlDataAllVoice[topNum+48])&0x1; /* BIT0 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_66]/* portamento time                                  */ = dx100CtrlDataAllVoice[topNum+49];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_67]/* foot volume range                                */ = dx100CtrlDataAllVoice[topNum+50];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_68]/* sustain foot switch                              */ = ((dx100CtrlDataAllVoice[topNum+48])>>2)&0x1; /* BIT2 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_69]/* portamento foot switch                           */ = ((dx100CtrlDataAllVoice[topNum+48])>>1)&0x1; /* BIT1 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_70]/* chorus switch                                    */ = ((dx100CtrlDataAllVoice[topNum+48])>>4)&0x1; /* BIT4 */
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_71]/* modulation wheel pitch modulation range          */ = dx100CtrlDataAllVoice[topNum+51];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_72]/* modulation wheel amplitude modulation range      */ = dx100CtrlDataAllVoice[topNum+52];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_73]/* breath control pitch modulation range            */ = dx100CtrlDataAllVoice[topNum+53];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_74]/* breath control amplitude modulation range        */ = dx100CtrlDataAllVoice[topNum+54];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_75]/* breath control pitch bias range                  */ = dx100CtrlDataAllVoice[topNum+55];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_76]/* breath control eg bias range                     */ = dx100CtrlDataAllVoice[topNum+56];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_77]/* voice name 1                                     */ = dx100CtrlDataAllVoice[topNum+57];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_78]/* voice name 2                                     */ = dx100CtrlDataAllVoice[topNum+58];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_79]/* voice name 3                                     */ = dx100CtrlDataAllVoice[topNum+59];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_80]/* voice name 4                                     */ = dx100CtrlDataAllVoice[topNum+60];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_81]/* voice name 5                                     */ = dx100CtrlDataAllVoice[topNum+61];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_82]/* voice name 6                                     */ = dx100CtrlDataAllVoice[topNum+62];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_83]/* voice name 7                                     */ = dx100CtrlDataAllVoice[topNum+63];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_84]/* voice name 8                                     */ = dx100CtrlDataAllVoice[topNum+64];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_85]/* voice name 9                                     */ = dx100CtrlDataAllVoice[topNum+65];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_86]/* voice name 10                                    */ = dx100CtrlDataAllVoice[topNum+66];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_87]/* pitch eg rate 1                                  */ = dx100CtrlDataAllVoice[topNum+67];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_88]/* pitch eg rate 2                                  */ = dx100CtrlDataAllVoice[topNum+68];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_89]/* pitch eg rate 3                                  */ = dx100CtrlDataAllVoice[topNum+69];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_90]/* pitch eg level 1                                 */ = dx100CtrlDataAllVoice[topNum+70];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_91]/* pitch eg level 2                                 */ = dx100CtrlDataAllVoice[topNum+71];
        dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_92]/* pitch eg level 3                                 */ = dx100CtrlDataAllVoice[topNum+72];

        copyToParamCtrl(DX100_CTRL_SEQ_1VOICE);
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
dx100voiceCopyFromOneToOneOfAll( INT voiceNum )
{
    if( voiceNum < 32 )
    {
        INT topNum = DX100_SYSEX_ALL_VOICE_DATA + (voiceNum*DX100_SYSEX_VMEM_MAX);

        dx100CtrlDataAllVoice[topNum+ 0] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_00];/* OP4 ATTACK RATE                                  */
        dx100CtrlDataAllVoice[topNum+ 1] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_01];/* OP4 DECAY 1 RATE                                 */
        dx100CtrlDataAllVoice[topNum+ 2] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_02];/* OP4 DECAY 2 RATE                                 */
        dx100CtrlDataAllVoice[topNum+ 3] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_03];/* OP4 RELEASE RATE                                 */
        dx100CtrlDataAllVoice[topNum+ 4] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_04];/* OP4 DECAY 1 LEVEL                                */
        dx100CtrlDataAllVoice[topNum+ 5] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_05];/* OP4 KEYBOARD SCALING LEVEL                       */
        dx100CtrlDataAllVoice[topNum+ 6] = ((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_08]&0x1)<<6)|((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_07]&0x7)<<3)|(dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_09]&0x7);/* OP4 AMPLITUDE MODULATION ENABLE,EG BIAS SENSITIVITY,KEY VELOCITY */
        dx100CtrlDataAllVoice[topNum+ 7] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_10];/* OP4 OUTPUT LEVEL                                 */
        dx100CtrlDataAllVoice[topNum+ 8] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_11];/* OP4 OSCILLATOR FREQUENCY                         */
        dx100CtrlDataAllVoice[topNum+ 9] = ((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_06])<<3)|(dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_12]&0x7);/* OP4 KEYBOARD SCALING RATE,DETUNE                 */
        dx100CtrlDataAllVoice[topNum+10] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_13];/* OP2 ATTACK RATE                                  */
        dx100CtrlDataAllVoice[topNum+11] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_14];/* OP2 DECAY 1 RATE                                 */
        dx100CtrlDataAllVoice[topNum+12] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_15];/* OP2 DECAY 2 RATE                                 */
        dx100CtrlDataAllVoice[topNum+13] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_16];/* OP2 RELEASE RATE                                 */
        dx100CtrlDataAllVoice[topNum+14] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_17];/* OP2 DECAY 1 LEVEL                                */
        dx100CtrlDataAllVoice[topNum+15] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_18];/* OP2 KEYBOARD SCALING LEVEL                       */
        dx100CtrlDataAllVoice[topNum+16] = ((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_21]&0x1)<<6)|((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_20]&0x7)<<3)|(dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_22]&0x7);/* OP2 AMPLITUDE MODULATION ENABLE,EG BIAS SENSITIVITY,KEY VELOCITY */
        dx100CtrlDataAllVoice[topNum+17] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_23];/* OP2 OUTPUT LEVEL                                 */
        dx100CtrlDataAllVoice[topNum+18] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_24];/* OP2 OSCILLATOR FREQUENCY                         */
        dx100CtrlDataAllVoice[topNum+19] = ((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_19])<<3)|(dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_25]&0x7);/* OP2 KEYBOARD SCALING RATE,DETUNE                 */
        dx100CtrlDataAllVoice[topNum+20] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_26];/* OP3 ATTACK RATE                                  */
        dx100CtrlDataAllVoice[topNum+21] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_27];/* OP3 DECAY 1 RATE                                 */
        dx100CtrlDataAllVoice[topNum+22] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_28];/* OP3 DECAY 2 RATE                                 */
        dx100CtrlDataAllVoice[topNum+23] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_29];/* OP3 RELEASE RATE                                 */
        dx100CtrlDataAllVoice[topNum+24] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_30];/* OP3 DECAY 1 LEVEL                                */
        dx100CtrlDataAllVoice[topNum+25] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_31];/* OP3 KEYBOARD SCALING LEVEL                       */
        dx100CtrlDataAllVoice[topNum+26] = ((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_34]&0x1)<<6)|((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_33]&0x7)<<3)|(dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_35]&0x7);/* OP3 AMPLITUDE MODULATION ENABLE,EG BIAS SENSITIVITY,KEY VELOCITY */
        dx100CtrlDataAllVoice[topNum+27] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_36];/* OP3 OUTPUT LEVEL                                 */
        dx100CtrlDataAllVoice[topNum+28] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_37];/* OP3 OSCILLATOR FREQUENCY                         */
        dx100CtrlDataAllVoice[topNum+29] = ((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_32])<<3)|(dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_38]&0x7);/* OP4 KEYBOARD SCALING RATE,DETUNE                 */
        dx100CtrlDataAllVoice[topNum+30] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_39];/* OP1 ATTACK RATE                                  */
        dx100CtrlDataAllVoice[topNum+31] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_40];/* OP1 DECAY 1 RATE                                 */
        dx100CtrlDataAllVoice[topNum+32] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_41];/* OP1 DECAY 2 RATE                                 */
        dx100CtrlDataAllVoice[topNum+33] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_42];/* OP1 RELEASE RATE                                 */
        dx100CtrlDataAllVoice[topNum+34] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_43];/* OP1 DECAY 1 LEVEL                                */
        dx100CtrlDataAllVoice[topNum+35] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_44];/* OP1 KEYBOARD SCALING LEVEL                       */
        dx100CtrlDataAllVoice[topNum+36] = ((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_47]&0x1)<<6)|((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_46]&0x7)<<3)|(dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_48]&0x7);/* OP1 AMPLITUDE MODULATION ENABLE,EG BIAS SENSITIVITY,KEY VELOCITY */
        dx100CtrlDataAllVoice[topNum+37] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_49];/* OP1 OUTPUT LEVEL                                 */
        dx100CtrlDataAllVoice[topNum+38] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_50];/* OP1 OSCILLATOR FREQUENCY                         */
        dx100CtrlDataAllVoice[topNum+39] = ((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_45])<<3)|(dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_51]&0x7);/* OP1 KEYBOARD SCALING RATE,DETUNE                 */
        dx100CtrlDataAllVoice[topNum+40] = ((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_58]&0x3)<<6)|((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_53]&0x7)<<3)|(dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_52]&0x7);/*LFO SYNC,FEEDBACK LEVEL,ALGORITHM */
        dx100CtrlDataAllVoice[topNum+41] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_54];/* LFO SPEED                                        */
        dx100CtrlDataAllVoice[topNum+42] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_55];/* LFO DELAY                                        */
        dx100CtrlDataAllVoice[topNum+43] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_56];/* PITCH MODULATION DEPTH                           */
        dx100CtrlDataAllVoice[topNum+44] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_57];/* amplitude MODULATION depth                       */
        dx100CtrlDataAllVoice[topNum+45] = ((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_60]&0x7)<<4)|((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_61]&0x3)<<2)|(dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_59]&0x3);/* pitch modulation sensitivity,amplitude modulation sensitivity,lfo wave*/
        dx100CtrlDataAllVoice[topNum+46] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_62];/* transpose                                        */
        dx100CtrlDataAllVoice[topNum+47] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_64];/* pitch bend range                                 */
        dx100CtrlDataAllVoice[topNum+48] = ((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_70]&0x1)<<4)|((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_63]&0x1)<<3)|((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_68]&0x1)<<2)|((dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_69]&0x1)<<1)|(dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_65]&0x1);/* chorus switch,play mode poly/mono,sustain foot switch,portamento foot switch,portamento mode*/
        dx100CtrlDataAllVoice[topNum+49] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_66];/* portamento time                                  */
        dx100CtrlDataAllVoice[topNum+50] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_67];/* foot volume range                                */
        dx100CtrlDataAllVoice[topNum+51] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_71];/* modulation wheel pitch modulation range          */
        dx100CtrlDataAllVoice[topNum+52] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_72];/* modulation wheel amplitude modulation range      */
        dx100CtrlDataAllVoice[topNum+53] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_73];/* breath control pitch modulation range            */
        dx100CtrlDataAllVoice[topNum+54] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_74];/* breath control amplitude modulation range        */
        dx100CtrlDataAllVoice[topNum+55] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_75];/* breath control pitch bias range                  */
        dx100CtrlDataAllVoice[topNum+56] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_76];/* breath control eg bias range                     */
        dx100CtrlDataAllVoice[topNum+57] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_77];/* voice name 1                                     */
        dx100CtrlDataAllVoice[topNum+58] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_78];/* voice name 2                                     */
        dx100CtrlDataAllVoice[topNum+59] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_79];/* voice name 3                                     */
        dx100CtrlDataAllVoice[topNum+60] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_80];/* voice name 4                                     */
        dx100CtrlDataAllVoice[topNum+61] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_81];/* voice name 5                                     */
        dx100CtrlDataAllVoice[topNum+62] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_82];/* voice name 6                                     */
        dx100CtrlDataAllVoice[topNum+63] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_83];/* voice name 7                                     */
        dx100CtrlDataAllVoice[topNum+64] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_84];/* voice name 8                                     */
        dx100CtrlDataAllVoice[topNum+65] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_85];/* voice name 9                                     */
        dx100CtrlDataAllVoice[topNum+66] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_86];/* voice name 10                                    */
        dx100CtrlDataAllVoice[topNum+67] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_87];/* pitch eg rate 1                                  */
        dx100CtrlDataAllVoice[topNum+68] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_88];/* pitch eg rate 2                                  */
        dx100CtrlDataAllVoice[topNum+69] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_89];/* pitch eg rate 3                                  */
        dx100CtrlDataAllVoice[topNum+70] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_90];/* pitch eg level 1                                 */
        dx100CtrlDataAllVoice[topNum+71] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_91];/* pitch eg level 2                                 */
        dx100CtrlDataAllVoice[topNum+72] = dx100CtrlDataOneVoice[DX100_SYSEX_1VOICE_DATA_92];/* pitch eg level 3                                 */

        copyToParamCtrl(DX100_CTRL_SEQ_ALL_VOICE);
    }
    else
    {
        nop();
    }
}

//#define DX100_CTRL_DEBUG_DATA_DISP
/*********************************************
 * 内容   : 
 * 引数   : DX100_CTRL_SEQ_ID seqId
 * 戻り値 : なし
 **********************************************/
static void
debugDataArrayPrint( INT rxDataSize, BYTE *rxDataPtr, PTSTR ptstrTitle )
{
    INT i;

#ifdef DX100_CTRL_DEBUG_DATA_DISP
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
