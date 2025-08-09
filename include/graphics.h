#ifndef _H_GRAPHICS
#define _H_GRAPHICS
#include <SDL3/SDL.h>

typedef struct {
    unsigned int width;
    unsigned int height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    const char* title;
} gb_window;

void gb_window_init();

gb_window* gb_window_create(unsigned int width, unsigned int height, const char* title);
void gb_window_delete(gb_window** window);
#endif
