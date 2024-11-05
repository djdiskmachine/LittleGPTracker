
#include "VITASystem.h"
#include "Adapters/Dummy/Midi/DummyMidi.h"
#include "Adapters/SDL2/Audio/SDLAudio.h"
#include "Adapters/SDL2/GUI/SDLEventManager.h"
#include "Adapters/SDL2/GUI/GUIFactory.h"
#include "Adapters/SDL2/GUI/SDLGUIWindowImp.h"
#include "Adapters/SDL2/Process/SDLProcess.h"
#include "Adapters/VITA/FileSystem/VITAFileSystem.h"
#include "Adapters/SDL2/Timer/SDLTimer.h"
#include "Application/Model/Config.h"
#include "System/Console/Logger.h"
#include <time.h>
#include <sys/time.h>
#include <malloc.h>
#include <stdlib.h>

#include <psp2/kernel/clib.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/power.h>

EventManager *VITASystem::eventManager_ = NULL ;

int VITASystem::MainLoop() 
{
	eventManager_->InstallMappings() ;
	return eventManager_->MainLoop() ;
} ;

void VITASystem::Boot(int argc,char **argv) {
	// Install System
	System::Install(new VITASystem()) ;

	// Install FileSystem
	FileSystem::Install(new VITAFileSystem()) ;
	FileSystem::GetInstance()->MakeDir("ux0:/data/lgpt/");

	Path::SetAlias("bin","ux0:/data/lgpt/") ;
	Path::SetAlias("root","ux0:/data/lgpt/") ;

	Config::GetInstance()->ProcessArguments(argc,argv) ;

	Path logPath("bin:lgpt.log");
	FileLogger *fileLogger=new FileLogger(logPath);

	if(fileLogger->Init().Succeeded())
	{
		Trace::GetInstance()->SetLogger(*fileLogger);    
	}
	
	// Install GUI Factory
	I_GUIWindowFactory::Install(new GUIFactory()) ;

	// Install Timers
	TimerService::GetInstance()->Install(new SDLTimerService()) ;

	// Install Sound
	AudioSettings hints ;
	hints.bufferSize_ = 1024 ;
	hints.preBufferCount_ = 8 ;
	Audio::Install(new SDLAudio(hints)) ;

	// Install Midi
	MidiService::Install(new DummyMidi()) ;

	// Install Threads

	SysProcessFactory::Install(new SDLProcessFactory()) ;

	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK|SDL_INIT_TIMER) < 0 )   {
		return;
	}
#ifndef SDL2
    SDL_EnableUNICODE(1);
#endif
    SDL_ShowCursor(SDL_DISABLE);

	atexit(SDL_Quit);

	eventManager_=I_GUIWindowFactory::GetInstance()->GetEventManager() ;
	eventManager_->Init() ;

	// VITA SDL Basic config

	bool invert=false ;
	Config *config=Config::GetInstance() ;
	const char *s=config->GetValue("INVERT") ;

	if ((s)&&(!strcmp(s,"YES"))) {
		invert=true ;
	}

	if (!invert) {
		eventManager_->MapAppButton("but:0:1",APP_BUTTON_B) ;
		eventManager_->MapAppButton("but:0:2",APP_BUTTON_A) ;
	}else {
		eventManager_->MapAppButton("but:0:1",APP_BUTTON_A) ;
		eventManager_->MapAppButton("but:0:2",APP_BUTTON_B) ;
	}
	eventManager_->MapAppButton("but:0:7",APP_BUTTON_LEFT) ;
	eventManager_->MapAppButton("but:0:9",APP_BUTTON_RIGHT) ;
	eventManager_->MapAppButton("but:0:8",APP_BUTTON_UP) ;
	eventManager_->MapAppButton("but:0:6",APP_BUTTON_DOWN) ;
	eventManager_->MapAppButton("but:0:4",APP_BUTTON_L) ;
	eventManager_->MapAppButton("but:0:5",APP_BUTTON_R) ;
	eventManager_->MapAppButton("but:0:11",APP_BUTTON_START) ;
} ;

void VITASystem::Shutdown() {
} ;

unsigned long VITASystem::GetClock() {
	struct timeval now;
	Uint32 ticks;
	gettimeofday(&now, NULL);
	ticks=(now.tv_sec)*1000+(now.tv_usec)/1000;
	return(ticks);
}

void VITASystem::Sleep(int millisec) {
}

void *VITASystem::Malloc(unsigned size) {
	return malloc(size) ;
}

void VITASystem::Free(void *ptr) {
	free(ptr) ;
} 

void VITASystem::Memset(void *addr,char val,int size) {
    memset(addr,val,size) ;
} ;

void *VITASystem::Memcpy(void *s1, const void *s2, int n) {
    return memcpy(s1,s2,n) ;
} ;

void VITASystem::PostQuitMessage() {
	SDLEventManager::GetInstance()->PostQuitMessage() ;
} ;

unsigned int VITASystem::GetMemoryUsage() {
	struct mallinfo m=mallinfo();
	return m.uordblks ;
}

int VITASystem::GetBatteryLevel() {
	if (sceKernelGetModel() == SCE_KERNEL_MODEL_VITA)
	{
		return scePowerGetBatteryLifePercent() ;
	}
	return -1 ;
} ;
