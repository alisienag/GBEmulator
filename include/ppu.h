#ifndef _H_PPU
#define _H_PPU

#include "memory.h"

#define GB_PPU_SCANLINE_DUR 114 //456 / 4 = 114

#define GB_PPU_LCDC 0xFF40
#define GB_PPU_STAT 0xFF41
#define GB_PPU_SCY 0xFF42
#define GB_PPU_SCX 0xFF43
#define GB_PPU_LY 0xFF44
#define GB_PPU_LYC 0xFF45
#define GB_PPU_BGP 0xFF47
#define GB_PPU_OBP0 0xFF48
#define GB_PPU_OBP1 0xFF49
#define GB_PPU_WY 0xFF4A
#define GB_PPU_WX 0xFF4B

#define GB_PPU_MODE0
#define GB_PPU_MODE1
#define GB_PPU_MODE2
#define GB_PPU_MODE3

typedef struct {
    uint8_t LCDC;
    uint8_t STAT;
    uint8_t SCY;
    uint8_t SCX;
    uint8_t LY;
    uint8_t LYC;
    uint8_t BGP;
    uint8_t OBP0;
    uint8_t OBP1;
    uint8_t WY;
    uint8_t WX;

    int scanline_counter;
} gb_ppu;

gb_ppu* gb_ppu_create();

void gb_ppu_step(gb_ppu* ppu, gb_memory* mem, int cycles);
void gb_ppu_sync_from_mem(gb_ppu* ppu, gb_memory* mem);
void gb_ppu_sync_to_mem(gb_ppu* ppu, gb_memory* mem);

#endif
