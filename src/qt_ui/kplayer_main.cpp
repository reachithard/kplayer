#include "kplayer_main.h"
#include "./ui_kplayer_main.h"

#include <QWindow>
#include <QScreen>
#include <QRect>
#include <QVariant>
#include <QDialog>
#include <QDebug>
#include <unistd.h>

#include "kplayer_loading.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "player.h"

#ifdef __cplusplus
}
#endif

KplayerMain::KplayerMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::KplayerMain)
    , playList_(this)
    , title_(this)
{
    ui->setupUi(this);

    // todo style

    setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowSystemMenuHint*/ | Qt::WindowMinimizeButtonHint);
    setMouseTracking(true);
}

KplayerMain::~KplayerMain()
{
    delete ui;
}

bool KplayerMain::Init() {
    QDialog *dockDialog = new QDialog(this); // <-----added a parent widget for the dock
    ui->PlaylistWid->setTitleBarWidget(dockDialog);
    ui->PlaylistWid->setWidget(&playList_);

    QWidget *emTitle = new QWidget(this);
    ui->TitleWid->setTitleBarWidget(emTitle);
    ui->TitleWid->setWidget(&title_);

    if (!SigConnect()) {
        return false;
    }

    if (!ui->CtrlBarWid->Init() || !ui->ShowWid->Init() || !title_.Init() || !playList_.Init()) {
        return false;
    }

    ctrlAnimHide_ = new QPropertyAnimation(ui->CtrlBarWid, "geometry");
    ctrlAnimShow_ = new QPropertyAnimation(ui->CtrlBarWid, "geometry");
    return true;
}

bool KplayerMain::SigConnect() {
    (void)connect(&title_, &KplayerTitle::SigMin, this, &KplayerMain::OnMin);
    (void)connect(&title_, &KplayerTitle::SigMaxOrNormal, this, &KplayerMain::OnMaxOrNormal);
    (void)connect(&title_, &KplayerTitle::SigFullScreen, this, &KplayerMain::OnFullScreen);
    (void)connect(&title_, &KplayerTitle::SigClose, this, &KplayerMain::OnClose, Qt::ConnectionType::UniqueConnection);

    (void)connect(ui->ShowWid, &KplayerShow::SigShowFullScreen, this, &KplayerMain::OnFullScreen);
    (void)connect(ui->ShowWid, &KplayerShow::SigVideoPlaySeconds, ui->CtrlBarWid, &KplayerCtrl::OnVideoPlaySeconds);
    (void)connect(ui->ShowWid, &KplayerShow::SigVideoTotalSeconds, ui->CtrlBarWid, &KplayerCtrl::OnVideoTotalSeconds);
//    (void)connect(ui->ShowWid, &KplayerShow::SigVideoStop, ui->CtrlBarWid, &KplayerCtrl::OnVideoStop);
    (void)connect(ui->ShowWid, &KplayerShow::SigVideoVolume, ui->CtrlBarWid, &KplayerCtrl::OnVideoVolume);
//    (void)connect(ui->ShowWid, &KplayerShow::SigVideoPauseState, ui->CtrlBarWid, &KplayerCtrl::OnVideoPauseState);
//    (void)connect(ui->ShowWid, &KplayerShow::SigVideoStopFinished, ui->CtrlBarWid, &KplayerCtrl::OnVideoStopFinished);
//    (void)connect(ui->ShowWid, &KplayerShow::SigVideoPlayStart, ui->CtrlBarWid, &KplayerCtrl::OnVideoPlayStart);
    (void)connect(ui->ShowWid, &KplayerShow::SigAddPlayFile, this, &KplayerMain::OnAddPlayFile);

    (void)connect(ui->CtrlBarWid, &KplayerCtrl::SigPlayOrPause, this, &KplayerMain::OnPlayOrPause);
    (void)connect(ui->CtrlBarWid, &KplayerCtrl::SigMute, this, &KplayerMain::OnMute);
    (void)connect(ui->CtrlBarWid, &KplayerCtrl::SigForward, this, &KplayerMain::OnForward);
    (void)connect(ui->CtrlBarWid, &KplayerCtrl::SigBackward, this, &KplayerMain::OnBackward);
    (void)connect(ui->CtrlBarWid, &KplayerCtrl::SigSpeedChanged, this, &KplayerMain::OnSpeedChanged);
    (void)connect(ui->CtrlBarWid, &KplayerCtrl::SigShowOrHidePlaylist, this, &KplayerMain::OnShowOrHidePlaylist);
    (void)connect(ui->CtrlBarWid, &KplayerCtrl::SigVolumeChanged, ui->ShowWid, &KplayerShow::OnVolumeChanged);
    (void)connect(ui->CtrlBarWid, &KplayerCtrl::SigPlayerChanged, ui->ShowWid, &KplayerShow::OnVideoJump);

    (void)connect(&playList_, &KplayerPlayList::SigPlayVideo, this, &KplayerMain::OnPlayVideo);
    (void)connect(&playList_, &KplayerPlayList::SigASR, this, &KplayerMain::OnASR);
    return true;
}

