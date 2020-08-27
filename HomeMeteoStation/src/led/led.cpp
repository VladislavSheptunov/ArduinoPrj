#include "led.h"

LED::LED(uint8_t pin) {
    pinMode(this->pin = pin, OUTPUT);
}

LED::~LED() {

}

void LED::turnOn() {
    digitalWrite(this->pin, HIGH);
}

void LED::turnOff() {
    digitalWrite(this->pin, LOW);
}

void LED::blink() {
    static bool state = true;

    if (state) {
        this->turnOn();
        state = false;
    } else {
        this->turnOff();
        state = true;
    }
}
