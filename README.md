source ~/emsdk/emsdk_env.sh
^ Or wherever you have it.

git clone https://github.com/ford442/SDL2.git -b optimize
cd SDL2
cd test
emcc loopwave.c -o sdl2-wav.js -s ENVIRONMENT=web -s INITIAL_MEMORY=1400mb -s ALLOW_MEMORY_GROWTH=0 -s MALLOC="emmalloc" -s FORCE_FILESYSTEM=1  -O3 -s USE_SDL=2 -s EXPORTED_FUNCTIONS='["_pl"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall"]' --closure 1 -g2 -flto -s SUPPORT_LONGJMP=0



                         Simple DirectMedia Layer

                                  (SDL)

                                Version 2.0

---
https://www.libsdl.org/

Simple DirectMedia Layer is a cross-platform development library designed
to provide low level access to audio, keyboard, mouse, joystick, and graphics
hardware via OpenGL and Direct3D. It is used by video playback software,
emulators, and popular games including Valve's award winning catalog
and many Humble Bundle games.

More extensive documentation is available in the docs directory, starting
with README.md

Enjoy!
	Sam Lantinga				(slouken@libsdl.org)
