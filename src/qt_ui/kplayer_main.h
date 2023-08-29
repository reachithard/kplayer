#ifndef KPLAYERMAIN_H
#define KPLAYERMAIN_H

#include <QMainWindow>

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

private:
    bool SigConnect();
    Ui::KplayerMain *ui;
    KplayerPlayList playList_;
    KplayerTitle title_;
};
#endif // KPLAYERMAIN_H
