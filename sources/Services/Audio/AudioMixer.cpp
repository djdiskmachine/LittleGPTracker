#include "AudioMixer.h"
#include "System/System/System.h"
#include <math.h>

#define MAX_POSITIVE_FIXED i2fp(32767)
#define MAX_NEGATIVE_FIXED i2fp(-32768)

AudioMixer::AudioMixer(const char *name):
	T_SimpleList<AudioModule>(false),
	enableRendering_(0),
	writer_(0),
	name_(name)
{
	volume_=(i2fp(1)) ;
    softclip_ = -1;
    softclipGain_ = 0 ;
	masterVolume_ = 100 ;
	clipped_ = false ;
	
	// Precalculate constant values for softclipping algorithm
	softClipData_[0].alpha = 1.45f; // -1.5db (approx.)
	softClipData_[1].alpha = 1.07f; // -3db (approx.)
	softClipData_[2].alpha = 0.75f; // -6db (approx.)
	softClipData_[3].alpha = 0.53f; // -9db (approx.)

	for (int i = 0; i < 4; i++) {
		softClipData_[i].alpha23 = softClipData_[i].alpha * (2.0f / 3.0f);
		softClipData_[i].alphaInv = 1.0f / softClipData_[i].alpha;

		if (softClipData_[i].alpha > 1.0f) {
			/* calculates gain compensation differently for
			 * modes with alpha > 1, so there's no drop in loudness
			 * and we can still drive the hard clipper when the input
			 * goes over 1.0
			 */
			softClipData_[i].gainCmp = 1.0f / (1.0f - (pow(softClipData_[i].alphaInv, 2.0f) / 3.0f));
		} else {
			softClipData_[i].gainCmp = 1.0f / softClipData_[i].alpha23;
		}
	}
} ;

AudioMixer::~AudioMixer() {
}

void AudioMixer::SetFileRenderer(const char *path) {
	renderPath_=path ;
} ;

void AudioMixer::EnableRendering(bool enable) {

	if (enable==enableRendering_) {
		return ;
	}

	if (enable) {
		writer_=new WavFileWriter(renderPath_.c_str()) ;
	} 

	enableRendering_=enable ;
	if (!enable) {
		writer_->Close() ;
		SAFE_DELETE(writer_) ;
	}
} ;

bool AudioMixer::Render(fixed *buffer,int samplecount) {
    clipped_ = false;

    fixed *mixBuffer = 0;
    bool gotData = false;
    IteratorPtr<AudioModule> it(GetIterator());
    for (it->Begin(); !it->IsDone(); it->Next()) {
        AudioModule &current = it->CurrentItem();
        if (!gotData) {
            gotData=current.Render(buffer,samplecount) ;           
         } else {
            if (!mixBuffer) {
               mixBuffer=(fixed *)malloc(samplecount*2*sizeof(fixed)) ;
            } 
            if (current.Render(mixBuffer,samplecount)) {
               fixed *dst=buffer ;
               fixed *src=mixBuffer ;
               int count=samplecount*2 ;
               while (count--) {
                 *dst+=*src ;
                 dst++ ;
                 src++ ;
               }
            }
         }
     }

     //  Apply volume

     if (gotData) {
         fixed *c = buffer;
         float damp = pow((float)masterVolume_ / 100, 4.0f);

         if (volume_ != i2fp(1)) {
             for (int i = 0; i < samplecount * 2; i++) {
                 fixed v = fp_mul(*c, volume_);
                 *c++ = v;
             }
         }

         // Apply soft/hard clipping before recording
         c = buffer;
         for (int i = 0; i < samplecount * 2; i++) {
             fixed sample = *c;
             sample = fl2fp(damp * fp2fl(hardClip(softClip(sample))));
             *c++ = sample;
         }
     }
    if (enableRendering_&&writer_) {
		if (!gotData) {
			memset(buffer,0,samplecount*2*sizeof(fixed)) ;
		} ;
		writer_->AddBuffer(buffer,samplecount) ;
	}
     SAFE_FREE(mixBuffer) ;
     return gotData ;
} ;

void AudioMixer::SetVolume(fixed volume) { volume_ = volume; }

void AudioMixer::SetSoftclip(int clip, int gain) {
    softclip_ = clip - 1;
	softclipGain_ = gain;
}

void AudioMixer::SetMasterVolume(int volume) {
	masterVolume_ = volume;
}

bool AudioMixer::Clipped() { return clipped_; }

fixed AudioMixer::hardClip(fixed sample) {
    if (sample > MAX_POSITIVE_FIXED || sample < MAX_NEGATIVE_FIXED) {
        clipped_ = true;
		return sample > 0 ? MAX_POSITIVE_FIXED : MAX_NEGATIVE_FIXED;
    }
    return sample;
}

/* Implements standard cubic algorithm
 * https://wiki.analog.com/resources/tools-software/sigmastudio/toolbox/nonlinearprocessors/standardcubic
 */
fixed AudioMixer::softClip(fixed sample) {
    if (softclip_ == -1 || sample == 0)
        return sample;

    float x;
    float sampleFloat = fp2fl(sample);
	float maxFloat = fp2fl(sampleFloat > 0 ? MAX_POSITIVE_FIXED : MAX_NEGATIVE_FIXED);
	SoftClipData* data = &softClipData_[softclip_];

    x = data->alphaInv * (sampleFloat / maxFloat);
    if (x > -1.0f && x < 1.0f) {
        sampleFloat = maxFloat * (data->alpha * (x - (pow(x, 3.0f) / 3.0f)));
    } else {
        sampleFloat = maxFloat * data->alpha23;
    }

    if (softclipGain_) {
        sampleFloat = sampleFloat * data->gainCmp;
    }

    return fl2fp(sampleFloat);
}
