#ifndef _HOME_METEO_STATION_H_
#define _HOME_METEO_STATION_H_

#include "../builder/builder.h"
#include "../memory/memory.h"

#include "../DS1302/DS1302.h"
#include "../LCD1602/LCD1602.h"

#include "DHT.h"
#include "../led/led.h"
#include "../button/button.h"
#include "../timer/timer.h"

#define HMS_NORMAL_TEMPERATURE             (24.0)
#define HMS_NORMAL_DIV_IN_PER_TEMPERATURE  (10)

#define HMS_NORMAL_HUMIDITY                (50.0)
#define HMS_NORMAL_DIV_IN_PER_HUMIDITY     (20)

#define HMS_COUNT_DIVIATION     (90)
#define HMS_COUNT_ALARM         (11)

class HMS {
  public:
    enum DigitalPin : int {
      PIN_RESERVED_0    = 0,
      PIN_RESERVED_1    = 1,

      PIN_DHT_DAT       = 8,

      PIN_DIOD_LEFT     = 9,
      PIN_DIOD_RIGHT    = 4,

      PIN_DS1302_RST    = 5,
      PIN_DS1302_DAT    = 6,
      PIN_DS1302_CLK    = 7,

      PIN_BUTTON_LEFT   = 2,
      PIN_BUTTON_RIGHT  = 3,

      PIN_RESERVED_10   = 10,
      PIN_BACKLIGHT     = 11,
      PIN_RESERVED_12   = 12,
      PIN_RESERVED_13   = 13,
    };

    enum Menu : int {
        MENU_GENERAL     = 0,
        MENU_DIVIATION   = 1,
        MENU_PARAM       = 2,
        MENU_ABOUT       = 3,

        MENU_COUNT
    };

    HMS();
    ~HMS();

    void init(void);

    void show(void);

    void updateDateTime(void);
    void updateTemperature(void);
    void updateHumidity(void);
    void updatePressure(void);

    void updateBacklight(void);

    void updateConfiguration(void);

    void updateLeftButton();
    void updateRightButton();

    void setControlOnTemperature(uint8_t diviation);
    void setControlOnHumidity(uint8_t diviation);
  
  private:
    BUILDER *builder;

    LCD1602 *lcd;
    DS1302 *rtc;
    DHT *dht;

    LED *leftLED;
    LED *rightLED;

    BUTTON *leftButton;
    BUTTON *rightButton;

    DateTime dateTime;

    float temperature;
    float temperatureDiv;

    float humidity;
    float humidityDiv;

    float pressure;
    float pressureDiv;

    uint8_t backlight;

    struct {
        uint8_t  number;
        uint32_t displayTime;
        bool     isCapture;

        uint8_t ptrChgGeneral;
        uint8_t ptrChgParam;
    } menu;

    TIMER   *timerMenu;
    
    void nextMenu(void);
    void previousMenu(void);

    void checkDivOnNormalTemperature(void);
    void checkDivOnNormalHumidity(void);
};

#endif // _HOME_METEO_STATION_H_
