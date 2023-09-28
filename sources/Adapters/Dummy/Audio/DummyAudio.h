#ifndef _DUMMYAUDIO_H_
#define _DUMMYAUDIO_H_

#include "Services/Audio/Audio.h"
#include "DummyAudioDriver.h"


class DummyAudio: public Audio {
  public:
    DummyAudio(AudioSettings &hints);
    ~DummyAudio();
    virtual void Init();
    virtual void Close();
    virtual int GetMixerVolume();
    virtual void SetMixerVolume(int volume);
  private:
    DummyAudioDriver *drv;	
    AudioSettings hints_;
};
#endif
