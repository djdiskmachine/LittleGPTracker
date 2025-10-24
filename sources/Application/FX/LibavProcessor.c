/**
 * Apply effects to a file using libav
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>

static AVFormatContext *ifmt_ctx;
static AVFormatContext *ir_fmt_ctx;
static AVFormatContext *ofmt_ctx;
static AVFilterGraph *filter_graph;
static AVFilterContext *buffersrc_ctx;
static AVFilterContext *ir_buffersrc_ctx;
static AVFilterContext *buffersink_ctx;
static AVCodecContext *enc_ctx = NULL;

static int open_input_file(const char *filename)
{
    AVCodec *dec;
    AVCodecContext *dec_ctx;
    AVStream *stream;
    int ret;

    if ((ret = avformat_open_input(&ifmt_ctx, filename, NULL, NULL)) < 0) { 
        av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
        return ret;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
        return ret;
    }

    /* select the audio stream */
    ret = av_find_best_stream(ifmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, &dec, 0);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find an audio stream in the input file\n");
        return ret;
    }

    stream = ifmt_ctx->streams[ret];

    /* create decoding context */
    dec_ctx = avcodec_alloc_context3(dec);
    if (!dec_ctx)
        return AVERROR(ENOMEM);
    avcodec_parameters_to_context(dec_ctx, stream->codecpar);

    /* init the audio decoder */
    if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open audio decoder\n");
        return ret;
    }

    stream->codecpar->codec_id = dec_ctx->codec_id;
    stream->codecpar->sample_rate = dec_ctx->sample_rate;
    stream->codecpar->format = dec_ctx->sample_fmt;
#ifdef FFMPEG_LEGACY_API
    stream->codecpar->channels = dec_ctx->channels;
    stream->codecpar->channel_layout = dec_ctx->channel_layout;
#else
    av_channel_layout_copy(&stream->codecpar->ch_layout, &dec_ctx->ch_layout);
#endif

    avcodec_free_context(&dec_ctx);

    return 0;
}

static int open_ir_file(const char *filename)
{
    AVCodec *dec;
    AVCodecContext *dec_ctx;
    AVStream *stream;
    int ret;

    if ((ret = avformat_open_input(&ir_fmt_ctx, filename, NULL, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open IR file\n");
        return ret;
    }

    if ((ret = avformat_find_stream_info(ir_fmt_ctx, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find stream information in IR file\n");
        return ret;
    }

    /* select the audio stream */
    ret = av_find_best_stream(ir_fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, &dec, 0);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find an audio stream in the IR file\n");
        return ret;
    }

    stream = ir_fmt_ctx->streams[ret];

    /* create decoding context */
    dec_ctx = avcodec_alloc_context3(dec);
    if (!dec_ctx)
        return AVERROR(ENOMEM);
    avcodec_parameters_to_context(dec_ctx, stream->codecpar);

    /* init the audio decoder */
    if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open IR audio decoder\n");
        return ret;
    }

    stream->codecpar->codec_id = dec_ctx->codec_id;
    stream->codecpar->sample_rate = dec_ctx->sample_rate;
    stream->codecpar->format = dec_ctx->sample_fmt;
#ifdef FFMPEG_LEGACY_API
    stream->codecpar->channels = dec_ctx->channels;
    stream->codecpar->channel_layout = dec_ctx->channel_layout;
#else
    av_channel_layout_copy(&stream->codecpar->ch_layout, &dec_ctx->ch_layout);
#endif

    avcodec_free_context(&dec_ctx);

    return 0;
}

static int open_output_file(const char *filename)
{
    AVStream *out_stream;
    AVStream *in_stream;
    const AVCodec *encoder;
    int ret;

    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, filename);
    if (!ofmt_ctx) {
        av_log(NULL, AV_LOG_ERROR, "Could not create output context\n");
        return AVERROR_UNKNOWN;
    }

    in_stream = ifmt_ctx->streams[0];
    out_stream = avformat_new_stream(ofmt_ctx, NULL);
    if (!out_stream) {
        av_log(NULL, AV_LOG_ERROR, "Failed allocating output stream\n");
        return AVERROR_UNKNOWN;
    }

    /* find encoder - force PCM 16-bit little-endian as requested */
    encoder = avcodec_find_encoder(AV_CODEC_ID_PCM_S16LE);
    if (!encoder) {
        av_log(NULL, AV_LOG_FATAL, "Necessary encoder not found\n");
        return AVERROR_INVALIDDATA;
    }

    enc_ctx = avcodec_alloc_context3(encoder);
    if (!enc_ctx) {
        av_log(NULL, AV_LOG_FATAL, "Failed to allocate the encoder context\n");
        return AVERROR(ENOMEM);
    }

    /* Set output parameters to target format: 44.1kHz, 16-bit, stereo unless IR is mono */
    enc_ctx->sample_rate = 44100;
    
    /* Set channel layout based on IR format */
#ifdef FFMPEG_LEGACY_API
    int ir_channels = ir_fmt_ctx->streams[0]->codecpar->channels;
    if (ir_channels == 0) {
        ir_channels = ir_fmt_ctx->streams[0]->codecpar->channels;
    }
    
    if (ir_channels == 1) {
        enc_ctx->channels = 1;
        enc_ctx->channel_layout = AV_CH_LAYOUT_MONO;
    } else {
        enc_ctx->channels = 2;
        enc_ctx->channel_layout = AV_CH_LAYOUT_STEREO;
    }
#else
    int ir_channels = ir_fmt_ctx->streams[0]->codecpar->ch_layout.nb_channels;
    if (ir_channels == 0) {
        ir_channels = ir_fmt_ctx->streams[0]->codecpar->channels;
    }
    
    if (ir_channels == 1) {
        av_channel_layout_default(&enc_ctx->ch_layout, 1);  /* Mono */
    } else {
        av_channel_layout_default(&enc_ctx->ch_layout, 2);  /* Stereo */
    }
#endif

    /* Use S16 format to match encoder */
    enc_ctx->sample_fmt = AV_SAMPLE_FMT_S16;

    /* Third parameter can be used to pass settings to encoder */
    ret = avcodec_open2(enc_ctx, encoder, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open video encoder for stream\n");
        return ret;
    }
    ret = avcodec_parameters_from_context(out_stream->codecpar, enc_ctx);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Failed to copy encoder parameters to output stream\n");
        return ret;
    }

    out_stream->time_base = enc_ctx->time_base;

    av_dump_format(ofmt_ctx, 0, filename, 1);

    /* open the output file, if needed */
    if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Could not open output file '%s'", filename);
            return ret;
        }
    }

    /* init muxer, write output file header */
    ret = avformat_write_header(ofmt_ctx, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Error occurred when opening output file\n");
        return ret;
    }

    return 0;
}

