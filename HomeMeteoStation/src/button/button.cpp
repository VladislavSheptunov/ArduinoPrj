#include "button.h"

BUTTON::BUTTON(uint8_t pin) {
    pinMode(this->pin = pin, INPUT);
}

BUTTON::~BUTTON() {

}

bool BUTTON::state() {
    return (bool)digitalRead(this->pin);
}
