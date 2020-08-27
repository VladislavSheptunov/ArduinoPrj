#include "hms.h"

#define RANGE(a,x,b)        ((x >= a) && (x <= b))
#define SIGN_FLOAT(value)   (((0.0 < (value))) ? '+' : ((0.0 == (value)) ? ' ' : '-'))

//Serial.begin(9600);
//Serial.println("Test");

HMS::HMS() {
    this->builder     = new BUILDER(0, 0, 2, 0);

    this->lcd         = new LiquidCrystal_I2C(0x27, 16, 2);
    this->rtc         = new DS1302(HMS::PIN_DS1302_RST, HMS::PIN_DS1302_DAT, HMS::PIN_DS1302_CLK);
    this->dht         = new DHT(HMS::PIN_DHT_DAT, DHT11);

    this->leftLED     = new LED(HMS::PIN_DIOD_LEFT);
    this->rightLED    = new LED(HMS::PIN_DIOD_RIGHT);

    this->leftButton  = new BUTTON(HMS::PIN_BUTTON_LEFT);
    this->rightButton = new BUTTON(HMS::PIN_BUTTON_RIGHT);

    this->timerMenu   = new TIMER(10000);

    this->temperature = 0.0;
    this->humidity    = 0;
    
    this->numMenu     = 0;
}

HMS::~HMS() {
    delete this->builder;

    delete this->lcd;
    delete this->rtc;
    delete this->dht;

    delete this->leftLED;
    delete this->rightLED;

    delete this->leftButton;
    delete this->rightButton;

    delete this->timerMenu;
}

void HMS::init() {
    this->lcd->init();

    this->rtc->writeProtect(false);
    this->rtc->halt(false);
    this->dht->begin();

    DateTime dateTimeBuild = { BUILDER_DATE_TIME_INVERSE, DateTime::DAY_WEDNESDAY };
    uint32_t hash = dateTimeBuild.hour * 60 * 60 + dateTimeBuild.min * 60 + dateTimeBuild.sec;

    if (hash != MEM_readUint32(0)) {
        MEM_writeUint32(0, hash);
        this->rtc->setDateTime(dateTimeBuild);
    }
}

void HMS::enableBacklight(bool flag) {
    this->lcd->setBacklight(flag);
}

void HMS::show() {
    switch (this->numMenu) {
        case MENU_GENERAL: {
            // -- Temperature
            lcd->setCursor(0, 0);
            lcd->print(SIGN_FLOAT(this->temperature));
            lcd->setCursor(1, 0);
            lcd->print(abs(this->temperature), 1);
            lcd->print("t ");

            // -- Humidity
            lcd->setCursor(0, 1);
            lcd->print((uint8_t)this->humidity);
            lcd->print("% ");

            // -- Date and Time
            lcd->setCursor(7, 0);
            lcd->print(this->dateTime.toString());
            lcd->setCursor(4, 1);
            lcd->print(this->dateTime.toString() + 9);

            break;
        }
        case MENU_PARAM: {
            // -- Temperature Diviation
            lcd->setCursor(0, 0);
            lcd->print("DeviationT: ");
            lcd->print(SIGN_FLOAT(this->temperatureDiv));
            lcd->print(abs(this->temperatureDiv), 1);

            // -- Humidity Diviation
            lcd->setCursor(0, 1);
            lcd->print("DeviationH: ");
            lcd->print(SIGN_FLOAT(this->humidityDiv));
            lcd->print(abs(this->humidityDiv), 1);

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
        default: {
            this->numMenu = 0;
            return;
        }
    }

    if (this->timerMenu->isExpired()) {
        this->timerMenu->stop();
        this->numMenu = 0;
    }
}

void HMS::updateDateTime() {
    this->dateTime = this->rtc->getDateTime();
}

void HMS::updateTemperature() {
    const size_t countMeas = 3;

    static size_t it = 0;
    static float bufferMeas[countMeas];

    float temperature = this->dht->readTemperature();

    if (temperature > 99.0) {
        temperature = 99.0;
    }

    if (temperature < -99.0) {
        temperature = -99.0;
    }

    bufferMeas[it++] = temperature;

    if (it != countMeas) {
        return;
    }

    it = 0;
    this->temperature = sqrt((pow(bufferMeas[0], 2) + pow(bufferMeas[1], 2) + pow(bufferMeas[2], 2)) / countMeas);
    this->checkDivOnNormalTemperature();
}

void HMS::updateHumidity() {
    const size_t countMeas = 3;

    static size_t it = 0;
    static float bufferMeas[countMeas];

    float humidity = this->dht->readHumidity();
    humidity       = (humidity < 99.0) ? humidity : 99.0;

    bufferMeas[it++] = humidity;

    if (it != countMeas) {
        return;
    }

    it = 0;
    this->humidity = sqrt((pow(bufferMeas[0], 2) + pow(bufferMeas[1], 2) + pow(bufferMeas[2], 2)) / countMeas);
    this->checkDivOnNormalHumidity();
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
    if (HMS::MENU_COUNT <= this->numMenu) {
        this->numMenu = 0;
        return;
    }

    this->numMenu++;
    this->timerMenu->start();
}

void HMS::previousMenu() {
    if (!this->numMenu) {
        this->numMenu = HMS::MENU_COUNT - 1;
        return;
    }

    this->numMenu--;
    this->timerMenu->start();
}

void HMS::checkDivOnNormalTemperature() {
    static uint32_t countDiv   = HMS_COUNT_DIVIATION;
    static uint32_t countAlarm = HMS_COUNT_ALARM;

    float temperatureNormalDiv = (HMS_NORMAL_TEMPERATURE * (float)HMS_NORMAL_DIV_IN_PER_TEMPERATURE) / 100.0;
    this->temperatureDiv       = this->temperature - HMS_NORMAL_TEMPERATURE;
    
    if (temperatureNormalDiv < abs(this->temperatureDiv)) {
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
    this->humidityDiv       = (float)this->humidity - HMS_NORMAL_HUMIDITY;
    
    if (humidityNormalDiv < abs(this->humidityDiv)) {
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
