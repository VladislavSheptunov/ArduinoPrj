#ifndef _DIVCONTOL_H_
#define _DIVCONTOL_H_

#include <stdint.h>
#include <stdbool.h>

#include "Arduino.h"

#define COUNT_DIVIATION  (90)
#define COUNT_ALARM      (11)

class DIVCONTOL {
public:
    DIVCONTOL(float *contolValue, float normalValue, uint8_t percent);
    ~DIVCONTOL();

    float getDiviation();

    bool isGoingBeyond();

private:
    float *contolValue;
    float normalValue;
    uint8_t percent;

    float divValue;
};

#endif // _DIVCONTOL_H_
