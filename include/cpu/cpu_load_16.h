#ifndef _CPU_LOAD_16
#define _CPU_LOAD_16

#include "../cpu.h"

GB_CPU_OP(gb_cpu_op_ld_bc_n16);
GB_CPU_OP(gb_cpu_op_ld_de_n16);
GB_CPU_OP(gb_cpu_op_ld_hl_n16);
GB_CPU_OP(gb_cpu_op_ld_sp_n16);

GB_CPU_OP(gb_cpu_op_pop_bc);
GB_CPU_OP(gb_cpu_op_pop_de);
GB_CPU_OP(gb_cpu_op_pop_hl);
GB_CPU_OP(gb_cpu_op_pop_af);

GB_CPU_OP(gb_cpu_op_push_bc);
GB_CPU_OP(gb_cpu_op_push_de);
GB_CPU_OP(gb_cpu_op_push_hl);
GB_CPU_OP(gb_cpu_op_push_af);

GB_CPU_OP(gb_cpu_op_ld_a16_sp);
GB_CPU_OP(gb_cpu_op_ld_hl_sp_e8);
GB_CPU_OP(gb_cpu_op_ld_sp_hl);
#endif
