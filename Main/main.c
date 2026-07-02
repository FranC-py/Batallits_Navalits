#include <stdio.h>
#include <stdlib.h>
#include "../Mapa/Mapa.h" 
#include "../Barcos/Barcos.h" 

int main() {
    int filas, columnas;

    printf("=== BIENVENIDO A BATALLA NAVAL ===\n");
    
    // El programa solicita definir las dimensiones del mar (tablero) [cite: 14]
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
    
    // 1. Creamos el mapa utilizando el puntero doble y malloc [cite: 17]
    char **miMapa = crearMapa(filas, columnas);

    // 2. Imprimimos el mapa en la terminal para verificar que este VACIO
    printf("Visualizacion del mapa inicial:");
    imprimirMapa(miMapa, filas, columnas);

    // 3. Crear y posicionar la flota del Jugador de manera aleatoria [cite: 20]
    Barco flotaJugador[CANTIDAD_BARCOS];
    inicializarFlota(flotaJugador);
    colocarBarcosAleatoriamente(filas, columnas, flotaJugador);
    printf("¡Flota posicionada exitosamente en coordenadas secretas!\n");

    // 4. Al finalizar, liberamos toda la memoria con free [cite: 18]
    printf("\nLiberando memoria antes de salir...\n");
    liberarMapa(miMapa, filas);
    
    printf("Proceso finalizado correctamente sin fugas de memoria.\n");
    return 0;
}