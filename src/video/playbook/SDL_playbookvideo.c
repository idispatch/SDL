/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2009 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

/* Dummy SDL video driver implementation; this is just enough to make an
 *  SDL-based application THINK it's got a working video driver, for
 *  applications that call SDL_Init(SDL_INIT_VIDEO) when they don't need it,
 *  and also for use as a collection of stubs when porting SDL to a new
 *  platform for which you haven't yet written a valid video driver.
 *
 * This is also a great way to determine bottlenecks: if you think that SDL
 *  is a performance problem for a given platform, enable this driver, and
 *  then see if your application runs faster without video overhead.
 *
 * Initial work by Ryan C. Gordon (icculus@icculus.org). A good portion
 *  of this was cut-and-pasted from Stephane Peter's work in the AAlib
 *  SDL video driver.  Renamed to "PLAYBOOK" by Sam Lantinga.
 */

#include "SDL_video.h"
#include "SDL_mouse.h"
#include "SDL_syswm.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include <bps/bps.h>
#include <bps/screen.h>
#include <bps/event.h>
#include <bps/orientation.h>
#include <bps/navigator.h>

#include <EGL/egl.h>
#include <bps/virtualkeyboard.h>

#include "touchcontroloverlay.h"
#include <unistd.h>
#include <time.h>

#include "SDL_playbookvideo.h"
#include "SDL_playbookvideo_c.h"
#include "SDL_playbookvideo_8bit_c.h"
#include "SDL_playbookvideo_gl_c.h"
#include "SDL_playbookevents_c.h"
#include "SDL_playbookhw_c.h"
#include "SDL_playbooktouch_c.h"
#include "SDL_playbookyuv_c.h"

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>


/*
 * The environment variable STRETCH_MODE will make major differences in how the content is laid out on the screen.
 *
 * fill will stretch arbitrary resolutions onto the full screen, distorting if needed.
 * aspect will stretch uniformly until one dimension is the screen dimension,
 *    and letterbox with black in the other dimension.
 * noscale will center a window of the correct resolution in the middle of the full screen.
 *
 * aspect is the default.
 */

#define PLAYBOOKVID_DRIVER_NAME "playbook"

static int PLAYBOOK_Available(void)
{
    return 1;
}

static void PLAYBOOK_DeleteDevice(SDL_VideoDevice *device)
{
    SDL_free(device);
}

static SDL_VideoDevice *PLAYBOOK_CreateDevice(int devindex)
{
    SDL_VideoDevice *device;

    /* Initialize all variables that we clean on shutdown */
    device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice) + sizeof(struct SDL_PrivateVideoData));
    if ( device ) {
        SDL_memset(device, 0, (sizeof *device));
        device->hidden = (struct SDL_PrivateVideoData *)(device+1);
    } else {
        SDL_OutOfMemory();
        return(0);
    }

    /* Set the function pointers */
    device->VideoInit = PLAYBOOK_VideoInit;
    device->ListModes = PLAYBOOK_ListModes;
    device->SetVideoMode = PLAYBOOK_SetVideoMode;
    device->CreateYUVOverlay = PLAYBOOK_CreateYUVOverlay;
    device->SetColors = PLAYBOOK_SetColors;
    device->UpdateRects = PLAYBOOK_UpdateRects;
    device->VideoQuit = PLAYBOOK_VideoQuit;
    device->AllocHWSurface = PLAYBOOK_AllocHWSurface;
    device->CheckHWBlit = NULL;
    device->FillHWRect = PLAYBOOK_FillHWRect;
    device->SetHWColorKey = NULL;
    device->SetHWAlpha = NULL;
    device->LockHWSurface = PLAYBOOK_LockHWSurface;
    device->UnlockHWSurface = PLAYBOOK_UnlockHWSurface;
    device->FlipHWSurface = PLAYBOOK_FlipHWSurface;
    device->FreeHWSurface = PLAYBOOK_FreeHWSurface;
    device->SetCaption = NULL;
    device->SetIcon = NULL;
    device->IconifyWindow = NULL;
    device->GrabInput = NULL;
    device->GetWMInfo = PLAYBOOK_GetWMInfo;
    device->InitOSKeymap = PLAYBOOK_InitOSKeymap;
    device->PumpEvents = PLAYBOOK_PumpEvents;

    device->GL_LoadLibrary = 0; //PLAYBOOK_GL_LoadLibrary;
    device->GL_GetProcAddress = PLAYBOOK_GL_GetProcAddress;
    device->GL_GetAttribute = PLAYBOOK_GL_GetAttribute;
    device->GL_MakeCurrent = PLAYBOOK_GL_MakeCurrent;
    device->GL_SwapBuffers = PLAYBOOK_GL_SwapBuffers;

    device->free = PLAYBOOK_DeleteDevice;

    return device;
}

