#include "SensorEC.h"

SensorEC::SensorEC() : analogPin(-1), initialized(false) {}

bool SensorEC::init(int pin) {
    analogPin = pin;
    pinMode(analogPin, INPUT);
    initialized = true;
    Serial.printf("[EC] Sensor initialized on GPIO%d\n", pin);
    return true;
}

float SensorEC::readRawVoltage() {
    if (!initialized) return -1.0;

    const int samples = 50;
    int totalADC = 0;

    for (int i = 0; i < samples; i++) {
        totalADC += analogRead(analogPin);
        delay(5);
    }

    float avgADC = totalADC / (float)samples;
    return (avgADC * 3.3) / 4095.0;
}

float SensorEC::readEC() {
    float voltage = readRawVoltage();
    if (voltage < 0) return -1.0;

    float ec = voltage * 1000.0;
    return ec < 0 ? 0 : ec;
}

bool SensorEC::isInitialized() const {
    return initialized;
}