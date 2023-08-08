#ifndef _PLAYER_FILTER_H_
#define _PLAYER_FILTER_H_

#if CONFIG_AVFILTER
int opt_add_vfilter(void *optctx, const char *opt, const char *arg);
#endif

#if CONFIG_AVFILTER
int configure_filtergraph(AVFilterGraph *graph, const char *filtergraph,
                                 AVFilterContext *source_ctx, AVFilterContext *sink_ctx);

int configure_video_filters(AVFilterGraph *graph, VideoState *is, const char *vfilters, AVFrame *frame);

int configure_audio_filters(VideoState *is, const char *afilters, int force_output_format);
#endif  /* CONFIG_AVFILTER */


#endif // _PLAYER_FILTER_H_