
#include "SDLAudio.h"
#include "SDLAudioDriver.h"
#include "Services/Audio/AudioOutDriver.h"

SDLAudio::SDLAudio(AudioSettings &hints) : Audio(hints) {
    hints_ = hints;
    drv_ = NULL;
}

SDLAudio::~SDLAudio() {}

void SDLAudio::Init() {
    AudioSettings settings;
    settings.audioAPI_ = GetAudioAPI();

    settings.bufferSize_ = GetAudioBufferSize();
    settings.preBufferCount_ = GetAudioPreBufferCount();
    settings.sampleRate_ = GetPreferredSampleRate();

    drv_ = new SDLAudioDriver(settings);
    AudioOut *out = new AudioOutDriver(*drv_);
    Insert(out);
};

void SDLAudio::Close() {
    IteratorPtr<AudioOut> it(GetIterator());
    for (it->Begin(); !it->IsDone(); it->Next()) {
        AudioOut &current = it->CurrentItem();
        current.Close();
    }
};

int SDLAudio::GetSampleRate() {
    if (!drv_) {
        Trace::Error(
            "AUDIO",
            "Sample rate requested before audio driver is initialised!");
    }
    return drv_->GetSampleRate();
};

int SDLAudio::GetMixerVolume() { return 100; };

void SDLAudio::SetMixerVolume(int volume) {};
