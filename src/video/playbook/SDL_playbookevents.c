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
#include "SDL_syswm.h"
#include "SDL_config.h"
#include "SDL.h"
#include "../../events/SDL_sysevents.h"
#include "../../events/SDL_events_c.h"
#include "SDL_keysym.h"

#include "SDL_playbookvideo.h"
#include "SDL_playbookevents_c.h"

#include <bps/bps.h>
#include <bps/screen.h>
#include <bps/event.h>
#include <bps/orientation.h>
#include <bps/navigator.h>
#include <bps/virtualkeyboard.h>
#include "touchcontroloverlay.h"

#include <errno.h>

#define SDLK_PB_TILDE 160 // Conflicts with SDLK_WORLD_0.
static SDL_keysym Playbook_Keycodes[256];
static SDLKey *Playbook_specialsyms;

struct TouchEvent {
    int pending;
    int touching;
    int pos[2];
};
static struct TouchEvent moveEvent;
static int keyboardVisible = 0;


static void handlePointerEvent(screen_event_t event, screen_window_t window)
{
    int buttonState = 0;
    screen_get_event_property_iv(event, SCREEN_PROPERTY_BUTTONS, &buttonState);

    int coords[2];
    screen_get_event_property_iv(event, SCREEN_PROPERTY_SOURCE_POSITION, coords);

    int screen_coords[2];
    screen_get_event_property_iv(event, SCREEN_PROPERTY_POSITION, screen_coords);

    int wheel_delta;
    screen_get_event_property_iv(event, SCREEN_PROPERTY_MOUSE_WHEEL, &wheel_delta);

    if (coords[1] < 0) {
        fprintf(stderr, "Detected pointer swipe event: %d,%d\n", coords[0], coords[1]);
        return;
    }
    if (wheel_delta != 0) {
        int button;
        if ( wheel_delta > 0 )
            button = SDL_BUTTON_WHEELDOWN;
        else if ( wheel_delta < 0 )
            button = SDL_BUTTON_WHEELUP;
        SDL_PrivateMouseButton(
            SDL_PRESSED, button, 0, 0);
        SDL_PrivateMouseButton(
            SDL_RELEASED, button, 0, 0);
    }

    // FIXME: Pointer events have never been tested.
    static int lastButtonState = 0;
    if (lastButtonState == buttonState) {
        moveEvent.touching = buttonState;
        moveEvent.pos[0] = coords[0];
        moveEvent.pos[1] = coords[1];
        moveEvent.pending = 1;
        return;
    }
    lastButtonState = buttonState;

    SDL_PrivateMouseButton(buttonState ? SDL_PRESSED : SDL_RELEASED, SDL_BUTTON_LEFT, coords[0], coords[1]); // FIXME: window
    moveEvent.pending = 0;
}

static int TranslateBluetoothKeyboard(int sym, int mods, int flags, int scan, int cap, SDL_keysym *keysym)
{
    if (flags == 32) {
        return 0; // No translation for this - this is an addition message sent
        // with arrow keys, right ctrl, right ctrl and pause
    }
    // FIXME: Figure out how to separate left and right modifiers
    if (scan > 128)
        scan -= 128; // Keyup events have the high bit set, but we want to have the same scan for down and up.
    keysym->scancode = scan;
    keysym->mod = 0;
    if (mods & (0x1))
        keysym->mod |= KMOD_LSHIFT;
    if (mods & (0x2))
        keysym->mod |= KMOD_LCTRL;
    if (mods & (0x4))
        keysym->mod |= KMOD_LALT;
    if (mods & (0x10000))
        keysym->mod |= KMOD_CAPS;
    if (mods & (0x20000)) // FIXME: guessing
        keysym->mod |= KMOD_NUM;

    if (sym & 0xf000) {
        sym = sym & 0xff;
        keysym->sym = Playbook_specialsyms[sym];
    } else {
        keysym->sym = Playbook_Keycodes[sym].sym;
    }

    return 1;
}

