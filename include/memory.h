#ifndef _H_MEMORY
#define _H_MEMORY
#include <stdint.h>

typedef struct {
    uint8_t bios[0x100];
    uint8_t rom[0x8000];      // 32KB ROM (can be banked, but this is a good start)
    uint8_t vram[0x2000];     // 8KB Video RAM
    uint8_t eram[0x2000];     // 8KB External RAM (from cartridge)
    uint8_t wram[0x2000];     // 8KB Work RAM
    uint8_t oam[0xA0];        // Sprite Attribute Table
    uint8_t io[0x80];         // I/O Registers
    uint8_t hram[0x7F];       // High RAM (stack & fast access)
    uint8_t interrupt_enable; // 1 byte at 0xFFFF
    uint8_t bios_enabled;
} gb_memory;

gb_memory* gb_memory_create();
void gb_memory_delete(gb_memory*);

void gb_memory_write(gb_memory* mem, uint16_t addr, uint8_t value);
uint8_t gb_memory_read(gb_memory* mem, uint16_t addr);

void gb_memory_dump(gb_memory* mem, uint16_t start_address, unsigned int count);

#endif
