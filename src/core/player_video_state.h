#ifndef _PLAYER_VIDEO_STATE_H_
#define _PLAYER_VIDEO_STATE_H_

#include "player_pod.h"

void video_image_display(Player *player, VideoState *is);

static void video_audio_display(Player *player, VideoState *s);

void stream_component_close(Player *player, VideoState *is, int stream_index);

void stream_close(Player* player, VideoState *is);

void do_exit(VideoState *is);

int video_open(Player *player, VideoState *is);

void video_display(Player *player, VideoState *is);

int get_master_sync_type(VideoState *is);

double get_master_clock(VideoState *is);

void check_external_clock_speed(VideoState *is);

void stream_seek(VideoState *is, int64_t pos, int64_t rel, int by_bytes);

void stream_toggle_pause(VideoState *is);

void toggle_pause(VideoState *is);

void toggle_mute(VideoState *is);

void update_volume(VideoState *is, int sign, double step);

void step_to_next_frame(VideoState *is);

double compute_target_delay(double delay, VideoState *is);

double vp_duration(VideoState *is, Frame *vp, Frame *nextvp);

void update_video_pts(VideoState *is, double pts, int64_t pos, int serial);

int queue_picture(VideoState *is, AVFrame *src_frame, double pts, double duration, int64_t pos, int serial);

int get_video_frame(Player* player, VideoState *is, AVFrame *frame);

void update_sample_display(VideoState *is, short *samples, int samples_size);

int synchronize_audio(VideoState *is, int nb_samples);

int audio_decode_frame(VideoState *is);

int stream_component_open(Player *player, VideoState *is, int stream_index);

void stream_cycle_channel(Player *player, VideoState *is, int codec_type);

void toggle_full_screen(Player *player, VideoState *is);

void toggle_audio_display(VideoState *is);

void seek_chapter(VideoState *is, int incr);

#endif // _PLAYER_VIDEO_STATE_H_