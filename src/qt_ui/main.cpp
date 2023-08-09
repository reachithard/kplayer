#include "kplayer_main.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KplayerMain w;
    w.show();
    return a.exec();
}
