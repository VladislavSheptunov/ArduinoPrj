#ifndef _DS1307_H_
#define _DS1307_H_

#include "../datatime/datatime.h"

class DS1307 {
public:
    DS1307();
    static bool getDateTime(DateTime &dt);
    static bool setDateTime(DateTime &dt);
    static unsigned char isRunning();
    static void setCalibration(char calValue);
    static char getCalibration();

private:
    static uint8_t dec2bcd(uint8_t num);
    static uint8_t bcd2dec(uint8_t num);
};

#endif // _DS1307_H_
