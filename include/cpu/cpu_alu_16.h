#ifndef _H_CPU_ALU_16
#define _H_CPU_ALU_16


#include "../cpu.h"
#include "../cpu/cpu_flags.h"

GB_CPU_OP(gb_cpu_op_inc_bc);
GB_CPU_OP(gb_cpu_op_inc_de);
GB_CPU_OP(gb_cpu_op_inc_hl);
GB_CPU_OP(gb_cpu_op_inc_sp);

GB_CPU_OP(gb_cpu_op_add_hl_bc);
GB_CPU_OP(gb_cpu_op_add_hl_de);
GB_CPU_OP(gb_cpu_op_add_hl_hl);
GB_CPU_OP(gb_cpu_op_add_hl_sp);

GB_CPU_OP(gb_cpu_op_dec_bc);
GB_CPU_OP(gb_cpu_op_dec_de);
GB_CPU_OP(gb_cpu_op_dec_hl);
GB_CPU_OP(gb_cpu_op_dec_sp);

GB_CPU_OP(gb_cpu_op_add_sp_e8);

#endif