VideoBootStrap PLAYBOOK_bootstrap = {
    PLAYBOOKVID_DRIVER_NAME, "SDL PlayBook (libscreen) video driver",
    PLAYBOOK_Available, PLAYBOOK_CreateDevice
};

int PLAYBOOK_8Bit_VideoInit(SDL_VideoDevice *this, SDL_PixelFormat *vformat)
{
    if (PLAYBOOK_VideoInit(this, vformat) == -1)
        return -1;
    else {
        /* Determine the screen depth (use default 32-bit depth) */
        vformat->BitsPerPixel = 8;
        vformat->BytesPerPixel = 1;
        this->info.blit_fill = 0;
        this->info.hw_available = 0;
    }
    return 0;
}

int PLAYBOOK_GetWMInfo(SDL_VideoDevice *this, SDL_SysWMinfo *info)
{
   if ( info->version.major <= SDL_MAJOR_VERSION ) {
       info->window = this->hidden->screenWindow;
       info->context = this->hidden->screenContext;
       info->mainWindow = this->hidden->mainWindow;
       return(1);
   } else {
       SDL_SetError("Application not compiled with SDL %d.%d\n",
                   SDL_MAJOR_VERSION, SDL_MINOR_VERSION);
       return(-1);
   }
}

int PLAYBOOK_VideoInit(SDL_VideoDevice *this, SDL_PixelFormat *vformat)
{
    int i;
    screen_display_t *displays = 0;
    int displayCount = 0;
    int screenResolution[2];

    int rc = screen_create_context(&this->hidden->screenContext, 0);
    if (rc) {
        SDL_SetError("Cannot create screen context: %s", strerror(errno));
        return -1;
    }

    rc = screen_create_event(&this->hidden->screenEvent);
    if (rc) {
        SDL_SetError("Cannot create event object: %s", strerror(errno));
        screen_destroy_context(this->hidden->screenContext);
        return -1;
    }

    if (BPS_SUCCESS != bps_initialize()) {
        SDL_SetError("Cannot initialize BPS library: %s", strerror(errno));
        screen_destroy_event(this->hidden->screenEvent);
        screen_destroy_context(this->hidden->screenContext);
        return -1;
    }

    if (BPS_SUCCESS != navigator_rotation_lock(getenv("AUTO_ORIENTATION") != NULL ? false : true)) {
        SDL_SetError("Cannot set rotation lock: %s", strerror(errno));
        bps_shutdown();
        screen_destroy_event(this->hidden->screenEvent);
        screen_destroy_context(this->hidden->screenContext);
        return -1;
    }

    if (BPS_SUCCESS != navigator_extend_timeout(120000, 0)) {
        SDL_SetError("Cannot extend timeout: %s", strerror(errno));
        bps_shutdown();
        screen_destroy_event(this->hidden->screenEvent);
        screen_destroy_context(this->hidden->screenContext);
        return -1;
    }

    if (BPS_SUCCESS != navigator_request_events(0)) {
        SDL_SetError("Cannot get navigator events: %s", strerror(errno));
        bps_shutdown();
        screen_destroy_event(this->hidden->screenEvent);
        screen_destroy_context(this->hidden->screenContext);
        return -1;
    }

    if (BPS_SUCCESS != screen_request_events(this->hidden->screenContext)) {
        SDL_SetError("Cannot get screen events: %s", strerror(errno));
        bps_shutdown();
        screen_destroy_event(this->hidden->screenEvent);
        screen_destroy_context(this->hidden->screenContext);
        return -1;
    }

    if (BPS_SUCCESS != virtualkeyboard_request_events(0)) {
        SDL_SetError("Cannot get VKB events: %s", strerror(errno));
        bps_shutdown();
        screen_destroy_event(this->hidden->screenEvent);
        screen_destroy_context(this->hidden->screenContext);
        return -1;
    }

    rc = screen_get_context_property_iv(this->hidden->screenContext, SCREEN_PROPERTY_DISPLAY_COUNT, &displayCount);
    if (rc || displayCount <= 0) {
        SDL_SetError("Cannot get display count: %s", strerror(errno));
        screen_stop_events(this->hidden->screenContext);
        screen_destroy_event(this->hidden->screenEvent);
        screen_destroy_context(this->hidden->screenContext);
        bps_shutdown();
        return -1;
    }

    displays = SDL_malloc(displayCount * sizeof(screen_display_t));
    if (!displays) {
        SDL_SetError("Cannot get current display: %s", strerror(errno));
        screen_stop_events(this->hidden->screenContext);
        screen_destroy_event(this->hidden->screenEvent);
        screen_destroy_context(this->hidden->screenContext);
        bps_shutdown();
        return -1;
    }

    rc = screen_get_context_property_pv(this->hidden->screenContext, SCREEN_PROPERTY_DISPLAYS, (void**)displays);
    if (rc) {
        SDL_SetError("Cannot get current display: %s", strerror(errno));
        SDL_free(displays);
        screen_stop_events(this->hidden->screenContext);
        screen_destroy_event(this->hidden->screenEvent);
        screen_destroy_context(this->hidden->screenContext);
        bps_shutdown();
        return -1;
    }

    if (getenv("WIDTH") != NULL && getenv("HEIGHT") != NULL) {
         screenResolution[0] = atoi(getenv("WIDTH"));
         screenResolution[1] = atoi(getenv("HEIGHT"));
    } else {
        rc = screen_get_display_property_iv(displays[0], SCREEN_PROPERTY_NATIVE_RESOLUTION, screenResolution);
        if (rc) {
            SDL_SetError("Cannot get native resolution: %s", strerror(errno));
            SDL_free(displays);
            screen_stop_events(this->hidden->screenContext);
            screen_destroy_event(this->hidden->screenEvent);
            screen_destroy_context(this->hidden->screenContext);
            bps_shutdown();
            return -1;
        }
    }

    // FIXME: Bad hack for PlayBook to avoid rotation issues.
    if (screenResolution[0] == 600 && screenResolution[1] == 1024) {
        int angle = 0;
        char *orientation = getenv("ORIENTATION");
        if (orientation) {
             fprintf(stderr, "Received orientation: %s\n", orientation);
             angle = atoi(orientation);
             if (angle == 0) {
                 screenResolution[0] = 1024;
                 screenResolution[1] = 600;
             }
        }
    }

    rc = screen_create_window(&this->hidden->mainWindow, this->hidden->screenContext);
    if (rc) {
        SDL_SetError("Cannot create main application window: %s", strerror(errno));
            SDL_free(displays);
        screen_stop_events(this->hidden->screenContext);
        screen_destroy_event(this->hidden->screenEvent);
        screen_destroy_context(this->hidden->screenContext);
        bps_shutdown();
        return -1;
    }

    char groupName[256];
    snprintf(groupName, 256, "SDL-window-%d", getpid());
    rc = screen_create_window_group(this->hidden->mainWindow, groupName);
    if (rc) {
        SDL_SetError("Cannot create main window group: %s", strerror(errno));
        screen_destroy_window(this->hidden->mainWindow);
        SDL_free(displays);
        screen_stop_events(this->hidden->screenContext);
        screen_destroy_event(this->hidden->screenEvent);
        screen_destroy_context(this->hidden->screenContext);
        bps_shutdown();
        return -1;
    }

    SDL_free(displays);

    this->hidden->screenWindow = 0;
    this->hidden->surface = 0;
    this->hidden->eglInfo.eglDisplay = 0;
    this->hidden->eglInfo.eglContext = 0;
    this->hidden->eglInfo.eglSurface = 0;

    for ( i=0; i<SDL_NUMMODES; ++i ) {
        this->hidden->SDL_modelist[i] = SDL_malloc(sizeof(SDL_Rect));
        this->hidden->SDL_modelist[i]->x = this->hidden->SDL_modelist[i]->y = 0;
    }

    /* Modes sorted largest to smallest */
    i = 0;

    /* 0: Default device screen size */
    this->hidden->SDL_modelist[i]->w = screenResolution[0];
    this->hidden->SDL_modelist[i++]->h = screenResolution[1];

	/* 1: BlackBerry Z10 */
    this->hidden->SDL_modelist[i]->w = 1280;
    this->hidden->SDL_modelist[i++]->h = 768;

    /* 2: BlackBerry PlayBook */
    this->hidden->SDL_modelist[i]->w = 1024;
    this->hidden->SDL_modelist[i++]->h = 600;

    /* 3: SVGA */
    this->hidden->SDL_modelist[i]->w = 800;
    this->hidden->SDL_modelist[i++]->h = 600;

    /* 4: BlackBerry Q10, Q5 */
    this->hidden->SDL_modelist[i]->w = 720;
    this->hidden->SDL_modelist[i++]->h = 720;

    /* 5: VGA */
    this->hidden->SDL_modelist[i]->w = 640;
    this->hidden->SDL_modelist[i++]->h = 480;

    /* 6: X-Mode VGA */
    this->hidden->SDL_modelist[i]->w = 320;
    this->hidden->SDL_modelist[i++]->h = 400;

    /* 7: QVGA */
    this->hidden->SDL_modelist[i]->w = 320;
    this->hidden->SDL_modelist[i++]->h = 240;

    /* 8: CGA */
    this->hidden->SDL_modelist[i]->w = 320;
    this->hidden->SDL_modelist[i++]->h = 200;

    /* 9: Sentinel (no screen) */
    this->hidden->SDL_modelist[i] = NULL;

    /* Determine the screen depth (use default 32-bit depth) */
    vformat->BitsPerPixel = 32;
    vformat->BytesPerPixel = 4;

    /* Hardware surfaces are not available */
    this->info.hw_available = 1;
    this->info.blit_fill = 1;
    this->info.blit_hw = 0;
    this->info.blit_hw_A = 0;
    this->info.blit_hw_CC = 0;

    /* There is no window manager to talk to */
    this->info.wm_available = 0;

    /* Full screen size is screenResolution */
    this->info.current_w = screenResolution[0];
    this->info.current_h = screenResolution[1];

    /* We're done! */
    return 0;
}

