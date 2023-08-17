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

    ui->label->setUpdatesEnabled(false);
    setMouseTracking(true);
    setAcceptDrops(true);
    setAttribute(Qt::WA_OpaquePaintEvent);
}

KplayerShow::~KplayerShow() {
    delete ui;
}

int KplayerShow::Init() {
    int ret = wrapper_.Init();
    if (ret != KPLAYER_SUCCESS) {
        return ret;
    }

    wrapper_.SetWid((void*)ui->label->winId());
    QString url = "C:\\Users\\luo\\Documents\\Captura\\2023-08-09\\02-40-14.mp4";
    PlayOrPause(url);
    return 0;
}

int KplayerShow::PlayOrPause(const QString &url) {
    return wrapper_.VideoPlay(url);
}

void KplayerShow::resizeEvent(QResizeEvent *event) {
    AdjustSizeShow();
    QWidget::resizeEvent(event);
}

void KplayerShow::AdjustSizeShow() {
    std::lock_guard<std::mutex> lk(refresh_lock_);
    if (width_ == 0 && height_ == 0) {
        ui->label->setGeometry(0, 0, width(), height());
    } else {
        float aspect_ratio;
        int width, height, x, y;
        int scr_width = this->width();
        int scr_height = this->height();

        aspect_ratio = (float)width_ / (float)height_;

        height = scr_height;
        width = lrint(height * aspect_ratio) & ~1;
        if (width > scr_width) {
            width = scr_width;
            height = lrint(width / aspect_ratio) & ~1;
        }
        x = (scr_width - width) / 2;
        y = (scr_height - height) / 2;

        ui->label->setGeometry(x, y, width, height);
    }
}
