#include "timer.h"

TIMER::TIMER(uint64_t interval) {
    this->countTick    = 0;
    this->intervalTick = interval;
    this->status       = false;
}

TIMER::~TIMER() {

}

void TIMER::start() {
    this->countTick = millis();
    this->status = true;
}

void TIMER::stop() {
    this->status = false;
}

void TIMER::changeInterval(uint64_t interval) {
    this->intervalTick = interval;
}

bool TIMER::isActive() {
  return this->status;  
}

bool TIMER::isExpired() {
    if (this->status && this->intervalTick) {
        uint64_t systemTick = millis();
        if ((systemTick - this->countTick) >= this->intervalTick) {
            return true;
        }
    }

    return false;
}
