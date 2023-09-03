//
// Created by luo on 2023/9/2.
//

#include "player_asr_queue.h"

int asr_queue_put(AsrQueue *q, AsrPacket *pkt) {
    AsrPacket pkt1;
    int ret;

    uint8_t *data = malloc(pkt->data_size);
    if (!data) {
        pkt->data = NULL;
        pkt->data_size = 0;
        return -1;
    }

    memcpy(data, pkt->data, pkt->data_size);
    pkt1.data = data;
    pkt1.data_size = pkt->data_size;
//    av_log(NULL, AV_LOG_WARNING, "asr queue put:%d %d\n", pkt1.data_size, pkt->data_size);

    pkt->data = NULL;
    pkt->data_size = 0;

    SDL_LockMutex(q->mutex);
    ret = av_fifo_write(q->pkt_list, &pkt1, 1);
    SDL_CondSignal(q->cond);
    SDL_UnlockMutex(q->mutex);

    return ret;
}

int asr_queue_init(AsrQueue *q) {
    memset(q, 0, sizeof(AsrQueue));
    q->pkt_list = av_fifo_alloc2(1, sizeof(AsrPacket), AV_FIFO_FLAG_AUTO_GROW);
    if (!q->pkt_list)
        return AVERROR(ENOMEM);
    q->mutex = SDL_CreateMutex();
    if (!q->mutex) {
        av_log(NULL, AV_LOG_FATAL, "SDL_CreateMutex(): %s\n", SDL_GetError());
        return AVERROR(ENOMEM);
    }
    q->cond = SDL_CreateCond();
    if (!q->cond) {
        av_log(NULL, AV_LOG_FATAL, "SDL_CreateCond(): %s\n", SDL_GetError());
        return AVERROR(ENOMEM);
    }
    return 0;
}

void asr_queue_flush(AsrQueue *q) {
    AsrPacket pkt1;

    SDL_LockMutex(q->mutex);
    while (av_fifo_read(q->pkt_list, &pkt1, 1) >= 0) {
        // av_packet_free(&pkt1.pkt); // 进行释放
        free((void*)pkt1.data);
        pkt1.data_size = 0;
    }
    SDL_UnlockMutex(q->mutex);
}

void asr_queue_destroy(AsrQueue *q) {
    asr_queue_flush(q);
    av_fifo_freep2(&q->pkt_list);
    SDL_DestroyMutex(q->mutex);
    SDL_DestroyCond(q->cond);
}

void asr_queue_abort(AsrQueue *q) {
    SDL_LockMutex(q->mutex);


    SDL_CondSignal(q->cond);

    SDL_UnlockMutex(q->mutex);
}

void asr_queue_start(AsrQueue *q) {
    SDL_LockMutex(q->mutex);
    SDL_UnlockMutex(q->mutex);
}

int asr_queue_get(AsrQueue *q, AsrPacket *pkt, int block) {
    AsrPacket pkt1;
    int ret;

    SDL_LockMutex(q->mutex);

    for (;;) {
//        if (q->abort_request) {
//            ret = -1;
//            break;
//        }

        if (av_fifo_read(q->pkt_list, &pkt1, 1) >= 0) {
            pkt->data = pkt1.data;
            pkt->data_size = pkt1.data_size;
            ret = 1;
            break;
        } else if (!block) {
            ret = 0;
            break;
        } else {
            SDL_CondWait(q->cond, q->mutex);
        }
    }
    SDL_UnlockMutex(q->mutex);
    return ret;
}

//int asr_queue_peek(AsrQueue *q, AsrPacket *pkt, int block) {
//    AsrPacket pkt1;
//    int ret;
//
//    SDL_LockMutex(q->mutex);
//
//    for (;;) {
////        if (q->abort_request) {
////            ret = -1;
////            break;
////        }
//
//        if (av_fifo_peek(q->pkt_list, &pkt1, 1) >= 0) {
//            pkt->data = pkt1.data;
//            pkt->data_size = pkt1.data_size;
//            ret = 1;
//            break;
//        } else if (!block) {
//            ret = 0;
//            break;
//        } else {
//            SDL_CondWait(q->cond, q->mutex);
//        }
//    }
//    SDL_UnlockMutex(q->mutex);
//    return ret;
//}