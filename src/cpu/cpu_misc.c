#include "cpu/cpu_misc.h"
#include "cpu.h"

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
        printf("EXECUTING CB INSTRUCTION: 0x%02X\n", opcode);
        opcode_function_table_cb[opcode](cpu, memory);
    } else {
        printf("EXECUTING UNWRITTEN INSTRUCTION: 0x%02X\n", opcode);
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
