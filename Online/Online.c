//LIBRERIAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "Online.h"

int iniciarServidor(int puerto) { // Inicia el servidor, recibe el puerto y devuelve el descriptor del socket conectado
    int socket_servidor, socket_cliente; // Variables para almacenar los descriptores de los sockets
    struct sockaddr_in direccion_servidor, direccion_cliente; // Estructuras para guardar las direcciones IP y puertos
    socklen_t tamano_cliente = sizeof(direccion_cliente); // Calcula el tamaño de la estructura de la dirección del cliente

    socket_servidor = socket(AF_INET, SOCK_STREAM, 0); // Crea el socket del servidor (IPv4, TCP)
    if (socket_servidor < 0) { // Verifica si hubo error al crear el socket
        perror("Error al crear el socket del servidor"); // Muestra el error en consola
        return -1; // Retorna -1 si falla
    }

    int opt = 1; // Variable auxiliar para habilitar la opción de reutilización de dirección
    setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // Permite usar el mismo puerto inmediatamente tras cerrar

    direccion_servidor.sin_family = AF_INET; // Configura el uso de direcciones IPv4
    direccion_servidor.sin_addr.s_addr = INADDR_ANY; // Acepta conexiones desde cualquier IP disponible
    direccion_servidor.sin_port = htons(puerto); // Convierte el número de puerto a formato de red (Big-Endian)

    if (bind(socket_servidor, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) < 0) { // Vincula el socket al puerto
        perror("Error en la operacion bind"); // Error si el puerto está ocupado o restringido
        close(socket_servidor); // Cierra el socket antes de salir
        return -1; // Retorna error
    }

    if (listen(socket_servidor, 1) < 0) { // Pone el socket en modo espera para recibir conexiones
        perror("Error en la operacion listen"); // Error al intentar escuchar
        close(socket_servidor); // Cierra el socket
        return -1; // Retorna error
    }

    printf("Sala creada. Esperando que el oponente se conecte en el puerto %d...\n", puerto); // Notifica al usuario
    
    socket_cliente = accept(socket_servidor, (struct sockaddr *)&direccion_cliente, &tamano_cliente); // Frena la ejecución hasta recibir un cliente
    if (socket_cliente < 0) { // Verifica si la aceptación de la conexión falló
        perror("Error al aceptar la conexion del oponente"); // Muestra error
        close(socket_servidor); // Cierra el socket del servidor
        return -1; // Retorna error
    }

    printf("¡Oponente conectado desde la IP: %s!\n", inet_ntoa(direccion_cliente.sin_addr)); // Imprime la IP del cliente conectado
    
    close(socket_servidor); // Cierra el socket principal de escucha, solo queda activo el de la conexión
    return socket_cliente; // Retorna el descriptor del socket para comunicarse con el cliente
}

int conectarAlServidor(const char *ip, int puerto) {
    int socket_conexion; // Variable para el socket cliente
    struct sockaddr_in direccion_servidor; // Estructura con datos del servidor remoto

    socket_conexion = socket(AF_INET, SOCK_STREAM, 0); // Crea el socket local para el cliente
    if (socket_conexion < 0) { // Verifica si hubo error al crearlo
        perror("Error al crear el socket cliente"); // Reporta el error
        return -1; // Retorna error
    }

    direccion_servidor.sin_family = AF_INET; // Define protocolo IPv4
    direccion_servidor.sin_port = htons(puerto); // Configura el puerto del servidor remoto

    if (inet_pton(AF_INET, ip, &direccion_servidor.sin_addr) <= 0) { // Convierte la IP de texto a binario
        perror("Direccion IP invalida o no soportada"); // Reporta error de formato de IP
        close(socket_conexion); // Cierra el socket abierto
        return -1; // Retorna error
    }

    printf("Intentando conectar a la sala en %s:%d...\n", ip, puerto); // Avisa que inicia la conexión
    if (connect(socket_conexion, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) < 0) { // Intenta establecer conexión
        perror("No se pudo establecer conexion con el servidor"); // Error si el servidor no responde
        close(socket_conexion); // Cierra el socket
        return -1; // Retorna error
    }

    printf("¡Te has unido a la sala exitosamente!\n"); // Confirmación de conexión
    return socket_conexion; // Retorna el socket listo para comunicarse
}