static int TranslateVKB(int sym, int mods, int flags, int scan, int cap, SDL_keysym *keysym)
{
    int shifted = 0;
    // FIXME: Keyboard handling (modifiers are currently ignored, some keys are as well)
    if (sym & 0xf000) {
        sym = sym & 0xff;
        keysym->sym = Playbook_specialsyms[sym];
    } else {
        keysym->sym = Playbook_Keycodes[sym].sym;
    }

    if (mods & (0x1))
        shifted = 1;

    // FIXME: These scan codes are really just implemented this way for DOSbox.
    // See keyboard.cpp inside DOSbox (KEYBOARD_AddKey) for a reference.
    switch (keysym->sym)
    {
    case SDLK_EXCLAIM:
        shifted = 1;
        /* no break */
    case SDLK_1:
        keysym->scancode = 2;
        break;
    case SDLK_AT:
        shifted = 1;
        /* no break */
    case SDLK_2:
        keysym->scancode = 3;
        break;
    case SDLK_HASH:
        shifted = 1;
        /* no break */
    case SDLK_3:
        keysym->scancode = 4;
        break;
    case SDLK_DOLLAR:
        shifted = 1;
        /* no break */
    case SDLK_4:
        keysym->scancode = 5;
        break;
    case SDLK_5:
        keysym->scancode = 6;
        break;
    case SDLK_CARET:
        shifted = 1;
        /* no break */
    case SDLK_6:
        keysym->scancode = 7;
        break;
    case SDLK_AMPERSAND:
        shifted = 1;
        /* no break */
    case SDLK_7:
        keysym->scancode = 8;
        break;
    case SDLK_ASTERISK:
        shifted = 1;
        /* no break */
    case SDLK_8:
        keysym->scancode = 9;
        break;
    case SDLK_LEFTPAREN:
        shifted = 1;
        /* no break */
    case SDLK_9:
        keysym->scancode = 10;
        break;
    case SDLK_RIGHTPAREN:
        shifted = 1;
        /* no break */
    case SDLK_0:
        keysym->scancode = 11;
        break;
    case SDLK_UNDERSCORE:
        shifted = 1;
        /* no break */
    case SDLK_MINUS:
        keysym->scancode = 12;
        break;
    case SDLK_PLUS:
        shifted = 1;
        /* no break */
    case SDLK_EQUALS:
        keysym->scancode = 13;
        break;
    case SDLK_BACKSPACE:
        keysym->scancode = 14;
        break;
    case SDLK_TAB:
        keysym->scancode = 15;
        break;
    case SDLK_q:
        keysym->scancode = 16;
        break;
    case SDLK_w:
        keysym->scancode = 17;
        break;
    case SDLK_e:
        keysym->scancode = 18;
        break;
    case SDLK_r:
        keysym->scancode = 19;
        break;
    case SDLK_t:
        keysym->scancode = 20;
        break;
    case SDLK_y:
        keysym->scancode = 21;
        break;
    case SDLK_u:
        keysym->scancode = 22;
        break;
    case SDLK_i:
        keysym->scancode = 23;
        break;
    case SDLK_o:
        keysym->scancode = 24;
        break;
    case SDLK_p:
        keysym->scancode = 25;
        break;
    case SDLK_LEFTBRACKET:
        keysym->scancode = 26;
        break;
    case SDLK_RIGHTBRACKET:
        keysym->scancode = 27;
        break;
    case SDLK_RETURN:
        keysym->scancode = 28;
        break;
    case SDLK_a:
        keysym->scancode = 30;
        break;
    case SDLK_s:
        keysym->scancode = 31;
        break;
    case SDLK_d:
        keysym->scancode = 32;
        break;
    case SDLK_f:
        keysym->scancode = 33;
        break;
    case SDLK_g:
        keysym->scancode = 34;
        break;
    case SDLK_h:
        keysym->scancode = 35;
        break;
    case SDLK_j:
        keysym->scancode = 36;
        break;
    case SDLK_k:
        keysym->scancode = 37;
        break;
    case SDLK_l:
        keysym->scancode = 38;
        break;
    case SDLK_COLON:
        shifted = 1;
        /* no break */
    case SDLK_SEMICOLON:
        keysym->scancode = 39;
        break;
    case SDLK_QUOTEDBL:
        shifted = 1;
        /* no break */
    case SDLK_QUOTE:
        keysym->scancode = 40;
        break;
    case SDLK_PB_TILDE:
        shifted = 1;
        /* no break */
    case SDLK_BACKQUOTE:
        keysym->scancode = 41;
        break;
    case SDLK_BACKSLASH:
        keysym->scancode = 43;
        break;
    case SDLK_z:
        keysym->scancode = 44;
        break;
    case SDLK_x:
        keysym->scancode = 45;
        break;
    case SDLK_c:
        keysym->scancode = 46;
        break;
    case SDLK_v:
        keysym->scancode = 47;
        break;
    case SDLK_b:
        keysym->scancode = 48;
        break;
    case SDLK_n:
        keysym->scancode = 49;
        break;
    case SDLK_m:
        keysym->scancode = 50;
        break;
    case SDLK_LESS:
        shifted = 1;
        /* no break */
    case SDLK_COMMA:
        keysym->scancode = 51;
        break;
    case SDLK_GREATER:
        shifted = 1;
        /* no break */
    case SDLK_PERIOD:
        keysym->scancode = 52;
        break;
    case SDLK_QUESTION:
        shifted = 1;
        /* no break */
    case SDLK_SLASH:
        keysym->scancode = 53;
        break;
    case SDLK_SPACE:
        keysym->scancode = 57;
        break;
    default:
        break;
    }

    switch (keysym->sym)
    {
    case SDLK_a:
        keysym->unicode = 'a';
        break;
    case SDLK_b:
        keysym->unicode = 'b';
        break;
    case SDLK_c:
        keysym->unicode = 'c';
        break;
    case SDLK_d:
        keysym->unicode = 'd';
        break;
    case SDLK_e:
        keysym->unicode = 'e';
        break;
    case SDLK_f:
        keysym->unicode = 'f';
        break;
    case SDLK_g:
        keysym->unicode = 'g';
        break;
    case SDLK_h:
        keysym->unicode = 'h';
        break;
    case SDLK_i:
        keysym->unicode = 'i';
        break;
    case SDLK_j:
        keysym->unicode = 'j';
        break;
    case SDLK_k:
        keysym->unicode = 'k';
        break;
    case SDLK_l:
        keysym->unicode = 'l';
        break;
    case SDLK_m:
        keysym->unicode = 'm';
        break;
    case SDLK_n:
        keysym->unicode = 'n';
        break;
    case SDLK_o:
        keysym->unicode = 'o';
        break;
    case SDLK_p:
        keysym->unicode = 'p';
        break;
    case SDLK_q:
        keysym->unicode = 'q';
        break;
    case SDLK_r:
        keysym->unicode = 'r';
        break;
    case SDLK_s:
        keysym->unicode = 's';
        break;
    case SDLK_t:
        keysym->unicode = 't';
        break;
    case SDLK_u:
        keysym->unicode = 'u';
        break;
    case SDLK_v:
        keysym->unicode = 'v';
        break;
    case SDLK_w:
        keysym->unicode = 'w';
        break;
    case SDLK_x:
        keysym->unicode = 'x';
        break;
    case SDLK_y:
        keysym->unicode = 'y';
        break;
    case SDLK_z:
        keysym->unicode = 'z';
        break;
    case SDLK_1:
        keysym->unicode = '1';
        break;
    case SDLK_2:
        keysym->unicode = '2';
        break;
    case SDLK_3:
        keysym->unicode = '3';
        break;
    case SDLK_4:
        keysym->unicode = '4';
        break;
    case SDLK_5:
        keysym->unicode = '5';
        break;
    case SDLK_6:
        keysym->unicode = '6';
        break;
    case SDLK_7:
        keysym->unicode = '7';
        break;
    case SDLK_8:
        keysym->unicode = '8';
        break;
    case SDLK_9:
        keysym->unicode = '9';
        break;
    case SDLK_0:
        keysym->unicode = '0';
        break;
    case SDLK_COMMA:
        keysym->unicode = ',';
        break;
    case SDLK_PERIOD:
        keysym->unicode = '.';
        break;
    case SDLK_COLON:
        keysym->unicode = ':';
        break;
    case SDLK_SLASH:
        keysym->unicode = '/';
        break;
    case SDLK_BACKSPACE:
        keysym->unicode = SDLK_BACKSPACE;
        break;
    case SDLK_DELETE:
        keysym->unicode = SDLK_DELETE;
        break;
    case SDLK_ESCAPE:
        keysym->unicode = SDLK_ESCAPE;
        break;
    case SDLK_RETURN:
        keysym->unicode = SDLK_RETURN;
        break;
    case SDLK_DOLLAR:
        keysym->unicode = '$';
        break;
    case SDLK_SPACE:
        keysym->unicode = ' ';
        break;
    case SDLK_MINUS:
        keysym->unicode = '-';
        break;
    case SDLK_EXCLAIM:
        keysym->unicode = '!';
        break;
    case SDLK_QUESTION:
        keysym->unicode = '?';
        break;
    case SDLK_HASH:
        keysym->unicode = '#';
        break;
    case SDLK_AT:
        keysym->unicode = '@';
        break;
    case SDLK_ASTERISK:
        keysym->unicode = '*';
        break;
    case SDLK_LEFTPAREN:
        keysym->unicode = '(';
        break;
    case SDLK_RIGHTPAREN:
        keysym->unicode = ')';
        break;
    case SDLK_EQUALS:
        keysym->unicode = '=';
        break;
    case SDLK_PLUS:
        keysym->unicode = '+';
        break;
    case SDLK_LESS:
        keysym->unicode = '<';
        break;
    case SDLK_GREATER:
        keysym->unicode = '>';
        break;
    case SDLK_SEMICOLON:
        keysym->unicode = ';';
        break;
    case SDLK_QUOTEDBL:
        keysym->unicode = '"';
        break;
    case SDLK_QUOTE:
        keysym->unicode = '\'';
        break;
    default:
        break;
    }
    keysym->mod = KMOD_NONE;
    return shifted;
}

