#include "Application/Application.h"
#include "Adapters/VITA/System/VITASystem.h"
#include "Foundation/T_Singleton.h"
#include <SDL2/SDL.h>
#include <string.h>
#include "Adapters/SDL2/GUI/SDLGUIWindowImp.h"
#include "Application/Persistency/PersistencyService.h" 
#include "Adapters/SDL2/GUI/SDLGUIWindowImp.h"

#include <psp2/kernel/clib.h>

/* Define printf, just to make typing easier */
#define printf	sceClibPrintf

int _newlib_heap_size_user = 100*1024*1024;

int main(int argc,char *argv[]) 
{
	VITASystem::Boot(argc,argv) ;

	SDLCreateWindowParams params ;
	params.title="littlegptracker" ;
	params.cacheFonts_=false ;
    params.framebuffer_=false ;
	Application::GetInstance()->Init(params) ;
	VITASystem::MainLoop() ;
    VITASystem::Shutdown() ; 
	return 0 ;
}

