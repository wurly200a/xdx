#ifndef Midi_H

BOOL MidiInit( void );
HMIDIOUT MidiOutOpen( UINT uDeviceId );
BOOL MidiOutClose( void );

HMIDIIN MidiInOpen( UINT uDeviceId );
BOOL MidiInClose( void );

BOOL MidiInExclusiveData( BYTE *dataPtr, INT *dataSizePtr );
BOOL MidiOutExclusiveData( BYTE *dataPtr, INT dataSize );

BOOL MidiNoteOn( void );
BOOL MidiNoteOff( void );

void MidiCycleProc( void );
BOOL MidiInqInExec( void );
BOOL MidiInqOutExec( void );

BOOL MidiRcvByte( BYTE *dataPtr, WORD size, DWORD dwTimeout,HANDLE hExitEvent);
BOOL MidiSndByte( BYTE *dataPtr, WORD size );

#define Midi_H
#endif /* Midi_H */
