#include <SDL3/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>

void gb_window_init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        printf("ERROR::GRAPHICS::GB_WINDOW_INIT::FAILED_TO_INIT_SDL");
    }
}

gb_window* gb_window_create(unsigned int width, unsigned int height, const char* title) {
    gb_window* window = malloc(sizeof(gb_window));
    window->window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL);
    if (window->window == NULL) {
        printf("ERROR::GRAPHICS::GB_WINDOW_CREATE::WINDOW_NULL\n");
        return NULL;
    }
    window->renderer = SDL_CreateRenderer(window->window, NULL);
    SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 255);
    SDL_RenderClear(window->renderer);
    SDL_RenderPresent(window->renderer);
    return window;
}

void gb_window_delete(gb_window** window) {
    SDL_DestroyWindow((*window)->window);
    SDL_Quit();
    free(*window);
}
