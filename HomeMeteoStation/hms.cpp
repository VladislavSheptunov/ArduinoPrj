#include "hms.h"

#include "builder.h"
#include "memory.h"

#define RANGE(a,x,b)  ((x >= a) && (x <= b))

// -- PIN`s
#define PIN_DHT_DAT    2

#define PIN_DIOD_LEFT  3
#define PIN_DIOD_RIGHT 4

#define PIN_DS1302_RST 5
#define PIN_DS1302_DAT 6
#define PIN_DS1302_CLK 7

// --

HMS::HMS() {
    // Serial.begin(9600);
    // Serial.println(t.toString());

    this->lcd = new LiquidCrystal_I2C(0x27, 16, 2);
    this->rtc = new DS1302(PIN_DS1302_RST, PIN_DS1302_DAT, PIN_DS1302_CLK);
    this->dht = new DHT(PIN_DHT_DAT, DHT11);

    pinMode(PIN_DIOD_LEFT,  OUTPUT);
    pinMode(PIN_DIOD_RIGHT, OUTPUT);
}

HMS::~HMS() {
    delete this->lcd;
    delete this->rtc;
    delete this->dht;
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
    char sign = ((0.0 < this->temperature) ? '+' : ((0.0 == this->temperature) ? ' ' : '-'));

    // -- Temperature
    lcd->setCursor(0, 0);
    lcd->print(sign);
    lcd->setCursor(1, 0);
    lcd->print(abs(this->temperature), 1);
    lcd->print("t");

    // -- Humidity
    lcd->setCursor(0, 1);
    lcd->print(this->humidity);
    lcd->print("%");

    // -- Date and Time
    lcd->setCursor(7, 0);
    lcd->print(this->dateTime.toString());
    lcd->setCursor(4, 1);
    lcd->print(this->dateTime.toString() + 9);
}

void HMS::updateDateTime() {
    this->dateTime = this->rtc->getDateTime();
}

void HMS::updateTemperature() {
    static float before = 0;

    float temperature = this->dht->readTemperature();

    if (temperature > 99.0) {
        this->temperature = 99.0;
    }
    else if (temperature < -100.0) {
        this->temperature = -99.0;
    }
    else {
        this->temperature = temperature;
    }

    if (before != temperature) {
        before = temperature;
        digitalWrite(PIN_DIOD_LEFT,  HIGH);
        digitalWrite(PIN_DIOD_RIGHT, HIGH);
    } else {
        digitalWrite(PIN_DIOD_LEFT,  LOW);
        digitalWrite(PIN_DIOD_RIGHT, LOW);
    }
}

void HMS::updateHumidity() {
    this->humidity = (uint8_t)this->dht->readHumidity();
    this->humidity = (this->humidity < 99) ? this->humidity : 99;
}

void HMS::blink() {
    static bool state = true;

    if (state) {
        state = false;
        digitalWrite(PIN_DIOD_LEFT,  HIGH);
        digitalWrite(PIN_DIOD_RIGHT, HIGH);
    } else {
        state = true;
        digitalWrite(PIN_DIOD_LEFT,  LOW);
        digitalWrite(PIN_DIOD_RIGHT, LOW);
    }
}
