#ifndef _H_CPU
#define _H_CPU

#define GB_CPU_OP(x) void (x)(gb_cpu* cpu, gb_memory* memory) 


#include "memory.h"
#include <cpu_register.h>
#include <stdio.h>

#define GB_READ_8(mem, addr) gb_memory_read(mem, addr)
#define GB_WRITE_8(mem, addr, value) gb_memory_write(mem, addr, value)
#define GB_READ_16(mem, addr) (GB_READ_8(mem, addr+1) << 8 | GB_READ_8(mem, addr))
#define GB_WRITE_16(mem, addr) GB_WRITE_8(mem, addr, value & 0xFF); GB_WRITE_8(mem, addr+1, value >> 8)

#define REG_A cpu->cpu_register->a
#define REG_B cpu->cpu_register->b
#define REG_C cpu->cpu_register->c
#define REG_D cpu->cpu_register->d
#define REG_E cpu->cpu_register->e
#define REG_H cpu->cpu_register->h
#define REG_L cpu->cpu_register->l
#define REG_F cpu->cpu_register->f
#define REG_AF ((REG_A << 8) | REG_F)
#define REG_BC ((REG_B << 8) | REG_C)
#define REG_DE ((REG_D << 8) | REG_E)
#define REG_HL ((REG_H << 8) | REG_L)
#define REG_SP cpu->cpu_register->sp
#define REG_PC cpu->cpu_register->pc

#define FLAG_Z 0b10000000
#define FLAG_N 0b01000000
#define FLAG_H 0b00100000
#define FLAG_C 0b00010000

#define GB_CYCLES_INC(x) cpu->cycles += x

typedef struct {
    gb_cpu_register* cpu_register;
    unsigned int cycles;

    unsigned int _executed_count;
} gb_cpu;

typedef void (*gb_cpu_op_function_pointer)(gb_cpu*, gb_memory*);

extern gb_cpu_op_function_pointer opcode_function_table[256];
extern gb_cpu_op_function_pointer opcode_function_table_cb[256];

gb_cpu* gb_cpu_create();
void gb_cpu_delete(gb_cpu* cpu);

void gb_cpu_execute(gb_cpu* cpu, gb_memory* memory);

//0x0
GB_CPU_OP(gb_cpu_op_nop);
//0x1
GB_CPU_OP(gb_cpu_op_jr_i8);
//0x2
GB_CPU_OP(gb_cpu_op_ld_hl_u16);
//0x3
GB_CPU_OP(gb_cpu_op_ld_sp_u16);
//0xA

void gb_cpu_init();
void gb_cpu_init_extended();

#endif // !_H_CPU
