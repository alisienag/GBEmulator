#include "cpu/cpu_cb.h"
#include "cpu.h"
#include "cpu/cpu_alu.h"
#include "cpu/cpu_flags.h"

#define GB_OPCODE_DECODE_PTR(opcode, x) do {\
    switch(GB_CPU_ALU_WHICH_REG(opcode)) {\
        case GB_OPCODE_DECODE_REG_A:\
            x = &REG_A;\
            break;\
        case GB_OPCODE_DECODE_REG_B:\
            x = &REG_B;\
            break;\
        case GB_OPCODE_DECODE_REG_C:\
            x = &REG_C;\
            break;\
        case GB_OPCODE_DECODE_REG_D:\
            x = &REG_D;\
            break;\
        case GB_OPCODE_DECODE_REG_E:\
            x = &REG_E;\
            break;\
        case GB_OPCODE_DECODE_REG_H:\
            x = &REG_H;\
            break;\
        case GB_OPCODE_DECODE_REG_L:\
            x = &REG_L;\
            break;\
        case GB_OPCODE_DECODE_REG_HL:\
            x = NULL;\
            break;\
    }\
    } while (0)

uint8_t GB_OPCODE_DECODE_BIT(uint8_t opcode) {
    return (opcode >> 3) & 0b111;
}

GB_CPU_OP(gb_cpu_cb_rlc) {
    uint8_t opcode = GB_READ_8(REG_PC - 1);
    uint8_t* reg_ptr;
    uint8_t original;
    uint8_t done;
    GB_OPCODE_DECODE_PTR(opcode, reg_ptr);
    if (reg_ptr) {
        original = *reg_ptr;
        done = original << 1;
        if (original & 0b10000000) {
            done += 1;
            GB_FLAG_SET(FLAG_C);
        } else {
            GB_FLAG_CLEAR(FLAG_C);
        }
        *reg_ptr = done;
    } else {
        original = GB_READ_8(REG_HL);
        done = original << 1;
        if (original & 0b10000000) {
            done += 1;
            GB_FLAG_SET(FLAG_C);
        } else {
            GB_FLAG_CLEAR(FLAG_C);
        }
        GB_WRITE_8(REG_HL, done);
        GB_CYCLES(8);
    }

    GB_FLAG_TEST_Z(done);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);

    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_cb_rrc) {
    uint8_t opcode = GB_READ_8(REG_PC - 1);
    uint8_t* reg_ptr;
    uint8_t original;
    uint8_t done;
    GB_OPCODE_DECODE_PTR(opcode, reg_ptr);
    if (reg_ptr) {
        original = *reg_ptr;
        done = original >> 1;
        if (original & 0b00000001) {
            done |= 0b10000000;
            GB_FLAG_SET(FLAG_C);
        } else {
            GB_FLAG_CLEAR(FLAG_C);
        }
        *reg_ptr = done;
    } else {
        original = GB_READ_8(REG_HL);
        done = original >> 1;
        if (original & 0b00000001) {
            done |= 0b10000000;
            GB_FLAG_SET(FLAG_C);
        } else {
            GB_FLAG_CLEAR(FLAG_C);
        }
        GB_WRITE_8(REG_HL, done);
        GB_CYCLES(8);
    }

    GB_FLAG_TEST_Z(done);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);

    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_cb_rl) {
    uint8_t opcode = GB_READ_8(REG_PC - 1);
    uint8_t* reg_ptr;
    uint8_t original;
    uint8_t done;
    GB_OPCODE_DECODE_PTR(opcode, reg_ptr);
    if (reg_ptr) {
        original = *reg_ptr;
        done = original << 1;
        if (GB_FLAG_BIT(FLAG_C)) {
            done += 1;
        }
        if (original & 0b10000000) {
            GB_FLAG_SET(FLAG_C);
        } else {
            GB_FLAG_CLEAR(FLAG_C);
        }
        *reg_ptr = done;
    } else {
        original = GB_READ_8(REG_HL);
        done = original << 1;
        if (GB_FLAG_BIT(FLAG_C)) {
            done += 1;
        }
        if (original & 0b10000000) {
            GB_FLAG_SET(FLAG_C);
        } else {
            GB_FLAG_CLEAR(FLAG_C);
        }
        GB_WRITE_8(REG_HL, done);
        GB_CYCLES(8);
    }
    GB_FLAG_TEST_Z(done);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);
    GB_CYCLES(8);   
}

GB_CPU_OP(gb_cpu_cb_rr) {
    uint8_t opcode = GB_READ_8(REG_PC - 1);
    uint8_t* reg_ptr;
    uint8_t original;
    uint8_t done;
    GB_OPCODE_DECODE_PTR(opcode, reg_ptr);
    if (reg_ptr) {
        original = *reg_ptr;
        done = original >> 1;
        if (GB_FLAG_BIT(FLAG_C)) {
            done |= 0b10000000;
        }
        if (original & 0b00000001) {
            GB_FLAG_SET(FLAG_C);
        } else {
            GB_FLAG_CLEAR(FLAG_C);
        }
        *reg_ptr = done;
    } else {
        original = GB_READ_8(REG_HL);
        done = original >> 1;
        if (GB_FLAG_BIT(FLAG_C)) {
            done |= 0b10000000;
        }
        if (original & 0b00000001) {
            GB_FLAG_SET(FLAG_C);
        } else {
            GB_FLAG_CLEAR(FLAG_C);
        }
        GB_WRITE_8(REG_HL, done);
        GB_CYCLES(8);
    }
    GB_FLAG_TEST_Z(done);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);
    GB_CYCLES(8);   
}

