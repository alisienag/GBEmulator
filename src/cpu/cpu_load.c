#include <cpu/cpu_load.h>


GB_CPU_OP(gb_cpu_op_ld_bc_a) {
    GB_WRITE_8(REG_BC, REG_A);
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_ld_b_d8) {
    REG_B = GB_READ_8(REG_PC);
    REG_PC++;
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_ld_a_bc) {
    REG_A = GB_READ_8(REG_BC);
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_ld_c_d8) {
    REG_C = GB_READ_8(REG_PC);
    REG_PC++;
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_ld_de_a) {
    GB_WRITE_8(REG_DE, REG_A);
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_ld_d_d8) {
    REG_D = GB_READ_8(REG_PC);
    REG_PC++;
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_ld_a_de) {
    REG_A = GB_READ_8(REG_DE);
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_ld_e_d8) {
    REG_E = GB_READ_8(REG_PC);
    REG_PC++;
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_ld_hli_a) {
    GB_WRITE_8(REG_HL, REG_A);
    REG_HL++;
    GB_CYCLES(8);
}
