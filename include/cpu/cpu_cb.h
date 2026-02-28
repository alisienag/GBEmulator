#ifndef _CPU_CB
#define _CPU_CB

#include "../cpu.h"
#include "../cpu/cpu_flags.h"
#include "../cpu/cpu_alu.h"

uint8_t GB_OPCODE_DECODE_BIT(uint8_t opcode);

GB_CPU_OP(gb_cpu_cb_rlc);
GB_CPU_OP(gb_cpu_cb_rrc);
GB_CPU_OP(gb_cpu_cb_rl);
GB_CPU_OP(gb_cpu_cb_rr);
GB_CPU_OP(gb_cpu_cb_sla);
GB_CPU_OP(gb_cpu_cb_sra);
GB_CPU_OP(gb_cpu_cb_swap);
GB_CPU_OP(gb_cpu_cb_srl);
GB_CPU_OP(gb_cpu_cb_bit);
GB_CPU_OP(gb_cpu_cb_res);
GB_CPU_OP(gb_cpu_cb_set);
#endif
