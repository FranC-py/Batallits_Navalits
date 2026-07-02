#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "../Mapa/Mapa.h" 
#include "../Barcos/Barcos.h" 
#include "../Juego/Juego.h" 
#include "../Online/Online.h"

#define PUERTO_JUEGO 8888

int main() {
    int filas = 0, columnas = 0;
    int opcionModo = 0;
    int sock_red = -1;
    char ip_servidor[50] = "127.0.0.1";

    printf("=== BIENVENIDO A BATALLA NAVAL ===\n");
    printf("1. Jugar Offline (Modo de practica local)\n");
    printf("2. Jugar Online - Crear una sala (Servidor)\n");
    printf("3. Jugar Online - Unirse a una sala (Cliente)\n");
    printf("Seleccione una opcion: ");
    
    if (scanf("%d", &opcionModo) != 1 || opcionModo < 1 || opcionModo > 3) {
        printf("Error: Opcion invalida.\n");
        return 1;
    }

    // --- FASE 1: CONFIGURACIÓN SEGÚN EL MODO ELEGIDO ---
    if (opcionModo == 1) {
        // MODO OFFLINE
        printf("\n--- MODO OFFLINE ---\n");
        printf("Ingrese la cantidad de filas (N): ");
        scanf("%d", &filas);
        printf("Ingrese la cantidad de columnas (M): ");
        scanf("%d", &columnas);
        
        if (filas <= 0 || columnas <= 0) {
            printf("Dimensiones invalidas.\n");
            return 1;
        }

    } else if (opcionModo == 2) {
        // MODO ONLINE - SERVIDOR
        printf("\n--- MODO ONLINE (SERVIDOR) ---\n");
        printf("Ingrese la cantidad de filas (N): ");
        scanf("%d", &filas);
        printf("Ingrese la cantidad de columnas (M): ");
        scanf("%d", &columnas);
        
        if (filas <= 0 || columnas <= 0) return 1;

        sock_red = iniciarServidor(PUERTO_JUEGO);
        if (sock_red < 0) return 1;

        // Sincronizar dimensiones con el cliente
        send(sock_red, &filas, sizeof(int), 0);
        send(sock_red, &columnas, sizeof(int), 0);

    } else if (opcionModo == 3) {
        // MODO ONLINE - CLIENTE
        printf("\n--- MODO ONLINE (CLIENTE) ---\n");
        printf("Ingrese la IP del Servidor (ej. 127.0.0.1 si es la misma PC): ");
        scanf("%s", ip_servidor);

        sock_red = conectarAlServidor(ip_servidor, PUERTO_JUEGO);
        if (sock_red < 0) return 1;

        // Recibir dimensiones validadas desde el Servidor
        printf("Esperando sincronizacion de mapa por parte del host...\n");
        recv(sock_red, &filas, sizeof(int), 0);
        recv(sock_red, &columnas, sizeof(int), 0);
        printf("Tablero sincronizado correctamente: %dx%d\n", filas, columnas);
    }

    // --- FASE 2: PREPARACIÓN LOCAL (COMÚN PARA TODOS LOS MODOS) ---
    printf("\nAsignando memoria dinamicamente para tu tablero...\n");
    char **miMapa = crearMapa(filas, columnas);

    Barco flotaJugador[CANTIDAD_BARCOS];
    inicializarFlota(flotaJugador);
    colocarBarcosAleatoriamente(filas, columnas, flotaJugador);
    printf("¡Tu flota ha sido posicionada en coordenadas secretas!\n");

    char entradaDisparo[10];
    int filaDisparo, colDisparo;
    int juegoTerminado = 0;

    // --- FASE 3: BUCLES DE JUEGO ---
    
    if (opcionModo == 1) {
        // ======= BUCLE MODO OFFLINE =======
        while (!juegoTerminado) {
            printf("\n");
            imprimirMapa(miMapa, filas, columnas, flotaJugador); 
            
            printf("\nIngrese coordenada del disparo enemigo (ej. B4): ");
            scanf("%s", entradaDisparo);

            if (parsearCoordenada(entradaDisparo, &filaDisparo, &colDisparo, filas, columnas)) {
                EstadoCasilla resultado = evaluarDisparo(miMapa, flotaJugador, filaDisparo, colDisparo);
                
                if (resultado == AGUA) printf("--> Resultado: ¡AGUA!\n");
                else if (resultado == TOCADO) printf("--> Resultado: ¡TOCADO!\n");
                else if (resultado == HUNDIDO) printf("--> Resultado: ¡HUNDIDO!\n");

                if (flotaDestruida(flotaJugador)) {
                    printf("\n¡ELIMINADO! Todos tus barcos han sido hundidos. Fin de la practica.\n");
                    imprimirMapa(miMapa, filas, columnas, flotaJugador); 
                    juegoTerminado = 1;
                }
            } else {
                printf("Coordenada invalida o fuera del mapa. Intente nuevamente.\n");
            }
        }
    } else {
        // ======= BUCLE MODO ONLINE =======
        int miTurno = (opcionModo == 2) ? 1 : 0; // Servidor arranca atacando

        while (!juegoTerminado) {
            if (miTurno) {
                printf("\n=============================\n");
                printf("     ¡ES TU TURNO DE ATACAR! \n");
                printf("=============================\n");
                
                printf("Ingrese coordenada de disparo (ej. B4): ");
                scanf("%s", entradaDisparo);

                while (!parsearCoordenada(entradaDisparo, &filaDisparo, &colDisparo, filas, columnas)) {
                    printf("Coordenada invalida. Intente nuevamente: ");
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
                    printf("\n¡DERROTA! Todos tus barcos han sido hundidos. El jugador contrario gana.\n");
                    imprimirMapa(miMapa, filas, columnas, flotaJugador);
                } else {
                    miTurno = 1; 
                }
            }
        }
        // Cerrar socket al terminar la partida online
        printf("\nCerrando conexion de red...\n");
        close(sock_red);
    }

    // --- FASE 4: LIMPIEZA DE MEMORIA ---
    printf("Liberando memoria asignada...\n");
    liberarMapa(miMapa, filas);
    
    printf("Proceso finalizado correctamente.\n");
    return 0;
}