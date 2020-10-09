#ifndef _HOME_METEO_STATION_H_
#define _HOME_METEO_STATION_H_

#include "../builder/builder.h"
#include "../memory/memory.h"

#include "../ds1307/ds1307.h"
#include "../lcd1602/lcd1602.h"
#include "../bme/bme.h"

#include "../led/led.h"
#include "../button/button.h"
#include "../timer/timer.h"

#define HMS_NORMAL_TEMPERATURE             (24.0)
#define HMS_NORMAL_DIV_IN_PER_TEMPERATURE  (10)

#define HMS_NORMAL_HUMIDITY                (50.0)
#define HMS_NORMAL_DIV_IN_PER_HUMIDITY     (20)

#define HMS_COUNT_DIVIATION     (90)
#define HMS_COUNT_ALARM         (11)

struct StructMeas {
    float val;
    float div;
};
typedef struct StructMeas Meas, *MeasLink;

class HMS {
  public:
    enum DigitalPin : int {
      PIN_RESERVED_0    = 0,
      PIN_RESERVED_1    = 1,

      PIN_BUTTON_LEFT   = 2,
      PIN_BUTTON_RIGHT  = 3,

      // -- LCD and BME280
      PIN_RESERVED_SDA  = 4,
      PIN_RESERVED_SCK  = 5,
      // --

      PIN_DIOD_LEFT     = 6,
      PIN_DIOD_RIGHT    = 7,

      PIN_DS1302_RST    = 8,
      PIN_DS1302_DAT    = 9,
      PIN_DS1302_CLK    = 10,

      PIN_LCD_BACKLIGHT = 11,

      PIN_RESERVED_12   = 12,
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
    void updateMeas(void);
    void updateBacklight(void);
    void updateConfiguration(void);

    void updateLeftButton();
    void updateRightButton();

  private:
    BUILDER *builder;

    LCD1602 *lcd;
    DS1307  *rtc;
    BME     *bme;

    LED *leftLED;
    LED *rightLED;

    BUTTON *leftButton;
    BUTTON *rightButton;

    DateTime dateTime;

    Meas temperature;
    Meas humidity;
    Meas pressure;

    struct {
        uint8_t  number;
        uint32_t displayTime;
        bool     isCapture;

        uint8_t ptrChangeGeneral;
        uint8_t ptrChangeParam;
    } menu;

    TIMER   *timerMeas;
    TIMER   *timerMenu;

    uint8_t backlight;
    
    void nextMenu(void);
    void previousMenu(void);

    void checkDivOnNormalTemperature(void);
    void checkDivOnNormalHumidity(void);
};

#endif // _HOME_METEO_STATION_H_
