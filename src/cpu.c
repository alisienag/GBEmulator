#include "cpu_register.h"
#include "memory.h"
#include <SDL3/SDL_timer.h>
#include <cpu.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

gb_cpu_op_function_pointer opcode_function_table[256] = {NULL};
gb_cpu_op_function_pointer opcode_function_table_cb[256] = {NULL};

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
    printf("%d: ", cpu->_executed_count);
    if(opcode_function_table[opcode] != NULL) {
        opcode_function_table[opcode](cpu, memory);
    } else {
        printf("EXECUTING UNWRITTEN INSTRUCTION: 0x%02X\n", opcode);
        SDL_Delay(1000);
    }
    cpu->_executed_count += 1;
}

//OPCODES STARTING WITH 0x0

GB_CPU_OP(gb_cpu_op_nop) {
    printf("EXECUTING_OP::NOP::0x00\n");
    GB_CYCLES_INC(4);
}

GB_CPU_OP(gb_cpu_op_dec_b) {
    printf("EXECUTING_OP::DEC_B::0x05\n");
    uint8_t old_value = REG_B;
    REG_F = (REG_F & FLAG_C) | FLAG_N;
    REG_B = REG_B - 1;
    if (REG_B == 0) {
        REG_F |= FLAG_Z;
    }
    if ((old_value & 0xF) == 0x0) {
        REG_F |= FLAG_H;
    }
    GB_CYCLES_INC(4);
}

GB_CPU_OP(gb_cpu_ld_b_u8) {
    printf("EXECUTING_OP::LD_B_u8::0x06\n");
    REG_B = GB_READ_8(memory, REG_PC);
    REG_PC += 1;
    GB_CYCLES_INC(8);
}

GB_CPU_OP(gb_cpu_op_ld_c_u8) {
    printf("EXECUTING_OP::LD_C_U8::0x0E\n");
    REG_C = GB_READ_8(memory, REG_PC);
    REG_PC += 1;
    GB_CYCLES_INC(8);
}

GB_CPU_OP(gb_cpu_op_inc_c) {
    printf("EXECUTING_OP::INC_C::0x0C\n");
    uint8_t pre_value = REG_C;
    gb_cpu_register_set_c(cpu->cpu_register, REG_C + 1);
    uint8_t post_value = REG_C;
    uint8_t zero_flag = 0x0;
    uint8_t half_carry_flag = 0x0;
    if (REG_C == 0) {
        zero_flag = FLAG_Z;
    }
    if ((pre_value & 0xF) + 1 > 0xF) {
        half_carry_flag = FLAG_H;
    }
    gb_cpu_register_set_f(cpu->cpu_register, (0x0) | (zero_flag) | (half_carry_flag) | (REG_F & FLAG_C));
    GB_CYCLES_INC(4);
}

//OPCODES STARTINGW WITH 0x1
    
GB_CPU_OP(gb_cpu_op_ld_de_u16) {
    printf("EXECUTING_OP::LD_DE_U16::0x11\n");
    gb_cpu_register_set_de(cpu->cpu_register, GB_READ_16(memory, REG_PC));
    REG_PC += 2;
    GB_CYCLES_INC(12);
}

GB_CPU_OP(gb_cpu_op_rla) {
    printf("EXECUTING_OP::RLA::0x17\n");
    uint8_t value = REG_A;
    REG_A = REG_A << 1;
    if ((REG_F & FLAG_C) == FLAG_C) {
        REG_A |= 0b00000001;
    }
    REG_F = 0;
    if ((value & 0b10000000) == 0b10000000) {
        REG_F |= FLAG_C;
    }

    GB_CYCLES_INC(4);
}

GB_CPU_OP(gb_cpu_op_jr_i8) {
    printf("EXECUTING_OP::JR_i8::0x18\n");
    int8_t jmp_count = (int8_t)gb_memory_read(memory, REG_PC);
    REG_PC += 1;
    REG_PC += jmp_count;
       GB_CYCLES_INC(12);
}

