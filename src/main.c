#include "cpu_register.h"
#include "gb.h"
#include "graphics.h"
#include <SDL2/SDL_timer.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_scancode.h>
#include <stdio.h>
#include <memory.h>
#include <cpu.h>
#include <string.h>
#include <sys/types.h>

int main() {
    gb_window_init();
    gb_window* window = gb_window_create(400, 200, "hello");
    gb* console = gb_create();
    gb_cpu_register_dump(console->cpu->cpu_register);
    gb_bios_load(console, "gb_bios.bin");
    
    int _WINDOW_CLOSE = 0;

    int running_status = 1;
    char cmd[16];
    while (_WINDOW_CLOSE == 0) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                _WINDOW_CLOSE = 1;
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.scancode == SDL_SCANCODE_SPACE) {
                    if (running_status == 0) {
                        running_status = 1;
                    }
                }
            }
        }

        while (running_status == 1) {
            char line[64];
            printf("Waiting for instruction...\n>");
            if (fgets(line, sizeof(line), stdin) == NULL) {
                break;
            }
            if (sscanf(line, "%s", cmd) != 1) {
                continue;
            }

            if (strcmp(cmd, "reg_dump") == 0) {
                gb_cpu_register_dump(console->cpu->cpu_register);
            } else if(strcmp(cmd, "r") == 0 || strcmp(cmd, "run") == 0) {
                running_status = 0;
            } else if(strcmp(cmd, "q") == 0 || strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0) {
                return 0;
            } else if (strcmp(cmd, "c") == 0 || strcmp(cmd, "continue") == 0) {
                break;
            } else if (strcmp(cmd, "mem_dump") == 0) {
                printf("Memdump: Enter Start Adress and count\n");
                uint16_t start_address = 0;
                unsigned int count = 0;
                if (fgets(line, sizeof(line), stdin) == NULL)
                    continue;
                if (sscanf(line, "0x%04X %d", &start_address, &count) != 2) {
                    printf("Memdump: Usage [0xXXXX] [X] where X is any hexadecimal character!");
                    continue;
                }
                printf("Memdump: Started...\n");
                gb_memory_dump(console->memory, start_address, count);
                printf("Memdump: Ended...\n");
            }
        }
        gb_cpu_execute(console->cpu, console->memory);
        

        SDL_Delay(17);
    }
    gb_delete(&console);
    gb_window_delete(&window);
    return 0;
}
