#ifndef _DS1302_H_
#define _DS1302_H_

#include "../datatime/datatime.h"

class DS1302 {
public:
    static const int sizeRAM = 31;
    
    DS1302(uint8_t pinRST, uint8_t pinDAT, uint8_t pinCLK);

    void writeProtect(bool enable);
    void halt(bool value);

    void getDateTime(DateTime &dt);
    void setDateTime(DateTime &dt);

    void writeRam(uint8_t address, uint8_t value);
    uint8_t readRam(uint8_t address);

    void writeRamBulk(const uint8_t* data, int len);
    void readRamBulk(uint8_t* data, int len);

    uint8_t readRegister(uint8_t reg);
    void writeRegister(uint8_t reg, uint8_t value);
    
private:
    uint8_t pinRST;
    uint8_t pinDAT;
    uint8_t pinCLK;

    void writeOut(uint8_t value, bool readAfter = false);
    uint8_t readIn();
};

#endif  // DS1302_H_
