#ifndef ONLINE_H
#define ONLINE_H

// Levanta el servidor en un puerto específico y espera una conexión.
// Devuelve el descriptor del socket del cliente conectado, o -1 si falla.
int iniciarServidor(int puerto);

// Se conecta a un servidor remoto mediante su dirección IP y puerto.
// Devuelve el descriptor del socket de la conexión, o -1 si falla.
int conectarAlServidor(const char *ip, int puerto);

#endif