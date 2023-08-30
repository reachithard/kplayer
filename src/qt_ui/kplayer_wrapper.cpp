//
// Created by luo on 2023/8/10.
//

#include "kplayer_wrapper.h"

#include "kplayer_errors.h"

KplayerWrapper::KplayerWrapper() {

}

KplayerWrapper::~KplayerWrapper() {
    if (player_) {
        // 进行停止
        player_stop(player_);
        player_destroy(player_);
    }
}

int KplayerWrapper::Init() {
    if (player_) {
        return KPLAYER_WRAPPER_DUPLICUTE_INIT_ERROR;
    }
    player_ = player_create();
    if (!player_) {
        return KPLAYER_WRAPPER_INIT_ERROR;
    }

    player_->userdata = this;
    player_->callbacks.video_dimensions_changed = &KplayerWrapper::VideoDimensionsChanged;
    player_->callbacks.video_play_seconds = &KplayerWrapper::VideoPlaySeconds;
    player_->callbacks.video_total_seconds = &KplayerWrapper::VideoTotalSeconds;
    player_->callbacks.video_stop = &KplayerWrapper::VideoStop;
    player_->callbacks.video_volume = &KplayerWrapper::VideoVolume;
    player_->callbacks.video_pause_state = &KplayerWrapper::VideoPauseState;
    player_->callbacks.video_stop_finished = &KplayerWrapper::VideoStopFinished;
    player_->callbacks.video_play_start = &KplayerWrapper::VideoPlayStart;
    return 0;
}

int KplayerWrapper::VideoPlay(const QString &url) {
    if (!player_) {
        return KPLAYER_WRAPPER_INIT_ERROR;
    }

    if (!player_->input_filename) {
        // 进行拷贝
        player_->input_filename = strdup(url.toStdString().c_str());
        if (player_play(player_) != 0) {
            return KPLAYER_WRAPPER_PLAY_ERROR;
        }
        return KPLAYER_SUCCESS;
    }

    // todo 返回值之类判断
    if (strcmp(url.toStdString().c_str(), player_->input_filename)) {
        player_stop(player_);
    } else {
        player_pause(player_);
    }
    return KPLAYER_SUCCESS;
}

int KplayerWrapper::VideoToggleMute() {
    if (!player_) {
        return KPLAYER_WRAPPER_INIT_ERROR;
    }

    player_mute(player_);
    return KPLAYER_SUCCESS;
}

void KplayerWrapper::SetWid(void *wid) {
    player_->w_id = wid;
}

void KplayerWrapper::VideoDimensionsChanged(void *userdata, int width, int height) {
    if (userdata) {
        KplayerWrapper *wrapper = reinterpret_cast<KplayerWrapper*>(userdata);
        emit wrapper->SigVideoDimensionsChanged(width, height);
    }
}

void KplayerWrapper::VideoPlaySeconds(void *userdata, int sec) {
    if (userdata) {
        KplayerWrapper *wrapper = reinterpret_cast<KplayerWrapper*>(userdata);
        emit wrapper->SigVideoPlaySeconds(sec);
    }
}

void KplayerWrapper::VideoTotalSeconds(void *userdata, int sec) {
    if (userdata) {
        KplayerWrapper *wrapper = reinterpret_cast<KplayerWrapper*>(userdata);
        emit wrapper->SigVideoTotalSeconds(sec);
    }
}

void KplayerWrapper::VideoStop(void *userdata) {
    if (userdata) {
        KplayerWrapper *wrapper = reinterpret_cast<KplayerWrapper*>(userdata);
        emit wrapper->SigVideoStop();
    }
}

void KplayerWrapper::VideoVolume(void *userdata, double volume) {
    if (userdata) {
        KplayerWrapper *wrapper = reinterpret_cast<KplayerWrapper*>(userdata);
        emit wrapper->SigVideoVolume(volume);
    }
}

void KplayerWrapper::VideoPauseState(void *userdata, int state) {
    if (userdata) {
        KplayerWrapper *wrapper = reinterpret_cast<KplayerWrapper*>(userdata);
        emit wrapper->SigVideoPauseState(state);
    }
}

void KplayerWrapper::VideoStopFinished(void *userdata) {
    if (userdata) {
        KplayerWrapper *wrapper = reinterpret_cast<KplayerWrapper*>(userdata);
        emit wrapper->SigVideoStopFinished();
    }
}

void KplayerWrapper::VideoPlayStart(void *userdata, const char *filename) {
    if (userdata) {
        KplayerWrapper *wrapper = reinterpret_cast<KplayerWrapper*>(userdata);
        emit wrapper->SigVideoPlayStart(filename);
    }
}

int KplayerWrapper::VideoSeek(double incr) {
    if (!player_) {
        return KPLAYER_WRAPPER_INIT_ERROR;
    }

    player_seek(player_, incr);
    return KPLAYER_SUCCESS;
}

int KplayerWrapper::VideoSetSpeed(float speed) {
    if (!player_) {
        return KPLAYER_WRAPPER_INIT_ERROR;
    }

    player_set_speed(player_, speed);
    return 0;
}
