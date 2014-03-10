/*
  Simple DirectMedia Layer
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

#if SDL_VIDEO_DRIVER_EMSCRIPTEN

#include <emscripten/html5.h>

#include "../../events/SDL_events_c.h"
#include "../../events/SDL_keyboard_c.h"
#include "../../events/SDL_touch_c.h"

#include "SDL_emscriptenevents.h"
#include "SDL_emscriptenvideo.h"

int Emscripten_HandleMouseMove(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    SDL_WindowData *window_data = userData;
    SDL_SendMouseMotion(window_data->window, 0, 0, mouseEvent->canvasX, mouseEvent->canvasY);
    return 0;
}

int Emscripten_HandleMouseButton(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    SDL_WindowData *window_data = userData;
    uint32_t sdl_button;
    switch (mouseEvent->button) {
        case 0:
            sdl_button = SDL_BUTTON_LEFT;
            break;
        case 1:
            sdl_button = SDL_BUTTON_MIDDLE;
            break;
        case 2:
            sdl_button = SDL_BUTTON_RIGHT;
            break;
        default:
            return 0;
    }
    SDL_SendMouseButton(window_data->window, 0, eventType == EMSCRIPTEN_EVENT_MOUSEDOWN ? SDL_PRESSED : SDL_RELEASED, sdl_button);
    return 1;
}

void
Emscripten_RegisterEventHandlers(SDL_WindowData *data)
{
    /* There is only one window and that window is the canvas */
    emscripten_set_mousemove_callback("#canvas", data, 0, Emscripten_HandleMouseMove);

    emscripten_set_mousedown_callback("#canvas", data, 0, Emscripten_HandleMouseButton);
    emscripten_set_mouseup_callback("#canvas", data, 0, Emscripten_HandleMouseButton);
}

#endif /* SDL_VIDEO_DRIVER_EMSCRIPTEN */

/* vi: set ts=4 sw=4 expandtab: */
