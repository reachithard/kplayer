//
// Created by luo on 2023/9/5.
//

#include <iostream>
#include <vector>


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

#define DR_WAV_IMPLEMENTATION

#include "test_asr_whisper_wrapper.h"
#include "test_dr_wav.h"

#define COMMON_SAMPLE_RATE 16000

int stream_open(const char *filename, const char *format, std::vector<float> &temp_f32) {
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
//                    av_fast_malloc(&audio_buf1, &audio_buf1_size, out_size);
//                    av_fast_malloc(buffer, size, *size + out_size);
                    std::vector<float> temp;
                    temp.resize(frame->nb_samples);
                    uint8_t *audio_buf1 = (uint8_t *)temp.data();
                    int len2 = swr_convert(swr_ctx, &audio_buf1, out_size, in, frame->nb_samples);
                    if (len2 < 0) {
                        av_log(NULL, AV_LOG_ERROR, "swr_convert() failed\n");
                        return -1;
                    }

                    temp_f32.insert(temp_f32.end(), temp.begin(), temp.end());

//                    uint8_t **temp = (uint8_t**)buffer;
////                    memcpy(*temp + *size, audio_buf1, len2);
                }
            } else {

            }
        } else {
        }
//
    }
    av_packet_free(&pkt);
    av_frame_free(&frame);
    return 0;

fail:
    return ret;
}

bool read_wav(const std::string & fname, std::vector<int16_t>& pcm16, bool stereo) {
    drwav wav;
    std::vector<uint8_t> wav_data; // used for pipe input from stdin

    if (fname == "-") {
        {
            uint8_t buf[1024];
            while (true)
            {
                const size_t n = fread(buf, 1, sizeof(buf), stdin);
                if (n == 0) {
                    break;
                }
                wav_data.insert(wav_data.end(), buf, buf + n);
            }
        }

        if (drwav_init_memory(&wav, wav_data.data(), wav_data.size(), nullptr) == false) {
            fprintf(stderr, "error: failed to open WAV file from stdin\n");
            return false;
        }

        fprintf(stderr, "%s: read %zu bytes from stdin\n", __func__, wav_data.size());
    }
    else if (drwav_init_file(&wav, fname.c_str(), nullptr) == false) {
        fprintf(stderr, "error: failed to open '%s' as WAV file\n", fname.c_str());
        return false;
    }

    if (wav.channels != 1 && wav.channels != 2) {
        fprintf(stderr, "%s: WAV file '%s' must be mono or stereo\n", __func__, fname.c_str());
        return false;
    }

    if (stereo && wav.channels != 2) {
        fprintf(stderr, "%s: WAV file '%s' must be stereo for diarization\n", __func__, fname.c_str());
        return false;
    }

    if (wav.sampleRate != COMMON_SAMPLE_RATE) {
        fprintf(stderr, "%s: WAV file '%s' must be %i kHz\n", __func__, fname.c_str(), COMMON_SAMPLE_RATE/1000);
        return false;
    }

    if (wav.bitsPerSample != 16) {
        fprintf(stderr, "%s: WAV file '%s' must be 16-bit\n", __func__, fname.c_str());
        return false;
    }

    const uint64_t n = wav_data.empty() ? wav.totalPCMFrameCount : wav_data.size()/(wav.channels*wav.bitsPerSample/8);

    pcm16.resize(n*wav.channels);
    drwav_read_pcm_frames_s16(&wav, n, pcm16.data());
    drwav_uninit(&wav);

//    // convert to mono, float
//    pcmf32.resize(n);
//    if (wav.channels == 1) {
//        for (uint64_t i = 0; i < n; i++) {
//            pcmf32[i] = float(pcm16[i])/32768.0f;
//        }
//    } else {
//        for (uint64_t i = 0; i < n; i++) {
//            pcmf32[i] = float(pcm16[2*i] + pcm16[2*i + 1])/65536.0f;
//        }
//    }
//
//    if (stereo) {
//        // convert to stereo, float
//        pcmf32s.resize(2);
//
//        pcmf32s[0].resize(n);
//        pcmf32s[1].resize(n);
//        for (uint64_t i = 0; i < n; i++) {
//            pcmf32s[0][i] = float(pcm16[2*i])/32768.0f;
//            pcmf32s[1][i] = float(pcm16[2*i + 1])/32768.0f;
//        }
//    }

    return true;
}

