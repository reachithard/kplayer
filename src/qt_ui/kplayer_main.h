#ifndef KPLAYERMAIN_H
#define KPLAYERMAIN_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QPropertyAnimation>
#include <QRect>

#include "kplayer_play_list.h"
#include "kplayer_title.h"

QT_BEGIN_NAMESPACE
namespace Ui { class KplayerMain; }
QT_END_NAMESPACE

class KplayerMain : public QMainWindow
{
    Q_OBJECT

public:
    KplayerMain(QWidget *parent = nullptr);
    ~KplayerMain();

    bool Init();

protected:
    void keyReleaseEvent(QKeyEvent *event);

private:
    void OnMin();
    void OnMaxOrNormal();
    void OnFullScreen();
    void OnClose();
    void OnShowOrHidePlaylist();

    bool SigConnect();

    QPropertyAnimation *ctrlAnimShow_ = nullptr;
    QPropertyAnimation *ctrlAnimHide_ = nullptr;
    QRect ctrlShow_;//控制面板显示区域
    QRect ctrlHidden_;//控制面板隐藏区域

    Ui::KplayerMain *ui;
    KplayerPlayList playList_;
    KplayerTitle title_;
};
#endif // KPLAYERMAIN_H
