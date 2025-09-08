#include "cpu/cpu_load.h"
#include "cpu.h"


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

GB_CPU_OP(gb_cpu_op_ld_h_d8) {
    REG_H = GB_READ_8(REG_PC);
    REG_PC++;
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_ld_a_hli) {
    REG_A = GB_READ_8(REG_HL);
    REG_HL++;
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_ld_l_d8) {
    REG_L = GB_READ_8(REG_PC);
    REG_PC++;
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_ld_hld_a) {
    GB_WRITE_8(REG_HL, REG_A);
    REG_HL--;
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_ld_hl_d8) {
    GB_WRITE_8(REG_HL, GB_READ_8(REG_PC));
    REG_PC++;
    GB_CYCLES(12);
}

GB_CPU_OP(gb_cpu_op_ld_a_hld) {
    REG_A = GB_READ_8(REG_HL);
    REG_HL--;
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_ld_a_d8) {
    REG_A = GB_READ_8(REG_PC);
    REG_PC++;
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_ld_r_r) {
    uint8_t opcode = GB_READ_8(REG_PC-1);
    uint8_t* first_ptr;
    uint8_t* second_ptr;
    if ((opcode & 0xF0) == 0x40) {
        if ((opcode & 0xF) <= 7) {
            first_ptr = &REG_B;
        } else {
            first_ptr = &REG_C;
        }
    } else if ((opcode & 0xF0) == 0x50) {
        if ((opcode & 0xF) <= 7) {
            first_ptr = &REG_D;
        } else {
            first_ptr = &REG_E;
        }
    } else if ((opcode & 0xF0) == 0x60) {
        if ((opcode & 0xF) <= 7) {
            first_ptr = &REG_H;
        } else {
            first_ptr = &REG_L;
        }
    } else if ((opcode & 0xF0) == 0x70) {
        if ((opcode & 0xF) <= 7) {
            first_ptr = NULL;
        } else {
            first_ptr = &REG_A;
        }
    }
    switch (opcode&0xF) {
        case 0x0:
        case 0x8:
            second_ptr = &REG_B;
            break;
        case 0x1:
        case 0x9:
            second_ptr = &REG_C;
            break;
        case 0x2:
        case 0xA:
            second_ptr = &REG_D;
            break;
        case 0x3:
        case 0xB:
            second_ptr = &REG_E;
            break;
        case 0x4:
        case 0xC:
            second_ptr = &REG_H;
            break;
        case 0x5:
        case 0xD:
            second_ptr = &REG_L;
            break;
        case 0x6:
        case 0xE:
            second_ptr = NULL;
            break;
        case 0x7:
        case 0xF:
            second_ptr = &REG_A;
            break;
    }
    if (first_ptr == NULL && second_ptr == NULL) {
        printf("ERROR::CPU_LOAD::HALT_CALLED_FROM_LD_R_R\n");
        return;
    }
    if (first_ptr == NULL) {
        GB_WRITE_8(REG_HL, *second_ptr);
        GB_CYCLES(8);
    } else if (second_ptr == NULL) {
        *first_ptr = GB_READ_8(REG_HL);
        GB_CYCLES(8);
    } else {
        *first_ptr = *second_ptr;
        GB_CYCLES(4);
    }
}

GB_CPU_OP(gb_cpu_op_ldh_a8_a) {
    uint8_t a8 = GB_READ_8(REG_PC);
    GB_WRITE_8(0xFF00 + a8, REG_A);
    REG_PC++;
    GB_CYCLES(12);
}

GB_CPU_OP(gb_cpu_op_ld_c_a) {
    GB_WRITE_8(0xFF00 + REG_C, REG_A);
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_ld_a16_a) {
    GB_WRITE_8(GB_READ_16(REG_PC), REG_A);
    REG_PC+=2;
    GB_CYCLES(16);
}

GB_CPU_OP(gb_cpu_op_ldh_a_a8) {
    REG_A = GB_READ_8(0xFF00 + GB_READ_8(REG_PC));
    REG_PC++;
    GB_CYCLES(12);
}

GB_CPU_OP(gb_cpu_op_ld_a_c) {
    REG_A = GB_READ_8(0xFF00 + REG_C);
    GB_CYCLES(8);
}

GB_CPU_OP(gb_cpu_op_ld_a_a16) {
    REG_A = GB_READ_8(GB_READ_16(REG_PC));
    REG_PC+=2;
    GB_CYCLES(16);
}
