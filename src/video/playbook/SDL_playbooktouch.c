#include "SDL_config.h"

#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"
#include "SDL_playbookvideo.h"
#include <stdio.h>
#include <screen/screen.h>

int handleKey(int sym, int mod, int scancode, uint16_t unicode, int event)
{
    int sdlEvent;
    switch (event)
    {
    case TCO_KB_DOWN:
        sdlEvent = SDL_PRESSED;
        break;
    case TCO_KB_UP:
        sdlEvent = SDL_RELEASED;
        break;
    default:
        return TCO_UNHANDLED;
    }

    SDL_keysym keysym;
    keysym.sym = sym;
    keysym.mod = mod;
    keysym.scancode = scancode;
    keysym.unicode = unicode;
    SDL_PrivateKeyboard(sdlEvent, &keysym);
    return TCO_SUCCESS;
}

int handleDPad(int angle, int event)
{
    static int pressed[4] = {0, 0, 0, 0}; // Up, Down, Right, Left
    int tmp[4] = {0,0,0,0};
    switch (event)
    {
    case TCO_KB_DOWN:
        {
            if (angle <= -158 || angle >= 158) {
                // Left: -180 to -158, 158 to 180
                tmp[3] = 1;
            } else if (angle <= -103) {
                tmp[3] = 1;
                tmp[0] = 1;
                // Up-Left: -157 to -103
            } else if (angle <= -68) {
                tmp[0] = 1;
                // Up: -68 to -102
            } else if (angle <= -23) {
                tmp[0] = 1;
                tmp[2] = 1;
                // Up-Right: -23 to -67
            } else if (angle <= 22) {
                tmp[2] = 1;
                // Right: -22 to 22
            } else if (angle <= 67) {
                tmp[1] = 1;
                tmp[2] = 1;
                // Down-Right: 23 to 67
            } else if (angle <= 102) {
                tmp[1] = 1;
                // Down: 68 to 102
            } else if (angle <= 157) {
                tmp[1] = 1;
                tmp[3] = 1;
                // Down-Left: 103 to 157
            } else {
                fprintf(stderr, "Unknown dpad angle: %d\n", angle);
                return TCO_UNHANDLED;
            }
        }
        break;
    case TCO_KB_UP:
        break;
    default:
        return TCO_UNHANDLED;
    }

    int sdlState = SDL_PRESSED;
    SDL_keysym keysym;
    int scancodes[4] = {72, 75, 77, 80}; // From DosBox, keyboard.cpp
    int i;
    for (i=0; i<4; i++) {
        if (pressed[i] != tmp[i]) {
            if (tmp[i]) {
                sdlState = SDL_PRESSED;
            } else {
                sdlState = SDL_RELEASED;
            }
            keysym.sym = SDLK_UP + i;
            keysym.scancode = scancodes[i];
            SDL_PrivateKeyboard(sdlState, &keysym);
            pressed[i] = tmp[i];
        }
    }
    return TCO_SUCCESS;
}

int handleTouch(int dx, int dy)
{
    SDL_PrivateMouseMotion(SDL_GetMouseState(0, 0), 1, dx, dy);
    return TCO_SUCCESS;
}