static int init_filters(int ir_wet, int ir_pad)
{
    char args[512];
    char ir_args[512];
    int ret = 0;
    const AVFilter *abuffer, *abuffersink;
    AVFilterInOut *outputs = avfilter_inout_alloc();
    AVFilterInOut *inputs  = avfilter_inout_alloc();
    AVRational time_base = ifmt_ctx->streams[0]->time_base;
    AVRational ir_time_base = ir_fmt_ctx->streams[0]->time_base;

    /* Determine the target format - always 44.1kHz, stereo unless IR is mono */
    char target_layout[64];
    int target_sample_rate = 44100;
    
    /* Determine output channel layout based on IR format */
#ifdef FFMPEG_LEGACY_API
    int ir_channels = ir_fmt_ctx->streams[0]->codecpar->channels;
    if (ir_channels == 0) {
        ir_channels = ir_fmt_ctx->streams[0]->codecpar->channels;
    }
#else
    int ir_channels = ir_fmt_ctx->streams[0]->codecpar->ch_layout.nb_channels;
    if (ir_channels == 0) {
        ir_channels = ir_fmt_ctx->streams[0]->codecpar->channels;
    }
#endif

    if (ir_channels == 1) {
        strcpy(target_layout, "mono");  /* If IR is mono, output mono for simplicity */
    } else {
        strcpy(target_layout, "stereo"); /* Otherwise, output stereo */
    }

    filter_graph = avfilter_graph_alloc();
    if (!outputs || !inputs || !filter_graph) {
        ret = AVERROR(ENOMEM);
        goto end;
    }

    /* buffer audio source: the decoded frames from the decoder will be inserted here. */
    abuffer = avfilter_get_by_name("abuffer");
    if (!abuffer) {
        av_log(NULL, AV_LOG_ERROR, "Could not find the abuffer filter.\n");
        ret = AVERROR_FILTER_NOT_FOUND;
        goto end;
    }

    buffersrc_ctx = avfilter_graph_alloc_filter(filter_graph, abuffer, "in");
    if (!buffersrc_ctx) {
        av_log(NULL, AV_LOG_ERROR, "Could not allocate the abuffer instance.\n");
        ret = AVERROR(ENOMEM);
        goto end;
    }

    /* Set the filter options through the AVOptions API. */
    char ch_layout_str[64];
    // Use the actual channel layout from the input file
#ifdef FFMPEG_LEGACY_API
    if (ifmt_ctx->streams[0]->codecpar->channels == 1) {
        strcpy(ch_layout_str, "mono");
    } else if (ifmt_ctx->streams[0]->codecpar->channels == 2) {
        strcpy(ch_layout_str, "stereo");
    } else {
        snprintf(ch_layout_str, sizeof(ch_layout_str), "%dc", ifmt_ctx->streams[0]->codecpar->channels);
    }
#else
    if (ifmt_ctx->streams[0]->codecpar->ch_layout.nb_channels == 0) {
        // If channel layout is unknown, default based on channel count
        if (ifmt_ctx->streams[0]->codecpar->channels == 1) {
            strcpy(ch_layout_str, "mono");
        } else if (ifmt_ctx->streams[0]->codecpar->channels == 2) {
            strcpy(ch_layout_str, "stereo");
        } else {
            snprintf(ch_layout_str, sizeof(ch_layout_str), "%dc", ifmt_ctx->streams[0]->codecpar->channels);
        }
    } else {
        av_channel_layout_describe(&buffersink_ctx->inputs[0]->ch_layout,
                                   ch_layout_str, sizeof(ch_layout_str));
    }
#endif
    snprintf(args, sizeof(args),
             "time_base=%d/%d:sample_rate=%d:sample_fmt=%s:channel_layout=%s",
             time_base.num, time_base.den, ifmt_ctx->streams[0]->codecpar->sample_rate,
             av_get_sample_fmt_name(ifmt_ctx->streams[0]->codecpar->format),
             ch_layout_str);
    ret = avfilter_init_str(buffersrc_ctx, args);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not initialize the abuffer filter.\n");
        goto end;
    }

    /* buffer audio source for IR: the decoded IR frames will be inserted here. */
    ir_buffersrc_ctx = avfilter_graph_alloc_filter(filter_graph, abuffer, "ir_in");
    if (!ir_buffersrc_ctx) {
        av_log(NULL, AV_LOG_ERROR, "Could not allocate the IR abuffer instance.\n");
        ret = AVERROR(ENOMEM);
        goto end;
    }

    char ir_ch_layout_str[64];
    // Use the actual channel layout from the IR file
