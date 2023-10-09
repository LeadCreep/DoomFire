#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>

struct Timer {
    long pre_tick;
    float elapsed_sec;
};
void Timer_tick(struct Timer *timer);

void Engine_quit();
void Engine_loop(float fps, void (*update_loop)(float), void (*draw_loop)(float));

#endif