int handleMouseButton(int button, int mask, int event)
{
    int mouseX, mouseY;
    int sdlEvent;
    int sdlButton;

    switch (event)
    {
    case TCO_MOUSE_BUTTON_UP:
        sdlEvent = SDL_RELEASED;
        break;
    case TCO_MOUSE_BUTTON_DOWN:
        sdlEvent = SDL_PRESSED;
        break;
    default:
        fprintf(stderr, "No mouse button event?? (%d)\n", event);
        sdlEvent = SDL_PRESSED;
        break;
    }

    switch (button)
    {
    case TCO_MOUSE_LEFT_BUTTON:
        sdlButton = SDL_BUTTON_LEFT;
        break;
    case TCO_MOUSE_RIGHT_BUTTON:
        sdlButton = SDL_BUTTON_RIGHT;
        break;
    case TCO_MOUSE_MIDDLE_BUTTON:
        sdlButton = SDL_BUTTON_MIDDLE;
        break;
    default:
        fprintf(stderr, "No mouse button?? (%d)\n", button);
        sdlButton = SDL_BUTTON_LEFT;
        break;
    }
    SDL_GetMouseState(&mouseX, &mouseY);
    mouseY += (current_video->hidden)->eventYOffset;

    SDL_keysym shift, ctrl, alt;
    shift.scancode = 42;
    shift.sym = SDLK_LSHIFT;
    ctrl.scancode = 29;
    ctrl.sym = SDLK_LCTRL;
    alt.scancode = 56;
    alt.sym = SDLK_LALT;

    if (sdlEvent == SDL_PRESSED) {
        if (mask & TCO_SHIFT) {
            SDL_PrivateKeyboard(SDL_PRESSED, &shift);
        }
        if (mask & TCO_CTRL) {
            SDL_PrivateKeyboard(SDL_PRESSED, &ctrl);
        }
        if (mask & TCO_ALT) {
            SDL_PrivateKeyboard(SDL_PRESSED, &alt);
        }
    }
    SDL_PrivateMouseButton(sdlEvent, sdlButton, mouseX, mouseY);
    if (sdlEvent == SDL_RELEASED) {
        if (mask & TCO_SHIFT) {
            SDL_PrivateKeyboard(SDL_RELEASED, &shift);
        }
        if (mask & TCO_CTRL) {
            SDL_PrivateKeyboard(SDL_RELEASED, &ctrl);
        }
        if (mask & TCO_ALT) {
            SDL_PrivateKeyboard(SDL_RELEASED, &alt);
        }
    }
    return TCO_SUCCESS;
}

int handleTap()
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    SDL_PrivateMouseButton(SDL_PRESSED, SDL_BUTTON_LEFT, mouseX, mouseY);
    SDL_PrivateMouseButton(SDL_RELEASED, SDL_BUTTON_LEFT, mouseX, mouseY);
    return TCO_SUCCESS;
}

int handleTouchScreen(int x, int y, int tap, int hold)
{
    if (tap) {
        SDL_PrivateMouseButton(SDL_PRESSED, SDL_BUTTON_LEFT, x, y);
        SDL_PrivateMouseButton(SDL_RELEASED, SDL_BUTTON_LEFT, x, y);
    } else if (hold) {
        SDL_PrivateMouseButton(SDL_PRESSED, SDL_BUTTON_RIGHT, x, y);
        SDL_PrivateMouseButton(SDL_RELEASED, SDL_BUTTON_RIGHT, x, y);
    } else {
        SDL_PrivateMouseMotion(SDL_GetMouseState(0, 0), 0, x, y);
    }
    return TCO_SUCCESS;
}

int initialize_touch_controls(SDL_VideoDevice *this, screen_window_t screenWindow)
{
    struct tco_callbacks callbacks = {
        handleKey,
        handleDPad,
        handleTouch,
        handleMouseButton,
        handleTap,
        handleTouchScreen
    };

    int sizeOfWindow[2];
    int rc = screen_get_window_property_iv(screenWindow, SCREEN_PROPERTY_SIZE, sizeOfWindow);
    if(rc) {
        return TCO_FAILURE;
    }

    char controlsFileName[128];
    snprintf(controlsFileName,
             sizeof(controlsFileName),
             "app/native/controls-%dx%d.json", sizeOfWindow[0], sizeOfWindow[1]);
    char userControlsFileName[128];
    snprintf(userControlsFileName,
             sizeof(userControlsFileName),
             "data/controls-%dx%d.json", sizeOfWindow[0], sizeOfWindow[1]);
    if(tco_initialize(&this->hidden->tco_context,
                      this->hidden->screenContext,
                      callbacks) == TCO_SUCCESS) {
        if (tco_loadcontrols(this->hidden->tco_context,
                             controlsFileName,
                             userControlsFileName) == TCO_SUCCESS) {
            tco_draw(this->hidden->tco_context, screenWindow);
            return TCO_SUCCESS;
        } else {
            this->hidden->tco_context = NULL;
            fprintf(stderr, "Failed to initialize touch controls\n");
            tco_shutdown(this->hidden->tco_context);
        }
    }
    return TCO_FAILURE;
}
