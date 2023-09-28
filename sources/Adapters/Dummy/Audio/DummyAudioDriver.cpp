#include "DummyAudioDriver.h"


DummyAudioDriver::DummyAudioDriver(AudioSettings &settings):AudioDriver(settings) {};


void DummyAudioDriver::CloseDriver() {};


int DummyAudioDriver::GetPlayedBufferPercentage() {
  return 0;
};


int DummyAudioDriver::GetSampleRate() {
  return 44100;
};


double DummyAudioDriver::GetStreamTime() {
  return 0.0;
};


bool DummyAudioDriver::InitDriver() {
   return true ;
}; 


bool DummyAudioDriver::Interlaced() {
  return true;
};


bool DummyAudioDriver::StartDriver() {
  return 1;
}; 

void DummyAudioDriver::StopDriver() {};