static void handleKeyboardEvent(screen_event_t event)
{
    static const int BLUETOOTH_KEYBOARD_MASK = 0x20;
    static const int Q10_KEYBOARD_MASK = 0xe0;
    int sym = 0;
    screen_get_event_property_iv(event, SCREEN_PROPERTY_KEY_SYM, &sym);
    int modifiers = 0;
    screen_get_event_property_iv(event, SCREEN_PROPERTY_KEY_MODIFIERS, &modifiers);
    int flags = 0;
    screen_get_event_property_iv(event, SCREEN_PROPERTY_KEY_FLAGS, &flags);
    int scan = 0;
    screen_get_event_property_iv(event, SCREEN_PROPERTY_KEY_SCAN, &scan);
    int cap = 0;
    screen_get_event_property_iv(event, SCREEN_PROPERTY_KEY_CAP, &cap);

    int shifted = 0;
    SDL_keysym keysym;
    if (flags & Q10_KEYBOARD_MASK) {
      if (sym == 0xf0d3 ) {
        // Special case for the symbol key.
        if (!(flags & 0x1)) {
          if (!keyboardVisible) {
            virtualkeyboard_change_options(VIRTUALKEYBOARD_LAYOUT_SYMBOL, VIRTUALKEYBOARD_ENTER_DEFAULT);
            virtualkeyboard_show();
          } else {
            virtualkeyboard_hide();
          }
        }
        return;
      }
      shifted = TranslateVKB(sym, modifiers, flags, scan, cap, &keysym);
    } else if (flags & BLUETOOTH_KEYBOARD_MASK) {
        if (!TranslateBluetoothKeyboard(sym, modifiers, flags, scan, cap, &keysym))
        {
            return; // No translation
        }
    } else {
        shifted = TranslateVKB(sym, modifiers, flags, scan, cap, &keysym);
    }

    if (shifted) {
        SDL_keysym temp;
        temp.scancode = 42;
        temp.sym = SDLK_LSHIFT;
        SDL_PrivateKeyboard(SDL_PRESSED, &temp);
    }

    SDL_PrivateKeyboard((flags & 0x1)?SDL_PRESSED:SDL_RELEASED, &keysym);

    if (shifted) {
        SDL_keysym temp;
        temp.scancode = 42;
        temp.sym = SDLK_LSHIFT;
        SDL_PrivateKeyboard(SDL_RELEASED, &temp);
    }
}

