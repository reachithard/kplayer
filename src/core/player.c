#include "player.h"

inline static void ResetPlayerInternal(Player *player) {

}

inline static void init_dynload(const char *dir)
{
#if _WIN32
    /* Calling SetDllDirectory with the empty string (but not NULL) removes the
     * current working directory from the DLL search path as a security pre-caution. */
    SetDllDirectory(dir);
#endif
}

int32_t PlayerGlobalInit(const KPlayerGlobal *config) {
    init_dynload(config->dynamic_dir);
    av_log_set_flags(config->log_level);

    avcodec_register_all();
    /* register all codecs, demux and protocols */
#if CONFIG_AVDEVICE
    avdevice_register_all();
#endif
#if CONFIG_AVFILTER
    avfilter_register_all();
#endif

    avformat_network_init();

    int flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
    if (SDL_Init (flags)) {
        av_log(NULL, AV_LOG_FATAL, "Could not initialize SDL - %s\n", SDL_GetError());
        av_log(NULL, AV_LOG_FATAL, "(Did you set the DISPLAY variable?)\n");
        return PLAYER_GLOBAL_INIT_ERROR;
    }

    // 自定义初始化 初始化标志 使用原子变量
}

void PlayerGlobalUnInit() {
    avformat_network_deinit();
}

Player *CreatePlayer() {
    Player* player = (Player*) av_mallocz(sizeof(Player));
    if (!player) {
        return NULL;
    }
}

void DestroyPlayer(Player *player) {

}

void ResetPlayer(Player *player) {

}

int PlayerPlay(Player *player, const char *fileName) {

}