#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>

#define i2c_Address 0x3C

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

const int pinLM35 = A0;

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  Wire.begin();

  delay(250);

  if (!display.begin(i2c_Address, true)) {
    Serial.println("ERROR: OLED no encontrada");
    while (1)
      ;
  }

  Serial.println("OLED funcionando correctamente");
}

void loop() {
  int lecturaActual = analogRead(pinLM35);

  float temperatura = (lecturaActual * (5.0 / 1024.0) * 100.0);

  Serial.print("Lectura ADC: ");
  Serial.print(lecturaActual);
  Serial.print(" | Temperatura: ");
  Serial.print(temperatura, 1);
  Serial.println(" C");

  display.clearDisplay();

  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(18, 5);
  display.println("TERMOMETRO LM35");

  display.setTextSize(2);
  display.setCursor(20, 24);
  display.print(temperatura, 1);
  display.print(" ");
  display.cp437(true);
  display.write(248);
  display.print("C");

  display.setTextColor(SH110X_BLACK, SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(12, 50);
  display.println(" LECTURA EN VIVO ");

  display.display();

  delay(1000);
}