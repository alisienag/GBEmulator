#include "cpu/cpu_cb.h"
#include "cpu.h"
#include "cpu/cpu_alu.h"

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
        case GB_OPCODE_DECODE_REG_HL:\
            x = NULL;\
            break;\
    }\
    } while (0)

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

    GB_FLAG_CLEAR(FLAG_N);
    GB_FLAG_CLEAR(FLAG_H);

    GB_CYCLES(8);
}
