TARGET = dx100ed.exe
CC      = "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\cl.exe"
RC      = "C:\Program Files (x86)\Windows Kits\8.1\bin\x86\rc.exe"
LINK    = "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX86\x86\link.exe"
LIBPATH = "C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x86"

CFLAGS = //I ../inc //W4 //O2 //MT //EHsc
#CFLAGS = //I ../inc //W4 //Zi //O2 //MT //EHsc //D"UNICODE" //D"_UNICODE" 

LDFLAGS = /SUBSYSTEM:WINDOWS /LIBPATH:$(LIBPATH)
LDLIBS  = ComCtl32.Lib ComDlg32.Lib User32.Lib Gdi32.Lib kernel32.Lib Imm32.Lib WinMM.Lib

OBJECTS = DebugWnd.o DebugWndMenu.o DebugWndFont.o DebugWndConfig.o Config.o DateTime.o File.o Font.o LinkedList.o Version.o WinMain.o EditWnd.o EditWndBuffer.o EditWndBufferData.o MainWnd.o MainWndMenu.o MainWndScroll.o ModalDialog.o ParamCtrl.o SomeCtrl.o StsBar.o Midi.o dx100Ctrl.o resource.o

$(TARGET): $(OBJECTS)
		$(LINK) /OUT:$@ $(LDFLAGS) $^ $(LDLIBS)

clean:
		$(RM) $(OBJECTS)
		$(RM) $(TARGET)

DebugWnd.o: ../develop/DebugWnd/DebugWnd.c
		$(CC) //c $< $(CFLAGS) //Fo$@

DebugWndMenu.o: ../develop/DebugWnd/DebugWndMenu.c
		$(CC) //c $< $(CFLAGS) //Fo$@

DebugWndFont.o: ../develop/DebugWnd/DebugWndFont.c
		$(CC) //c $< $(CFLAGS) //Fo$@

DebugWndConfig.o: ../develop/DebugWnd/DebugWndConfig.c
		$(CC) //c $< $(CFLAGS) //Fo$@

Config.o: ../sys/Config/Config.c
		$(CC) //c $< $(CFLAGS) //Fo$@

DateTime.o: ../sys/DateTime/DateTime.c
		$(CC) //c $< $(CFLAGS) //Fo$@

File.o: ../sys/File/File.c
		$(CC) //c $< $(CFLAGS) //Fo$@

Font.o: ../sys/Font/Font.c
		$(CC) //c $< $(CFLAGS) //Fo$@

LinkedList.o: ../sys/LinkedList/LinkedList.c
		$(CC) //c $< $(CFLAGS) //Fo$@

Version.o: ../sys/Version/Version.c
		$(CC) //c $< $(CFLAGS) //Fo$@

WinMain.o: ../sys/WinMain/WinMain.c
		$(CC) //c $< $(CFLAGS) //Fo$@

EditWnd.o: ../wnd/EditWnd/EditWnd.c
		$(CC) //c $< $(CFLAGS) //Fo$@

EditWndBuffer.o: ../wnd/EditWnd/EditWndBuffer.c
		$(CC) //c $< $(CFLAGS) //Fo$@

EditWndBufferData.o: ../wnd/EditWnd/EditWndBufferData.c
		$(CC) //c $< $(CFLAGS) //Fo$@

MainWnd.o: ../wnd/MainWnd/MainWnd.c
		$(CC) //c $< $(CFLAGS) //Fo$@

MainWndMenu.o: ../wnd/MainWnd/MainWndMenu.c
		$(CC) //c $< $(CFLAGS) //Fo$@

MainWndScroll.o: ../wnd/MainWnd/MainWndScroll.c
		$(CC) //c $< $(CFLAGS) //Fo$@

ModalDialog.o: ../wnd/ModalDlg/ModalDialog.c
		$(CC) //c $< $(CFLAGS) //Fo$@

ParamCtrl.o: ../wnd/ParamCtrl/ParamCtrl.c
		$(CC) //c $< $(CFLAGS) //Fo$@

SomeCtrl.o: ../wnd/SomeCtrl/SomeCtrl.c
		$(CC) //c $< $(CFLAGS) //Fo$@

StsBar.o: ../wnd/StsBar/StsBar.c
		$(CC) //c $< $(CFLAGS) //Fo$@

Midi.o: ../sys/Midi/Midi.c
		$(CC) //c $< $(CFLAGS) //Fo$@

dx100Ctrl.o: ../ctrl/Midi/dx100Ctrl.c
		$(CC) //c $< $(CFLAGS) //Fo$@

resource.o: ../resources/resource.rc
		$(RC) -FO $@ $<


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

