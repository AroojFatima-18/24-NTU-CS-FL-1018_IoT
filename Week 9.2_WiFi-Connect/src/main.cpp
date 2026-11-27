#include <WiFi.h>

const char* ssid = "....";
const char* password = "Ma@12345";

void setup() {

    Serial.begin(115200);

    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting...");
    }

    Serial.println("Connected!");

    Serial.println(WiFi.localIP());
}

void loop() {

}