#ifndef _CPU_MISC
#define _CPU_MISC

#include "cpu.h"
#include "cpu/cpu_flags.h"

GB_CPU_OP(gb_cpu_op_nop);
GB_CPU_OP(gb_cpu_op_stop);
GB_CPU_OP(gb_cpu_op_halt);
GB_CPU_OP(gb_cpu_op_cb);
GB_CPU_OP(gb_cpu_op_di);
GB_CPU_OP(gb_cpu_op_ei);
GB_CPU_OP(gb_cpu_op_daa);
GB_CPU_OP(gb_cpu_op_scf);
// NON CB Bit Operations
GB_CPU_OP(gb_cpu_op_rlca);
GB_CPU_OP(gb_cpu_op_rla);
GB_CPU_OP(gb_cpu_op_rrca);
GB_CPU_OP(gb_cpu_op_rra);
#endif
