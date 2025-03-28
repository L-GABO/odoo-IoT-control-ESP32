#include <WiFi.h>

// Configuración WiFi
const char* ssid = "Tenda_453370";
const char* password = "dancethink626";
const char* serverIP = "192.168.0.191";
const int serverPort = 5000;

// Configuración de los sensores
const int sensorPin1 = 4;  // Primer sensor
const int sensorPin2 = 5;  // Segundo sensor
bool lastState1 = LOW;
bool currentState1 = LOW;
bool lastState2 = LOW;
bool currentState2 = LOW;
const unsigned long debounceDelay = 50;  // Tiempo antirebote en ms

WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin1, INPUT_PULLDOWN);  // Configuración PULLDOWN para sensor PNP
  pinMode(sensorPin2, INPUT_PULLDOWN);  // Configuración igual para el segundo sensor
  
  // Espera inicial para estabilización
  delay(1000);
  
  // Leer estado inicial de los sensores
  lastState1 = digitalRead(sensorPin1);
  lastState2 = digitalRead(sensorPin2);
  
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
  // Leer estado actual de los sensores (sin antirebote todavía)
  bool reading1 = digitalRead(sensorPin1);
  bool reading2 = digitalRead(sensorPin2);
  
  // Aplicar antirebote solo si hay cambio
  if (reading1 != lastState1) {
    currentState1 = debounceRead(sensorPin1);
  } else {
    currentState1 = reading1;
  }
  
  if (reading2 != lastState2) {
    currentState2 = debounceRead(sensorPin2);
  } else {
    currentState2 = reading2;
  }
  
  // Detección para el primer sensor
  if (currentState1 == HIGH && lastState1 == LOW) {
    Serial.println("Detección magnética en Sensor 1");
    sendDetectionToServer("DETECTION1");
  }
  
  // Detección para el segundo sensor
  if (currentState2 == HIGH && lastState2 == LOW) {
    Serial.println("Detección magnética en Sensor 2");
    sendDetectionToServer("DETECTION2");
  }
  
  // Actualizar últimos estados
  lastState1 = currentState1;
  lastState2 = currentState2;
  
  // Verificar conexión WiFi periódicamente
  if (millis() % 5000 == 0 && WiFi.status() != WL_CONNECTED) {
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

void sendDetectionToServer(const char* message) {
  if (client.connect(serverIP, serverPort)) {
    Serial.print("Enviando detección al servidor: ");
    Serial.println(message);
    client.println(message);
    client.stop();
  } else {
    Serial.println("Error de conexión al servidor");
  }
}