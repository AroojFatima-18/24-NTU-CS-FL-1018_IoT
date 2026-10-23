#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
constexpr uint8_t LED1_PIN = 18;
constexpr uint8_t LED2_PIN = 19;
constexpr uint8_t LED1_CHANNEL = 0;
constexpr uint8_t LED2_CHANNEL = 1;
constexpr uint32_t PWM_FREQUENCY = 5000;
constexpr uint8_t PWM_RESOLUTION = 8;
constexpr uint8_t OLED_SDA = 21;
constexpr uint8_t OLED_SCL = 22;
constexpr uint16_t PWM_MAX = 255;
constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;
constexpr uint8_t OLED_ADDRESS = 0x3C;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
bool displayReady = false;
void showOnDisplay(uint32_t duty1, uint32_t duty2)
{
  if (!displayReady) return;

  int p1 = (duty1 * 100 + PWM_MAX / 2) / PWM_MAX;
  int p2 = (duty2 * 100 + PWM_MAX / 2) / PWM_MAX;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("ESP32 PWM - 2 LEDs");
  display.drawFastHLine(0, 10, SCREEN_WIDTH, SSD1306_WHITE);

  display.setCursor(0, 14);
  display.printf("LED1 GPIO18: %3d%%", p1);
  display.drawRect(0, 24, SCREEN_WIDTH, 8, SSD1306_WHITE);
  display.fillRect(2, 26, (SCREEN_WIDTH - 4) * duty1 / PWM_MAX, 4, SSD1306_WHITE);

  display.setCursor(0, 36);
  display.printf("LED2 GPIO19: %3d%%", p2);
  display.drawRect(0, 46, SCREEN_WIDTH, 8, SSD1306_WHITE);
  display.fillRect(2, 48, (SCREEN_WIDTH - 4) * duty2 / PWM_MAX, 4, SSD1306_WHITE);

  display.setCursor(0, 56);
  display.print("5kHz | 8-bit");

  display.display();
}
void setup()
{
 Serial.begin(115200);
 Wire.begin(OLED_SDA, OLED_SCL);
displayReady = display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
if (!displayReady)
{
  Serial.println("OLED not found! Check wiring/address.");
}
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
 showOnDisplay(64, 191);
 delay(2000);
 ledcWrite(LED1_CHANNEL, 191);
 ledcWrite(LED2_CHANNEL, 64);
 showOnDisplay(191, 64);
 delay(2000);
}
