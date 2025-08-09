#include "cpu_register.h"
#include "gb.h"
#include "graphics.h"
#include <SDL2/SDL_timer.h>
#include <SDL3/SDL_events.h>
#include <stdio.h>
#include <memory.h>
#include <cpu.h>

int main() {
    gb_window_init();
    gb_window* window = gb_window_create(400, 200, "hello");
    gb* console = gb_create();
    gb_cpu_register_dump(console->cpu->cpu_register);
    gb_bios_load(console, "gba_bios.bin");
    
    int _WINDOW_CLOSE = 0;
    while (_WINDOW_CLOSE == 0) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                _WINDOW_CLOSE = 1;
            }
        }

        gb_cpu_execute(console->cpu, console->memory);
        

        SDL_Delay(17);
    }
    gb_delete(&console);
    gb_window_delete(&window);
    return 0;
}
