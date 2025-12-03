#ifndef MODULE4G_H
#define MODULE4G_H

#include <Arduino.h>
#define TINY_GSM_MODEM_SIM7600
#include <TinyGsmClient.h>
#include <PubSubClient.h>

extern TinyGsm modem;
extern TinyGsmClient client;
extern PubSubClient mqtt;

bool FourG_init(const char* apn);
bool MQTT_init(const char* broker, const char* clientID);
bool MQTT_publish(const char* topic, const char* payload);
void FourG_loop();   // để xử lý mqtt.loop()

#endif
