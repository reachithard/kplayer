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
    , playList(this)
    , title(this)
{
    ui->setupUi(this);

    TestPlay();
}

KplayerMain::~KplayerMain()
{
    delete ui;
}

void KplayerMain::TestPlay() {
//    int ret = player_global_init();
//    if (ret) {
//        return;
//    }
//
//    Player *player = player_create();
//    if (!player) {
//        return;
//    }
//
//    player->input_filename = "C:\\Users\\luo\\Documents\\Captura\\2023-08-09\\02-40-14.mp4";
//    ret = player_play(player);
//    if (ret) {
//        return;
//    }
}

bool KplayerMain::Init() {
    QWidget *em = new QWidget(this);
    ui->PlaylistWid->setTitleBarWidget(em);
    ui->PlaylistWid->setWidget(&playList);
    //ui->PlaylistWid->setFixedWidth(100);

    QWidget *emTitle = new QWidget(this);
    ui->TitleWid->setTitleBarWidget(emTitle);
    ui->TitleWid->setWidget(&title);

    if (ui->CtrlBarWid->Init() || ui->ShowWid->Init()) {
        return false;
    }
    return true;
}

