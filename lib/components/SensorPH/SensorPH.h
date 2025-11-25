#ifndef SENSOR_PH_H
#define SENSOR_PH_H

#include <Arduino.h>

class SensorPH {
private:
    int analogPin;
    bool initialized;
    
public:
    SensorPH();
    bool init(int pin);
    
    int readRawADC();           // Get raw ADC value
    float readRawVoltage();     // Get voltage
    float readPH();             // Get calibrated pH
    
    bool isInitialized() const;
};

#endif
