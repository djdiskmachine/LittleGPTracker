#include "SwitchSystem.h"
#include "Adapters/Dummy/Midi/DummyMidi.h"
#include "Adapters/SDL2/Audio/SDLAudio.h"
#include "Adapters/SDL2/GUI/GUIFactory.h"
#include "Adapters/SDL2/GUI/SDLEventManager.h"
#include "Adapters/SDL2/GUI/SDLGUIWindowImp.h"
#include "Adapters/SDL2/Timer/SDLTimer.h"
#include "Adapters/SDL2/Process/SDLProcess.h"
#include "Adapters/Unix/FileSystem/UnixFileSystem.h"
#include "Application/Commands/NodeList.h"
#include "Application/Controllers/ControlRoom.h"
#include "Application/Model/Config.h"
#include "System/Console/Logger.h"
#include "SwitchNxlink.h"
#include <sys/time.h>
#include <time.h>

// homebrew apps live under a fixed sdmc: path, there is no /proc/self/exe
// equivalent to derive it from under libnx
#define SWITCH_APP_ROOT "sdmc:/switch/lgpt"

EventManager *SwitchSystem::eventManager_ = NULL;
static int secbase = 0;

/*
 * starts the main loop
 */
int SwitchSystem::MainLoop() {
    eventManager_->InstallMappings();
    return eventManager_->MainLoop();
};

/*
 * initializes the application
 */
void SwitchSystem::Boot(int argc, char **argv) {

    // Redirect stdout/stderr to nxlink so `nxlink -s lgpt-switch.nro` streams
    // Trace::Log output live; a no-op if not launched through nxlink.
    SwitchNxlinkInit();

    // Install System
    System::Install(new SwitchSystem());

    // Install FileSystem
    FileSystem::Install(new UnixFileSystem());

    // Install aliases
    Path::SetAlias("bin", SWITCH_APP_ROOT);
    Path::SetAlias("root", SWITCH_APP_ROOT);

    // always use stdout, visible via nxlink if the app was launched that way
    Trace::GetInstance()->SetLogger(*(new StdOutLogger()));

    // Process arguments
    Config::GetInstance()->ProcessArguments(argc, argv);

    // Install GUI Factory
    I_GUIWindowFactory::Install(new GUIFactory());

    // Install Timers
    TimerService::GetInstance()->Install(new SDLTimerService());

    Trace::Log("System", "Installing SDL audio");
    AudioSettings hint;
    hint.bufferSize_ = 1024;
    hint.preBufferCount_ = 8;
    // switch-sdl2's audio renderer runs at a hardcoded 48kHz mix rate
    // (AudioRendererOutputRate_48kHz in its SDL_switchaudio.c) regardless
    // of what we request; the per-voice rate conversion from a requested
    // 44100 up to that fixed 48kHz mix rate wasn't behaving correctly in
    // practice (playback audibly sped up, ~48000/44100 = 1.088x). Request
    // 48000 directly so the voice rate matches the renderer's native rate
    // and no resampling is needed at all.
    hint.sampleRate_ = 48000;
    Audio::Install(new SDLAudio(hint));

    Trace::Log("System", "Installing DUMMY MIDI");
    MidiService::Install(new DummyMidi());

    // Install Threads.
    SysProcessFactory::Install(new SDLProcessFactory());

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        return;
    }
    SDL_ShowCursor(SDL_DISABLE);

    atexit(SDL_Quit);

    eventManager_ = I_GUIWindowFactory::GetInstance()->GetEventManager();
    eventManager_->Init();
};

void SwitchSystem::Shutdown() {};

/*
 * get current time for status display
 */
unsigned long SwitchSystem::GetClock() {
    struct timeval tp;

    gettimeofday(&tp, NULL);
    if (!secbase) {
        secbase = tp.tv_sec;
        return long(tp.tv_usec / 1000.0);
    }
    return long((tp.tv_sec - secbase) * 1000 + tp.tv_usec / 1000.0);
}

/*
 * wraps sleep, guess we never sleep!
 */
void SwitchSystem::Sleep(int millisec) {
}

/*
 * wraps malloc
 */
void *SwitchSystem::Malloc(unsigned size) {
    return malloc(size);
}

/*
 * wraps free
 */
void SwitchSystem::Free(void *ptr) { free(ptr); }

/*
 * wraps memset
 */
void SwitchSystem::Memset(void *addr, char val, int size) {
    memset(addr, val, size);
};

/*
 * wraps memcpy
 */
void *SwitchSystem::Memcpy(void *s1, const void *s2, int n) {
    return memcpy(s1, s2, n);
};

/*
 * logprint
 */
void SwitchSystem::AddUserLog(const char *msg) {
    fprintf(stderr, "LOG: %s\n", msg);
};

/*
 * print after quit
 */
void SwitchSystem::PostQuitMessage() {
    SDLEventManager::GetInstance()->PostQuitMessage();
};

/*
 * get memory usage, guess it's infinite
 */
unsigned int SwitchSystem::GetMemoryUsage() { return 0; };
