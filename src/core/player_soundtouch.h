//
// Created by luo on 2023/8/30.
//

#ifndef KPLAYER_PLAYER_SOUNDTOUCH_H
#define KPLAYER_PLAYER_SOUNDTOUCH_H

#include "soundtouch/SoundTouchDLL.h"

typedef struct PlayerSoundtouch {
    HANDLE handle;
} PlayerSoundtouch;

PlayerSoundtouch *create_touch();

int touch_translate(PlayerSoundtouch *touch, short* data, float speed, float pitch, int len, int bytes_per_sample, int channel, int sample_rate);

void destroy_touch(PlayerSoundtouch *touch);

#endif //KPLAYER_PLAYER_SOUNDTOUCH_H
