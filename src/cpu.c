#include "cpu_register.h"
#include "memory.h"
#include <cpu.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

gb_cpu_op_function_pointer opcode_function_table[256] = {NULL};

gb_cpu* gb_cpu_create() {
    gb_cpu_init();
    gb_cpu* cpu = malloc(sizeof(gb_cpu));
    memset(cpu, 0, sizeof(gb_cpu));
    cpu->cpu_register = gb_cpu_register_create();
    cpu->cycles = 0;
    return cpu;
}

void gb_cpu_execute(gb_cpu* cpu, gb_memory* memory) {
    uint8_t opcode = gb_memory_read(memory, cpu->cpu_register->pc);
    cpu->cpu_register->pc++;  
    if(opcode_function_table[opcode] != NULL) {
        opcode_function_table[opcode](cpu, memory);
    } else {
        printf("EXECUTING UNWRITTEN INSTRUCTION: 0x%02X\n", opcode);
    }
}

//OPCODES STARTING WITH 0x0

GB_CPU_OP(gb_cpu_op_nop) {
    printf("EXECUTING_OP::NOP::0x00\n");
    GB_CYCLES_INC(4);
}

GB_CPU_OP(gb_cpu_op_ld_c_u8) {
    printf("EXECUTING_OP::LD_C_U8::0x0E\n");
    REG_C = GB_READ_8(memory, REG_PC);
    GB_CYCLES_INC(8);
}

//OPCODES STARTINGW WITH 0x1

GB_CPU_OP(gb_cpu_op_ld_de_u16) {
    printf("EXECUTING_OP::LD_DE_U16::0x11\n");
    gb_cpu_register_set_de(cpu->cpu_register, GB_READ_16(memory, REG_PC));
    GB_CYCLES_INC(12);
}

GB_CPU_OP(gb_cpu_op_jr_i8) {
    printf("EXECUTING_OP::JR_i8::0x18\n");
    cpu->cpu_register->pc += (int8_t)gb_memory_read(memory, REG_PC);
    REG_PC += 1;
    GB_CYCLES_INC(12);
}

//OPCODES STARTING WITH 0x2

GB_CPU_OP(gb_cpu_op_jr_nz_i8) {
    printf("EXECUTING_OP::JR_NZ_I8::0x20\n");
    int8_t jmp_count = GB_READ_8(memory, cpu->cpu_register->pc);
    cpu->cpu_register->pc += 1;
    GB_CYCLES_INC(8);
    if ((REG_F & FLAG_Z) == 0) {
        cpu->cpu_register->pc += jmp_count;
        GB_CYCLES_INC(4);
    }
}

GB_CPU_OP(gb_cpu_op_ld_hl_u16) {
    printf("EXECUTING_OP::LD_HL_U16::0x21\n");
    gb_cpu_register_set_hl(cpu->cpu_register, GB_READ_16(memory, cpu->cpu_register->pc));
    cpu->cpu_register->pc += 2;
    GB_CYCLES_INC(12);
}

//OPCODES STARTING WITH 0x3

GB_CPU_OP(gb_cpu_op_ld_sp_u16) {
    printf("EXECUTING_OP::LD_SP_u16::0x31\n");
    uint8_t low_byte = gb_memory_read(memory, cpu->cpu_register->pc);
    uint8_t high_byte = gb_memory_read(memory, cpu->cpu_register->pc+1);
    cpu->cpu_register->pc += 2;
    gb_cpu_register_set_sp(cpu->cpu_register, (high_byte << 8) | low_byte);
    GB_CYCLES_INC(12);
}

GB_CPU_OP(gb_cpu_op_ld_hld_a) {
    printf("EXECUTING_OP::LD_(HL-)_A::0x32\n");
    gb_memory_write(memory, REG_HL, REG_A);
    gb_cpu_register_set_hl(cpu->cpu_register, REG_HL - 1);
    cpu->cpu_register->pc += 1;
    GB_CYCLES_INC(8);
}

//OPCODES STARTING WITH 0x7C

GB_CPU_OP(gb_cpu_op_ld_a_h) {
    printf("EXECUTING_OP::LD_A_H::0x7C\n");
    REG_A = REG_H;
    GB_CYCLES_INC(4);
}

//OPCODES STARTING WITH 0xA

GB_CPU_OP(gb_cpu_op_xor_a_a) {
    printf("EXECUTING_OP::XOR_A_A::0xAF\n");
    uint8_t value = gb_cpu_register_get_a(cpu->cpu_register);
    value ^= value;
    gb_cpu_register_set_a(cpu->cpu_register, value);
    gb_cpu_register_set_f(cpu->cpu_register, FLAG_Z);
    GB_CYCLES_INC(4);
}

void gb_cpu_init() {
    for (unsigned int i = 0; i < 256; i++) {
        opcode_function_table[i] = NULL;
    }
    opcode_function_table[0x00] = gb_cpu_op_nop;
    opcode_function_table[0x0E] = gb_cpu_op_ld_c_u8;
    opcode_function_table[0x11] = gb_cpu_op_ld_de_u16;
    opcode_function_table[0x18] = gb_cpu_op_jr_i8;
    opcode_function_table[0x20] = gb_cpu_op_jr_nz_i8;
    opcode_function_table[0x21] = gb_cpu_op_ld_hl_u16;
    opcode_function_table[0x31] = gb_cpu_op_ld_sp_u16;
    opcode_function_table[0x32] = gb_cpu_op_ld_hld_a;
    opcode_function_table[0x7C] = gb_cpu_op_ld_a_h;
    opcode_function_table[0xAF] = gb_cpu_op_xor_a_a;
}

void gb_cpu_delete(gb_cpu* cpu) {
    free(cpu->cpu_register);
    cpu->cpu_register = NULL;
    free(cpu);
    cpu = NULL;
}
