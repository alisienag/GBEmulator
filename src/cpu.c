#include "cpu_register.h"
#include "memory.h"
#include <SDL3/SDL_timer.h>
#include <cpu.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cpu/cpu_load.h>

gb_cpu_op_function_pointer opcode_function_table[256] = {NULL};
gb_cpu_op_function_pointer opcode_function_table_cb[256] = {NULL};

gb_cpu* gb_cpu_create() {
    gb_cpu_init();
    gb_cpu* cpu = malloc(sizeof(gb_cpu));
    memset(cpu, 0, sizeof(gb_cpu));
    cpu->cpu_register = gb_cpu_register_create();
    cpu->cycles = 0;
    cpu->_executed_count = 0;
    cpu->running = 1;
    return cpu;
}

void gb_cpu_execute(gb_cpu* cpu, gb_memory* memory) {
    if (cpu->running == 0) {
        return;
    }
    uint8_t opcode = gb_memory_read(memory, cpu->cpu_register->pc);
    cpu->cpu_register->pc++;
    printf("%d: ", cpu->_executed_count);
    if(opcode_function_table[opcode] != NULL) {
        printf("EXECUTING INSTRUCTION: 0x%02X\n", opcode);
        opcode_function_table[opcode](cpu, memory);
    } else {
        printf("EXECUTING UNWRITTEN INSTRUCTION: 0x%02X\n", opcode);
        SDL_Delay(1000);
    }
    cpu->_executed_count += 1;
}


void gb_cpu_init() {
    for (unsigned int i = 0; i < 256; i++) {
        opcode_function_table[i] = NULL;
    }

    //from cpu_load
    opcode_function_table[0x02] = gb_cpu_op_ld_bc_a;
    opcode_function_table[0x06] = gb_cpu_op_ld_b_d8;
    opcode_function_table[0x0A] = gb_cpu_op_ld_a_bc;
    opcode_function_table[0x0E] = gb_cpu_op_ld_c_d8;
    opcode_function_table[0x12] = gb_cpu_op_ld_de_a;
    opcode_function_table[0x16] = gb_cpu_op_ld_d_d8;
    opcode_function_table[0x1A] = gb_cpu_op_ld_a_de;
    opcode_function_table[0x1E] = gb_cpu_op_ld_e_d8;
    opcode_function_table[0x22] = gb_cpu_op_ld_hli_a;
    opcode_function_table[0x26] = gb_cpu_op_ld_h_d8;
    opcode_function_table[0x2A] = gb_cpu_op_ld_a_hli;
    opcode_function_table[0x2E] = gb_cpu_op_ld_l_d8;
    opcode_function_table[0x32] = gb_cpu_op_ld_hld_a;
    opcode_function_table[0x36] = gb_cpu_op_ld_hl_d8;
    opcode_function_table[0x3A] = gb_cpu_op_ld_a_hld;
    opcode_function_table[0x3E] = gb_cpu_op_ld_a_d8;

    for (unsigned int i = 0; i < 15*4; i++) {
        opcode_function_table[0x40 + i] = gb_cpu_op_ld_r_r;
    }

    //from misc (NOT YET IMPLEMENTED)
    //opcode_function_table[0x76] = gb_cpu_op_ld_halt;
    gb_cpu_init_extended_instructions();
}

void gb_cpu_init_extended_instructions() {
    for (unsigned i = 0; i < 256; i++) {
        opcode_function_table_cb[i] = NULL;
    }
}

void gb_cpu_delete(gb_cpu* cpu) {
    free(cpu->cpu_register);
    cpu->cpu_register = NULL;
    free(cpu);
    cpu = NULL;
}
