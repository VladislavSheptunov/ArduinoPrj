#ifndef _LCD1602_H_
#define _LCD1602_H_

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <stdint.h>

class LCD1602 : public LiquidCrystal_I2C {
  public:
    enum SmallCustomSymbol : int {
      SCS_NUM_1
    };

    enum BigCustomSymbol : int {
      BCS_NUM_0  = 0,
      BCS_NUM_1  = 1,
      BCS_NUM_2  = 2,
      BCS_NUM_3  = 3,
      BCS_NUM_4  = 4,
      BCS_NUM_5  = 5,
      BCS_NUM_6  = 6,
      BCS_NUM_7  = 7,
      BCS_NUM_8  = 8,
      BCS_NUM_9  = 9,

      BCS_NUM_DOT = 10,
    };

    LCD1602(uint8_t addr, uint8_t cols, uint8_t rows);
    ~LCD1602();

    void startup();

    void printCustomSymbol(BigCustomSymbol value, uint8_t pos);
    void printCustomSymbol(SmallCustomSymbol value);
};

#endif // _LCD1602_H_
