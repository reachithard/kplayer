#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "player_pod.h"

int32_t PlayerGlobalInit(const KPlayerGlobal *config);
void PlayerGlobalUnInit();

Player *CreatePlayer();
void DestroyPlayer(Player *player);
void ResetPlayer(Player *player);

int PlayerPlay(Player *player);
int PlayerStart(Player *player, uint64_t msec);

int audio_thread(void *arg);
void video_refresh(void *opaque, double *remaining_time);
int video_thread(void *arg);
int subtitle_thread(void *arg);

#endif // _PLAYER_H_