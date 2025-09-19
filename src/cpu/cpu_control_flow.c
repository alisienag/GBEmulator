#include "cpu/cpu_control_flow.h"
#include "cpu.h"
#include "cpu/cpu_flags.h"

GB_CPU_OP(gb_cpu_op_jr_nz_e8) {
    if (GB_FLAG_BIT(FLAG_Z) == 0) {
        int8_t e8 = (int8_t)GB_READ_8(REG_PC);
        REG_PC += e8 + 1;
        GB_CYCLES(12);
    } else {
        REG_PC++;
        GB_CYCLES(8);
    }
}

GB_CPU_OP(gb_cpu_op_jr_nc_e8) {
    if (GB_FLAG_BIT(FLAG_C) == 0) {
        int8_t e8 = (int8_t)GB_READ_8(REG_PC);
        REG_PC += e8 + 1;
        GB_CYCLES(12);
    } else {
        REG_PC++;
        GB_CYCLES(8);
    }
}

GB_CPU_OP(gb_cpu_op_ret_nz) {
    if (GB_FLAG_BIT(FLAG_Z) == 0) {
        REG_PC = GB_READ_16(REG_SP);
        REG_SP += 2;
        GB_CYCLES(20);
    } else {
        GB_CYCLES(8);
    }
}

GB_CPU_OP(gb_cpu_op_ret_nc) {
    if (GB_FLAG_BIT(FLAG_C) == 0) {
        REG_PC = GB_READ_16(REG_SP);
        REG_SP += 2;
        GB_CYCLES(20);
    } else {
        GB_CYCLES(8);
    }
}

GB_CPU_OP(gb_cpu_op_jp_nz_a16) {
    if (GB_FLAG_BIT(FLAG_Z) == 0) {
        uint16_t address = GB_READ_16(REG_PC);
        REG_PC = address;
        GB_CYCLES(16);
    } else {
        REG_PC += 2;
        GB_CYCLES(12);
    }
}

GB_CPU_OP(gb_cpu_op_jp_nc_a16) {
    if (GB_FLAG_BIT(FLAG_C) == 0) {
        uint16_t address = GB_READ_16(REG_PC);
        REG_PC = address;
        GB_CYCLES(16);
    } else {
        REG_PC += 2;
        GB_CYCLES(12);
    }
}

GB_CPU_OP(gb_cpu_op_jp_a16) {
    uint16_t address = GB_READ_16(REG_PC);
    REG_PC = address;
    GB_CYCLES(16);
}

GB_CPU_OP(gb_cpu_op_call_nz_a16) {
    uint16_t address = GB_READ_16(REG_PC); 
    REG_PC += 2;
    if (GB_FLAG_BIT(FLAG_Z) == 0) {
        REG_SP -= 2;
        GB_WRITE_16(REG_SP, REG_PC);
        REG_PC = address;
        GB_CYCLES(24);
    } else {
        GB_CYCLES(12);
    }
}
GB_CPU_OP(gb_cpu_op_call_nc_a16) {
    uint16_t address = GB_READ_16(REG_PC); 
    REG_PC += 2;
    if (GB_FLAG_BIT(FLAG_C) == 0) {
        REG_SP -= 2;
        GB_WRITE_16(REG_SP, REG_PC);
        REG_PC = address;
        GB_CYCLES(24);
    } else {
        GB_CYCLES(12);
    }
}

GB_CPU_OP(gb_cpu_op_rst_00) {
    REG_SP -= 2;
    GB_WRITE_16(REG_SP, REG_PC);
    REG_PC = 0x00;
    GB_CYCLES(16);
}
GB_CPU_OP(gb_cpu_op_rst_10) {
    REG_SP -= 2;
    GB_WRITE_16(REG_SP, REG_PC);
    REG_PC = 0x10;
    GB_CYCLES(16);
}
GB_CPU_OP(gb_cpu_op_rst_20) {
    REG_SP -= 2;
    GB_WRITE_16(REG_SP, REG_PC);
    REG_PC = 0x20;
    GB_CYCLES(16);
}
GB_CPU_OP(gb_cpu_op_rst_30) {
    REG_SP -= 2;
    GB_WRITE_16(REG_SP, REG_PC);
    REG_PC = 0x30;
    GB_CYCLES(16);
}

GB_CPU_OP(gb_cpu_op_jr_e8) {
    int8_t offset = GB_READ_8(REG_PC);
    REG_PC++;

    REG_PC += offset;
    GB_CYCLES(12);
}

