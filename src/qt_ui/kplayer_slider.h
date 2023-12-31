//
// Created by luo on 2023/8/31.
//

#ifndef KPLAYER_KPLAYER_SLIDER_H
#define KPLAYER_KPLAYER_SLIDER_H

#include <QSlider>
#include <QMouseEvent>
#include <mutex>

class KplayerSlider : public QSlider{
Q_OBJECT
public:
    KplayerSlider(QWidget* parent);
    ~KplayerSlider();

    void SetValue(int value);
protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
signals:
    void SigSliderValueChanged(int value);//自定义的鼠标单击信号，用于捕获并处理
private:
    bool pressed;
    std::mutex lk;
};


#endif //KPLAYER_KPLAYER_SLIDER_H
