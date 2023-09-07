//
// Created by luo on 2023/9/2.
//

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include "asr_whisper_wrapper.h"
#include "dr_wav.h"

//#ifdef __cplusplus
//extern "C" {
//#endif

struct AsrBufferCtx {
    std::vector<float> pcmf32;
    std::vector<float> pcmf32_old;
    std::vector<float> pcmf32_new;
};

AsrWhisperCtx *asr_create() {
    AsrWhisperCtx* asr = (AsrWhisperCtx*)malloc(sizeof(AsrWhisperCtx));
    if (!asr) {
        return asr;
    }

    memset(asr, 0, sizeof(AsrWhisperCtx));
    // 设置默认值
    asr->n_threads  = 2;
    asr->step_ms    = 1000;
    asr->length_ms  = 1500;
    asr->keep_ms    = 200;
    asr->capture_id = -1;
    asr->max_tokens = 32;
    asr->audio_ctx  = 0;

    asr->vad_thold    = 0.6f;
    asr->freq_thold   = 100.0f;

    asr->speed_up      = false;
    asr->translate     = false;
    asr->no_fallback   = false;
    asr->print_special = false;
    asr->no_context    = true;
    asr->no_timestamps = false;

    asr->ctx = NULL;
    asr->model = NULL;
    asr->language = NULL;
    return asr;
}

int asr_init(AsrWhisperCtx *asr, const char *model, const char *language) {
    if (!asr || !model || !language) {
        return -1;
    }

    // 参数修正
    asr->keep_ms   = fmin(asr->keep_ms,   asr->step_ms);
    asr->length_ms = fmax(asr->length_ms, asr->step_ms);

    asr->n_samples_step = (1e-3*asr->step_ms  )*WHISPER_SAMPLE_RATE;
    asr->n_samples_len  = (1e-3*asr->length_ms)*WHISPER_SAMPLE_RATE;
    asr->n_samples_keep = (1e-3*asr->keep_ms  )*WHISPER_SAMPLE_RATE;
    asr->n_samples_30s  = (1e-3*30000.0         )*WHISPER_SAMPLE_RATE;

    asr->use_vad = asr->n_samples_step <= 0; // sliding window mode uses VAD

    asr->n_new_line = !asr->use_vad ? fmax(1, asr->length_ms / asr->step_ms - 1) : 1; // number of steps to print new line

    asr->no_timestamps  = !asr->use_vad;
    asr->no_context    |= asr->use_vad;
    asr->max_tokens     = 0;

    asr->language = strdup(language);
    asr->model = strdup(model);

    if (strcmp("auto", asr->language) && whisper_lang_id(asr->language) == -1) {
        return -1;
    }

    asr->ctx = whisper_init_from_file(model);
    if (!asr->ctx) {
        return -1;
    }

    if (!whisper_is_multilingual(asr->ctx)) {
        if (asr->language != "en" || asr->translate) {
            asr->language = "en";
            asr->translate = false;
        }
    }

    // 内存的处理，由上层控制
    asr->wparams = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);

    asr->wparams.print_progress   = false;
    asr->wparams.print_special    = asr->print_special;
    asr->wparams.print_realtime   = false;
    asr->wparams.print_timestamps = !asr->no_timestamps;
    asr->wparams.translate        = asr->translate;
    asr->wparams.single_segment   = !asr->use_vad;
    asr->wparams.max_tokens       = asr->max_tokens;
    asr->wparams.language         = asr->language;
    asr->wparams.n_threads        = asr->n_threads;

    asr->wparams.audio_ctx        = asr->audio_ctx;
    asr->wparams.speed_up         = asr->speed_up;

    // disable temperature fallback
    //asr->wparams.temperature_inc  = -1.0f;
    asr->wparams.temperature_inc  = asr->no_fallback ? 0.0f : asr->wparams.temperature_inc;

    asr->wparams.prompt_tokens = NULL;
    asr->wparams.prompt_n_tokens = 0;

//    asr->buffer = malloc(sizeof(AsrBufferCtx));
//    AsrBufferCtx *buffer = (AsrBufferCtx*)asr->buffer;
//    buffer->pcmf32.reserve(asr->n_samples_30s);
//    std::fill(buffer->pcmf32.begin(), buffer->pcmf32.end(), 0.0f);
//    buffer->pcmf32_old;
//    buffer->pcmf32_new.reserve(asr->n_samples_30s);
//    std::fill(buffer->pcmf32_new.begin(), buffer->pcmf32_new.end(), 0.0f);

