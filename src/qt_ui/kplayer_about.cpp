//
// Created by luo on 2023/8/10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_kplayer_about.h" resolved

#include "kplayer_about.h"
#include "ui_kplayer_about.h"


KplayerAbout::KplayerAbout(QWidget *parent) :
        QWidget(parent), ui(new Ui::KplayerAbout) {
    ui->setupUi(this);
}

KplayerAbout::~KplayerAbout() {
    delete ui;
}
