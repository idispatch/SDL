/*
 * SDL_playbookvideo_8bit_c.h
 *
 *  Created on: Nov 23, 2011
 *      Author: jnicholl
 */

#ifndef SDL_PLAYBOOKVIDEO_8BIT_C_H_
#define SDL_PLAYBOOKVIDEO_8BIT_C_H_

#include "SDL_config.h"

#include "SDL_video.h"
#include "SDL_playbookvideo.h"

extern int PLAYBOOK_8Bit_VideoInit(SDL_VideoDevice *this, SDL_PixelFormat *vformat);
extern SDL_Rect **PLAYBOOK_8Bit_ListModes(SDL_VideoDevice *this, SDL_PixelFormat *format, Uint32 flags);
extern SDL_Surface *PLAYBOOK_8Bit_SetVideoMode(SDL_VideoDevice *this, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
extern void PLAYBOOK_8Bit_VideoQuit(SDL_VideoDevice *this);
extern int PLAYBOOK_8Bit_SetColors(SDL_VideoDevice *this, int firstcolor, int ncolors, SDL_Color *colors);
extern void PLAYBOOK_8Bit_UpdateRects(SDL_VideoDevice *this, int numrects, SDL_Rect *rects);

#endif /* SDL_PLAYBOOKVIDEO_8BIT_C_H_ */
