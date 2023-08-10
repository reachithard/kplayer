//
// Created by luo on 2023/8/10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_kplayer_title.h" resolved

#include "kplayer_title.h"
#include "ui_kplayer_title.h"


KplayerTitle::KplayerTitle(QWidget *parent) :
        QWidget(parent), ui(new Ui::KplayerTitle) {
    ui->setupUi(this);
}

KplayerTitle::~KplayerTitle() {
    delete ui;
}
