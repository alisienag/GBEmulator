#include "cpu/cpu_alu_16.h"
#include "cpu.h"
#include "cpu/cpu_flags.h"

GB_CPU_OP(gb_cpu_op_inc_bc) {
    REG_BC = REG_BC + 1;
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_inc_de) {
    REG_DE = REG_DE + 1;
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_inc_hl) {
    REG_HL = REG_HL + 1;
    GB_CYCLES(8);
}
GB_CPU_OP(gb_cpu_op_inc_sp) {
    REG_SP = REG_SP + 1;
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_add_hl_bc) {
    uint16_t result = REG_HL + REG_BC;

    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_TEST_ADD_H_16(REG_HL, REG_BC);
    GB_FLAG_TEST_ADD_C_16(REG_HL, REG_BC);

    REG_HL = result;

    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_add_hl_de) {
    uint16_t result = REG_HL + REG_DE;

    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_TEST_ADD_H_16(REG_HL, REG_DE);
    GB_FLAG_TEST_ADD_C_16(REG_HL, REG_DE);

    REG_HL = result;

    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_add_hl_hl) {
    
    uint16_t result = REG_HL + REG_HL;

    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_TEST_ADD_H_16(REG_HL, REG_HL);
    GB_FLAG_TEST_ADD_C_16(REG_HL, REG_HL);

    REG_HL = result;

    GB_CYCLES(8);
}
    
GB_CPU_OP(gb_cpu_op_add_hl_sp) {
    uint16_t result = REG_HL + REG_SP;

    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_TEST_ADD_H_16(REG_HL, REG_SP);
    GB_FLAG_TEST_ADD_C_16(REG_HL, REG_SP);

    REG_HL = result;

    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_dec_bc) {
    REG_BC = REG_BC - 1;
    GB_CYCLES(8);
}
GB_CPU_OP(gb_cpu_op_dec_de) {
    REG_DE = REG_DE - 1;
    GB_CYCLES(8);
}
GB_CPU_OP(gb_cpu_op_dec_hl) {
    REG_HL = REG_HL - 1;
    GB_CYCLES(8);
}
GB_CPU_OP(gb_cpu_op_dec_sp) {
    REG_SP = REG_SP - 1;
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_add_sp_e8) {
    int8_t e8 = (int8_t)GB_READ_8(REG_PC);

    GB_FLAG_CLEAR(FLAG_Z);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_TEST_ADD_H_8((uint8_t)REG_SP, (uint8_t)e8);
    GB_FLAG_TEST_ADD_C_8((uint8_t)REG_SP, (uint8_t)e8);

    REG_SP += e8;
    REG_PC++;

    GB_CYCLES(16);
}
