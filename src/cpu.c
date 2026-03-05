#include "cpu/cpu_cb.h"
#include "cpu_register.h"
#include "memory.h"
#include <SDL3/SDL_timer.h>
#include <cpu.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu/cpu_load.h"
#include "cpu/cpu_load_16.h"
#include "cpu/cpu_alu.h"
#include "cpu/cpu_alu_16.h"
#include "cpu/cpu_control_flow.h"
#include "cpu/cpu_misc.h"
#include "ppu.h"

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
    cpu->cpu_register->ime = 0;
    return cpu;
}

void gb_cpu_execute(gb_cpu* cpu, gb_ppu* ppu, gb_memory* memory) {
    if (cpu->running == GB_CPU_STOPPED) {
        return;
    }
    //INTERRUPT HANDLING!
    if (cpu->cpu_register->ime == GB_IME_ENABLED) {
        if (gb_memory_read(memory, GB_IE) & gb_memory_read(memory, GB_IF) & 1) {
            cpu->running = GB_CPU_RUNNING;
            //VBLANK INTERRUPT REQUSTED AND CAN RUN!
            GB_IME_DISABLE();
            printf("VBLANK INTERRUPT CALLED!!!!!!!!\n");
            cpu->cpu_register->sp -= 2;
            gb_memory_write(memory, cpu->cpu_register->sp + 1, (cpu->cpu_register->pc >> 8) & 0xFF);
            gb_memory_write(memory, cpu->cpu_register->sp, cpu->cpu_register->pc & 0xFF);
            cpu->cpu_register->pc = 0x40;
            exit(1);
        } else if (gb_memory_read(memory, GB_IE) & gb_memory_read(memory, GB_IF) & 0x2) {
            cpu->running = GB_CPU_RUNNING;
            GB_IME_DISABLE();
            printf("STAT INTERRUPT CALLED!!!!!!!!\n");
            cpu->cpu_register->sp -= 2;
            gb_memory_write(memory, cpu->cpu_register->sp + 1, (cpu->cpu_register->pc >> 8) & 0xFF);
            gb_memory_write(memory, cpu->cpu_register->sp, cpu->cpu_register->pc & 0xFF);
            cpu->cpu_register->pc = 0x48;
        } else if (gb_memory_read(memory, GB_IE) & gb_memory_read(memory, GB_IF) & 0x4) {
            cpu->running = GB_CPU_RUNNING;
            GB_IME_DISABLE();
            printf("TIMER INTERRUPT CALLED!!!!!!!!\n");
            cpu->cpu_register->sp -= 2;
            gb_memory_write(memory, cpu->cpu_register->sp + 1, (cpu->cpu_register->pc >> 8) & 0xFF);
            gb_memory_write(memory, cpu->cpu_register->sp, cpu->cpu_register->pc & 0xFF);
            cpu->cpu_register->pc = 0x50;
        }
    }
    if (cpu->cpu_register->ime == GB_IME_ENABLING) {
        cpu->cpu_register->ime = GB_IME_ENABLED;
    }
    //Instruction Handling
    if (cpu->running == GB_CPU_RUNNING) {
        uint8_t opcode = gb_memory_read(memory, cpu->cpu_register->pc);
        cpu->cpu_register->pc++;
        if(opcode_function_table[opcode] != NULL) {
            opcode_function_table[opcode](cpu, memory);
            gb_ppu_step(ppu, memory, cpu->cycles);
            cpu->total_cycles += cpu->cycles;
            cpu->timer += cpu->cycles;
            cpu->div_timer += cpu->cycles;
            cpu->cycles = 0;
        }
    } else {
        gb_ppu_step(ppu, memory, 4);
        cpu->total_cycles += 4;
        cpu->timer += 4;
        cpu->div_timer += 4;
        cpu->cycles = 0;
    }

    while (cpu->div_timer >= 256) {
        cpu->div_timer -= 256;
        memory->io[GB_TIMER_DIV - 0xFF00] = (memory->io[GB_TIMER_DIV - 0xFF00] + 1) & 0xFF;
    }

    //Timer Handling
    if (gb_memory_read(memory, GB_TIMER_TAC) & 0x40) { //If Timer Enabled
        uint8_t clock_select = gb_memory_read(memory, GB_TIMER_TAC) & 0x3;
        uint16_t period = 0;
        switch(clock_select) {
            case 0: period = 256 * 4; break;
            case 1: period = 4 * 4; break;
            case 2: period = 16 * 4; break;
            case 3: period = 64 * 4; break;
        }
        while (cpu->timer >= period) {
            cpu->timer -= period;
            uint8_t tima = gb_memory_read(memory, GB_TIMER_TIMA);
            if (tima == 0xFF) {
                tima = gb_memory_read(memory, GB_TIMER_TMA);
                gb_memory_write(memory, GB_IF, gb_memory_read(memory, GB_IF) | 0x4);
            } else {
                tima += 1;
            }
            gb_memory_write(memory, GB_TIMER_TIMA, tima);
        }
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
    for (unsigned int i = 0; i < 16*4; i++) {
        opcode_function_table[0x40 + i] = gb_cpu_op_ld_r_r;
    }
    opcode_function_table[0xE0] = gb_cpu_op_ldh_a8_a;
    opcode_function_table[0xE2] = gb_cpu_op_ld_c_a;
    opcode_function_table[0xEA] = gb_cpu_op_ld_a16_a;
    opcode_function_table[0xF0] = gb_cpu_op_ldh_a_a8;
    opcode_function_table[0xF2] = gb_cpu_op_ld_a_c;
    opcode_function_table[0xFA] = gb_cpu_op_ld_a_a16;

    //from cpu_load_16
    
    opcode_function_table[0x01] = gb_cpu_op_ld_bc_n16;
    opcode_function_table[0x11] = gb_cpu_op_ld_de_n16;
    opcode_function_table[0x21] = gb_cpu_op_ld_hl_n16;
    opcode_function_table[0x31] = gb_cpu_op_ld_sp_n16;

    opcode_function_table[0xC1] = gb_cpu_op_pop_bc;
    opcode_function_table[0xD1] = gb_cpu_op_pop_de;
    opcode_function_table[0xE1] = gb_cpu_op_pop_hl;
    opcode_function_table[0xF1] = gb_cpu_op_pop_af;

    opcode_function_table[0xC5] = gb_cpu_op_push_bc;
    opcode_function_table[0xD5] = gb_cpu_op_push_de;
    opcode_function_table[0xE5] = gb_cpu_op_push_hl;
    opcode_function_table[0xF5] = gb_cpu_op_push_af;

    opcode_function_table[0x08] = gb_cpu_op_ld_a16_sp;
    opcode_function_table[0xF8] = gb_cpu_op_ld_hl_sp_e8;
    opcode_function_table[0xF9] = gb_cpu_op_ld_sp_hl;

    //from cpu_alu
    //
    for (int i = 0; i < 4; i++) {
        opcode_function_table[(i << 4) + 0x4] = gb_cpu_op_inc_r;
    }
    for (int i = 0; i < 4; i++) {
        opcode_function_table[(i << 4) + 0x5] = gb_cpu_op_dec_r;
    }
    for (int i = 0; i < 4; i++) {
        opcode_function_table[(i << 4) + 0xC] = gb_cpu_op_inc_r;
    }
    for (int i = 0; i < 4; i++) {
        opcode_function_table[(i << 4) + 0xD] = gb_cpu_op_dec_r;
    }
    opcode_function_table[0x2f] = gb_cpu_op_cpl;
    opcode_function_table[0x3f] = gb_cpu_op_ccf;
    //
    for (int i = 0; i < 8; i++) {
        opcode_function_table[0x80 + i] = gb_cpu_op_add_a_r;
    }
    for (int i = 0; i < 8; i++) {
        opcode_function_table[0x88 + i] = gb_cpu_op_adc_a_r;
    }
    for (int i = 0; i < 8; i++) { 
        opcode_function_table[0x90 + i] = gb_cpu_op_sub_a_r;
    }
    for (int i = 0; i < 8; i++) {
        opcode_function_table[0x98 + i] = gb_cpu_op_sbc_a_r;
    }
    for (int i = 0; i < 8; i++) { 
        opcode_function_table[0xA0 + i] = gb_cpu_op_and_a_r;
    }
    for (int i = 0; i < 8; i++) {
        opcode_function_table[0xA8 + i] = gb_cpu_op_xor_a_r;
    }
    for (int i = 0; i < 8; i++) { 
        opcode_function_table[0xB0 + i] = gb_cpu_op_or_a_r;
    }
    for (int i = 0; i < 8; i++) {
        opcode_function_table[0xB8 + i] = gb_cpu_op_cp_a_r;
    }
    opcode_function_table[0xC6] = gb_cpu_op_add_a_n8;
    opcode_function_table[0xD6] = gb_cpu_op_sub_a_n8;
    opcode_function_table[0xE6] = gb_cpu_op_and_a_n8;
    opcode_function_table[0xF6] = gb_cpu_op_or_a_n8;
    opcode_function_table[0xCE] = gb_cpu_op_adc_a_n8;
    opcode_function_table[0xDE] = gb_cpu_op_sbc_a_n8;
    opcode_function_table[0xEE] = gb_cpu_op_xor_a_n8;
    opcode_function_table[0xFE] = gb_cpu_op_cp_a_n8;

    //from cpu_alu_16
    opcode_function_table[0x03] = gb_cpu_op_inc_bc;
    opcode_function_table[0x13] = gb_cpu_op_inc_de;
    opcode_function_table[0x23] = gb_cpu_op_inc_hl;
    opcode_function_table[0x33] = gb_cpu_op_inc_sp;
    opcode_function_table[0x09] = gb_cpu_op_add_hl_bc;
    opcode_function_table[0x19] = gb_cpu_op_add_hl_de;
    opcode_function_table[0x29] = gb_cpu_op_add_hl_hl;
    opcode_function_table[0x39] = gb_cpu_op_add_hl_sp;
    opcode_function_table[0x0B] = gb_cpu_op_dec_bc;
    opcode_function_table[0x1B] = gb_cpu_op_dec_de;
    opcode_function_table[0x2B] = gb_cpu_op_dec_hl;
    opcode_function_table[0x3B] = gb_cpu_op_dec_sp;
    opcode_function_table[0xE8] = gb_cpu_op_add_sp_e8;

    //from cpu_control_flow
    opcode_function_table[0x20] = gb_cpu_op_jr_nz_e8;
    opcode_function_table[0x30] = gb_cpu_op_jr_nc_e8;
    opcode_function_table[0xC0] = gb_cpu_op_ret_nz;
    opcode_function_table[0xD0] = gb_cpu_op_ret_nc;
    opcode_function_table[0xC2] = gb_cpu_op_jp_nz_a16;
    opcode_function_table[0xD2] = gb_cpu_op_jp_nc_a16;
    opcode_function_table[0xC3] = gb_cpu_op_jp_a16;
    opcode_function_table[0xC4] = gb_cpu_op_call_nz_a16;
    opcode_function_table[0xD4] = gb_cpu_op_call_nc_a16;
    opcode_function_table[0xC7] = gb_cpu_op_rst_00;   
    opcode_function_table[0xD7] = gb_cpu_op_rst_10;   
    opcode_function_table[0xE7] = gb_cpu_op_rst_20;   
    opcode_function_table[0xF7] = gb_cpu_op_rst_30;   
    opcode_function_table[0x18] = gb_cpu_op_jr_e8;
    opcode_function_table[0x28] = gb_cpu_op_jr_z_e8;
    opcode_function_table[0x38] = gb_cpu_op_jr_c_e8;
    opcode_function_table[0xC8] = gb_cpu_op_ret_z;
    opcode_function_table[0xD8] = gb_cpu_op_ret_c;
    opcode_function_table[0xC9] = gb_cpu_op_ret;
    opcode_function_table[0xD9] = gb_cpu_op_reti;
    opcode_function_table[0xE9] = gb_cpu_op_jp_hl;
    opcode_function_table[0xCA] = gb_cpu_op_jp_z_a16;
    opcode_function_table[0xDA] = gb_cpu_op_jp_c_a16;
    opcode_function_table[0xCC] = gb_cpu_op_call_z_a16;
    opcode_function_table[0xDC] = gb_cpu_op_call_c_a16;
    opcode_function_table[0xCD] = gb_cpu_op_call_a16;
    opcode_function_table[0xCF] = gb_cpu_op_rst_08;   
    opcode_function_table[0xDF] = gb_cpu_op_rst_18;   
    opcode_function_table[0xEF] = gb_cpu_op_rst_28;   
    opcode_function_table[0xFF] = gb_cpu_op_rst_38;   

    //from cpu_misc
    opcode_function_table[0x00] = gb_cpu_op_nop;
    opcode_function_table[0x10] = gb_cpu_op_stop;
    opcode_function_table[0x76] = gb_cpu_op_halt;
    opcode_function_table[0xCB] = gb_cpu_op_cb;
    opcode_function_table[0xF3] = gb_cpu_op_di;
    opcode_function_table[0xFB] = gb_cpu_op_ei;
    opcode_function_table[0x27] = gb_cpu_op_daa;
    opcode_function_table[0x37] = gb_cpu_op_scf;
    opcode_function_table[0x07] = gb_cpu_op_rlca;
    opcode_function_table[0x17] = gb_cpu_op_rla;
    opcode_function_table[0x0F] = gb_cpu_op_rrca;
    opcode_function_table[0x1F] = gb_cpu_op_rra;

    //from misc (NOT YET IMPLEMENTED)
    //opcode_function_table[0x76] = gb_cpu_op_ld_halt;
    gb_cpu_init_extended_instructions();
}

void gb_cpu_init_extended_instructions() {
    for (unsigned i = 0; i < 256; i++) {
        opcode_function_table_cb[i] = NULL;
    }
    for (uint8_t i = 0; i < 8; i++) {
        opcode_function_table_cb[0x00 + i] = gb_cpu_cb_rlc;
    }
    for (uint8_t i = 0; i < 8; i++) {
        opcode_function_table_cb[0x08 + i] = gb_cpu_cb_rrc;
    }
    for (uint8_t i = 0; i < 8; i++) {
        opcode_function_table_cb[0x10 + i] = gb_cpu_cb_rl;
    }
    for (uint8_t i = 0; i < 8; i++) {
        opcode_function_table_cb[0x18 + i] = gb_cpu_cb_rr;
    }
    for (uint8_t i = 0; i < 8; i++) {
        opcode_function_table_cb[0x20 + i] = gb_cpu_cb_sla;
    }
    for (uint8_t i = 0; i < 8; i++) {
        opcode_function_table_cb[0x28 + i] = gb_cpu_cb_sra;
    }
    for (uint8_t i = 0; i < 8; i++) {
        opcode_function_table_cb[0x30 + i] = gb_cpu_cb_swap;
    }
    for (uint8_t i = 0; i < 8; i++) {
        opcode_function_table_cb[0x38 + i] = gb_cpu_cb_srl;
    }
    for (uint8_t i = 0; i < 16 * 4; i++) {
        opcode_function_table_cb[0x40 + i] = gb_cpu_cb_bit;
    }
    for (uint8_t i = 0; i < 16 * 4; i++) {
        opcode_function_table_cb[0x80 + i] = gb_cpu_cb_res;
    }
    for (uint8_t i = 0; i < 16 * 4; i++) {
        opcode_function_table_cb[0xC0 + i] = gb_cpu_cb_set;
    }
}

void gb_cpu_delete(gb_cpu* cpu) {
    free(cpu->cpu_register);
    cpu->cpu_register = NULL;
    free(cpu);
    cpu = NULL;
}
