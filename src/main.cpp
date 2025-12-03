#define EC_CHANNEL 1

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <DFRobot_ESP_EC.h>
#include <EEPROM.h>
#include "SensorEC/SensorEC.h"
#include <4G/4G.h>

const char* mqtt_server = "test.mosquitto.org";
const char* mqtt_topic  = "test/topic";

SensorEC ecSensor;

void setup() {
  Serial.begin(115200);
  delay(1000);

  FourG_init("internet");
  MQTT_init(mqtt_server, "ESP32Client");
  ecSensor.begin(10, 11, EC_CHANNEL);
  
//   ecSensor.setTemperature(26.0);
}

void loop() {
    FourG_loop();

    ecSensor.update();

    static unsigned long t = millis();
    if (millis() - t > 5000) {
        t = millis();   
        float ec = ecSensor.readEC();
        mqtt.publish(mqtt_topic, String(ec, 4).c_str());
    }

    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        ecSensor.handleCommand(cmd);
    }

    delay(5000);
}