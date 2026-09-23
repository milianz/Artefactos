#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SH1106G display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

const int TOUCH_PIN_1 = 32; // T9
const int TOUCH_PIN_2 = 33; // T8

// Si el pin 33 tiene un rango diferente, puedes ajustar cada umbral por separado:
const int UMBRAL_1 = 300; // Umbral para pin 32[cite: 3]
const int UMBRAL_2 = 300;  // Umbral para pin 33 (ajústalo según lo que leas en el monitor)

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  if (!display.begin(OLED_ADDRESS, true)) {
    Serial.println("Error OLED");
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
}

void loop() {
  int v1 = touchRead(TOUCH_PIN_1);
  int v2 = touchRead(TOUCH_PIN_2);

  // Imprime en terminal para verificar el comportamiento en vivo
  Serial.print("V1: "); Serial.print(v1);
  Serial.print(" | V2: "); Serial.println(v2);

  bool boton1 = (v1 < UMBRAL_1);
  bool boton2 = (v2 < UMBRAL_2);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(20, 5);
  display.println("ESP32 DUAL TOUCH");

  if (boton1 && !boton2) {
    display.setCursor(18, 28);
    display.println("Boton 1 Activado!");
    display.fillCircle(25, 52, 6, SH110X_WHITE);
  } 
  else if (!boton1 && boton2) {
    display.setCursor(18, 28);
    display.println("Boton 2 Activado!");
    display.fillCircle(103, 52, 6, SH110X_WHITE);
  } 
  else if (boton1 && boton2) {
    display.setCursor(14, 28);
    display.println("Ambos Activados!");
    display.fillCircle(25, 52, 6, SH110X_WHITE);
    display.fillCircle(103, 52, 6, SH110X_WHITE);
  } 
  else {
    display.setCursor(28, 28);
    display.println("Esperando toque...");
    display.drawCircle(64, 52, 5, SH110X_WHITE);
  }

  display.display();
  delay(100);
}