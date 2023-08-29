//
// Created by luo on 2023/8/10.
//

#ifndef KPLAYER_KPLAYER_TITLE_H
#define KPLAYER_KPLAYER_TITLE_H

#include <QWidget>
#include <QMenu>

QT_BEGIN_NAMESPACE
namespace Ui { class KplayerTitle; }
QT_END_NAMESPACE

class KplayerTitle : public QWidget {
Q_OBJECT

public:
    explicit KplayerTitle(QWidget *parent = nullptr);

    ~KplayerTitle() override;

    bool Init();

signals:
    void SigClose();
    void SigMin();
    void SigMaxOrNormal();
    void SigFullScreen();
private:
    Ui::KplayerTitle *ui;
};


#endif //KPLAYER_KPLAYER_TITLE_H
