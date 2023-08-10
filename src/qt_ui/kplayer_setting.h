//
// Created by luo on 2023/8/10.
//

#ifndef KPLAYER_KPLAYER_SETTING_H
#define KPLAYER_KPLAYER_SETTING_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class KplayerSetting; }
QT_END_NAMESPACE

class KplayerSetting : public QWidget {
Q_OBJECT

public:
    explicit KplayerSetting(QWidget *parent = nullptr);

    ~KplayerSetting() override;

private:
    Ui::KplayerSetting *ui;
};


#endif //KPLAYER_KPLAYER_SETTING_H
