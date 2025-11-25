#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>

class Internet_4G {
public:
    Internet_4G(HardwareSerial &port = Serial1) : _port(port) {}
    void set_Begin(int rxPin, int txPin, uint32_t baud = 115200);
    void setup4G();
    void check4G();
    void sendAndPrint(const char* cmd);
private:
    HardwareSerial &_port;
    void flushInput();
};