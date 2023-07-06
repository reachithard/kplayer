#ifndef _PLAYER_PACKET_QUEUE_H_
#define _PLAYER_PACKET_QUEUE_H_

#include "player_pod.h"

int packet_queue_put_private(PacketQueue *q, AVPacket *pkt);

int packet_queue_put(PacketQueue *q, AVPacket *pkt);

int packet_queue_put_nullpacket(PacketQueue *q, AVPacket *pkt, int stream_index);

int packet_queue_init(PacketQueue *q);

void packet_queue_flush(PacketQueue *q);

void packet_queue_destroy(PacketQueue *q);

void packet_queue_abort(PacketQueue *q);

void packet_queue_start(PacketQueue *q);

int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block, int *serial);

#endif // _PLAYER_PACKET_QUEUE_H_