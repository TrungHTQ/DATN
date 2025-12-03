#define TINY_GSM_MODEM_SIM7600
#include "4G.h"
#include <TinyGsmClient.h>
#include <PubSubClient.h>

HardwareSerial SerialAT(1);
TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

static const char* mqttClientID = nullptr;

bool FourG_init(const char* apn)
{
    Serial.println("[4G] Khởi động SerialAT...");
    SerialAT.begin(115200, SERIAL_8N1, 18, 17);
    delay(3000);

    // Lặp đến khi restart modem thành công
    while (!modem.restart()) {
        Serial.println("[4G] ❌ Restart thất bại! Đang thử lại...");
        delay(2000);
    }

    Serial.println("[4G] Chờ mạng...");
    while (!modem.waitForNetwork(10000)) {
        Serial.println("[4G] ❌ Không tìm thấy mạng! Đang thử lại...");
        delay(2000);
    }

    Serial.println("[4G] Đã lên sóng!");

    Serial.println("[4G] Kết nối GPRS...");
    while (!modem.gprsConnect(apn)) {
        Serial.println("[4G] ❌ Không kết nối GPRS! Đang thử lại...");
        delay(2000);
    }

    Serial.println("[4G] ✔ GPRS OK!");
    return true;
}

bool MQTT_init(const char* broker, const char* clientID)
{
    mqtt.setServer(broker, 1883);
    mqttClientID = clientID;

    Serial.println("[MQTT] Kết nối MQTT...");
    // Lặp đến khi kết nối MQTT thành công
    while (!mqtt.connect(clientID)) {
        Serial.println("[MQTT] ❌ Kết nối thất bại! Đang thử lại...");
        delay(2000);
    }

    Serial.println("[MQTT] ✔ MQTT Connected!");
    return true;
}

bool MQTT_publish(const char* topic, const char* payload)
{
    if (!mqtt.connected()) {
        Serial.println("[MQTT] Mất kết nối → thử reconnect...");
        while (!mqtt.connect(mqttClientID)) {
            Serial.println("[MQTT] ❌ Reconnect thất bại! Đang thử lại...");
            delay(2000);
        }
    }

    return mqtt.publish(topic, payload);
}

void FourG_loop()
{
    mqtt.loop();
}