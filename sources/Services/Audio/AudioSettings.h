#ifndef _AUDIO_SETTINGS_H_
#define _AUDIO_SETTINGS_H_

#include <string>
// Used to propagate audio hints & settings

struct AudioSettings {
    std::string audioAPI_;
    std::string audioDevice_;
    int bufferSize_ = 0;
    int preBufferCount_ = 0;
    int sampleRate_ = 0;
};

#endif
