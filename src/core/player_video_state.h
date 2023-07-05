#ifndef _PLAYER_VIDEO_STATE_H_
#define _PLAYER_VIDEO_STATE_H_

#include "player_pod.h"

int get_master_sync_type(VideoState *is);
void check_external_clock_speed(VideoState *is);
double compute_target_delay(double delay, VideoState *is);
double vp_duration(VideoState *is, Frame *vp, Frame *nextvp);
void update_video_pts(VideoState *is, double pts, int64_t pos, int serial);

void stream_toggle_pause(VideoState *is);
#endif // _PLAYER_VIDEO_STATE_H_