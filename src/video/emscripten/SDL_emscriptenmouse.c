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

#include "SDL_emscriptenmouse.h"

#include "../../events/SDL_mouse_c.h"
#include "SDL_assert.h"


static SDL_Cursor*
Emscripten_CreateDefaultCursor()
{
    SDL_Cursor* cursor;

    cursor = SDL_calloc(1, sizeof(SDL_Cursor));
    if (cursor) {
    }
    else {
        SDL_OutOfMemory();
    }

    return cursor;
}

static SDL_Cursor*
Emscripten_CreateCursor(SDL_Surface* sruface, int hot_x, int hot_y)
{
    return Emscripten_CreateDefaultCursor();
}

static SDL_Cursor*
Emscripten_CreateSystemCursor(SDL_SystemCursor id)
{
    switch(id) {
        case SDL_SYSTEM_CURSOR_ARROW:
            break;
        case SDL_SYSTEM_CURSOR_IBEAM:
            break;
        case SDL_SYSTEM_CURSOR_WAIT:
            break;
        case SDL_SYSTEM_CURSOR_CROSSHAIR:
            break;
        case SDL_SYSTEM_CURSOR_WAITARROW:
            break;
        case SDL_SYSTEM_CURSOR_SIZENWSE:
            break;
        case SDL_SYSTEM_CURSOR_SIZENESW:
            break;
        case SDL_SYSTEM_CURSOR_SIZEWE:
            break;
        case SDL_SYSTEM_CURSOR_SIZENS:
            break;
        case SDL_SYSTEM_CURSOR_SIZEALL:
            break;
        case SDL_SYSTEM_CURSOR_NO:
            break;
        case SDL_SYSTEM_CURSOR_HAND:
            break;
        default:
            SDL_assert(0);
            return NULL;
    }

    return Emscripten_CreateDefaultCursor();
}

static void
Emscripten_FreeCursor(SDL_Cursor* cursor)
{
    if (cursor)
      SDL_free(cursor);
}

static int
Emscripten_ShowCursor(SDL_Cursor* cursor)
{
    return 0;
}

static void
Emscripten_WarpMouse(SDL_Window* window, int x, int y)
{
    SDL_Unsupported();
}

static int
Emscripten_SetRelativeMouseMode(SDL_bool enabled)
{
    return SDL_Unsupported();
}

void
Emscripten_InitMouse()
{
    SDL_Mouse* mouse = SDL_GetMouse();

    mouse->CreateCursor         = Emscripten_CreateCursor;
    mouse->ShowCursor           = Emscripten_ShowCursor;
    mouse->FreeCursor           = Emscripten_FreeCursor;
    mouse->WarpMouse            = Emscripten_WarpMouse;
    mouse->CreateSystemCursor   = Emscripten_CreateSystemCursor;
    mouse->SetRelativeMouseMode = Emscripten_SetRelativeMouseMode;

    SDL_SetDefaultCursor(Emscripten_CreateDefaultCursor());
}

void
Emscripten_FiniMouse()
{
    SDL_Mouse* mouse = SDL_GetMouse();

    Emscripten_FreeCursor(mouse->def_cursor);
    mouse->def_cursor = NULL;

    mouse->CreateCursor         = NULL;
    mouse->ShowCursor           = NULL;
    mouse->FreeCursor           = NULL;
    mouse->WarpMouse            = NULL;
    mouse->CreateSystemCursor   = NULL;
    mouse->SetRelativeMouseMode = NULL;
}

#endif /* SDL_VIDEO_DRIVER_EMSCRIPTEN */

/* vi: set ts=4 sw=4 expandtab: */

