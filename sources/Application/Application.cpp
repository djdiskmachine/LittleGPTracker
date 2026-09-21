#include "Application/Application.h" 
#include "Application/AppWindow.h" 
#include "UIFramework/Interfaces/I_GUIWindowFactory.h"
#include "Application/Persistency/PersistencyService.h" 
#include "Services/Audio/Audio.h"
#include "Application/Commands/CommandDispatcher.h"
#include "Application/Controllers/ControlRoom.h"
#include "Application/Model/Config.h"
#include "Services/Midi/MidiService.h"

#include <math.h>

Application *Application::instance_=NULL ;

Application::Application() {
}

void Application::initMidiInput()
{
  const char *preferedDevice=Config::GetInstance()->GetValue("MIDICTRLDEVICE");

  // MIDICTRLDEVICE normally names one interface, matched on a prefix because
  // drivers append their own port number ("Midi Through" matches
  // "Midi Through:0").  "*" means "every input that is plugged in", which is
  // what a handheld wants: the name of whatever keyboard is connected is not
  // known in advance and cannot be put in a config file that ships with the
  // build.  Interfaces that nothing is mapped to cost nothing, so opening all
  // of them is harmless.
  bool allDevices=(preferedDevice)&&(!strcmp(preferedDevice,"*")) ;

  IteratorPtr<MidiInDevice>it(MidiService::GetInstance()->GetInIterator()) ;
  for(it->Begin();!it->IsDone();it->Next())
  {
    MidiInDevice &in=it->CurrentItem() ;
    if ((preferedDevice) &&
        (allDevices || (!strncmp(in.GetName(), preferedDevice, strlen(preferedDevice)))))
    {
      if (in.Init())
      {
        if (in.Start())
        {
          Trace::Log("MIDI","Controlling activated for MIDI interface %s",in.GetName()) ;
        }
        else
        {
          in.Close() ;
        }
      }
    }
  }
}

bool Application::Init(GUICreateWindowParams &params) {
	const char* root=Config::GetInstance()->GetValue("ROOTFOLDER") ;
	if (root) {
		Path::SetAlias("root",root) ;
	} ;
	window_=AppWindow::Create(params) ;
	PersistencyService::GetInstance() ;
  Audio *audio=Audio::GetInstance() ;
  audio->Init() ;
	CommandDispatcher::GetInstance()->Init() ;
  initMidiInput();
	ControlRoom::GetInstance()->LoadMapping("bin:mapping.xml") ;
	return true ;
} ;

GUIWindow *Application::GetWindow() {
	return window_ ;
} ;

Application::~Application() {
	delete window_ ;
}
