#include "kplayer_main.h"
#include "./ui_kplayer_main.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "player.h"

#ifdef __cplusplus
}
#endif

KplayerMain::KplayerMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::KplayerMain)
    , playList_(this)
    , title_(this)
{
    ui->setupUi(this);

    // todo style

    setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowSystemMenuHint*/ | Qt::WindowMinimizeButtonHint);
    setMouseTracking(true);
}

KplayerMain::~KplayerMain()
{
    delete ui;
}

bool KplayerMain::Init() {
    QWidget *em = new QWidget(this);
    ui->PlaylistWid->setTitleBarWidget(em);
    ui->PlaylistWid->setWidget(&playList_);

    QWidget *emTitle = new QWidget(this);
    ui->TitleWid->setTitleBarWidget(emTitle);
    ui->TitleWid->setWidget(&title_);

    if (!SigConnect()) {
        return false;
    }

    if (ui->CtrlBarWid->Init() || ui->ShowWid->Init()) {
        return false;
    }
    return true;
}

bool KplayerMain::SigConnect() {

    return true;
}

