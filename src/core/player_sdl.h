#ifndef _PLAYER_SDL_H_
#define _PLAYER_SDL_H_

#include "player_pod.h"

void fill_rectangle(SDL_Renderer *renderer, int x, int y, int w, int h);

int realloc_texture(SDL_Renderer *renderer, SDL_Texture **texture, Uint32 new_format, int new_width, int new_height, SDL_BlendMode blendmode, int init_texture);

void calculate_display_rect(SDL_Rect *rect,
                            int scr_xleft, int scr_ytop, int scr_width, int scr_height,
                            int pic_width, int pic_height, AVRational pic_sar);

void get_sdl_pix_fmt_and_blendmode(int format, Uint32 *sdl_pix_fmt, SDL_BlendMode *sdl_blendmode);

int upload_texture(SDL_Renderer *renderer, SDL_Texture **tex, AVFrame *frame, struct SwsContext **img_convert_ctx, int sws_flags);

void set_sdl_yuv_conversion_mode(AVFrame *frame);

#endif // _PLAYER_SDL_H_