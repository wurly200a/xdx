#ifndef DX100_CTRL_DEF_H
#define DX100_CTRL_DEF_H

#define EX_STATUS           0xF0
#define EX_ID_NUMBER_YAMAHA 0x43
#define EX_ETX              0xF7

#define EX_BYTE_CNT_SIZE    2
#define EX_CHECKSUM_SIZE    1
#define EX_ETX_SIZE         1

typedef INT DX100_SYSEX_INDEX;

/* ダンプリクエスト */
enum
{
    DX100_SYSEX_BULK_DUMP_REQUEST_STATUS    ,
    DX100_SYSEX_BULK_DUMP_REQUEST_ID_NO     ,
    DX100_SYSEX_BULK_DUMP_REQUEST_SUB_STATUS,
    DX100_SYSEX_BULK_DUMP_REQUEST_FORMAT_NO ,
    DX100_SYSEX_BULK_DUMP_REQUEST_EOX       ,
    DX100_SYSEX_BULK_DUMP_REQUEST_INDEX_MAX 
};

#define DX100_DUMP_REQ_FORMAT_1VOICE  0x03
#define DX100_DUMP_REQ_FORMAT_32VOICE 0x04

/* 1音色バルクデータ */
enum
{
    DX100_SYSEX_1VOICE_HEADER_REQUEST_STATUS    ,                                                    /*   0  0x00 */
    DX100_SYSEX_1VOICE_HEADER_REQUEST_ID_NO     ,                                                    /*   1  0x01 */
    DX100_SYSEX_1VOICE_HEADER_REQUEST_SUB_STATUS,                                                    /*   2  0x02 */
    DX100_SYSEX_1VOICE_HEADER_REQUEST_FORMAT_NO ,                                                    /*   3  0x03 */
    DX100_SYSEX_1VOICE_HEADER_BYTE_COUNT_HIGH   ,                                                    /*   4  0x04 */
    DX100_SYSEX_1VOICE_HEADER_BYTE_COUNT_LOW    ,                                                    /*   5  0x05 */
    DX100_SYSEX_1VOICE_DATA_00                  ,DX100_SYSEX_1VOICE_DATA=DX100_SYSEX_1VOICE_DATA_00, /*   6  0x06 (0x00)  */
    DX100_SYSEX_1VOICE_DATA_01                  ,                                                    /*   7  0x07 (0x01)  */
    DX100_SYSEX_1VOICE_DATA_02                  ,                                                    /*   8  0x08 (0x02)  */
    DX100_SYSEX_1VOICE_DATA_03                  ,                                                    /*   9  0x09 (0x03)  */
    DX100_SYSEX_1VOICE_DATA_04                  ,                                                    /*  10  0x0A (0x04)  */
    DX100_SYSEX_1VOICE_DATA_05                  ,                                                    /*  11  0x0B (0x05)  */
    DX100_SYSEX_1VOICE_DATA_06                  ,                                                    /*  12  0x0C (0x06)  */
    DX100_SYSEX_1VOICE_DATA_07                  ,                                                    /*  13  0x0D (0x07)  */
    DX100_SYSEX_1VOICE_DATA_08                  ,                                                    /*  14  0x0E (0x08)  */
    DX100_SYSEX_1VOICE_DATA_09                  ,                                                    /*  15  0x0F (0x09)  */
    DX100_SYSEX_1VOICE_DATA_10                  ,                                                    /*  16  0x10 (0x0A)  */
    DX100_SYSEX_1VOICE_DATA_11                  ,                                                    /*  17  0x11 (0x0B)  */
    DX100_SYSEX_1VOICE_DATA_12                  ,                                                    /*  18  0x12 (0x0C)  */
    DX100_SYSEX_1VOICE_DATA_13                  ,                                                    /*  19  0x13 (0x0D)  */
    DX100_SYSEX_1VOICE_DATA_14                  ,                                                    /*  20  0x14 (0x0E)  */
    DX100_SYSEX_1VOICE_DATA_15                  ,                                                    /*  21  0x15 (0x0F)  */
    DX100_SYSEX_1VOICE_DATA_16                  ,                                                    /*  22  0x16 (0x10)  */
    DX100_SYSEX_1VOICE_DATA_17                  ,                                                    /*  23  0x17 (0x11)  */
    DX100_SYSEX_1VOICE_DATA_18                  ,                                                    /*  24  0x18 (0x12)  */
    DX100_SYSEX_1VOICE_DATA_19                  ,                                                    /*  25  0x19 (0x13)  */
    DX100_SYSEX_1VOICE_DATA_20                  ,                                                    /*  26  0x1A (0x14)  */
    DX100_SYSEX_1VOICE_DATA_21                  ,                                                    /*  27  0x1B (0x15)  */
    DX100_SYSEX_1VOICE_DATA_22                  ,                                                    /*  28  0x1C (0x16)  */
    DX100_SYSEX_1VOICE_DATA_23                  ,                                                    /*  29  0x1D (0x17)  */
    DX100_SYSEX_1VOICE_DATA_24                  ,                                                    /*  30  0x1E (0x18)  */
    DX100_SYSEX_1VOICE_DATA_25                  ,                                                    /*  31  0x1F (0x19)  */
    DX100_SYSEX_1VOICE_DATA_26                  ,                                                    /*  32  0x20 (0x1A)  */
    DX100_SYSEX_1VOICE_DATA_27                  ,                                                    /*  33  0x21 (0x1B)  */
    DX100_SYSEX_1VOICE_DATA_28                  ,                                                    /*  34  0x22 (0x1C)  */
    DX100_SYSEX_1VOICE_DATA_29                  ,                                                    /*  35  0x23 (0x1D)  */
    DX100_SYSEX_1VOICE_DATA_30                  ,                                                    /*  36  0x24 (0x1E)  */
    DX100_SYSEX_1VOICE_DATA_31                  ,                                                    /*  37  0x25 (0x1F)  */
    DX100_SYSEX_1VOICE_DATA_32                  ,                                                    /*  38  0x26 (0x20)  */
    DX100_SYSEX_1VOICE_DATA_33                  ,                                                    /*  39  0x27 (0x21)  */
    DX100_SYSEX_1VOICE_DATA_34                  ,                                                    /*  40  0x28 (0x22)  */
    DX100_SYSEX_1VOICE_DATA_35                  ,                                                    /*  41  0x29 (0x23)  */
    DX100_SYSEX_1VOICE_DATA_36                  ,                                                    /*  42  0x2A (0x24)  */
    DX100_SYSEX_1VOICE_DATA_37                  ,                                                    /*  43  0x2B (0x25)  */
    DX100_SYSEX_1VOICE_DATA_38                  ,                                                    /*  44  0x2C (0x26)  */
    DX100_SYSEX_1VOICE_DATA_39                  ,                                                    /*  45  0x2D (0x27)  */
    DX100_SYSEX_1VOICE_DATA_40                  ,                                                    /*  46  0x2E (0x28)  */
    DX100_SYSEX_1VOICE_DATA_41                  ,                                                    /*  47  0x2F (0x29)  */
    DX100_SYSEX_1VOICE_DATA_42                  ,                                                    /*  48  0x30 (0x2A)  */
    DX100_SYSEX_1VOICE_DATA_43                  ,                                                    /*  49  0x31 (0x2B)  */
    DX100_SYSEX_1VOICE_DATA_44                  ,                                                    /*  50  0x32 (0x2C)  */
    DX100_SYSEX_1VOICE_DATA_45                  ,                                                    /*  51  0x33 (0x2D)  */
    DX100_SYSEX_1VOICE_DATA_46                  ,                                                    /*  52  0x34 (0x2E)  */
    DX100_SYSEX_1VOICE_DATA_47                  ,                                                    /*  53  0x35 (0x2F)  */
    DX100_SYSEX_1VOICE_DATA_48                  ,                                                    /*  54  0x36 (0x30)  */
    DX100_SYSEX_1VOICE_DATA_49                  ,                                                    /*  55  0x37 (0x31)  */
    DX100_SYSEX_1VOICE_DATA_50                  ,                                                    /*  56  0x38 (0x32)  */
    DX100_SYSEX_1VOICE_DATA_51                  ,                                                    /*  57  0x39 (0x33)  */
    DX100_SYSEX_1VOICE_DATA_52                  ,                                                    /*  58  0x3A (0x34)  */
    DX100_SYSEX_1VOICE_DATA_53                  ,                                                    /*  59  0x3B (0x35)  */
    DX100_SYSEX_1VOICE_DATA_54                  ,                                                    /*  60  0x3C (0x36)  */
    DX100_SYSEX_1VOICE_DATA_55                  ,                                                    /*  61  0x3D (0x37)  */
    DX100_SYSEX_1VOICE_DATA_56                  ,                                                    /*  62  0x3E (0x38)  */
    DX100_SYSEX_1VOICE_DATA_57                  ,                                                    /*  63  0x3F (0x39)  */
    DX100_SYSEX_1VOICE_DATA_58                  ,                                                    /*  64  0x40 (0x3A)  */
    DX100_SYSEX_1VOICE_DATA_59                  ,                                                    /*  65  0x41 (0x3B)  */
    DX100_SYSEX_1VOICE_DATA_60                  ,                                                    /*  66  0x42 (0x3C)  */
    DX100_SYSEX_1VOICE_DATA_61                  ,                                                    /*  67  0x43 (0x3D)  */
    DX100_SYSEX_1VOICE_DATA_62                  ,                                                    /*  68  0x44 (0x3E)  */
    DX100_SYSEX_1VOICE_DATA_63                  ,                                                    /*  69  0x45 (0x3F)  */
    DX100_SYSEX_1VOICE_DATA_64                  ,                                                    /*  70  0x46 (0x40)  */
    DX100_SYSEX_1VOICE_DATA_65                  ,                                                    /*  71  0x47 (0x41)  */
    DX100_SYSEX_1VOICE_DATA_66                  ,                                                    /*  72  0x48 (0x42)  */
    DX100_SYSEX_1VOICE_DATA_67                  ,                                                    /*  73  0x49 (0x43)  */
    DX100_SYSEX_1VOICE_DATA_68                  ,                                                    /*  74  0x4A (0x44)  */
    DX100_SYSEX_1VOICE_DATA_69                  ,                                                    /*  75  0x4B (0x45)  */
    DX100_SYSEX_1VOICE_DATA_70                  ,                                                    /*  76  0x4C (0x46)  */
    DX100_SYSEX_1VOICE_DATA_71                  ,                                                    /*  77  0x4D (0x47)  */
    DX100_SYSEX_1VOICE_DATA_72                  ,                                                    /*  78  0x4E (0x48)  */
    DX100_SYSEX_1VOICE_DATA_73                  ,                                                    /*  79  0x4F (0x49)  */
    DX100_SYSEX_1VOICE_DATA_74                  ,                                                    /*  80  0x50 (0x4A)  */
    DX100_SYSEX_1VOICE_DATA_75                  ,                                                    /*  81  0x51 (0x4B)  */
    DX100_SYSEX_1VOICE_DATA_76                  ,                                                    /*  82  0x52 (0x4C)  */
    DX100_SYSEX_1VOICE_DATA_77                  ,                                                    /*  83  0x53 (0x4D)  */
    DX100_SYSEX_1VOICE_DATA_78                  ,                                                    /*  84  0x54 (0x4E)  */
    DX100_SYSEX_1VOICE_DATA_79                  ,                                                    /*  85  0x55 (0x4F)  */
    DX100_SYSEX_1VOICE_DATA_80                  ,                                                    /*  86  0x56 (0x50)  */
    DX100_SYSEX_1VOICE_DATA_81                  ,                                                    /*  87  0x57 (0x51)  */
    DX100_SYSEX_1VOICE_DATA_82                  ,                                                    /*  88  0x58 (0x52)  */
    DX100_SYSEX_1VOICE_DATA_83                  ,                                                    /*  89  0x59 (0x53)  */
    DX100_SYSEX_1VOICE_DATA_84                  ,                                                    /*  90  0x5A (0x54)  */
    DX100_SYSEX_1VOICE_DATA_85                  ,                                                    /*  91  0x5B (0x55)  */
    DX100_SYSEX_1VOICE_DATA_86                  ,                                                    /*  92  0x5C (0x56)  */
    DX100_SYSEX_1VOICE_DATA_87                  ,                                                    /*  93  0x5D (0x57)  */
    DX100_SYSEX_1VOICE_DATA_88                  ,                                                    /*  94  0x5E (0x58)  */
    DX100_SYSEX_1VOICE_DATA_89                  ,                                                    /*  95  0x5F (0x59)  */
    DX100_SYSEX_1VOICE_DATA_90                  ,                                                    /*  96  0x60 (0x5A)  */
    DX100_SYSEX_1VOICE_DATA_91                  ,                                                    /*  97  0x61 (0x5B)  */
    DX100_SYSEX_1VOICE_DATA_92                  ,                                                    /*  98  0x62 (0x5C)  */
    DX100_SYSEX_1VOICE_FOOTER_CHECKSUM          =99,                                                 /*  99  0x63         */
    DX100_SYSEX_1VOICE_FOOTER_ETX               ,                                                    /* 100  0x64         */
    DX100_SYSEX_1VOICE_INDEX_MAX
};