SDL_Rect **PLAYBOOK_ListModes(SDL_VideoDevice *this, SDL_PixelFormat *format, Uint32 flags)
{
    if (flags & SDL_FULLSCREEN ) {
        return this->hidden->SDL_modelist;
    } else {
        return (SDL_Rect**)-1; // We only support full-screen video modes.
    }
}

screen_window_t PLAYBOOK_CreateWindow(SDL_VideoDevice *this, SDL_Surface *current,
        int width, int height, int bpp)
{
    screen_window_t screenWindow;
    int rc = 0;
    int position[2] = {0, 0};
    int idle_mode = SCREEN_IDLE_MODE_KEEP_AWAKE; // TODO: Handle idle gracefully?

    if (!this->hidden->screenWindow) {
        char groupName[256];

        rc = screen_create_window(&screenWindow, this->hidden->screenContext);
        if (rc) {
            SDL_SetError("Cannot create window: %s", strerror(errno));
            return NULL;
        }

        snprintf(groupName, 256, "sdl-%dx%dx%d-%u", width, height, bpp, time(NULL));
        rc = screen_create_window_group(screenWindow, groupName);
        if (rc) {
            SDL_SetError("Cannot set window group: %s", strerror(errno));
            screen_destroy_window(screenWindow);
            return NULL;
        }
    } else {
        if (current->hwdata)
            SDL_free(current->hwdata);
        if (this->hidden->tco_context) {
            tco_shutdown(this->hidden->tco_context);
            this->hidden->tco_context = NULL;
        }
        screen_destroy_window_buffers(this->hidden->screenWindow);
        screenWindow = this->hidden->screenWindow;
    }

    rc = screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_POSITION, position);
    if (rc) {
        SDL_SetError("Cannot position window: %s", strerror(errno));
        screen_destroy_window(screenWindow);
        return NULL;
    }

    rc = screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_IDLE_MODE, &idle_mode);
    if (rc) {
        SDL_SetError("Cannot disable idle mode: %s", strerror(errno));
        screen_destroy_window(screenWindow);
        return NULL;
    }

    return screenWindow;
}

