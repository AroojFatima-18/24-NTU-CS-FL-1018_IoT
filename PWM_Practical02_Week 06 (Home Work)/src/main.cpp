// Week 6 - PWM Practical 02: Two LEDs + OLED display (SSD1306, I2C)

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifndef ESP_ARDUINO_VERSION_MAJOR
#define ESP_ARDUINO_VERSION_MAJOR 2
#endif

// ---------------- Pins ----------------
constexpr uint8_t LED1_PIN = 18;
constexpr uint8_t LED2_PIN = 19;
constexpr uint8_t OLED_SDA = 21;
constexpr uint8_t OLED_SCL = 22;

// ---------------- PWM settings ----------------
constexpr uint32_t PWM_FREQUENCY = 5000;  // 5 kHz
constexpr uint8_t  PWM_RESOLUTION = 8;    // 8-bit -> duty 0..255
constexpr uint16_t PWM_MAX = 255;
constexpr uint8_t  LED1_CHANNEL = 0;      // used only by the 2.x API
constexpr uint8_t  LED2_CHANNEL = 1;

// ---------------- OLED settings ----------------
constexpr uint8_t  SCREEN_WIDTH = 128;
constexpr uint8_t  SCREEN_HEIGHT = 64;
constexpr uint8_t  OLED_ADDRESS = 0x3C;   // some modules use 0x3D
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
bool displayReady = false;

// ---------------- PWM helpers (2.x / 3.x compatible) ----------------
bool pwmAttach(uint8_t pin, uint8_t channel)
{
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  (void)channel;
  return ledcAttach(pin, PWM_FREQUENCY, PWM_RESOLUTION);
#else
  if (ledcSetup(channel, PWM_FREQUENCY, PWM_RESOLUTION) == 0) return false;
  ledcAttachPin(pin, channel);
  return true;
#endif
}

void pwmWrite(uint8_t pin, uint8_t channel, uint32_t duty)
{
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  (void)channel;
  ledcWrite(pin, duty);
#else
  (void)pin;
  ledcWrite(channel, duty);
#endif
}

// ---------------- Display ----------------
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

void setDuty(uint32_t duty1, uint32_t duty2)
{
  pwmWrite(LED1_PIN, LED1_CHANNEL, duty1);
  pwmWrite(LED2_PIN, LED2_CHANNEL, duty2);

  Serial.printf("LED1 duty = %3lu (%3lu%%) | LED2 duty = %3lu (%3lu%%)\n",
                (unsigned long)duty1, (unsigned long)(duty1 * 100 / PWM_MAX),
                (unsigned long)duty2, (unsigned long)(duty2 * 100 / PWM_MAX));

  showOnDisplay(duty1, duty2);
}

void setup()
{
  Serial.begin(115200);
  delay(300);

  bool led1Ready = pwmAttach(LED1_PIN, LED1_CHANNEL);
  bool led2Ready = pwmAttach(LED2_PIN, LED2_CHANNEL);
  if (!led1Ready || !led2Ready)
  {
    Serial.println("PWM setup failed.");
    while (true) { delay(1000); }
  }

  Wire.begin(OLED_SDA, OLED_SCL);
  displayReady = display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  if (!displayReady)
  {
    Serial.println("OLED not found! Check SDA/SCL wiring and address (0x3C / 0x3D). LEDs will still work.");
  }

  Serial.println("Two-channel PWM started.");
}

void loop()
{
  setDuty(64, 191);   // LED1 ~25%, LED2 ~75%
  delay(2000);
  setDuty(191, 64);   // LED1 ~75%, LED2 ~25%
  delay(2000);
}
