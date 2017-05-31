#ifndef DX100_CTRL_DEF_H
#define DX100_CTRL_DEF_H

enum
{
    MIDI_EX_HEADER_STATUS    ,
    MIDI_EX_HEADER_ID_NUMBER ,
#if 1
    MIDI_EX_HEADER_SUB_STATUS,
    MIDI_EX_HEADER_PARAM     ,
#else
    MIDI_EX_HEADER_DEVICE_ID ,
    MIDI_EX_HEADER_MODEL_ID  ,
    MIDI_EX_HEADER_COMMAND_ID,
    MIDI_EX_HEADER_ADRS0     ,
    MIDI_EX_HEADER_ADRS1     ,
    MIDI_EX_HEADER_ADRS2     ,
    MIDI_EX_HEADER_ADRS3     ,
#endif
    MIDI_EX_HEADER_DATA      ,
};
typedef INT MIDI_EX_HEADER_INDEX;

#define EX_STATUS           0xF0
#define EX_ID_NUMBER_YAMAHA 0x43
#define EX_DEVICE_ID        0x10
#define EX_MODEL_ID_DX100   0x6A
#define EX_ETX              0xF7

#if 1
#define EX_FOOTER_SIZE 1 /* ETX */
#else
#define EX_FOOTER_SIZE 2 /* Checksum,ETX */
#endif

/* Command */
#define RQ1 0x11 /* DataReq1 */
#define DT1 0x12 /* DataSet1 */

/* Adrs */
#define A_BLK_SYS_COMMON           0x00000000
#define A_BLK_TMP_PERF             0x01000000
#define A_BLK_PERF_MODE_TMP_PATCH  0x02000000
#define A_BLK_PATCH_MODE_TMP_PATCH 0x03000000
#define A_BLK_USER_PERF            0x10000000
#define A_BLK_USER_PATCH           0x11000000

#define A_OFFSET_PARTCM            0x0000
#define A_OFFSET_PART01            0x1000
#define A_OFFSET_PART02            0x1100
#define A_OFFSET_PART03            0x1200
#define A_OFFSET_PART04            0x1300
#define A_OFFSET_PART05            0x1400
#define A_OFFSET_PART06            0x1500
#define A_OFFSET_PART07            0x1600
#define A_OFFSET_PART08            0x1700
#define A_OFFSET_PART09            0x1800
#define A_OFFSET_PART10            0x1900
#define A_OFFSET_PART11            0x1A00
#define A_OFFSET_PART12            0x1B00
#define A_OFFSET_PART13            0x1C00
#define A_OFFSET_PART14            0x1D00
#define A_OFFSET_PART15            0x1E00
#define A_OFFSET_PART16            0x1F00

#define OFST_PCH_NUM 0x10000

#define OFST_PCH_CMN 0x0000
#define OFST_PCH_TN1 0x1000
#define OFST_PCH_TN2 0x1200
#define OFST_PCH_TN3 0x1400
#define OFST_PCH_TN4 0x1600

