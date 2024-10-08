#ifndef _SDL_EVENT_MANAGER_
#define _SDL_EVENT_MANAGER_

#include <SDL2/SDL.h>
#include <string>
#include "Foundation/T_Singleton.h"
#include "Services/Controllers/ButtonControllerSource.h"
#include "Services/Controllers/HatControllerSource.h"
#include "Services/Controllers/JoystickControllerSource.h"
#include "Services/Controllers/KeyboardControllerSource.h"
#include "UIFramework/SimpleBaseClasses/EventManager.h"

#define MAX_JOY_COUNT 4



class SDLEventManager: public T_Singleton<SDLEventManager>,public EventManager {
public:
	SDLEventManager() ;
	~SDLEventManager() ;
	virtual bool Init() ;
	virtual int MainLoop() ;
	virtual void PostQuitMessage() ;
	virtual int GetKeyCode(const char *name) ;

private:
	static bool finished_ ;
	static bool dumpEvent_ ;
	SDL_Joystick *joystick_[MAX_JOY_COUNT];
	ButtonControllerSource *buttonCS_[MAX_JOY_COUNT] ;
	JoystickControllerSource *joystickCS_[MAX_JOY_COUNT] ;
	HatControllerSource *hatCS_[MAX_JOY_COUNT] ;
	KeyboardControllerSource *keyboardCS_ ;
} ;
#endif
