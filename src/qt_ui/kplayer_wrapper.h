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

    int VideoPlay(const QString &url);

    int VideoToggleMute();

    int VideoUpdateVolume();

    int VideoNextFrame();

    int VideoStop();
private:
    Player *player = nullptr;
};


#endif //KPLAYER_KPLAYER_WRAPPER_H
