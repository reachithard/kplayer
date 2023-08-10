//
// Created by luo on 2023/8/10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_kplayer_play_list.h" resolved

#include "kplayer_play_list.h"
#include "ui_kplayer_play_list.h"


KplayerPlayList::KplayerPlayList(QWidget *parent) :
        QWidget(parent), ui(new Ui::KplayerPlayList) {
    ui->setupUi(this);
}

KplayerPlayList::~KplayerPlayList() {
    delete ui;
}