/* 1-1-1.System Common */
enum
{
    SYSCMN_SOUNDMODE                ,/* 0x0000  0000 00aa  Sound Mode 0 - 2 *1                        */
    SYSCMN_PERFORMANCENUMBER        ,/* 0x0001  0aaa aaaa  Performance Number 0 - 127 *2              */
    SYSCMN_PATCHGROUPTYPE           ,/* 0x0002  0000 00aa  Patch Group Type 0 - 2 *3                  */
    SYSCMN_PATCHGROUPID             ,/* 0x0003  0aaa aaaa  Patch Group ID 0 - 127                     */
    SYSCMN_PATCHNUMBER_H            ,/* 0x0004  0000 aaaa  (H) Patch Number 0 - 255                   */
    SYSCMN_PATCHNUMBER_L            ,/* 0x0005  0000 bbbb  (L) Patch Number (001 - 256)               */
    SYSCMN_MASTERTUNE               ,/* 0x0006  0aaa aaaa  Master Tune 0 - 126 *4                     */
    SYSCMN_SCALETUNESWITCH          ,/* 0x0007  0000 000a  Scale Tune Switch 0 - 1 (OFF,ON)           */
    SYSCMN_EFXSWITCH                ,/* 0x0008  0000 000a  EFX Switch 0 - 1 (OFF,ON)                  */
    SYSCMN_CHORUSSWITCH             ,/* 0x0009  0000 000a  Chorus Switch 0 - 1 (OFF,ON)               */
    SYSCMN_REVERBSWITCH             ,/* 0x000A  0000 000a  Reverb Switch 0 - 1 (OFF,ON)               */
    SYSCMN_PATCHREMAIN              ,/* 0x000B  0000 000a  Patch Remain 0 - 1 (OFF,ON)                */
    SYSCMN_CLOCKSOURCE              ,/* 0x000C  0000 000a  Clock Source 0 - 1 (INT,MIDI)              */
    SYSCMN_TAPCONTROLSOURC          ,/* 0x000D  0000 0aaa  TAP Control Source 0 - 4 *5                */
    SYSCMN_HOLDCONTROLSOURCE        ,/* 0x000E  0000 0aaa  Hold Control Source 0 - 4 *6               */
    SYSCMN_PEAKCONTROLSOURCE        ,/* 0x000F  0000 0aaa  Peak Control Source 0 - 4 *6               */
    SYSCMN_VOLUMECONTROLSOURCE      ,/* 0x0010  0000 000a  Volume Control Source 0 - 1 *7             */
    SYSCMN_AFTERTOUCHSOURCE         ,/* 0x0011  0000 00aa  Aftertouch Source 0 - 2 *8                 */
    SYSCMN_SYSTEMCONTROLSOURCE11_97 ,/* 0x0012  0aaa aaaa  System Control Source 1 1 - 97 *9          */
    SYSCMN_SYSTEMCONTROLSOURCE21_97 ,/* 0x0013  0aaa aaaa  System Control Source 2 1 - 97 *9          */
    SYSCMN_RECEIVEPROGRAMCHANGE     ,/* 0x0014  0000 000a  Receive Program Change 0 - 1 (OFF,ON)      */
    SYSCMN_RECEIVEBANKSELECT        ,/* 0x0015  0000 000a  Receive Bank Select 0 - 1 (OFF,ON)         */
    SYSCMN_RECEIVECONTROLCHANGE     ,/* 0x0016  0000 000a  Receive Control Change 0 - 1 (OFF,ON)      */
    SYSCMN_RECEIVEMODULATION        ,/* 0x0017  0000 000a  Receive Modulation 0 - 1 (OFF,ON)          */
    SYSCMN_RECEIVEVOLUME            ,/* 0x0018  0000 000a  Receive Volume 0 - 1 (OFF,ON)              */
    SYSCMN_RECEIVEHOLD              ,/* 0x0019  0000 000a  Receive Hold-1 0 - 1 (OFF,ON)              */
    SYSCMN_RECEIVEPITCHBEND         ,/* 0x001A  0000 000a  Receive Pitch Bend 0 - 1 (OFF,ON)          */
    SYSCMN_RECEIVEAFTERTOUCH        ,/* 0x001B  0000 000a  Receive Aftertouch 0 - 1 (OFF,ON)          */
    SYSCMN_CONTROLCHANNEL           ,/* 0x001C  000a aaaa  Control Channel 0 - 16 (1 - 16,OFF)        */
    SYSCMN_PATCHRECEIVECHANNEL      ,/* 0x001D  0000 aaaa  Patch Receive Channel 0 - 15 (1 - 16)      */
    SYSCMN_RHYTHMEDITSOURCE         ,/* 0x001E  0000 000a  Rhythm Edit Source 0 - 1 *10               */
    SYSCMN_PREVIEWSOUNDMODE         ,/* 0x001F  0000 00aa  Preview Sound Mode 0 - 2 *11               */
    SYSCMN_PREVIEWNOTESET10_127     ,/* 0x0020  0aaa aaaa  Preview Note Set 1 0 - 127 (C-1 - G9)      */
    SYSCMN_PREVIEWVELOCITYSET10_127 ,/* 0x0021  0aaa aaaa  Preview Velocity Set 1 0 - 127 *12         */
    SYSCMN_PREVIEWNOTESET20_127     ,/* 0x0022  0aaa aaaa  Preview Note Set 2 0 - 127 (C-1 - G9)      */
    SYSCMN_PREVIEWVELOCITYSET20_127 ,/* 0x0023  0aaa aaaa  Preview Velocity Set 2 0 - 127 *12         */
    SYSCMN_PREVIEWNOTESET30_127     ,/* 0x0024  0aaa aaaa  Preview Note Set 3 0 - 127 (C-1 - G9)      */
    SYSCMN_PREVIEWVELOCITYSET30_127 ,/* 0x0025  0aaa aaaa  Preview Velocity Set 3 0 - 127 *12         */
    SYSCMN_PREVIEWNOTESET40_127     ,/* 0x0026  0aaa aaaa  Preview Note Set 4 0 - 127 (C-1 - G9)      */
    SYSCMN_PREVIEWVELOCITYSET40_127 ,/* 0x0027  0aaa aaaa  Preview Velocity Set 4 0 - 127 *12         */
    SYSCMN_TRANSMITPROGRAMCHANGE    ,/* 0x0028  0000 000a  Transmit Program Change 0 - 1 (<OFF,ON>)   */
    SYSCMN_TRANSMITBANKSELECT       ,/* 0x0029  0000 000a  Transmit Bank Select 0 - 1 (<OFF,ON>)      */
    SYSCMN_PATCHTRANSMITCHANNEL     ,/* 0x002A  000a aaaa  Patch Transmit Channel 0 - 17 *13          */
    SYSCMN_TRANSPOSESWITCH          ,/* 0x002B  0000 000a  Transpose Switch 0 - 1 (OFF,ON)            */
    SYSCMN_TRANSPOSEVALUE           ,/* 0x002C  0000 aaaa  Transpose Value 0 - 11 (-5 - +6)           */
    SYSCMN_OCTAVESHIFT              ,/* 0x002D  0000 0aaa  Octave Shift 0 - 6 (-3 - +3)               */
    SYSCMN_KEYBOARDVELOCITY         ,/* 0x002E  0aaa aaaa  Keyboard Velocity 0 - 127 *14              */
    SYSCMN_KEYBOARDSENS             ,/* 0x002F  0000 00aa  Keyboard Sens 0 - 2 *15                    */
    SYSCMN_AFTERTOUCHSENS           ,/* 0x0030  0aaa aaaa  Aftertouch Sens 0 - 100                    */
    SYSCMN_PEDAL_1_ASSIGN           ,/* 0x0031  0aaa aaaa  Pedal(1) Assign 1 - 104 *16                */
    SYSCMN_PEDAL_1_OUTPUTMODE       ,/* 0x0032  0000 00aa  Pedal(1) Output Mode 0 - 3 *18             */
    SYSCMN_PEDAL_1_POLARITY         ,/* 0x0033  0000 000a  Pedal(1) Polarity 0 - 1 *20                */
    SYSCMN_PEDAL2ASSIGN             ,/* 0x0034  0aaa aaaa  Pedal2 Assign 1 - 104 *17                  */
    SYSCMN_PEDAL2OUTPUTMODE         ,/* 0x0035  0000 00aa  Pedal2 Output Mode 0 - 3 *19               */
    SYSCMN_PEDAL2POLARITY           ,/* 0x0036  0000 000a  Pedal2 Polarity 0 - 1 *21                  */
    SYSCMN_C1ASSIGN                 ,/* 0x0037  0aaa aaaa  C1 Assign 1 - 97 *9                        */
    SYSCMN_C1OUTPUTMODE             ,/* 0x0038  0000 00aa  C1 Output Mode 0 - 3 *18                   */
    SYSCMN_C2ASSIGN                 ,/* 0x0039  0aaa aaaa  C2 Assign 1 - 97 *9                        */
    SYSCMN_C2OUTPUTMODE             ,/* 0x003A  0000 00aa  C2 Output Mode 0 - 3 *18                   */
    SYSCMN_HOLDPEDALOUTPUTMODE      ,/* 0x003B  0000 00aa  Hold Pedal Output Mode 0 - 3 *18           */
    SYSCMN_HOLDPEDALPOLARITY        ,/* 0x003C  0000 000a  Hold Pedal Polarity 0 - 1 *20              */
    SYSCMN_BANKSELECTGROUP1SWITCH   ,/* 0x003D  0000 000a  Bank Select Group1 Switch 0 - 1 (OFF,ON)   */
    SYSCMN_BANKSELECTGROUP1MSB      ,/* 0x003E  0aaa aaaa  Bank Select Group1 MSB 0 - 127             */
    SYSCMN_BANKSELECTGROUP1LSB      ,/* 0x003F  0aaa aaaa  Bank Select Group1 LSB 0 - 127             */
    SYSCMN_BANKSELECTGROUP2SWITCH   ,/* 0x0040  0000 000a  Bank Select Group2 Switch 0 - 1 (OFF,ON)   */
    SYSCMN_BANKSELECTGROUP2MSB0     ,/* 0x0041  0aaa aaaa  Bank Select Group2 MSB 0 - 127             */
    SYSCMN_BANKSELECTGROUP2LSB0     ,/* 0x0042  0aaa aaaa  Bank Select Group2 LSB 0 - 127             */
    SYSCMN_BANKSELECTGROUP3SWITCH   ,/* 0x0043  0000 000a  Bank Select Group3 Switch 0 - 1 (OFF,ON)   */
    SYSCMN_BANKSELECTGROUP3MSB      ,/* 0x0044  0aaa aaaa  Bank Select Group3 MSB 0 - 127             */
    SYSCMN_BANKSELECTGROUP3LSB      ,/* 0x0045  0aaa aaaa  Bank Select Group3 LSB 0 - 127             */
    SYSCMN_BANKSELECTGROUP4SWITCH   ,/* 0x0046  0000 000a  Bank Select Group4 Switch 0 - 1 (OFF,ON)   */
    SYSCMN_BANKSELECTGROUP4MSB      ,/* 0x0047  0aaa aaaa  Bank Select Group4 MSB 0 - 127             */
    SYSCMN_BANKSELECTGROUP4LSB      ,/* 0x0048  0aaa aaaa  Bank Select Group4 LSB 0 - 127             */
    SYSCMN_BANKSELECTGROUP5SWITCH   ,/* 0x0049  0000 000a  Bank Select Group5 Switch 0 - 1 (OFF,ON)   */
    SYSCMN_BANKSELECTGROUP5MSB      ,/* 0x004A  0aaa aaaa  Bank Select Group5 MSB 0 - 127             */
    SYSCMN_BANKSELECTGROUP5LSB      ,/* 0x004B  0aaa aaaa  Bank Select Group5 LSB 0 - 127             */
    SYSCMN_BANKSELECTGROUP6SWITCH   ,/* 0x004C  0000 000a  Bank Select Group6 Switch 0 - 1 (OFF,ON)   */
    SYSCMN_BANKSELECTGROUP6MSB      ,/* 0x004D  0aaa aaaa  Bank Select Group6 MSB 0 - 127             */
    SYSCMN_BANKSELECTGROUP6LSB      ,/* 0x004E  0aaa aaaa  Bank Select Group6 LSB 0 - 127             */
    SYSCMN_BANKSELECTGROUP7SWITCH   ,/* 0x004F  0000 000a  Bank Select Group7 Switch 0 - 1 (OFF,ON)   */
    SYSCMN_BANKSELECTGROUP7MSB      ,/* 0x0050  0aaa aaaa  Bank Select Group7 MSB 0 - 127             */
    SYSCMN_BANKSELECTGROUP7LSB      ,/* 0x0051  0aaa aaaa  Bank Select Group7 LSB 0 - 127             */
    SYSCMN_PEDAL3ASSIGN             ,/* 0x0052  0aaa aaaa  Pedal3 Assign 1 - 104 *17                  */
    SYSCMN_PEDAL3OUTPUTMODE         ,/* 0x0053  0000 00aa  Pedal3 Output Mode 0 - 3 *19               */
    SYSCMN_PEDAL3POLARITY           ,/* 0x0054  0000 000a  Pedal3 Polarity 0 - 1 *21                  */
    SYSCMN_PEDAL4ASSIGN             ,/* 0x0055  0aaa aaaa  Pedal4 Assign 1 - 104 *17                  */
    SYSCMN_PEDAL4OUTPUTMODE         ,/* 0x0056  0000 00aa  Pedal4 Output Mode 0 - 3 *19               */
    SYSCMN_PEDAL4POLARITY           ,/* 0x0057  0000 000a  Pedal4 Polarity 0 - 1 *21                  */
    SYSCMN_ARPEGGIOSTYLE            ,/* 0x0058  00aa aaaa  Arpeggio Style 0 - 42 (<1 - 43>)           */
    SYSCMN_ARPEGGIOMOTIF            ,/* 0x0059  00aa aaaa  Arpeggio Motif 0 - 37 (<1 - 38>)           */
    SYSCMN_ARPEGGIOBEATPATTERN      ,/* 0x005A  0aaa aaaa  Arpeggio Beat Pattern 0 - 114 (<1 - 115>)  */
    SYSCMN_ARPEGGIOACCENTRATE       ,/* 0x005B  0aaa aaaa  Arpeggio Accent Rate 0 - 100 (<0 - 100>)   */
    SYSCMN_ARPEGGIOSHUFFLERATE      ,/* 0x005C  0aaa aaaa  Arpeggio Shuffle Rate 50 - 90 (<50 - 90>)  */
    SYSCMN_ARPEGGIOKEYBOARDVELOCITY ,/* 0x005D  0aaa aaaa  Arpeggio Keyboard Velocity 0 - 127 *14     */
    SYSCMN_ARPEGGIOOCTAVERANGE      ,/* 0x005E  0000 0aaa  Arpeggio Octave Range 0 - 6 (<-3 - +3>)    */
    SYSCMN_ARPEGGIOPARTNUMBER       ,/* 0x005F  0000 aaaa  Arpeggio Part Number 0 - 15 *22            */
    SYSCMN_SYSTEMTEMPO_H            ,/* 0x0060  0000 aaaa  System Tempo (H)20 - 250                   */
    SYSCMN_SYSTEMTEMPO_L            ,/* 0x0061  0000 bbbb  System Tempo (L)                           */
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

/* 1-3-2.Patch Tone */
enum
{
    PATCHTONE_TONESWITCH                 ,/* 0x0000 0000 000a  Tone Switch 0 - 1 (OFF,ON)                           */
    PATCHTONE_WAVEGROUPTYPE              ,/* 0x0001 0000 00aa  Wave Group Type 0 - 2 *1                             */
    PATCHTONE_WAVEGROUPID                ,/* 0x0002 0aaa aaaa  Wave Group ID 0 - 127                                */
    PATCHTONE_WAVENUMBERH                ,/* 0x0003 0000 aaaa  Wave NumberH 0 - 254                                 */
    PATCHTONE_WAVENUMBERL                ,/* 0x0004 0000 bbbb  Wave NumberL (001 - 255)                             */
    PATCHTONE_WAVEGAIN                   ,/* 0x0005 0000 00aa  Wave Gain 0 - 3 *2                                   */
    PATCHTONE_FXMSWITCH                  ,/* 0x0006 0000 000a  FXM Switch 0 - 1 (OFF,ON)                            */
    PATCHTONE_FXMCOLOR                   ,/* 0x0007 0000 00aa  FXM Color 0 - 3 (1 - 4)                              */
    PATCHTONE_FXMDEPTH                   ,/* 0x0008 0000 aaaa  FXM Depth 0 - 15 (1 - 16)                            */
    PATCHTONE_TONEDELAYMODE              ,/* 0x0009 0000 0aaa  Tone Delay Mode 0 - 7 *3                             */
    PATCHTONE_TONEDELAYTIME              ,/* 0x000A 0aaa aaaa  Tone Delay Time 0 - 127                              */
    PATCHTONE_VELOCITYCROSSFADE          ,/* 0x000B 0aaa aaaa  Velocity Cross Fade 0 - 127                          */
    PATCHTONE_VELOCITYRANGELOWER         ,/* 0x000C 0aaa aaaa  Velocity Range Lower 1 - 127 *4                      */
    PATCHTONE_VELOCITYRANGEUPPER         ,/* 0x000D 0aaa aaaa  Velocity Range Upper 1 - 127 *5                      */
    PATCHTONE_KEYBOARDRANGELOWER         ,/* 0x000E 0aaa aaaa  Keyboard Range Lower 0 - 127 *6                      */
    PATCHTONE_KEYBOARDRANGEUPPER         ,/* 0x000F 0aaa aaaa  Keyboard Range Upper 0 - 127 *7                      */
    PATCHTONE_REDAMPERCONTROLSWITCH      ,/* 0x0010 0000 000a  Redamper Control Switch 0 - 1 (OFF,ON)               */
    PATCHTONE_VOLUMECONTROLSWITCH        ,/* 0x0011 0000 000a  Volume Control Switch 0 - 1 (OFF,ON)                 */
    PATCHTONE_HOLD_1CONTROLSWITCH        ,/* 0x0012 0000 000a  Hold-1 Control Switch 0 - 1 (OFF,ON)                 */
    PATCHTONE_PITCHBENDCONTROLSWITCH     ,/* 0x0013 0000 000a  Pitch Bend Control Switch 0 - 1 (OFF,ON)             */
    PATCHTONE_PANCONTROLSWITCH           ,/* 0x0014 0000 00aa  Pan Control Switch 0 - 2 *8                          */
    PATCHTONE_CONTROLLER1DESTINATION1    ,/* 0x0015 000a aaaa  Controller 1 Destination 1 0 - 18 *9                 */
    PATCHTONE_CONTROLLER1DEPTH1          ,/* 0x0016 0aaa aaaa  Controller 1 Depth 1 0 - 126 (-63 - +63)             */
    PATCHTONE_CONTROLLER1DESTINATION2    ,/* 0x0017 000a aaaa  Controller 1 Destination 2 0 - 18 *9                 */
    PATCHTONE_CONTROLLER1DEPTH2          ,/* 0x0018 0aaa aaaa  Controller 1 Depth 2 0 - 126 (-63 - +63)             */
    PATCHTONE_CONTROLLER1DESTINATION3    ,/* 0x0019 000a aaaa  Controller 1 Destination 3 0 - 18 *9                 */
    PATCHTONE_CONTROLLER1DEPTH3          ,/* 0x001A 0aaa aaaa  Controller 1 Depth 3 0 - 126 (-63 - +63)             */
    PATCHTONE_CONTROLLER1DESTINATION4    ,/* 0x001B 000a aaaa  Controller 1 Destination 4 0 - 18 *9                 */
    PATCHTONE_CONTROLLER1DEPTH4          ,/* 0x001C 0aaa aaaa  Controller 1 Depth 4 0 - 126 (-63 - +63)             */
    PATCHTONE_CONTROLLER2DESTINATION1    ,/* 0x001D 000a aaaa  Controller 2 Destination 1 0 - 18 *9                 */
    PATCHTONE_CONTROLLER2DEPTH1          ,/* 0x001E 0aaa aaaa  Controller 2 Depth 1 0 - 126 (-63 - +63)             */
    PATCHTONE_CONTROLLER2DESTINATION2    ,/* 0x001F 000a aaaa  Controller 2 Destination 2 0 - 18 *9                 */
    PATCHTONE_CONTROLLER2DEPTH2          ,/* 0x0020 0aaa aaaa  Controller 2 Depth 2 0 - 126 (-63 - +63)             */
    PATCHTONE_CONTROLLER2DESTINATION3    ,/* 0x0021 000a aaaa  Controller 2 Destination 3 0 - 18 *9                 */
    PATCHTONE_CONTROLLER2DEPTH3          ,/* 0x0022 0aaa aaaa  Controller 2 Depth 3 0 - 126 (-63 - +63)             */
    PATCHTONE_CONTROLLER2DESTINATION4    ,/* 0x0023 000a aaaa  Controller 2 Destination 4 0 - 18 *9                 */
    PATCHTONE_CONTROLLER2DEPTH4          ,/* 0x0024 0aaa aaaa  Controller 2 Depth 4 0 - 126 (-63 - +63)             */
    PATCHTONE_CONTROLLER3DESTINATION1    ,/* 0x0025 000a aaaa  Controller 3 Destination 1 0 - 18 *9                 */
    PATCHTONE_CONTROLLER3DEPTH1          ,/* 0x0026 0aaa aaaa  Controller 3 Depth 1 0 - 126 (-63 - +63)             */
    PATCHTONE_CONTROLLER3DESTINATION2    ,/* 0x0027 000a aaaa  Controller 3 Destination 2 0 - 18 *9                 */
    PATCHTONE_CONTROLLER3DEPTH2          ,/* 0x0028 0aaa aaaa  Controller 3 Depth 2 0 - 126 (-63 - +63)             */
    PATCHTONE_CONTROLLER3DESTINATION3    ,/* 0x0029 000a aaaa  Controller 3 Destination 3 0 - 18 *9                 */
    PATCHTONE_CONTROLLER3DEPTH3          ,/* 0x002A 0aaa aaaa  Controller 3 Depth 3 0 - 126 (-63 - +63)             */
    PATCHTONE_CONTROLLER3DESTINATION4    ,/* 0x002B 000a aaaa  Controller 3 Destination 4 0 - 18 *9                 */
    PATCHTONE_CONTROLLER3DEPTH4          ,/* 0x002C 0aaa aaaa  Controller 3 Depth 4 0 - 126 (-63 - +63)             */
    PATCHTONE_LFO1WAVEFORM               ,/* 0x002D 0000 0aaa  LFO1 Waveform 0 - 7 *10                              */
    PATCHTONE_LFO1KEYSYNC                ,/* 0x002E 0000 000a  LFO1 Key Sync 0 - 1 (OFF,ON)                         */
    PATCHTONE_LFO1RATE                   ,/* 0x002F 0aaa aaaa  LFO1 Rate 0 - 127                                    */
    PATCHTONE_LFO1OFFSET                 ,/* 0x0030 0000 0aaa  LFO1 Offset 0 - 4 *11                                */
    PATCHTONE_LFO1DELAYTIME              ,/* 0x0031 0aaa aaaa  LFO1 Delay Time 0 - 127                              */
    PATCHTONE_LFO1FADEMODE               ,/* 0x0032 0000 00aa  LFO1 Fade Mode 0 - 3 *12                             */
    PATCHTONE_LFO1FADETIME               ,/* 0x0033 0aaa aaaa  LFO1 Fade Time 0 - 127                               */
    PATCHTONE_LFO1EXTERNALSYNC           ,/* 0x0034 0000 00aa  LFO1 External Sync 0 - 2 *13                         */
    PATCHTONE_LFO2WAVEFORM               ,/* 0x0035 0000 0aaa  LFO2 Waveform 0 - 7 *10                              */
    PATCHTONE_LFO2KEYSYNC                ,/* 0x0036 0000 000a  LFO2 Key Sync 0 - 1 (OFF,ON)                         */
    PATCHTONE_LFO2RATE                   ,/* 0x0037 0aaa aaaa  LFO2 Rate 0 - 127                                    */
    PATCHTONE_LFO2OFFSET                 ,/* 0x0038 0000 0aaa  LFO2 Offset 0 - 4 *11                                */
    PATCHTONE_LFO2DELAYTIME              ,/* 0x0039 0aaa aaaa  LFO2 Delay Time 0 - 127                              */
    PATCHTONE_LFO2FADEMODE               ,/* 0x003A 0000 00aa  LFO2 Fade Mode 0 - 3 *12                             */
    PATCHTONE_LFO2FADETIME               ,/* 0x003B 0aaa aaaa  LFO2 Fade Time 0 - 127                               */
    PATCHTONE_LFO2EXTERNALSYNC           ,/* 0x003C 0000 00aa  LFO2 External Sync 0 - 2 *13                         */
    PATCHTONE_COARSETUNE                 ,/* 0x003D 0aaa aaaa  Coarse Tune 0 - 96 (-48 - +48)                       */
    PATCHTONE_FINETUNE                   ,/* 0x003E 0aaa aaaa  Fine Tune 0 - 100 (-50 - +50)                        */
    PATCHTONE_RANDOMPITCHDEPTH           ,/* 0x003F 000a aaaa  Random Pitch Depth 0 - 30 *14                        */
    PATCHTONE_PITCHKEYFOLLOW             ,/* 0x0040 0000 aaaa  Pitch Keyfollow 0 - 15 *15                           */
    PATCHTONE_PITCHENVELOPEDEPTH         ,/* 0x0041 000a aaaa  Pitch Envelope Depth 0 - 24 (-12 - +12)              */
    PATCHTONE_PITCHENVELOPEVELOCITYSENS  ,/* 0x0042 0aaa aaaa  Pitch Envelope Velocity Sens 0 - 125 *16             */
    PATCHTONE_PITCHENVELOPEVELOCITYTIME1 ,/* 0x0043 0000 aaaa  Pitch Envelope Velocity Time1 0 - 14 *17             */
    PATCHTONE_PITCHENVELOPEVELOCITYTIME4 ,/* 0x0044 0000 aaaa  Pitch Envelope Velocity Time4 0 - 14 *17             */
    PATCHTONE_PITCHENVELOPETIMEKEYFOLLOW ,/* 0x0045 0000 aaaa  Pitch Envelope Time Keyfollow 0 - 14 *17             */
    PATCHTONE_PITCHENVELOPETIME1         ,/* 0x0046 0aaa aaaa  Pitch Envelope Time 1 0 - 127                        */
    PATCHTONE_PITCHENVELOPETIME2         ,/* 0x0047 0aaa aaaa  Pitch Envelope Time 2 0 - 127                        */
    PATCHTONE_PITCHENVELOPETIME3         ,/* 0x0048 0aaa aaaa  Pitch Envelope Time 3 0 - 127                        */
    PATCHTONE_PITCHENVELOPETIME4         ,/* 0x0049 0aaa aaaa  Pitch Envelope Time 4 0 - 127                        */
    PATCHTONE_PITCHENVELOPELEVEL1        ,/* 0x004A 0aaa aaaa  Pitch Envelope Level 1 0 - 126 (-63 - +63)           */
    PATCHTONE_PITCHENVELOPELEVEL2        ,/* 0x004B 0aaa aaaa  Pitch Envelope Level 2 0 - 126 (-63 - +63)           */
    PATCHTONE_PITCHENVELOPELEVEL3        ,/* 0x004C 0aaa aaaa  Pitch Envelope Level 3 0 - 126 (-63 - +63)           */
    PATCHTONE_PITCHENVELOPELEVEL4        ,/* 0x004D 0aaa aaaa  Pitch Envelope Level 4 0 - 126 (-63 - +63)           */
    PATCHTONE_PITCHLFO1DEPTH             ,/* 0x004E 0aaa aaaa  Pitch LFO1 Depth 0 - 126 (-63 - +63)                 */
    PATCHTONE_PITCHLFO2DEPTH             ,/* 0x004F 0aaa aaaa  Pitch LFO2 Depth 0 - 126 (-63 - +63)                 */
    PATCHTONE_FILTERTYPE                 ,/* 0x0050 0000 0aaa  Filter Type 0 - 4 *18                                */
    PATCHTONE_CUTOFFFREQUENCY            ,/* 0x0051 0aaa aaaa  Cutoff Frequency 0 - 127                             */
    PATCHTONE_CUTOFFKEYFOLLOW            ,/* 0x0052 0000 aaaa  Cutoff Keyfollow 0 - 15 *15                          */
    PATCHTONE_RESONANCE                  ,/* 0x0053 0aaa aaaa  Resonance 0 - 127                                    */
    PATCHTONE_RESONANCEVELOCITYSENS      ,/* 0x0054 0aaa aaaa  Resonance Velocity Sens 0 - 125 *16                  */
    PATCHTONE_FILTERENVELOPEDEPTH        ,/* 0x0055 0aaa aaaa  Filter Envelope Depth 0 - 126 (-63 - +63)            */
    PATCHTONE_FILTERENVELOPEVELOCITYCURVE,/* 0x0056 0000 0aaa  Filter Envelope Velocity Curve 0 - 6 (1 - 7)         */
    PATCHTONE_FILTERENVELOPEVELOCITYSENS ,/* 0x0057 0aaa aaaa  Filter Envelope Velocity Sens 0 - 125 *16            */
    PATCHTONE_FILTERENVELOPEVELOCITYTIME1,/* 0x0058 0000 aaaa  Filter Envelope Velocity Time1 0 - 14 *17            */
    PATCHTONE_FILTERENVELOPEVELOCITYTIME4,/* 0x0059 0000 aaaa  Filter Envelope Velocity Time4 0 - 14 *17            */
    PATCHTONE_FILTERENVELOPETIMEKEYFOLLOW,/* 0x005A 0000 aaaa  Filter Envelope Time Keyfollow 0 - 14 *17            */
    PATCHTONE_FILTERENVELOPETIME1        ,/* 0x005B 0aaa aaaa  Filter Envelope Time 1 0 - 127                       */
    PATCHTONE_FILTERENVELOPETIME2        ,/* 0x005C 0aaa aaaa  Filter Envelope Time 2 0 - 127                       */
    PATCHTONE_FILTERENVELOPETIME3        ,/* 0x005D 0aaa aaaa  Filter Envelope Time 3 0 - 127                       */
    PATCHTONE_FILTERENVELOPETIME4        ,/* 0x005E 0aaa aaaa  Filter Envelope Time 4 0 - 127                       */
    PATCHTONE_FILTERENVELOPELEVEL1       ,/* 0x005F 0aaa aaaa  Filter Envelope Level 1 0 - 127                      */
    PATCHTONE_FILTERENVELOPELEVEL2       ,/* 0x0060 0aaa aaaa  Filter Envelope Level 2 0 - 127                      */
    PATCHTONE_FILTERENVELOPELEVEL3       ,/* 0x0061 0aaa aaaa  Filter Envelope Level 3 0 - 127                      */
    PATCHTONE_FILTERENVELOPELEVEL4       ,/* 0x0062 0aaa aaaa  Filter Envelope Level 4 0 - 127                      */
    PATCHTONE_FILTERLFO1DEPTH            ,/* 0x0063 0aaa aaaa  Filter LFO1 Depth 0 - 126 (-63 - +63)                */
    PATCHTONE_FILTERLFO2DEPTH            ,/* 0x0064 0aaa aaaa  Filter LFO2 Depth 0 - 126 (-63 - +63)                */
    PATCHTONE_TONELEVEL                  ,/* 0x0065 0aaa aaaa  Tone Level 0 - 127                                   */
    PATCHTONE_BIASDIRECTION              ,/* 0x0066 0000 00aa  Bias Direction 0 - 3 *19                             */
    PATCHTONE_BIASPOSITION               ,/* 0x0067 0aaa aaaa  Bias Position 0 - 127 (C-1 - G9)                     */
    PATCHTONE_BIASLEVEL                  ,/* 0x0068 0000 aaaa  Bias Level 0 - 14 *17                                */
    PATCHTONE_LEVELENVELOPEVELOCITYCURVE ,/* 0x0069 0000 0aaa  Level Envelope Velocity Curve 0 - 6 (1 - 7)          */
    PATCHTONE_LEVELENVELOPEVELOCITYSENS  ,/* 0x006A 0aaa aaaa  Level Envelope Velocity Sens 0 - 125 *16             */
    PATCHTONE_LEVELENVELOPEVELOCITYTIME1 ,/* 0x006B 0000 aaaa  Level Envelope Velocity Time1 0 - 14 *17             */
    PATCHTONE_LEVELENVELOPEVELOCITYTIME4 ,/* 0x006C 0000 aaaa  Level Envelope Velocity Time4 0 - 14 *17             */
    PATCHTONE_LEVELENVELOPETIMEKEYFOLLOW ,/* 0x006D 0000 aaaa  Level Envelope Time Keyfollow 0 - 14 *17             */
    PATCHTONE_LEVELENVELOPETIME1         ,/* 0x006E 0aaa aaaa  Level Envelope Time 1 0 - 127                        */
    PATCHTONE_LEVELENVELOPETIME2         ,/* 0x006F 0aaa aaaa  Level Envelope Time 2 0 - 127                        */
    PATCHTONE_LEVELENVELOPETIME3         ,/* 0x0070 0aaa aaaa  Level Envelope Time 3 0 - 127                        */
    PATCHTONE_LEVELENVELOPETIME4         ,/* 0x0071 0aaa aaaa  Level Envelope Time 4 0 - 127                        */
    PATCHTONE_LEVELENVELOPELEVEL1        ,/* 0x0072 0aaa aaaa  Level Envelope Level 1 0 - 127                       */
    PATCHTONE_LEVELENVELOPELEVEL2        ,/* 0x0073 0aaa aaaa  Level Envelope Level 2 0 - 127                       */
    PATCHTONE_LEVELENVELOPELEVEL3        ,/* 0x0074 0aaa aaaa  Level Envelope Level 3 0 - 127                       */
    PATCHTONE_LEVELLFO1DEPTH             ,/* 0x0075 0aaa aaaa  Level LFO1 Depth 0 - 126 (-63 - +63)                 */
    PATCHTONE_LEVELLFO2DEPTH             ,/* 0x0076 0aaa aaaa  Level LFO2 Depth 0 - 126 (-63 - +63)                 */
    PATCHTONE_TONEPAN                    ,/* 0x0077 0aaa aaaa  Tone Pan 0 - 127 (L64 - 63R)                         */
    PATCHTONE_PANKEYFOLLOW               ,/* 0x0078 0000 aaaa  Pan Keyfollow 0 - 14 *17                             */
    PATCHTONE_RANDOMPANDEPTH             ,/* 0x0079 00aa aaaa  Random Pan Depth 0 - 63                              */
    PATCHTONE_ALTERNATEPANDEPTH          ,/* 0x007A 0aaa aaaa  Alternate Pan Depth 1 - 127 (L63 - 63R)              */
    PATCHTONE_PANLFO1DEPTH               ,/* 0x007B 0aaa aaaa  Pan LFO1 Depth 0 - 126 (L63 - 63R)                   */
    PATCHTONE_PANLFO2DEPTH               ,/* 0x007C 0aaa aaaa  Pan LFO2 Depth 0 - 126 (L63 - 63R)                   */
    PATCHTONE_OUTPUTASSIGN               ,/* 0x007D 0000 00aa  Output Assign 0 - 3 *20                              */
    PATCHTONE_MIX_EFXSENDLEVEL           ,/* 0x007E 0aaa aaaa  Mix/EFX Send Level 0 - 127                           */
    PATCHTONE_CHORUSSENDLEVEL            ,/* 0x007F 0aaa aaaa  Chorus Send Level 0 - 127                            */
    PATCHTONE_REVERBSENDLEVEL            ,/* 0x0100 0aaa aaaa  Reverb Send Level 0 - 127                            */
    PATCHTONE_INDEX_MAX
};
typedef INT PATCHTONE_INDEX;

#endif /* DX100_CTRL_DEF_H */
