#include "ppu.h"
#include "cpu.h"
#include "memory.h"
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>

gb_ppu* gb_ppu_create() {
    gb_ppu* ppu = malloc(sizeof(gb_ppu));
    memset(ppu, 0, sizeof(gb_ppu));
    return ppu;
}

void gb_ppu_sync_from_mem(gb_ppu* ppu, gb_memory* mem) {
    ppu->LCDC = gb_memory_read(mem, GB_PPU_LCDC);
    ppu->STAT = gb_memory_read(mem, GB_PPU_STAT);
    ppu->SCY = gb_memory_read(mem, GB_PPU_SCY);
    ppu->SCX = gb_memory_read(mem, GB_PPU_SCX);
    ppu->LY = gb_memory_read(mem, GB_PPU_LY);
    ppu->LYC = gb_memory_read(mem, GB_PPU_LYC);
    ppu->BGP = gb_memory_read(mem, GB_PPU_BGP);
    ppu->OBP0 = gb_memory_read(mem, GB_PPU_OBP0);
    ppu->OBP1 = gb_memory_read(mem, GB_PPU_OBP1);
    ppu->WY = gb_memory_read(mem, GB_PPU_WY);
    ppu->WX = gb_memory_read(mem, GB_PPU_WX);
}

void gb_ppu_step(gb_ppu* ppu, gb_memory* mem, int cycles) {
    gb_ppu_sync_from_mem(ppu, mem);
    ppu->scanline_counter += cycles;   
    if (ppu->scanline_counter >= GB_PPU_SCANLINE_DUR) {
        ppu->scanline_counter -= GB_PPU_SCANLINE_DUR;
        ppu->LY++;
    }
    if (ppu->LY >= 154)
        ppu->LY = 0;
    if (ppu->LY >= 144) {
        ppu->STAT = (ppu->STAT & 0xFC) | 1;
        if (!(gb_memory_read(mem, 0xFF0F) & 1)) {
            gb_memory_write(mem, 0xFF0F, gb_memory_read(mem, 0xFF0F) | 1);
        }
    }
    if (ppu->scanline_counter < 80) {
        ppu->STAT = (ppu->STAT & 0xFC) | 2;
    } else if (ppu->scanline_counter < 252) {
        ppu->STAT = (ppu->STAT & 0xFC) | 3;
    } else {
        ppu->STAT = (ppu->STAT & 0xFC) | 0;
    }
    gb_ppu_sync_to_mem(ppu, mem);
}

void gb_ppu_sync_to_mem(gb_ppu* ppu, gb_memory* mem) {
    gb_memory_write(mem, GB_PPU_LCDC, ppu->LCDC);
    gb_memory_write(mem, GB_PPU_STAT, ppu->STAT);
    gb_memory_write(mem, GB_PPU_SCY, ppu->SCY);
    gb_memory_write(mem, GB_PPU_SCX, ppu->SCX);
    gb_memory_write(mem, GB_PPU_LY, ppu->LY);
    gb_memory_write(mem, GB_PPU_LYC, ppu->LYC);
    gb_memory_write(mem, GB_PPU_BGP, ppu->BGP);
    gb_memory_write(mem, GB_PPU_OBP0, ppu->OBP0);
    gb_memory_write(mem, GB_PPU_OBP1, ppu->OBP1);
    gb_memory_write(mem, GB_PPU_WY, ppu->WY);
    gb_memory_write(mem, GB_PPU_WX, ppu->WX);
}
