#ifndef _SWITCH_NXLINK_H_
#define _SWITCH_NXLINK_H_

// isolated from <switch.h> on purpose: libnx typedefs (AudioDriver, Result, ...)
// collide with this project's own class names of the same name
void SwitchNxlinkInit();

#endif
