#ifndef _FONTCONFIG_H_
#define _FONTCONFIG_H_

#define FONT_WIDTH 1024
#define FONT_COUNT 127
#define FONT_TYPE_COUNT 3

extern unsigned char font[8*8*128];
extern const unsigned char embeddedFontBank[FONT_TYPE_COUNT][8*8*128];

void FontConfig();

#endif