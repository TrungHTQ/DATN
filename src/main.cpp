#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "SensorPH/SensorPH.h"
#include "SensorPH/Calib.h"
#include "SensorPH/Filter_pH.h"
#include "DFRobot_ESP_EC.h"
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include "EEPROM.h"
#include <4G/4G.h>

// WiFi credentials
const char* ssid = "iPhone";
const char* password = "kimdiucute";

const char* serverUrl = "mongodb+srv://trunghabku_db_user:GIxpv920JW1AZOMG@cluster0-quangtrung.0hyvhzg.mongodb.net/";

Adafruit_ADS1115 ads;
DFRobot_ESP_EC ec;
SensorPH phSensor;
SimpleFilter filter;
Internet_4G internet_4G;
char enterCmd[] = "ENTER";
char exitCmd[] = "EXIT";

#define EC_CHANNEL 0 // ADS1115 channel A0

float voltage, ecValue, temperature = 25;
bool calibrating = false;

// ------------------ SETUP ------------------
void setup() {
    Serial.begin(115200);
    internet_4G.set_Begin(44, 43, 115200); // RX=44, TX=43
    delay(3000);

    internet_4G.setup4G();

    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("..");
    }
    Serial.println("\nWiFi connected!");

    phSensor.init(7);
    EEPROM.begin(32);
    Wire.begin(20, 19); // SDA = GPIO8, SCL = GPIO9
    ads.begin();
    ads.setGain(GAIN_ONE); // ±4.096V range, 0.125mV per bit
    ec.begin(); // Initialize EC sensor
    Serial.println("Type 'enterEC' to start EC calibration");
}

// ------------------ LOOP ------------------
void loop() {
    internet_4G.check4G();

    // float rawPH = phSensor.readPH();
    // float filteredPH = filter.addReading(rawPH);

    // Serial.printf("Raw pH: %.2f | Filtered pH: %.2f\n", rawPH, filteredPH);

    // if (filteredPH < 6.5) {
    //     Serial.println("📊 ACIDIC");
    // } else if (filteredPH > 7.5) {
    //     Serial.println("📊 BASIC");
    // } else {
    //     Serial.println("📊 NEUTRAL");
    // }

    static unsigned long timepoint = millis();
    if (millis() - timepoint > 1000U) //time interval: 1s
    {
        timepoint = millis();
        int16_t adcRaw = ads.readADC_SingleEnded(EC_CHANNEL);
        voltage = ads.readADC_SingleEnded(0) / 10;

        Serial.print("ADC Raw: ");
        Serial.println(adcRaw);

        Serial.print("Voltage: ");
        Serial.print(voltage);
        Serial.println(" mV");

        //temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
        Serial.print("temperature:");
        Serial.print(temperature, 1);
        Serial.println("^C");
    }

    ec.calibration(voltage, temperature); // calibration process by Serial CMD

    if (calibrating) {
        Serial.println("-----------------------------");
        Serial.println("Calibrating EC...");
    }

    ecValue = ec.readEC(voltage, temperature);
    Serial.printf("[EC] EC value: %.2f µS/cm\n", ecValue);

    // Nhận lệnh từ Serial để bật/tắt hiệu chuẩn

    delay(5000);
}


// #include "Arduino.h"
// #include "Adafruit_ADS1015.h"
// #include "DFRobot_ESP_EC.h"
// #include "EEPROM.h"

// DFRobot_ESP_EC ec;
// Adafruit_ADS1115 ads;

// float voltage, ecValue, temperature = 25;

// void setup()
// {
// 	Serial.begin(115200);
// 	EEPROM.begin(32);//needed EEPROM.begin to store calibration k in eeprom
// 	ec.begin();//by default lib store calibration k since 10 change it by set ec.begin(30); to start from 30
// 	ads.setGain(GAIN_ONE);
// 	ads.begin();
// }

// void loop()
// {
// 	static unsigned long timepoint = millis();
// 	if (millis() - timepoint > 1000U) //time interval: 1s
// 	{

// 		timepoint = millis();
// 		voltage = ads.readADC_SingleEnded(0) / 10;
// 		Serial.print("voltage:");
// 		Serial.println(voltage, 4);

// 		//temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
// 		Serial.print("temperature:");
// 		Serial.print(temperature, 1);
// 		Serial.println("^C");

// 		ecValue = ec.readEC(voltage, temperature); // convert voltage to EC with temperature compensation
// 		Serial.print("EC:");
// 		Serial.print(ecValue, 4);
// 		Serial.println("ms/cm");
// 	}
// 	ec.calibration(voltage, temperature); // calibration process by Serail CMD
// }

// float readTemperature()
// {
// 	//add your code here to get the temperature from your temperature sensor
// }