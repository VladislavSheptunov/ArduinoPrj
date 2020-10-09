#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "DS1302.h"

#include <stdint.h>
#include <string.h>

namespace {

typedef enum {
  REG_SECOND        = 0,
  REG_MINUTE        = 1,
  REG_HOUR          = 2,
  REG_DATE          = 3,
  REG_MONTH         = 4,
  REG_DAY           = 5,
  REG_YEAR          = 6,
  REG_WRITE_PROTECT = 7,

  // The RAM register space follows the clock register space.
  REG_RAM_ADDR0    = 32
} Register;

typedef enum  {
  CMD_CLOCK_BURST_READ  = 0xBF,
  CMD_CLOCK_BURST_WRITE = 0xBE,
  CMD_RAM_BURST_READ    = 0xFF,
  CMD_RAM_BURST_WRITE   = 0xFE
} Command;

class SPISession {
  public:
    SPISession(const int pinRST, const int pinDAT, const int pinCLK)
      : pinRST(pinRST), pinDAT(pinDAT), pinCLK(pinCLK) {
      digitalWrite(pinCLK, LOW);
      digitalWrite(pinRST, HIGH);
      delayMicroseconds(4);
    }

    ~SPISession() {
      digitalWrite(pinRST, LOW);
      delayMicroseconds(4);
    }

  private:
    const int pinRST;
    const int pinDAT;
    const int pinCLK;
};

uint8_t bcdToDec(const uint8_t bcd) {
  return (10 * ((bcd & 0xF0) >> 4) + (bcd & 0x0F));
}

uint8_t decToBcd(const uint8_t dec) {
  return ((dec / 10) << 4) | (dec % 10);
}

// Returns the hour in 24-hour format from the hour register value.
uint8_t hourFromRegisterValue(const uint8_t value) {
  uint8_t adjustment = (value & 128) ? (12 * ((value & 32) >> 5)) : (10 * ((value & (32 + 16)) >> 4));

  return (value & 15) + adjustment;
}

}  // namespace

DS1302::DS1302(const uint8_t pinRST, const uint8_t pinDAT, const uint8_t pinCLK) {
  this->pinRST = pinRST;
  this->pinDAT = pinDAT;
  this->pinCLK = pinCLK;

  pinMode(this->pinRST, OUTPUT);
  pinMode(this->pinDAT, INPUT);
  pinMode(this->pinCLK, OUTPUT);

  digitalWrite(this->pinRST, LOW);
  digitalWrite(this->pinCLK, LOW);
}

void DS1302::writeOut(const uint8_t value, bool readAfter) {
  pinMode(pinDAT, OUTPUT);

  for (int i = 0; i < 8; ++i) {
    digitalWrite(pinDAT, (value >> i) & 1);
    delayMicroseconds(1);
    digitalWrite(pinCLK, HIGH);
    delayMicroseconds(1);

    if (readAfter && i == 7) {
      // We're about to read data -- ensure the pin is back in input mode
      // before the clock is lowered.
      pinMode(pinDAT, INPUT);
    } else {
      digitalWrite(pinCLK, LOW);
      delayMicroseconds(1);
    }
  }
}

uint8_t DS1302::readIn() {
  uint8_t valueInput = 0;
  uint8_t bit = 0;
  pinMode(pinDAT, INPUT);

  // Bits from the DS1302 are output on the falling edge of the clock
  // cycle. This is called after readIn (which will leave the clock low) or
  // writeOut(..., true) (which will leave it high).
  for (int i = 0; i < 8; ++i) {
    digitalWrite(pinCLK, HIGH);
    delayMicroseconds(1);
    digitalWrite(pinCLK, LOW);
    delayMicroseconds(1);

    bit = digitalRead(pinDAT);
    valueInput |= (bit << i);  // Bits are read LSB first.
  }

  return valueInput;
}

uint8_t DS1302::readRegister(const uint8_t reg) {
  const SPISession s(pinRST, pinDAT, pinCLK);

  writeOut((0x81 | (reg << 1)), true);

  return readIn();
}

void DS1302::writeRegister(const uint8_t reg, const uint8_t value) {
  const SPISession s(pinRST, pinDAT, pinCLK);

  writeOut((0x80 | (reg << 1)));
  writeOut(value);
}

void DS1302::writeProtect(const bool enable) {
  writeRegister(REG_WRITE_PROTECT, (enable << 7));
}

void DS1302::halt(const bool enable) {
  uint8_t sec = readRegister(REG_SECOND);
  sec &= ~(1 << 7);
  sec |= (enable << 7);
  writeRegister(REG_SECOND, sec);
}

void DS1302::getDateTime(DateTime &dt) {
  const SPISession s(pinRST, pinDAT, pinCLK);
  writeOut(CMD_CLOCK_BURST_READ, true);

  dt.sec  = bcdToDec(readIn() & 0x7F);
  dt.min  = bcdToDec(readIn());
  dt.hour = hourFromRegisterValue(readIn());
  dt.date = bcdToDec(readIn());
  dt.mon  = bcdToDec(readIn());
  dt.day  = static_cast<DateTime::Day>(bcdToDec(readIn()));
  dt.year = bcdToDec(readIn());
}

void DS1302::setDateTime(DateTime &dt) {
  // We want to maintain the Clock Halt flag if it is set.
  const uint8_t ch_value = readRegister(REG_SECOND) & 0x80;
  const SPISession s(pinRST, pinDAT, pinCLK);

  writeOut(CMD_CLOCK_BURST_WRITE);
  writeOut(ch_value | decToBcd(dt.sec));
  writeOut(decToBcd(dt.min));
  writeOut(decToBcd(dt.hour));
  writeOut(decToBcd(dt.date));
  writeOut(decToBcd(dt.mon));
  writeOut(decToBcd(static_cast<uint8_t>(dt.day)));
  writeOut(decToBcd(dt.year));
  // All clock registers *and* the WP register have to be written for the DateTime
  // to be set.
  writeOut(0);  // Write protection register.
}

void DS1302::writeRam(const uint8_t address, const uint8_t value) {
  if (address >= sizeRAM) {
    return;
  }

  writeRegister(REG_RAM_ADDR0 + address, value);
}

uint8_t DS1302::readRam(const uint8_t address) {
  if (address >= sizeRAM) {
    return 0;
  }

  return readRegister(REG_RAM_ADDR0 + address);
}

void DS1302::writeRamBulk(const uint8_t* const data, int len) {
  if (len <= 0) {
    return;
  }

  if (len > sizeRAM) {
    len = sizeRAM;
  }

  const SPISession s(pinRST, pinDAT, pinCLK);

  writeOut(CMD_RAM_BURST_WRITE);
  for (int i = 0; i < len; ++i) {
    writeOut(data[i]);
  }
}

void DS1302::readRamBulk(uint8_t* const data, int len) {
  if (len <= 0) {
    return;
  }

  if (len > sizeRAM) {
    len = sizeRAM;
  }

  const SPISession s(pinRST, pinDAT, pinCLK);

  writeOut(CMD_RAM_BURST_READ, true);
  for (int i = 0; i < len; ++i) {
    data[i] = readIn();
  }
}
