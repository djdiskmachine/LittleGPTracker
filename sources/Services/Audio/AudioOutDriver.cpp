
#include "AudioOutDriver.h"
#include "Application/Model/Project.h"
#include "Application/Player/SyncMaster.h" // Should be installable
#include "AudioDriver.h"
#include "Services/Time/TimeService.h"
#include "System/Console/Trace.h"
#include "System/System/System.h"
#include <math.h>

AudioOutDriver::AudioOutDriver(AudioDriver &driver) {
    driver_ = &driver;
    driver.AddObserver(*this) ;
    primarySoundBuffer_=0 ;
    mixBuffer_ = 0;
    SetOwnership(false);
}

AudioOutDriver::~AudioOutDriver() {
    driver_->RemoveObserver(*this);
    delete driver_ ;
};

bool AudioOutDriver::Init() {
	primarySoundBuffer_=(fixed *)SYS_MALLOC(MIX_BUFFER_SIZE*sizeof(fixed)/2) ;
	mixBuffer_=(short *)SYS_MALLOC(MIX_BUFFER_SIZE) ;
    return driver_->Init();
} ;

void AudioOutDriver::Close() {
    driver_->Close();
    SAFE_FREE(primarySoundBuffer_) ;
	SAFE_FREE(mixBuffer_) ;     
}

bool AudioOutDriver::Start() {
    sampleCount_=0 ;
	return driver_->Start() ;
}

void AudioOutDriver::Stop() {
	driver_->Stop() ;
}

void AudioOutDriver::Trigger() {

	TimeService *ts=TimeService::GetInstance() ;

    prepareMixBuffers();
    hasSound_=AudioMixer::Render(primarySoundBuffer_,sampleCount_) ;
    clipToMix();
    driver_->AddBuffer(mixBuffer_,sampleCount_) ;
}

void AudioOutDriver::Update(Observable &o,I_ObservableData *d) 
{
    SetChanged();
    NotifyObservers(d) ;
}

void AudioOutDriver::prepareMixBuffers() {
    sampleCount_ = getPlaySampleCount();
} ;

void AudioOutDriver::SetMasterVolume(int volume) {
    AudioMixer::SetMasterVolume(volume);
}

void AudioOutDriver::clipToMix() {

    bool interlaced = driver_->Interlaced();

    if (!hasSound_) {
        SYS_MEMSET(mixBuffer_, 0, sampleCount_ * 2 * sizeof(short));
    } else {
        short *s1 = mixBuffer_;
		short *s2 = (interlaced) ? s1 + 1 : s1 + sampleCount_;
		int offset = (interlaced) ? 2 : 1;

        fixed *p = primarySoundBuffer_;

        for (int i = 0; i < sampleCount_; i++) {

            fixed leftSample = *p++;
            fixed rightSample = *p++;

            *s1 = short(fp2i(leftSample));
            s1 += offset;
			*s2 = short(fp2i(rightSample));
			s2 += offset;
        };
    }
} ;

int AudioOutDriver::GetPlayedBufferPercentage() {
	return driver_->GetPlayedBufferPercentage() ;
} ;

AudioDriver *AudioOutDriver::GetDriver() { return driver_; };

std::string AudioOutDriver::GetAudioAPI() {
	AudioSettings as=driver_->GetAudioSettings() ;
	return as.audioAPI_ ;
} ;

std::string AudioOutDriver::GetAudioDevice() {
	AudioSettings as=driver_->GetAudioSettings() ;
	return as.audioDevice_ ;
} ;

int AudioOutDriver::GetAudioBufferSize() {
	AudioSettings as=driver_->GetAudioSettings() ;
	return as.bufferSize_ ;
} ;

int AudioOutDriver::GetAudioRequestedBufferSize() {
	AudioSettings as=driver_->GetAudioSettings() ;
	return as.bufferSize_ ;
}

int AudioOutDriver::GetAudioPreBufferCount() {
	AudioSettings as=driver_->GetAudioSettings() ;
	return as.preBufferCount_ ;
} ;
double AudioOutDriver::GetStreamTime() { return driver_->GetStreamTime(); };
