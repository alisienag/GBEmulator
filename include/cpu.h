#ifndef _H_CPU
#define _H_CPU


#include "memory.h"
#include "cpu_register.h"
#include <stdio.h>

#define GB_CPU_OP(x) void (x)(gb_cpu* cpu, gb_memory* memory)

//Only use these in the context of "GB_CPU_OP"

#define REG_A cpu->cpu_register->a
#define REG_B cpu->cpu_register->b
#define REG_C cpu->cpu_register->c
#define REG_D cpu->cpu_register->d
#define REG_E cpu->cpu_register->e
#define REG_F cpu->cpu_register->f
#define REG_H cpu->cpu_register->h
#define REG_L cpu->cpu_register->l
#define REG_AF cpu->cpu_register->af
#define REG_BC cpu->cpu_register->bc
#define REG_DE cpu->cpu_register->de
#define REG_HL cpu->cpu_register->hl
#define REG_SP cpu->cpu_register->sp
#define REG_PC cpu->cpu_register->pc

#define GB_WRITE_8(loc, value) gb_memory_write(memory, loc, value)
#define GB_READ_8(loc) gb_memory_read(memory, loc)
#define GB_WRITE_16(loc, value) do {\
    GB_WRITE_8(loc, value & 0xFF);\
    GB_WRITE_8(loc+1, (value & 0xFF00) >> 8);\
    } while(0)
#define GB_READ_16(loc) (GB_READ_8(loc) | (GB_READ_8(loc+1) << 8))

#define GB_CYCLES(val) cpu->cycles += val

#define GB_IME_ENABLE() cpu->cpu_register->ime = 2
#define GB_IME_ENABLE_INSTANT() cpu->cpu_register->ime = 1
#define GB_IME_DISABLE() cpu->cpu_register->ime = 0

//

#define SET_FLAG(flag) (REG_F) |= flag
#define CLEAR_FLAG(flag) (REG_F) &= ~flag;
#define CHECK_FLAG(flag) REG_F & flag

#define FLAG_Z 0x80
#define FLAG_N 0x40
#define FLAG_H 0x20
#define FLAG_C 0x10

#define GB_IME_ENABLED 1
#define GB_IME_ENABLING 2
#define GB_IME_DISABLED 0

typedef struct {
    gb_cpu_register* cpu_register;
    unsigned int cycles;
    int running;
    unsigned int _executed_count;
} gb_cpu;

typedef void (*gb_cpu_op_function_pointer)(gb_cpu*, gb_memory*);

extern gb_cpu_op_function_pointer opcode_function_table[256];
extern gb_cpu_op_function_pointer opcode_function_table_cb[256];

gb_cpu* gb_cpu_create();
void gb_cpu_delete(gb_cpu* cpu);

void gb_cpu_execute(gb_cpu* cpu, gb_memory* memory);

void gb_cpu_init();
void gb_cpu_init_extended_instructions();

#endif // !_H_CPU
