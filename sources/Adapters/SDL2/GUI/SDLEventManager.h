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

	// Buttons that, held together, quit the tracker. Handhelds have no window
	// manager to close the window and the tracker's own quit control lives in
	// the project picker, which is unreachable once a project is loaded, so
	// without this there is no way out of a running project.
	unsigned int quitButtonMask_ ;
	unsigned int buttonsDown_ ;
	void LoadQuitCombo() ;
	// SDL reports a joystick *instance id* in events, which is not the same as
	// the index the joysticks were opened with. Map one to the other, or -1.
	int JoystickIndexForInstance(SDL_JoystickID instanceId) ;
} ;
#endif
