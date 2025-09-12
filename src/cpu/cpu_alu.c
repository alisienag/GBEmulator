#include "../include/cpu/cpu_alu.h"
#include "cpu.h"
#include "cpu/cpu_flags.h"

#define GB_OPCODE_DECODE_REG_A 0b00000111
#define GB_OPCODE_DECODE_REG_B 0b00000000
#define GB_OPCODE_DECODE_REG_C 0b00000001
#define GB_OPCODE_DECODE_REG_D 0b00000010
#define GB_OPCODE_DECODE_REG_E 0b00000011
#define GB_OPCODE_DECODE_REG_H 0b00000100
#define GB_OPCODE_DECODE_REG_L 0b00000101
#define GB_OPCODE_DECODE_REG_HL 0b00000110

GB_CPU_OP(gb_cpu_op_inc_r) {
    uint8_t opcode = GB_READ_8(REG_PC-1);
    uint8_t* reg_ptr = NULL;
    switch ((opcode & 0xF0) >> 8) {
        case 0x0: {
            if ((opcode & 0xF) == 0x4) {
                reg_ptr = &REG_B;
            } else {
                reg_ptr = &REG_C;
            }
            break;
        }
        case 0x1: {
            if ((opcode & 0xF) == 0x4) {
                reg_ptr = &REG_D;
            } else {
                reg_ptr = &REG_E;
            }
            break;
        }
        case 0x2: {
            if ((opcode & 0xF) == 0x4) {
                reg_ptr = &REG_H;
            } else {
                reg_ptr = &REG_L;
            }
            break;
        }
        case 0x3: {
            if ((opcode & 0xC) == 0x4) {
                reg_ptr = &REG_A;
            }
            break;
        }
    }
    if (reg_ptr == NULL) {
        uint8_t value_hl = GB_READ_8(REG_HL);
        GB_FLAG_TEST_Z(value_hl + 1);
        GB_FLAG_CLEAR(FLAG_N);
        GB_FLAG_TEST_ADD_H_8(value_hl, 1);
        GB_WRITE_8(REG_HL, value_hl + 1);
        GB_CYCLES(12);
    } else {
        GB_FLAG_TEST_Z(*reg_ptr + 1);
        GB_FLAG_CLEAR(FLAG_N);
        GB_FLAG_TEST_ADD_H_8(*reg_ptr, 1);
        *reg_ptr += 1;
        GB_CYCLES(4);
    }
}

GB_CPU_OP(gb_cpu_op_dec_r) {
    uint8_t opcode = GB_READ_8(REG_PC-1);
    uint8_t* reg_ptr = NULL;
    switch ((opcode & 0xF0) >> 8) {
        case 0x0: {
            if ((opcode & 0xF) == 0x4) {
                reg_ptr = &REG_B;
            } else {
                reg_ptr = &REG_C;
            }
            break;
        }
        case 0x1: {
            if ((opcode & 0xF) == 0x4) {
                reg_ptr = &REG_D;
            } else {
                reg_ptr = &REG_E;
            }
            break;
        }
        case 0x2: {
            if ((opcode & 0xF) == 0x4) {
                reg_ptr = &REG_H;
            } else {
                reg_ptr = &REG_L;
            }
            break;
        }
        case 0x3: {
            if ((opcode & 0xC) == 0x4) {
                reg_ptr = &REG_A;
            }
            break;
        }
    }
    if (reg_ptr == NULL) {
        uint8_t value_hl = GB_READ_8(REG_HL);
        GB_FLAG_TEST_Z(value_hl - 1);
        GB_FLAG_SET(FLAG_N);
        GB_FLAG_TEST_SUB_H_8(value_hl, 1);
        GB_WRITE_8(REG_HL, value_hl - 1);
        GB_CYCLES(12);
    } else {
        GB_FLAG_TEST_Z(*reg_ptr - 1);
        GB_FLAG_SET(FLAG_N);
        GB_FLAG_TEST_SUB_H_8(*reg_ptr, 1);
        *reg_ptr -= 1;
        GB_CYCLES(4);
    }
}

GB_CPU_OP(gb_cpu_op_cpl) {
    REG_A = ~REG_A;
    GB_FLAG_SET(FLAG_N);
    GB_FLAG_SET(FLAG_H);

    GB_CYCLES(4);
}

