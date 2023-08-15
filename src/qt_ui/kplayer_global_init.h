//
// Created by luo on 2023/8/14.
//

#ifndef KPLAYER_KPLAYER_GLOBAL_INIT_H
#define KPLAYER_KPLAYER_GLOBAL_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "kplayer_errors.h"

#ifdef __cplusplus
}
#endif

class KplayerGlobalInit {
public:
    KplayerGlobalInit();

    ~KplayerGlobalInit();

    int Init();
};


#endif //KPLAYER_KPLAYER_GLOBAL_INIT_H
