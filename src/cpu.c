#include "cpu_register.h"
#include "memory.h"
#include <cpu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

gb_cpu* gb_cpu_create() {
    gb_cpu* cpu = malloc(sizeof(gb_cpu));
    cpu = malloc(sizeof(gb_cpu));
    memset(cpu, 0, sizeof(gb_cpu));
    cpu->cpu_register = gb_cpu_register_create();
    return cpu;
}

void gb_cpu_execute(gb_cpu* cpu, gb_memory* memory) {
    uint8_t current_instruction = gb_memory_read(memory, cpu->cpu_register->pc);
    switch(current_instruction) {
        case 0x0:
            break;
        
        default: {
            printf("Weird Instruction Reached");
        }
    }
    cpu->cpu_register->pc++;
}

void gb_cpu_delete(gb_cpu* cpu) {
    free(cpu->cpu_register);
    cpu->cpu_register = NULL;
    free(cpu);
    cpu = NULL;
}