GB_CPU_OP(gb_cpu_op_ccf) {
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);
    if (GB_FLAG_BIT(FLAG_C) == 0) {
        GB_FLAG_SET(FLAG_C);
    } else {
        GB_FLAG_CLEAR(FLAG_C);
    }

    GB_CYCLES(4);
}

GB_CPU_OP(gb_cpu_op_add_a_r) {
    uint8_t opcode = GB_READ_8(REG_PC-1);
    uint8_t augend = REG_A;
    uint8_t addend = 0;

    switch (GB_CPU_ALU_WHICH_REG(opcode)) {
        case GB_OPCODE_DECODE_REG_A: {
            addend = REG_A;
            break;
        }
        case GB_OPCODE_DECODE_REG_B: {
            addend = REG_B;
            break;
        }
        case GB_OPCODE_DECODE_REG_C: {
            addend = REG_C;
            break;
        }
        case GB_OPCODE_DECODE_REG_D: {
            addend = REG_D;
            break;
        }
        case GB_OPCODE_DECODE_REG_E: {
            addend = REG_E;
            break;
        }
        case GB_OPCODE_DECODE_REG_H: {
            addend = REG_H;
            break;
        }
        case GB_OPCODE_DECODE_REG_L: {
            addend = REG_L;
            break;
        }
        case GB_OPCODE_DECODE_REG_HL: {
            addend = GB_READ_8(REG_HL);
            GB_CYCLES(4);
            break;
        }
    }

    GB_FLAG_TEST_Z(augend + addend);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_TEST_ADD_H_8(augend, addend);
    GB_FLAG_TEST_ADD_C_8(augend, addend);

    REG_A = (uint8_t)(augend + addend);

    GB_CYCLES(4);
}

GB_CPU_OP(gb_cpu_op_adc_a_r) {
    uint8_t opcode = GB_READ_8(REG_PC-1);
    uint8_t augend = REG_A;
    uint8_t addend = 0;

    switch (GB_CPU_ALU_WHICH_REG(opcode)) {
        case GB_OPCODE_DECODE_REG_A: {
            addend = REG_A;
            break;
        }
        case GB_OPCODE_DECODE_REG_B: {
            addend = REG_B;
            break;
        }
        case GB_OPCODE_DECODE_REG_C: {
            addend = REG_C;
            break;
        }
        case GB_OPCODE_DECODE_REG_D: {
            addend = REG_D;
            break;
        }
        case GB_OPCODE_DECODE_REG_E: {
            addend = REG_E;
            break;
        }
        case GB_OPCODE_DECODE_REG_H: {
            addend = REG_H;
            break;
        }
        case GB_OPCODE_DECODE_REG_L: {
            addend = REG_L;
            break;
        }
        case GB_OPCODE_DECODE_REG_HL: {
            addend = GB_READ_8(REG_HL);
            GB_CYCLES(4);
            break;
        }
    }

    GB_FLAG_TEST_Z(augend + addend + GB_FLAG_BIT(FLAG_C));
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_TEST_ADC_H_8(augend, addend, GB_FLAG_BIT(FLAG_C));
    GB_FLAG_TEST_ADC_C_8(augend, addend, GB_FLAG_BIT(FLAG_C));
    REG_A = (uint8_t)(augend + addend + GB_FLAG_BIT(FLAG_C));

    GB_CYCLES(4);
}

GB_CPU_OP(gb_cpu_op_sub_a_r) {
    uint8_t opcode = GB_READ_8(REG_PC-1);
    uint8_t augend = REG_A;
    uint8_t addend = 0;

    switch (GB_CPU_ALU_WHICH_REG(opcode)) {
        case GB_OPCODE_DECODE_REG_A: {
            addend = REG_A;
            break;
        }
        case GB_OPCODE_DECODE_REG_B: {
            addend = REG_B;
            break;
        }
        case GB_OPCODE_DECODE_REG_C: {
            addend = REG_C;
            break;
        }
        case GB_OPCODE_DECODE_REG_D: {
            addend = REG_D;
            break;
        }
        case GB_OPCODE_DECODE_REG_E: {
            addend = REG_E;
            break;
        }
        case GB_OPCODE_DECODE_REG_H: {
            addend = REG_H;
            break;
        }
        case GB_OPCODE_DECODE_REG_L: {
            addend = REG_L;
            break;
        }
        case GB_OPCODE_DECODE_REG_HL: {
            addend = GB_READ_8(REG_HL);
            GB_CYCLES(4);
            break;
        }
    }

    GB_FLAG_TEST_Z(augend + addend);
    GB_FLAG_SET(FLAG_N);
    GB_FLAG_TEST_SUB_H_8(augend, addend);
    GB_FLAG_TEST_SUB_C_8(augend, addend);

    REG_A = (uint8_t)(augend + addend);

    GB_CYCLES(4);
}