#ifdef FFMPEG_LEGACY_API
    if (ir_fmt_ctx->streams[0]->codecpar->channels == 1) {
        strcpy(ir_ch_layout_str, "mono");
    } else if (ir_fmt_ctx->streams[0]->codecpar->channels == 2) {
        strcpy(ir_ch_layout_str, "stereo");
    } else {
        snprintf(ir_ch_layout_str, sizeof(ir_ch_layout_str), "%dc", ir_fmt_ctx->streams[0]->codecpar->channels);
    }
#else
    if (ir_fmt_ctx->streams[0]->codecpar->ch_layout.nb_channels == 0) {
        // If channel layout is unknown, default based on channel count
        if (ir_fmt_ctx->streams[0]->codecpar->channels == 1) {
            strcpy(ir_ch_layout_str, "mono");
        } else if (ir_fmt_ctx->streams[0]->codecpar->channels == 2) {
            strcpy(ir_ch_layout_str, "stereo");
        } else {
            snprintf(ir_ch_layout_str, sizeof(ir_ch_layout_str), "%dc", ir_fmt_ctx->streams[0]->codecpar->channels);
        }
    } else {
        av_channel_layout_describe(&buffersink_ctx->inputs[0]->ch_layout,
                                   ir_ch_layout_str, sizeof(ir_ch_layout_str));
    }
