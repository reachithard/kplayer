//
// Created by luo on 2023/9/7.
//

#ifndef KPLAYER_KPLAYER_LOADING_H
#define KPLAYER_KPLAYER_LOADING_H

#include <QDialog>
#include <QMovie>
#include <QLabel>
#include <QDialog>
#include <QPainter>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#define USER_CANCEL -1


QT_BEGIN_NAMESPACE
namespace Ui { class KplayerLoading; }
QT_END_NAMESPACE

class KplayerLoading : public QWidget {
    Q_OBJECT
public:
    explicit KplayerLoading(QWidget *parent = nullptr);

    ~KplayerLoading() override;

    void Show();

    void setTipsText(QString strTipsText);

    void setCanCancel(bool bCanCancel);
    void moveToCenter(QWidget *pParent);

protected:
    void paintEvent(QPaintEvent *event) override;
private:
    void initUi();
    Q_SIGNALS:
    void cancelWaiting();
private slots:
    void cancelBtnClicked();
private:
    QFrame *m_pCenterFrame;
    QLabel *m_pMovieLabel;
    QMovie *m_pLoadingMovie;
    QLabel *m_pTipsLabel;
    QPushButton *m_pCancelBtn;

};


#endif //KPLAYER_KPLAYER_LOADING_H
