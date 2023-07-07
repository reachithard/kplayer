#ifndef _PLAYER_UTILS_H_
#define _PLAYER_UTILS_H_

#include "player_pod.h"
#include "player_def.h"

#if CONFIG_AVFILTER
int opt_add_vfilter(void *optctx, const char *opt, const char *arg);
#endif

int cmp_audio_fmts(enum AVSampleFormat fmt1, int64_t channel_count1,
                   enum AVSampleFormat fmt2, int64_t channel_count2);

void fill_rectangle(Player *player, int x, int y, int w, int h);

int compute_mod(int a, int b);

#endif // _PLAYER_UTILS_H_