GB_CPU_OP(gb_cpu_op_sbc_a_r) {
    uint8_t opcode = GB_READ_8(REG_PC-1);
    uint8_t augend = REG_A;
    uint8_t addend = 0;
    switch (GB_CPU_ALU_WHICH_REG(opcode)) {
        case GB_OPCODE_DECODE_REG_A: {
            addend = REG_A;
            break;
        }
        case GB_OPCODE_DECODE_REG_B: {
            addend = REG_B;
            break;
        }
        case GB_OPCODE_DECODE_REG_C: {
            addend = REG_C;
            break;
        }
        case GB_OPCODE_DECODE_REG_D: {
            addend = REG_D;
            break;
        }
        case GB_OPCODE_DECODE_REG_E: {
            addend = REG_E;
            break;
        }
        case GB_OPCODE_DECODE_REG_H: {
            addend = REG_H;
            break;
        }
        case GB_OPCODE_DECODE_REG_L: {
            addend = REG_L;
            break;
        }
        case GB_OPCODE_DECODE_REG_HL: {
            addend = GB_READ_8(REG_HL);
            GB_CYCLES(4);
            break;
        }
    }

    GB_FLAG_TEST_Z(augend - addend - GB_FLAG_BIT(FLAG_C));
    GB_FLAG_SET(FLAG_N);
    GB_FLAG_TEST_SBC_H_8(augend, addend, GB_FLAG_BIT(FLAG_C));
    GB_FLAG_TEST_SBC_C_8(augend, addend, GB_FLAG_BIT(FLAG_C));

    REG_A = (uint8_t)(augend - addend - GB_FLAG_BIT(FLAG_C));

    GB_CYCLES(4);
}

GB_CPU_OP(gb_cpu_op_and_a_r) {
    uint8_t opcode = GB_READ_8(REG_PC-1);
    uint8_t reg_a = REG_A;
    uint8_t reg_r = 0;

    switch (GB_CPU_ALU_WHICH_REG(opcode)) {
        case GB_OPCODE_DECODE_REG_A: {
            reg_r = REG_A;
            break;
        }
        case GB_OPCODE_DECODE_REG_B: {
            reg_r = REG_B;
            break;
        }
        case GB_OPCODE_DECODE_REG_C: {
            reg_r = REG_C;
            break;
        }
        case GB_OPCODE_DECODE_REG_D: {
            reg_r = REG_D;
            break;
        }
        case GB_OPCODE_DECODE_REG_E: {
            reg_r = REG_E;
            break;
        }
        case GB_OPCODE_DECODE_REG_H: {
            reg_r = REG_H;
            break;
        }
        case GB_OPCODE_DECODE_REG_L: {
            reg_r = REG_L;
            break;
        }
        case GB_OPCODE_DECODE_REG_HL: {
            reg_r = GB_READ_8(REG_HL);
            GB_CYCLES(4);
            break;
        }
    }

    REG_A &= reg_r;

    GB_FLAG_TEST_Z(reg_a);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_SET(FLAG_H);
    GB_FLAG_CLEAR(FLAG_C);

    GB_CYCLES(4);
}

