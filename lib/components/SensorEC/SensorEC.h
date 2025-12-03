#ifndef SENSOR_EC_H
#define SENSOR_EC_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <DFRobot_ESP_EC.h>
#include <EEPROM.h>

class SensorEC {
public:
    SensorEC();
    void begin(int sda, int scl, uint8_t channel);
    void update();
    void handleCommand(String cmd);
    float readEC() const;
    float getVoltage() const;

    void setTemperature(float temp) { temperature = temp; }

private:
    Adafruit_ADS1115 ads;
    DFRobot_ESP_EC ec;
    uint8_t ecChannel;
    float voltage;
    float temperature;
    bool calibrating;

    static const uint8_t FILTER_SIZE = 10;
    float ecBuffer[FILTER_SIZE];
    uint8_t bufferIndex;
    bool bufferFilled;

    float filterEC(float value);
};

#endif
