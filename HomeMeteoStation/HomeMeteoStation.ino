#include "hms.h"

HMS hms;

void setup() {
    hms.init();
    hms.enableBacklight(true);
}

void loop() {
    hms.updateDateTime();
    hms.updateTemperature();
    hms.updateHumidity();

    delay(300);
    hms.show();
}