void KplayerMain::OnMin() {
    showMinimized();
}

void KplayerMain::OnMaxOrNormal() {
    if (isMaximized()) {
        showNormal();
    } else {
        showMaximized();
    }
}

void KplayerMain::OnFullScreen() {
    if (!ui->ShowWid->isFullScreen()) { // 如果不是全屏 那么就去全屏
        ui->ShowWid->setWindowFlags(Qt::Window);
        QScreen *pStCurScreen = screen();
        ui->ShowWid->windowHandle()->setScreen(pStCurScreen);
        ui->ShowWid->showFullScreen();

        QRect stScreenRect = pStCurScreen->geometry();
        int nCtrlBarHeight = ui->CtrlBarWid->height();
        int nX = ui->ShowWid->x();
        ctrlShow_ = QRect(nX, stScreenRect.height() - nCtrlBarHeight, stScreenRect.width(), nCtrlBarHeight);
        ctrlHidden_ = QRect(nX, stScreenRect.height(), stScreenRect.width(), nCtrlBarHeight);

        ctrlAnimShow_->setStartValue(ctrlHidden_);
        ctrlAnimShow_->setEndValue(ctrlShow_);
        ctrlAnimShow_->setDuration(1000);

        ctrlAnimHide_->setStartValue(ctrlShow_);
        ctrlAnimHide_->setEndValue(ctrlHidden_);
        ctrlAnimHide_->setDuration(1000);

        ui->CtrlBarWid->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
        ui->CtrlBarWid->windowHandle()->setScreen(pStCurScreen);
        ui->CtrlBarWid->raise();
        ui->CtrlBarWid->setWindowOpacity(0.5);
        ui->CtrlBarWid->showNormal();
        ui->CtrlBarWid->windowHandle()->setScreen(pStCurScreen);

        ctrlAnimShow_->start();
        ui->ShowWid->setFocus();
    } else {
        ctrlAnimShow_->stop();
        ctrlAnimHide_->stop();
        ui->CtrlBarWid->setWindowOpacity(1);
        ui->CtrlBarWid->setWindowFlags(Qt::SubWindow);

        ui->ShowWid->setWindowFlags(Qt::SubWindow);

        ui->CtrlBarWid->showNormal();
        ui->ShowWid->showNormal();
        ui->ShowWid->setFocus();
    }
}

void KplayerMain::OnClose() {
    ui->ShowWid->Close();
    close();
}

void KplayerMain::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_F:
            OnFullScreen();
            break;
    }
}

void KplayerMain::OnShowOrHidePlaylist() {
    qDebug() << "OnShowOrHidePlaylist";
    if (ui->PlaylistWid->isHidden()) {
        ui->PlaylistWid->show();
    } else {
        ui->PlaylistWid->hide();
    }
}

void KplayerMain::OnPlayOrPause() {
    QString url = playList_.GetCurrentItem();
    ui->ShowWid->PlayOrPause(url);
}

void KplayerMain::OnMute() {
    ui->ShowWid->ToggleMute();
}

void KplayerMain::OnForward() {
    ui->ShowWid->Seek(3);
}

void KplayerMain::OnBackward() {
    ui->ShowWid->Seek(-3);
}

void KplayerMain::OnSpeedChanged(float speed) {
    ui->ShowWid->SetSpeed(speed);
}

void KplayerMain::OnPlayVideo(const QString &url) {
    if (url.isEmpty()) {
        return;
    }
    ui->ShowWid->PlayOrPause(url);
}

void KplayerMain::OnAddPlayFile(const QStringList &urls) {
    if (urls.empty()) {
        return;
    }

    playList_.AddFiles(urls);
    ui->ShowWid->PlayOrPause(urls.front());
}

void KplayerMain::OnASR(const QString &url) {
    KplayerLoading loading;
    loading.show();

    while (true) {
        qDebug() << "on asr";
        sleep(100);
    }
}

