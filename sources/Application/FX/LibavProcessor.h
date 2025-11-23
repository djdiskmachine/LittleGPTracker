#ifndef LIBAV_PROC_H
#define LIBAV_PROC_H

#ifdef __cplusplus
extern "C" {
#endif

int encode(const char *fi, const char *ir, const char *fo, int irWet, int irPad);

#ifdef __cplusplus
}
#endif

#endif