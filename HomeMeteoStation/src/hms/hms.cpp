#include "hms.h"

#define RANGE(a,x,b)  ((x >= a) && (x <= b))

HMS::HMS() {
    //Serial.begin(9600);
    //Serial.println("Column2");

    this->builder     = new BUILDER(0, 0, 1, 0);

    this->lcd         = new LiquidCrystal_I2C(0x27, 16, 2);
    this->rtc         = new DS1302(HMS_PIN_DS1302_RST, HMS_PIN_DS1302_DAT, HMS_PIN_DS1302_CLK);
    this->dht         = new DHT(HMS_PIN_DHT_DAT, DHT11);

    this->leftLED     = new LED(HMS_PIN_DIOD_LEFT);
    this->rightLED    = new LED(HMS_PIN_DIOD_RIGHT);

    this->leftButton  = new BUTTON(HMS_PIN_BUTTON_LEFT);
    this->rightButton = new BUTTON(HMS_PIN_BUTTON_RIGHT);

    //this->timerMenu   = new TIMER(TIMER::TIMER_INTERVAL_MIN);
    this->timerMenu   = new TIMER(5000);

    this->setControlOnTemperature(10);
    this->setControlOnHumidity(20);

    this->numMenu = 0;
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

    DateTime dateTimeBuild = { BUILDER_DATE_TIME_INVERSE, DateTime::DAY_TUESDAY };
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
        case 0: {
            char sign = ((0.0 < this->temperature) ? '+' : ((0.0 == this->temperature) ? ' ' : '-'));

            // -- Temperature
            lcd->setCursor(0, 0);
            lcd->print(sign);
            lcd->setCursor(1, 0);
            lcd->print(abs(this->temperature), 1);
            lcd->print("t ");

            // -- Humidity
            lcd->setCursor(0, 1);
            lcd->print(this->humidity);
            lcd->print("% ");

            // -- Date and Time
            lcd->setCursor(7, 0);
            lcd->print(this->dateTime.toString());
            lcd->setCursor(4, 1);
            lcd->print(this->dateTime.toString() + 9);

            break;
        }
        case 1: {   // -- About
            // -- Name
            lcd->setCursor(0, 0);
            lcd->print("   Home Meteo   ");

            // -- Version
            lcd->setCursor(0, 1);
            lcd->print("Version: ");
            lcd->print(this->builder->versionToString());

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
    float temperature = this->dht->readTemperature();

    if (temperature > 99.0) {
        this->temperature = 99.0;
    } else if (temperature < -100.0) {
        this->temperature = -99.0;
    } else {
        this->temperature = temperature;
    }

    if (this->isDivOnNormalTemperature()) {
        this->leftLED->turnOn();
    }
}

void HMS::updateHumidity() {
    this->humidity = (uint8_t)this->dht->readHumidity();
    this->humidity = (this->humidity < 99) ? this->humidity : 99;

    if (this->isDivOnNormalHumidity()) {
        this->rightLED->turnOn();
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

void HMS::setControlOnTemperature(uint8_t diviation) {
    if (!RANGE(0, diviation, 100)) {
        return;  
    }

    this->temperatureDiv = diviation;
}

bool HMS::isDivOnNormalTemperature(void) {
    float startRange = HMS_NORMAL_TEMPERATURE - ((HMS_NORMAL_TEMPERATURE * this->temperatureDiv) / 100.0);
    float endRange   = HMS_NORMAL_TEMPERATURE + ((HMS_NORMAL_TEMPERATURE * this->temperatureDiv) / 100.0);

    if (RANGE(startRange, this->temperature, endRange)) {
        return false;
    }

    return true;
}

void HMS::setControlOnHumidity(uint8_t diviation) {
    if (!RANGE(0, diviation, 100)) {
        return;  
    }

    this->humidityDiv = diviation;
}

bool HMS::isDivOnNormalHumidity(void) {
    float startRange = HMS_NORMAL_HUMIDITY - ((HMS_NORMAL_HUMIDITY * (float)this->humidityDiv) / 100.0);
    float endRange   = HMS_NORMAL_HUMIDITY + ((HMS_NORMAL_HUMIDITY * (float)this->humidityDiv) / 100.0);

    if (RANGE(startRange, (float)this->humidity, endRange)) {
        return  false;
    }

    return true;
}

void HMS::nextMenu(void) {
    if (HMS_MENU_ITEMS <= this->numMenu) {
        this->numMenu = 0;
        return;
    }

    this->numMenu++;
    this->timerMenu->start();
}

void HMS::previousMenu(void) {
    if (!this->numMenu) {
        this->numMenu = HMS_MENU_ITEMS - 1;
        return;
    }

    this->numMenu--;
    this->timerMenu->start();
}
