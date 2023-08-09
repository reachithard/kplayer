#ifndef _PLAYER_VIDEO_STATE_H_
#define _PLAYER_VIDEO_STATE_H_

#include "player_pod.h"

void video_image_display(VideoState *is);

int compute_mod(int a, int b);

void video_audio_display(VideoState *s);

void stream_component_close(VideoState *is, int stream_index);

void stream_close(VideoState *is);

void do_exit(VideoState *is);

void sigterm_handler(int sig);

void set_default_window_size(VideoState *is, int width, int height, AVRational sar);

int video_open(VideoState *is);

void video_display(VideoState *is);

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

void video_refresh(void *opaque, double *remaining_time);

int queue_picture(VideoState *is, AVFrame *src_frame, double pts, double duration, int64_t pos, int serial);

int get_video_frame(VideoState *is, AVFrame *frame);

int audio_thread(void *arg);

int video_thread(void *arg);

int subtitle_thread(void *arg);

void update_sample_display(VideoState *is, short *samples, int samples_size);

int synchronize_audio(VideoState *is, int nb_samples);

int audio_decode_frame(VideoState *is);

void sdl_audio_callback(void *opaque, Uint8 *stream, int len);

int audio_open(void *opaque, AVChannelLayout *wanted_channel_layout, int wanted_sample_rate, struct AudioParams *audio_hw_params);

int stream_component_open(VideoState *is, int stream_index);

int decode_interrupt_cb(void *ctx);

int stream_has_enough_packets(AVStream *st, int stream_id, PacketQueue *queue);

int is_realtime(AVFormatContext *s);

int read_thread(void *arg);

int refresh_thread(void *arg);

VideoState *stream_open(Player *player, const char *filename,
                        const AVInputFormat *iformat);

void stream_cycle_channel(VideoState *is, int codec_type);

void toggle_full_screen(VideoState *is);

void toggle_audio_display(VideoState *is);

void refresh_loop_wait_event(VideoState *is, SDL_Event *event);

void seek_chapter(VideoState *is, int incr);

AVDictionary *filter_codec_opts(AVDictionary *opts, enum AVCodecID codec_id,
                                AVFormatContext *s, AVStream *st, const AVCodec *codec);

AVDictionary **setup_find_stream_info_opts(AVFormatContext *s,
                                           AVDictionary *codec_opts);

int check_stream_specifier(AVFormatContext *s, AVStream *st, const char *spec);

#endif // _PLAYER_VIDEO_STATE_H_