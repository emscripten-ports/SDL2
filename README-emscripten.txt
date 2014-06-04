Build:
$ emconfigure ./configure --host=asmjs-unknown-emscripten --disable-assembly --disable-threads CFLAGS="-O2"
$ emmake make

To build one of the tests -
$ cd test/
$ emcc -O2 --js-opts 0 -g4 testdraw2.c -I../include ../build/.libs/libSDL2.a ../build/libSDL2_test.a -o a.html

Uses GLES2 renderer or software

tests: https://dl.dropboxusercontent.com/u/17360362/SDL2-em/index.html
