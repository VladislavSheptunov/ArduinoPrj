#ifndef _DS1302_H_
#define _DS1302_H_

#include <stdint.h>

class DateTime {
public:
    enum Day {
      DAY_SUNDAY    = 1,
      DAY_MONDAY    = 2,
      DAY_TUESDAY   = 3,
      DAY_WEDNESDAY = 4,
      DAY_THURSDAY  = 5,
      DAY_FRIDAY    = 6,
      DAY_SATURDAY  = 7
    };

    uint8_t sec;  // 0...59
    uint8_t min;  // 0...59
    uint8_t hour; // 0...23
    uint8_t date; // 1...31
    uint8_t mon;  // 1...12
    Day day;      // 1...7
    uint8_t year; // 0...99

    DateTime();
    DateTime(uint8_t year, uint8_t mon, uint8_t date, uint8_t hour, uint8_t min, uint8_t sec, Day day);

    DateTime& operator=(const DateTime &dateTime);

    friend bool operator==(const DateTime &dt1, const DateTime &dt2);
    friend bool operator!=(const DateTime &dt1, const DateTime &dt2);
    
    char* toString();
    char* toStringTime();
    char* toStringDate();
};

// An interface to the Dallas Semiconductor DS1302 Real Time Clock (RTC) chip.
//
// Accessing and setting individual components of the time are not supported in
// this interface as doing so can lead to errors if the time changes as it is
// being read or modified. Instead, using DS1302::time() guarantees safe reads
// and writes using the DS1302's burst mode feature.
class DS1302 {
public:
    // Size of the DS1302's RAM storage, in bytes.
    static const int sizeRAM = 31;
    
    DS1302(uint8_t pinRST, uint8_t pinDAT, uint8_t pinCLK);
    
    // Enables or disables write protection on the chip.
    //
    // While write protection is enabled, all attempts to write to the chip (e.g.,
    // setting the time) will have no effect.
    //
    // The DS1302 datasheet does not define the initial state of write protection,
    // so this method should be called at least once when initializing a device
    // for the first time.
    //
    // Args:
    //   enable: true to enable write protection.
    void writeProtect(bool enable);
    
    // Sets or clears Clock Halt flag on the chip.
    //
    // Enabling the Clock Halt flag disables the DS1302's clock oscillator and
    // places it into a low-power standby mode. While in this mode, the time does
    // not progress. The time can still be read from the chip while it is halted,
    // however.
    //
    // The DS1302 datasheet does not define the initial state of the Clock Halt
    // flag, so this method should be called at least once when initializing a
    // device for the first time.
    //
    // Args:
    //   value: true to set halt flag, false to clear.
    void halt(bool value);
    
    // Returns the current time and date in a Time object.
    //
    // Returns:
    //   Current time as Time object.
    DateTime getDateTime();
    
    // Sets the time and date to the instant specified in a given Time object.
    //
    // The time will not be set if write protection is enabled on the
    // chip. Setting the time with this function has no effect on the Clock Halt
    // flag.
    //
    // Args:
    //   t: Time instant.
    void setDateTime(DateTime t);
    
    // Writes a byte to RAM.
    //
    // The DS1302 has 31 bytes (kRamSize) of static RAM that can store arbitrary
    // data as long as the device has power.
    //
    // Writes to invalid addresses have no effect.
    //
    // Args:
    //   address: RAM address in [0, kRamSize).
    //   value: Byte to write to the RAM address.
    void writeRam(uint8_t address, uint8_t value);
    
    // Reads a byte from RAM.
    //
    // Reads of invalid addresses return 0.
    //
    // Args:
    //   address: RAM address in [0, kRamSize).
    //
    // Returns:
    //   Byte from RAM or 0 if the address is invalid.
    uint8_t readRam(uint8_t address);
    
    // Writes 'len' bytes into RAM from '*data', starting at RAM address 0.
    //
    // Args:
    //   data: Input data.
    //   len: Number of bytes of '*data' to read. Must be <= kRamSize.
    void writeRamBulk(const uint8_t* data, int len);
    
    // Reads 'len' bytes from RAM into '*data', starting at RAM address 0.
    //
    // Args:
    //   data: Output data.
    //   len: Number of bytes of RAM to read. Must be <= kRamSize.
    void readRamBulk(uint8_t* data, int len);
    
    // Reads register byte value.
    //
    // Args:
    //   reg: register number
    //
    // Returns:
    //   register value
    uint8_t readRegister(uint8_t reg);
    
    // Writes byte into register.
    //
    // Args:
    //   reg: register number
    //   value: byte to write
    void writeRegister(uint8_t reg, uint8_t value);
    
private:
    uint8_t pinRST;
    uint8_t pinDAT;
    uint8_t pinCLK;
    
    // Shifts out a value to the IO pin.
    //
    // Side effects: sets io_pin_ as OUTPUT.
    //
    // Args:
    //   value: byte to shift out
    //   readAfter: whether this will be followed by a read; if so, it
    //     will leave io_pin_ as INPUT.
    void writeOut(uint8_t value, bool readAfter = false);
    
    // Reads in a byte from the IO pin.
    //
    // Side effects: sets io_pin_ to INPUT.
    //
    // Returns:
    //   byte read in
    uint8_t readIn();
};

#endif  // DS1302_H_
