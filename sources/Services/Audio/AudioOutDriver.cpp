
#include "AudioOutDriver.h"
#include "System/System/System.h"
#include "AudioDriver.h"
#include "Application/Model/Project.h"
#include "Application/Player/SyncMaster.h" // Should be installable
#include "System/Console/Trace.h"
#include "Services/Time/TimeService.h"
#include <math.h> 

AudioOutDriver::AudioOutDriver(AudioDriver &driver) {
    driver_=&driver ;
    driver.AddObserver(*this) ;
	primarySoundBuffer_=0 ;
	mixBuffer_=0 ;        
    SetOwnership(false) ;                       
}

AudioOutDriver::~AudioOutDriver() {
    driver_->RemoveObserver(*this) ;
    delete driver_ ;
};

bool AudioOutDriver::Init() {
	primarySoundBuffer_=(fixed *)SYS_MALLOC(MIX_BUFFER_SIZE*sizeof(fixed)/2) ;
	mixBuffer_=(short *)SYS_MALLOC(MIX_BUFFER_SIZE) ;
    return driver_->Init() ;     
} ;

void AudioOutDriver::Close() {
     driver_->Close() ;
	SAFE_FREE(primarySoundBuffer_) ;
	SAFE_FREE(mixBuffer_) ;     
}

bool AudioOutDriver::Start() {
     clipped_=false ;
     sampleCount_=0 ;
     return driver_->Start() ;
}

void AudioOutDriver::Stop() {
	driver_->Stop() ;
}

bool AudioOutDriver::Clipped() {
     return clipped_ ;
} ;

void AudioOutDriver::Trigger() {

	TimeService *ts=TimeService::GetInstance() ;

  prepareMixBuffers() ;
  hasSound_=AudioMixer::Render(primarySoundBuffer_,sampleCount_) ;
  clipToMix() ;
  driver_->AddBuffer(mixBuffer_,sampleCount_) ;
}

void AudioOutDriver::Update(Observable &o,I_ObservableData *d) 
{
  SetChanged() ;
  NotifyObservers(d) ;    
}

void AudioOutDriver::prepareMixBuffers() {
	sampleCount_=getPlaySampleCount() ; 
	clipped_=false ;   
} ;

void AudioOutDriver::SetSoftclip(int clip) {
  if (clip == 1) {
    softclip_ = true;
  } else {
    softclip_ = false;
  }
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
        
		fixed v;
		fixed f_32767=i2fp(32767);
		fixed f_m32768=i2fp(-32768);
		fixed fl_32767=fp2fl(f_32767);
		fixed fl_m32768=fp2fl(f_m32768);

    float x;
    float alpha=1;
    float twoThirds=2.0/3.0;
    float gainCompensation=1.0/(alpha*twoThirds)-0.05;

		for (int i=0;i<sampleCount_;i++) {

      if (softclip_ == 0) {
        // Left
        v=*p++ ;
        if (v>f_32767) {
          v=f_32767;
        } else if (v<f_m32768) {
          v=f_m32768;
        }
        *s1=short(fp2i(v));
        s1+=offset;

        // Right
        v=*p++ ;
        if (v>f_32767) {
          v=f_32767;
          clipped_=true;
        } else if (v<f_m32768) {
          v=f_m32768;
          clipped_=true;
        }

        *s2=short(fp2i(v));
        s2+=offset;

      } else {
        // Left
        v=*p++;
        if (v>0) {
          x=(1/alpha)*(fp2fl(v)/fl_32767);
          if (x>=1) {
            v=fl_32767*(alpha*(twoThirds));
            clipped_=true;
          } else {
            v=fl_32767*(alpha*(x-(pow(x, 3)/3.0)));
          }
        }
        else if (v<0) {
          x=-((1/alpha)*(fp2fl(v)/fl_m32768));
          if (x<=-1) {
            v=fl_m32768*(alpha*(twoThirds));
            clipped_=true;
          } else {
            v=fl_m32768*-(alpha*(x-(pow(x, 3)/3.0)));
          }
        }
        *s1=short(fp2i(fl2fp(v*gainCompensation)));
        s1+=offset;

        // Right
        v=*p++;
        if (v>0) {
          x=(1/alpha)*(fp2fl(v)/fl_32767);
          if (x>=1) {
            v=fl_32767*(alpha*(twoThirds));
            clipped_=true;
          } else {
            v=fl_32767*(alpha*(x-(pow(x, 3)/3.0)));
          }
        }
        else if (v<0) {
          x=-((1/alpha)*(fp2fl(v)/fl_m32768));
          if (x<=-1) {
            v=fl_m32768*(alpha*(twoThirds));
            clipped_=true;
          } else {
            v=fl_m32768*-(alpha*(x-(pow(x, 3)/3.0)));
          }
        }
        *s2=short(fp2i(fl2fp(v*gainCompensation)));
        s2+=offset;

      }
    } ;
	}     
} ;

int AudioOutDriver::GetPlayedBufferPercentage() {
	return driver_->GetPlayedBufferPercentage() ;
} ;

AudioDriver *AudioOutDriver::GetDriver() {
    return driver_ ;
} ;

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
double AudioOutDriver::GetStreamTime() {
	return driver_->GetStreamTime() ;
} ;
