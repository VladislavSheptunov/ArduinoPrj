#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>

#include "Arduino.h"

class LED {
public:
    LED(uint8_t pin);
    ~LED();

    void turnOn(void);
    void turnOff(void);
    void blink(void);

private:
    uint8_t pin;
    bool state;
};

#endif // _LED_H_
