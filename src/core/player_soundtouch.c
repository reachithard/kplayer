//
// Created by luo on 2023/8/30.
//

#include <malloc.h>
#include "player_soundtouch.h"

PlayerSoundtouch *create_touch() {
    PlayerSoundtouch *touch = (PlayerSoundtouch*)malloc(sizeof(PlayerSoundtouch));
    touch->handle = soundtouch_createInstance();
    return touch;
}

int touch_translate(PlayerSoundtouch *touch, short* data, float speed, float pitch, int len, int bytes_per_sample, int channel, int sample_rate) {
    int put_n_sample = len / channel;
    int nb = 0;
    int pcm_data_size = 0;
    soundtouch_setPitch(touch->handle, pitch);
    soundtouch_setRate(touch->handle, speed);

    soundtouch_setSampleRate(touch->handle, sample_rate);
    soundtouch_setChannels(touch->handle, channel);

    soundtouch_putSamples_i16(touch->handle, data, put_n_sample);

    do {
        nb = soundtouch_receiveSamples_i16(touch->handle, data, sample_rate / channel);
        pcm_data_size += nb * channel * bytes_per_sample;
    } while (nb != 0);

    return pcm_data_size;
}

void destroy_touch(PlayerSoundtouch *touch) {
    if (touch) {
        soundtouch_destroyInstance(touch->handle);
        free(touch);
    }
}
