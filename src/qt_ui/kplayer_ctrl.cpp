//
// Created by luo on 2023/8/10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_kplayer_ctrl.h" resolved

#include "kplayer_ctrl.h"
#include "ui_kplayer_ctrl.h"


KplayerCtrl::KplayerCtrl(QWidget *parent) :
        QWidget(parent), ui(new Ui::KplayerCtrl) {
    ui->setupUi(this);
}

KplayerCtrl::~KplayerCtrl() {
    delete ui;
}

int KplayerCtrl::Init() {
    return 0;
}
