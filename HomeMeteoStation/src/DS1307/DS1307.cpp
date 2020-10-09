#if defined (__AVR_ATtiny84__) || defined(__AVR_ATtiny85__) || (__AVR_ATtiny2313__)
#include <TinyWireM.h>
#define Wire TinyWireM
#else
#include <Wire.h>
#endif

#include "DS1307.h"

#define DS1307_CTRL_ID 0x68 

DS1307::DS1307() {
    Wire.begin();
}

bool DS1307::getDateTime(DateTime &dt) {
    Wire.beginTransmission(DS1307_CTRL_ID);
    Wire.write(0x00);

    if (Wire.endTransmission() != 0) {
        return false;
    }

    Wire.requestFrom(DS1307_CTRL_ID, 7);
    if (Wire.available() < 7) { 
        return false;
    }

    dt.sec  = bcd2dec(Wire.read() & 0x7f);   
    dt.min  = bcd2dec(Wire.read());
    dt.hour = bcd2dec(Wire.read() & 0x3f);  // mask assumes 24hr clock
    dt.day  = static_cast<DateTime::Day>(bcd2dec(Wire.read()));
    dt.date = bcd2dec(Wire.read());
    dt.mon  = bcd2dec(Wire.read());
    dt.year = bcd2dec(Wire.read());

    return (dt.sec & 0x80) ? false : true;
}

bool DS1307::setDateTime(DateTime &dt) {
    Wire.beginTransmission(DS1307_CTRL_ID);
    Wire.write(0x00); // reset register pointer  
    Wire.write(0x80); // Stop the clock. The seconds will be written last
    Wire.write(dec2bcd(dt.min));
    Wire.write(dec2bcd(dt.hour));      // sets 24 hour format
    Wire.write(dec2bcd(dt.day));   
    Wire.write(dec2bcd(dt.date));
    Wire.write(dec2bcd(dt.mon));
    Wire.write(dec2bcd(dt.year));

    if (Wire.endTransmission() != 0) {
        return false;
    }

    // Now go back and set the seconds, starting the clock back up as a side effect
    Wire.beginTransmission(DS1307_CTRL_ID);
    Wire.write(0x00); // reset register pointer  
    Wire.write(dec2bcd(dt.sec)); // write the seconds, with the stop bit clear to restart

    return Wire.endTransmission() ? false : true;
}

uint8_t DS1307::isRunning() {
    Wire.beginTransmission(DS1307_CTRL_ID);
    Wire.write(0x00);
    Wire.endTransmission();

    // Just fetch the seconds register and check the top bit
    Wire.requestFrom(DS1307_CTRL_ID, 1);
    return !(Wire.read() & 0x80);
}

void DS1307::setCalibration(char calValue) {
    uint8_t calReg = abs(calValue) & 0x1F;

    if (calValue >= 0) { 
        calReg |= 0x20; // S bit is positive to speed up the clock
    }

    Wire.beginTransmission(DS1307_CTRL_ID);
    Wire.write(0x07); // Point to calibration register
    Wire.write(calReg);
    Wire.endTransmission();  
}

char DS1307::getCalibration() {
    Wire.beginTransmission(DS1307_CTRL_ID);
    Wire.write(0x07);
    Wire.endTransmission();

    Wire.requestFrom(DS1307_CTRL_ID, 1);
    uint8_t calReg = Wire.read();

    char out = calReg & 0x1f;
    if (!(calReg & 0x20)) { 
        out = -out;
    }

    return out;
}

uint8_t DS1307::dec2bcd(uint8_t num) {
    return ((num/10 * 16) + (num % 10));
}

uint8_t DS1307::bcd2dec(uint8_t num) {
    return ((num /16 * 10) + (num % 16));
}
