#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

gb_memory* gb_memory_create() {
        gb_memory* mem = malloc(sizeof(gb_memory));
        memset(mem, 0, sizeof(gb_memory));
        mem->bios_enabled = 1;
        return mem;
}

void gb_memory_delete(gb_memory* memory) {
        free(memory);
        memory = NULL;
}


void gb_memory_write(gb_memory* mem, uint16_t addr, uint8_t value) {
    if (addr < 0x8000) {
        printf("Writing to rom WHAT THE FUCK!\n");
        return;
    }
    else if (addr < 0xA000) mem->vram[addr - 0x8000] = value;
    else if (addr < 0xC000) mem->eram[addr - 0xA000] = value;
    else if (addr < 0xE000) mem->wram[addr - 0xC000] = value;
    else if (addr < 0xFE00) return; // Echo RAM or unusable
    else if (addr < 0xFEA0) mem->oam[addr - 0xFE00] = value;
    else if (addr < 0xFF00) return; // Unusable memory
    else if (addr < 0xFF80) {
        if (addr == 0xFF50 && value == 1) {
            mem->bios_enabled  = 0;
            printf("EXIT BIOS");
            exit(5);
        }
        mem->io[addr - 0xFF00] = value;
    }
    else if (addr < 0xFFFF) mem->hram[addr - 0xFF80] = value;
    else mem->interrupt_enable = value;
}

uint8_t gb_memory_read(gb_memory* mem, uint16_t addr) {
    if (addr < 0x0100 && mem->bios_enabled == 1) {
        return mem->bios[addr];
    }
    else if (addr < 0x8000) return mem->rom[addr];
    else if (addr < 0xA000) return mem->vram[addr - 0x8000];
    else if (addr < 0xC000) return mem->eram[addr - 0xA000];
    else if (addr < 0xE000) return mem->wram[addr - 0xC000];
    else if (addr < 0xFE00) return 0xFF; // Echo RAM or unusable
    else if (addr < 0xFEA0) return mem->oam[addr - 0xFE00];
    else if (addr < 0xFF00) return 0xFF; // Unusable memory
    else if (addr < 0xFF80) return mem->io[addr - 0xFF00];
    else if (addr < 0xFFFF) return mem->hram[addr - 0xFF80];
    else return mem->interrupt_enable;
}   

void gb_memory_dump(gb_memory* mem, uint16_t start_address, unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        printf("0x%04X: 0x%02X\n", start_address+i, gb_memory_read(mem, start_address+i));
    }
}