static void handleMtouchEvent(screen_event_t event, screen_window_t window, int type)
{
    int contactId;
    int pos[2];
    int screenPos[2];
    int orientation;
    int pressure;
    long long timestamp;
    int sequenceId;

    screen_get_event_property_iv(event, SCREEN_PROPERTY_TOUCH_ID, (int*)&contactId);
    screen_get_event_property_iv(event, SCREEN_PROPERTY_SOURCE_POSITION, pos);
    screen_get_event_property_iv(event, SCREEN_PROPERTY_POSITION, screenPos);
    screen_get_event_property_iv(event, SCREEN_PROPERTY_TOUCH_ORIENTATION, (int*)&orientation);
    screen_get_event_property_iv(event, SCREEN_PROPERTY_TOUCH_PRESSURE, (int*)&pressure); // Pointless - always 1 if down
    screen_get_event_property_llv(event, SCREEN_PROPERTY_TIMESTAMP, (long long*)&timestamp);
    screen_get_event_property_iv(event, SCREEN_PROPERTY_SEQUENCE_ID, (int*)&sequenceId);

    if (type == SCREEN_EVENT_MTOUCH_TOUCH) {
        SDL_PrivateMultiMouseMotion(contactId, 0, 0, pos[0], pos[1]);
        SDL_PrivateMultiMouseButton(contactId, SDL_PRESSED, SDL_BUTTON_LEFT, pos[0], pos[1]);
    } else if (type == SCREEN_EVENT_MTOUCH_RELEASE) {
        SDL_PrivateMultiMouseMotion(contactId, SDL_BUTTON_LEFT, 0, pos[0], pos[1]);
        SDL_PrivateMultiMouseButton(contactId, SDL_RELEASED, SDL_BUTTON_LEFT, pos[0], pos[1]);
    } else if (type == SCREEN_EVENT_MTOUCH_MOVE) {
        SDL_PrivateMultiMouseMotion(contactId, SDL_BUTTON_LEFT, 0, pos[0], pos[1]);
    }
}

