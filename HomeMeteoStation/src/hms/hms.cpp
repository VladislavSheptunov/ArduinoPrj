#include "hms.h"

#define RANGE(a,x,b)        ((x >= a) && (x <= b))
#define SIGN_FLOAT(value)   (((0.0 < (value))) ? '+' : ((0.0 == (value)) ? ' ' : '-'))

//Serial.begin(9600);
//Serial.println("Test");

HMS::HMS() {
    this->temperature = {0.0, 0.0};
    this->humidity    = {0.0, 0.0};
    this->pressure    = {0.0, 0.0};

    this->menu        = {0};

    this->builder     = new BUILDER(2, 0, 0, 0);

    this->lcd         = new LCD1602(0x27, 16, 2);
    //this->rtc         = new DS1302(HMS::PIN_DS1302_RST, HMS::PIN_DS1302_DAT, HMS::PIN_DS1302_CLK);
    this->rtc         = new DS1307();
    this->bme         = new BME();

    this->leftLED     = new LED(HMS::PIN_DIOD_LEFT);
    this->rightLED    = new LED(HMS::PIN_DIOD_RIGHT);

    this->leftButton  = new BUTTON(HMS::PIN_BUTTON_LEFT);
    this->rightButton = new BUTTON(HMS::PIN_BUTTON_RIGHT);

    this->timerMenu   = new TIMER(this->menu.displayTime = 10000);
    this->timerMeas   = new TIMER(1000);
}

HMS::~HMS() {
    delete this->builder;

    delete this->lcd;
    delete this->rtc;
    delete this->bme;

    delete this->leftLED;
    delete this->rightLED;

    delete this->leftButton;
    delete this->rightButton;

    delete this->timerMeas;
    delete this->timerMenu;
}

void HMS::init() {
    this->lcd->startup();

    if (this->rtc->isRunning()) {

    }

    //this->rtc->writeProtect(false);
    //this->rtc->halt(false);
    this->bme->startUp();

    DateTime dateTimeBuild = { BUILDER_DATE_TIME_INVERSE, DateTime::DAY_WEDNESDAY };
    uint32_t hash = dateTimeBuild.hour * 60 * 60 + dateTimeBuild.min * 60 + dateTimeBuild.sec;

    this->menu.displayTime = MEM_readUint32(MEM_DISPLAY_TIME);
    this->backlight        = MEM_readUint8(MEM_BACKLIGHT);

    if (hash != MEM_readUint32(MEM_INIT_FLAG)) {
        MEM_writeUint32(0, hash);
        this->rtc->setDateTime(dateTimeBuild);

        this->menu.displayTime = 5000;
        this->backlight        = 50;

        MEM_writeUint32(MEM_DISPLAY_TIME, this->menu.displayTime);
        MEM_writeUint8(MEM_BACKLIGHT, this->backlight);
    }

    delay(500);
    this->timerMeas->start();
}

