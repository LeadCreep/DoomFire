#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "engine.h"
#define FPS 5.F

SDL_Window* window;
SDL_Renderer* renderer;

#define FIRE_HEIGHT 168
#define FIRE_WIDTH 300
#define NUM_COLORS 37
#define PIXEL_W 6
#define PIXEL_H 6

unsigned char rgb_pallete[NUM_COLORS][3] = {
    {0x07, 0x07, 0x07},  // Noir
    {0x1F, 0x07, 0x07},
    {0x2F, 0x0F, 0x07},
    {0x47, 0x0F, 0x07},
    {0x57, 0x17, 0x07},
    {0x67, 0x1F, 0x07},
    {0x77, 0x1F, 0x07},
    {0x8F, 0x27, 0x07},
    {0x9F, 0x2F, 0x07},
    {0xAF, 0x3F, 0x07},
    {0xBF, 0x47, 0x07},
    {0xC7, 0x47, 0x07},
    {0xDF, 0x4F, 0x07},
    {0xDF, 0x57, 0x07},
    {0xDF, 0x57, 0x07},
    {0xD7, 0x5F, 0x07},
    {0xD7, 0x5F, 0x07},
    {0xD7, 0x67, 0x0F},
    {0xCF, 0x6F, 0x0F},
    {0xCF, 0x77, 0x0F},
    {0xCF, 0x7F, 0x0F},
    {0xCF, 0x87, 0x17},
    {0xC7, 0x87, 0x17},
    {0xC7, 0x8F, 0x17},
    {0xC7, 0x97, 0x1F},
    {0xBF, 0x9F, 0x1F},
    {0xBF, 0x9F, 0x1F},
    {0xBF, 0xA7, 0x27},
    {0xBF, 0xA7, 0x27},
    {0xBF, 0xAF, 0x2F},
    {0xB7, 0xAF, 0x2F},
    {0xB7, 0xB7, 0x2F},
    {0xB7, 0xB7, 0x37},
    {0xCF, 0xCF, 0x6F},
    {0xDF, 0xDF, 0x9F},
    {0xEF, 0xEF, 0xC7},
    {0xFF, 0xFF, 0xFF}  // Blanc
};
int fire_pixels[FIRE_HEIGHT * FIRE_WIDTH];

void start_fire() {
    for (int c = 0; c < FIRE_WIDTH; c++) {
        fire_pixels[(FIRE_HEIGHT - 1) * FIRE_WIDTH + c] = NUM_COLORS - 1;
    }
}

void stop_fire() {
    for (int c = 0; c < FIRE_WIDTH; c++) {
        fire_pixels[(FIRE_HEIGHT - 1) * FIRE_WIDTH + c] = 0;
    }
}

void dim_fire() {
    if (fire_pixels[(FIRE_HEIGHT - 1) * FIRE_WIDTH] == 0) {
        return;
    }
    for (int c = 0; c < FIRE_WIDTH; c++) {
        fire_pixels[(FIRE_HEIGHT - 1) * FIRE_WIDTH + c] = fire_pixels[(FIRE_HEIGHT - 1) * FIRE_WIDTH + c] - 1;
    }
}

void light_fire() {
    if (fire_pixels[(FIRE_HEIGHT - 1) * FIRE_WIDTH] >= NUM_COLORS - 2) {
        return;
    }
    for (int c = 0; c < FIRE_WIDTH; c++) {
        fire_pixels[(FIRE_HEIGHT - 1) * FIRE_WIDTH + c] = fire_pixels[(FIRE_HEIGHT - 1) * FIRE_WIDTH + c] + 1;
    }
}

void setup() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    window = SDL_CreateWindow("DOOM FIRE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, FIRE_WIDTH * PIXEL_W, FIRE_HEIGHT * PIXEL_H, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    srand(time(NULL));

    // Ecran Noir
    for (int r = 0; r < FIRE_HEIGHT; r++) {
        for (int c = 0; c < FIRE_WIDTH; c++) {
            fire_pixels[r * FIRE_WIDTH + c] = 0;
        }
    }

    start_fire();
}

void teardown() {
    SDL_Quit();
}

void poll_events() {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT || (ev.type == SDL_KEYUP && ev.key.keysym.sym == SDLK_ESCAPE)) {
            Engine_quit();
        }
        if (ev.type == SDL_KEYUP) {
            switch (ev.key.keysym.sym) {
                case SDLK_a:
                    start_fire();
                    break;
                case SDLK_e:
                    stop_fire();
                    break;
                case SDLK_d:
                    dim_fire();
                    break;
                case SDLK_q:
                    light_fire();
                    break;
                default:
                    break;
            }
        }
    }
}

void spread_fire(int from) {
    if (fire_pixels[from] == 0) {
        fire_pixels[from - FIRE_WIDTH] = 0;
    } else {
        int rando = rand() % 3;
        int randx = rand() % 3;
        int to = from - randx + 1;
        fire_pixels[to - FIRE_WIDTH] = fire_pixels[from] - (rando & 1);
    }
}

void update_loop(float ds) {
    poll_events();

    // Spread Fire
    for (int r = 1; r < FIRE_HEIGHT; r++) {  // Ne pas faire la première ligne
        for (int c = 0; c < FIRE_WIDTH; c++) {
            spread_fire(r * FIRE_WIDTH + c);
        }
    }
}

void draw_loop(float ds) {
    SDL_SetRenderDrawColor(renderer, 50, 20, 20, 255);
    SDL_RenderClear(renderer);

    for (int r = 0; r < FIRE_HEIGHT; r++) {
        for (int c = 0; c < FIRE_WIDTH; c++) {
            int index = fire_pixels[r * FIRE_WIDTH + c];
            unsigned char* rgb = rgb_pallete[index];
            SDL_SetRenderDrawColor(renderer, rgb[0], rgb[1], rgb[2], 255);
            SDL_Rect dest = {c * PIXEL_W, r * PIXEL_H, PIXEL_W, PIXEL_H};
            SDL_RenderFillRect(renderer, &dest);
        }
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char** argv[]) {
    setup();
    Engine_loop(FPS, update_loop, draw_loop);
    teardown();
    return EXIT_SUCCESS;
}