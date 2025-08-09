#ifndef _H_CPU_REGISTER
#define _H_CPU_REGISTER

#include <stdint.h>
typedef struct {
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t f;
    uint8_t h;
    uint8_t l;
    uint16_t pc;
    uint16_t sp;
} gb_cpu_register;

gb_cpu_register* gb_cpu_register_create();
void gb_cpu_register_delete(gb_cpu_register* reg);

void gb_cpu_register_set_a(gb_cpu_register* reg, uint8_t value);
uint8_t gb_cpu_register_get_a(gb_cpu_register* reg);

void gb_cpu_register_set_b(gb_cpu_register* reg, uint8_t value);
uint8_t gb_cpu_register_get_b(gb_cpu_register* reg);

void gb_cpu_register_set_c(gb_cpu_register* reg, uint8_t value);
uint8_t gb_cpu_register_get_c(gb_cpu_register* reg);

void gb_cpu_register_set_d(gb_cpu_register* reg, uint8_t value);
uint8_t gb_cpu_register_get_d(gb_cpu_register* reg);

void gb_cpu_register_set_e(gb_cpu_register* reg, uint8_t value);
uint8_t gb_cpu_register_get_e(gb_cpu_register* reg);

void gb_cpu_register_set_f(gb_cpu_register* reg, uint8_t value);
uint8_t gb_cpu_register_get_f(gb_cpu_register* reg);

void gb_cpu_register_set_h(gb_cpu_register* reg, uint8_t value);
uint8_t gb_cpu_register_get_h(gb_cpu_register* reg);

void gb_cpu_register_set_l(gb_cpu_register* reg, uint8_t value);
uint8_t gb_cpu_register_get_l(gb_cpu_register* reg);

void gb_cpu_register_set_pc(gb_cpu_register* reg, uint16_t value);
uint16_t gb_cpu_register_get_pc(gb_cpu_register* reg);

void gb_cpu_register_set_sp(gb_cpu_register* reg, uint16_t value);
uint16_t gb_cpu_register_get_sp(gb_cpu_register* reg);

void gb_cpu_register_set_af(gb_cpu_register* reg, uint16_t value);

uint16_t gb_cpu_register_get_af(gb_cpu_register* reg);

void gb_cpu_register_set_bc(gb_cpu_register* reg, uint16_t value);
uint16_t gb_cpu_register_get_bc(gb_cpu_register* reg);

void gb_cpu_register_set_de(gb_cpu_register* reg, uint16_t value);
uint16_t gb_cpu_register_get_de(gb_cpu_register* reg);

void gb_cpu_register_set_hl(gb_cpu_register* reg, uint16_t value);
uint16_t gb_cpu_register_get_hl(gb_cpu_register* reg);

void gb_cpu_register_dump(gb_cpu_register* reg);
#endif
