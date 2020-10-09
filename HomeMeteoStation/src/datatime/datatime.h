#ifndef _DATA_TIME_H_
#define _DATA_TIME_H_

#include <stdint.h>
#include <string.h>

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

#endif  // _DATA_TIME_H_