#endif
    snprintf(ir_args, sizeof(ir_args),
             "time_base=%d/%d:sample_rate=%d:sample_fmt=%s:channel_layout=%s",
             ir_time_base.num, ir_time_base.den, ir_fmt_ctx->streams[0]->codecpar->sample_rate,
             av_get_sample_fmt_name(ir_fmt_ctx->streams[0]->codecpar->format),
             ir_ch_layout_str);
    ret = avfilter_init_str(ir_buffersrc_ctx, ir_args);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not initialize the IR abuffer filter.\n");
        goto end;
    }

    /* buffer audio sink: to terminate the filter chain. */
    abuffersink = avfilter_get_by_name("abuffersink");
    if (!abuffersink) {
        av_log(NULL, AV_LOG_ERROR, "Could not find the abuffersink filter.\n");
        ret = AVERROR_FILTER_NOT_FOUND;
        goto end;
    }

    buffersink_ctx = avfilter_graph_alloc_filter(filter_graph, abuffersink, "out");
    if (!buffersink_ctx) {
        av_log(NULL, AV_LOG_ERROR, "Could not allocate the abuffersink instance.\n");
        ret = AVERROR(ENOMEM);
        goto end;
    }

    /* Configure the buffersink to accept the expected format */
    static const enum AVSampleFormat out_sample_fmts[] = { AV_SAMPLE_FMT_S16, -1 };
    ret = av_opt_set_int_list(buffersink_ctx, "sample_fmts", out_sample_fmts, -1, AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot set output sample format\n");
        goto end;
    }
    
    ret = av_opt_set(buffersink_ctx, "ch_layouts", target_layout, AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot set output channel layout\n");
        goto end;
    }

    /* This filter takes no options. */
    ret = avfilter_init_str(buffersink_ctx, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not initialize the abuffersink instance.\n");
        goto end;
    }

    /*
     * Set the endpoints for the filter graph. The filter_graph will
     * be linked to the graph described by filters_descr.
     */

    /*
     * The buffer source output must be connected to the input pad of
     * the first filter described by filters_descr; since the first
     * filter input label is "in", with the pointer, we leave this
     * pad open.
     */
    outputs->name       = av_strdup("in");
    outputs->filter_ctx = buffersrc_ctx;
    outputs->pad_idx    = 0;
    outputs->next       = avfilter_inout_alloc();

    outputs->next->name       = av_strdup("ir_in");
    outputs->next->filter_ctx = ir_buffersrc_ctx;
    outputs->next->pad_idx    = 0;
    outputs->next->next       = NULL;

    /*
     * The buffer sink input must be connected to the output pad of
     * the last filter described by filters_descr; since the last
     * filter output label is "out", with the pointer, we leave this
     * pad open.
     */
    inputs->name       = av_strdup("out");
    inputs->filter_ctx = buffersink_ctx;
    inputs->pad_idx    = 0;
    inputs->next       = NULL;

    /* Apply convolution reverb using afir filter with format normalization */
    char filters_descr[512];
    snprintf(filters_descr, sizeof(filters_descr),
             "[ir_in]aresample=%d,aformat=sample_fmts=fltp:channel_layouts=%s[ir_norm];"
             "[in]aresample=%d,aformat=sample_fmts=fltp:channel_layouts=%s,asplit[in_1][in_2];"
             "[in_1][ir_norm]afir=dry=10:wet=10[reverb];"
             "[in_2][reverb]amix=inputs=2:weights=100 %d,volume=1.5,aformat=sample_fmts=s16:channel_layouts=%s[out]",
             target_sample_rate, target_layout, target_sample_rate, target_layout, ir_wet, target_layout);

    av_log(NULL, AV_LOG_INFO, "Filter graph: %s\n", filters_descr);

    if ((ret = avfilter_graph_parse_ptr(filter_graph, filters_descr,
                                    &inputs, &outputs, NULL)) < 0)
        goto end;

    if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
        goto end;

end:
    avfilter_inout_free(&inputs);
    avfilter_inout_free(&outputs);

    return ret;
}

static int encode_write_frame(AVFrame *filt_frame, unsigned int stream_index, int *got_frame) {
    int ret;
    int got_frame_local;
    AVPacket *enc_pkt;

    if (!got_frame)
        got_frame = &got_frame_local;

    if (!enc_ctx) {
        return AVERROR(EINVAL);
    }

    enc_pkt = av_packet_alloc();
    if (!enc_pkt) {
        return AVERROR(ENOMEM);
    }

    /* encode filtered frame */
    ret = avcodec_send_frame(enc_ctx, filt_frame);
    if (ret < 0)
        goto end;

    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, enc_pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            ret = 0; /* These are not errors */
            break;
        } else if (ret < 0)
            goto end;

        /* prepare packet for muxing */
        enc_pkt->stream_index = stream_index;
        av_packet_rescale_ts(enc_pkt,
                           enc_ctx->time_base,
                           ofmt_ctx->streams[stream_index]->time_base);

        /* mux encoded frame */
        ret = av_interleaved_write_frame(ofmt_ctx, enc_pkt);
        if (ret < 0)
            break;
    }