void HMS::show() {
    switch (this->menu.number) {
        case MENU_GENERAL: {
            if (this->menu.isCapture) {
                DateTime oldDateTime = this->dateTime;

                if (this->rightButton->state()) {
                    this->menu.ptrChangeGeneral = (this->menu.ptrChangeGeneral < 4) ? this->menu.ptrChangeGeneral + 1 : 0;
                }

                switch (this->menu.ptrChangeGeneral) {
                    case 0: {
                        lcd->setCursor(1, 1);
                        if (!this->rightButton->state() && this->leftButton->state()) {
                            this->dateTime.hour = (this->dateTime.hour < 23) ? this->dateTime.hour + 1 : 0;
                        }
                        
                        break;
                    }
                    case 1: {
                        lcd->setCursor(4, 1);
                        if (!this->rightButton->state() && this->leftButton->state()) {
                            this->dateTime.min = (this->dateTime.min < 59) ? this->dateTime.min + 1 : 0;
                        }

                        break;
                    }
                    case 2: {
                        lcd->setCursor(7, 1);
                        if (!this->rightButton->state() && this->leftButton->state()) {
                            this->dateTime.date = (this->dateTime.date < 31) ? this->dateTime.date + 1 : 1;
                        }

                        break;
                    }
                    case 3: {
                        lcd->setCursor(10, 1);
                        if (!this->rightButton->state() && this->leftButton->state()) {
                            this->dateTime.mon = (this->dateTime.mon < 12) ? this->dateTime.mon + 1 : 1;
                        }

                        break;
                    }
                    case 4: {
                        lcd->setCursor(13, 1);
                        if (!this->rightButton->state() && this->leftButton->state()) {
                            this->dateTime.year = (this->dateTime.year < 30) ? this->dateTime.year + 1 : 20;
                        }
      
                        break;
                    }
                    default: {
                        break;
                    }
                }

                if (oldDateTime != this->dateTime) {
                    this->dateTime.sec = 0;
                    this->rtc->setDateTime(this->dateTime);
                }

                lcd->print("__");
                delay(50);
            }

            // --- Measured value
            lcd->setCursor(0, 0);

            // -- Temperature
            lcd->print(SIGN_FLOAT(this->temperature.val));
            lcd->print(abs(this->temperature.val), 1);
            lcd->print("t ");

            // -- Humidity
            lcd->print((uint8_t)this->humidity.val);
            lcd->print("% ");

            // -- Pressure
            lcd->print((uint16_t)this->pressure.val);
            lcd->print("mH");
            // ---

            // --- Date and Time
            lcd->setCursor(0, 1);
            lcd->print(" ");
            lcd->print(this->dateTime.toStringTime());
            lcd->print(" ");
            lcd->print(this->dateTime.toStringDate());
            lcd->print(" ");
            // ---

            break;
        }
        case MENU_DIVIATION: {
            lcd->setCursor(0, 0);
            lcd->print("   Deviation:   ");

            lcd->setCursor(0, 1);
            lcd->print("  ");

            // -- Temperature
            lcd->print(SIGN_FLOAT(this->temperature.div));
            lcd->print(abs(this->temperature.div), 1);
            lcd->print("t ");

            // -- Humidity
            lcd->print(SIGN_FLOAT(this->humidity.div));
            lcd->print(abs(this->humidity.div), 1);
            lcd->print("%   ");

            break;
        }
        case MENU_ABOUT: {
            // -- Name
            lcd->setCursor(0, 0);
            lcd->print("   Home Meteo   ");

            // -- Version
            lcd->setCursor(0, 1);
            lcd->print("Version: v");
            lcd->print(this->builder->versionToString());
            lcd->print(" ");

            break;
        }
        case MENU_PARAM: {
            if (this->menu.isCapture) {
                    uint8_t  oldBacklight   = this->backlight;
                    uint32_t oldDisplayTime = this->menu.displayTime;

                if (this->rightButton->state()) {
                    this->menu.ptrChangeParam = (this->menu.ptrChangeParam < 1) ? this->menu.ptrChangeParam + 1 : 0;
                }

                switch (this->menu.ptrChangeParam) {
                    case 0: {
                        lcd->setCursor(13, 0);
                        if (!this->rightButton->state() && this->leftButton->state()) {
                            this->backlight = (this->backlight < 100) ? this->backlight + 5 : 0;
                        }

                        break;
                    }
                    case 1: {
                        lcd->setCursor(13, 1);
                        if (!this->rightButton->state() && this->leftButton->state()) {
                            this->menu.displayTime = (this->menu.displayTime < 60000) ? this->menu.displayTime + 1000 : 1000;
                        }

                        break;
                    }
                    default: {
                        break;
                    }
                }

                if (oldBacklight != this->backlight) {
                    MEM_writeUint8(MEM_BACKLIGHT, this->backlight);
                }

                if (oldDisplayTime != this->menu.displayTime) {
                    MEM_writeUint32(MEM_DISPLAY_TIME, this->menu.displayTime);

                    this->timerMenu->changeInterval(this->menu.displayTime);
                    this->timerMenu->start();
                }

                lcd->print("__");
                delay(50);
            }

            lcd->setCursor(0, 0);
            lcd->print("Backlight:   ");

            if (this->backlight < 10) {
                lcd->print("0");
            }

            (this->backlight > 99) ? lcd->print(99) : lcd->print(this->backlight);
            lcd->print("%");

            lcd->setCursor(0, 1);
            lcd->print("DisplayTime: ");

            if ((this->menu.displayTime / 1000) < 10) {
                lcd->print("0");
            }

            lcd->print(this->menu.displayTime / 1000);
            lcd->print("s");

            break;
        }
        default: {
            this->menu.number = 0;
            return;
        }
    }

    if (this->timerMenu->isExpired() && !this->menu.isCapture) {
        this->timerMenu->stop();
        this->menu.number = 0;
    }
}

