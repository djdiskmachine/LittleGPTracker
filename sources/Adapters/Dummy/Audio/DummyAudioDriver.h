#ifndef _DUMMY_AUDIODRIVER_H_
#define _DUMMY_AUDIODRIVER_H_

#include "Services/Audio/AudioDriver.h"

class DummyAudioDriver: public AudioDriver {
  public:
    DummyAudioDriver(AudioSettings &settings);
    // Sound implementation
    virtual void CloseDriver();
    virtual int GetPlayedBufferPercentage();
    virtual int GetSampleRate();
    virtual double GetStreamTime();
    virtual bool InitDriver() ; 
    virtual bool Interlaced();
    virtual bool StartDriver() ; 
    virtual void StopDriver();
};
#endif
