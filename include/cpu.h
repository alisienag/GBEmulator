#ifndef _H_CPU
#define _H_CPU

#include "memory.h"
#include <cpu_register.h>

typedef struct {
    gb_cpu_register* cpu_register;
} gb_cpu;

gb_cpu* gb_cpu_create();
void gb_cpu_delete(gb_cpu* cpu);

void gb_cpu_execute(gb_cpu* cpu, gb_memory* memory);

#endif // !_H_CPU
