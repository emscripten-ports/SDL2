/*
  Copyright (C) 1997-2016 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/

#include "testnative.h"

#include <emscripten/emscripten.h>

#ifdef TEST_NATIVE_EMSCRIPTEN

static void *CreateWindowEmscripten(int w, int h);
static void DestroyWindowEmscripten(void *window);

NativeWindowFactory EmscriptenWindowFactory = {
    "emscripten",
    CreateWindowEmscripten,
    DestroyWindowEmscripten
};


static void *
CreateWindowEmscripten(int w, int h)
{
    EM_ASM_ARGS({
        var canvas = document.createElement("canvas");
        canvas.width = $0;
        canvas.height = $1;
        canvas.id = "test_canvas";
        document.body.appendChild(canvas);
    }, w, h);

    return (void *) "test_canvas";
}

static void
DestroyWindowEmscripten(void *window)
{
}

#endif