static void handleCustomEvent(SDL_VideoDevice *this, bps_event_t *event)
{
#if 0
	SDL_SysWMmsg wmmsg;
	SDL_VERSION(&wmmsg.version);
	wmmsg.event = event;
	SDL_PrivateSysWMEvent(&wmmsg);
#endif
}

static void handleNavigatorEvent(SDL_VideoDevice *this, bps_event_t *event)
{
    switch (bps_event_get_code(event))
    {
    case NAVIGATOR_INVOKE:
        break;
    case NAVIGATOR_EXIT:
        SDL_PrivateQuit(); // We can't stop it from closing anyway
        break;
    case NAVIGATOR_WINDOW_STATE:
        {
            navigator_window_state_t state = navigator_event_get_window_state(event);
            switch (state) {
            case NAVIGATOR_WINDOW_FULLSCREEN:
                SDL_PrivateAppActive(1, (SDL_APPACTIVE|SDL_APPINPUTFOCUS|SDL_APPMOUSEFOCUS));
                break;
            case NAVIGATOR_WINDOW_THUMBNAIL:
                SDL_PrivateAppActive(0, (SDL_APPINPUTFOCUS|SDL_APPMOUSEFOCUS));
                break;
            case NAVIGATOR_WINDOW_INVISIBLE:
                SDL_PrivateAppActive(0, (SDL_APPACTIVE|SDL_APPINPUTFOCUS|SDL_APPMOUSEFOCUS));
                break;
            }
        }
        break;
    case NAVIGATOR_SWIPE_DOWN: {
            SDL_Event event;
            SDL_UserEvent userevent;

            userevent.type = SDL_USEREVENT;
            userevent.code = 0;
            userevent.data1 = NULL;
            userevent.data2 = NULL;
            event.type = SDL_USEREVENT;
            event.user = userevent;

            SDL_PushEvent(&event);
        }
        break;
    case NAVIGATOR_SWIPE_START:
        break;
    case NAVIGATOR_LOW_MEMORY:
        break;
    case NAVIGATOR_ORIENTATION_CHECK:
        navigator_orientation_check_response(event, getenv("AUTO_ORIENTATION") != NULL ? true : false);
        break;
    case NAVIGATOR_ORIENTATION:
        break;
    case NAVIGATOR_BACK:
        break;
    case NAVIGATOR_WINDOW_ACTIVE:
        break;
    case NAVIGATOR_WINDOW_INACTIVE:
        break;
    case NAVIGATOR_ORIENTATION_DONE:
        break;
    case NAVIGATOR_ORIENTATION_RESULT:
        break;
    case NAVIGATOR_WINDOW_LOCK:
        break;
    case NAVIGATOR_WINDOW_UNLOCK:
        break;
    case NAVIGATOR_INVOKE_TARGET:
        break;
    case NAVIGATOR_INVOKE_QUERY_RESULT:
        break;
    case NAVIGATOR_INVOKE_VIEWER:
        break;
    case NAVIGATOR_INVOKE_TARGET_RESULT:
        break;
    case NAVIGATOR_INVOKE_VIEWER_RESULT:
        break;
    case NAVIGATOR_INVOKE_VIEWER_RELAY:
        break;
    case NAVIGATOR_INVOKE_VIEWER_STOPPED:
        break;
    case NAVIGATOR_KEYBOARD_STATE:
        break;
    case NAVIGATOR_KEYBOARD_POSITION:
        break;
    case NAVIGATOR_INVOKE_VIEWER_RELAY_RESULT:
        break;
    case NAVIGATOR_DEVICE_LOCK_STATE:
        break;
    case NAVIGATOR_WINDOW_COVER:
        break;
    case NAVIGATOR_WINDOW_COVER_ENTER:
        break;
    case NAVIGATOR_WINDOW_COVER_EXIT:
        break;
    case NAVIGATOR_CARD_PEEK_STARTED:
        break;
    case NAVIGATOR_CARD_PEEK_STOPPED:
        break;
    case NAVIGATOR_CARD_RESIZE:
        break;
    case NAVIGATOR_CHILD_CARD_CLOSED:
        break;
    case NAVIGATOR_CARD_CLOSED:
        break;
    case NAVIGATOR_INVOKE_GET_FILTERS_RESULT:
        break;
    case NAVIGATOR_APP_STATE:
        break;
    case NAVIGATOR_INVOKE_SET_FILTERS_RESULT:
        break;
    case NAVIGATOR_PEEK_STARTED:
        break;
    case NAVIGATOR_PEEK_STOPPED:
        break;
    case NAVIGATOR_CARD_READY_CHECK:
        break;
    case NAVIGATOR_POOLED:
        break;
    default:
        break;
    }
}

