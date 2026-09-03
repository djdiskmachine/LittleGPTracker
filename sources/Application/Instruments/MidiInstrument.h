#ifndef _MIDI_INSTRUMENT_H_
#define _MIDI_INSTRUMENT_H_

#include "I_Instrument.h"
#include "Application/Model/Song.h"
#include "Services/Midi/MidiService.h"

#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CC 0xB0
#define MIDI_PRG 0xC0

#define MIP_CHANNEL			MAKE_FOURCC('C','H','N','L')
#define MIP_NOTELENGTH		MAKE_FOURCC('L','E','N','G')
#define MIP_VOLUME    		MAKE_FOURCC('V','O','L','M')
#define MIP_PAN   			MAKE_FOURCC('P','A','N',' ')
#define MIP_TABLE			MAKE_FOURCC('T','A','B','L')
#define MIP_TABLEAUTO		MAKE_FOURCC('T','B','L','A')

class MidiInstrument:public I_Instrument {

public:
	MidiInstrument();
	virtual ~MidiInstrument() ;

	  virtual bool Init() ;

	  // Start & stop the instument
      virtual bool Start(int channel, unsigned char note, int flags = 1);
      virtual void Stop(int channel) ;

      // size refers to the number of samples
      // should always fill interleaved stereo / 16bit
      virtual bool Render(int channel, fixed *buffer, int size, int flags);
      virtual void ProcessCommand(int channel,FourCC cc,ushort value) ;

      virtual bool IsInitialized() ;

	  virtual bool IsEmpty() { return false ; } ;

	  virtual InstrumentType GetType() { return IT_MIDI ; } ;

	   virtual const char *GetName() ;

	  virtual void OnStart() ;

	   virtual void Purge() {} ;

	   virtual int GetTable() ;
	   virtual bool GetTableAutomation();
       virtual void GetTableState(TableSaveState &state);
       virtual void SetTableState(TableSaveState &state);
       virtual void QueueNote(bool note_on, int channel, unsigned char note,
                              unsigned char velocity);
       virtual void SetVolume(int channel, unsigned char volume);
       virtual void SetCC(int channel, unsigned char id, unsigned char value);
       virtual void SetPRG(int channel, unsigned char id);

       // external parameter list

       void SetChannel(int i);

     private:
       char name_[20]; // Instrument name
       T_SimpleList<unsigned char>
           *lastNote_[SONG_CHANNEL_COUNT]; // List of played note(s).
       int rootNote_[SONG_CHANNEL_COUNT];
       // Keep track of last requested note even if not played. This way,
       // 'root notes' of chords are stored even when the track is muted.
       int remainingTicks_;
       bool playing_;
       bool retrig_;
       int retrigLoop_;
       char velocity_;
       TableSaveState tableState_;
       bool first_[SONG_CHANNEL_COUNT];
       bool muted_[SONG_CHANNEL_COUNT];

       static MidiService *svc_;
} ;

#endif
