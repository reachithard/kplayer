//
// Created by luo on 2023/8/10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_kplayer_ctrl.h" resolved

#include "kplayer_ctrl.h"
#include "ui_kplayer_ctrl.h"
#include "kplayer_slider.h"

#include <QSlider>

KplayerCtrl::KplayerCtrl(QWidget *parent) :
        QWidget(parent), ui(new Ui::KplayerCtrl) {
    ui->setupUi(this);

    ui->PlaySlider->setMinimum(0);
    ui->PlaySlider->setMaximum(100);
    ui->PlaySlider->setSingleStep(1);

    ui->VolumeSlider->setMinimum(0);
    ui->VolumeSlider->setMaximum(100);
    ui->VolumeSlider->setSingleStep(1);
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
    connect(ui->PlaySlider, &KplayerSlider::SigSliderValueChanged, this, &KplayerCtrl::OnPlayerChanged);
    connect(ui->VolumeSlider, &KplayerSlider::SigSliderValueChanged, this, &KplayerCtrl::OnVolumeChanged);
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

void KplayerCtrl::OnVideoPlaySeconds(int sec) {
    int thh, tmm, tss;
    thh = sec / 3600;
    tmm = (sec % 3600) / 60;
    tss = (sec % 60);
    QTime time(thh, tmm, tss);

    ui->VideoPlayTimeTimeEdit->setTime(time);
    ui->PlaySlider->SetValue(((float)sec / (float)totalTime_) * 100);
}

void KplayerCtrl::OnVideoTotalSeconds(int sec) {
    totalTime_ = sec;
    int thh, tmm, tss;
    thh = sec / 3600;
    tmm = (sec % 3600) / 60;
    tss = (sec % 60);
    QTime time(thh, tmm, tss);

    ui->VideoTotalTimeTimeEdit->setTime(time);
}

void KplayerCtrl::OnVideoVolume(double volume) {
    ui->VolumeSlider->SetValue(volume * 100);
}

void KplayerCtrl::OnPlayerChanged(int value) {
    emit SigPlayerChanged(value * 0.01);
}

void KplayerCtrl::OnVolumeChanged(int value) {
    emit SigVolumeChanged(value * 0.01);
}
