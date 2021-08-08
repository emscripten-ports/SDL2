#include "SDL_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <emscripten/emscripten.h>
#include "SDL.h"
static struct
{
    SDL_AudioSpec spec;
    Uint8 *sound;               /* Pointer to wave data */
    Uint32 soundlen;            /* Length of wave data */
    int soundpos;               /* Current play position */
} wave;
static SDL_AudioDeviceID device;
static void
quit(int rc)
{
    SDL_Quit();
    exit(rc);
}
static void
close_audio()
{
    if (device != 0) {
        SDL_CloseAudioDevice(device);
        device = 0;
    }
}
static void
open_audio()
{
    /* Initialize fillerup() variables */
    device = SDL_OpenAudioDevice(NULL, SDL_FALSE, &wave.spec, NULL, 0);
    if (!device) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't open audio: %s\n", SDL_GetError());
        SDL_FreeWAV(wave.sound);
        quit(2);
    }
    SDL_PauseAudioDevice(device, SDL_FALSE);
}
static void reopen_audio()
{
    close_audio();
    open_audio();
}
void SDLCALL
fillerup(void *unused, Uint8 * stream, int len)
{
    Uint8 *waveptr;
    int waveleft;
    waveptr = wave.sound + wave.soundpos;
    waveleft = wave.soundlen - wave.soundpos;
    while (waveleft <= len) {
        SDL_memcpy(stream, waveptr, waveleft);
        stream += waveleft;
        len -= waveleft;
        waveptr = wave.sound;
        waveleft = wave.soundlen;
        wave.soundpos = 0;
    }
    SDL_memcpy(stream, waveptr, len);
    wave.soundpos += len;
}
static int done = 0;
void
loop()
{
    if(done || (SDL_GetAudioDeviceStatus(device) != SDL_AUDIO_PLAYING))
        emscripten_cancel_main_loop();
}
int
main(int argc, char *argv[])
{
    
EM_ASM(
const load=() => {
const request=new XMLHttpRequest();
request.open("GET","./sample.wav");
request.responseType="arraybuffer";
request.onload=function(){
let undecodedAudio=request.response;
};
request.send();
}
const uint8_view=new Uint8Array(undecodedAudio);
FS.writeFile('/sample.wav',uint8_view);
        
     );
    int i;
    char filename[4096];
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
    if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_EVENTS) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return (1);
    }
    if (argc > 1) {
        SDL_strlcpy(filename, argv[1], sizeof(filename));
    } else {
        SDL_strlcpy(filename, "sample.wav", sizeof(filename));
    }
    if (SDL_LoadWAV(filename, &wave.spec, &wave.sound, &wave.soundlen) == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load %s: %s\n", filename, SDL_GetError());
        quit(1);
    }
    wave.spec.callback = fillerup;
    SDL_Log("Available audio drivers:");
    for (i = 0; i < SDL_GetNumAudioDrivers(); ++i) {
        SDL_Log("%i: %s", i, SDL_GetAudioDriver(i));
    }
    SDL_Log("Using audio driver: %s\n", SDL_GetCurrentAudioDriver());
    open_audio();
    SDL_FlushEvents(SDL_AUDIODEVICEADDED, SDL_AUDIODEVICEREMOVED);
    emscripten_set_main_loop(loop, 0, 1);
    close_audio();
    SDL_FreeWAV(wave.sound);
    SDL_Quit();
    return (0);
}