GB_CPU_OP(gb_cpu_op_ld_a_de) {
    printf("EXECUTING_OP::LD_A_(DE)::0x1A\n");
    REG_A = GB_READ_16(memory, REG_DE);
    GB_CYCLES_INC(8);
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

GB_CPU_OP(gb_cpu_op_ld_hli_a) {
    printf("EXECUTING_OP::LD_(HL+)_A::0x22\n");
    GB_WRITE_8(memory, REG_HL, REG_A);
    gb_cpu_register_set_hl(cpu->cpu_register, REG_HL + 1);
    GB_CYCLES_INC(8);
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
    GB_CYCLES_INC(8);
}

GB_CPU_OP(gb_cpu_op_ld_a_u8) {
    printf("EXECUTING_OP::LD_A_U8::0x3E\n");
    gb_cpu_register_set_a(cpu->cpu_register, GB_READ_8(memory, REG_PC));
    cpu->cpu_register->pc += 1;
    GB_CYCLES_INC(8);
}

//OPCODES STARTING WITH 0x4
//
GB_CPU_OP(gb_cpu_op_ld_c_a) {
    printf("EXECUTING_OP::LD_C_A::0x4F\n");
    REG_C = REG_A;
    GB_CYCLES_INC(4);
}

//OPCODES STARTING WITH 0x7

GB_CPU_OP(gb_cpu_op_ld_hl_a) {
    printf("EXECUTING_OP::LD_(AL)_A::0x77\n");
    GB_WRITE_8(memory, REG_HL, REG_A);
    GB_CYCLES_INC(8);
}

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

//OPCODES STARTING WITH 0xC

GB_CPU_OP(gb_cpu_op_pop_bc) {
    printf("EXECUTING_OP::POP_BC::0xC1\n");
    uint8_t first_byte = GB_READ_8(memory, REG_SP);
    REG_SP += 1;
    uint8_t second_byte = GB_READ_8(memory, REG_SP);
    REG_SP += 1;

    REG_C = first_byte;
    REG_B = second_byte;
    GB_CYCLES_INC(12);
}

GB_CPU_OP(gb_cpu_op_push_bc) {
    printf("EXECUTING_OP::PUSH_BC::0xC5\n");
    REG_SP -= 1;
    GB_WRITE_8(memory, REG_SP, REG_B);
    REG_SP -= 1;
    GB_WRITE_8(memory, REG_SP, REG_C);
    GB_CYCLES_INC(16);
}

GB_CPU_OP(gb_cpu_op_call_u16) {
    printf("EXECUTING_OP::CALL_u16::0xCD\n");
    uint8_t low_byte = GB_READ_8(memory, REG_PC);
    uint8_t high_byte = GB_READ_8(memory, REG_PC+1);

    REG_PC += 2;

    REG_SP -= 1;
    GB_WRITE_8(memory, REG_SP, (REG_PC >> 8) & 0xFF);
    REG_SP -= 1;
    GB_WRITE_8(memory, REG_SP, REG_PC & 0xFF);
    REG_PC = (high_byte << 8) | low_byte;
    GB_CYCLES_INC(24);
}

//CB OPCODES

GB_CPU_OP(gb_cpu_op_cb) {
    printf("EXECUTING_OP::CB_INSTRUCTION::0xCB");

    uint8_t opcode = GB_READ_8(memory, REG_PC);
    REG_PC += 1;


    if(opcode_function_table_cb[opcode] != NULL) {
        opcode_function_table_cb[opcode](cpu, memory);
    } else {
        printf("-> Instruction not implemented: 0x%02X\n", opcode);
        SDL_Delay(1);
    }

}

//OPCODES STARTING WITH 0xE

GB_CPU_OP(gb_cpu_op_ld_ff00_u8_a) {
    printf("EXECUTING_OP::LD_FF00_u8_A::0xE0\n");
    GB_WRITE_8(memory, (0xFF00) + GB_READ_8(memory, REG_PC), REG_A);
    REG_PC += 1;
    GB_CYCLES_INC(12);
}

GB_CPU_OP(gb_cpu_op_ld_ff00_c_a) {
    printf("EXECUTING_OP::LD_FF00_C_A::0xE2\n");
    gb_memory_write(memory, (0xFF00 + REG_C), REG_A);
    GB_CYCLES_INC(8);
}

void gb_cpu_init() {
    for (unsigned int i = 0; i < 256; i++) {
        opcode_function_table[i] = NULL;
    }
    opcode_function_table[0x00] = gb_cpu_op_nop;
    opcode_function_table[0x05] = gb_cpu_op_dec_b;
    opcode_function_table[0x06] = gb_cpu_ld_b_u8;
    opcode_function_table[0x0E] = gb_cpu_op_ld_c_u8;
    opcode_function_table[0x0C] = gb_cpu_op_inc_c;
    opcode_function_table[0x11] = gb_cpu_op_ld_de_u16;
    opcode_function_table[0x17] = gb_cpu_op_rla;
    opcode_function_table[0x18] = gb_cpu_op_jr_i8;
    opcode_function_table[0x1A] = gb_cpu_op_ld_a_de;
    opcode_function_table[0x20] = gb_cpu_op_jr_nz_i8;
    opcode_function_table[0x21] = gb_cpu_op_ld_hl_u16;
    opcode_function_table[0x22] = gb_cpu_op_ld_hli_a;
    opcode_function_table[0x31] = gb_cpu_op_ld_sp_u16;
    opcode_function_table[0x32] = gb_cpu_op_ld_hld_a;
    opcode_function_table[0x3E] = gb_cpu_op_ld_a_u8;
    opcode_function_table[0x4F] = gb_cpu_op_ld_c_a;
    opcode_function_table[0x77] = gb_cpu_op_ld_hl_a;
    opcode_function_table[0x7C] = gb_cpu_op_ld_a_h;
    opcode_function_table[0xAF] = gb_cpu_op_xor_a_a;
    opcode_function_table[0xC1] = gb_cpu_op_pop_bc;
    opcode_function_table[0xC5] = gb_cpu_op_push_bc;
    opcode_function_table[0xCB] = gb_cpu_op_cb;
    opcode_function_table[0xCD] = gb_cpu_op_call_u16;
    opcode_function_table[0xE0] = gb_cpu_op_ld_ff00_u8_a;
    opcode_function_table[0xE2] = gb_cpu_op_ld_ff00_c_a;


    gb_cpu_init_extended();
}

GB_CPU_OP(gb_cpu_op_cb_rl_c) {
    printf(" -> ::0x11::RL_C\n");
    uint8_t value = REG_C;
    int old_carry = 0;
    if ((REG_F & FLAG_C) == FLAG_C) {
        old_carry = 1;
    }
    int new_carry = 0;
    if ((value >> 7) == 0b00000001) {
        new_carry = 1;
    }
    REG_C = (uint8_t)(REG_C << 1);
    if (old_carry == 1) {
        REG_C |= 0b00000001;
    }
    REG_F = 0x0;
    if (REG_C == 0x0) {
        REG_F |= FLAG_Z;
    }
    if (new_carry == 1) {
        REG_F |= FLAG_C;
    }
    GB_CYCLES_INC(8);
}

GB_CPU_OP(gb_cpu_op_cb_bit_7_h) {
    printf(" -> ::0x7C::BIT_7_H\n");
    REG_F = (REG_F & FLAG_C);
    if (((REG_H >> 7) & 1) == 0) {
        REG_F |= (0xFF & FLAG_Z);
    }
    REG_F |= (0x0 & FLAG_N);
    REG_F |= (FLAG_H);
    GB_CYCLES_INC(8);
}

void gb_cpu_init_extended() {
    for (unsigned i = 0; i < 256; i++) {
        opcode_function_table_cb[i] = NULL;
    }
    opcode_function_table_cb[0x11] = gb_cpu_op_cb_rl_c;
    opcode_function_table_cb[0x7C] = gb_cpu_op_cb_bit_7_h;
}

void gb_cpu_delete(gb_cpu* cpu) {
    free(cpu->cpu_register);
    cpu->cpu_register = NULL;
    free(cpu);
    cpu = NULL;
}