/* 32音色バルクデータ */
enum
{
    DX100_SYSEX_ALL_VOICE_HEADER_REQUEST_STATUS    ,   /*   0  0x00 */
    DX100_SYSEX_ALL_VOICE_HEADER_REQUEST_ID_NO     ,   /*   1  0x01 */
    DX100_SYSEX_ALL_VOICE_HEADER_REQUEST_SUB_STATUS,   /*   2  0x02 */
    DX100_SYSEX_ALL_VOICE_HEADER_REQUEST_FORMAT_NO ,   /*   3  0x03 */
    DX100_SYSEX_ALL_VOICE_HEADER_BYTE_COUNT_HIGH   ,   /*   4  0x04 */
    DX100_SYSEX_ALL_VOICE_HEADER_BYTE_COUNT_LOW    ,   /*   5  0x05 */
    DX100_SYSEX_ALL_VOICE_DATA                     ,   /*   6  0x06 (0x00)  */
    DX100_SYSEX_ALL_VOICE_FOOTER_CHECKSUM          =6+4096,
    DX100_SYSEX_ALL_VOICE_FOOTER_ETX               ,
    DX100_SYSEX_ALL_VOICE_INDEX_MAX
};

/* VCEDフォーマット */
enum
{
    DX100_SYSEX_VCED_00  ,/*   0 0x00 */
    DX100_SYSEX_VCED_01  ,/*   1 0x01 */
    DX100_SYSEX_VCED_02  ,/*   2 0x02 */
    DX100_SYSEX_VCED_03  ,/*   3 0x03 */
    DX100_SYSEX_VCED_04  ,/*   4 0x04 */
    DX100_SYSEX_VCED_05  ,/*   5 0x05 */
    DX100_SYSEX_VCED_06  ,/*   6 0x06 */
    DX100_SYSEX_VCED_07  ,/*   7 0x07 */
    DX100_SYSEX_VCED_08  ,/*   8 0x08 */
    DX100_SYSEX_VCED_09  ,/*   9 0x09 */
    DX100_SYSEX_VCED_10  ,/*  10 0x0A */
    DX100_SYSEX_VCED_11  ,/*  11 0x0B */
    DX100_SYSEX_VCED_12  ,/*  12 0x0C */
    DX100_SYSEX_VCED_13  ,/*  13 0x0D */
    DX100_SYSEX_VCED_14  ,/*  14 0x0E */
    DX100_SYSEX_VCED_15  ,/*  15 0x0F */
    DX100_SYSEX_VCED_16  ,/*  16 0x10 */
    DX100_SYSEX_VCED_17  ,/*  17 0x11 */
    DX100_SYSEX_VCED_18  ,/*  18 0x12 */
    DX100_SYSEX_VCED_19  ,/*  19 0x13 */
    DX100_SYSEX_VCED_20  ,/*  20 0x14 */
    DX100_SYSEX_VCED_21  ,/*  21 0x15 */
    DX100_SYSEX_VCED_22  ,/*  22 0x16 */
    DX100_SYSEX_VCED_23  ,/*  23 0x17 */
    DX100_SYSEX_VCED_24  ,/*  24 0x18 */
    DX100_SYSEX_VCED_25  ,/*  25 0x19 */
    DX100_SYSEX_VCED_26  ,/*  26 0x1A */
    DX100_SYSEX_VCED_27  ,/*  27 0x1B */
    DX100_SYSEX_VCED_28  ,/*  28 0x1C */
    DX100_SYSEX_VCED_29  ,/*  29 0x1D */
    DX100_SYSEX_VCED_30  ,/*  30 0x1E */
    DX100_SYSEX_VCED_31  ,/*  31 0x1F */
    DX100_SYSEX_VCED_32  ,/*  32 0x20 */
    DX100_SYSEX_VCED_33  ,/*  33 0x21 */
    DX100_SYSEX_VCED_34  ,/*  34 0x22 */
    DX100_SYSEX_VCED_35  ,/*  35 0x23 */
    DX100_SYSEX_VCED_36  ,/*  36 0x24 */
    DX100_SYSEX_VCED_37  ,/*  37 0x25 */
    DX100_SYSEX_VCED_38  ,/*  38 0x26 */
    DX100_SYSEX_VCED_39  ,/*  39 0x27 */
    DX100_SYSEX_VCED_40  ,/*  40 0x28 */
    DX100_SYSEX_VCED_41  ,/*  41 0x29 */
    DX100_SYSEX_VCED_42  ,/*  42 0x2A */
    DX100_SYSEX_VCED_43  ,/*  43 0x2B */
    DX100_SYSEX_VCED_44  ,/*  44 0x2C */
    DX100_SYSEX_VCED_45  ,/*  45 0x2D */
    DX100_SYSEX_VCED_46  ,/*  46 0x2E */
    DX100_SYSEX_VCED_47  ,/*  47 0x2F */
    DX100_SYSEX_VCED_48  ,/*  48 0x30 */
    DX100_SYSEX_VCED_49  ,/*  49 0x31 */
    DX100_SYSEX_VCED_50  ,/*  50 0x32 */
    DX100_SYSEX_VCED_51  ,/*  51 0x33 */
    DX100_SYSEX_VCED_52  ,/*  52 0x34 */
    DX100_SYSEX_VCED_53  ,/*  53 0x35 */
    DX100_SYSEX_VCED_54  ,/*  54 0x36 */
    DX100_SYSEX_VCED_55  ,/*  55 0x37 */
    DX100_SYSEX_VCED_56  ,/*  56 0x38 */
    DX100_SYSEX_VCED_57  ,/*  57 0x39 */
    DX100_SYSEX_VCED_58  ,/*  58 0x3A */
    DX100_SYSEX_VCED_59  ,/*  59 0x3B */
    DX100_SYSEX_VCED_60  ,/*  60 0x3C */
    DX100_SYSEX_VCED_61  ,/*  61 0x3D */
    DX100_SYSEX_VCED_62  ,/*  62 0x3E */
    DX100_SYSEX_VCED_63  ,/*  63 0x3F */
    DX100_SYSEX_VCED_64  ,/*  64 0x40 */
    DX100_SYSEX_VCED_65  ,/*  65 0x41 */
    DX100_SYSEX_VCED_66  ,/*  66 0x42 */
    DX100_SYSEX_VCED_67  ,/*  67 0x43 */
    DX100_SYSEX_VCED_68  ,/*  68 0x44 */
    DX100_SYSEX_VCED_69  ,/*  69 0x45 */
    DX100_SYSEX_VCED_70  ,/*  70 0x46 */
    DX100_SYSEX_VCED_71  ,/*  71 0x47 */
    DX100_SYSEX_VCED_72  ,/*  72 0x48 */
    DX100_SYSEX_VCED_73  ,/*  73 0x49 */
    DX100_SYSEX_VCED_74  ,/*  74 0x4A */
    DX100_SYSEX_VCED_75  ,/*  75 0x4B */
    DX100_SYSEX_VCED_76  ,/*  76 0x4C */
    DX100_SYSEX_VCED_77  ,/*  77 0x4D */
    DX100_SYSEX_VCED_78  ,/*  78 0x4E */
    DX100_SYSEX_VCED_79  ,/*  79 0x4F */
    DX100_SYSEX_VCED_80  ,/*  80 0x50 */
    DX100_SYSEX_VCED_81  ,/*  81 0x51 */
    DX100_SYSEX_VCED_82  ,/*  82 0x52 */
    DX100_SYSEX_VCED_83  ,/*  83 0x53 */
    DX100_SYSEX_VCED_84  ,/*  84 0x54 */
    DX100_SYSEX_VCED_85  ,/*  85 0x55 */
    DX100_SYSEX_VCED_86  ,/*  86 0x56 */
    DX100_SYSEX_VCED_87  ,/*  87 0x57 */
    DX100_SYSEX_VCED_88  ,/*  88 0x58 */
    DX100_SYSEX_VCED_89  ,/*  89 0x59 */
    DX100_SYSEX_VCED_90  ,/*  90 0x5A */
    DX100_SYSEX_VCED_91  ,/*  91 0x5B */
    DX100_SYSEX_VCED_92  ,/*  92 0x5C */
    DX100_SYSEX_VCED_MAX
};

