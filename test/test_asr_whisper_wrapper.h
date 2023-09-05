//
// Created by luo on 2023/9/2.
//

#ifndef KPLAYER_ASR_WHISPER_WRAPPER_H
#define KPLAYER_ASR_WHISPER_WRAPPER_H

#include <stdint.h>

#include "whisper.h"

#ifdef __cplusplus
extern "C" {
#endif

// command-line parameters
typedef struct AsrWhisperCtx {
    int32_t n_threads;
    int32_t step_ms;
    int32_t length_ms;
    int32_t keep_ms;
    int32_t capture_id;
    int32_t max_tokens;
    int32_t audio_ctx;

    float vad_thold;
    float freq_thold;

    bool speed_up;
    bool translate;
    bool no_fallback;
    bool print_special;
    bool no_context;
    bool no_timestamps;

    struct whisper_context *ctx;
    const char *language;
    const char *model;

    int n_samples_step;
    int n_samples_len;
    int n_samples_keep;
    int n_samples_30s;

    bool use_vad; // sliding window mode uses VAD
    int n_new_line; // number of steps to print new line

    struct whisper_full_params wparams;
    void *buffer;
    FILE *file;

//    std::string language  = "en";
//    std::string model     = "models/ggml-base.en.bin";
//    std::string fname_out;
} AsrWhisperCtx;

AsrWhisperCtx *asr_create();

int asr_init(AsrWhisperCtx *asr, const char *model, const char *language);

int asr_process(AsrWhisperCtx *asr, const float * samples, int n_samples);

void asr_destroy(AsrWhisperCtx *asr);

#ifdef __cplusplus
}
#endif


#endif //KPLAYER_ASR_WHISPER_WRAPPER_H
