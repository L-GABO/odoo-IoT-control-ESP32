import serial
import time
from datetime import datetime

def obtener_hora_actual():
    """Función para obtener la hora actual en formato legible."""
    return datetime.now().strftime("%Y-%m-%d %H:%M:%S")

try:
    # Configura el puerto serial (ajusta el puerto COM y la velocidad según tu configuración)
    arduino = serial.Serial('COM3', 115200, timeout=1)
    print("Conexión establecida con Arduino en COM3")
except serial.SerialException as e:
    print(f"Error al abrir el puerto serial: {e}")
    exit()

print("Esperando señal de Arduino...")

while True:
    try:
        if arduino.in_waiting > 0:  # Si hay datos disponibles en el puerto serial
            mensaje = arduino.readline().decode('utf-8').strip()  # Lee el mensaje

            if "Botón 2 presionado" in mensaje:
                hora_pulso = obtener_hora_actual()  # Obtiene la hora actual
                print(f"Botón 2 presionado detectado a las: {hora_pulso}")
    except KeyboardInterrupt:
        print("Programa terminado por el usuario.")
        break
    except Exception as e:
        print(f"Error: {e}")
        break

arduino.close()  # Cierra el puerto serial al finalizar