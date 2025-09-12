#ifndef _CPU_ALU
#define _CPU_ALU

#include "../cpu.h"
#include "../cpu/cpu_flags.h"

static inline uint8_t GB_CPU_ALU_WHICH_REG(uint8_t x);

GB_CPU_OP(gb_cpu_op_inc_r);
GB_CPU_OP(gb_cpu_op_dec_r);

GB_CPU_OP(gb_cpu_op_cpl);
GB_CPU_OP(gb_cpu_op_ccf);

GB_CPU_OP(gb_cpu_op_add_a_r);
GB_CPU_OP(gb_cpu_op_adc_a_r);
GB_CPU_OP(gb_cpu_op_sub_a_r);
GB_CPU_OP(gb_cpu_op_sbc_a_r);
GB_CPU_OP(gb_cpu_op_and_a_r);
GB_CPU_OP(gb_cpu_op_xor_a_r);
GB_CPU_OP(gb_cpu_op_or_a_r);
GB_CPU_OP(gb_cpu_op_cp_a_r);

GB_CPU_OP(gb_cpu_op_add_a_n8);
GB_CPU_OP(gb_cpu_op_sub_a_n8);
GB_CPU_OP(gb_cpu_op_and_a_n8);
GB_CPU_OP(gb_cpu_op_or_a_n8);
GB_CPU_OP(gb_cpu_op_adc_a_n8);
GB_CPU_OP(gb_cpu_op_sbc_a_n8);
GB_CPU_OP(gb_cpu_op_xor_a_n8);
GB_CPU_OP(gb_cpu_op_cp_a_n8);
#endif
