#ifndef _H_CPU_REGISTER
#define _H_CPU_REGISTER

#include <stdint.h>
typedef struct {
    union {
        struct { uint8_t f, a; };
        uint16_t af;
    };
    union {
        struct { uint8_t c, b; };
        uint16_t bc;
    };
    union {
        struct { uint8_t e, d; };
        uint16_t de;
    };
    union {
        struct { uint8_t l, h; };
        uint16_t hl;
    };
    uint16_t pc;
    uint16_t sp;
    uint8_t ime;
} gb_cpu_register;

gb_cpu_register* gb_cpu_register_create();
void gb_cpu_register_delete(gb_cpu_register* reg);

void gb_cpu_register_dump(gb_cpu_register* reg);
#endif
