#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP085.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

Adafruit_BMP085 bmp;

float lastPressure = 1013.25;

void setup() {

  Serial.begin(115200);

  // ESP32 I2C
  Wire.begin(SDA_PIN, SCL_PIN);

  // OLED Init
  if (!display.begin(
      SSD1306_SWITCHCAPVCC,
      0x3C)) {

    Serial.println("OLED FAILED");
    while (1);
  }

  // BMP180 Init
  if (!bmp.begin()) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 25);
    display.println("BMP180 FAIL");
    display.display();

    Serial.println("BMP180 NOT FOUND");
    while (1);
  }

  // Boot Screen
  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setTextSize(2);
  display.setCursor(14, 15);
  display.println("StormEye");

  display.setTextSize(1);
  display.setCursor(20, 42);
  display.println("Atmosphere AI");

  display.display();

  delay(2000);
}

void loop() {

  float temp =
      bmp.readTemperature();

  float pressure =
      bmp.readPressure() / 100.0;

  float altitude =
      bmp.readAltitude();

  // Weather Prediction
  String status = "STABLE";

  float diff =
      pressure - lastPressure;

  if (diff < -1.5) {
    status = "RAIN";
  }
  else if (diff > 1.5) {
    status = "CLEAR";
  }

  lastPressure = pressure;

  // Serial Monitor
  Serial.println("\n===== STORM EYE =====");

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println(" C");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");

  Serial.print("Forecast: ");
  Serial.println(status);

  // OLED Display
  display.clearDisplay();

  // Border
  display.drawRect(
      0, 0, 128, 64, WHITE);

  // Header
  display.drawLine(
      0, 12, 128, 12, WHITE);

  display.setTextSize(1);
  display.setCursor(30, 2);
  display.println("STORM EYE");

  // Temperature
  display.setCursor(5, 18);
  display.print("T: ");
  display.print(temp, 1);
  display.print(" C");

  // Pressure
  display.setCursor(5, 30);
  display.print("P: ");
  display.print(pressure, 0);
  display.print(" hPa");

  // Altitude
  display.setCursor(5, 42);
  display.print("A: ");
  display.print(altitude, 1);
  display.print(" m");

  // Weather Box
  display.drawRect(
      84, 20, 38, 18, WHITE);

  display.setCursor(88, 26);
  display.print(status);

  // Pressure Progress Bar
  int bar = map(
      pressure,
      950,
      1050,
      0,
      100);

  bar = constrain(bar, 0, 100);

  display.drawRect(
      10, 54, 100, 6, WHITE);

  display.fillRect(
      10, 54, bar, 6, WHITE);

  display.display();

  delay(1000);
}
