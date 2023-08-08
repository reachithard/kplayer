//
// Created by luo on 2023/8/8.
//

#ifdef __cplusplus
extern "C" {
#endif

#include "libavutil/avutil.h"

#ifdef __cplusplus
}
#endif

#include <iostream>
#include <windows.h>

int main(int argc, char **argv) {
#if defined(_WIN32)
    /* Calling SetDllDirectory with the empty string (but not NULL) removes the
     * current working directory from the DLL search path as a security pre-caution. */
    SetDllDirectory("C:\\workspace\\kplayer\\3rd\\windows\\bin");
#endif

    return 0;
}