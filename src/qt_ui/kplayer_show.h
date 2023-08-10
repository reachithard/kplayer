//
// Created by luo on 2023/8/10.
//

#ifndef KPLAYER_KPLAYER_SHOW_H
#define KPLAYER_KPLAYER_SHOW_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class KplayerShow; }
QT_END_NAMESPACE

class KplayerShow : public QWidget {
Q_OBJECT

public:
    explicit KplayerShow(QWidget *parent = nullptr);

    ~KplayerShow() override;

private:
    Ui::KplayerShow *ui;
};


#endif //KPLAYER_KPLAYER_SHOW_H