GB_CPU_OP(gb_cpu_op_jr_z_e8) {
    if (GB_FLAG_BIT(FLAG_Z) == 1) {
        int8_t offset = GB_READ_8(REG_PC);
        REG_PC += offset + 1;
        GB_CYCLES(12);
    } else {
        REG_PC++;
        GB_CYCLES(8);
    }
}
GB_CPU_OP(gb_cpu_op_jr_c_e8) {
    if (GB_FLAG_BIT(FLAG_Z) == 1) {
        int8_t offset = GB_READ_8(REG_PC);
        REG_PC += offset + 1;
        GB_CYCLES(12);
    } else {
        REG_PC++;
        GB_CYCLES(8);
    }
}

GB_CPU_OP(gb_cpu_op_ret_z) {
    if (GB_FLAG_BIT(FLAG_Z) == 1) {
        uint16_t address = GB_READ_16(REG_SP);
        REG_SP += 2;
        REG_PC = address;
        GB_CYCLES(20);
    } else {
        GB_CYCLES(8);
    }
}

GB_CPU_OP(gb_cpu_op_ret_c) {
    if (GB_FLAG_BIT(FLAG_C) == 1) {
        uint16_t address = GB_READ_16(REG_SP);
        REG_SP += 2;
        REG_PC = address;
        GB_CYCLES(20);
    } else {
        GB_CYCLES(8);
    }
}

GB_CPU_OP(gb_cpu_op_ret) {
    uint16_t address = GB_READ_16(REG_SP);
    REG_SP += 2;
    REG_PC = address;
    GB_CYCLES(16);
}
GB_CPU_OP(gb_cpu_op_reti) {
    uint16_t address = GB_READ_16(REG_SP);
    REG_SP += 2;
    REG_PC = address;
    GB_IME_ENABLE_INSTANT();
    GB_CYCLES(16);
}

GB_CPU_OP(gb_cpu_op_jp_hl) {
    REG_PC = REG_HL;
    GB_CYCLES(4);
}

GB_CPU_OP(gb_cpu_op_jp_z_a16) {
    if (GB_FLAG_BIT(FLAG_Z) == 1) {
        uint16_t address = GB_READ_16(REG_PC);
        REG_PC = address;
        GB_CYCLES(16);
    } else {
        REG_PC += 2;
        GB_CYCLES(12);
    }
}

GB_CPU_OP(gb_cpu_op_jp_c_a16) {
    if (GB_FLAG_BIT(FLAG_C) == 1) {
        uint16_t address = GB_READ_16(REG_PC);
        REG_PC = address;
        GB_CYCLES(16);
    } else {
        REG_PC += 2;
        GB_CYCLES(12);
    }
}

GB_CPU_OP(gb_cpu_op_call_z_a16) {
    if (GB_FLAG_BIT(FLAG_Z) == 1) {
        uint16_t address = GB_READ_16(REG_PC);
        REG_SP -= 2;
        GB_WRITE_16(REG_SP, REG_PC);
        REG_PC = address;
        GB_CYCLES(24);
    } else {
        REG_PC += 2;
        GB_CYCLES(12);
    }
}
GB_CPU_OP(gb_cpu_op_call_c_a16) {
    if (GB_FLAG_BIT(FLAG_C) == 1) {
        uint16_t address = GB_READ_16(REG_PC);
        REG_SP -= 2;
        GB_WRITE_16(REG_SP, REG_PC);
        REG_PC = address;
        GB_CYCLES(24);
    } else {
        REG_PC += 2;
        GB_CYCLES(12);
    }
}
GB_CPU_OP(gb_cpu_op_call_a16) {
    uint16_t address = GB_READ_16(REG_PC);
    REG_SP -= 2;
    GB_WRITE_16(REG_SP, REG_PC);
    REG_PC = address;
    GB_CYCLES(24);
}

GB_CPU_OP(gb_cpu_op_rst_08) {
    REG_SP -= 2;
    GB_WRITE_16(REG_SP, REG_PC);
    REG_PC = 0x08;
    GB_CYCLES(16);
}
GB_CPU_OP(gb_cpu_op_rst_18) {
    REG_SP -= 2;
    GB_WRITE_16(REG_SP, REG_PC);
    REG_PC = 0x18;
    GB_CYCLES(16);
}
GB_CPU_OP(gb_cpu_op_rst_28) {
    REG_SP -= 2;
    GB_WRITE_16(REG_SP, REG_PC);
    REG_PC = 0x28;
    GB_CYCLES(16);
}
GB_CPU_OP(gb_cpu_op_rst_38) {
    REG_SP -= 2;
    GB_WRITE_16(REG_SP, REG_PC);
    REG_PC = 0x38;
    GB_CYCLES(16);
}
