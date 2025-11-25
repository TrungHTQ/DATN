#include "SensorPH.h"
#include "Calib.h"  // Include calibration constants

SensorPH::SensorPH() {
    analogPin 
    = -1;
    initialized = false;
}

bool SensorPH::init(int pin) {
    analogPin = pin;
    pinMode(analogPin, INPUT);
    initialized = true;

    Serial.printf("===[pH] Sensor initialized on pin GPIO%d===\n", pin);
    return true;
}


float SensorPH::readPH() {
    if (!initialized) {
        Serial.println("[pH] Error: Sensor not initialized");
        return -1.0;
    }
    
    int adcValue = analogRead(analogPin);
    
    // Use calibrated conversion
    float pH = PHCalib::adcToPH(adcValue);
    
    return pH;
}

// Add method to get raw ADC
int SensorPH::readRawADC() {
    // DEBUG CRITICAL INFO
    Serial.printf("[DEBUG] initialized = %s\n", initialized ? "true" : "false");
    Serial.printf("[DEBUG] analogPin = %d\n", analogPin);
    
    if (!initialized) {
        Serial.println("[pH] Error: Sensor not initialized");
        return -1;
    }
    
    int adcValue = analogRead(analogPin);
    Serial.printf("[pH] Raw ADC from pin %d: %d\n", analogPin, adcValue);
    return adcValue;
}

bool SensorPH::isInitialized() const {
    return initialized;
}
