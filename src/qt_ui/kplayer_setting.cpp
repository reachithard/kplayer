//
// Created by luo on 2023/8/10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_kplayer_setting.h" resolved

#include "kplayer_setting.h"
#include "ui_kplayer_setting.h"


KplayerSetting::KplayerSetting(QWidget *parent) :
        QWidget(parent), ui(new Ui::KplayerSetting) {
    ui->setupUi(this);
}

KplayerSetting::~KplayerSetting() {
    delete ui;
}
