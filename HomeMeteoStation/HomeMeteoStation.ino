#include "hms.h"

HMS hms;

void setup() {
    hms.init();
    hms.enableBacklight(true);

    attachInterrupt(0, ISR_leftButton,  CHANGE);
    attachInterrupt(1, ISR_rightButton, CHANGE);
}

void loop() {
    hms.updateDateTime();
    hms.updateTemperature();
    hms.updateHumidity();

    delay(300);
    hms.show();
}

void ISR_leftButton() {
    hms.updateLeftButton();
}

void ISR_rightButton() {
    hms.updateRightButton();
}
