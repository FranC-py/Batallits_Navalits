//LIBRERIAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "Online.h"

int iniciarServidor(int puerto) { //aca se inicia el servidor, recibe el puerto y devuelve el descriptor del socket del cliente conectado, -1 si falla
    int socket_servidor, socket_cliente; //aca se definen los sockets del servidor y del cliente
    struct sockaddr_in direccion_servidor, direccion_cliente; //aca se definen las direcciones del servidor y del cliente
    socklen_t tamano_cliente = sizeof(direccion_cliente); //tamano(es tamaño pero no se puede poner ñ) del cliente, se inicializa con el tamaño de la estructura direccion_cliente

    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);//aca se crea el socket del servidor
    if (socket_servidor < 0) {
        perror("Error al crear el socket del servidor");
        return -1;
    }

    // Configuración para evitar el error "Address already in use" si cerrás y abrís rápido
    int opt = 1; //aca se define la variable opt como 1, para que el socket pueda ser reutilizado   
    setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    direccion_servidor.sin_family = AF_INET; 
    direccion_servidor.sin_addr.s_addr = INADDR_ANY;
    direccion_servidor.sin_port = htons(puerto);

    if (bind(socket_servidor, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) < 0) { 
        perror("Error en la operacion bind");
        close(socket_servidor);
        return -1;
    }

    if (listen(socket_servidor, 1) < 0) {
        perror("Error en la operacion listen");
        close(socket_servidor);
        return -1;
    }

    printf("Sala creada. Esperando que el oponente se conecte en el puerto %d...\n", puerto);
    
    socket_cliente = accept(socket_servidor, (struct sockaddr *)&direccion_cliente, &tamano_cliente);
    if (socket_cliente < 0) {
        perror("Error al aceptar la conexion del oponente");
        close(socket_servidor);
        return -1;
    }

    printf("¡Oponente conectado desde la IP: %s!\n", inet_ntoa(direccion_cliente.sin_addr));
    
    close(socket_servidor); 
    return socket_cliente;
}

int conectarAlServidor(const char *ip, int puerto) {
    int socket_conexion;
    struct sockaddr_in direccion_servidor;

    socket_conexion = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_conexion < 0) {
        perror("Error al crear el socket cliente");
        return -1;
    }

    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_port = htons(puerto);

    if (inet_pton(AF_INET, ip, &direccion_servidor.sin_addr) <= 0) {
        perror("Direccion IP invalida o no soportada");
        close(socket_conexion);
        return -1;
    }

    printf("Intentando conectar a la sala en %s:%d...\n", ip, puerto);
    if (connect(socket_conexion, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) < 0) {
        perror("No se pudo establecer conexion con el servidor");
        close(socket_conexion);
        return -1;
    }

    printf("¡Te has unido a la sala exitosamente!\n");
    return socket_conexion;
}