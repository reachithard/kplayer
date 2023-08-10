//
// Created by luo on 2023/8/10.
//

#ifndef KPLAYER_KPLAYER_PLAY_LIST_H
#define KPLAYER_KPLAYER_PLAY_LIST_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class KplayerPlayList; }
QT_END_NAMESPACE

class KplayerPlayList : public QWidget {
Q_OBJECT

public:
    explicit KplayerPlayList(QWidget *parent = nullptr);

    ~KplayerPlayList() override;

private:
    Ui::KplayerPlayList *ui;
};


#endif //KPLAYER_KPLAYER_PLAY_LIST_H
