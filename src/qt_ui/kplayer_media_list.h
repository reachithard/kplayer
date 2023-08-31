//
// Created by luo on 2023/8/31.
//

#ifndef KPLAYER_KPLAYER_MEDIA_LIST_H
#define KPLAYER_KPLAYER_MEDIA_LIST_H

#include <QListWidget>
#include <QMenu>
#include <QAction>

class KplayerMediaList : public QListWidget
{
    Q_OBJECT
public:
    KplayerMediaList(QWidget *parent = 0);
    ~KplayerMediaList();
    bool Init();
protected:
    void contextMenuEvent(QContextMenuEvent* event);
private:
    void AddFile();
    void RemoveFile();
signals:
    void SigAddFile(QString strFileName);

private:
    QMenu menu_;

    QAction actAdd_;     //添加文件
    QAction actRemove_;  //移除文件
    QAction actClear_;//清空列表

};


#endif //KPLAYER_KPLAYER_MEDIA_LIST_H