SDL_Surface *PLAYBOOK_SetVideoMode(SDL_VideoDevice *this, SDL_Surface *current,
                int width, int height, int bpp, Uint32 flags)
{
#ifdef _DEBUG
    fprintf(stderr, "SetVideoMode: %dx%d %dbpp\n", width, height, bpp);
#endif
    if (width == 640 && height == 400) {
        this->hidden->eventYOffset = 40;
        height = 480;
    }
    if (flags & SDL_OPENGL) {
        return PLAYBOOK_SetVideoMode_GL(this, current, width, height, bpp, flags);
    }
    screen_window_t screenWindow = PLAYBOOK_CreateWindow(this, current, width, height, bpp);
    if (screenWindow == NULL)
        return NULL;

    int rc;
    int format = 0;

    int sizeOfWindow[2];
#ifdef __STRETCHED_VIDEO_SCREEN__
    rc = screen_get_window_property_iv(screenWindow, SCREEN_PROPERTY_SIZE, sizeOfWindow);
    if (rc) {
        SDL_SetError("Cannot get resolution: %s", strerror(errno));
        screen_destroy_window(screenWindow);
        return NULL;
    }
#else
    int hwResolution[2];

    rc = screen_get_window_property_iv(screenWindow, SCREEN_PROPERTY_SIZE, hwResolution);
    if (rc) {
        SDL_SetError("Cannot get resolution: %s", strerror(errno));
        screen_destroy_window(screenWindow);
        return NULL;
    }

    float hwRatio, appRatio;
    hwRatio = (float)hwResolution[0]/(float)hwResolution[1];
    appRatio = (float)width/(float)height;

    double newResolution[2];
    if(hwRatio > appRatio){
        newResolution[0] = ((double)height / ((double)hwResolution[1] / (double)hwResolution[0]));
        newResolution[1] = (double)height;
    }else{
        newResolution[0] = (double)width;
        newResolution[1] = (((double)hwResolution[1] / (double)hwResolution[0]) * (double)width);
    }

    sizeOfWindow[0] = (int)(ceil(newResolution[0]));
    sizeOfWindow[1] = (int)(ceil(newResolution[1]));

    rc = screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_SIZE, sizeOfWindow);
    if (rc) {
        SDL_SetError("Cannot resize window: %s", strerror(errno));
        screen_destroy_window(screenWindow);
        return NULL;
    }
