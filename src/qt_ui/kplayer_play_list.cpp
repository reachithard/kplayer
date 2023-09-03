//
// Created by luo on 2023/8/10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_kplayer_play_list.h" resolved

#include <QFileInfo>

#include <QDragEnterEvent>
#include <QMimeData>

#include "kplayer_play_list.h"
#include "ui_kplayer_play_list.h"


KplayerPlayList::KplayerPlayList(QWidget *parent) :
        QWidget(parent), ui(new Ui::KplayerPlayList) {
    ui->setupUi(this);
    setAcceptDrops(true);
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

void KplayerPlayList::AddFiles(const QStringList &urls) {
    if (urls.isEmpty()) {
        return;
    }

    for (auto &url : urls) {
        QFileInfo fileInfo(url);
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
}

void KplayerPlayList::dropEvent(QDropEvent *event) {
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
    {
        return;
    }

    QStringList temp;
    for(QUrl url: urls)
    {
        // 进行后缀名判断
        QString strFileName = url.toLocalFile();
        bool bSupportMovie = strFileName.endsWith(".mkv", Qt::CaseInsensitive) ||
                             strFileName.endsWith(".rmvb", Qt::CaseInsensitive) ||
                             strFileName.endsWith(".mp4", Qt::CaseInsensitive) ||
                             strFileName.endsWith(".avi", Qt::CaseInsensitive) ||
                             strFileName.endsWith(".flv", Qt::CaseInsensitive) ||
                             strFileName.endsWith(".wmv", Qt::CaseInsensitive) ||
                             strFileName.endsWith(".3gp", Qt::CaseInsensitive) ||
                            strFileName.endsWith(".wav", Qt::CaseInsensitive);
        if (bSupportMovie)
        {
            temp.push_back(strFileName);
        }
    }

    AddFiles(temp);
    QWidget::dropEvent(event);
}

void KplayerPlayList::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
    QWidget::dragEnterEvent(event);
}