end:
    av_packet_free(&enc_pkt);
    return ret;
}

static int filter_encode_write_frame(AVFrame *frame, AVFrame *ir_frame, unsigned int stream_index)
{
    int ret;
    AVFrame *filt_frame;

    /* Only add frames if they are provided */
    if (frame) {
        ret = av_buffersrc_add_frame_flags(buffersrc_ctx, frame, AV_BUFFERSRC_FLAG_KEEP_REF);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error while feeding the input filtergraph\n");
            return ret;
        }
    }

    if (ir_frame) {
        ret = av_buffersrc_add_frame_flags(ir_buffersrc_ctx, ir_frame, AV_BUFFERSRC_FLAG_KEEP_REF);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error while feeding the IR filtergraph\n");
            return ret;
        }
    }

    /* Always try to pull filtered frames from the filtergraph */
    while (1) {
        filt_frame = av_frame_alloc();
        if (!filt_frame) {
            ret = AVERROR(ENOMEM);
            break;
        }
        ret = av_buffersink_get_frame(buffersink_ctx, filt_frame);
        if (ret < 0) {
            /* if no more frames for output - returns AVERROR(EAGAIN)
             * if flushed and no more frames for output - returns AVERROR_EOF
             * rewrite retcode to 0 to show it as normal procedure completion
             */
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                ret = 0;
            av_frame_free(&filt_frame);
            break;
        }

        printf("Got filtered frame: %d samples, %d channels, %d Hz\n",
#ifdef FFMPEG_LEGACY_API
               filt_frame->nb_samples, filt_frame->channels,
               filt_frame->sample_rate);
#else
               filt_frame->nb_samples, filt_frame->ch_layout.nb_channels,
               filt_frame->sample_rate);
#endif

        ret = encode_write_frame(filt_frame, stream_index, NULL);
        av_frame_free(&filt_frame);
        if (ret < 0)
            break;
    }

    return ret;
}

