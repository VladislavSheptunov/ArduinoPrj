#include "led.h"

LED::LED(uint8_t pin) {
    pinMode(this->pin = pin, OUTPUT);
    this->state = false;
}

LED::~LED() {

}

void LED::turnOn() {
    digitalWrite(this->pin, HIGH);
    this->state = true;
}

void LED::turnOff() {
    digitalWrite(this->pin, LOW);
    this->state = false;
}

void LED::blink() {
    (this->state) ? this->turnOff() : this->turnOn();
}
