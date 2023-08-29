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

bool KplayerTitle::Init() {
    setAttribute(Qt::WA_TranslucentBackground);
    (void)connect(ui->MinBtn, &QPushButton::clicked, this, &KplayerTitle::SigMin);
    (void)connect(ui->MaxBtn, &QPushButton::clicked, this, &KplayerTitle::SigMaxOrNormal);
    (void)connect(ui->FullScreenBtn, &QPushButton::clicked, this, &KplayerTitle::SigFullScreen);
    (void)connect(ui->CloseBtn, &QPushButton::clicked, this, &KplayerTitle::SigClose);
    return true;
}
