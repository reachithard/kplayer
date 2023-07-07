#ifndef _PLAYER_INTERNAL_H_
#define _PLAYER_INTERNAL_H_

#include "player_pod.h"

int decoder_decode_frame(Player *player, Decoder *d, AVFrame *frame, AVSubtitle *sub);

#endif // _PLAYER_INTERNAL_H_