void HMS::updateDateTime() {
    this->rtc->getDateTime(this->dateTime);
}

void HMS::updateMeas() {
    if (!this->timerMeas->isExpired()) {
        return;
    }

    this->bme->refresh();

    this->temperature.val = bme->getTemperature();
    this->humidity.val    = bme->getHumidity();
    this->pressure.val    = bme->getPressure();

    this->checkDivOnNormalTemperature();
    this->checkDivOnNormalHumidity();

    this->timerMeas->start();
}

void HMS::updateBacklight() {
    uint8_t light = this->backlight;

    light = map(light, 0, 100, 0, 255);
    analogWrite(HMS::PIN_LCD_BACKLIGHT, light);
}

void HMS::updateConfiguration(void) {
    if (this->leftButton->state() && this->rightButton->state()) {
        this->menu.isCapture = !this->menu.isCapture;

        if (!this->menu.isCapture) {
            this->menu.number           = 0;
            this->menu.ptrChangeGeneral = 0;
            this->menu.ptrChangeParam   = 0;
        }

        delay(500);
    }
}

void HMS::updateLeftButton() {
    if (this->leftButton->state()) {
        this->previousMenu();
        this->leftLED->turnOn();
    } else {
        this->leftLED->turnOff();
    }
}

void HMS::updateRightButton() {
    if (this->rightButton->state()) {
        this->nextMenu();
        this->rightLED->turnOn();
    } else {
        this->rightLED->turnOff();
    }
}

void HMS::nextMenu() {
    if (this->menu.isCapture) {
        return;
    }

    if (HMS::MENU_COUNT <= this->menu.number) {
        this->menu.number = 0;
        return;
    }

    this->menu.number++;
    this->timerMenu->start();
}

void HMS::previousMenu() {
    if (this->menu.isCapture) {
        return;
    }

    if (!this->menu.number) {
        this->menu.number = HMS::MENU_COUNT - 1;
        return;
    }

    this->menu.number--;
    this->timerMenu->start();
}

void HMS::checkDivOnNormalTemperature() {
    static uint32_t countDiv   = HMS_COUNT_DIVIATION;
    static uint32_t countAlarm = HMS_COUNT_ALARM;

    float temperatureNormalDiv = (HMS_NORMAL_TEMPERATURE * (float)HMS_NORMAL_DIV_IN_PER_TEMPERATURE) / 100.0;
    this->temperature.div       = this->temperature.val - HMS_NORMAL_TEMPERATURE;
    
    if (temperatureNormalDiv < abs(this->temperature.div )) {
        countDiv = (countDiv) ? (countDiv - 1) : countDiv;
    }

    if (countDiv) {
        return;
    }

    if (--countAlarm) {
        this->leftLED->blink();
        return;
    }

    countDiv   = HMS_COUNT_DIVIATION;
    countAlarm = HMS_COUNT_ALARM;
}

void HMS::checkDivOnNormalHumidity() {
    static uint32_t countDiv   = HMS_COUNT_DIVIATION;
    static uint32_t countAlarm = HMS_COUNT_ALARM;

    float humidityNormalDiv = (HMS_NORMAL_HUMIDITY * (float)HMS_NORMAL_DIV_IN_PER_HUMIDITY) / 100.0;
    this->humidity.div      = (float)this->humidity.val - HMS_NORMAL_HUMIDITY;
    
    if (humidityNormalDiv < abs(this->humidity.div)) {
        countDiv = (countDiv) ? (countDiv - 1) : countDiv;
    }

    if (countDiv) {
        return;
    }

    if (--countAlarm) {
        this->rightLED->blink();
        return;
    }

    countDiv   = HMS_COUNT_DIVIATION;
    countAlarm = HMS_COUNT_ALARM;
}
