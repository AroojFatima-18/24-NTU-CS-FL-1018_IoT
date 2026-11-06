#include <Arduino.h>
#define DAC_PIN 26

void setup() {
    Serial.begin(115200);
    Serial.println("Step 5: DAC2 on GPIO26");
}

void loop() {
    for (int i = 0; i <= 255; i++) {
        dacWrite(DAC_PIN, i);
        delay(20);
    }

    for (int i = 255; i >= 0; i--) {
        dacWrite(DAC_PIN, i);
        delay(20);
    }
}
