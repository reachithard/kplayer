//
// Created by luo on 2023/8/8.
//

#ifdef __cplusplus
extern "C" {
#endif

#include "player.h"

#ifdef __cplusplus
}
#endif

#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>

#undef main
int main(int argc, char **argv) {
#if defined(_WIN32)
    /* Calling SetDllDirectory with the empty string (but not NULL) removes the
     * current working directory from the DLL search path as a security pre-caution. */
    SetDllDirectory("C:\\workspace\\kplayer\\3rd\\windows\\bin");
#endif
    int ret = player_global_init();
    if (ret) {
        return ret;
    }

    Player *player = player_create();
    if (!player) {
        return -1;
    }

    player->input_filename = "C:\\Users\\luo\\Documents\\Captura\\2023-08-09\\02-40-14.mp4";
    ret = player_play(player);
    if (ret) {
        return -1;
    }

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}