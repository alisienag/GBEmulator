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
    _gb_load(console, path, 256, console->memory->bios);
    console->cpu->cpu_register->pc = 0;
}

void gb_rom_load(gb* console, const char* path) {
    _gb_load(console, path, 0x8000, console->memory->rom);
}

void gb_rom_unload();

void gb_delete(gb** console) {
    free(*console);
    *console = NULL;
}

void _gb_load(gb* console, const char *path, unsigned int length, uint8_t* start) {
    FILE* file;
    file = fopen(path, "rb");
    if (file == NULL) {
        printf("ERROR::GB::_GB_LOAD::FILE_NULL\n");
        exit(1);
        return;
    }
    unsigned int i = 0;
    for (int i = 0; i < length; i++) {
        int byte = fgetc(file);
        if (byte == EOF) break;
        start[i] = (uint8_t)byte;
    }
    fclose(file);
}
