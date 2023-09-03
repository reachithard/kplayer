//
// Created by luo on 2023/9/2.
//

#ifndef KPLAYER_PLAYER_ASR_QUEUE_H
#define KPLAYER_PLAYER_ASR_QUEUE_H

#include "player_pod.h"

int asr_queue_put(AsrQueue *q, AsrPacket *pkt);

int asr_queue_init(AsrQueue *q);

void asr_queue_flush(AsrQueue *q);

void asr_queue_destroy(AsrQueue *q);

void asr_queue_abort(AsrQueue *q);

void asr_queue_start(AsrQueue *q);

int asr_queue_get(AsrQueue *q, AsrPacket *pkt, int block);

int asr_queue_peek(AsrQueue *q, AsrPacket *pkt, int block);

#endif //KPLAYER_PLAYER_ASR_QUEUE_H
