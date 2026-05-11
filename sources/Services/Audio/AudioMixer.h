#ifndef _AUDIO_MIXER_H_
#define _AUDIO_MIXER_H_

#include "AudioModule.h"
#include "Foundation/T_SimpleList.h"
#include "Application/Instruments/WavFileWriter.h"
#include <string>

struct SoftClipData {
    float alpha;
	float alpha23;
	float alphaInv;
	float gainCmp;
};

class AudioMixer: public AudioModule,public T_SimpleList<AudioModule> {
public:
	AudioMixer(const char *name) ;
	virtual ~AudioMixer() ;
	virtual bool Render(fixed *buffer,int samplecount) ;
	void SetFileRenderer(const char *path) ;
	void EnableRendering(bool enable) ;
	void SetVolume(fixed volume) ;
    virtual void SetSoftclip(int clip, int gain);
    virtual void SetMasterVolume(int volume) ;
	virtual bool Clipped() ;
	
private:
  fixed hardClip(fixed sample);
  fixed softClip(fixed sample);
  bool enableRendering_;
  std::string renderPath_;
  WavFileWriter *writer_;
  fixed volume_;
  std::string name_;
  SoftClipData softClipData_[4];
  int softclip_;
  int softclipGain_;
  int masterVolume_;
  bool clipped_;
} ;
#endif
