/*
 * SDL_playbookvideo_gl.c
 *
 *  Created on: 2012-01-17
 *      Author: asimonov
 */

#include "SDL_config.h"

#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include <bps/bps.h>
#include <bps/screen.h>
#include <bps/event.h>
#include <bps/orientation.h>
#include <bps/navigator.h>

#include "touchcontroloverlay.h"

#include "SDL_playbookvideo_c.h"
#include "SDL_playbookvideo_gl_c.h"
#include "SDL_playbookevents_c.h"
#include "SDL_playbookhw_c.h"
#include "SDL_playbooktouch_c.h"
#include "SDL_playbookyuv_c.h"

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <errno.h> // ::errno
#include <time.h> // struct tm, clock_gettime

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

static void egl_perror(const char *msg)
{
    static const char *errmsg[] = {
        "function succeeded",
        "EGL is not initialized, or could not be initialized, for the specified display",
        "cannot access a requested resource",
        "failed to allocate resources for the requested operation",
        "an unrecognized attribute or attribute value was passed in an attribute list",
        "an EGLConfig argument does not name a valid EGLConfig",
        "an EGLContext argument does not name a valid EGLContext",
        "the current surface of the calling thread is no longer valid",
        "an EGLDisplay argument does not name a valid EGLDisplay",
        "arguments are inconsistent",
        "an EGLNativePixmapType argument does not refer to a valid native pixmap",
        "an EGLNativeWindowType argument does not refer to a valid native window",
        "one or more argument values are invalid",
        "an EGLSurface argument does not name a valid surface configured for rendering",
        "a power management event has occurred",
    };

    fprintf(stderr, "%s: %s\n", msg, errmsg[eglGetError() - EGL_SUCCESS]);
}

SDL_Surface *PLAYBOOK_SetVideoMode_GL(SDL_VideoDevice *this, SDL_Surface *current,
                int width, int height, int bpp, Uint32 flags)
{
    int rc;
    EGLint attributes[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
        EGL_NONE
    };
    EGLint contextAttributes[3] = { EGL_CONTEXT_CLIENT_VERSION, 1, EGL_NONE };
    EGLConfig configs[1];
    EGLint configCount;
    screen_window_t screenWindow;
    int format = SCREEN_FORMAT_RGBX8888;
    int sizeOfWindow[2] = {this->info.current_w, this->info.current_h};
    int sizeOfBuffer[2] = {width, height};
    int usage = SCREEN_USAGE_OPENGL_ES1;
    EGLint eglSurfaceAttributes[3] = { EGL_RENDER_BUFFER, EGL_BACK_BUFFER, EGL_NONE };

    if (this->gl_config.major_version == 2) {
        contextAttributes[1] = 2;
        attributes[11] = EGL_OPENGL_ES2_BIT;
        usage = SCREEN_USAGE_OPENGL_ES2;
    }

    if (getenv("WIDTH") != NULL && getenv("HEIGHT") != NULL) {
        sizeOfWindow[0] = atoi(getenv("WIDTH"));
        sizeOfWindow[1] = atoi(getenv("HEIGHT"));
    } else {
        rc = screen_get_window_property_iv(this->hidden->mainWindow, SCREEN_PROPERTY_SIZE, sizeOfWindow);
        if (rc) {
            goto error1;
        }

        if (this->hidden->screenWindow) {
            rc = eglMakeCurrent(this->hidden->eglInfo.eglDisplay, 0, 0, 0);
            if (rc != EGL_TRUE) {
                egl_perror("eglMakeNoneCurrent");
                goto error1;
            }

            rc = eglDestroySurface(this->hidden->eglInfo.eglDisplay, this->hidden->eglInfo.eglSurface);
            if (rc != EGL_TRUE) {
                egl_perror("eglDestroySurface");
                goto error1;
            }

            this->hidden->eglInfo.eglSurface = 0;
       }
    }

    if (!this->hidden->eglInfo.eglDisplay) {
        this->hidden->eglInfo.eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (this->hidden->eglInfo.eglDisplay == EGL_NO_DISPLAY) {
            egl_perror("eglGetDisplay");
            goto error1;
        }
    }

    rc = eglInitialize(this->hidden->eglInfo.eglDisplay, NULL, NULL);
    if (rc != EGL_TRUE) {
        egl_perror("eglInitialize");
        goto error1;
    }

    rc = eglBindAPI(EGL_OPENGL_ES_API);
    if (rc != EGL_TRUE) {
        egl_perror("eglBindAPI");
        goto error2;
    }

    rc = eglChooseConfig(this->hidden->eglInfo.eglDisplay, attributes, configs, 1, &configCount);
    if (rc != EGL_TRUE)    {
        egl_perror("eglChooseConfig");
        goto error2;
    } else if (configCount <= 0)    {
        fprintf(stderr, "No matching configurations found.");
        goto error2;
    }

    if (!this->hidden->eglInfo.eglContext) {
        this->hidden->eglInfo.eglContext = eglCreateContext(this->hidden->eglInfo.eglDisplay, configs[0], EGL_NO_CONTEXT, contextAttributes);
        if (this->hidden->eglInfo.eglContext == EGL_NO_CONTEXT)    {
            egl_perror("eglCreateContext");
            goto error2;
        }
    }

    screenWindow = PLAYBOOK_CreateWindow(this, current, width, height, bpp);
    if (screenWindow == NULL) {
        goto error3;
    }

    rc = PLAYBOOK_SetupStretch(this, screenWindow, width, height);
    if (rc) {
        SDL_SetError("Failed to set window size: %s", strerror(errno));
        goto error4;
    }

    rc = screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_FORMAT, &format);
    if (rc) {
        SDL_SetError("Cannot set window format: %s", strerror(errno));
        goto error4;
    }

    rc = screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_USAGE, &usage);
    if (rc) {
        SDL_SetError("Cannot set window usage: %s", strerror(errno));
        goto error4;
    }

    int angle = 0;
    char *orientation = getenv("ORIENTATION");
    if (orientation) {
         angle = atoi(orientation);
    }
    rc = screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_ROTATION, &angle);
    if (rc) {
        SDL_SetError("Cannot set window rotation: %s", strerror(errno));
        goto error4;
    }

    rc = screen_create_window_buffers(screenWindow, 2);
    if (rc) {
        SDL_SetError("Cannot create window buffers: %s", strerror(errno));
        goto error4;
    }

    this->hidden->eglInfo.eglSurface = eglCreateWindowSurface(this->hidden->eglInfo.eglDisplay, configs[0],
            screenWindow, (EGLint*)&eglSurfaceAttributes);
    if (this->hidden->eglInfo.eglSurface == EGL_NO_SURFACE) {
        egl_perror("eglCreateWindowSurface");
        goto error4;
    }

    rc = eglMakeCurrent(this->hidden->eglInfo.eglDisplay, this->hidden->eglInfo.eglSurface, this->hidden->eglInfo.eglSurface, this->hidden->eglInfo.eglContext);
    if (rc != EGL_TRUE) {
        egl_perror("eglMakeCurrent");
        goto error5;
    }

    initialize_touch_controls(this, screenWindow);

    this->hidden->screenWindow = screenWindow;

    current->flags &= ~SDL_RESIZABLE;
    current->flags |= SDL_FULLSCREEN;
    current->flags |= SDL_HWSURFACE;
    current->flags |= SDL_OPENGL;
    current->w = width;
    current->h = height;
    current->pixels = 0;
    this->hidden->surface = current;
    return current;
