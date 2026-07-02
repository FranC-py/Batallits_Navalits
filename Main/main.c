#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "../Mapa/Mapa.h" 
#include "../Barcos/Barcos.h" 
#include "../Juego/Juego.h" 
#include "../Online/Online.h" // Apuntamos a la nueva carpeta

#define PUERTO_JUEGO 8888

int main() {
    int filas = 0, columnas = 0;
    int opcionRed = 0;
    int sock_red = -1;
    char ip_servidor[50] = "127.0.0.1";

    printf("=== BIENVENIDO A BATALLA NAVAL ONLINE ===\n");
    printf("1. Crear una sala (Servidor)\n");
    printf("2. Unirse a una sala (Cliente)\n");
    printf("Seleccione una opcion: ");
    if (scanf("%d", &opcionRed) != 1 || (opcionRed != 1 && opcionRed != 2)) {
        printf("Opcion invalida.\n");
        return 1;
    }

    // --- FASE DE CONEXIÓN ---
    if (opcionRed == 1) {
        printf("\nIngrese la cantidad de filas (N): ");
        scanf("%d", &filas);
        printf("Ingrese la cantidad de columnas (M): ");
        scanf("%d", &columnas);

        if (filas <= 0 || columnas <= 0) {
            printf("Dimensiones invalidas.\n");
            return 1;
        }

        sock_red = iniciarServidor(PUERTO_JUEGO);
        if (sock_red < 0) return 1;

        // Sincronizar dimensiones con el cliente
        send(sock_red, &filas, sizeof(int), 0);
        send(sock_red, &columnas, sizeof(int), 0);
    } else {
        printf("\nIngrese la IP del Servidor (usa 127.0.0.1 si es la misma PC): ");
        scanf("%s", ip_servidor);

        sock_red = conectarAlServidor(ip_servidor, PUERTO_JUEGO);
        if (sock_red < 0) return 1;

        printf("Esperando sincronizacion de mapa por parte del host...\n");
        recv(sock_red, &filas, sizeof(int), 0);
        recv(sock_red, &columnas, sizeof(int), 0);
        printf("Tablero sincronizado correctamente: %dx%d\n", filas, columnas);
    }

    // --- PREPARACIÓN LOCAL ---
    printf("\nAsignando memoria dinamicamente para tu tablero...\n");
    char **miMapa = crearMapa(filas, columnas);

    Barco flotaJugador[CANTIDAD_BARCOS];
    inicializarFlota(flotaJugador);
    colocarBarcosAleatoriamente(filas, columnas, flotaJugador);
    printf("¡Tu flota ha sido posicionada en coordenadas secretas!\n");

    // --- BUCLE PRINCIPAL ONLINE ---
    char entradaDisparo[10];
    int filaDisparo, colDisparo;
    int juegoTerminado = 0;
    int miTurno = (opcionRed == 1) ? 1 : 0; 

    while (!juegoTerminado) {
        if (miTurno) {
            printf("\n=============================\n");
            printf("     ¡ES TU TURNO DE ATACAR! \n");
            printf("=============================\n");
            
            printf("Ingrese coordenada de disparo (ej. B4): ");
            scanf("%s", entradaDisparo);

            while (!parsearCoordenada(entradaDisparo, &filaDisparo, &colDisparo, filas, columnas)) {
                printf("Coordenada invalida o fuera de rango. Intente nuevamente: ");
                scanf("%s", entradaDisparo);
            }

            send(sock_red, entradaDisparo, sizeof(entradaDisparo), 0);
            printf("Disparo enviado, esperando respuesta del radar enemigo...\n");

            EstadoCasilla resultadoImpacto;
            recv(sock_red, &resultadoImpacto, sizeof(EstadoCasilla), 0);
            recv(sock_red, &juegoTerminado, sizeof(int), 0);

            if (resultadoImpacto == AGUA) printf("--> Radar: ¡AGUA!\n");
            else if (resultadoImpacto == TOCADO) printf("--> Radar: ¡TOCADO!\n");
            else if (resultadoImpacto == HUNDIDO) printf("--> Radar: ¡HUNDIDO!\n");

            if (juegoTerminado) {
                printf("\n¡VICTORIA! Has hundido el ultimo barco enemigo. Ganaste la partida.\n");
            } else {
                miTurno = 0; 
            }
        } else {
            printf("\n=============================\n");
            printf("    ESPERANDO TIRO ENEMIGO... \n");
            printf("=============================\n");
            imprimirMapa(miMapa, filas, columnas, flotaJugador);

            recv(sock_red, entradaDisparo, sizeof(entradaDisparo), 0);
            parsearCoordenada(entradaDisparo, &filaDisparo, &colDisparo, filas, columnas);
            
            printf("El oponente disparo a la coordenada: %s\n", entradaDisparo);

            EstadoCasilla resultadoLocal = evaluarDisparo(miMapa, flotaJugador, filaDisparo, colDisparo);
            
            if (flotaDestruida(flotaJugador)) {
                juegoTerminado = 1;
            }

            send(sock_red, &resultadoLocal, sizeof(EstadoCasilla), 0);
            send(sock_red, &juegoTerminado, sizeof(int), 0);

            if (juegoTerminado) {
                printf("\n¡ELIMINADO! Todos tus barcos han sido hundidos. El jugador contrario gana la partida.\n");
                imprimirMapa(miMapa, filas, columnas, flotaJugador);
            } else {
                miTurno = 1; 
            }
        }
    }

    // --- LIMPIEZA ---
    printf("\nCerrando sockets de comunicacion...\n");
    close(sock_red);

    printf("Liberando memoria estatica y dinamica...\n");
    liberarMapa(miMapa, filas);
    
    printf("Juego finalizado correctamente.\n");
    return 0;
}