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

#ifdef SDL_FILESYSTEM_EMSCRIPTEN

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* System dependent filesystem routines                                */

#include "SDL_error.h"
#include "SDL_filesystem.h"

#include <emscripten/emscripten.h>

char *
SDL_GetBasePath(void)
{
  char *retval = "/";
  return retval;
}

char *
SDL_GetPrefPath(const char *org, const char *app)
{
  char *retval = "/libsdl/";
  
  int success = EM_ASM_INT_V({
    try {
      FS.mkdir('/libsdl/');
    } catch(e) {
      if (e.errno === ERRNO_CODES.EEXIST) {
        return 0;
      } else {
        return 1;
      }
    }

    return 0;
  });

  if (success != 0) {
    SDL_SetError("Couldn't create directory '%s'", retval);
    SDL_free(retval);
    return NULL;
  }

  return retval;
}

#endif /* SDL_FILESYSTEM_EMSCRIPTEN */

/* vi: set ts=4 sw=4 expandtab: */