//    asr->file = fopen("test2.log", "aw+");
    return 0;
}

int asr_process(AsrWhisperCtx *asr, const float * samples, int n_samples) {

//    asr->wparams.prompt_tokens    = asr->no_context ? nullptr : prompt_tokens.data();
//    asr->wparams.prompt_n_tokens  = asr->no_context ? 0       : prompt_tokens.size();

    if (false) {
        AsrBufferCtx *buffer = (AsrBufferCtx*)asr->buffer;
        buffer->pcmf32_new.resize(n_samples);
        memcpy(buffer->pcmf32_new.data(), samples, n_samples);
        const int n_samples_new = buffer->pcmf32_new.size();

        // take up to params.length_ms audio from previous iteration
        std::cout << "test" << asr->n_samples_keep << " " << asr->n_samples_len << " " << n_samples_new << std::endl;
        const int n_samples_take = std::min((int) buffer->pcmf32_old.size(), std::max(0, asr->n_samples_keep + asr->n_samples_len - n_samples_new));

        printf("processing: take = %d, new = %d, old = %d\n", n_samples_take, n_samples_new, (int) buffer->pcmf32_old.size());

        buffer->pcmf32.resize(n_samples_new + n_samples_take);

        for (int i = 0; i < n_samples_take; i++) {
            buffer->pcmf32[i] = buffer->pcmf32_old[buffer->pcmf32_old.size() - n_samples_take + i];
        }

        memcpy(buffer->pcmf32.data() + n_samples_take, buffer->pcmf32_new.data(), n_samples_new*sizeof(float));

        buffer->pcmf32_old = buffer->pcmf32;

        printf("sample step:%d %d\n", asr->n_samples_step, n_samples);

        // 进行处理
        printf("process begin %d\n", (int)buffer->pcmf32.size());
        if (whisper_full(asr->ctx, asr->wparams, buffer->pcmf32.data(), buffer->pcmf32.size()) != 0) {
            return -1;
        }

        const int n_segments = whisper_full_n_segments(asr->ctx);
        for (int i = 0; i < n_segments; ++i) {
            const char * text = whisper_full_get_segment_text(asr->ctx, i);
            printf("get trans:%s\n", text);
        }
        printf("process end\n");
    } else {
        if (whisper_full(asr->ctx, asr->wparams, samples, n_samples) != 0) {
            return -1;
        }

        const int n_segments = whisper_full_n_segments(asr->ctx);
        for (int i = 0; i < n_segments; ++i) {
            const char * text = whisper_full_get_segment_text(asr->ctx, i);
            printf("get trans:%s\n", text);
        }
//        AsrBufferCtx *buffer = (AsrBufferCtx*)asr->buffer;
//        printf("process begin %d\n", (int)buffer->pcmf32.size());
//        buffer->pcmf32_new.resize(n_samples);
//        memcpy(buffer->pcmf32_new.data(), samples, n_samples);
//
//        buffer->pcmf32.insert(buffer->pcmf32.end(), buffer->pcmf32_new.begin(), buffer->pcmf32_new.end());
//        if (buffer->pcmf32.size() == 16384) {
//            // 进行处理
//            for (int idx = 0; idx < buffer->pcmf32.size(); idx++) {
//                fprintf(asr->file, "%x", *(int*)&buffer->pcmf32[idx]);
//            }
//
////            if (whisper_full(asr->ctx, asr->wparams, buffer->pcmf32.data(), buffer->pcmf32.size()) != 0) {
////                return -1;
////            }
////
////            const int n_segments = whisper_full_n_segments(asr->ctx);
////            for (int i = 0; i < n_segments; ++i) {
////                const char * text = whisper_full_get_segment_text(asr->ctx, i);
////                printf("get trans:%s\n", text);
////            }
////            printf("process end\n");
//            buffer->pcmf32.clear();
//        }
    }


//    if (n_samples > 2 * asr->n_samples_step) {
//        printf("cannot process audio fast enough, dropping audio\n");
//        return -1;
//    }

//    if (n_samples < asr->n_samples_step / 2) {
//        printf("too small\n");
//        return -1;
//    }


    return 0;
}

void asr_destroy(AsrWhisperCtx *asr) {
    if (asr->language) {
        free((void*)asr->language);
    }

    if (asr->model) {
        free((void*)asr->model);
    }

    free((void*)asr);
}

//#ifdef __cplusplus
//}
//#endif