#endif


#ifdef __STRETCHED_VIDEO_SCREEN__
    int sizeOfBuffer[2] = {width, height};
#else
    int sizeOfBuffer[2] = {sizeOfWindow[0], sizeOfWindow[1]};
#endif

    rc = screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_BUFFER_SIZE, sizeOfBuffer);
    if (rc) {
        SDL_SetError("Cannot resize window buffer: %s", strerror(errno));
        screen_destroy_window(screenWindow);
        return NULL;
    }

    switch (bpp) {
    case 8:
        fprintf(stderr, "Unsupported bpp: set pb-8bit environment variable!\n");
        format = SCREEN_FORMAT_BYTE;
        return NULL;
        break;
    case 16:
        SDL_ReallocFormat(current, 16, 0x0000f800, 0x0000007e0, 0x0000001f, 0);
        format = SCREEN_FORMAT_RGB565;
        break;
    case 32:
        SDL_ReallocFormat(current, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0);
        format = SCREEN_FORMAT_RGBX8888;
        break;
    }
    rc = screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_FORMAT, &format);
    if (rc) {
        SDL_SetError("Cannot set window format: %s", strerror(errno));
        screen_destroy_window(screenWindow);
        return NULL;
    }

    int usage = SCREEN_USAGE_NATIVE | SCREEN_USAGE_READ | SCREEN_USAGE_WRITE; // FIXME: GL needs other usage
    rc = screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_USAGE, &usage);
    if (rc) {
        SDL_SetError("Cannot set window usage: %s", strerror(errno));
        screen_destroy_window(screenWindow);
        return NULL;
    }

    int angle = 0;
    char *orientation = getenv("ORIENTATION");
    if (orientation) {
         angle = atoi(orientation);
    }
    rc = screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_ROTATION, &angle);
    if (rc) {
        SDL_SetError("Cannot set window rotation: %s", strerror(errno));
        return NULL;
    }

    int bufferCount = 1; // FIXME: (flags & SDL_DOUBLEBUF)?2:1; - Currently double-buffered surfaces are slow!
    rc = screen_create_window_buffers(screenWindow, bufferCount);
    if (rc) {
        SDL_SetError("Cannot create window buffer: %s", strerror(errno));
        return NULL;
    }

    screen_buffer_t windowBuffer[bufferCount];
    rc = screen_get_window_property_pv(screenWindow,
            SCREEN_PROPERTY_RENDER_BUFFERS, (void**)&windowBuffer);
    if (rc) {
        SDL_SetError("Cannot get window render buffers: %s", strerror(errno));
        return NULL;
    }

    rc = screen_get_buffer_property_pv(windowBuffer[0], SCREEN_PROPERTY_POINTER, &this->hidden->pixels);
    if (rc) {
        SDL_SetError("Cannot get buffer pointer: %s", strerror(errno));
        return NULL;
    }

    rc = screen_get_buffer_property_iv(windowBuffer[0], SCREEN_PROPERTY_STRIDE, &this->hidden->pitch);
    if (rc) {
        SDL_SetError("Cannot get stride: %s", strerror(errno));
        return NULL;
    }

    initialize_touch_controls(this, screenWindow);

    this->hidden->frontBuffer = windowBuffer[0];
    this->hidden->screenWindow = screenWindow;

    current->hwdata = SDL_malloc(sizeof(struct private_hwdata));
    current->hwdata->pixmap = 0;
    current->hwdata->window = screenWindow;
    current->hwdata->front = windowBuffer[0];
    if (bufferCount > 1) {
        current->flags |= SDL_DOUBLEBUF;
        current->hwdata->back = windowBuffer[1];
    } else {
        current->hwdata->back = 0;
    }
    current->flags &= ~SDL_RESIZABLE; /* no resize for Direct Context */
    current->flags |= SDL_FULLSCREEN;
    current->flags |= SDL_HWSURFACE;
