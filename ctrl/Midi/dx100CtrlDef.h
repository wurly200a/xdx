#ifndef DX100_CTRL_DEF_H
#define DX100_CTRL_DEF_H

#define EX_STATUS           0xF0
#define EX_ID_NUMBER_YAMAHA 0x43
#define EX_ETX              0xF7

#define EX_BYTE_CNT_SIZE    2
#define EX_CHECKSUM_SIZE    1
#define EX_ETX_SIZE         1

enum
{
    MIDI_EX_HEADER_STATUS    ,
    MIDI_EX_HEADER_ID_NUMBER ,
    MIDI_EX_HEADER_SUB_STATUS,
    MIDI_EX_HEADER_PARAM     ,
    MIDI_EX_HEADER_DATA      ,
};
typedef INT MIDI_EX_HEADER_INDEX;

enum
{
    SYSCMN_SOUNDMODE                ,
    SYSCMN_INDEX_MAX
};
typedef INT SYSCMN_INDEX;

/* VOICE */
enum
{
    VOICE_00  ,/* 0x0000  */
    VOICE_01  ,/* 0x0001  */
    VOICE_02  ,/* 0x0002  */
    VOICE_03  ,/* 0x0003  */
    VOICE_04  ,/* 0x0004  */
    VOICE_05  ,/* 0x0005  */
    VOICE_06  ,/* 0x0006  */
    VOICE_07  ,/* 0x0007  */
    VOICE_08  ,/* 0x0008  */
    VOICE_09  ,/* 0x0009  */
    VOICE_10  ,/* 0x000A  */
    VOICE_11  ,/* 0x000B  */
    VOICE_12  ,/* 0x000C  */
    VOICE_13  ,/* 0x000D  */
    VOICE_14  ,/* 0x000E  */
    VOICE_15  ,/* 0x000F  */
    VOICE_16  ,/* 0x0010  */
    VOICE_17  ,/* 0x0011  */
    VOICE_18  ,/* 0x0012  */
    VOICE_19  ,/* 0x0013  */
    VOICE_20  ,/* 0x0014  */
    VOICE_21  ,/* 0x0015  */
    VOICE_22  ,/* 0x0016  */
    VOICE_23  ,/* 0x0017  */
    VOICE_24  ,/* 0x0018  */
    VOICE_25  ,/* 0x0019  */
    VOICE_26  ,/* 0x001A  */
    VOICE_27  ,/* 0x001B  */
    VOICE_28  ,/* 0x001C  */
    VOICE_29  ,/* 0x001D  */
    VOICE_30  ,/* 0x001E  */
    VOICE_31  ,/* 0x001F  */
    VOICE_32  ,/* 0x0020  */
    VOICE_33  ,/* 0x0021  */
    VOICE_34  ,/* 0x0022  */
    VOICE_35  ,/* 0x0023  */
    VOICE_36  ,/* 0x0024  */
    VOICE_37  ,/* 0x0025  */
    VOICE_38  ,/* 0x0026  */
    VOICE_39  ,/* 0x0027  */
    VOICE_40  ,/* 0x0028  */
    VOICE_41  ,/* 0x0029  */
    VOICE_42  ,/* 0x002A  */
    VOICE_43  ,/* 0x002B  */
    VOICE_44  ,/* 0x002C  */
    VOICE_45  ,/* 0x002D  */
    VOICE_46  ,/* 0x002E  */
    VOICE_47  ,/* 0x002F  */
    VOICE_48  ,/* 0x0030  */
    VOICE_49  ,/* 0x0031  */
    VOICE_50  ,/* 0x0032  */
    VOICE_51  ,/* 0x0033  */
    VOICE_52  ,/* 0x0034  */
    VOICE_53  ,/* 0x0035  */
    VOICE_54  ,/* 0x0036  */
    VOICE_55  ,/* 0x0037  */
    VOICE_56  ,/* 0x0038  */
    VOICE_57  ,/* 0x0039  */
    VOICE_58  ,/* 0x003A  */
    VOICE_59  ,/* 0x003B  */
    VOICE_60  ,/* 0x003C  */
    VOICE_61  ,/* 0x003D  */
    VOICE_62  ,/* 0x003E  */
    VOICE_63  ,/* 0x003F  */
    VOICE_64  ,/* 0x0040  */
    VOICE_65  ,/* 0x0041  */
    VOICE_66  ,/* 0x0042  */
    VOICE_67  ,/* 0x0043  */
    VOICE_68  ,/* 0x0044  */
    VOICE_69  ,/* 0x0045  */
    VOICE_70  ,/* 0x0046  */
    VOICE_71  ,/* 0x0047  */
    VOICE_72  ,/* 0x0048  */
    VOICE_73  ,/* 0x0049  */
    VOICE_74  ,/* */
    VOICE_75  ,/* */
    VOICE_76  ,/* */
    VOICE_77  ,/* */
    VOICE_78  ,/* */
    VOICE_79  ,/* */
    VOICE_80  ,/* */
    VOICE_81  ,/* */
    VOICE_82  ,/* */
    VOICE_83  ,/* */
    VOICE_84  ,/* */
    VOICE_85  ,/* */
    VOICE_86  ,/* */
    VOICE_87  ,/* */
    VOICE_88  ,/* */
    VOICE_89  ,/* */
    VOICE_90  ,/* */
    VOICE_91  ,/* */
    VOICE_92  ,/* */
    VOICE_INDEX_MAX
};
typedef INT PATCHCMN_INDEX;

#endif /* DX100_CTRL_DEF_H */
