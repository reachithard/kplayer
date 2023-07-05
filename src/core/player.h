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

#endif // _PLAYER_H_