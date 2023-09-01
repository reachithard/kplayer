//
// Created by luo on 2023/8/10.
//

#ifndef KPLAYER_KPLAYER_CTRL_H
#define KPLAYER_KPLAYER_CTRL_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class KplayerCtrl; }
QT_END_NAMESPACE

class KplayerCtrl : public QWidget {
Q_OBJECT

public:
    explicit KplayerCtrl(QWidget *parent = nullptr);

    ~KplayerCtrl() override;

    bool Init();

    void OnVideoPlaySeconds(int sec);
    void OnVideoTotalSeconds(int sec);
    void OnVideoStop();
    void OnVideoVolume(double volume);
    void OnVideoPauseState(int state);
    void OnVideoStopFinished();
    void OnVideoPlayStart(const char *filename);
    void OnPlayerChanged(int value);
    void OnVolumeChanged(int value);

public slots:
    void OnSpeedChanged(int idx);

signals:
    void SigPlayOrPause();
    void SigMute();
    void SigForward();
    void SigBackward();
    void SigShowOrHidePlaylist();
    void SigSpeedChanged(float speed);
    void SigVolumeChanged(float volume);
    void SigPlayerChanged(float volume);
private:
    void SigConnect();
    Ui::KplayerCtrl *ui;
    int totalTime_;
};


#endif //KPLAYER_KPLAYER_CTRL_H
