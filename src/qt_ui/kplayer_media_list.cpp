//
// Created by luo on 2023/8/31.
//

#include "kplayer_media_list.h"

#include <QContextMenuEvent>
#include <QFileDialog>

KplayerMediaList::KplayerMediaList(QWidget *parent) : QListWidget(parent), menu_(this), actAdd_(this), actRemove_(this),
                                                      actClear_(this) {

}

KplayerMediaList::~KplayerMediaList() {

}

bool KplayerMediaList::Init() {
    actAdd_.setText("add");
    menu_.addAction(&actAdd_);
    actRemove_.setText("remove");
    menu_.addAction(&actRemove_);
    actClear_.setText("clear");
    menu_.addAction(&actClear_);


    connect(&actAdd_, &QAction::triggered, this, &KplayerMediaList::AddFile);
    connect(&actRemove_, &QAction::triggered, this, &KplayerMediaList::RemoveFile);
    connect(&actClear_, &QAction::triggered, this, &QListWidget::clear);

    clear();
    return true;
}

void KplayerMediaList::contextMenuEvent(QContextMenuEvent *event) {
    menu_.exec(event->globalPos());
}

void KplayerMediaList::AddFile() {
    QStringList listFileName = QFileDialog::getOpenFileNames(this, "打开文件", QDir::homePath(),
                                                             "视频文件(*.mkv *.rmvb *.mp4 *.avi *.flv *.wmv *.3gp)");

    for (QString strFileName : listFileName)
    {
        emit SigAddFile(strFileName);
    }
}

void KplayerMediaList::RemoveFile() {
    takeItem(currentRow());
}
