#ifndef _PLAYER_SDL_H_
#define _PLAYER_SDL_H_

#include "player_pod.h"

int realloc_texture(SDL_Texture **texture, Uint32 new_format, int new_width, int new_height, SDL_BlendMode blendmode, int init_texture);

void calculate_display_rect(SDL_Rect *rect,
                                   int scr_xleft, int scr_ytop, int scr_width, int scr_height,
                                   int pic_width, int pic_height, AVRational pic_sar);

void get_sdl_pix_fmt_and_blendmode(int format, Uint32 *sdl_pix_fmt, SDL_BlendMode *sdl_blendmode);

int upload_texture(SDL_Texture **tex, AVFrame *frame, struct SwsContext **img_convert_ctx);

void set_default_window_size(int width, int height, AVRational sar);

#endif // _PLAYER_SDL_H_