//
// Created by luo on 2023/8/10.
//

#include "kplayer_wrapper.h"

#include "kplayer_errors.h"

KplayerWrapper::KplayerWrapper() {

}

KplayerWrapper::~KplayerWrapper() {
    if (player) {
        // 进行停止
        player_stop(player);
        player_destroy(player);
    }
}

int KplayerWrapper::Init() {
    if (player) {
        return KPLAYER_WRAPPER_DUPLICUTE_INIT_ERROR;
    }
    player = player_create();
    if (!player) {
        return KPLAYER_WRAPPER_INIT_ERROR;
    }

    return 0;
}

int KplayerWrapper::VideoPlay(const QString &url) {
    if (!player) {
        return KPLAYER_WRAPPER_INIT_ERROR;
    }

    if (!player->input_filename) {
        // 进行拷贝
        player->input_filename = strdup(url.toStdString().c_str());
        if (player_play(player) != 0) {
            return KPLAYER_WRAPPER_PLAY_ERROR;
        }
        return KPLAYER_SUCCESS;
    }

    // todo 返回值之类判断
    if (strcmp(url.toStdString().c_str(), player->input_filename)) {
        player_stop(player);
    } else {
        player_pause(player);
    }
    return KPLAYER_SUCCESS;
}

int KplayerWrapper::VideoToggleMute() {
    if (!player) {
        return KPLAYER_WRAPPER_INIT_ERROR;
    }

    player_mute(player);
    return KPLAYER_SUCCESS;
}

void KplayerWrapper::SetWid(void *wid) {
    player->w_id = wid;
}
