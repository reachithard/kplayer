#ifndef KPLAYERMAIN_H
#define KPLAYERMAIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class KplayerMain; }
QT_END_NAMESPACE

class KplayerMain : public QMainWindow
{
    Q_OBJECT

public:
    KplayerMain(QWidget *parent = nullptr);
    ~KplayerMain();

protected:
    void TestPlay();

private:
    Ui::KplayerMain *ui;
};
#endif // KPLAYERMAIN_H
