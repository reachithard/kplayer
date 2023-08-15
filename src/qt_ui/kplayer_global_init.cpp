//
// Created by luo on 2023/8/14.
//

#include "kplayer_global_init.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "player.h"
#include "kplayer_errors.h"

#ifdef __cplusplus
}
#endif

KplayerGlobalInit::KplayerGlobalInit() {

}

KplayerGlobalInit::~KplayerGlobalInit() {

}

bool KplayerGlobalInit::Init() {
    if (player_global_init() != 0) {
        return KPLAYER_GLOBAL_INIT_ERROR;
    }
    return true;
}