static void handleScreenEvent(SDL_VideoDevice *this, bps_event_t *event)
{
    int type;
    screen_event_t screen_event = screen_event_get_event(event);
    int rc = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE, &type);
    if (rc || type == SCREEN_EVENT_NONE)
        return;

    screen_window_t window;
    screen_get_event_property_pv(screen_event, SCREEN_PROPERTY_WINDOW, (void **)&window);
    if (!window && type != SCREEN_EVENT_KEYBOARD)
        return;

    switch (type)
    {
        case SCREEN_EVENT_CLOSE:
            SDL_PrivateQuit(); // We can't stop it from closing anyway
            break;
        case SCREEN_EVENT_PROPERTY:
            {
                int val;
                screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_NAME, &val);
            }
            break;
        case SCREEN_EVENT_POINTER:
            handlePointerEvent(screen_event, window);
            break;
        case SCREEN_EVENT_KEYBOARD:
            handleKeyboardEvent(screen_event);
            break;
        case SCREEN_EVENT_MTOUCH_TOUCH:
        case SCREEN_EVENT_MTOUCH_MOVE:
        case SCREEN_EVENT_MTOUCH_RELEASE:
            handleMtouchEvent(screen_event, window, type);
            break;
    }
}

void
PLAYBOOK_PumpEvents(SDL_VideoDevice *this)
{
    while (true)
    {
        const int event_timeout = 0;
        bps_event_t * event = NULL;
        if(BPS_SUCCESS != bps_get_event(&event, event_timeout)) {
            return;
        }
        if(!event) {
            tco_handle_events(this->hidden->tco_context,
                              this->hidden->screenWindow,
                              event);
            return;
        }

        int result = tco_handle_events(this->hidden->tco_context,
                                       this->hidden->screenWindow,
                                       event);
        switch(result) {
        case TCO_SUCCESS:
            /* TCO handled the event */
            continue;
        case TCO_FAILURE:
            /* TCO failed to handle the event */
            return;
        case TCO_UNHANDLED:
        default:
            {
                /* TCO did not handle the event */
                int domain = bps_event_get_domain(event);
                if (domain == navigator_get_domain())
                {
                    handleNavigatorEvent(this, event);
                }
                else if (domain == screen_get_domain())
                {
                    handleScreenEvent(this, event);
                }
                else if (domain == virtualkeyboard_get_domain())
                {
                    switch (bps_event_get_code(event)) {
                    case VIRTUALKEYBOARD_EVENT_VISIBLE:
                        keyboardVisible = 1;
                        break;
                    case VIRTUALKEYBOARD_EVENT_HIDDEN:
                        keyboardVisible = 0;
                        break;
                    }
                }

                // post SDL_SYSWMEVENT event containing the bps event pointer
                handleCustomEvent(this, event);
            }
            continue;
        }
    }
}