int main() {
    std::cout << "hello" << std::endl;

    std::vector<int16_t> pcmf16;               // mono-channel F32 PCM
    std::string fname_inp = "C:\\workspace\\kplayer\\res\\jfk.wav";

    if (!::read_wav(fname_inp, pcmf16, false)) {
        fprintf(stderr, "error: failed to read WAV file '%s'\n", fname_inp.c_str());
        return -1;
    }

    float *buffer = NULL;
    unsigned int size = 0;
    std::vector<float> temp_f32;
    stream_open(fname_inp.c_str(), NULL, temp_f32);

    if (temp_f32.size() != pcmf16.size()) {
        std::cout << "error size" << std::endl;
        return -1;
    }

    std::vector<float> test_f32;
    test_f32.resize(pcmf16.size());
    for (uint64_t i = 0; i < pcmf16.size(); i++) {
        test_f32[i] = float(pcmf16[i])/32768.0f;
    }

    for (int i = 0; i < temp_f32.size(); i++) {
        if (temp_f32[i] != test_f32[i]) {
            std::cout << "not equals" << std::endl;
        }
    }

    AsrWhisperCtx *ctx = asr_create();
    if (!ctx) {
        std::cout << "asr_create" << std::endl;
        return -1;
    }

    if (asr_init(ctx, "C:\\workspace\\kplayer\\res\\ggml-small.bin", "en")) {
        std::cout << "asr_init" << std::endl;
        return -1;
    }
    int keep_ms = 200;
    int32_t step_ms    = 3000;
    int32_t length_ms  = 10000;
    keep_ms = std::min(keep_ms,   step_ms);
    length_ms = std::max(length_ms, step_ms);

    const int n_samples_step = (1e-3*step_ms  )*WHISPER_SAMPLE_RATE;
    const int n_samples_len  = (1e-3*length_ms)*WHISPER_SAMPLE_RATE;
    const int n_samples_keep = (1e-3*keep_ms  )*WHISPER_SAMPLE_RATE;
    const int n_samples_30s  = (1e-3*30000.0         )*WHISPER_SAMPLE_RATE;

    std::vector<float> pcmf32    (n_samples_30s, 0.0f);
    std::vector<float> pcmf32_old;
    std::vector<float> pcmf32_new(n_samples_30s, 0.0f);

    int idx = 0;
    int length = 1600;

    while (true) {
        if (idx >= temp_f32.size()) {
            break;
        }
        pcmf32_new.clear();
        if (idx + length < temp_f32.size()) {
            pcmf32_new.insert(pcmf32_new.end(), temp_f32.begin() + idx, temp_f32.begin() + idx + length);
            idx += length;
        } else {
            pcmf32_new.insert(pcmf32_new.end(), temp_f32.begin() + idx, temp_f32.end());
            idx = temp_f32.size();
        }

        const int n_samples_new = pcmf32_new.size();

        // take up to params.length_ms audio from previous iteration
        const int n_samples_take = std::min((int) pcmf32_old.size(), std::max(0, n_samples_keep + n_samples_len - n_samples_new));

        //printf("processing: take = %d, new = %d, old = %d\n", n_samples_take, n_samples_new, (int) pcmf32_old.size());

        pcmf32.resize(n_samples_new + n_samples_take);

        for (int i = 0; i < n_samples_take; i++) {
            pcmf32[i] = pcmf32_old[pcmf32_old.size() - n_samples_take + i];
        }

        memcpy(pcmf32.data() + n_samples_take, pcmf32_new.data(), n_samples_new*sizeof(float));

        pcmf32_old = pcmf32;

        if (asr_process(ctx, pcmf32.data(), pcmf32.size()) != 0) {
            std::cout << "asr_process error" << std::endl;
        }
    }

    return 0;
}