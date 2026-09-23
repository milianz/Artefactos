const int TOUCH_PIN = 32; 

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("PRUEBA SENSOR TOUCH (T9 - GPIO32)");
  Serial.println();
}

void loop() {
  int valorTouch = touchRead(TOUCH_PIN);

  Serial.print("Valor Touch T9: ");
  Serial.println(valorTouch);

  delay(200);
}