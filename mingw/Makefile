.SUFFIXES: .rc .res

TARGET = xdx.exe
CC = gcc
RC = windres

#Debug
#CFLAGS = -I../inc -Ic:/usr/MinGW/inc -O0 -g -Wall -fmessage-length=0 -finput-charset=cp932 -fexec-charset=cp932
#Release
#CFLAGS = -I../inc -Ic:/usr/MinGW/inc -O1 -Wall -fmessage-length=0 -finput-charset=cp932 -fexec-charset=cp932
CFLAGS = -I../inc -Ic:/usr/MinGW/inc -O1 -Wall -fmessage-length=0 -finput-charset=cp932

CPPFLAGS = -c

LDFLAGS = -Wl,--subsystem,windows -mwindows
LDLIBS = -lcomctl32 -lcomdlg32 -luser32 -lgdi32 -lkernel32 -limm32 -lwinmm

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
          ../wnd/ModalDlg/ModalDialogResource.o	  \
          ..\\wnd\\SomeCtrl\\SomeCtrl.o           \
          ..\\wnd\\StsBar\\StsBar.o               \
          ..\\sys\\Midi\\Midi.o                   \
          ..\\ctrl\\Dx100Ctrl\\dx100Ctrl.o        \
          ..\\ctrl\\Dx100Ctrl\\dx100ParamCtrl.o   \
          ..\\ctrl\\Dx7Ctrl\\dx7Ctrl.o            \
          ..\\ctrl\\Dx7Ctrl\\dx7ParamCtrl.o       \
          ../resources/resource.o                 \

#          ..\\sys\\LinkedList\\LinkedList.o       \
#          ..\\wnd\\EditWnd\\EditWnd.o             \
#          ..\\wnd\\EditWnd\\EditWndBuffer.o       \
#          ..\\wnd\\EditWnd\\EditWndBufferData.o   \

$(TARGET): $(OBJECTS)
		$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
		$(RM) $(OBJECTS)
		$(RM) $(TARGET)

.c.o:
		$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@

.rc.o:
		$(RC) $< -o $@
