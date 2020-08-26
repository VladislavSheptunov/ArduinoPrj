#ifndef _HOME_METEO_STATION_H_
#define _HOME_METEO_STATION_H_

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>  // FOR debug

#include "builder.h"
#include "memory.h"

#include "DS1302.h"
#include "DHT.h"
#include "led.h"
#include "button.h"
#include "timer.h"

#define HMS_NORMAL_TEMPERATURE  (24.0)
#define HMS_NORMAL_HUMIDITY     (50.0)
#define HMS_MENU_ITEMS          (2)

class HMS {
  public:
    enum HMSPin {
      HMS_PIN_RESERVED_0   = 0x00,
      HMS_PIN_RESERVED_1   = 0x01,

      HMS_PIN_DHT_DAT      = 0x08,

      HMS_PIN_DIOD_LEFT    = 0x09,
      HMS_PIN_DIOD_RIGHT   = 0x04,

      HMS_PIN_DS1302_RST   = 0x05,
      HMS_PIN_DS1302_DAT   = 0x06,
      HMS_PIN_DS1302_CLK   = 0x07,

      HMS_PIN_BUTTON_LEFT  = 0x02,
      HMS_PIN_BUTTON_RIGHT = 0x03,

      HMS_PIN_RESERVED_10  = 0x10,
      HMS_PIN_RESERVED_11  = 0x11,
      HMS_PIN_RESERVED_12  = 0x12,
      HMS_PIN_RESERVED_13  = 0x13,
    };

    HMS();
    ~HMS();

    void init(void);

    void enableBacklight(bool flag);
    void show(void);

    void updateDateTime(void);
    void updateTemperature(void);
    void updateHumidity(void);

    void updateLeftButton();
    void updateRightButton();

    void setControlOnTemperature(uint8_t diviation);
    bool isDivOnNormalTemperature(void);
    
    void setControlOnHumidity(uint8_t diviation);
    bool isDivOnNormalHumidity(void);

  private:
    BUILDER *builder;

    LiquidCrystal_I2C *lcd;
    DS1302 *rtc;
    DHT *dht;

    LED *leftLED;
    LED *rightLED;

    BUTTON *leftButton;
    BUTTON *rightButton;

    DateTime dateTime;

    float    temperature;
    uint8_t  temperatureDiv;

    uint8_t  humidity;
    uint8_t  humidityDiv;

    uint8_t numMenu;
    TIMER   *timerMenu;

    void nextMenu(void);
    void previousMenu(void);
};

#endif // _HOME_METEO_STATION_H_
