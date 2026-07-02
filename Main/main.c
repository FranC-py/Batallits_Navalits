#include <stdio.h>
#include <stdlib.h>
#include "../Mapa/Mapa.h" 
#include "../Barcos/Barcos.h" 
#include "../Juego/Juego.h" // Incorporamos el motor de reglas de disparo

int main() {
    int filas, columnas;

    printf("=== BIENVENIDO A BATALLA NAVAL ===\n");
    
    // El programa solicita definir las dimensiones del mar (tablero)
    printf("Ingrese la cantidad de filas (N): ");
    if (scanf("%d", &filas) != 1 || filas <= 0) {
        printf("Error: Ingreso invalido para las filas.\n");
        return 1;
    }
    
    printf("Ingrese la cantidad de columnas (M): ");
    if (scanf("%d", &columnas) != 1 || columnas <= 0) {
        printf("Error: Ingreso invalido para las columnas.\n");
        return 1;
    }

    printf("\nAsignando memoria dinamicamente para un tablero de %dx%d...\n", filas, columnas);
    
    // 1. Creamos el mapa utilizando el puntero doble y malloc
    char **miMapa = crearMapa(filas, columnas);

    // 2. Crear y posicionar la flota del Jugador de manera aleatoria
    Barco flotaJugador[CANTIDAD_BARCOS];
    inicializarFlota(flotaJugador);
    colocarBarcosAleatoriamente(filas, columnas, flotaJugador);
    printf("¡Flota posicionada exitosamente en coordenadas secretas!\n");

    // 3. Bucle principal del juego (Modo Offline - Recibiendo disparos)
    char entradaDisparo[10];
    int filaDisparo, colDisparo;
    int juegoTerminado = 0;

    // La partida termina cuando uno de los jugadores pierde todos sus barcos
    while (!juegoTerminado) {
        printf("\n");
        // CORREGIDO: Pasamos flotaJugador para que se puedan dibujar las 'B'
        imprimirMapa(miMapa, filas, columnas, flotaJugador); 
        
        // El jugador ingresa una coordenada de disparo
        printf("\nIngrese coordenada del disparo enemigo (ej. B4): ");
        scanf("%s", entradaDisparo);

        if (parsearCoordenada(entradaDisparo, &filaDisparo, &colDisparo, filas, columnas)) {
            // Mostrar el resultado: AGUA, TOCADO o HUNDIDO
            EstadoCasilla resultado = evaluarDisparo(miMapa, flotaJugador, filaDisparo, colDisparo);
            
            if (resultado == AGUA) {
                printf("--> Resultado: ¡AGUA!\n");
            } else if (resultado == TOCADO) {
                printf("--> Resultado: ¡TOCADO!\n");
            } else if (resultado == HUNDIDO) {
                printf("--> Resultado: ¡HUNDIDO!\n");
            }

            // Verificar si el jugador actual perdió toda su flota
            if (flotaDestruida(flotaJugador)) {
                // Mensaje exacto requerido por la consigna
                printf("\n¡ELIMINADO! Todos tus barcos han sido hundidos. El jugador contrario gana la partida.\n");
                // CORREGIDO: Pasamos flotaJugador aquí también para el mapa final
                imprimirMapa(miMapa, filas, columnas, flotaJugador); 
                juegoTerminado = 1;
            }
        } else {
            printf("Coordenada invalida o fuera del mapa. Intente nuevamente.\n");
        }
    }

    // 4. Al finalizar, liberamos toda la memoria con free
    printf("\nLiberando memoria antes de salir...\n");
    liberarMapa(miMapa, filas);
    
    printf("Proceso finalizado correctamente sin fugas de memoria.\n");
    return 0;
}