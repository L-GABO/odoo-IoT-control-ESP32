import socket
from xmlrpc import client
from datetime import datetime

# Configuración de Odoo
url = 'https://koteos.odoo.com'
db = 'koteos'
username = 'alejandra.estrada@koteosgroup.com'
password = 'Koteos2468!+'

# Autenticación Odoo
common = client.ServerProxy(f'{url}/xmlrpc/2/common')
uid = common.authenticate(db, username, password, {})
models = client.ServerProxy(f'{url}/xmlrpc/2/object')

def crear_registro_en_odoo(sensor_num):
    # Formato: "YYYY-MM-DD HH:MM:SS"
    fecha_hora = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

    valores = {
        'x_name': f"Detección Magnética - Sensor {sensor_num}",
        'x_studio_date': fecha_hora.split()[0],  # Solo la fecha para este campo
        'x_studio_hora_del_pulso_2': fecha_hora,  # Fecha y hora completas
    }

    registro_id = models.execute_kw(db, uid, password, 'x_senal_entrante', 'create', [valores])
    print(f"Registro creado: Detección del Sensor {sensor_num} - {fecha_hora}")

def main():
    host = '0.0.0.0'
    port = 5000

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((host, port))
    server_socket.listen(1)
    print(f"Escuchando en {host}:{port}...")

    while True:
        client_socket, addr = server_socket.accept()
        data = client_socket.recv(1024).decode('utf-8').strip()

        if data == "DETECTION1":
            print("Detección recibida del Sensor 1")
            crear_registro_en_odoo(1)
        elif data == "DETECTION2":
            print("Detección recibida del Sensor 2")
            crear_registro_en_odoo(2)
        else:
            print(f"Señal recibida no reconocida: '{data}'")

        client_socket.close()

if __name__ == "__main__":
    main()