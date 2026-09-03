#ifndef _SWITCH_SYSTEM_H_
#define _SWITCH_SYSTEM_H_

#include <SDL2/SDL.h>
#include "System/System/System.h"
#include "UIFramework/SimpleBaseClasses/EventManager.h"

class SwitchSystem: public System {
public:
	static void Boot(int argc,char **argv);
	static void Shutdown();
	static int MainLoop();

public: // System implementation
	virtual unsigned long GetClock();
	virtual void Sleep(int millisec);
	virtual void *Malloc(unsigned size);
	virtual void Free(void *);
	virtual void Memset(void *addr,char val,int size);
	virtual void *Memcpy(void *s1, const void *s2, int n);
	virtual void AddUserLog(const char *);
	virtual int GetBatteryLevel() { return -1 ; };
	virtual void PostQuitMessage();
	virtual unsigned int GetMemoryUsage();

private:
	static EventManager *eventManager_;
};
#endif
