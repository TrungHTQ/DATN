#include "sensorEC.h"

SensorEC::SensorEC()
: ecChannel(0), voltage(0), temperature(25.0), calibrating(false),
  bufferIndex(0), bufferFilled(false)
{
    for (uint8_t i = 0; i < FILTER_SIZE; i++) ecBuffer[i] = 0;
}

void SensorEC::begin(int sda, int scl, uint8_t channel) {
    EEPROM.begin(32);
    ecChannel = channel;

    Wire.begin(sda, scl);
    ads.begin();
    ads.setGain(GAIN_ONE);  // ±4.096V, 0.125mV/bit

    ec.begin();

    Serial.println("[EC] Init OK");
}

void SensorEC::handleCommand(String cmd) {
    if (cmd == "enterec") {
        calibrating = true;
        ec.calibration(voltage, temperature, "ENTEREC");
        Serial.println("[EC] Calibration MODE ON");
    }
    else if (cmd == "calec") {
        ec.calibration(voltage, temperature, "CALEC");
        Serial.println("[EC] CALIBRATED");
    }
    else if (cmd == "exitec") {
        calibrating = false;
        ec.calibration(voltage, temperature, "EXITEC");
        EEPROM.commit();
        Serial.println("[EC] Calibration SAVED");
    }
}

float SensorEC::readEC() const {
    if (bufferFilled) {
        float sum = 0;
        for (uint8_t i = 0; i < FILTER_SIZE; i++) sum += ecBuffer[i];
        return sum / FILTER_SIZE;
    }
    else {
        float sum = 0;
        for (uint8_t i = 0; i < bufferIndex; i++) sum += ecBuffer[i];
        return (bufferIndex > 0) ? sum / bufferIndex : 0;
    }
}

float SensorEC::getVoltage() const {
    return voltage;
}

float SensorEC::filterEC(float value) {
    ecBuffer[bufferIndex] = value;
    bufferIndex++;
    if (bufferIndex >= FILTER_SIZE) {
        bufferIndex = 0;
        bufferFilled = true;
    }
    return readEC();
}

void SensorEC::update() {
    static unsigned long last = 0;
    if (millis() - last < 1000) return; // 1s interval
    last = millis();

    int16_t raw = ads.readADC_SingleEnded(ecChannel);
    voltage = raw * 0.125 - 75; // mV
    if (voltage < 0) voltage = raw *0.125;

    float rawEC = ec.readEC(voltage, temperature);
    float filteredEC = filterEC(rawEC);

    Serial.print("[EC] Voltage = ");
    Serial.print(voltage);
    Serial.print("  → EC = ");
    Serial.println(filteredEC, 4);
};

// void SensorTask::readEC() {
//     // Cập nhật dữ liệu từ cảm biến EC
//     ecSensor.update();

//     // Lấy giá trị EC đã lọc
//     float ecValue = ecSensor.readEC();

//     if (ecValue > 0) {
//         currentSensorData.ec_us_cm = ecValue;
//         currentSensorData.ec_valid = true;
//         currentSensorData.ec_last_read = millis();

//         Serial.printf("[SensorTask] EC: %.1f uS/cm [CACHED]\n", currentSensorData.ec_us_cm);
//     } else {
//         Serial.println("[SensorTask] Failed to read EC sensor");
//         currentSensorData.ec_valid = false;
//     }
// }