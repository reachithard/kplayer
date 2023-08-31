//
// Created by luo on 2023/8/31.
//

#ifndef KPLAYER_KPLAYER_SLIDER_H
#define KPLAYER_KPLAYER_SLIDER_H

#include <QSlider>
#include <QMouseEvent>

class KplayerSlider : public QSlider{
Q_OBJECT

public:
    KplayerSlider(QWidget* parent);
    ~KplayerSlider();
protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
signals:
    void SigValueChanged();

private:
    bool pressed;
};


#endif //KPLAYER_KPLAYER_SLIDER_H
