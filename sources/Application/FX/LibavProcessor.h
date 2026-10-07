#ifndef LIBAV_PROC_H
#define LIBAV_PROC_H

#ifdef __cplusplus
extern "C" {
#endif

int encode(const char *fi, const char *ir, const char *fo, int irWet, int irPad);
static int decode_into_buffer(AVFormatContext *fmt_ctx, AVCodecContext *dec_ctx,
                               AVFilterContext *buffersrc_ctx, AVPacket *pkt, AVFrame *frame) {

#ifdef __cplusplus
}
#endif

#endif