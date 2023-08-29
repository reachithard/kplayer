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

signals:
    void SigShowOrHidePlaylist();
private:
    void SigConnect();
    Ui::KplayerCtrl *ui;
};


#endif //KPLAYER_KPLAYER_CTRL_H
