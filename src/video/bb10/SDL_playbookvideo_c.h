/*
 * SDL_playbookvideo_c.h
 *
 *  Created on: Nov 23, 2011
 *      Author: jnicholl
 */

#ifndef SDL_PLAYBOOKVIDEO_C_H_
#define SDL_PLAYBOOKVIDEO_C_H_

#include "SDL_config.h"

#include "SDL_video.h"
#include "SDL_playbookvideo.h"
#include <screen/screen.h>

/* Initialization/Query functions */
extern int PLAYBOOK_GetWMInfo(SDL_VideoDevice *this, SDL_SysWMinfo *info);
extern int PLAYBOOK_VideoInit(SDL_VideoDevice *this, SDL_PixelFormat *vformat);
extern SDL_Rect **PLAYBOOK_ListModes(SDL_VideoDevice *this, SDL_PixelFormat *format, Uint32 flags);
extern screen_window_t PLAYBOOK_CreateWindow();
extern SDL_Surface *PLAYBOOK_SetVideoMode(SDL_VideoDevice *this, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
extern int PLAYBOOK_SetColors(SDL_VideoDevice *this, int firstcolor, int ncolors, SDL_Color *colors);
extern void PLAYBOOK_VideoQuit(SDL_VideoDevice *this);

/* etc. */
extern int PLAYBOOK_SetupStretch(SDL_VideoDevice *this, screen_window_t screenWindow, int width, int height);
extern void PLAYBOOK_UpdateRects(SDL_VideoDevice *this, int numrects, SDL_Rect *rects);

#endif /* SDL_PLAYBOOKVIDEO_C_H_ */
