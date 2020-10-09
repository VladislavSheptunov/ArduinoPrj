#include "bme.h"

#include <stdint.h>
#include <string.h>

BME::BME() {}

BME::~BME() {}

void BME::startUp() {
    //Wire.begin(0x3C);
    sensor.begin(); 
}

void BME::refresh() {
    sensor.refresh();
}

void BME::endUp() {

}

float BME::getTemperature() {
    return this->sensor.temperature;
}

float BME::getHumidity() {
    return this->sensor.humidity;
}

float BME::getPressure() {
    return this->sensor.pressure / 133.3224;
}
