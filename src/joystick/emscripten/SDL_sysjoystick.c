/*
max_  Simple DirectMedia Layer
  Copyright (C) 1997-2014 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "../../SDL_internal.h"

#ifdef SDL_JOYSTICK_EMSCRIPTEN

#include <stdio.h>              /* For the definition of NULL */
#include "SDL_error.h"
#include "SDL_events.h"

#if !SDL_EVENTS_DISABLED
#include "../../events/SDL_events_c.h"
#endif

#include "SDL_joystick.h"
#include "SDL_hints.h"
#include "SDL_assert.h"
#include "SDL_timer.h"
#include "SDL_log.h"
#include "SDL_sysjoystick_c.h"
#include "../SDL_joystick_c.h"

static SDL_joylist_item * JoystickByDeviceId(int index);

static SDL_joylist_item *SDL_joylist = NULL;
static SDL_joylist_item *SDL_joylist_tail = NULL;
static int numjoysticks = 0;
static int instance_counter = 0;

static int
keycode_to_SDL(int keycode)
{
    /* see http://www.w3.org/TR/gamepad/#remapping for more info */
    int button = 0;
    switch(keycode)
    {
        /* Some gamepad buttons (API 9) */
        case EMSCRIPTEN_CONTROLLER_BUTTON_A:
            button = SDL_CONTROLLER_BUTTON_A;
            break;
        case EMSCRIPTEN_CONTROLLER_BUTTON_B:
            button = SDL_CONTROLLER_BUTTON_B;
            break;
        case EMSCRIPTEN_CONTROLLER_BUTTON_X:
            button = SDL_CONTROLLER_BUTTON_X;
            break;
        case EMSCRIPTEN_CONTROLLER_BUTTON_Y:
            button = SDL_CONTROLLER_BUTTON_Y;
            break;
        case EMSCRIPTEN_CONTROLLER_BUTTON_L1:
            button = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
            break;
        case EMSCRIPTEN_CONTROLLER_BUTTON_R1:
            button = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
            break;
        case EMSCRIPTEN_CONTROLLER_BUTTON_LEFTSTICK:
            button = SDL_CONTROLLER_BUTTON_LEFTSTICK;
            break;
        case EMSCRIPTEN_CONTROLLER_BUTTON_RIGHTSTICK:
            button = SDL_CONTROLLER_BUTTON_RIGHTSTICK;
            break;
        case EMSCRIPTEN_CONTROLLER_BUTTON_BACK:
            button = SDL_CONTROLLER_BUTTON_BACK;
            break;
        case EMSCRIPTEN_CONTROLLER_BUTTON_START:
            button = SDL_CONTROLLER_BUTTON_START;
            break;
        case EMSCRIPTEN_CONTROLLER_BUTTON_GUIDE:
            button = SDL_CONTROLLER_BUTTON_GUIDE;
            break;
        case EMSCRIPTEN_CONTROLLER_BUTTON_L2:
            button = SDL_CONTROLLER_BUTTON_MAX; /* Not supported by GameController */
            break;
        case EMSCRIPTEN_CONTROLLER_BUTTON_R2:
            button = SDL_CONTROLLER_BUTTON_MAX+1; /* Not supported by GameController */
            break;

        case EMSCRIPTEN_CONTROLLER_DPAD_UP:
            button = SDL_CONTROLLER_BUTTON_DPAD_UP;
            break;
        case EMSCRIPTEN_CONTROLLER_DPAD_DOWN:
            button = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
            break;
        case EMSCRIPTEN_CONTROLLER_DPAD_LEFT:
            button = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
            break;
        case EMSCRIPTEN_CONTROLLER_DPAD_RIGHT:
            button = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
            break;

        default:
            return -1;
            break;
    }

    SDL_assert(button < EMSCRIPTEN_MAX_NBUTTONS);
    return button;
}

int
Emscripten_JoyStickConnected(int eventType, const EmscriptenGamepadEvent *gamepadEvent, void *userData)
{
    /* why are we even doing this? */
    /* this callback wouldn't even fire unless this was true */
    if (eventType != EMSCRIPTEN_EVENT_GAMEPADCONNECTED)
    {
        return -1;
    }

    SDL_joylist_item *item;

#if !SDL_EVENTS_DISABLED
    SDL_Event event;
#endif

    item = (SDL_joylist_item *) SDL_malloc(sizeof (SDL_joylist_item));
    if (item == NULL) {
        return -1;
    }

    SDL_zerop(item);
    item->index = gamepadEvent->index;

    item->name = SDL_strdup(gamepadEvent->id);
    if ( item->name == NULL ) {
         SDL_free(item);
         return -1;
    }
    item->mapping = SDL_strdup(gamepadEvent->mapping);
    if ( item->mapping == NULL ) {
         SDL_free(item);
         return -1;
    }
    item->naxes = gamepadEvent->numAxes;
    item->nbuttons = gamepadEvent->numButtons;

    if (SDL_joylist_tail == NULL) {
        SDL_joylist = SDL_joylist_tail = item;
    } else {
        SDL_joylist_tail->next = item;
        SDL_joylist_tail = item;
    }

    /* Need to increment the joystick count before we post the event */
    ++numjoysticks;

#if !SDL_EVENTS_DISABLED
    event.type = SDL_JOYDEVICEADDED;

    if (SDL_GetEventState(event.type) == SDL_ENABLE) {
        event.jdevice.which = (numjoysticks - 1);
        if ( (SDL_EventOK == NULL) ||
             (*SDL_EventOK) (SDL_EventOKParam, &event) ) {
            SDL_PushEvent(&event);
        }
    }
#endif /* !SDL_EVENTS_DISABLED */

    SDL_Log("Added joystick with index %d", index);

    return numjoysticks;
}

