#ifndef _HOME_METEO_STATION_H_
#define _HOME_METEO_STATION_H_

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <stdio.h>  // FOR debug

#include "DS1302.h"
#include "DHT.h"

// -- PIN`s
#define PIN_DS1302_RST 5
#define PIN_DS1302_DAT 6
#define PIN_DS1302_CLK 7

#define PIN_DHT_DAT    2
// --

class HMS {
public:
    HMS();
    ~HMS();

    void init(void);

    void enableBacklight(bool flag);
    void show(void);
    void blink();

    void updateDateTime(void);
    void updateTemperature(void);
    void updateHumidity(void);
private:
    LiquidCrystal_I2C *lcd;
    DS1302 *rtc;
    DHT *dht;

    DateTime dateTime;
    float    temperature;
    uint8_t  humidity;
};

#endif // _HOME_METEO_STATION_H_
