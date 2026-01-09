#ifndef _AUDIO_OUT_DRIVER_H_
#define _AUDIO_OUT_DRIVER_H_

#include "AudioOut.h"
#include "Foundation/Observable.h"
#include "Application/Instruments/WavFileWriter.h"

class AudioDriver ;

#define MIX_BUFFER_SIZE 40000
#define MAX_POSITIVE_FIXED i2fp(32767)
#define MAX_NEGATIVE_FIXED i2fp(-32768)

class AudioOutDriver: public AudioOut,protected I_Observer {
  public:
    AudioOutDriver(AudioDriver &) ;
    virtual ~AudioOutDriver() ;

    virtual bool Init() ;
    virtual void Close() ;
    virtual bool Start() ;
    virtual void Stop() ;

    virtual void Trigger();
    virtual void SetMasterVolume(int volume);

    virtual int GetPlayedBufferPercentage() ;

    AudioDriver *GetDriver() ;

    virtual std::string GetAudioAPI() ;
    virtual std::string GetAudioDevice() ;
    virtual int GetAudioBufferSize() ;
    virtual int GetAudioRequestedBufferSize() ;
    virtual int GetAudioPreBufferCount() ;
    virtual double GetStreamTime() ;

  protected:

    virtual void Update(Observable &o,I_ObservableData *d) ;

    void prepareMixBuffers();
    void clipToMix();

  private:
    AudioDriver *driver_;
    bool hasSound_;

    fixed *primarySoundBuffer_ ;
    short *mixBuffer_ ;
    int sampleCount_ ;       
} ;
#endif
