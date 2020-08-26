#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>
#include <stdbool.h>

#include "Arduino.h"

class TIMER {
public:
    enum {
        TIMER_INTERVAL_MSEC = 1ul,
        TIMER_INTERVAL_SEC  = TIMER_INTERVAL_MSEC * 1000ul,
        TIMER_INTERVAL_MIN  = TIMER_INTERVAL_SEC * 60ul,
        TIMER_INTERVAL_HOUR = TIMER_INTERVAL_MIN * 60ul
    } TimerInterval;

    TIMER(uint64_t interval);
    ~TIMER();

    void start(void);
    void stop(void);

    void changeInterval(uint64_t interval);

    bool isActive(void);
    bool isExpired(void);
private:
    uint64_t    countTick;
    uint64_t    intervalTick;
    bool        status;
};

#endif // _TIMER_H_
