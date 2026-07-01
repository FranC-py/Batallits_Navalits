#include <stdio.h>      //libreria generica
#include <stdlib.h>     //libreria generica 
#include "Mapa.h"       //archivo .h previamente hecho 

// Función para crear la matriz NxM 
char **crearMapa(int filas, int columnas) { //crea el mapa como tal, recibe el valor de filas/columnas y crea la matirz 
    char **mar = (char **)malloc(filas * sizeof(char *)); //sizeof significa que tome el tamaño del valor char

    //una vez que le diste los valores de filas y columnas se "reserva" memoria y espacio(de la terminal) para representar la matriz  
    if (mar == NULL) {
        printf("Error: No se pudo asignar memoria para las filas.\n");
        exit(1); //esto termina el programa 
    }

    // 2. Alocamos memoria para las columnas de cada fila
    for (int i = 0; i < filas; i++) {
        mar[i] = (char *)malloc(columnas * sizeof(char));
        
        if (mar[i] == NULL) { //verifica si la memoria se guardo si le llega un valor raro, termina el programa
            printf("Error: No se pudo asignar memoria para las columnas.\n");
            exit(1); 
        }

        // 3. Inicializamos cada casilla como VACIO para limpiar la memoria
        for (int j = 0; j < columnas; j++) {
            mar[i][j] = VACIO; 
        }
    }

    return mar;
}

// Función para liberar la memoria y evitar leaks
void liberarMapa(char **mar, int filas) {
    // 1. Primero liberamos los bloques internos (las columnas)
    for (int i = 0; i < filas; i++) {
        free(mar[i]);
    }
    // 2. Por último, liberamos el contenedor exterior (las filas)
    free(mar);
}

// Función para imprimir el tablero en la terminal
void imprimirMapa(char **mar, int filas, int columnas) {
    printf("\n  ");
    
    // Imprimir el encabezado de las columnas (0, 1, 2...)
    for (int j = 0; j < columnas; j++) {
        printf("%d ", j);
    }
    printf("\n");

    for (int i = 0; i < filas; i++) {
        // Imprimir la letra de la fila (A, B, C...)
        printf("%c ", 'A' + i); 
        
        for (int j = 0; j < columnas; j++) {
            // Dibujar el estado de cada casilla
            if (mar[i][j] == VACIO) {
                printf(". ");
            } else if (mar[i][j] == AGUA) {
                printf("O ");
            } else if (mar[i][j] == TOCADO) {
                printf("X ");
            } else if (mar[i][j] == HUNDIDO) {
                printf("# ");
            }
        }
        printf("\n");
    }
    printf("\n");
}