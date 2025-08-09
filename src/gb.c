#include "cpu.h"
#include "memory.h"
#include <gb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

gb* gb_create() {
    gb* console = malloc(sizeof(gb));
    memset(console, 0, sizeof(gb));
    console->cpu = gb_cpu_create();
    console->memory = gb_memory_create();
    return console;
}

void gb_bios_load(gb* console,const char *path) {
    _gb_load(console, path, 256, 0x0);
    console->cpu->cpu_register->pc = 0;
}

void gb_rom_load(gb* console, const char* path) {
    
}

void gb_rom_unload();

void gb_delete(gb** console) {
    free(*console);
    *console = NULL;
}

void _gb_load(gb* console, const char *path, unsigned int length, uint16_t start) {
    FILE* file;
    file = fopen(path, "rb");
    if (file == NULL) {
        printf("ERROR::GB::_GB_LOAD::FILE_NULL\n");
        return;
    }
    unsigned int i = 0;
    while (1) {
        int byte = fgetc(file);
        if (byte == -1) {
            break;
        }
        gb_memory_write(console->memory, start+i, (uint8_t)byte);
        i++;
    }
}
