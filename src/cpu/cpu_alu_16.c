#include "cpu/cpu_alu_16.h"
#include "cpu.h"
#include "cpu/cpu_flags.h"

GB_CPU_OP(GB_CPU_OP_INC_BC) {
    REG_BC = REG_BC + 1;
    GB_CYCLES(8);
}

GB_CPU_OP(GB_CPU_OP_INC_DE) {
    REG_DE = REG_DE + 1;
    GB_CYCLES(8);
}

GB_CPU_OP(GB_CPU_OP_INC_HL) {
    REG_HL = REG_HL + 1;
    GB_CYCLES(8);
}
GB_CPU_OP(GB_CPU_OP_INC_SP) {
    REG_SP = REG_SP + 1;
    GB_CYCLES(8);
}

GB_CPU_OP(GB_CPU_OP_ADD_HL_BC) {
    uint16_t result = REG_HL + REG_BC;

    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_TEST_ADD_H_16(REG_HL, REG_BC);
    GB_FLAG_TEST_ADD_C_16(REG_HL, REG_BC);

    REG_HL = result;

    GB_CYCLES(8);
}

GB_CPU_OP(GB_CPU_OP_ADD_HL_DE) {
    uint16_t result = REG_HL + REG_DE;

    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_TEST_ADD_H_16(REG_HL, REG_DE);
    GB_FLAG_TEST_ADD_C_16(REG_HL, REG_DE);

    REG_HL = result;

    GB_CYCLES(8);
}

GB_CPU_OP(GB_CPU_OP_ADD_HL_HL) {
    
    uint16_t result = REG_HL + REG_HL;

    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_TEST_ADD_H_16(REG_HL, REG_HL);
    GB_FLAG_TEST_ADD_C_16(REG_HL, REG_HL);

    REG_HL = result;

    GB_CYCLES(8);
}
    
GB_CPU_OP(GB_CPU_OP_ADD_HL_SP) {
    uint16_t result = REG_HL + REG_SP;

    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_TEST_ADD_H_16(REG_HL, REG_SP);
    GB_FLAG_TEST_ADD_C_16(REG_HL, REG_SP);

    REG_HL = result;

    GB_CYCLES(8);
}

GB_CPU_OP(GB_CPU_OP_DEC_BC) {
    REG_BC = REG_BC - 1;
    GB_CYCLES(8);
}
GB_CPU_OP(GB_CPU_OP_DEC_DE) {
    REG_DE = REG_DE - 1;
    GB_CYCLES(8);
}
GB_CPU_OP(GB_CPU_OP_DEC_HL) {
    REG_HL = REG_HL - 1;
    GB_CYCLES(8);
}
GB_CPU_OP(GB_CPU_OP_DEC_SP) {
    REG_SP = REG_SP - 1;
    GB_CYCLES(8);
}

GB_CPU_OP(GB_CPU_OP_ADD_SP_e8) {
    int8_t e8 = (int8_t)GB_READ_8(REG_PC);

    GB_FLAG_CLEAR(FLAG_Z);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_TEST_ADD_H_8((uint8_t)REG_SP, (uint8_t)e8);
    GB_FLAG_TEST_ADD_C_8((uint8_t)REG_SP, (uint8_t)e8);

    REG_SP = REG_SP + e8;
    REG_PC++;

    GB_CYCLES(16);
}
