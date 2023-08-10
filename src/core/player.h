#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "player_pod.h"

int  player_global_init();

Player *player_create();

int player_play(Player *player);

int player_pause(Player *player);

int player_mute(Player *player);

int player_update_volume(Player *player, int incr);

int player_next_frame(Player *player);

int player_stop(Player *player);

void player_destroy(Player *player);

#endif // _PLAYER_H_