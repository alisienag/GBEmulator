#include "cpu.h"
#include <cpu_register.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


gb_cpu_register* gb_cpu_register_create() {
    gb_cpu_register* reg;
    reg = malloc(sizeof(gb_cpu_register));
    memset(reg, 0, sizeof(gb_cpu_register));
    return reg;
}
void gb_cpu_register_delete(gb_cpu_register* reg) {
    free(reg);
    reg = NULL;
}


void gb_cpu_register_set_a(gb_cpu_register* reg, uint8_t value) {
    reg->a = value;
}
uint8_t gb_cpu_register_get_a(gb_cpu_register* reg) { return reg->a; };

void gb_cpu_register_set_b(gb_cpu_register* reg, uint8_t value) { reg->b = value; };
uint8_t gb_cpu_register_get_b(gb_cpu_register* reg) { return reg->b; };

void gb_cpu_register_set_c(gb_cpu_register* reg, uint8_t value) { reg->c = value; };
uint8_t gb_cpu_register_get_c(gb_cpu_register* reg) { return reg->c; };

void gb_cpu_register_set_d(gb_cpu_register* reg, uint8_t value) { reg->d = value; };
uint8_t gb_cpu_register_get_d(gb_cpu_register* reg) { return reg->d; };

void gb_cpu_register_set_e(gb_cpu_register* reg, uint8_t value) { reg->e = value; };
uint8_t gb_cpu_register_get_e(gb_cpu_register* reg) { return reg->e; };

// gameboys never use lower 4 bits of F, so we always set it to 0 just to prevent bugs
void gb_cpu_register_set_f(gb_cpu_register* reg, uint8_t value) { reg->f = (value & 0xF0); };
uint8_t gb_cpu_register_get_f(gb_cpu_register* reg) { return reg->f; };

void gb_cpu_register_set_h(gb_cpu_register* reg, uint8_t value) { reg->h = value; };
uint8_t gb_cpu_register_get_h(gb_cpu_register* reg) { return reg->h; };

void gb_cpu_register_set_l(gb_cpu_register* reg, uint8_t value) { reg->l = value; };
uint8_t gb_cpu_register_get_l(gb_cpu_register* reg) { return reg->l; };

void gb_cpu_register_set_pc(gb_cpu_register* reg, uint16_t value) {
    reg->pc = value;
}

uint16_t gb_cpu_register_get_pc(gb_cpu_register* reg) {
    return reg->pc;
}

void gb_cpu_register_set_sp(gb_cpu_register* reg, uint16_t value) {
    reg->sp = value;
}

uint16_t gb_cpu_register_get_sp(gb_cpu_register* reg) {
    return reg->sp;
}

void gb_cpu_register_set_af(gb_cpu_register* reg, uint16_t value) {
    reg->a = (value & 0xFF00) >> 8;
    reg->f = (value & 0xFF);
}

uint16_t gb_cpu_register_get_af(gb_cpu_register* reg) {
    return (reg->a << 8) | (reg->f & 0xF0); //Gameboys never use the lower 4 bits of F
}

void gb_cpu_register_set_bc(gb_cpu_register* reg, uint16_t value) {
    reg->b = (value & 0xFF00) >> 8;
    reg->c = (value & 0xFF);
}

uint16_t gb_cpu_register_get_bc(gb_cpu_register* reg) {
    return (reg->b << 8) | (reg->c);
}

void gb_cpu_register_set_de(gb_cpu_register* reg, uint16_t value) {
    reg->d = (value & 0xFF00) >> 8;
    reg->e = (value & 0xFF);
}

uint16_t gb_cpu_register_get_de(gb_cpu_register* reg) {
    return (reg->d << 8) | (reg->e);
}

void gb_cpu_register_set_hl(gb_cpu_register* reg, uint16_t value) {
    reg->h = (value & 0xFF00) >> 8;
    reg->l = (value & 0xFF);
}

uint16_t gb_cpu_register_get_hl(gb_cpu_register* reg) {
    return (reg->h << 8) | (reg->l);
}

void gb_cpu_register_dump(gb_cpu_register* reg) {
    printf("AF: %04X  BC: %04X  DE: %04X  HL: %04X  SP: %04X  PC: %04X\n",
           gb_cpu_register_get_af(reg),
           gb_cpu_register_get_bc(reg),
           gb_cpu_register_get_de(reg),
           gb_cpu_register_get_hl(reg),
           gb_cpu_register_get_sp(reg),
           gb_cpu_register_get_pc(reg));
}
