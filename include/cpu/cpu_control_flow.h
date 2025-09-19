#ifndef _H_CPU_CONTROL_FLOW
#define _H_CPU_CONTROL_FLOW

#include "cpu.h"
#include "cpu/cpu_flags.h"

GB_CPU_OP(gb_cpu_op_jr_nz_e8);
GB_CPU_OP(gb_cpu_op_jr_nc_e8);

GB_CPU_OP(gb_cpu_op_ret_nz);
GB_CPU_OP(gb_cpu_op_ret_nc);

GB_CPU_OP(gb_cpu_op_jp_nz_a16);
GB_CPU_OP(gb_cpu_op_jp_nc_a16);
GB_CPU_OP(gb_cpu_op_jp_a16);

GB_CPU_OP(gb_cpu_op_call_nz_a16);
GB_CPU_OP(gb_cpu_op_call_nc_a16);

GB_CPU_OP(gb_cpu_op_rst_00);
GB_CPU_OP(gb_cpu_op_rst_10);
GB_CPU_OP(gb_cpu_op_rst_20);
GB_CPU_OP(gb_cpu_op_rst_30);

GB_CPU_OP(gb_cpu_op_jr_e8);
GB_CPU_OP(gb_cpu_op_jr_z_e8);
GB_CPU_OP(gb_cpu_op_jr_c_e8);

GB_CPU_OP(gb_cpu_op_ret_z);
GB_CPU_OP(gb_cpu_op_ret_c);

GB_CPU_OP(gb_cpu_op_ret);
GB_CPU_OP(gb_cpu_op_reti);
GB_CPU_OP(gb_cpu_op_jp_hl);

GB_CPU_OP(gb_cpu_op_jp_z_a16);
GB_CPU_OP(gb_cpu_op_jp_c_a16);

GB_CPU_OP(gb_cpu_op_call_z_a16);
GB_CPU_OP(gb_cpu_op_call_c_a16);
GB_CPU_OP(gb_cpu_op_call_a16);

GB_CPU_OP(gb_cpu_op_rst_08);
GB_CPU_OP(gb_cpu_op_rst_18);
GB_CPU_OP(gb_cpu_op_rst_28);
GB_CPU_OP(gb_cpu_op_rst_38);

#endif