GB_CPU_OP(gb_cpu_op_xor_a_r) {
    uint8_t opcode = GB_READ_8(REG_PC-1);
    uint8_t reg_r = 0;

    switch (GB_CPU_ALU_WHICH_REG(opcode)) {
        case GB_OPCODE_DECODE_REG_A: {
            reg_r = REG_A;
            break;
        }
        case GB_OPCODE_DECODE_REG_B: {
            reg_r = REG_B;
            break;
        }
        case GB_OPCODE_DECODE_REG_C: {
            reg_r = REG_C;
            break;
        }
        case GB_OPCODE_DECODE_REG_D: {
            reg_r = REG_D;
            break;
        }
        case GB_OPCODE_DECODE_REG_E: {
            reg_r = REG_E;
            break;
        }
        case GB_OPCODE_DECODE_REG_H: {
            reg_r = REG_H;
            break;
        }
        case GB_OPCODE_DECODE_REG_L: {
            reg_r = REG_L;
            break;
        }
        case GB_OPCODE_DECODE_REG_HL: {
            reg_r = GB_READ_8(REG_HL);
            GB_CYCLES(4);
            break;
        }
    }

    REG_A ^= reg_r;

    GB_FLAG_TEST_Z(REG_A);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);
    GB_FLAG_CLEAR(FLAG_C);

    GB_CYCLES(4);
}

GB_CPU_OP(gb_cpu_op_or_a_r) {
    uint8_t opcode = GB_READ_8(REG_PC-1);
    uint8_t reg_r = 0;

    switch (GB_CPU_ALU_WHICH_REG(opcode)) {
        case GB_OPCODE_DECODE_REG_A: {
            reg_r = REG_A;
            break;
        }
        case GB_OPCODE_DECODE_REG_B: {
            reg_r = REG_B;
            break;
        }
        case GB_OPCODE_DECODE_REG_C: {
            reg_r = REG_C;
            break;
        }
        case GB_OPCODE_DECODE_REG_D: {
            reg_r = REG_D;
            break;
        }
        case GB_OPCODE_DECODE_REG_E: {
            reg_r = REG_E;
            break;
        }
        case GB_OPCODE_DECODE_REG_H: {
            reg_r = REG_H;
            break;
        }
        case GB_OPCODE_DECODE_REG_L: {
            reg_r = REG_L;
            break;
        }
        case GB_OPCODE_DECODE_REG_HL: {
            reg_r = GB_READ_8(REG_HL);
            GB_CYCLES(4);
            break;
        }
    }

    REG_A |= reg_r;

    GB_FLAG_TEST_Z(REG_A);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);
    GB_FLAG_CLEAR(FLAG_C);

    GB_CYCLES(4);
}

GB_CPU_OP(gb_cpu_op_cp_a_r) {
    uint8_t opcode = GB_READ_8(REG_PC-1);
    uint8_t reg_r = 0;

    switch (GB_CPU_ALU_WHICH_REG(opcode)) {
        case GB_OPCODE_DECODE_REG_A: {
            reg_r = REG_A;
            break;
        }
        case GB_OPCODE_DECODE_REG_B: {
            reg_r = REG_B;
            break;
        }
        case GB_OPCODE_DECODE_REG_C: {
            reg_r = REG_C;
            break;
        }
        case GB_OPCODE_DECODE_REG_D: {
            reg_r = REG_D;
            break;
        }
        case GB_OPCODE_DECODE_REG_E: {
            reg_r = REG_E;
            break;
        }
        case GB_OPCODE_DECODE_REG_H: {
            reg_r = REG_H;
            break;
        }
        case GB_OPCODE_DECODE_REG_L: {
            reg_r = REG_L;
            break;
        }
        case GB_OPCODE_DECODE_REG_HL: {
            reg_r = GB_READ_8(REG_HL);
            GB_CYCLES(4);
            break;
        }
    }

    GB_FLAG_TEST_Z(REG_A - reg_r);
    GB_FLAG_SET(FLAG_N);
    GB_FLAG_TEST_SUB_H_8(REG_A, reg_r);
    GB_FLAG_TEST_SUB_C_8(REG_A, reg_r);

    GB_CYCLES(4);
}

