#define TRIG_PIN 18
#define ECHO_PIN 19

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Limpiar el pin TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Generar pulso de disparo de 10 microsegundos
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  // Medir el tiempo que el pin ECHO permanece en HIGH
  long duracion = pulseIn(ECHO_PIN, HIGH);

  // Si duracion es 0, significa que se superó el tiempo de espera (timeout)
  if (duracion == 0) {
    Serial.println("No se detectó eco");
  } else {
    // Cálculo de la distancia en centímetros:
    // Distancia = (duración * 0.0343) / 2
    float distancia = duracion * 0.0343 / 2.0;

    Serial.print("Distancia: ");
    Serial.print(distancia);
    Serial.println(" cm");
  }

  delay(500);
}