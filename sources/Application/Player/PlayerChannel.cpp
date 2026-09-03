
#include "PlayerChannel.h"
#include "Application/Mixer/MixerService.h"
#include "Application/Model/Mixer.h"
#include "Application/Player/SyncMaster.h"
#include "Application/Utils/fixed.h"
#include <math.h>

PlayerChannel::PlayerChannel(int index) {             
    index_=index ;
    instr_=0 ;
    muted_=false ;
	mixBus_=0 ;
	busIndex_=-1 ;
    volume_ = i2fp(1);
    hpfPrevInput_[0] = hpfPrevInput_[1] = i2fp(0);
    hpfPrevOutput_[0] = hpfPrevOutput_[1] = i2fp(0);
    hpfAlpha_ = i2fp(0);
    hpfMode_ = 0;
    lpfPrevOutput_[0] = lpfPrevOutput_[1] = i2fp(0);
    lpfAlpha_ = i2fp(0);
    lpfFreq_ = 0;
}

PlayerChannel::~PlayerChannel() {
}

void PlayerChannel::StartInstrument(I_Instrument *instr,unsigned char note,bool trigger) {
   if (instr_) {
      StopInstrument() ;
   }
   /* The note will not trigger if it is out the keymap or, in the case of a
      MIDI instrument, if the channel is muted. */
   if (instr->Start(index_,note, trigger | (muted_ << 1))) {
	   instr_=instr ;
   } else {
	   instr_=0 ;
   };
} ;

void PlayerChannel::StopInstrument() {
     if (instr_) {
       instr_->Stop(index_) ;
     }
     instr_=0 ;
} ;

bool PlayerChannel::Render(fixed *buffer,int samplecount) {
   if (instr_) {
     bool tableSlice=SyncMaster::GetInstance()->TableSlice() ;
     bool status = instr_->Render(index_, buffer, samplecount,
                                  tableSlice | (muted_ << 1));
     if (status && !muted_) {
         // Apply HPF if enabled
         if (hpfMode_ != 0) {
             for (int n = 0; n < samplecount; n++) {
                 int idx = n * 2;
                 fixed in_l = buffer[idx];
                 fixed in_r = buffer[idx + 1];
                 fixed out_l =
                     fp_mul(hpfAlpha_, fp_add(hpfPrevOutput_[0],
                                              fp_sub(in_l, hpfPrevInput_[0])));
                 fixed out_r =
                     fp_mul(hpfAlpha_, fp_add(hpfPrevOutput_[1],
                                              fp_sub(in_r, hpfPrevInput_[1])));
                 buffer[idx] = out_l;
                 buffer[idx + 1] = out_r;
                 hpfPrevInput_[0] = in_l;
                 hpfPrevInput_[1] = in_r;
                 hpfPrevOutput_[0] = out_l;
                 hpfPrevOutput_[1] = out_r;
             }
         }

         // Apply LPF if enabled
         if (lpfFreq_ != 0) {
             fixed one_minus_alpha = fp_sub(i2fp(1), lpfAlpha_);
             for (int n = 0; n < samplecount; n++) {
                 int idx = n * 2;
                 fixed in_l = buffer[idx];
                 fixed in_r = buffer[idx + 1];
                 fixed out_l = fp_add(fp_mul(lpfAlpha_, in_l),
                                      fp_mul(one_minus_alpha, lpfPrevOutput_[0]));
                 fixed out_r = fp_add(fp_mul(lpfAlpha_, in_r),
                                      fp_mul(one_minus_alpha, lpfPrevOutput_[1]));
                 buffer[idx] = out_l;
                 buffer[idx + 1] = out_r;
                 lpfPrevOutput_[0] = out_l;
                 lpfPrevOutput_[1] = out_r;
             }
         }

         // Apply per-channel volume
         if (volume_ != i2fp(1)) {
             for (int i = 0; i < samplecount * 2; i++) {
                 buffer[i] = fp_mul(buffer[i], volume_);
             }
         }
     }
     return (status && !muted_);
   } else {
       return false;
   }
} ;

I_Instrument *PlayerChannel::GetInstrument() {
   return instr_ ;
} ;

void PlayerChannel::SetMute(bool muted) {
     muted_=muted ;
}

bool PlayerChannel::IsMuted() { return muted_; }

void PlayerChannel::SetVolume(fixed volume) { volume_ = volume; };

void PlayerChannel::SetHPFMode(unsigned char mode) {
    if (hpfMode_ == mode)
        return;
    hpfMode_ = mode;
    // reset state
    hpfPrevInput_[0] = hpfPrevInput_[1] = i2fp(0);
    hpfPrevOutput_[0] = hpfPrevOutput_[1] = i2fp(0);
    if (hpfMode_ == 0) {
        hpfAlpha_ = i2fp(0);
        return;
    }
    // compute alpha for one-pole HPF: alpha = RC/(RC+dt), RC=1/(2*pi*fc),
    // dt=1/fs
    float fc = (hpfMode_ == 1) ? 20.0f : 90.0f;
    float fs = 44100.0f;
    const float PI = 3.14159265358979323846f;
    float RC = 1.0f / (2.0f * PI * fc);
    float dt = 1.0f / fs;
    float alpha = RC / (RC + dt);
    hpfAlpha_ = fl2fp(alpha);
}

void PlayerChannel::SetLPFFreq(unsigned short freq) {
    if (lpfFreq_ == freq)
        return;
    lpfFreq_ = freq;
    lpfPrevOutput_[0] = lpfPrevOutput_[1] = i2fp(0);
    if (lpfFreq_ == 0) {
        lpfAlpha_ = i2fp(0);
        return;
    }
    // compute alpha for one-pole LPF: alpha = dt/(RC+dt), RC=1/(2*pi*fc),
    // dt=1/fs
    float fc = (float)lpfFreq_;
    float fs = 44100.0f;
    const float PI = 3.14159265358979323846f;
    float RC = 1.0f / (2.0f * PI * fc);
    float dt = 1.0f / fs;
    float alpha = dt / (RC + dt);
    lpfAlpha_ = fl2fp(alpha);
}

void PlayerChannel::SetMixBus(int i) {

	if (i==busIndex_) return ;

	if (mixBus_) {
		mixBus_->Remove(*this) ;
	}
    busIndex_ = i;
    mixBus_=MixerService::GetInstance()->GetMixBus(i) ;
	if (mixBus_) {
		mixBus_->Insert(*this) ;
	}
} ;

void PlayerChannel::Reset() {
    if (mixBus_) {
        mixBus_->Remove(*this) ;
    }
    muted_=false ;
  busIndex_=-1 ;
  hpfPrevInput_[0]=hpfPrevInput_[1]=i2fp(0);
  hpfPrevOutput_[0]=hpfPrevOutput_[1]=i2fp(0);
  hpfAlpha_ = i2fp(0);
  hpfMode_ = 0;
  lpfPrevOutput_[0] = lpfPrevOutput_[1] = i2fp(0);
  lpfAlpha_ = i2fp(0);
  lpfFreq_ = 0;
};