error5:
    eglDestroySurface(this->hidden->eglInfo.eglDisplay, this->hidden->eglInfo.eglSurface);
    this->hidden->eglInfo.eglSurface = 0;
error4:
    screen_destroy_window(screenWindow);
error3:
    eglDestroyContext(this->hidden->eglInfo.eglDisplay, this->hidden->eglInfo.eglContext);
    this->hidden->eglInfo.eglContext = 0;
error2:
    eglTerminate(this->hidden->eglInfo.eglDisplay);
    this->hidden->eglInfo.eglDisplay = 0;
error1:
    return NULL;
}

/* Sets the dll to use for OpenGL and loads it */
int PLAYBOOK_GL_LoadLibrary(SDL_VideoDevice *this, const char *path)
{
    fprintf(stderr, "%s\n", __FUNCTION__);
    return 0;
}

/* Retrieves the address of a function in the gl library */
void* PLAYBOOK_GL_GetProcAddress(SDL_VideoDevice *this, const char *proc) {
    fprintf(stderr, "%s\n", __FUNCTION__);
    return 0;
}

/* Get attribute information from the windowing system. */
int PLAYBOOK_GL_GetAttribute(SDL_VideoDevice *this, SDL_GLattr attrib, int* value)
{
    fprintf(stderr, "%s\n", __FUNCTION__);
    return 0;
}

/* Make the context associated with this driver current */
int PLAYBOOK_GL_MakeCurrent(SDL_VideoDevice *this)
{
    int rc = eglMakeCurrent(this->hidden->eglInfo.eglDisplay, this->hidden->eglInfo.eglSurface, this->hidden->eglInfo.eglSurface, this->hidden->eglInfo.eglContext);
//    fprintf(stderr, "%s: %d\n", __FUNCTION__, rc);
    return rc;
}

/* Swap the current buffers in double buffer mode. */
void PLAYBOOK_GL_SwapBuffers(SDL_VideoDevice *this)
{
//    fprintf(stderr, "%s\n", __FUNCTION__);
    eglSwapBuffers(this->hidden->eglInfo.eglDisplay, this->hidden->eglInfo.eglSurface);
}
