import socket
from xmlrpc import client
from datetime import datetime

# Configuración de Odoo
url = 'https://koteos.odoo.com'  # URL de tu instancia de Odoo
db = 'koteos'  # Nombre de la base de datos
username = 'alejandra.estrada@koteosgroup.com'  # Usuario de Odoo
password = 'Koteos2468!+'  # Contraseña de Odoo

# Autenticación Odoo
common = client.ServerProxy(f'{url}/xmlrpc/2/common')
uid = common.authenticate(db, username, password, {})
models = client.ServerProxy(f'{url}/xmlrpc/2/object')

def crear_registro_en_odoo(tipo_pulso):
    fecha = datetime.now().strftime('%Y-%m-%d')
    hora = datetime.now().strftime('%H:%M:%S')

    nombre_registro = {
        "30_seg": "Pulso Largo (30 seg)",
        "2_2_2": "Pulso Intermitente (2/2/2)",
        "Normal": "Pulso Normal"
    }.get(tipo_pulso, "Señal desconocida")

    valores = {
        'x_name': nombre_registro,
        'x_studio_date': fecha,
        'x_studio_hora_del_pulso_1': hora,
    }

    registro_id = models.execute_kw(db, uid, password, 'x_senal_entrante', 'create', [valores])
    print(f"Registro creado: {nombre_registro} a las {hora}")

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

        if data in ["30_seg", "2_2_2", "Normal"]:
            print(f"Señal recibida: {data}")
            crear_registro_en_odoo(data)
        elif data == "2_2_2_OFF":
            print("Pulso intermitente (OFF) - No se crea registro")
        else:
            print(f"ERROR: Señal no reconocida -> '{data}'")

        client_socket.close()

if __name__ == "__main__":
    main()