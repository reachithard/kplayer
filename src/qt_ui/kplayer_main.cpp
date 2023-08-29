#include "kplayer_main.h"
#include "./ui_kplayer_main.h"

#include <QWindow>
#include <QScreen>
#include <QRect>
#include <QVariant>

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
    QWidget *em = new QWidget(this);
    ui->PlaylistWid->setTitleBarWidget(em);
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

