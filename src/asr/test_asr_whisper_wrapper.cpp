#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>

#define DR_WAV_IMPLEMENTATION

#include "asr_whisper_wrapper.h"
#include "test_dr_wav.h"

#define COMMON_SAMPLE_RATE 16000

bool read_wav(const std::string & fname, std::vector<float>& pcmf32, std::vector<std::vector<float>>& pcmf32s, bool stereo) {
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

    std::vector<int16_t> pcm16;
    pcm16.resize(n*wav.channels);
    drwav_read_pcm_frames_s16(&wav, n, pcm16.data());
    drwav_uninit(&wav);

    // convert to mono, float
    pcmf32.resize(n);
    if (wav.channels == 1) {
        for (uint64_t i = 0; i < n; i++) {
            pcmf32[i] = float(pcm16[i])/32768.0f;
        }
    } else {
        for (uint64_t i = 0; i < n; i++) {
            pcmf32[i] = float(pcm16[2*i] + pcm16[2*i + 1])/65536.0f;
        }
    }

    if (stereo) {
        // convert to stereo, float
        pcmf32s.resize(2);

        pcmf32s[0].resize(n);
        pcmf32s[1].resize(n);
        for (uint64_t i = 0; i < n; i++) {
            pcmf32s[0][i] = float(pcm16[2*i])/32768.0f;
            pcmf32s[1][i] = float(pcm16[2*i + 1])/32768.0f;
        }
    }

    return true;
}

int main() {
    std::cout << "hello" << std::endl;

    AsrWhisperCtx *ctx = asr_create();
    if (!ctx) {
        std::cout << "asr_create" << std::endl;
        return -1;
    }

    if (asr_init(ctx, "C:\\workspace\\kplayer\\res\\ggml-small.bin", "en")) {
        std::cout << "asr_init" << std::endl;
        return -1;
    }

    bool loop = true;

    std::vector<float> pcmf32;               // mono-channel F32 PCM
    std::vector<std::vector<float>> pcmf32s; // stereo-channel F32 PCM
    std::string fname_inp = "C:\\workspace\\kplayer\\res\\jfk.wav";

    if (!::read_wav(fname_inp, pcmf32, pcmf32s, false)) {
        fprintf(stderr, "error: failed to read WAV file '%s'\n", fname_inp.c_str());
        return -1;
    }

    std::cout << "length" << pcmf32.size() << std::endl;
    int idx = 0;
    int step = 176000 / 5;
    int length = step;
    while (loop) {
        if (idx > pcmf32.size() - 1) {
            break;
        }

        if (idx + length > pcmf32.size()) {
            length = pcmf32.size() - idx;
        }

        if (asr_process(ctx, pcmf32.data() + idx, length) != 0) {
            std::cout << "asr_process error" << std::endl;
            length += step;
        } else {
            std::cout << "asr_process success:" << idx << " length" << length << std::endl;
            idx += length;
            length = step;
        }

    }

    return 0;
}