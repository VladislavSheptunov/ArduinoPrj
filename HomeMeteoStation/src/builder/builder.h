#ifndef _BUILDER_H_
#define _BUILDER_H_

#include <stdint.h>

#define BUILDER_ASCII_NUM_TO_HEX(NUM) \
( \
    RANGE(0x30, NUM, 0x39) ? ((uint8_t)(NUM) - 0x30) : (uint8_t)0x00 \
)

#define BUILDER_YEAR \
( \
    (BUILDER_ASCII_NUM_TO_HEX(__DATE__[9]) * (uint8_t)10) + BUILDER_ASCII_NUM_TO_HEX(__DATE__[10]) \
)

#define BUILDER_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILDER_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILDER_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILDER_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILDER_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILDER_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILDER_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILDER_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILDER_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILDER_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILDER_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILDER_MONTH_IS_DEC (__DATE__[0] == 'D')

#define BUILDER_MONTH \
( \
    (BUILDER_MONTH_IS_JAN) ? (uint8_t)1  : \
    (BUILDER_MONTH_IS_FEB) ? (uint8_t)2  : \
    (BUILDER_MONTH_IS_MAR) ? (uint8_t)3  : \
    (BUILDER_MONTH_IS_APR) ? (uint8_t)4  : \
    (BUILDER_MONTH_IS_MAY) ? (uint8_t)5  : \
    (BUILDER_MONTH_IS_JUN) ? (uint8_t)6  : \
    (BUILDER_MONTH_IS_JUL) ? (uint8_t)7  : \
    (BUILDER_MONTH_IS_AUG) ? (uint8_t)8  : \
    (BUILDER_MONTH_IS_SEP) ? (uint8_t)9  : \
    (BUILDER_MONTH_IS_OCT) ? (uint8_t)10 : \
    (BUILDER_MONTH_IS_NOV) ? (uint8_t)11 : \
    (BUILDER_MONTH_IS_DEC) ? (uint8_t)12 : \
    (0)                                    \
)

#define BUILDER_DAY \
( \
    (BUILDER_ASCII_NUM_TO_HEX(__DATE__[4]) * (uint8_t)10) + BUILDER_ASCII_NUM_TO_HEX(__DATE__[5]) \
)

#define BUILDER_HOUR \
( \
    (BUILDER_ASCII_NUM_TO_HEX(__TIME__[0]) * (uint8_t)10) + BUILDER_ASCII_NUM_TO_HEX(__TIME__[1]) \
)

#define BUILDER_MINUTE \
( \
    (BUILDER_ASCII_NUM_TO_HEX(__TIME__[3]) * (uint8_t)10) + BUILDER_ASCII_NUM_TO_HEX(__TIME__[4]) \
)

#define BUILDER_SECOND \
( \
    (BUILDER_ASCII_NUM_TO_HEX(__TIME__[6]) * (uint8_t)10) + BUILDER_ASCII_NUM_TO_HEX(__TIME__[7]) \
)

#define BUILDER_DATE_TIME \
    BUILDER_SECOND,       \
    BUILDER_MINUTE,       \
    BUILDER_HOUR,         \
    BUILDER_DAY,          \
    BUILDER_MONTH,        \
    BUILDER_YEAR

#define BUILDER_DATE_TIME_INVERSE \
    BUILDER_YEAR,         \
    BUILDER_MONTH,        \
    BUILDER_DAY,          \
    BUILDER_HOUR,         \
    BUILDER_MINUTE,       \
    BUILDER_SECOND        

class BUILDER {
public:
    BUILDER(uint32_t major, uint32_t minor, uint32_t patch, uint32_t build);
    ~BUILDER();

    uint32_t versionToNumber(void);
    char* versionToString(void);

private:
    uint32_t version;
};

#endif // _BUILDER_H_
