#include "datatime.h"

DateTime::DateTime() {
  this->year = 99;
  this->mon  = 1;
  this->date = 1;
  this->hour = 0;
  this->min  = 0;
  this->sec  = 0;
  this->day  = DateTime::DAY_SUNDAY;
}

DateTime::DateTime(const uint8_t year, const uint8_t mon, const uint8_t date,
                   const uint8_t hour, const uint8_t min, const uint8_t sec,
                   const Day day) {
  this->year = year;
  this->mon  = mon;
  this->date = date;
  this->hour = hour;
  this->min  = min;
  this->sec  = sec;
  this->day  = day;
}

DateTime& DateTime::operator=(const DateTime &dt) {
  this->year = dt.year;
  this->mon  = dt.mon;
  this->date = dt.date;
  this->hour = dt.hour;
  this->min  = dt.min;
  this->sec  = dt.sec;
  this->day  = dt.day;
}

bool operator==(const DateTime &dt1, const DateTime &dt2) {
    return (
         (dt1.year == dt2.year) 
      && (dt1.mon  == dt2.mon )
      && (dt1.date == dt2.date)
      && (dt1.hour == dt2.hour)
      && (dt1.min  == dt2.min )
      && (dt1.sec  == dt2.sec )
      && (dt1.day  == dt2.day )
    );
}

bool operator!=(const DateTime &dt1, const DateTime &dt2) {
    return !(dt1 == dt2);
}

char * DateTime::toString() {
  static char templateStr[] = "hh:mm:ss Day dd.mm.yy";
  char *dayWeek = templateStr + 9;

  // In ASCII Code
  templateStr[0]  = 0x30 + (this->hour / 10);
  templateStr[1]  = 0x30 + (this->hour % 10);

  templateStr[3]  = 0x30 + (this->min / 10);
  templateStr[4]  = 0x30 + (this->min % 10);

  templateStr[6]  = 0x30 + (this->sec / 10);
  templateStr[7]  = 0x30 + (this->sec % 10);

  (this->day == DAY_SUNDAY   ) ? memcpy(dayWeek, "SUN", 3) :
  (this->day == DAY_MONDAY   ) ? memcpy(dayWeek, "MON", 3) :
  (this->day == DAY_TUESDAY  ) ? memcpy(dayWeek, "TUE", 3) :
  (this->day == DAY_WEDNESDAY) ? memcpy(dayWeek, "WED", 3) :
  (this->day == DAY_THURSDAY ) ? memcpy(dayWeek, "THU", 3) :
  (this->day == DAY_FRIDAY   ) ? memcpy(dayWeek, "FRI", 3) :
  (this->day == DAY_SATURDAY ) ? memcpy(dayWeek, "SAT", 3) : memcpy(dayWeek, "DAY", 3);

  templateStr[13] = 0x30 + (this->date / 10);
  templateStr[14] = 0x30 + (this->date % 10);

  templateStr[16] = 0x30 + (this->mon / 10);
  templateStr[17] = 0x30 + (this->mon % 10);

  templateStr[19] = 0x30 + (this->year / 10);
  templateStr[20] = 0x30 + (this->year % 10);

  return templateStr;
}

char * DateTime::toStringTime() {
  static char templateStrTime[] = "hh:mm";

  // In ASCII Code
  templateStrTime[0]  = 0x30 + (this->hour / 10);
  templateStrTime[1]  = 0x30 + (this->hour % 10);

  templateStrTime[3]  = 0x30 + (this->min / 10);
  templateStrTime[4]  = 0x30 + (this->min % 10);

  return templateStrTime;
}

char * DateTime::toStringDate() {
  static char templateStrDate[] = "dd.mm.yy";

  // In ASCII Code
  templateStrDate[0] = 0x30 + (this->date / 10);
  templateStrDate[1] = 0x30 + (this->date % 10);

  templateStrDate[3] = 0x30 + (this->mon / 10);
  templateStrDate[4] = 0x30 + (this->mon % 10);

  templateStrDate[6] = 0x30 + (this->year / 10);
  templateStrDate[7] = 0x30 + (this->year % 10);

  return templateStrDate;
}
