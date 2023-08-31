//
// Created by luo on 2023/8/10.
//

#ifndef KPLAYER_KPLAYER_SHOW_H
#define KPLAYER_KPLAYER_SHOW_H

#include <QWidget>

#include "kplayer_wrapper.h"

#include <mutex>

QT_BEGIN_NAMESPACE
namespace Ui { class KplayerShow; }
QT_END_NAMESPACE

class KplayerShow : public QWidget {
Q_OBJECT

public:
    explicit KplayerShow(QWidget *parent = nullptr);

    ~KplayerShow() override;

    bool Init();

    int PlayOrPause(const QString &url);

    int ToggleMute();

    int Seek(double incr);

    int SetSpeed(float speed);

    void Close();

    void OnVolumeChanged(float volume);
signals:
    void SigShowFullScreen();
    void SigVideoPlaySeconds(int sec);
    void SigVideoTotalSeconds(int sec);
    void SigVideoStop();
    void SigVideoVolume(double volume);
    void SigVideoPauseState(int state);
    void SigVideoStopFinished();
    void SigVideoPlayStart(const char *filename);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void AdjustSizeShow();

    void OnVideoDimensionsChanged(int width, int height);

    Ui::KplayerShow *ui;
    KplayerWrapper wrapper_;
    uint32_t width_ = 0;
    uint32_t height_ = 0;
    std::mutex refresh_lock_;
};


#endif //KPLAYER_KPLAYER_SHOW_H
