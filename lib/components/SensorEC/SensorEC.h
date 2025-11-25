#ifndef SENSOR_EC_H
#define SENSOR_EC_H

#include <Arduino.h>

class SensorEC {
public:
    SensorEC();
    bool init(int pin);
    float readRawVoltage();
    float readEC();
    bool isInitialized() const;

private:
    int analogPin;
    bool initialized;
};

#endif