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

void gb_cpu_register_dump(gb_cpu_register* reg) {
    printf("AF: %04X  BC: %04X  DE: %04X  HL: %04X  SP: %04X  PC: %04X\n",
           reg->af,
           reg->bc,
           reg->de,
           reg->hl,
           reg->sp,
           reg->pc);
}
