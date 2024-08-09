#ifndef _KEYBOARD_CONTROLLER_SOURCE_H_
#define _KEYBOARD_CONTROLLER_SOURCE_H_

#include "ControllerSource.h"

#ifdef SDL2
// SDL_NUM_SCANCODES is currently 512
#define MAX_KEY 512
#else
#define MAX_KEY 400
#endif

class KeyboardControllerSource: public ControllerSource {
public:
	KeyboardControllerSource(const char *name) ;
	virtual ~KeyboardControllerSource() ;
	virtual Channel *GetChannel(const char *url) ;
	virtual bool IsRunning() { return true ; } ;
	void SetKey(int key,bool value) ;
private:
	Channel *channel_[MAX_KEY] ;
} ;

#endif

