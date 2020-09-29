#include "divcontol.h"

DIVCONTOL::DIVCONTOL(float *contolValue, float normalValue, uint8_t percent) {
    this->contolValue = contolValue;
    this->normalValue = normalValue;
    this->percent     = percent;
}

DIVCONTOL::~DIVCONTOL() {

}

bool DIVCONTOL::isGoingBeyond() {
    static uint32_t countDiv   = COUNT_DIVIATION;
    static uint32_t countAlarm = COUNT_ALARM;

    float normalDiv = (this->normalValue * (float)this->percent) / 100.0;
    this->divValue  = *this->contolValue - this->normalValue;
    
    if (normalDiv < abs(this->divValue)) {
        countDiv = (countDiv) ? (countDiv - 1) : countDiv;
    }

    if (countDiv) {
        return true;
    }

    if (--countAlarm) {
        return true;
    }

    countDiv   = COUNT_DIVIATION;
    countAlarm = COUNT_ALARM;

    return false;
}