int encode(const char *fi, const char *ir, const char *fo, int irWet, int irPad)
{
    int ret;
    AVPacket *packet = NULL, *ir_packet = NULL;
    AVFrame *frame = NULL, *ir_frame = NULL;
    AVCodecContext *dec_ctx = NULL, *ir_dec_ctx = NULL;
    unsigned int stream_index;
    int ir_loaded = 0;

    if ((ret = open_input_file(fi)) < 0)
        goto end;
    if ((ret = open_ir_file(ir)) < 0)
        goto end;
    if ((ret = open_output_file(fo)) < 0)
        goto end;
    if ((ret = init_filters(irWet, irPad)) < 0)
        goto end;

    packet = av_packet_alloc();
    ir_packet = av_packet_alloc();
    frame = av_frame_alloc();
    ir_frame = av_frame_alloc();
    if (!packet || !ir_packet || !frame || !ir_frame) {
        fprintf(stderr, "Could not allocate frame or packet\n");
        return(1);
    }

    /* Set up decoder contexts */
    AVCodec *dec =
        avcodec_find_decoder(ifmt_ctx->streams[0]->codecpar->codec_id);
    AVCodec *ir_dec = avcodec_find_decoder(ir_fmt_ctx->streams[0]->codecpar->codec_id);
    
    dec_ctx = avcodec_alloc_context3(dec);
    ir_dec_ctx = avcodec_alloc_context3(ir_dec);
    
    avcodec_parameters_to_context(dec_ctx, ifmt_ctx->streams[0]->codecpar);
    avcodec_parameters_to_context(ir_dec_ctx, ir_fmt_ctx->streams[0]->codecpar);
    
    avcodec_open2(dec_ctx, dec, NULL);
    avcodec_open2(ir_dec_ctx, ir_dec, NULL);

    /* Process both streams - load ALL IR data first, then process main audio */
    int ir_eof = 0, main_eof = 0;
    
    /* First load all IR data */
    while (!ir_eof && av_read_frame(ir_fmt_ctx, ir_packet) >= 0) {
        if (ir_packet->stream_index == 0) {
            ret = avcodec_send_packet(ir_dec_ctx, ir_packet);
            while (ret >= 0) {
                ret = avcodec_receive_frame(ir_dec_ctx, ir_frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break;
                } else if (ret < 0) {
                    fprintf(stderr, "Error while decoding IR\n");
                    goto end;
                }

                ret = av_buffersrc_add_frame_flags(ir_buffersrc_ctx, ir_frame, AV_BUFFERSRC_FLAG_KEEP_REF);
                if (ret < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Error while feeding the IR filtergraph\n");
                    goto end;
                }
                ir_loaded = 1;
            }
        }
        av_packet_unref(ir_packet);
    }
    
    /* Signal end of IR stream */
    ret = av_buffersrc_add_frame_flags(ir_buffersrc_ctx, NULL, 0);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Error closing IR filtergraph\n");
        goto end;
    }
    
    if (!ir_loaded) {
        fprintf(stderr, "No impulse response data loaded\n");
        goto end;
    }
        
    /* Now process the main audio */
    while (!main_eof && av_read_frame(ifmt_ctx, packet) >= 0) {
        if (packet->stream_index == 0) {
            ret = avcodec_send_packet(dec_ctx, packet);
            while (ret >= 0) {
                ret = avcodec_receive_frame(dec_ctx, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break;
                } else if (ret < 0) {
                    fprintf(stderr, "Error while decoding\n");
                    goto end;
                }

                ret = av_buffersrc_add_frame_flags(buffersrc_ctx, frame, AV_BUFFERSRC_FLAG_KEEP_REF);
                if (ret < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Error while feeding the input filtergraph\n");
                    goto end;
                }                
                /* Pull filtered frames immediately */
                while (1) {
                    AVFrame *filt_frame = av_frame_alloc();
                    ret = av_buffersink_get_frame(buffersink_ctx, filt_frame);
                    if (ret == AVERROR(EAGAIN)) {
                        av_frame_free(&filt_frame);
                        break; /* Need more input */
                    } else if (ret == AVERROR_EOF) {
                        av_frame_free(&filt_frame);
                        break; /* No more frames */
                    } else if (ret < 0) {
                        printf("Error getting frame from filter: %s\n", av_err2str(ret));
                        av_frame_free(&filt_frame);
                        goto end;
                    }

                    ret = encode_write_frame(filt_frame, 0, NULL);
                    av_frame_free(&filt_frame);
                    if (ret < 0) {
                        goto end;
                    }
                }
            }
        }
        av_packet_unref(packet);
    }

    /* flush the filter graph by sending EOF to input sources */
    ret = av_buffersrc_add_frame_flags(buffersrc_ctx, NULL, 0);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Error while closing the input filtergraph\n");
        goto end;
    }
    
    /* Pull any remaining frames from the filter graph */
    ret = filter_encode_write_frame(NULL, NULL, 0);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Flushing filter failed\n");
        goto end;
    }

    /* flush the encoder */
    ret = avcodec_send_frame(enc_ctx, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Error sending NULL frame to encoder\n");
        goto end;
    }
    
    while (ret >= 0) {
        AVPacket *enc_pkt = av_packet_alloc();
        ret = avcodec_receive_packet(enc_ctx, enc_pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            av_packet_free(&enc_pkt);
            break;
        } else if (ret < 0) {
            av_packet_free(&enc_pkt);
            av_log(NULL, AV_LOG_ERROR, "Error during encoder flush\n");
            goto end;
        }
        
        enc_pkt->stream_index = 0;
        av_packet_rescale_ts(enc_pkt, enc_ctx->time_base, ofmt_ctx->streams[0]->time_base);
        ret = av_interleaved_write_frame(ofmt_ctx, enc_pkt);
        av_packet_free(&enc_pkt);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error writing final packet\n");
            goto end;
        }
    }

    av_write_trailer(ofmt_ctx);
end:
    if (dec_ctx) {
        avcodec_free_context(&dec_ctx);
    }
    if (ir_dec_ctx) {
        avcodec_free_context(&ir_dec_ctx);
    }
    av_frame_free(&frame);
    av_frame_free(&ir_frame);
    av_packet_free(&packet);
    av_packet_free(&ir_packet);
    avfilter_graph_free(&filter_graph);
    avformat_close_input(&ifmt_ctx);
    avformat_close_input(&ir_fmt_ctx);
    
    if (enc_ctx) {
        avcodec_free_context(&enc_ctx);
    }

    if (ofmt_ctx && !(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        avio_closep(&ofmt_ctx->pb);
    }
    avformat_free_context(ofmt_ctx);

    if (ret < 0 && ret != AVERROR_EOF) {
        fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
        return(1);
    }
    return 0;
}
