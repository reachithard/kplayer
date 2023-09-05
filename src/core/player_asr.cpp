//
// Created by luo on 2023/9/6.
//

#include <vector>
#include "player_asr.h"
#include "asr_whisper_wrapper.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavutil/avstring.h"
#include "libavutil/channel_layout.h"
#include "libavutil/eval.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/fifo.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavutil/bprint.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"
#include "libswresample/swresample.h"

#ifdef __cplusplus
}
#endif

static int stream_open(const char *filename, const char *format, std::vector<float> &temp_f32) {
    if (!filename) {
        return -1;
    }
    const AVInputFormat *iformat = format ? av_find_input_format(format) : nullptr;

    SwrContext *swr_ctx = nullptr; // 进行重采样
    int ret = 0;
    int video_stream = 0;
    int audio_stream = 0;
    AVCodecContext *avctx = nullptr;
    const AVCodec *codec = nullptr;
    AVPacket* pkt = av_packet_alloc();
    AVChannelLayout out_layout;
//    uint8_t *audio_buf1 = nullptr;
//    unsigned int audio_buf1_size = 0;
    memset(&out_layout, 0, sizeof(AVChannelLayout));

    if (!pkt) {
        av_log(NULL, AV_LOG_FATAL, "Could not allocate packet.\n");
        ret = AVERROR(ENOMEM);
        return -1;
    }

    AVFrame *frame = av_frame_alloc();
    if (!frame)
        return AVERROR(ENOMEM);

    AVFormatContext *ic = avformat_alloc_context();
    if (!ic) {
        av_log(NULL, AV_LOG_FATAL, "Could not allocate context.\n");
        ret = -1;
        goto fail;
    }

    ret = avformat_open_input(&ic, filename, iformat, NULL);
    if (ret < 0) {
        ret = -1;
        goto fail;
    }

    av_format_inject_global_side_data(ic);

    ret = avformat_find_stream_info(ic, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_WARNING,
               "%s: could not find codec parameters\n", filename);
        ret = -1;
        goto fail;
    }

    video_stream =
            av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO,
                                video_stream, -1, NULL, 0);

    audio_stream =
            av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO,
                                audio_stream,
                                video_stream,
                                NULL, 0);

    if (audio_stream >= 0) {
        // 进行设置
        avctx = avcodec_alloc_context3(nullptr);
        if (!avctx) {
            return AVERROR(ENOMEM);
        }

        ret = avcodec_parameters_to_context(avctx, ic->streams[audio_stream]->codecpar);
        if (ret < 0) {
            goto fail;
        }

        avctx->pkt_timebase = ic->streams[audio_stream]->time_base;

        codec = avcodec_find_decoder(avctx->codec_id);
        if (!codec) {
            ret = AVERROR(EINVAL);
            goto fail;
        }

        avctx->codec_id = codec->id;
        if ((ret = avcodec_open2(avctx, codec, NULL)) < 0) {
            goto fail;
        }
        ic->streams[audio_stream]->discard = AVDISCARD_DEFAULT;

    }

    // 读取文件进行解析
    for (;;) {
        ret = av_read_frame(ic, pkt);
        if (ret == AVERROR_EOF || avio_feof(ic->pb)) {
            break; // 文件读取完毕 进行退出
        }

        if (pkt->stream_index == audio_stream) {
            if (avcodec_send_packet(avctx, pkt) == AVERROR(EAGAIN)) {
            } else {
            }

            ret = avcodec_receive_frame(avctx, frame);
            if (ret >= 0) {
                const uint8_t **in = (const uint8_t **)frame->extended_data;
                if (!swr_ctx) {
                    swr_free(&swr_ctx);
                    if (av_channel_layout_copy(&out_layout, &frame->ch_layout) < 0) {
                        av_log(NULL, AV_LOG_ERROR, "av_channel_layout_copy() failed\n");
                        return -1;
                    }

                    swr_alloc_set_opts2(&swr_ctx,
                                        &out_layout, AV_SAMPLE_FMT_FLT, 16000,
                                        &frame->ch_layout, (AVSampleFormat)frame->format, frame->sample_rate,
                                        0, NULL);
                    if (swr_init(swr_ctx) < 0) {
                        swr_free(&swr_ctx);
                        av_log(NULL, AV_LOG_ERROR, "swr_init() failed\n");
                        return -1;
                    }
                }

                if (swr_ctx) {
                    int out_size  = av_samples_get_buffer_size(NULL, out_layout.nb_channels, frame->nb_samples, AV_SAMPLE_FMT_FLT, 0);
                    std::vector<float> temp;
                    temp.resize(frame->nb_samples);
                    uint8_t *audio_buf1 = (uint8_t *)temp.data();
                    int len2 = swr_convert(swr_ctx, &audio_buf1, out_size, in, frame->nb_samples);
                    if (len2 < 0) {
                        av_log(NULL, AV_LOG_ERROR, "swr_convert() failed\n");
                        return -1;
                    }

                    temp_f32.insert(temp_f32.end(), temp.begin(), temp.end());
                }
            } else {

            }
        } else {
        }
    }
    av_packet_free(&pkt);
    av_frame_free(&frame);
    return 0;

    fail:
    return ret;
}

int player_asr_translate(const char *file, const char *format, const char *model, const char *language) {
    if (!file || !model || !language) {
        return -1;
    }

    std::vector<float> pcmf32;
    if (stream_open(file, format, pcmf32) != 0) {
        return -1;
    }

    AsrWhisperCtx *ctx = asr_create();
    if (!ctx) {
        return -1;
    }

    if (asr_init(ctx, model, language)) {
        return -1;
    }

    if (asr_process(ctx, pcmf32.data(), pcmf32.size()) != 0) {
        return -1;
    }

    return 0;
}