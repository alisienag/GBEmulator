#ifndef _H_GB
#define _H_GB

#include "cpu.h"
#include "memory.h"
typedef struct {
    const int rom_loaded;
    gb_cpu* cpu;
    gb_memory* memory;
} gb;

gb* gb_create();
void gb_delete(gb** console);

void gb_bios_load(gb* console, const char* path);

void gb_rom_load(gb* console, const char* path);
void gb_rom_unload();


void _gb_load(gb* gb, const char* path, unsigned int length, uint8_t* start);

#endif
