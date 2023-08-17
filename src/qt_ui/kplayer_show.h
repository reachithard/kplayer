//
// Created by luo on 2023/8/10.
//

#ifndef KPLAYER_KPLAYER_SHOW_H
#define KPLAYER_KPLAYER_SHOW_H

#include <QWidget>

#include "kplayer_wrapper.h"

#include <mutex>

QT_BEGIN_NAMESPACE
namespace Ui { class KplayerShow; }
QT_END_NAMESPACE

class KplayerShow : public QWidget {
Q_OBJECT

public:
    explicit KplayerShow(QWidget *parent = nullptr);

    ~KplayerShow() override;

    int Init();

    int PlayOrPause(const QString &url);

protected:
    void resizeEvent(QResizeEvent *event) override;
private:
    void AdjustSizeShow();

    Ui::KplayerShow *ui;
    KplayerWrapper wrapper_;
    uint32_t width_ = 0;
    uint32_t height_ = 0;
    std::mutex refresh_lock_;
};


#endif //KPLAYER_KPLAYER_SHOW_H
