
#include "AudioOutDriver.h"
#include "Application/Model/Project.h"
#include "Application/Player/SyncMaster.h" // Should be installable
#include "AudioDriver.h"
#include "Services/Time/TimeService.h"
#include "System/Console/Trace.h"
#include "System/System/System.h"
#include <math.h>

AudioOutDriver::AudioOutDriver(AudioDriver &driver) {
    maxPositiveFixed_ = i2fp(32767);
    maxNegativeFixed_=i2fp(-32768);
	driver_=&driver ;
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
    clipped_ = false;
    sampleCount_=0 ;
	return driver_->Start() ;
}

void AudioOutDriver::Stop() {
	driver_->Stop() ;
}

bool AudioOutDriver::Clipped() { return clipped_; };

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
	sampleCount_=getPlaySampleCount() ; 
	clipped_=false ;   
} ;

void AudioOutDriver::SetSoftclip(int clip) { softclip_ = clip; }

fixed AudioOutDriver::hardClip(fixed sample) {
	fixed clippedSample=sample;

	if (sample>maxPositiveFixed_) {
		clippedSample=maxPositiveFixed_;
		clipped_=true;
	} else if (sample<maxNegativeFixed_) {
		clippedSample=maxNegativeFixed_;
		clipped_=true;
	}

	return clippedSample;
}

/* Implements standard cubic algorithm
 * https://wiki.analog.com/resources/tools-software/sigmastudio/toolbox/nonlinearprocessors/standardcubic
 */
fixed AudioOutDriver::softClip(fixed sample) {

	if (softclip_ == 0 || sample == 0) return sample;

	float sampleFloat=fp2fl(sample);
	fixed maxPositiveFloat=fp2fl(maxPositiveFixed_);
	fixed maxNegativeFloat=fp2fl(maxNegativeFixed_);
	float maxFloat;

	if (sampleFloat>0) {
		maxFloat=maxPositiveFloat;
	}
	else {
		maxFloat=maxNegativeFloat;
	}

	float x;
	float alpha;		// hardness

	switch (softclip_) {
		case 1:
			alpha=1.45; // -1.5db (approx.)
			break;
		case 2:
			alpha=1.07; // -3db (approx.)
			break;
		case 3:
			alpha=0.75; // -6db (approx.)
			break;
		case 4:
			alpha=0.53; // -9db (approx.)
			break;
		default:
			alpha=1.45;
	}

	float twoThirds=2.0/3.0;
	float alphaTwoThirds=alpha*twoThirds;
	float invertedAlpha=1.0/alpha;

	x=invertedAlpha*(sampleFloat/maxFloat);
	if (x>-1.0 && x<1.0) {
		sampleFloat=maxFloat*(alpha*(x-(pow(x, 3)/3.0)));
	} else {
		sampleFloat=maxFloat*alphaTwoThirds;
	}

	float gainCompensation;
	if (alpha > 1.0) {
		gainCompensation=1.0/(alpha*(invertedAlpha-pow(invertedAlpha, 3)/3.0));
	} else {
		gainCompensation=1.0/alphaTwoThirds;
	}

	return fl2fp(sampleFloat*gainCompensation);
}

void AudioOutDriver::clipToMix() {

	bool interlaced=driver_->Interlaced()  ;

	if (!hasSound_) {
		SYS_MEMSET(mixBuffer_,0,sampleCount_*2*sizeof(short)) ;
	} else {
		short *s1=mixBuffer_ ;
		short *s2=(interlaced)?s1+1:s1+sampleCount_ ;
		int offset=(interlaced)?2:1 ;

		fixed *p=primarySoundBuffer_ ;

        fixed leftSample;
        fixed rightSample;

        for (int i=0;i<sampleCount_;i++) {

			leftSample=hardClip(softClip(*p++));
			rightSample=hardClip(softClip(*p++));

            *s1=short(fp2i(leftSample));
			s1+=offset;
			*s2=short(fp2i(rightSample));
			s2+=offset;
		} ;
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
