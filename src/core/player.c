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
    player->startup_volume = 50;
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
    player->play_rate = 1.0f;
    player->touch = create_touch();
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

int player_seek(Player *player, double incr) {
    if (player) {
        double pos = get_master_clock(&player->is);
        if (isnan(pos))
            pos = (double)player->is.seek_pos / AV_TIME_BASE;
        pos += incr;
        if (player->is.ic->start_time != AV_NOPTS_VALUE && pos < player->is.ic->start_time / (double)AV_TIME_BASE)
            pos = player->is.ic->start_time / (double)AV_TIME_BASE;
        stream_seek(&player->is, (int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), 0);
    }
    return 0;
}

int player_jump(Player *player, float pos) {
    if (player) {
        int64_t ts = pos * player->is.ic->duration;
        if (player->is.ic->start_time != AV_NOPTS_VALUE)
            ts += player->is.ic->start_time;
        stream_seek(&player->is, ts, 0, 0);
    }
    return 0;
}

int player_set_speed(Player *player, float speed) {
    if (player) {
        player->play_rate = speed;
    }
    return 0;
}

int player_pause(Player *player) {
    if (player) {
        toggle_pause(&player->is);
    }
    return 0;
}

int player_mute(Player *player) {
    if (player) {
        toggle_mute(&player->is);
    }
    return 0;
}

int player_update_volume(Player *player, float volume) {
    if (player) {
        float tmp = volume * SDL_MIX_MAXVOLUME;
        player->is.audio_volume = tmp;
    }
    return 0;
}

int player_next_frame(Player *player) {
    return 0;
}

int player_stop(Player *player) {
    // stop和destory的区别是 stop不会全部清空
    return 0;
}

void player_destroy(Player *player) {
    destroy_touch(player->touch);
}