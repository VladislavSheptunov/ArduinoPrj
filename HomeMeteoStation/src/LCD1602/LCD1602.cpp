#include "LCD1602.h"

#include <avr/pgmspace.h>

const PROGMEM uint8_t bigSymbols[] = {
    0xFF, 0x00, 0xFF, 0xFF, 0x01, 0xFF, //0
    0x00, 0xFF, 0x20, 0x01, 0xFF, 0x01, //1
    0x00, 0x00, 0xFF, 0xFF, 0x02, 0x01, //2
    0x00, 0x02, 0xFF, 0x01, 0x01, 0xFF, //3
    0xFF, 0x20, 0xFF, 0x00, 0x00, 0xFF, //4
    0xFF, 0x02, 0x02, 0x01, 0x01, 0xFF, //5
    0xFF, 0x02, 0x02, 0xFF, 0x01, 0xFF, //6
    0x00, 0x00, 0xFF, 0x20, 0x20, 0xFF, //7
    0xFF, 0x02, 0xFF, 0xFF, 0x01, 0xFF, //8
    0xFF, 0x02, 0xFF, 0x01, 0x01, 0xFF, //9
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20 //'.'
};

LCD1602::LCD1602(uint8_t addr, uint8_t cols, uint8_t rows)
    : LiquidCrystal_I2C(addr, cols, rows) {
}

LCD1602::~LCD1602() {

}

void LCD1602::startup() {
    uint8_t codes[8] = {0};
    
    this->init();
    this->backlight();
    this->clear();
    this->home();

    codes[0] = codes[1] = 0xFF;
    this->createChar(0, codes);

    codes[6] = codes[7] = 0xFF;
    this->createChar(2, codes);

    codes[0] = codes[1] = 0x00;
    this->createChar(1, codes);
}

void LCD1602::printCustomSymbol(BigCustomSymbol value, uint8_t pos) {
    this->setCursor(pos, 0);
    for (size_t i = 0; i < 3; ++i) {
        this->write(pgm_read_byte_near((bigSymbols + value * 6 + i)));
    }

    this->setCursor(pos, 1);
    for (size_t i = 0; i < 3; ++i) {
        this->write(pgm_read_byte_near(bigSymbols + value * 6 + i + 3));
    }
}

void LCD1602::printCustomSymbol(SmallCustomSymbol value) {

}
