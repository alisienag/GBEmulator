#include "cpu/cpu_load_16.h"
#include "cpu.h"
#include "cpu/cpu_flags.h"

GB_CPU_OP(gb_cpu_op_ld_bc_n16) {
    uint16_t value = GB_READ_16(REG_PC);
    REG_PC += 2;

    REG_BC = value;
    GB_CYCLES(12);
}

GB_CPU_OP(gb_cpu_op_ld_de_n16) {
    uint16_t value = GB_READ_16(REG_PC);
    REG_PC += 2;

    REG_DE = value;
    GB_CYCLES(12);
}

GB_CPU_OP(gb_cpu_op_ld_hl_n16) {
    uint16_t value = GB_READ_16(REG_PC);
    REG_PC += 2;

    REG_HL = value;
    GB_CYCLES(12);
}

GB_CPU_OP(gb_cpu_op_ld_sp_n16) {
    uint16_t value = GB_READ_16(REG_PC);
    REG_PC += 2;

    REG_SP = value;
    GB_CYCLES(12);
}

GB_CPU_OP(gb_cpu_op_pop_bc) {
    uint16_t value = GB_READ_16(REG_SP);
    REG_SP += 2;

    REG_BC = value;
    GB_CYCLES(12);
}

GB_CPU_OP(gb_cpu_op_pop_de) {
    uint16_t value = GB_READ_16(REG_SP);
    REG_SP += 2;

    REG_DE = value;
    GB_CYCLES(12);
}

GB_CPU_OP(gb_cpu_op_pop_hl) {
    uint16_t value = GB_READ_16(REG_SP);
    REG_SP += 2;

    REG_HL = value;
    GB_CYCLES(12);
}

GB_CPU_OP(gb_cpu_op_pop_af) {
    uint16_t value = GB_READ_16(REG_SP);
    REG_SP += 2;

    if ((value & 0xF) != 0) {
        printf("Wow, garbage bit found during pop af\n");
    }

    REG_AF = value & 0xFFF0;
    GB_CYCLES(12);
}

GB_CPU_OP(gb_cpu_op_push_bc) {
    REG_SP -= 2;
    GB_WRITE_16(REG_SP, REG_BC);

    GB_CYCLES(16);
}

GB_CPU_OP(gb_cpu_op_push_de) {
    REG_SP -= 2;
    GB_WRITE_16(REG_SP, REG_DE);

    GB_CYCLES(16);
}

GB_CPU_OP(gb_cpu_op_push_hl) {
    REG_SP -= 2;
    GB_WRITE_16(REG_SP, REG_HL);

    GB_CYCLES(16);
}

GB_CPU_OP(gb_cpu_op_push_af) {
    REG_SP -= 2;
    GB_WRITE_16(REG_SP, REG_AF & 0xFF0);

    GB_CYCLES(16);
}

GB_CPU_OP(gb_cpu_op_ld_a16_sp) {
    GB_WRITE_16(GB_READ_16(REG_PC), REG_SP);
    REG_PC += 2;

    GB_CYCLES(20);
}

GB_CPU_OP(gb_cpu_op_ld_hl_sp_e8) {
    uint8_t e8 = GB_READ_8(REG_PC);
    REG_PC++;

    GB_FLAG_CLEAR(FLAG_Z);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_TEST_ADD_H_8((uint8_t)REG_SP, e8);
    GB_FLAG_TEST_ADD_C_8((uint8_t)REG_SP, e8);
    
    REG_HL = REG_SP + e8;
    GB_CYCLES(12);
}

GB_CPU_OP(gb_cpu_op_ld_sp_hl) {
    REG_SP = REG_HL;
    GB_CYCLES(8);
}
