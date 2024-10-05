#include "Application.h"

#include "Application/AppWindow.h"
#include "Application/Commands/CommandDispatcher.h"
#include "Application/Controllers/ControlRoom.h"
#include "Application/Model/Config.h"
#include "Application/Persistency/PersistencyService.h"
#include "Services/Audio/Audio.h"
#include "Services/Midi/MidiService.h"
#include "UIFramework/Interfaces/I_GUIWindowFactory.h"
#include <math.h>

Application *Application::instance_ = NULL;

Application::Application() {}

void Application::initMidiInput() {
    const char *preferedDevice =
        Config::GetInstance()->GetValue("MIDICTRLDEVICE");
    if (preferedDevice) {
        MidiService::GetInstance()->SelectDevice(preferedDevice);
    }
}

bool Application::Init(GUICreateWindowParams &params) {
    const char *root = Config::GetInstance()->GetValue("ROOTFOLDER");
    if (root) {
        Path::SetAlias("root", root);
    }
    window_ = AppWindow::Create(params);
    PersistencyService::GetInstance();
    Audio *audio = Audio::GetInstance();
    audio->Init();
    CommandDispatcher::GetInstance()->Init();
    initMidiInput();
    ControlRoom::GetInstance()->LoadMapping("bin:mapping.xml");
    return true;
}

GUIWindow *Application::GetWindow() { return window_; }

Application::~Application() { delete window_; }