#ifdef __STRETCHED_VIDEO_SCREEN__
    current->w = width;
    current->h = height;
#else
    current->w = sizeOfWindow[0];
    current->h = sizeOfWindow[1];
#endif
    current->pitch = this->hidden->pitch;
    current->pixels = this->hidden->pixels;
    this->hidden->surface = current;
    return current;
}

void PLAYBOOK_UpdateRects(SDL_VideoDevice *this, int numrects, SDL_Rect *rects)
{
    static int dirtyRects[256*4];
    int index = 0, i = 0;
    for (i=0; i<numrects; i++) {
        dirtyRects[index] = rects[i].x;
        dirtyRects[index+1] = rects[i].y;
        dirtyRects[index+2] = rects[i].x + rects[i].w;
        dirtyRects[index+3] = rects[i].y + rects[i].h;
        index += 4;
    }

    screen_post_window(this->hidden->screenWindow, this->hidden->frontBuffer, numrects, dirtyRects, 0);
}

int PLAYBOOK_SetColors(SDL_VideoDevice *this, int firstcolor, int ncolors, SDL_Color *colors)
{
    return(1);
}

/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
// FIXME: Fix up cleanup process
void PLAYBOOK_VideoQuit(SDL_VideoDevice *this)
{
    if (this->hidden->screenWindow) {
        if (this->hidden->eglInfo.eglDisplay) {
            eglDestroySurface(this->hidden->eglInfo.eglDisplay, this->hidden->eglInfo.eglSurface);
            screen_destroy_window(this->hidden->screenWindow);
            eglDestroyContext(this->hidden->eglInfo.eglDisplay, this->hidden->eglInfo.eglContext);
            eglTerminate(this->hidden->eglInfo.eglDisplay);
        } else {
            screen_destroy_window_buffers(this->hidden->screenWindow);
            screen_destroy_window(this->hidden->screenWindow);
        }
    }
    screen_stop_events(this->hidden->screenContext);
    screen_destroy_event(this->hidden->screenEvent);
    screen_destroy_context(this->hidden->screenContext);

    if (this->hidden->tco_context) {
        tco_shutdown(this->hidden->tco_context);
        this->hidden->tco_context = NULL;
    }
    this->screen = 0;
    bps_shutdown();
}
