#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <stdint.h>
#include <stdbool.h>

#include "Arduino.h"

class BUTTON {
public:
    BUTTON(uint8_t pin);
    ~BUTTON();

    bool state(void);

private:
    uint8_t pin;
};

#endif // _BUTTON_H_
