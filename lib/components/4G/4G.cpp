#include "4G.h"

void Internet_4G::set_Begin(int rxPin, int txPin, uint32_t baud) {
    _port.begin(baud, SERIAL_8N1, rxPin, txPin);
    delay(3000);
}
void Internet_4G::flushInput() { while (_port.available()) _port.read(); }
void Internet_4G::setup4G() {
    flushInput();
    _port.println("AT");
}

void Internet_4G::sendAndPrint(const char* cmd) {
    _port.println(cmd);
    Serial.print("> ");
    Serial.println(cmd);

    delay(1200);

    while (_port.available()) {
        String resp = _port.readStringUntil('\n');
        resp.trim();
        if (resp.length() > 0)
            Serial.println(resp);
    }
}
void Internet_4G::check4G() {
    Serial.println("\n===== Kiểm tra 4G A7670C =====");

    flushInput();   // chỉ dùng 1 lần duy nhất

    sendAndPrint("AT+CPIN?");
    sendAndPrint("AT+CEREG?");
    sendAndPrint("AT+CSQ");
    sendAndPrint("AT+CGATT?");
    sendAndPrint("AT+CGPADDR");

    Serial.println("===== Kiểm tra xong =====");
}
