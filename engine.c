#include "engine.h"

#include <SDL2/SDL.h>

int engine_quit;
struct Timer sys_timer, update_timer, draw_timer;

void Timer_tick(struct Timer* timer) {
    long curr_ticks = SDL_GetPerformanceCounter();
    long delta = curr_ticks - timer->pre_tick;
    timer->pre_tick = curr_ticks;
    long ticks_per_sec = SDL_GetPerformanceFrequency();
    timer->elapsed_sec = (float)delta / (float)ticks_per_sec;
}

void Engine_quit() {
    engine_quit = 1;
}

void Engine_loop(float fps, void (*update_loop)(float), void (*draw_loop)(float)) {
    float accumulated_seconds = 0.0f;
    float cycle_time = 1.0f / fps;
    engine_quit = 0;

    Timer_tick(&sys_timer);
    while (!engine_quit) {
        Timer_tick(&sys_timer);
        accumulated_seconds += sys_timer.elapsed_sec;

        //\ Update Loop
        if (accumulated_seconds > cycle_time) {
            accumulated_seconds -= cycle_time;
            Timer_tick(&update_timer);
            update_loop(update_timer.elapsed_sec);
        }

        //\ Draw Loop
        Timer_tick(&draw_timer);
        draw_loop(draw_timer.elapsed_sec);
    }
}