int
Emscripten_JoyStickDisconnected(int eventType, const EmscriptenGamepadEvent *gamepadEvent, void *userData)
{
    /* why are we even doing this? */
    /* this callback wouldn't even fire unless this was true */
    if (eventType != EMSCRIPTEN_EVENT_GAMEPADDISCONNECTED)
    {
        return -1;
    }

    SDL_joylist_item *item = SDL_joylist;
    SDL_joylist_item *prev = NULL;
#if !SDL_EVENTS_DISABLED
    SDL_Event event;
#endif

    while (item != NULL) {
        if (item->index == index) {
            break;
        }
        prev = item;
        item = item->next;
    }

    if (item == NULL) {
        return -1;
    }

    const int retval = item->index;
    if (item->joystick) {
        item->joystick->hwdata = NULL;
    }

    if (prev != NULL) {
        prev->next = item->next;
    } else {
        SDL_assert(SDL_joylist == item);
        SDL_joylist = item->next;
    }
    if (item == SDL_joylist_tail) {
        SDL_joylist_tail = prev;
    }

    /* Need to decrement the joystick count before we post the event */
    --numjoysticks;

#if !SDL_EVENTS_DISABLED
    event.type = SDL_JOYDEVICEREMOVED;

    if (SDL_GetEventState(event.type) == SDL_ENABLE) {
        event.jdevice.which = item->device_instance;
        if ( (SDL_EventOK == NULL) ||
             (*SDL_EventOK) (SDL_EventOKParam, &event) ) {
            SDL_PushEvent(&event);
        }
    }
#endif /* !SDL_EVENTS_DISABLED */

    SDL_Log("Removed joystick with index %d", retval);
    SDL_free(item->name);
    SDL_free(item->mapping);
    SDL_free(item);
    return retval;
}

/* Function to scan the system for joysticks.
 * It should return 0, or -1 on an unrecoverable fatal error.
 */
int
SDL_SYS_JoystickInit(void)
{
    int retval;
    numjoysticks = emscripten_get_num_gamepads();

    // Check if gamepad is supported by browser
    if (numjoysticks == -1) {
        return -1;
    }

    retval = emscripten_set_gamepadconnected_callback(NULL,
                                                      0,
                                                      Emscripten_JoyStickConnected);

    if(retval == -1) {
        return -1;
    }

    retval = emscripten_set_gamepaddisconnected_callback(NULL,
                                                         0,
                                                         Emscripten_JoyStickDisconnected);
    if(retval == -1) {
        return -1;
    }

    return 0;
}

static SDL_joylist_item *
JoystickByIndex(int index)
{
    SDL_joylist_item *item = SDL_joylist;

    if (index < 0) {
        return NULL;
    }

    while (item != NULL) {
        if (item->index == index) {
            break;
        }
        prev = item;
        item = item->next;
    }

    return item;
}

int SDL_SYS_NumJoysticks()
{
    return numjoysticks;
}

void SDL_SYS_JoystickDetect()
{
}

// we need to poll to see if the gamepad state has changed
SDL_bool SDL_SYS_JoystickNeedsPolling()
{
    return SDL_TRUE;
}

/* Function to get the device-dependent name of a joystick */
const char *
SDL_SYS_JoystickNameForDeviceIndex(int index)
{
    SDL_joylist_item *item = JoystickByIndex(index);
    if (item == NULL) {
        SDL_SetError("Joystick with index %d not found", index);
        return NULL;
    }

    return item->name;
}

/* Function to perform the mapping from device index to the instance id for this index */
SDL_JoystickID SDL_SYS_GetInstanceIdOfDeviceIndex(int device_index)
{
    return device_index;
}

/* Function to open a joystick for use.
   The joystick to open is specified by the index field of the joystick.
   This should fill the nbuttons and naxes fields of the joystick structure.
   It returns 0, or -1 if there is an error.
 */
int
SDL_SYS_JoystickOpen(SDL_Joystick * joystick, int device_index)
{
    return SDL_SetError("Logic error: No joysticks available");
}

/* Function to determine is this joystick is attached to the system right now */
SDL_bool SDL_SYS_JoystickAttached(SDL_Joystick *joystick)
{
    return SDL_TRUE;
}

/* Function to update the state of a joystick - called as a device poll.
 * This function shouldn't update the joystick structure directly,
 * but instead should call SDL_PrivateJoystick*() to deliver events
 * and update joystick device state.
 */
void
SDL_SYS_JoystickUpdate(SDL_Joystick * joystick)
{
    return;
}

/* Function to close a joystick after use */
void
SDL_SYS_JoystickClose(SDL_Joystick * joystick)
{
    return;
}

/* Function to perform any system-specific joystick related cleanup */
void
SDL_SYS_JoystickQuit(void)
{
    return;
}

SDL_JoystickGUID SDL_SYS_JoystickGetDeviceGUID( int device_index )
{
    SDL_JoystickGUID guid;
    /* the GUID is just the first 16 chars of the name for now */
    const char *name = SDL_SYS_JoystickNameForDeviceIndex( device_index );
    SDL_zero( guid );
    SDL_memcpy( &guid, name, SDL_min( sizeof(guid), SDL_strlen( name ) ) );
    return guid;
}


SDL_JoystickGUID SDL_SYS_JoystickGetGUID(SDL_Joystick * joystick)
{
    SDL_JoystickGUID guid;
    /* the GUID is just the first 16 chars of the name for now */
    const char *name = joystick->name;
    SDL_zero( guid );
    SDL_memcpy( &guid, name, SDL_min( sizeof(guid), SDL_strlen( name ) ) );
    return guid;
}

#endif /* SDL_JOYSTICK_EMSCRIPTEN */
