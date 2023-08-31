//
// Created by luo on 2023/8/10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_kplayer_play_list.h" resolved

#include <QFileInfo>
#include "kplayer_play_list.h"
#include "ui_kplayer_play_list.h"


KplayerPlayList::KplayerPlayList(QWidget *parent) :
        QWidget(parent), ui(new Ui::KplayerPlayList) {
    ui->setupUi(this);
}

KplayerPlayList::~KplayerPlayList() {
    delete ui;
}

bool KplayerPlayList::Init() {
    if (!ui->List->Init()) {
        return false;
    }

    connect(ui->List, &KplayerMediaList::SigAddFile, this, &KplayerPlayList::OnAddFile);
    connect(ui->List, &KplayerMediaList::itemDoubleClicked, this, &KplayerPlayList::OnItemDoubleClicked);
    return true;
}

void KplayerPlayList::OnAddFile(QString strFileName) {
    QFileInfo fileInfo(strFileName);
    QList<QListWidgetItem *> listItem = ui->List->findItems(fileInfo.fileName(), Qt::MatchExactly);
    QListWidgetItem *pItem = nullptr;
    if (listItem.isEmpty())
    {
        pItem = new QListWidgetItem(ui->List);
        pItem->setData(Qt::UserRole, QVariant(fileInfo.filePath()));  // 用户数据
        pItem->setText(fileInfo.fileName());  // 显示文本
        pItem->setToolTip(fileInfo.filePath());
        ui->List->addItem(pItem);
    }
}

void KplayerPlayList::OnItemDoubleClicked(QListWidgetItem *item) {
    // 进行播放
    emit SigPlayVideo(item->data(Qt::UserRole).toString());
    int idx = ui->List->row(item);
    ui->List->setCurrentRow(idx);
}

bool KplayerPlayList::IsEmpty() const {
    return ui->List->count() == 0;
}

QString KplayerPlayList::GetCurrentItem() {
    if (IsEmpty()) {
        return QString("");
    }

    return ui->List->currentItem()->data(Qt::UserRole).toString();
}
