//
// Created by luo on 2023/8/10.
//

#ifndef KPLAYER_KPLAYER_WRAPPER_H
#define KPLAYER_KPLAYER_WRAPPER_H

#include <QObject>

#ifdef __cplusplus
extern "C" {
#endif

#include "player.h"

#ifdef __cplusplus
}
#endif

class KplayerWrapper : public QObject {
Q_OBJECT
public:
    KplayerWrapper();

    ~KplayerWrapper();

    int Init();

    void SetWid(void *wid);

    int VideoPlay(const QString &url);

    int VideoToggleMute();

    int VideoUpdateVolume();

    int VideoNextFrame();

    int VideoStop();
private:
    static void VideoDimensionsChanged(void *userdata, int width, int height);
    static void VideoPlaySeconds(void *userdata, int sec);
    static void VideoTotalSeconds(void *userdata, int sec);
    static void VideoStop(void *userdata);
    static void VideoVolume(void *userdata, double volume);
    static void VideoPauseState(void *userdata, int state);
    static void VideoStopFinished(void *userdata);
    static void VideoPlayStart(void *userdata, const char *filename);
    Player *player_ = nullptr;
};


#endif //KPLAYER_KPLAYER_WRAPPER_H
