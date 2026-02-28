#include "cpu/cpu_misc.h"
#include "cpu.h"
#include <stdlib.h>

GB_CPU_OP(gb_cpu_op_nop) {
    GB_CYCLES(4);
}
GB_CPU_OP(gb_cpu_op_stop) {
    REG_PC++;
    GB_CYCLES(4);
}
GB_CPU_OP(gb_cpu_op_halt) {
    GB_CYCLES(4);
}
GB_CPU_OP(gb_cpu_op_cb) {
    uint8_t opcode = GB_READ_8(REG_PC);
    REG_PC++;
    if(opcode_function_table_cb[opcode] != NULL) {
        //printf("EXECUTING CB INSTRUCTION: 0x%02X\n", opcode);
        opcode_function_table_cb[opcode](cpu, memory);
    } else {
        printf("EXECUTING UNWRITTEN INSTRUCTION: 0x%02X\n", opcode);
        exit(2);
    }
}
GB_CPU_OP(gb_cpu_op_di) {
    GB_IME_DISABLE();
    GB_CYCLES(4);
}
GB_CPU_OP(gb_cpu_op_ei) {
    GB_IME_ENABLE();
    GB_CYCLES(4);
}

GB_CPU_OP(gb_cpu_op_daa) {
    uint8_t a = REG_A;
    uint8_t correction = 0;
    int carry = 0;

    if (!GB_FLAG_BIT(FLAG_N)) {
        if (GB_FLAG_BIT(FLAG_H) || (a & 0x0F) > 9) {
            correction |= 0x06;
        }
        if (GB_FLAG_BIT(FLAG_C) || a > 0x99) {
            correction |= 0x60;
            carry = 1;
        }
        a += correction;
    } else {
        if (GB_FLAG_BIT(FLAG_H)) {
            correction |= 0x06;
        }
        if (GB_FLAG_BIT(FLAG_C)) {
            correction |= 0x60;
        }
        a -= correction;
    }

    REG_A = a;
    GB_FLAG_CLEAR(FLAG_H);
    if (carry) {
        GB_FLAG_SET(FLAG_C);
    }
    GB_FLAG_TEST_Z(REG_A);

    GB_CYCLES(4);
}

GB_CPU_OP(gb_cpu_op_scf) {
    CLEAR_FLAG(FLAG_N);
    CLEAR_FLAG(FLAG_H);
    SET_FLAG(FLAG_C);
    GB_CYCLES(4);
}

// NON CB Bit Operations

GB_CPU_OP(gb_cpu_op_rlca) {
    uint8_t original = REG_A;
    uint8_t done = original << 1;
    if (original & 0b10000000) {
        done += 1;
        GB_FLAG_SET(FLAG_C);
    } else {
        GB_FLAG_CLEAR(FLAG_C);
    }
    REG_A = done;
    GB_FLAG_CLEAR(FLAG_Z);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);
    GB_CYCLES(4);
}
GB_CPU_OP(gb_cpu_op_rla) {
    uint8_t original = REG_A;
    uint8_t done = original << 1;
    if (GB_FLAG_BIT(FLAG_C)) {
        done += 1;
    }
    if (original & 0b10000000) {
        GB_FLAG_SET(FLAG_C);
    } else {
        GB_FLAG_CLEAR(FLAG_C);
    }
    REG_A = done;
    GB_FLAG_CLEAR(FLAG_Z);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);
    GB_CYCLES(4);
}
GB_CPU_OP(gb_cpu_op_rrca) {
    uint8_t original = REG_A;
    uint8_t done = original >> 1;
    if (original & 0b00000001) {
        done |= 0b10000000;
        GB_FLAG_SET(FLAG_C);
    } else {
        GB_FLAG_CLEAR(FLAG_C);
    }
    REG_A = done;
    GB_FLAG_CLEAR(FLAG_Z);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);
    GB_CYCLES(4);
}
GB_CPU_OP(gb_cpu_op_rra) {
    uint8_t original = REG_A;
    uint8_t done = original >> 1;
    if (GB_FLAG_BIT(FLAG_C)) {
        done |= 0b10000000;
    }
    if (original & 0b00000001) {
        GB_FLAG_SET(FLAG_C);
    } else {
        GB_FLAG_CLEAR(FLAG_C);
    }
    REG_A = done;
    GB_FLAG_CLEAR(FLAG_Z);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);
    GB_CYCLES(4);
}
