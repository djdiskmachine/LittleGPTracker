
#include "RTMidiOutDevice.h"
#include "System/Console/Trace.h"


RTMidiOutDevice::RTMidiOutDevice(RtMidiOut &out,int index,const char *name):
	MidiOutDevice(name),
	rtMidiOut_(out),
	index_(index),
	running_(false)
{
} ;

RTMidiOutDevice::~RTMidiOutDevice() {
} ;

bool RTMidiOutDevice::Init(){
	return true ;
} ;

void RTMidiOutDevice::Close(){
}  ;

bool RTMidiOutDevice::Start(){
	// RtMidi signals failure by throwing, and the port list this index came
	// from was built when the project was opened: the interface may have been
	// unplugged since.  Failing to open is reported to the caller rather than
	// taking the tracker down with it.
	try {
		rtMidiOut_.openPort( index_ );
	} catch (RtError &error) {
		Trace::Log("RTMidiOutDevice","Could not open %s: %s",GetName(),error.getMessageString()) ;
		return false ;
	}
	running_=true ;
	return true ;
}  ;

void RTMidiOutDevice::Stop(){
	running_=false ;
	try {
		rtMidiOut_.closePort() ;
	} catch (RtError &error) {
		Trace::Log("RTMidiOutDevice","Could not close %s: %s",GetName(),error.getMessageString()) ;
	}
} ;

void RTMidiOutDevice::SendMessage(MidiMessage &msg)
{
  if (running_)
  {
    std::vector<unsigned char> message;
    message.push_back(msg.status_) ;

    if (msg.data1_ != MidiMessage::UNUSED_BYTE)
    {
      message.push_back(msg.data1_) ;
    }

    if (msg.data2_ != MidiMessage::UNUSED_BYTE)
    {
      message.push_back(msg.data2_) ;
    }

    // Same reason as Start(): an interface unplugged mid-song must not take
    // the audio thread down with it.
    try {
      rtMidiOut_.sendMessage( &message );
    } catch (RtError &error) {
      Trace::Log("RTMidiOutDevice","Could not send to %s: %s",GetName(),error.getMessageString()) ;
      running_=false ;
    }
  }
}
