import socket
from xmlrpc import client
from datetime import datetime

# Configuración de Odoo
url = 'https://koteos.odoo.com'  # URL de tu instancia de Odoo
db = 'koteos'  # Nombre de la base de datos
username = 'alejandra.estrada@koteosgroup.com'  # Usuario de Odoo
password = 'Koteos2468!+'  # Contraseña de Odoo

# Autenticación en Odoo
common = client.ServerProxy(f'{url}/xmlrpc/2/common')
uid = common.authenticate(db, username, password, {})

models = client.ServerProxy(f'{url}/xmlrpc/2/object')

def crear_registro_en_odoo():
    # Obtener la fecha y hora actual
    fecha_actual = datetime.now().strftime('%Y-%m-%d')
    hora_actual = datetime.now().strftime('%H:%M:%S')

    # Datos del nuevo registro
    valores = {
        'x_name': 'Nueva señal recibida',  # Nombre del registro
        'x_studio_date': fecha_actual,  # Fecha actual
        'x_studio_hora_del_pulso_1': hora_actual,  # Hora actual
    }

    # Crear el registro en el modelo personalizado 'x_señal_entrante'
    registro_id = models.execute_kw(db, uid, password, 'x_senal_entrante', 'create', [valores])
    print(f"Registro creado en Odoo con ID: {registro_id}")

def main():
    host = '0.0.0.0'  # Escucha en todas las interfaces de red
    port = 5000  # Puerto en el que escucha el script

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((host, port))
    server_socket.listen(1)

    print(f"Escuchando en {host}:{port}...")

    while True:
        client_socket, addr = server_socket.accept()
        print(f"Conexión establecida con {addr}")

        data = client_socket.recv(1024).decode('utf-8')
        if data == "Boton presionado":
            print("Señal recibida: Botón presionado")
            # Crear un nuevo registro en Odoo
            crear_registro_en_odoo()

        client_socket.close()

if __name__ == "__main__":
    main()