#include <Arduino.h>

#define DAC_PIN 25
#define PWM_PIN 13

const int pwmChannel = 0;
const int freq = 5000;
const int resolution = 8;

void setup()
{
    Serial.begin(115200);

    ledcSetup(pwmChannel, freq, resolution);
    ledcAttachPin(PWM_PIN, pwmChannel);
}

void loop()
{
    for (int value = 0; value <= 255; value++)
    {
        dacWrite(DAC_PIN, value);
        ledcWrite(pwmChannel, value);
        delay(10);
    }

    for (int value = 255; value >= 0; value--)
    {
        dacWrite(DAC_PIN, value);
        ledcWrite(pwmChannel, value);
        delay(10);
    }
}
