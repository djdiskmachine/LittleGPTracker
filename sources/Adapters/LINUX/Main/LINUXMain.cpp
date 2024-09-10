#include <string.h>
#include "Adapters/LINUX/System/LINUXSystem.h"
#include "Adapters/SDL2/GUI/SDLGUIWindowImp.h"
#include "Application/Application.h"
/*
 * generic entrypoint for linux based targets
 */
int main(int argc,char *argv[]) {
	LINUXSystem::Boot(argc,argv);

	SDLCreateWindowParams params;
	params.title="littlegptracker";
	params.cacheFonts_=true;

	Application::GetInstance()->Init(params);

	return LINUXSystem::MainLoop();
}

void _assert() {};
