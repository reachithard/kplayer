//
// Created by luo on 2023/8/10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_kplayer_show.h" resolved

#include "kplayer_show.h"
#include "ui_kplayer_show.h"

#include "kplayer_errors.h"

KplayerShow::KplayerShow(QWidget *parent) :
        QWidget(parent), ui(new Ui::KplayerShow) {
    ui->setupUi(this);
}

KplayerShow::~KplayerShow() {
    delete ui;
}

int KplayerShow::Init() {
    int ret = wrapper.Init();
    if (ret != KPLAYER_SUCCESS) {
        return ret;
    }

    wrapper.SetWid((void*)ui->label->winId());
    QString url = "C:\\Users\\luo\\Documents\\Captura\\2023-08-09\\02-40-14.mp4";
    PlayOrPause(url);
    return 0;
}

int KplayerShow::PlayOrPause(const QString &url) {
    return wrapper.VideoPlay(url);
}
