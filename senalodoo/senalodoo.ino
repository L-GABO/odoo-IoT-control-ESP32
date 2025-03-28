void setup() {
  // Inicializa la comunicación serial a 9600 baudios
  Serial.begin(9600);
}

void loop() {
  // Valor constante que quieres enviar (por ejemplo, 1.234 kg)
  float valorConstante = 1.234;  // Puedes cambiar este valor

  // Envía el valor constante en el formato que una báscula usaría
  Serial.print(valorConstante, 3);  // Envía el valor con 3 decimales
  Serial.println(" g");            // Agrega la unidad (opcional)

  // Espera un segundo antes de enviar el siguiente valor
  delay(1000);
}