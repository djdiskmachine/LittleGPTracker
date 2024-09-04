#include "Application/Application.h"
#include "Adapters/MacOS/MacOSSystem/MacOSSystem.h"
#include "Adapters/SDL2/GUI/SDLGUIWindowImp.h"

#include <string.h>

int main(int argc,char *argv[]) 
{
    MacOSSystem::Boot(argc,argv) ;

	SDLCreateWindowParams params ;
	params.title="littlegptracker" ;
	params.cacheFonts_=false ;

	Application::GetInstance()->Init(params) ;

	int retval=MacOSSystem::MainLoop() ;

	return retval ;
}