GB_CPU_OP(gb_cpu_op_add_a_n8) {
    uint8_t n8 = GB_READ_8(REG_PC);
    uint16_t result = REG_A + n8;

    GB_FLAG_TEST_Z((uint8_t)result);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_TEST_ADD_H_8(REG_A, n8);
    GB_FLAG_TEST_ADD_C_8(REG_A, n8);

    REG_A = (uint8_t)result;
    REG_PC++;

    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_sub_a_n8) {
    uint8_t n8 = GB_READ_8(REG_PC);
    uint16_t result = REG_A - n8;

    GB_FLAG_TEST_Z((uint8_t)result);
    GB_FLAG_SET(FLAG_N);
    GB_FLAG_TEST_SUB_H_8(REG_A, n8);
    GB_FLAG_TEST_SUB_C_8(REG_A, n8);
    
    REG_A = (uint8_t)result;
    REG_PC++;

    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_and_a_n8) {
    uint8_t n8 = GB_READ_8(REG_PC);
    uint8_t result = REG_A &= n8;

    GB_FLAG_TEST_Z(result);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_SET(FLAG_H);
    GB_FLAG_CLEAR(FLAG_C);

    REG_A = result;
    REG_PC++;

    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_or_a_n8) {
    uint8_t n8 = GB_READ_8(REG_PC);
    uint8_t result = REG_A |= n8;

    GB_FLAG_TEST_Z(result);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);
    GB_FLAG_CLEAR(FLAG_C);

    REG_A = result;
    REG_PC++;

    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_adc_a_n8) {
    uint8_t n8 = GB_READ_8(REG_PC);
    uint16_t result = REG_A + n8 + GB_FLAG_BIT(FLAG_C);

    GB_FLAG_TEST_Z((uint8_t)result);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_TEST_ADC_H_8(REG_A, n8, GB_FLAG_BIT(FLAG_C));
    GB_FLAG_TEST_ADC_C_8(REG_A, n8, GB_FLAG_BIT(FLAG_C));

    REG_A = (uint8_t)(result);
    REG_PC++;

    GB_CYCLES(8);
}
GB_CPU_OP(gb_cpu_op_sbc_a_n8) {
    uint8_t n8 = GB_READ_8(REG_PC);
    uint16_t result = REG_A - n8 - GB_FLAG_BIT(FLAG_C);
    GB_FLAG_TEST_Z((uint8_t)result);
    GB_FLAG_SET(FLAG_N);
    GB_FLAG_TEST_SBC_H_8(REG_A, n8, GB_FLAG_BIT(FLAG_C));
    GB_FLAG_TEST_SBC_C_8(REG_A, n8, GB_FLAG_BIT(FLAG_C));

    REG_A = (uint8_t)(REG_A - n8 - GB_FLAG_BIT(FLAG_C));
    REG_PC++;

    GB_CYCLES(8);
}
GB_CPU_OP(gb_cpu_op_xor_a_n8) {
    uint8_t n8 = GB_READ_8(REG_PC);

    uint8_t result = REG_A ^= n8;

    GB_FLAG_TEST_Z(result);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);
    GB_FLAG_CLEAR(FLAG_C);

    REG_A = result;
    REG_PC++;

    GB_CYCLES(8);
}
GB_CPU_OP(gb_cpu_op_cp_a_n8) {
    uint8_t n8 = GB_READ_8(REG_PC);

    GB_FLAG_TEST_Z(REG_A - n8);
    GB_FLAG_SET(FLAG_N);
    GB_FLAG_TEST_SUB_H_8(REG_A, n8);
    GB_FLAG_TEST_SUB_C_8(REG_A, n8);

    REG_PC++;

    GB_CYCLES(8);
}

static inline uint8_t GB_CPU_ALU_WHICH_REG(uint8_t x) {
    x = x & 0b00000111;   
    switch (x) {
        case GB_OPCODE_DECODE_REG_A:
            return GB_OPCODE_DECODE_REG_A;
        case GB_OPCODE_DECODE_REG_B:
            return GB_OPCODE_DECODE_REG_B;
        case GB_OPCODE_DECODE_REG_C:
            return GB_OPCODE_DECODE_REG_C;
        case GB_OPCODE_DECODE_REG_D:
            return GB_OPCODE_DECODE_REG_D;
        case GB_OPCODE_DECODE_REG_E:
            return GB_OPCODE_DECODE_REG_E;
        case GB_OPCODE_DECODE_REG_H:
            return GB_OPCODE_DECODE_REG_H;
        case GB_OPCODE_DECODE_REG_L:
            return GB_OPCODE_DECODE_REG_L;
        case GB_OPCODE_DECODE_REG_HL:
            return GB_OPCODE_DECODE_REG_HL;
        default: {
            printf("ERROR::GB_CPU_ALU_WHICH_REG::DEFAULT CASE\n");
            return 0xFF;
        }
    }
}
