#include "WSDLSystem.h"
#include "Adapters/SDL2/GUI/GUIFactory.h"
#include "Adapters/SDL2/GUI/SDLEventManager.h"
#include "Adapters/SDL2/GUI/SDLGUIWindowImp.h"
#include "Adapters/SDL2/Timer/SDLTimer.h"
#include "Adapters/W32FileSystem/W32FileSystem.h"
#include "Adapters/W32/Process/W32Process.h"
#include "Application/Model/Config.h"
#include "System/Console/Logger.h"
#include <SDL2/SDL.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#ifdef SDLAUDIO
#include "Adapters/SDL2/Audio/SDLAudio.h"
#endif

#ifdef RTAUDIO
#include "Adapters/RTAudio/RTAudioStub.h"
#endif

#ifdef RTMIDI
#include "Adapters/RTMidi/RTMidiService.h"
#endif

EventManager *WSDLSystem::eventManager_ = NULL ;

int WSDLSystem::MainLoop() {
	eventManager_->InstallMappings() ;
	return eventManager_->MainLoop() ;
} ;

void WSDLSystem::Boot(int argc,char **argv) {

	// Install System
	System::Install(new WSDLSystem()) ;

	// Install FileSystem
	FileSystem::Install(new W32FileSystem()) ;

	// Get application directory & install platform specific aliases
	HMODULE module = GetModuleHandle(NULL);
	char temp_path[MAX_PATH];
	int length = GetModuleFileName(module,temp_path,MAX_PATH);
	int n = (int)strlen(temp_path)-1;
	while (temp_path[n] !='\\')
		n--;
	if (n<3)
		n=3;

	temp_path[n]=0;

	Path::SetAlias("bin",temp_path) ;
	Path::SetAlias("root","bin:..") ;

	// Tracing
#ifdef _DEBUG
	Trace::GetInstance()->SetLogger(*(new StdOutLogger()));
#else
	Path logPath("bin:lgpt.log");
	FileLogger *fileLogger=new FileLogger(logPath);
	if(fileLogger->Init().Succeeded())
	{
		Trace::GetInstance()->SetLogger(*fileLogger);
	}
#endif

	// Process arguments
	Config::GetInstance()->ProcessArguments(argc,argv) ;

	// Install GUI Factory
	I_GUIWindowFactory::Install(new GUIFactory()) ;

	// Install Timers
	TimerService::GetInstance()->Install(new SDLTimerService()) ;

#ifdef SDLAUDIO
	Trace::Log("System","Installing SDL audio") ;
	AudioSettings hint;
	hint.bufferSize_ = 1024;
	hint.preBufferCount_ = 8;
	Audio::Install(new SDLAudio(hint));
#endif

#ifdef RTAUDIO
	Trace::Log("System","Installing RT audio") ;
	AudioSettings hints ;
	hints.bufferSize_ = 512 ;
	hints.preBufferCount_ = 10 ;
	Audio::Install(new RTAudioStub(hints)) ;
#endif

#ifdef RTMIDI
	Trace::Log("System","Installing RT MIDI") ;
	MidiService::Install(new RTMidiService()) ;
#endif

	// Install Threads
	SysProcessFactory::Install(new W32ProcessFactory()) ;

	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_JOYSTICK) < 0 )   {
		return;
	}
	SDL_ShowCursor(SDL_DISABLE);

	atexit(SDL_Quit);

	eventManager_=I_GUIWindowFactory::GetInstance()->GetEventManager() ;
	eventManager_->Init() ;
} ;

void WSDLSystem::Shutdown() {
	delete Audio::GetInstance() ;
} ;

unsigned long WSDLSystem::GetClock() {
	return (clock()*1000)/CLOCKS_PER_SEC ;
}

void WSDLSystem::Sleep(int millisec) {
	if (millisec>0)
		::Sleep(millisec) ;
}

void *WSDLSystem::Malloc(unsigned size) {
	return malloc(size) ;
}

void WSDLSystem::Free(void *ptr) {
	free(ptr) ;
}

void WSDLSystem::Memset(void *addr,char val,int size) {

	unsigned int ad=(unsigned int)addr ;
	if (((ad&0x3)==0)&&((size&0x3)==0)) { // Are we 4-byte aligned ?
		unsigned int intVal=0 ;
		for (int i=0;i<4;i++) {
			intVal=(intVal<<8)+val ;
		}
		unsigned int *dst=(unsigned int *)addr ;
		size_t intSize=size>>2 ;

		for (unsigned int i=0;i<intSize;i++) {
			*dst++=intVal ;
		}
	} else {
		memset(addr,val,size) ;
	} ;
} ;

void *WSDLSystem::Memcpy(void *s1, const void *s2, int n)
{
	return memcpy(s1,s2,n) ;
} ;

void WSDLSystem::PostQuitMessage()
{
	SDLEventManager::GetInstance()->PostQuitMessage()  ;
} ;

unsigned int  WSDLSystem::GetMemoryUsage()
{
	return 0 ;
} ;

std::string WSDLSystem::SGetLastErrorString()
{
	LPVOID lpMsgBuf;

	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	std::string error = (char *)lpMsgBuf;

	LocalFree(lpMsgBuf);

	return error;
}
