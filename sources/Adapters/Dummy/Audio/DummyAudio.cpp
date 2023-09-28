#include "DummyAudio.h"
#include "DummyAudioDriver.h"
#include "Services/Audio/AudioOutDriver.h"


DummyAudio::DummyAudio(AudioSettings &hints):Audio(hints) {
  hints_ = hints;
};


DummyAudio::~DummyAudio() {};


void DummyAudio::Close() {
  IteratorPtr<AudioOut> it(GetIterator());
  for (it -> Begin(); !it -> IsDone(); it -> Next()) {
    AudioOut &current = it -> CurrentItem();
    current.Close();
  }
};


int DummyAudio::GetMixerVolume() {
	return 100;
};


void DummyAudio::Init() {
  DummyAudioDriver* drv = new DummyAudioDriver(hints_);
  AudioOut *out = new AudioOutDriver(*drv);
  Insert(out);
};


void DummyAudio::SetMixerVolume(int volume) {};
