//
// Created by luo on 2023/8/31.
//

#include "kplayer_slider.h"

KplayerSlider::KplayerSlider(QWidget *parent) : QSlider(parent) {

}

KplayerSlider::~KplayerSlider() {

}

void KplayerSlider::mousePressEvent(QMouseEvent *ev) {
    pressed = true;
    QSlider::mousePressEvent(ev);
}

void KplayerSlider::mouseReleaseEvent(QMouseEvent *ev) {
    pressed = false;
    QSlider::mouseReleaseEvent(ev);
}

void KplayerSlider::mouseMoveEvent(QMouseEvent *ev) {
    QSlider::mouseMoveEvent(ev);
}