GB_CPU_OP(gb_cpu_cb_sla) {
    uint8_t opcode = GB_READ_8(REG_PC - 1);
    uint8_t* reg_ptr;
    uint8_t original;
    uint8_t done;
    GB_OPCODE_DECODE_PTR(opcode, reg_ptr);
    if (reg_ptr) {
        original = *reg_ptr;
        done = original << 1;
        if (original & 0b10000000) {
            GB_FLAG_SET(FLAG_C);
        } else {
            GB_FLAG_CLEAR(FLAG_C);
        }
        *reg_ptr = done;
    } else {
        original = GB_READ_8(REG_HL);
        done = original << 1;
        if (original & 0b10000000) {
            GB_FLAG_SET(FLAG_C);
        } else {
            GB_FLAG_CLEAR(FLAG_C);
        }
        GB_WRITE_8(REG_HL, done);
        GB_CYCLES(8);
    }
    GB_FLAG_TEST_Z(done);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_cb_sra) {
    uint8_t opcode = GB_READ_8(REG_PC - 1);
    uint8_t* reg_ptr;
    uint8_t original;
    uint8_t done;
    GB_OPCODE_DECODE_PTR(opcode, reg_ptr);
    if (reg_ptr) {
        original = *reg_ptr;
        done = original >> 1;
        if (original & 0b00000001) {
            GB_FLAG_SET(FLAG_C);
        } else {
            GB_FLAG_CLEAR(FLAG_C);
        }
        *reg_ptr = done;
    } else {
        original = GB_READ_8(REG_HL);
        done = original >> 1;
        if (original & 0b00000001) {
            GB_FLAG_SET(FLAG_C);
        } else {
            GB_FLAG_CLEAR(FLAG_C);
        }
        GB_WRITE_8(REG_HL, done);
        GB_CYCLES(8);
    }
    GB_FLAG_TEST_Z(done);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_cb_swap) {
    uint8_t opcode = GB_READ_8(REG_PC - 1);
    uint8_t* reg_ptr;
    uint8_t original;
    uint8_t done;
    GB_OPCODE_DECODE_PTR(opcode, reg_ptr);
    if (reg_ptr) {
        original = *reg_ptr;
        done = 0;
        done |= (original & 0xF) << 4;
        done |= (original & 0xF0) >> 4;
        *reg_ptr = done;
    } else {
        original = GB_READ_8(REG_HL);
        done = 0;
        done |= (original & 0xF) << 4;
        done |= (original & 0xF0) >> 4;
        GB_WRITE_8(REG_HL, done);
        GB_CYCLES(8);
    }
    GB_FLAG_TEST_Z(done);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);
    GB_FLAG_CLEAR(FLAG_C);
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_cb_srl) {
    uint8_t opcode = GB_READ_8(REG_PC - 1);
    uint8_t* reg_ptr;
    uint8_t original;
    uint8_t done;
    GB_OPCODE_DECODE_PTR(opcode, reg_ptr);
    if (reg_ptr) {
        original = *reg_ptr;
        done = original >> 1;
        if (original & 0b00000001) {
            GB_FLAG_SET(FLAG_C);
        } else {
            GB_FLAG_CLEAR(FLAG_C);
        }
        *reg_ptr = done;
    } else {
        original = GB_READ_8(REG_HL);
        done = original >> 1;
        if (original & 0b00000001) {
            GB_FLAG_SET(FLAG_C);
        } else {
            GB_FLAG_CLEAR(FLAG_C);
        }
        GB_WRITE_8(REG_HL, done);
        GB_CYCLES(8);
    }
    GB_FLAG_TEST_Z(done);
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_cb_bit) {
    uint8_t opcode = GB_READ_8(REG_PC - 1);
    uint8_t* reg_ptr;
    uint8_t original;
    GB_OPCODE_DECODE_PTR(opcode, reg_ptr);
    uint8_t bit = GB_OPCODE_DECODE_BIT(opcode);
    if (reg_ptr) {
        original = *reg_ptr;
        if (original & (1 << bit)) {
            GB_FLAG_CLEAR(FLAG_Z);
        } else {
            GB_FLAG_SET(FLAG_Z);
        }
    } else {
        original = GB_READ_8(REG_HL);
        if (original & (1 << bit)) {
            GB_FLAG_CLEAR(FLAG_Z);
        } else {
            GB_FLAG_SET(FLAG_Z);
        }
        GB_CYCLES(4);
    }
    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_SET(FLAG_H);
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_cb_res) {
    uint8_t opcode = GB_READ_8(REG_PC - 1);
    uint8_t* reg_ptr;
    uint8_t original;
    GB_OPCODE_DECODE_PTR(opcode, reg_ptr);
    uint8_t bit = GB_OPCODE_DECODE_BIT(opcode);
    if (reg_ptr) {
        original = *reg_ptr;
        original = original & ~(1 << bit);
        *reg_ptr = original;
    } else {
        original = GB_READ_8(REG_HL);
        original = original & ~(1 << bit);
        GB_WRITE_8(REG_HL, original);
        GB_CYCLES(8);
    }

    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_cb_set) {
    uint8_t opcode = GB_READ_8(REG_PC - 1);
    uint8_t* reg_ptr;
    uint8_t original;
    GB_OPCODE_DECODE_PTR(opcode, reg_ptr);
    uint8_t bit = GB_OPCODE_DECODE_BIT(opcode);
    if (reg_ptr) {
        original = *reg_ptr;
        original = original | (1 << bit);
        *reg_ptr = original;
    } else {
        original = GB_READ_8(REG_HL);
        original = original | (1 << bit);
        GB_WRITE_8(REG_HL, original);
        GB_CYCLES(8);
    }

    GB_CYCLES(8);
}
