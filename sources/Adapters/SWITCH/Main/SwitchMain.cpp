#include <string.h>
#include "Adapters/SWITCH/System/SwitchSystem.h"
#include "Adapters/SDL2/GUI/SDLGUIWindowImp.h"
#include "Application/Application.h"
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
