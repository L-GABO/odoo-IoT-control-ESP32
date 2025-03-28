#include <Servo.h>

Servo miServo;  // Crea un objeto servo
int posicionInicial = 0;  // Posición inicial del servo (0 grados)
int posicionFinal = 90;   // Posición a la que girará (90 grados)
int pinServo = 9;         // Pin donde conectas el servo (puedes cambiarlo)

void setup() {
  miServo.attach(pinServo);  // Asocia el servo al pin especificado
  miServo.write(posicionInicial);  // Inicia el servo en 0 grados
  delay(1000);              // Pequeña pausa para estabilización
}

void loop() {
  // Mueve el servo a 90 grados
  miServo.write(posicionFinal);
  delay(1000);  // Pequeña pausa para que complete el movimiento
  
  // Espera 1 minuto (60000 milisegundos)
  delay(10000);
  
  // Vuelve el servo a 0 grados
  miServo.write(posicionInicial);
  delay(1000);  // Pequeña pausa para que complete el movimiento
  
  // Espera otro minuto antes de repetir el ciclo
  delay(5000);
}