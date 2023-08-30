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
signals:
    void SigShowFullScreen();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void OnVideoPlaySeconds(int sec);
    void OnVideoTotalSeconds(int sec);
    void OnVideoStop();
    void OnVideoVolume(double volume);
    void OnVideoPauseState(int state);
    void OnVideoStopFinished();
    void OnVideoPlayStart(const char *filename);

    void AdjustSizeShow();

    void OnVideoDimensionsChanged(int width, int height);

    Ui::KplayerShow *ui;
    KplayerWrapper wrapper_;
    uint32_t width_ = 0;
    uint32_t height_ = 0;
    std::mutex refresh_lock_;
};


#endif //KPLAYER_KPLAYER_SHOW_H
