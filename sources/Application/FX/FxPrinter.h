#ifndef FxPrinter_H
#define FxPrinter_H

#include "Application/Instruments/InstrumentBank.h"
#include "Application/Instruments/SampleInstrument.h"
#include "Application/Instruments/SamplePool.h"
#include "Application/Views/ViewData.h"
#include "System/FileSystem/FileSystem.h"
#include <sstream>
#include <string>
#ifdef FFMPEG_ENABLED
#include "LibavProcessor.h"
#endif

class FxPrinter {
public:
    FxPrinter(ViewData* viewData);
    bool Run();
    char *GetNotification();

  private:
    void setParams();
    void setPaths();
    std::string parseCommand();
    Path samples_dir;
    Path impulse_dir;
    SampleInstrument* instrument_;
    ViewData* viewData_;
    int irPad_;
    int irWet_;
    std::string fi_;
    std::string fiPath_;
    std::string fo_;
    std::string foPath_;
    std::string ir_;
    char* notificationResult_;
};

#endif // FxPrinter_H