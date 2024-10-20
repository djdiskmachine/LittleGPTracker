#ifndef FxPrinter_H
#define FxPrinter_H

#include <string>
#include <sstream>
#include "Application/Views/ViewData.h"
#include "Application/Instruments/InstrumentBank.h"
#include "Application/Instruments/SampleInstrument.h"
#include "System/FileSystem/FileSystem.h"
#include "Application/Instruments/SamplePool.h"

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
    std::string fo_;
    std::string ir_;
    std::string foWav_;
    std::string ffmpeg_;
    char* notificationResult_;
};

#endif // FxPrinter_H