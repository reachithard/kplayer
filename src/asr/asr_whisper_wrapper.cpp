//
// Created by luo on 2023/9/2.
//

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "asr_whisper_wrapper.h"

#ifdef __cplusplus
extern "C" {
#endif

AsrWhisperCtx *asr_create() {
    AsrWhisperCtx* asr = (AsrWhisperCtx*)malloc(sizeof(AsrWhisperCtx));
    if (!asr) {
        return asr;
    }

    memset(asr, 0, sizeof(AsrWhisperCtx));
    // 设置默认值
    asr->n_threads  = 1;
    asr->step_ms    = 3000;
    asr->length_ms  = 10000;
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

    return 0;
}

int asr_process(AsrWhisperCtx *asr, const float * samples, int n_samples) {
    // 内存的处理，由上层控制
    whisper_full_params wparams = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);

    wparams.print_progress   = false;
    wparams.print_special    = asr->print_special;
    wparams.print_realtime   = false;
    wparams.print_timestamps = !asr->no_timestamps;
    wparams.translate        = asr->translate;
    wparams.single_segment   = !asr->use_vad;
    wparams.max_tokens       = asr->max_tokens;
    wparams.language         = asr->language;
    wparams.n_threads        = asr->n_threads;

    wparams.audio_ctx        = asr->audio_ctx;
    wparams.speed_up         = asr->speed_up;

    // disable temperature fallback
    //wparams.temperature_inc  = -1.0f;
    wparams.temperature_inc  = asr->no_fallback ? 0.0f : wparams.temperature_inc;

    wparams.prompt_tokens = NULL;
    wparams.prompt_n_tokens = 0;
//    wparams.prompt_tokens    = asr->no_context ? nullptr : prompt_tokens.data();
//    wparams.prompt_n_tokens  = asr->no_context ? 0       : prompt_tokens.size();

    // 进行处理
    printf("process begin\n");
    if (whisper_full(asr->ctx, wparams, samples, n_samples) != 0) {
        return -1;
    }

    const int n_segments = whisper_full_n_segments(asr->ctx);
    for (int i = 0; i < n_segments; ++i) {
        const char * text = whisper_full_get_segment_text(asr->ctx, i);
        printf("get trans:%s\n", text);
    }
    printf("process end\n");
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

#ifdef __cplusplus
}
#endif