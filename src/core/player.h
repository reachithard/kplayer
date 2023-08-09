#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "player_pod.h"

int  player_global_init();

Player *player_create();

int player_play(Player *player);

#endif // _PLAYER_H_