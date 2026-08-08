#include <string.h>
#include "Adapters/WSDLSystem/WSDLSystem.h"
#include "Adapters/SDL2/GUI/SDLGUIWindowImp.h"
#include "Application/Application.h"

int main(int argc,char *argv[]) {
	WSDLSystem::Boot(argc,argv);

	SDLCreateWindowParams params;
	params.title="littlegptracker";
	params.cacheFonts_=true;

	Application::GetInstance()->Init(params);

	int retval=WSDLSystem::MainLoop();

	WSDLSystem::Shutdown();
	return retval;
}
