#include "kplayer_main.h"

#include <QApplication>

#include "kplayer_global_init.h"
#include "kplayer_loading.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    KplayerGlobalInit init{};
    if (init.Init() != KPLAYER_SUCCESS) {
        return KPLAYER_GLOBAL_INIT_ERROR;
    }

    KplayerMain w;
    if (!w.Init()) {
        return -1;
    }
    w.show();
    return a.exec();
}
