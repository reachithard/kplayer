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
    if (pressed) {
        int pos = value();
        emit SigSliderValueChanged(pos);
        pressed = false;
    }

    QSlider::mouseReleaseEvent(ev);
}

void KplayerSlider::mouseMoveEvent(QMouseEvent *ev) {
    pressed = true;
    QSlider::mouseMoveEvent(ev);
}

void KplayerSlider::SetValue(int value) {
    if (!pressed) {
        std::lock_guard<std::mutex> tm(lk);
        setValue(value);
    }
}
