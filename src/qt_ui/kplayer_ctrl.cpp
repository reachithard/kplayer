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

bool KplayerCtrl::Init() {
    SigConnect();

    const static QStringList list{"0.5", "0.75", "1.0", "1.25", "1.5", "2.0"};
    ui->SpeedComb->addItems(list);
    ui->SpeedComb->setCurrentIndex(2);
    return true;
}

void KplayerCtrl::SigConnect() {
    connect(ui->PlayOrPauseBtn, &QPushButton::clicked, this, &KplayerCtrl::SigPlayOrPause);
    connect(ui->MuteBtn, &QPushButton::clicked, this, &KplayerCtrl::SigMute);
    connect(ui->BackwardBtn, &QPushButton::clicked, this, &KplayerCtrl::SigBackward);
    connect(ui->ForwardBtn, &QPushButton::clicked, this, &KplayerCtrl::SigForward);
    connect(ui->PlaylistCtrlBtn, &QPushButton::clicked, this, &KplayerCtrl::SigShowOrHidePlaylist);
    connect(ui->SpeedComb, SIGNAL(currentIndexChanged(int)),this,SLOT(OnSpeedChanged(int)));
}

void KplayerCtrl::OnSpeedChanged(int idx) {
    QString current = ui->SpeedComb->itemText(idx);
    bool ok = false;
    float speed = current.toFloat(&ok);
    if (ok) {
        emit SigSpeedChanged(speed);
    }
}
