#ifndef _CPU_FLAGS
#define _CPU_FLAGS

#define GB_FLAG_Z 0x80
#define GB_FLAG_N 0x40
#define GB_FLAG_H 0x20
#define GB_FLAG_C 0x10

//Use only in the context of GB_CPU_OP

#define GB_FLAG_SET(x) REG_F |= x
#define GB_FLAG_CLEAR(x) REG_F &= ~x

#define GB_FLAG_BIT(x) (((REG_F & x) != 0) ? 1 : 0)

#define GB_FLAG_TEST_Z(x) do {\
    if (x == 0) {\
        GB_FLAG_SET(GB_FLAG_Z);\
    } else {\
        GB_FLAG_CLEAR(GB_FLAG_Z);\
    }\
}while(0);

#define GB_FLAG_TEST_ADD_H_8(x, y) do {\
    unsigned int result = ((uint8_t)x & 0xF) + ((uint8_t)y & 0xF);\
    if (result > 0xF) {\
        GB_FLAG_SET(GB_FLAG_H);\
    } else {\
        GB_FLAG_CLEAR(GB_FLAG_H);\
    }\
} while(0);

 //c !SHOULD! only ever be 1 or 0, so cast not needed
#define GB_FLAG_TEST_ADC_H_8(x, y, c) do {\
    unsigned int result = ((uint8_t)x & 0xF) + ((uint8_t)y & 0xF) + (c & 0xF);\
    if (result > 0xF) {\
        GB_FLAG_SET(GB_FLAG_H);\
    } else {\
        GB_FLAG_CLEAR(GB_FLAG_H);\
    }\
} while(0);

#define GB_FLAG_TEST_SUB_H_8(x, y) do {\
    int result = ((uint8_t)x & 0xF) - ((uint8_t)y & 0xF);\
    if (result < 0) {\
        GB_FLAG_SET(GB_FLAG_H);\
    } else {\
        GB_FLAG_CLEAR(GB_FLAG_H);\
    }\
} while(0);

#define GB_FLAG_TEST_SBC_H_8(x, y, c) do {\
    int result = ((uint8_t)x & 0xF) - ((uint8_t)y & 0xF) - (uint8_t)(c & 0xF);\
    if (result < 0) {\
        GB_FLAG_SET(GB_FLAG_H);\
    } else {\
        GB_FLAG_CLEAR(GB_FLAG_H);\
    }\
} while(0);

#define GB_FLAG_TEST_ADD_C_8(x, y) do {\
    unsigned int result = (uint8_t)x + (uint8_t)y;\
    if (result > 0xFF) {\
        GB_FLAG_SET(GB_FLAG_C);\
    } else {\
        GB_FLAG_CLEAR(GB_FLAG_C);\
    }\
} while(0);

#define GB_FLAG_TEST_ADC_C_8(x, y, c) do {\
    unsigned int result = x + y + c;\
    if (result > 0xFF) {\
        GB_FLAG_SET(GB_FLAG_C);\
    } else {\
        GB_FLAG_CLEAR(GB_FLAG_C);\
    }\
} while(0);

#define GB_FLAG_TEST_SUB_C_8(x, y) do {\
    int result = (uint8_t)x - (uint8_t)y;\
    if (result < 0) {\
        GB_FLAG_SET(GB_FLAG_C);\
    } else {\
        GB_FLAG_CLEAR(GB_FLAG_C);\
    }\
} while(0);

#define GB_FLAG_TEST_SBC_C_8(x, y, c) do {\
    int result = (uint8_t)x - (uint8_t)y - (uint8_t)c;\
    if (result < 0) {\
        GB_FLAG_SET(GB_FLAG_C);\
    } else {\
        GB_FLAG_CLEAR(GB_FLAG_C);\
    }\
} while(0);

#define GB_FLAG_TEST_ADD_C_16(x, y) do {\
    unsigned int result = (uint16_t)x + (uint16_t)y;\
    if (result > 0xFFFF) {\
        GB_FLAG_SET(GB_FLAG_C);\
    } else {\
        GB_FLAG_CLEAR(GB_FLAG_C);\
    }\
} while(0);
//end

#endif
