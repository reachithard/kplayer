//
// Created by luo on 2023/8/31.
//

#include "kplayer_slider.h"

KplayerSlider::KplayerSlider(QWidget *parent) : QSlider(parent) {
    pressed = false;
}

KplayerSlider::~KplayerSlider() {

}

void KplayerSlider::mousePressEvent(QMouseEvent *ev) {
    pressed = true;
    QSlider::mousePressEvent(ev);
}

void KplayerSlider::mouseReleaseEvent(QMouseEvent *ev) {
    pressed = false;
    int pos = value();
    emit SigSliderValueChanged(pos);
    QSlider::mouseReleaseEvent(ev);
}

void KplayerSlider::mouseMoveEvent(QMouseEvent *ev) {
    QSlider::mouseMoveEvent(ev);
}
