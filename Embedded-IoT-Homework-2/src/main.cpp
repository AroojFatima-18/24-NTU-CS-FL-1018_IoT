#include <Arduino.h>
constexpr uint8_t LED1_PIN = 18;
constexpr uint8_t LED2_PIN = 19;
constexpr uint8_t LED1_CHANNEL = 0;
constexpr uint8_t LED2_CHANNEL = 1;
constexpr uint32_t PWM_FREQUENCY = 5000;
constexpr uint8_t PWM_RESOLUTION = 8;
void setup()
{
 Serial.begin(115200);
 bool led1Ready = ledcSetup(LED1_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION) > 0;
 bool led2Ready = ledcSetup(LED2_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION) > 0;
 ledcAttachPin(LED1_PIN, LED1_CHANNEL);
 ledcAttachPin(LED2_PIN, LED2_CHANNEL);

 if (!led1Ready || !led2Ready)
 {
 Serial.println("PWM setup failed.");
 while (true) { delay(1000); }
 }
 Serial.println("Two-channel PWM started.");
}
void loop()
{
 ledcWrite(LED1_CHANNEL, 64); // About 25%
 ledcWrite(LED2_CHANNEL, 191); // About 75%
 delay(2000);
 ledcWrite(LED1_CHANNEL, 191);
 ledcWrite(LED2_CHANNEL, 64);
 delay(2000);
}