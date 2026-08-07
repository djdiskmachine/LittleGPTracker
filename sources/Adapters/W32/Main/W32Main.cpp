#include <string.h>
#include "Adapters/W32/System/W32System.h"
#include "Adapters/SDL2/GUI/SDLGUIWindowImp.h"
#include "Application/Application.h"

int main(int argc,char *argv[]) {
	W32System::Boot(argc,argv);

	SDLCreateWindowParams params;
	params.title="littlegptracker";
	params.cacheFonts_=true;

	Application::GetInstance()->Init(params);

	int retval=W32System::MainLoop();

	W32System::Shutdown();
	return retval;
}
