//LIBRERIAS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "../Mapa/Mapa.h" 
#include "../Barcos/Barcos.h" 
#include "../Juego/Juego.h" 
#include "../Online/Online.h"

#define PUERTO_JUEGO 8888 //aca se define el puerto, se puede poner en realidad cualquier numero, pero se pone 8888 por convencion

int main() {
    //VARIABLES
    int filas = 0, columnas = 0; //variables para almacenar las dimensiones del tablero, se inicializan en 0 para indicar que no hay dimensiones definidas
    int opcionModo = 0; //variable para almacenar la opcion del menu, se inicializa en 0 para indicar que no hay opcion seleccionada
    int sock_red = -1; //variable para almacenar el descriptor del socket de red, se inicializa en -1 para indicar que no hay conexion 
    char ip_servidor[50] = "127.0.0.1"; //variable para almacenar la IP del servidor, esa variable es por defecto la local

    //MENU INICIAL
    printf("=== BIENVENIDO A BATALLA NAVAL ===\n"); //texto 
    printf("1. Jugar Offline (Modo de practica local)\n");  //texto
    printf("2. Jugar Online - Crear una sala (Servidor)\n"); //texto
    printf("3. Jugar Online - Unirse a una sala (Cliente)\n"); //texto
    printf("Seleccione una opcion: "); //texto
    
    if (scanf("%d", &opcionModo) != 1 || opcionModo < 1 || opcionModo > 3) { //aca se selecciona el modo
        printf("Error: Opcion invalida.\n"); //si no esta entre 1 y 3, error
        return 1; 
    }

    //CONFIG DEL MODO DEL TABLERO
    if (opcionModo == 1) { //modo local
        // MODO OFFLINE
        printf("\n--- MODO OFFLINE ---\n"); //texto
        printf("Ingrese la cantidad de filas (N): "); //texto
        scanf("%d", &filas); //aca se ingresa la cantidad de filas
        printf("Ingrese la cantidad de columnas (M): "); //texto
        scanf("%d", &columnas); //aca se ingresa la cantidad de columnas
        
        if (filas <= 0 || columnas <= 0) { //aca se verifica que las dimensiones sean validas
            printf("Dimensiones invalidas.\n");
            return 1; //si dimensiones = invalidas, se termina el programa
        }

    } else if (opcionModo == 2) { //modo online
        // MODO ONLINE - HOST (SERVIDOR)
        printf("\n--- MODO ONLINE (SERVIDOR) ---\n"); //texto
        printf("Ingrese la cantidad de filas (N): "); //texto
        scanf("%d", &filas); //aca se ingresa la cantidad de filas
        printf("Ingrese la cantidad de columnas (M): "); //texto
        scanf("%d", &columnas); //aca se ingresa la cantidad de columnas
        
        if (filas <= 0 || columnas <= 0) return 1; //si dimensiones = invalidas, se termina el programa

        sock_red = iniciarServidor(PUERTO_JUEGO); //aca se inicia el servidor
        if (sock_red < 0) return 1; //si no se pudo iniciar el servidor, se termina el programa

        // Sincronizar dimensiones con el cliente
        send(sock_red, &filas, sizeof(int), 0); //le envia al cliente el valor de filas
        send(sock_red, &columnas, sizeof(int), 0); //le envia al cliente el valor de columnas

    } else if (opcionModo == 3) {
        // MODO ONLINE - CLIENTE (INVITADO)
        printf("\n--- MODO ONLINE (CLIENTE) ---\n"); //texto
        printf("Ingrese la IP del Servidor (127.0.0.1 si es la misma PC): "); //esta ip se llama localhost se usa cuando queres probar una funcion online pero en una misma pc
        scanf("%s", ip_servidor); //aca se ingresa la ip del servidor al que se quiere conectar

        sock_red = conectarAlServidor(ip_servidor, PUERTO_JUEGO); //aca se intenta conectar al servidor
        if (sock_red < 0) return 1; //si no se pudo conectar al servidor, se termina el programa

        // Recibir dimensiones validadas desde el Servidor
        printf("Esperando sincronizacion de mapa por parte del host...\n"); //texto
        recv(sock_red, &filas, sizeof(int), 0); //aca se recibe el valor de filas del host
        recv(sock_red, &columnas, sizeof(int), 0); //aca se recibe el valor de columnas del host
        printf("Tablero sincronizado correctamente: %dx%d\n", filas, columnas); //aca se imprime el valor de filas y columnas del host
    }

    // CREACION DEL MAPA Y POSICIONAMIENTO DE BARCOS
    printf("\nAsignando memoria dinamicamente para tu tablero...\n"); //texto
    char **miMapa = crearMapa(filas, columnas); //aca se crea el mapa dinamicamente con las dimensiones ingresadas

    Barco flotaJugador[CANTIDAD_BARCOS]; //aca se crea un array de barcos, que es la flota del jugador, con la cantidad de barcos definida en Barcos.h
    inicializarFlota(flotaJugador); //aca se inicializa la flota del jugador
    colocarBarcosAleatoriamente(filas, columnas, flotaJugador); //aca se colocan los barcos aleatoriamente en el mapa
    printf("¡Tu flota ha sido posicionada en coordenadas secretas!\n"); //aca se imprime un mensaje indicando que los barcos han sido posicionados

    char entradaDisparo[10]; //variable para almacenar la entrada del disparo
    int filaDisparo, colDisparo; //variables para almacenar la fila y columna del disparo
    int juegoTerminado = 0; //variable para indicar si el juego ha terminado, se inicializa en 0 para indicar que no ha terminado

    // LOGICA PRINCIPAL DEL JUEGO
    
    if (opcionModo == 1) {
        //  BUCLE MODO OFFLINE 
        while (!juegoTerminado) { //aca se entra en el bucle del juego
            printf("\n"); //texto
            imprimirMapa(miMapa, filas, columnas, flotaJugador); //aca se imprime el mapa del jugador
            
            printf("\nIngrese coordenada del disparo enemigo (ej. B4): "); //texto
            scanf("%s", entradaDisparo); //aca se ingresa la coordenada del disparo enemigo

            if (parsearCoordenada(entradaDisparo, &filaDisparo, &colDisparo, filas, columnas)) { //aca se parsea la coordenada del disparo enemigo
                EstadoCasilla resultado = evaluarDisparo(miMapa, flotaJugador, filaDisparo, colDisparo); //aca se evalua el disparo enemigo y se guarda el resultado en la variable resultado
                
                if (resultado == AGUA) printf("--> Resultado: ¡AGUA!\n"); //en estas variables se imprime el resultado
                else if (resultado == TOCADO) printf("--> Resultado: ¡TOCADO!\n");
                else if (resultado == HUNDIDO) printf("--> Resultado: ¡HUNDIDO!\n");

                if (flotaDestruida(flotaJugador)) { //aca se verifica si la flota del jugador ha sido destruida
                    printf("\n¡ELIMINADO! Todos tus barcos han sido hundidos. Fin de la practica.\n");
                    imprimirMapa(miMapa, filas, columnas, flotaJugador); 
                    juegoTerminado = 1;
                }
            } else {
                printf("Coordenada invalida o fuera del mapa. Intente nuevamente.\n"); //aca se imprime un mensaje de error
            }
        }
    } else {
        //  BUCLE MODO ONLINE 
        int miTurno = (opcionModo == 2) ? 1 : 0; // Servidor arranca atacando

        while (!juegoTerminado) { //aca se entra en el bucle del juego
            if (miTurno) {
                printf("\n=============================\n");
                printf("     ¡ES TU TURNO DE ATACAR! \n");
                printf("=============================\n");
                
                printf("Ingrese coordenada de disparo (ej. B4): ");
                scanf("%s", entradaDisparo); //aca se ingresa la coordenada del disparo enemigo

                while (!parsearCoordenada(entradaDisparo, &filaDisparo, &colDisparo, filas, columnas)) { //aca se parsea la coordenada del disparo enemigo, si es invalida, se vuelve a pedir la coordenada
                    printf("Coordenada invalida. Intente nuevamente: ");
                    scanf("%s", entradaDisparo);//aca se ingresa la coordenada del disparo enemigo, entra el valor como tal
                }

                send(sock_red, entradaDisparo, sizeof(entradaDisparo), 0); //aca se envia la coordenada del disparo enemigo al host
                printf("Disparo enviado, esperando respuesta del radar enemigo...\n");

                EstadoCasilla resultadoImpacto; //variable para almacenar el resultado del impacto
                recv(sock_red, &resultadoImpacto, sizeof(EstadoCasilla), 0); //aca se recibe el resultado del impacto del disparo enemigo
                recv(sock_red, &juegoTerminado, sizeof(int), 0);

                if (resultadoImpacto == AGUA) printf("--> Radar: ¡AGUA!\n"); //aca se imprime el resultado del impacto
                else if (resultadoImpacto == TOCADO) printf("--> Radar: ¡TOCADO!\n");
                else if (resultadoImpacto == HUNDIDO) printf("--> Radar: ¡HUNDIDO!\n");

                if (juegoTerminado) { //aca se verifica si el juego ha terminado
                    printf("\n¡VICTORIA! Has hundido el ultimo barco enemigo. Ganaste la partida.\n");
                } else {
                    miTurno = 0; 
                }
            } else { //si no termino el juego, que pase el siguiente turno
                printf("\n=============================\n");
                printf("    ESPERANDO TIRO ENEMIGO... \n");
                printf("=============================\n");
                imprimirMapa(miMapa, filas, columnas, flotaJugador); //aca se imprime el mapa del jugador

                recv(sock_red, entradaDisparo, sizeof(entradaDisparo), 0); //aca se recibe la coordenada del disparo enemigo
                parsearCoordenada(entradaDisparo, &filaDisparo, &colDisparo, filas, columnas); //aca se parsea la coordenada del disparo enemigo
                
                printf("El oponente disparo a la coordenada: %s\n", entradaDisparo); //aca se imprime la coordenada del disparo enemigo

                EstadoCasilla resultadoLocal = evaluarDisparo(miMapa, flotaJugador, filaDisparo, colDisparo); //aca se evalua el disparo enemigo y se guarda el resultado en la variable resultadoLocal
                
                if (flotaDestruida(flotaJugador)) { //aca se verifica si la flota del jugador ha sido destruida
                    juegoTerminado = 1; //si es destruida termina el juego valuando juegoTermiando = 1
                }

                send(sock_red, &resultadoLocal, sizeof(EstadoCasilla), 0); //aca se envia el resultado del impacto
                send(sock_red, &juegoTerminado, sizeof(int), 0); //aca se envia el resultado del juego

                if (juegoTerminado) { //aca se verifica si el juego ha terminado
                    printf("\n¡DERROTA! Todos tus barcos han sido hundidos. El jugador contrario gana.\n");
                    imprimirMapa(miMapa, filas, columnas, flotaJugador); //aca se imprime el mapa del jugador
                } else {
                    miTurno = 1; 
                }
            }
        }
        // Cerrar socket al terminar la partida online
        printf("\nCerrando conexion de red...\n"); //
        close(sock_red); //se cierra la conexion 
    }

    // se limpia la memoria para evitar fugas de memoria
    printf("Liberando memoria asignada...\n");
    liberarMapa(miMapa, filas);
    
    printf("Proceso finalizado correctamente.\n");
    return 0;
}