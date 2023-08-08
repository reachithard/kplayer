#ifndef _PLAYER_UTILS_H_
#define _PLAYER_UTILS_H_

#include "player_pod.h"

int cmp_audio_fmts(enum AVSampleFormat fmt1, int64_t channel_count1,
                   enum AVSampleFormat fmt2, int64_t channel_count2);

#endif // _PLAYER_UTILS_H_