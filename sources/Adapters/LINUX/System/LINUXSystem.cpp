#include "LINUXSystem.h"
#include <libgen.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "Adapters/SDL2/GUI/GUIFactory.h"
#include "Adapters/SDL2/GUI/SDLEventManager.h"
#include "Adapters/SDL2/GUI/SDLGUIWindowImp.h"
#include "Adapters/SDL2/Timer/SDLTimer.h"
#include "Adapters/Unix/FileSystem/UnixFileSystem.h"
#include "Adapters/Unix/Process/UnixProcess.h"
#include "Application/Controllers/ControlRoom.h"
#include "Application/Commands/NodeList.h"
#include "Application/Model/Config.h"
#include "System/Console/Logger.h"

#ifdef DUMMYMIDI
#include "Adapters/Dummy/Midi/DummyMidi.h"
#endif

#ifdef JACKAUDIO
#include "Adapters/Jack/Audio/JackAudio.h"
#include "Adapters/Jack/Client/JackClient.h"
#endif

#ifdef JACKMIDI
#include "Adapters/Jack/Midi/JackMidiService.h"
#endif

#ifdef RTAUDIO
#include "Adapters/RTAudio/RTAudioStub.h"
#endif

#ifdef RTMIDI
#include "Adapters/RTMidi/RTMidiService.h"
#endif

#ifdef SDLAUDIO
#include "Adapters/SDL2/Audio/SDLAudio.h"
#endif

EventManager *LINUXSystem::eventManager_ = NULL;
static int secbase = 0;

/*
 * starts the main loop
 */
int LINUXSystem::MainLoop() {
	eventManager_->InstallMappings();
	return eventManager_->MainLoop() ;
};

/*
 * initializes the application
 */
void LINUXSystem::Boot(int argc,char **argv) {

    SDL_setenv((char *)"SDL_VIDEO_X11_WMCLASS",(char *)"LittleGPTracker",1) ;

	// Install System
	System::Install(new LINUXSystem());

	// Install FileSystem
	FileSystem::Install(new UnixFileSystem());

  // Install aliases
	char buff[1024];
	ssize_t len = ::readlink("/proc/self/exe",buff,sizeof(buff)-1);
	if (len != -1)
	{
		buff[len] = 0;
	}
	else
	{
		strcpy(buff,".");
	}
	Path::SetAlias("bin",dirname(buff)) ;
	Path::SetAlias("root",".") ;

  // always use stdout, user can capture in launch script
  Trace::GetInstance()->SetLogger(*(new StdOutLogger()));

  // Process arguments
  Config::GetInstance()->ProcessArguments(argc,argv) ;

  // Install GUI Factory
  I_GUIWindowFactory::Install(new GUIFactory()) ;

  // Install Timers
  TimerService::GetInstance()->Install(new SDLTimerService()) ;

#ifdef JACKAUDIO
	Trace::Log("System","Installing JACK audio") ;
	Audio::Install(new JackAudio(AudioSettings hints));
#endif

#ifdef RTAUDIO
	Trace::Log("System","Installing RT audio") ;
	AudioSettings hints ;
	hints.bufferSize_= 256 ;
	hints.preBufferCount_=2 ;
	Audio::Install(new RTAudioStub(hints)) ;
#endif

#ifdef SDLAUDIO
	Trace::Log("System","Installing SDL audio") ;
	AudioSettings hint;
	hint.bufferSize_ = 1024;
	hint.preBufferCount_ = 8;
	Audio::Install(new SDLAudio(hint));
#endif

#ifdef DUMMYMIDI
	Trace::Log("System","Installing DUMMY MIDI") ;
	MidiService::Install(new DummyMidi());
#endif

#ifdef JACKMIDI
	Trace::Log("System","Installing JACK MIDI") ;
  MidiService::Install(new JackMidiService()) ;
#endif

#ifdef RTMIDI
	Trace::Log("System","Installing RT MIDI") ;
	MidiService::Install(new RTMidiService()) ;
#endif

	// Install Threads
	SysProcessFactory::Install(new UnixProcessFactory());

	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0 ) {
		return;
	}
	SDL_ShowCursor(SDL_DISABLE);

	atexit(SDL_Quit);

	eventManager_ = I_GUIWindowFactory::GetInstance()->GetEventManager();
	eventManager_ -> Init();
};

void LINUXSystem::Shutdown() {};

/*
 * get current time for status display
 */
unsigned long LINUXSystem::GetClock() {
	struct timeval tp;

	gettimeofday(&tp, NULL);
	if (!secbase) {
		secbase = tp.tv_sec;
		return long(tp.tv_usec/1000.0);
	}
	return long((tp.tv_sec - secbase)*1000 + tp.tv_usec/1000.0);
}

/*
 * wraps sleep, guess we never sleep!
 */
void LINUXSystem::Sleep(int millisec) {
	//if (millisec>0)
	//::Sleep(millisec) ;
}

/*
 * wraps malloc
 */
void *LINUXSystem::Malloc(unsigned size) {
	void *ptr=malloc(size) ;
	//Trace::Debug("alloc:%x  (%d)",ptr,size) ;
	return ptr ;
}

/*
 * wraps free
 */
void LINUXSystem::Free(void *ptr) {
	free(ptr);
} 

/*
 * wraps memset
 */
void LINUXSystem::Memset(void *addr,char val,int size) {
#ifdef _64BIT
    unsigned int ad = (intptr_t)addr;
#else
    unsigned int ad=(unsigned int)addr;
#endif
    if (((ad&0x3)==0)&&((size&0x3)==0)) { // Are we 4-byte aligned ?
        unsigned int intVal=0;
        for (int i=0;i<4;i++) {
             intVal=(intVal<<8)+val;
        }
        unsigned int *dst=(unsigned int *)addr;
        size_t intSize=size>>2 ;

        for (unsigned int i=0;i<intSize;i++) {
            *dst++=intVal ;
        }
    } else {
        memset(addr,val,size) ;
    };
};

/*
 * wraps memcpy
 */
void *LINUXSystem::Memcpy(void *s1, const void *s2, int n) {
    return memcpy(s1,s2,n) ;
};

/*
 * logprint
 */
void LINUXSystem::AddUserLog(const char *msg) {
	fprintf(stderr,"LOG: %s\n",msg) ;
};

/*
 * print after quit
 */
void LINUXSystem::PostQuitMessage() {
	SDLEventManager::GetInstance()->PostQuitMessage() ;
}; 

/*
 * get memory usage, guess it's infinite
 */
unsigned int LINUXSystem::GetMemoryUsage() { return 0; };
