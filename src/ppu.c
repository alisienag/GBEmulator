#include "ppu.h"
#include "graphics.h"
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

void gb_ppu_step(gb_window* window, gb_ppu* ppu, gb_memory* mem, int cycles) {
    gb_ppu_sync_from_mem(ppu, mem);
    uint8_t old_mode = ppu->STAT & 0x3;
    uint8_t old_ly_comparison = (ppu->LY == ppu->LYC);

    ppu->scanline_counter += cycles;   


    if (ppu->scanline_counter >= GB_PPU_SCANLINE_DUR) {
        ppu->scanline_counter -= GB_PPU_SCANLINE_DUR;
        ppu->LY++;
        if (ppu->LY < 144) {
            gb_ppu_draw_bg_scanline(window, ppu, mem);
        }
    }
    if (ppu->LY >= 154)
        ppu->LY = 0;
    if (ppu->LY >= 144) {
        ppu->STAT = (ppu->STAT & 0xFC) | 1;
        /*if (!(gb_memory_read(mem, 0xFF0F) & 1)) {
            gb_memory_write(mem, 0xFF0F, gb_memory_read(mem, 0xFF0F) | GB_INTERRUPT_VBLANK_BIT);
        }*/
    } else if (ppu->scanline_counter < 80) {
        ppu->STAT = (ppu->STAT & 0xFC) | 2;
    } else if (ppu->scanline_counter < 252) {
        ppu->STAT = (ppu->STAT & 0xFC) | 3;
    } else {
        ppu->STAT = (ppu->STAT & 0xFC) | 0;
    }

    uint8_t new_mode = ppu->STAT & 0x3;
    uint8_t new_ly_comparison = (ppu->LY == ppu->LYC);

    if (new_ly_comparison) {
        ppu->STAT |= (0x4);
    } else {
        ppu->STAT &= ~(0x4);
    }
    
    if ((ppu->STAT & 0x40) && new_ly_comparison && !old_ly_comparison) {
        gb_memory_write(mem, GB_IF, gb_memory_read(mem, GB_IF) | GB_INTERRUPT_STAT_BIT);
    }

    if (old_mode != new_mode) {
        if (new_mode == 0 && (ppu->STAT & 0x8)) {
            gb_memory_write(mem, GB_IF, gb_memory_read(mem, GB_IF) | GB_INTERRUPT_STAT_BIT);
        } else if (new_mode == 1 && (ppu->STAT & 0x10)) {
            gb_memory_write(mem, GB_IF, gb_memory_read(mem, GB_IF) | GB_INTERRUPT_STAT_BIT);
        } else if (new_mode == 2 && (ppu->STAT & 0x20)) {
            gb_memory_write(mem, GB_IF, gb_memory_read(mem, GB_IF) | GB_INTERRUPT_STAT_BIT);
        }
    }

    gb_ppu_sync_to_mem(ppu, mem);
}
void gb_ppu_draw_bg_scanline(gb_window* window, gb_ppu* ppu, gb_memory* mem) {
    uint16_t tilemap_start = 0;
    uint16_t tiledata_start = 0;
    if (ppu->LCDC & 0x8) {
        tilemap_start = 0x9C00;  
    } else {
        tilemap_start = 0x9800;
    }
    if (ppu->LCDC & 0x10) {
        tiledata_start = 0x8000;
    } else {
        tiledata_start = 0x9000;
    }
    uint8_t scx = ppu->SCX;
    uint8_t scy = ppu->SCY;
    for (int x = 0; x < 160; x++) {
        uint8_t colour = 0;
        uint8_t tilemap_x = ((scx + x) % 256) / 8;
        uint8_t tilemap_y = ((scy + ppu->LY) % 256) / 8;
        uint8_t tiledata_index = gb_memory_read(mem, tilemap_start + (tilemap_y * 32) + tilemap_x);
        
        uint8_t byte1;
        uint8_t byte2;
        if (tiledata_start == 0x8000) {
            byte1 = gb_memory_read(mem, tiledata_start + (tiledata_index * 16) + (((scy + ppu->LY) % 8) * 2));
            byte2 = gb_memory_read(mem, tiledata_start + (tiledata_index * 16) + (((scy + ppu->LY) % 8) * 2)+1);
        } else {
            byte1 = gb_memory_read(mem, tiledata_start + (int8_t)(tiledata_index * 16) + (((scy + ppu->LY) % 8) * 2));
            byte2 = gb_memory_read(mem, tiledata_start + (int8_t)(tiledata_index * 16) + (((scy + ppu->LY) % 8) * 2)+1);
        }

        int bit_index = (scx + x) % 8;

        colour = ((byte1 >> (7 - bit_index)) & 0x1) | (((byte2 >> (7 - bit_index)) & 0x1) << 1);
        
        gb_window_set_pixel(window, x, ppu->LY, colour);
    }
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
