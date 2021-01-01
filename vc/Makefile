.SUFFIXES: .rc .res

TARGET = dx100ed.exe
CC      = "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\cl.exe"
RC      = "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Bin\RC.Exe"
LINK    = "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\link.exe"
LIBPATH = "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib"
#export LIB="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib;C:\Program Files (x86)\Windows Kits\10\Lib\10.0.14393.0\ucrt\x86"

CFLAGS = //I ../inc //I "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include" \
                    //I "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include" \
                    //I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\ucrt" \
                    //W4 //O2 //MT //EHsc //Z7
#CFLAGS = //I ../inc //W4 //Zi //O2 //MT //EHsc //D"UNICODE" //D"_UNICODE" 

RCFLAGS = //i "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include" \
          //i "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include" \
          //i "C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\ucrt"

LDFLAGS = /SUBSYSTEM:WINDOWS /LIBPATH:$(LIBPATH) /DEBUG:FULL
LDLIBS  = ComCtl32.Lib ComDlg32.Lib User32.Lib Gdi32.Lib kernel32.Lib Imm32.Lib WinMM.Lib

OBJECTS = \
          ..\\develop\\DebugWnd\\DebugWnd.o       \
          ..\\develop\\DebugWnd\\DebugWndMenu.o   \
          ..\\develop\\DebugWnd\\DebugWndFont.o   \
          ..\\develop\\DebugWnd\\DebugWndConfig.o \
          ..\\sys\\Config\\Config.o               \
          ..\\sys\\DateTime\\DateTime.o           \
          ..\\sys\\File\\File.o                   \
          ..\\sys\\Font\\Font.o                   \
          ..\\sys\\Version\\Version.o             \
          ..\\sys\\WinMain\\WinMain.o             \
          ..\\wnd\\MainWnd\\MainWnd.o             \
          ..\\wnd\\MainWnd\\MainWndMenu.o         \
          ..\\wnd\\MainWnd\\MainWndScroll.o       \
          ..\\wnd\\ModalDlg\\ModalDialog.o        \
          ..\\wnd\\ModalDlg\\ModalDialogResource.res \
          ..\\wnd\\SomeCtrl\\SomeCtrl.o           \
          ..\\wnd\\StsBar\\StsBar.o               \
          ..\\sys\\Midi\\Midi.o                   \
          ..\\ctrl\\Dx100Ctrl\\dx100Ctrl.o        \
          ..\\ctrl\\Dx100Ctrl\\dx100ParamCtrl.o   \
          ..\\ctrl\\Dx7Ctrl\\dx7Ctrl.o            \
          ..\\ctrl\\Dx7Ctrl\\dx7ParamCtrl.o       \
          ..\\ctrl\\DxConvert\\DxConvert.o        \
          ../resources/resource.res                 \

#          ..\\sys\\LinkedList\\LinkedList.o       \
#          ..\\wnd\\EditWnd\\EditWnd.o             \
#          ..\\wnd\\EditWnd\\EditWndBuffer.o       \
#          ..\\wnd\\EditWnd\\EditWndBufferData.o   \

$(TARGET): $(OBJECTS)
		$(LINK) /OUT:$@ $(LDFLAGS) $^ $(LDLIBS)

clean:
		$(RM) $(OBJECTS)
		$(RM) $(TARGET)

.c.o:
		$(CC) //c $< $(CFLAGS) //Fo$@

.rc.res:
		$(RC) $(RCFLAGS) -FO $@ $<


# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../develop/DebugWnd/DebugWnd.c        /I ../inc /FoDebugWnd.o         
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../develop/DebugWnd/DebugWndMenu.c    /I ../inc /FoDebugWndMenu.o     
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../develop/DebugWnd/DebugWndFont.c    /I ../inc /FoDebugWndFont.o     
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../develop/DebugWnd/DebugWndConfig.c  /I ../inc /FoDebugWndConfig.o   
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../sys/Config/Config.c                /I ../inc /FoConfig.o           
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../sys/DateTime/DateTime.c            /I ../inc /FoDateTime.o         
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../sys/File/File.c                    /I ../inc /FoFile.o             
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../sys/Font/Font.c                    /I ../inc /FoFont.o             
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../sys/LinkedList/LinkedList.c        /I ../inc /FoLinkedList.o       
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../sys/Version/Version.c              /I ../inc /FoVersion.o          
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../sys/WinMain/WinMain.c              /I ../inc /FoWinMain.o          
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../wnd/EditWnd/EditWnd.c              /I ../inc /FoEditWnd.o          
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../wnd/EditWnd/EditWndBuffer.c        /I ../inc /FoEditWndBuffer.o    
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../wnd/EditWnd/EditWndBufferData.c    /I ../inc /FoEditWndBufferData.o
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../wnd/MainWnd/MainWnd.c              /I ../inc /FoMainWnd.o          
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../wnd/MainWnd/MainWndMenu.c          /I ../inc /FoMainWndMenu.o      
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../wnd/MainWnd/MainWndScroll.c        /I ../inc /FoMainWndScroll.o    
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../wnd/ModalDlg/ModalDialog.c         /I ../inc /FoModalDialog.o      
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../wnd/ParamCtrl/ParamCtrl.c          /I ../inc /FoParamCtrl.o        
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../wnd/SomeCtrl/SomeCtrl.c            /I ../inc /FoSomeCtrl.o         
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../wnd/StsBar/StsBar.c                /I ../inc /FoStsBar.o           
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../sys/Midi/Midi.c                    /I ../inc /FoMidi.o             
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe" /c ../ctrl/Midi/dx100Ctrl.c              /I ../inc /Fodx100Ctrl.o        
# "C:\Program Files (x86)\Windows Kits\8.1\bin\x86\rc.exe" -FO resource.o ../resources/resource.rc
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\link.exe" /SUBSYSTEM:WINDOWS /LIBPATH:"C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x86" /OUT:dx100ed.exe DebugWnd.obj DebugWndMenu.obj DebugWndFont.obj DebugWndConfig.obj Config.obj DateTime.obj File.obj Font.obj LinkedList.obj Version.obj WinMain.obj EditWnd.obj EditWndBuffer.obj EditWndBufferData.obj MainWnd.obj MainWndMenu.obj MainWndScroll.obj ModalDialog.obj ParamCtrl.obj SomeCtrl.obj StsBar.obj Midi.obj dx100Ctrl.obj resource.res

