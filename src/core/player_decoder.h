#ifndef _PLAYER_DECODER_H_
#define _PLAYER_DECODER_H_

#include "player_pod.h"
#include "player_frame_queue.h"

int decoder_init(Decoder *d, AVCodecContext *avctx, PacketQueue *queue, SDL_cond *empty_queue_cond);

void decoder_destroy(Decoder *d);

void decoder_abort(Decoder *d, FrameQueue *fq);

int decoder_start(Decoder *d, int (*fn)(void *), const char *thread_name, void* arg);


#endif // _PLAYER_DECODER_H_