/* VMEMフォーマット */
enum
{
    DX100_SYSEX_VMEM_00  ,/*   0 0x00 */
    DX100_SYSEX_VMEM_01  ,/*   1 0x01 */
    DX100_SYSEX_VMEM_02  ,/*   2 0x02 */
    DX100_SYSEX_VMEM_03  ,/*   3 0x03 */
    DX100_SYSEX_VMEM_04  ,/*   4 0x04 */
    DX100_SYSEX_VMEM_05  ,/*   5 0x05 */
    DX100_SYSEX_VMEM_06  ,/*   6 0x06 */
    DX100_SYSEX_VMEM_07  ,/*   7 0x07 */
    DX100_SYSEX_VMEM_08  ,/*   8 0x08 */
    DX100_SYSEX_VMEM_09  ,/*   9 0x09 */
    DX100_SYSEX_VMEM_10  ,/*  10 0x0A */
    DX100_SYSEX_VMEM_11  ,/*  11 0x0B */
    DX100_SYSEX_VMEM_12  ,/*  12 0x0C */
    DX100_SYSEX_VMEM_13  ,/*  13 0x0D */
    DX100_SYSEX_VMEM_14  ,/*  14 0x0E */
    DX100_SYSEX_VMEM_15  ,/*  15 0x0F */
    DX100_SYSEX_VMEM_16  ,/*  16 0x10 */
    DX100_SYSEX_VMEM_17  ,/*  17 0x11 */
    DX100_SYSEX_VMEM_18  ,/*  18 0x12 */
    DX100_SYSEX_VMEM_19  ,/*  19 0x13 */
    DX100_SYSEX_VMEM_20  ,/*  20 0x14 */
    DX100_SYSEX_VMEM_21  ,/*  21 0x15 */
    DX100_SYSEX_VMEM_22  ,/*  22 0x16 */
    DX100_SYSEX_VMEM_23  ,/*  23 0x17 */
    DX100_SYSEX_VMEM_24  ,/*  24 0x18 */
    DX100_SYSEX_VMEM_25  ,/*  25 0x19 */
    DX100_SYSEX_VMEM_26  ,/*  26 0x1A */
    DX100_SYSEX_VMEM_27  ,/*  27 0x1B */
    DX100_SYSEX_VMEM_28  ,/*  28 0x1C */
    DX100_SYSEX_VMEM_29  ,/*  29 0x1D */
    DX100_SYSEX_VMEM_30  ,/*  30 0x1E */
    DX100_SYSEX_VMEM_31  ,/*  31 0x1F */
    DX100_SYSEX_VMEM_32  ,/*  32 0x20 */
    DX100_SYSEX_VMEM_33  ,/*  33 0x21 */
    DX100_SYSEX_VMEM_34  ,/*  34 0x22 */
    DX100_SYSEX_VMEM_35  ,/*  35 0x23 */
    DX100_SYSEX_VMEM_36  ,/*  36 0x24 */
    DX100_SYSEX_VMEM_37  ,/*  37 0x25 */
    DX100_SYSEX_VMEM_38  ,/*  38 0x26 */
    DX100_SYSEX_VMEM_39  ,/*  39 0x27 */
    DX100_SYSEX_VMEM_40  ,/*  40 0x28 */
    DX100_SYSEX_VMEM_41  ,/*  41 0x29 */
    DX100_SYSEX_VMEM_42  ,/*  42 0x2A */
    DX100_SYSEX_VMEM_43  ,/*  43 0x2B */
    DX100_SYSEX_VMEM_44  ,/*  44 0x2C */
    DX100_SYSEX_VMEM_45  ,/*  45 0x2D */
    DX100_SYSEX_VMEM_46  ,/*  46 0x2E */
    DX100_SYSEX_VMEM_47  ,/*  47 0x2F */
    DX100_SYSEX_VMEM_48  ,/*  48 0x30 */
    DX100_SYSEX_VMEM_49  ,/*  49 0x31 */
    DX100_SYSEX_VMEM_50  ,/*  50 0x32 */
    DX100_SYSEX_VMEM_51  ,/*  51 0x33 */
    DX100_SYSEX_VMEM_52  ,/*  52 0x34 */
    DX100_SYSEX_VMEM_53  ,/*  53 0x35 */
    DX100_SYSEX_VMEM_54  ,/*  54 0x36 */
    DX100_SYSEX_VMEM_55  ,/*  55 0x37 */
    DX100_SYSEX_VMEM_56  ,/*  56 0x38 */
    DX100_SYSEX_VMEM_57  ,/*  57 0x39 */
    DX100_SYSEX_VMEM_58  ,/*  58 0x3A */
    DX100_SYSEX_VMEM_59  ,/*  59 0x3B */
    DX100_SYSEX_VMEM_60  ,/*  60 0x3C */
    DX100_SYSEX_VMEM_61  ,/*  61 0x3D */
    DX100_SYSEX_VMEM_62  ,/*  62 0x3E */
    DX100_SYSEX_VMEM_63  ,/*  63 0x3F */
    DX100_SYSEX_VMEM_64  ,/*  64 0x40 */
    DX100_SYSEX_VMEM_65  ,/*  65 0x41 */
    DX100_SYSEX_VMEM_66  ,/*  66 0x42 */
    DX100_SYSEX_VMEM_67  ,/*  67 0x43 */
    DX100_SYSEX_VMEM_68  ,/*  68 0x44 */
    DX100_SYSEX_VMEM_69  ,/*  69 0x45 */
    DX100_SYSEX_VMEM_70  ,/*  70 0x46 */
    DX100_SYSEX_VMEM_71  ,/*  71 0x47 */
    DX100_SYSEX_VMEM_72  ,/*  72 0x48 */
    DX100_SYSEX_VMEM_73  ,/*  73 0x49 dummy */
    DX100_SYSEX_VMEM_74  ,/*  74 0x4A dummy */
    DX100_SYSEX_VMEM_75  ,/*  75 0x4B dummy */
    DX100_SYSEX_VMEM_76  ,/*  76 0x4C dummy */
    DX100_SYSEX_VMEM_77  ,/*  77 0x4D dummy */
    DX100_SYSEX_VMEM_78  ,/*  78 0x4E dummy */
    DX100_SYSEX_VMEM_79  ,/*  79 0x4F dummy */
    DX100_SYSEX_VMEM_80  ,/*  80 0x50 dummy */
    DX100_SYSEX_VMEM_81  ,/*  81 0x51 dummy */
    DX100_SYSEX_VMEM_82  ,/*  82 0x52 dummy */
    DX100_SYSEX_VMEM_83  ,/*  83 0x53 dummy */
    DX100_SYSEX_VMEM_84  ,/*  84 0x54 dummy */
    DX100_SYSEX_VMEM_85  ,/*  85 0x55 dummy */
    DX100_SYSEX_VMEM_86  ,/*  86 0x56 dummy */
    DX100_SYSEX_VMEM_87  ,/*  87 0x57 dummy */
    DX100_SYSEX_VMEM_88  ,/*  88 0x58 dummy */
    DX100_SYSEX_VMEM_89  ,/*  89 0x59 dummy */
    DX100_SYSEX_VMEM_90  ,/*  90 0x5A dummy */
    DX100_SYSEX_VMEM_91  ,/*  91 0x5B dummy */
    DX100_SYSEX_VMEM_92  ,/*  92 0x5C dummy */
    DX100_SYSEX_VMEM_93  ,/*  93 0x5D dummy */
    DX100_SYSEX_VMEM_94  ,/*  94 0x5E dummy */
    DX100_SYSEX_VMEM_95  ,/*  95 0x5F dummy */
    DX100_SYSEX_VMEM_96  ,/*  96 0x60 dummy */
    DX100_SYSEX_VMEM_97  ,/*  97 0x61 dummy */
    DX100_SYSEX_VMEM_98  ,/*  98 0x62 dummy */
    DX100_SYSEX_VMEM_99  ,/*  99 0x63 dummy */
    DX100_SYSEX_VMEM_100 ,/* 100 0x64 dummy */
    DX100_SYSEX_VMEM_101 ,/* 101 0x65 dummy */
    DX100_SYSEX_VMEM_102 ,/* 102 0x66 dummy */
    DX100_SYSEX_VMEM_103 ,/* 103 0x67 dummy */
    DX100_SYSEX_VMEM_104 ,/* 104 0x68 dummy */
    DX100_SYSEX_VMEM_105 ,/* 105 0x69 dummy */
    DX100_SYSEX_VMEM_106 ,/* 106 0x6A dummy */
    DX100_SYSEX_VMEM_107 ,/* 107 0x6B dummy */
    DX100_SYSEX_VMEM_108 ,/* 108 0x6C dummy */
    DX100_SYSEX_VMEM_109 ,/* 109 0x6D dummy */
    DX100_SYSEX_VMEM_110 ,/* 110 0x6E dummy */
    DX100_SYSEX_VMEM_111 ,/* 111 0x6F dummy */
    DX100_SYSEX_VMEM_112 ,/* 112 0x70 dummy */
    DX100_SYSEX_VMEM_113 ,/* 113 0x71 dummy */
    DX100_SYSEX_VMEM_114 ,/* 114 0x72 dummy */
    DX100_SYSEX_VMEM_115 ,/* 115 0x73 dummy */
    DX100_SYSEX_VMEM_116 ,/* 116 0x74 dummy */
    DX100_SYSEX_VMEM_117 ,/* 117 0x75 dummy */
    DX100_SYSEX_VMEM_118 ,/* 118 0x76 dummy */
    DX100_SYSEX_VMEM_119 ,/* 119 0x77 dummy */
    DX100_SYSEX_VMEM_120 ,/* 120 0x78 dummy */
    DX100_SYSEX_VMEM_121 ,/* 121 0x79 dummy */
    DX100_SYSEX_VMEM_122 ,/* 122 0x7A dummy */
    DX100_SYSEX_VMEM_123 ,/* 123 0x7B dummy */
    DX100_SYSEX_VMEM_124 ,/* 124 0x7C dummy */
    DX100_SYSEX_VMEM_125 ,/* 125 0x7D dummy */
    DX100_SYSEX_VMEM_126 ,/* 126 0x7E dummy */
    DX100_SYSEX_VMEM_127 ,/* 127 0x7F dummy */
    DX100_SYSEX_VMEM_MAX
};

#endif /* DX100_CTRL_DEF_H */
