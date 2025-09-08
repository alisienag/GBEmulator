#ifndef _CPU_FLAGS
#define _CPU_FLAGS

#define FLAG_Z 0x80
#define FLAG_N 0x40
#define FLAG_H 0x20
#define FLAG_C 0x10

//Use only in the context of GB_CPU_OP

#define SET_FLAG(x) REG_F |= x
#define CLEAR_FLAG(x) REG_F &= ~x
//end

#endif
