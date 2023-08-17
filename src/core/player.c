#include "player.h"

#include "player_common.h"
#include "player_video_state.h"

int player_global_init() {
    av_log_set_flags(AV_LOG_SKIP_REPEATED);

    avdevice_register_all();
    avformat_network_init();

    int flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
    if (SDL_Init (flags)) {
        av_log(NULL, AV_LOG_FATAL, "Could not initialize SDL - %s\n", SDL_GetError());
        av_log(NULL, AV_LOG_FATAL, "(Did you set the DISPLAY variable?)\n");
        return -1;
    }

    SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

    return 0;
}

Player *player_create() {
    Player *player = av_mallocz(sizeof(Player));
    if (!player) {
        return player;
    }

    /* options specified by the user */
    player->default_width  = 640;
    player->default_height = 480;
    player->screen_width  = 0;
    player->screen_height = 0;
    player->screen_left = SDL_WINDOWPOS_CENTERED;
    player->screen_top = SDL_WINDOWPOS_CENTERED;
    // const char* wanted_stream_spec[AVMEDIA_TYPE_NB] = {0};
    player->seek_by_bytes = -1;
    player->seek_interval = 10;
    player->startup_volume = 100;
    player->show_status = -1;
    player->av_sync_type = AV_SYNC_AUDIO_MASTER;
    player->start_time = AV_NOPTS_VALUE;
    player->duration = AV_NOPTS_VALUE;
    player->fast = 0;
    player->genpts = 0;
    player->lowres = 0;
    player->decoder_reorder_pts = -1;
    player->loop = 1;
    player->framedrop = -1;
    player->infinite_buffer = -1;
    player->show_mode = SHOW_MODE_NONE;
    player->rdftspeed = 0.02;
    player->cursor_hidden = 0;
#if CONFIG_AVFILTER
    player->vfilters_list = NULL;
    player->nb_vfilters = 0;
    player->afilters = NULL;
#endif
    player->autorotate = 1;
    player->find_stream_info = 1;
    player->filter_nbthreads = 0;

    // SDL_RendererInfo renderer_info = {0};
    player->refresh_loop = 1;
    player->refresh_thread = 1;
    player->w_id = NULL;
    return player;
}

int player_play(Player *player) {
    if (!player->input_filename) {
        return -1;
    }

    if (player->callbacks.video_play_start) {
        player->callbacks.video_play_start(player->userdata, player->input_filename);
    }

    VideoState *is = stream_open(player, player->input_filename, player->file_iformat);
    if (!is) {
        av_log(NULL, AV_LOG_FATAL, "Failed to initialize VideoState!\n");
        return -1;
    }

    if (player->refresh_thread) {
        is->refresh_tid = SDL_CreateThread(refresh_thread, "refresh_thread", is);
        if (!is->refresh_tid) {
            av_log(NULL, AV_LOG_FATAL, "Failed to initialize refresh thread!\n");
            return -1;
        }
    } else {
        refresh_thread(is);
    }

    return 0;
}

int player_pause(Player *player) {

}

int player_mute(Player *player) {

}

int player_update_volume(Player *player, int incr) {

}

int player_next_frame(Player *player) {

}

int player_stop(Player *player) {
    // stop和destory的区别是 stop不会全部清空
}

void player_destroy(Player *player) {

}