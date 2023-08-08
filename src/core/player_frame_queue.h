#ifndef _PLAYER_FRAME_QUEUE_H_
#define _PLAYER_FRAME_QUEUE_H_

#include "player_pod.h"

int frame_queue_init(FrameQueue *f, PacketQueue *pktq, int max_size, int keep_last);

void frame_queue_destory(FrameQueue *f);

void frame_queue_signal(FrameQueue *f);

Frame *frame_queue_peek(FrameQueue *f);

Frame *frame_queue_peek_next(FrameQueue *f);

Frame *frame_queue_peek_last(FrameQueue *f);

Frame *frame_queue_peek_writable(FrameQueue *f);

Frame *frame_queue_peek_readable(FrameQueue *f);

void frame_queue_push(FrameQueue *f);

void frame_queue_next(FrameQueue *f);

int frame_queue_nb_remaining(FrameQueue *f);

int64_t frame_queue_last_pos(FrameQueue *f);


#endif // _PLAYER_FRAME_QUEUE_H_