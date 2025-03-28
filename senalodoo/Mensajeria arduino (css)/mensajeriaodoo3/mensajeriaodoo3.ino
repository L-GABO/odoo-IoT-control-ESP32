#include <WiFi.h>  // Usa <ESP8266WiFi.h> para ESP8266

const char* ssid = "iPhone de Luis";
const char* password = "ziperono";
const char* serverIP = "172.20.10.5";
const int serverPort = 5000;  // Puerto en el que escucha tu script de Python

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");
}

void loop() {
  // 1. Pulso Largo (30 segundos)
  enviarPulso("30_seg");
  delay(30000);  // Espera 30 segundos (duración del pulso)

  // Delay adicional de 2 segundos (para separar el pulso largo de los intermitentes)
  delay(60000);

  // 2. Pulso Intermitente (2/2/2)
  enviarPulso("2_2_2");
  delay(2000);  // ON por 2 segundos
  enviarPulso("2_2_2_OFF");
  delay(2000);  // OFF por 2 segundos
  enviarPulso("2_2_2");  // ON nuevamente por 2 segundos
  delay(2000);

  // Delay adicional antes del pulso normal
  delay(60000);

  // 3. Pulso Normal (1 vez)
  enviarPulso("Normal");
  delay(5000);  // Espera 5 segundos antes de repetir el ciclo
}

void enviarPulso(String tipoPulso) {
  if (client.connect(serverIP, serverPort)) {
    Serial.print("Enviando señal: ");
    Serial.println(tipoPulso);
    client.println(tipoPulso);
    client.stop();
  } else {
    Serial.println("Error de conexión al servidor");
  }
}