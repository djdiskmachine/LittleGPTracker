#include "Adapters/SDL2/GUI/SDLGUIWindowImp.h"
#include "Adapters/SWITCH/System/SwitchSystem.h"
#include "Application/Application.h"
#include <string.h>
/*
 * entrypoint for the Nintendo Switch homebrew target
 */
int main(int argc,char *argv[]) {
	SwitchSystem::Boot(argc,argv);

	SDLCreateWindowParams params;
	params.title="littlegptracker";
	params.cacheFonts_=true;

	Application::GetInstance()->Init(params);

	return SwitchSystem::MainLoop();
}

void _assert() {};
