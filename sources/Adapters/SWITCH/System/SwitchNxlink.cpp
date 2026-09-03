#include "SwitchNxlink.h"
#include <switch.h>

void SwitchNxlinkInit() {
    socketInitializeDefault();
    nxlinkStdio();
}
