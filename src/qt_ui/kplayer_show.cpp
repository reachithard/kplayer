//
// Created by luo on 2023/8/10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_kplayer_show.h" resolved

#include "kplayer_show.h"
#include "ui_kplayer_show.h"


KplayerShow::KplayerShow(QWidget *parent) :
        QWidget(parent), ui(new Ui::KplayerShow) {
    ui->setupUi(this);
}

KplayerShow::~KplayerShow() {
    delete ui;
}