void PLAYBOOK_InitOSKeymap(SDL_VideoDevice *this)
{
    {
        // We match perfectly from 32 to 64
        int i = 32;
        for (; i<65; i++)
        {
            Playbook_Keycodes[i].sym = i;
        }
        // Capital letters
        for (; i<91; i++)
        {
            Playbook_Keycodes[i].sym = i+32;
            Playbook_Keycodes[i].mod = KMOD_LSHIFT;
        }
        // Perfect matches again from 91 to 122
        for (; i<123; i++)
        {
            Playbook_Keycodes[i].sym = i;
        }

        // Handle tilde
        Playbook_Keycodes[126].sym = SDLK_PB_TILDE;
    }

    {
        Playbook_specialsyms = (SDLKey *)malloc(256 * sizeof(SDLKey));
        Playbook_specialsyms[SDLK_BACKSPACE] = SDLK_BACKSPACE;
        Playbook_specialsyms[SDLK_TAB] = SDLK_TAB;
        Playbook_specialsyms[SDLK_RETURN] = SDLK_RETURN;
        Playbook_specialsyms[SDLK_PAUSE] = SDLK_PAUSE;
        Playbook_specialsyms[SDLK_ESCAPE] = SDLK_ESCAPE;
        Playbook_specialsyms[0xff] = SDLK_DELETE;
        Playbook_specialsyms[0x52] = SDLK_UP;
        Playbook_specialsyms[0x54] = SDLK_DOWN;
        Playbook_specialsyms[0x53] = SDLK_RIGHT;
        Playbook_specialsyms[0x51] = SDLK_LEFT;
        Playbook_specialsyms[0x63] = SDLK_INSERT;
        Playbook_specialsyms[0x50] = SDLK_HOME;
        Playbook_specialsyms[0x57] = SDLK_END;
        Playbook_specialsyms[0x55] = SDLK_PAGEUP;
        Playbook_specialsyms[0x56] = SDLK_PAGEDOWN;
        Playbook_specialsyms[0xbe] = SDLK_F1;
        Playbook_specialsyms[0xbf] = SDLK_F2;
        Playbook_specialsyms[0xc0] = SDLK_F3;
        Playbook_specialsyms[0xc1] = SDLK_F4;
        Playbook_specialsyms[0xc2] = SDLK_F5;
        Playbook_specialsyms[0xc3] = SDLK_F6;
        Playbook_specialsyms[0xc4] = SDLK_F7;
        Playbook_specialsyms[0xc5] = SDLK_F8;
        Playbook_specialsyms[0xc6] = SDLK_F9;
        Playbook_specialsyms[0xc7] = SDLK_F10;
        Playbook_specialsyms[0xc8] = SDLK_F11;
        Playbook_specialsyms[0xc9] = SDLK_F12;
        Playbook_specialsyms[0xe5] = SDLK_CAPSLOCK;
        Playbook_specialsyms[0x14] = SDLK_SCROLLOCK;
        Playbook_specialsyms[0xe2] = SDLK_RSHIFT;
        Playbook_specialsyms[0xe1] = SDLK_LSHIFT;
        Playbook_specialsyms[0xe4] = SDLK_RCTRL;
        Playbook_specialsyms[0xe3] = SDLK_LCTRL;
        Playbook_specialsyms[0xe8] = SDLK_RALT;
        Playbook_specialsyms[0xe9] = SDLK_LALT;
        Playbook_specialsyms[0xbe] = SDLK_MENU;
        Playbook_specialsyms[0x61] = SDLK_SYSREQ;
        Playbook_specialsyms[0x6b] = SDLK_BREAK;
    }
}
