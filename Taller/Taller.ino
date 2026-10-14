#include <WiFi.h>
#include <PubSubClient.h>

// ======================================================
// CONFIGURACIÓN WIFI
// ======================================================

const char* WIFI_SSID = "Milianzz";
const char* WIFI_PASSWORD = "hola4120";

// ======================================================
// CONFIGURACIÓN MQTT
// ======================================================

const char* MQTT_SERVER = "172.20.10.9";
const int MQTT_PORT = 1883;

// Topics utilizados por el dashboard React.
const char* TOPIC_TEMPERATURA = "clima/esp32/temperatura";
const char* TOPIC_HUMEDAD = "clima/esp32/humedad";
const char* TOPIC_LUZ = "clima/esp32/luz";

// ======================================================
// CONFIGURACIÓN DE SENSORES
// ======================================================

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Enviar una lectura cada 2 segundos.
const unsigned long INTERVALO_ENVIO = 2000;
unsigned long ultimoEnvio = 0;

void conectarWiFi() {
  Serial.print("Conectando a WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado");
  Serial.print("IP del ESP32: ");
  Serial.println(WiFi.localIP());
}

void conectarMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Conectando a Mosquitto...");

    // Crear un Client ID diferente para este ESP32.
    String clientId = "ESP32-";
    clientId += String(
      (uint32_t)(ESP.getEfuseMac() & 0xFFFFFFFF),
      HEX
    );

    if (mqttClient.connect(clientId.c_str())) {
      Serial.println(" conectado");
    } else {
      Serial.print(" error, rc=");
      Serial.print(mqttClient.state());
      Serial.println(". Reintentando en 2 segundos...");
      delay(2000);
    }
  }
}

// ======================================================
// LEER Y PUBLICAR SENSORES
// ======================================================

void publicarSensores() {
  // Ejemplo básico de valores simulados (puedes reemplazarlo por tus lecturas reales de sensores)
  float temperatura = 25.5;
  float humedad = 60.0;
  int luz = 300;

  // Convertir valores a String o Char array para publicar
  char bufferTemp[10];
  char bufferHum[10];
  char bufferLuz[10];

  sprintf(bufferTemp, "%.2f", temperatura);
  sprintf(bufferHum, "%.2f", humedad);
  sprintf(bufferLuz, "%d", luz);

  // Publicar cada dato en su topic correspondiente
  mqttClient.publish(TOPIC_TEMPERATURA, bufferTemp);
  mqttClient.publish(TOPIC_HUMEDAD, bufferHum);
  mqttClient.publish(TOPIC_LUZ, bufferLuz);

  // Mostrar las mediciones en el Monitor Serial
  Serial.println("--- Datos publicados ---");
  Serial.print("Temperatura: "); Serial.println(bufferTemp);
  Serial.print("Humedad: "); Serial.println(bufferHum);
  Serial.print("Luz: "); Serial.println(bufferLuz);
}

void setup() {
  Serial.begin(115200);

  // Inicializar los sensores y configurar los pines aquí si los necesitas
  // pinMode(PIN_LUZ, INPUT);

  conectarWiFi();
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop() {
  // Si se pierde WiFi, volver a conectarse.
  if (WiFi.status() != WL_CONNECTED) {
    conectarWiFi();
  }

  // Si se pierde MQTT, volver a conectarse.
  if (!mqttClient.connected()) {
    conectarMQTT();
  }

  mqttClient.loop();

  // Publicar sensores cada 2 segundos (Corregido el operador '-')
  if (millis() - ultimoEnvio >= INTERVALO_ENVIO) {
    ultimoEnvio = millis();
    publicarSensores();
  }
}