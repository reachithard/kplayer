//
// Created by luo on 2023/8/10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_kplayer_show.h" resolved

#include "kplayer_show.h"
#include "ui_kplayer_show.h"

#include <QKeyEvent>
#include <QMimeData>
#include <QDebug>

#include "kplayer_errors.h"

KplayerShow::KplayerShow(QWidget *parent) :
        QWidget(parent), ui(new Ui::KplayerShow) {
    ui->setupUi(this);

    setMouseTracking(true);
    setAcceptDrops(true);
    ui->label->setAttribute(Qt::WA_OpaquePaintEvent);
}

KplayerShow::~KplayerShow() {
    delete ui;
}

bool KplayerShow::Init() {
    int ret = wrapper_.Init();
    if (ret != KPLAYER_SUCCESS) {
        return false;
    }

    // 进行connect
    (void)connect(&wrapper_, &KplayerWrapper::SigVideoDimensionsChanged, this, &KplayerShow::OnVideoDimensionsChanged);
    (void)connect(&wrapper_, &KplayerWrapper::SigVideoPlaySeconds, this, &KplayerShow::SigVideoPlaySeconds);
    (void)connect(&wrapper_, &KplayerWrapper::SigVideoTotalSeconds, this, &KplayerShow::SigVideoTotalSeconds);
    (void)connect(&wrapper_, &KplayerWrapper::SigVideoStop, this, &KplayerShow::SigVideoStop);
    (void)connect(&wrapper_, &KplayerWrapper::SigVideoVolume, this, &KplayerShow::SigVideoVolume);
    (void)connect(&wrapper_, &KplayerWrapper::SigVideoPauseState, this, &KplayerShow::SigVideoPauseState);
    (void)connect(&wrapper_, &KplayerWrapper::SigVideoStopFinished, this, &KplayerShow::SigVideoStopFinished);
    (void)connect(&wrapper_, &KplayerWrapper::SigVideoPlayStart, this, &KplayerShow::SigVideoPlayStart);

    wrapper_.SetWid((void*)ui->label->winId());
    return true;
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

void KplayerShow::OnVideoDimensionsChanged(int width, int height) {
    width_ = width;
    height_ = height;
    AdjustSizeShow();
}

void KplayerShow::Close() {

}

void KplayerShow::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_F:
            emit SigShowFullScreen();
            break;
    }
}

int KplayerShow::ToggleMute() {
    return wrapper_.VideoToggleMute();
}

int KplayerShow::Seek(double incr) {
    return wrapper_.VideoSeek(incr);
}

int KplayerShow::SetSpeed(float speed) {
    return wrapper_.VideoSetSpeed(speed);
}

void KplayerShow::OnVolumeChanged(float volume) {
    wrapper_.VideoUpdateVolume(volume);
}

void KplayerShow::OnVideoJump(float pos) {
    wrapper_.VideoJump(pos);
}

void KplayerShow::dropEvent(QDropEvent *event) {
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
    {
        return;
    }

    QStringList temp;
    for(QUrl url: urls)
    {
        // 进行后缀名判断
        QString strFileName = url.toLocalFile();
        bool bSupportMovie = strFileName.endsWith(".mkv", Qt::CaseInsensitive) ||
                strFileName.endsWith(".rmvb", Qt::CaseInsensitive) ||
                strFileName.endsWith(".mp4", Qt::CaseInsensitive) ||
                strFileName.endsWith(".avi", Qt::CaseInsensitive) ||
                strFileName.endsWith(".flv", Qt::CaseInsensitive) ||
                strFileName.endsWith(".wmv", Qt::CaseInsensitive) ||
                strFileName.endsWith(".3gp", Qt::CaseInsensitive);
        if (bSupportMovie)
        {
            temp.push_back(strFileName);
        }
    }
    qDebug() << "drop event";
    emit SigAddPlayFile(temp);
}

void KplayerShow::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
    QWidget::dragEnterEvent(event);
}
