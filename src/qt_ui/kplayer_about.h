//
// Created by luo on 2023/8/10.
//

#ifndef KPLAYER_KPLAYER_ABOUT_H
#define KPLAYER_KPLAYER_ABOUT_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class KplayerAbout; }
QT_END_NAMESPACE

class KplayerAbout : public QWidget {
Q_OBJECT

public:
    explicit KplayerAbout(QWidget *parent = nullptr);

    ~KplayerAbout() override;

private:
    Ui::KplayerAbout *ui;
};


#endif //KPLAYER_KPLAYER_ABOUT_H
