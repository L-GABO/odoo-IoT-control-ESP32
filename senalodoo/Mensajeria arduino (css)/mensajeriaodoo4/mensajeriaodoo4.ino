#include <WiFi.h>

// Configuración WiFi
const char* ssid = "Tenda_453370";
const char* password = "dancethink626";
const char* serverIP = "192.168.0.191";
const int serverPort = 5000;

// Configuración del sensor
const int sensorPin = 4;  // Conectar el cable BK (salida) del sensor a este pin
bool lastState = false;
bool currentState = false;
unsigned long lastDetectionTime = 0;
const unsigned long debounceDelay = 50;  // Tiempo antirebote en ms

WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT_PULLDOWN);  // Usar PULLDOWN porque el sensor es PNP (+24V cuando activo)
  
  connectToWiFi();
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.println("Conectando a WiFi...");
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado a WiFi");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nError al conectar a WiFi");
  }
}

void loop() {
  // Leer estado del sensor con antirebote
  currentState = debounceRead(sensorPin);
  
  // Detección de flanco ascendente (sensor activado)
  if (currentState && !lastState) {
    Serial.println("Detección magnética");
    sendDetectionToServer();
    lastDetectionTime = millis();
  }
  
  // Detección de flanco descendente (sensor desactivado)
  if (!currentState && lastState) {
    Serial.println("Fin de detección");
    // Puedes enviar un mensaje de "OFF" si lo necesitas
    // sendToServer("OFF");
  }
  
  lastState = currentState;
  
  // Verificar conexión WiFi periódicamente
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi desconectado. Reconectando...");
    connectToWiFi();
  }
  
  delay(10);  // Pequeña pausa para evitar sobrecarga
}

bool debounceRead(int pin) {
  bool lastReading = digitalRead(pin);
  unsigned long lastDebounceTime = millis();
  
  while ((millis() - lastDebounceTime) < debounceDelay) {
    bool currentReading = digitalRead(pin);
    if (currentReading != lastReading) {
      lastReading = currentReading;
      lastDebounceTime = millis();
    }
  }
  
  return lastReading;
}

void sendDetectionToServer() {
  if (client.connect(serverIP, serverPort)) {
    Serial.println("Enviando detección al servidor");
    client.println("DETECTION");  // Envía un mensaje simple de detección
    client.stop();
  } else {
    Serial.println("Error de conexión al servidor");
  }
}