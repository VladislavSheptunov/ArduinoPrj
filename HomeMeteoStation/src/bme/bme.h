#ifndef _BME_H_
#define _BME_H_

#include <Wire.h> 
#include <BME280_t.h> 

#include <stdint.h>

class BME {
public:
    BME();
    ~BME();

    void startUp();
    void refresh(void);
    void endUp(void);

    float getTemperature(void);
    float getHumidity(void);
    float getPressure(void);

private:
    BME280<> sensor;
};

#endif  // _BME_H_
