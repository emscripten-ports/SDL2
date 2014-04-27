Build:
emconfigure ./configure --host=asmjs-unknown-emscripten --disable-assembly --disable-threads CFLAGS="-O2"

Uses GLES2 renderer (-s FULL_ES2=1 required) or software

tests: https://dl.dropboxusercontent.com/u/17360362/SDL2-em/index.html
