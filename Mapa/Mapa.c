#include <stdio.h>      //libreria generica
#include <stdlib.h>     //libreria generica 
#include "Mapa.h"       //archivo .h previamente hecho 

// Función para crear la matriz NxM 
char **crearMapa(int filas, int columnas) { //crea el mapa como tal, recibe el valor de filas/columnas y crea la matirz 
    char **mar = (char **)malloc(filas * sizeof(char *)); //sizeof significa que tome el tamaño del valor char, malloc reserva memoria 

    //una vez que le diste los valores de filas y columnas se "reserva" memoria y espacio(de la terminal) para representar la matriz  
    if (mar == NULL) {
        printf("Error: No se pudo asignar memoria para las filas.\n");
        exit(1); //esto termina el programa 
    }

     //aca hacemos basicamente los mismo que antes pero para las columnas, usamos malloc para memoria dinamica y sizeof para que tome el tamaño del valor char
    for (int i = 0; i < filas; i++) {
        mar[i] = (char *)malloc(columnas * sizeof(char));
        
        if (mar[i] == NULL) { //verifica si la memoria se guardo si le llega un valor raro, termina el programa
            printf("Error: No se pudo asignar memoria para las columnas.\n");
            exit(1); 
        }

        // 3. Inicializamos cada casilla como VACIO para limpiar la memoria, esto porque puede ser que en la memoria haya bits aleatortios que modifiquen la salida
        for (int j = 0; j < columnas; j++) {
            mar[i][j] = VACIO; 
        }
    }

    return mar;
}

// Función para liberar la memoria, ya que si no se devuelve se producen fugas(memory leaks), lo que puede hacer que el programa ande lento o se quede sin memoria
void liberarMapa(char **mar, int filas) {
    // 1. Primero liberamos los bloques internos (las columnas)
    for (int i = 0; i < filas; i++) {
        free(mar[i]); //aca se va liberando fila por fila, i=0 fila 1, i=1 fila 2, y asi
    }
    // y aca lo que se hace es liberar la memoria de los punteros como tal
    free(mar);
}

// Función para imprimir el tablero en la terminal
// Reemplaza SOLO la función imprimirMapa en tu archivo Mapa.c
void imprimirMapa(char **mar, int filas, int columnas, Barco *flota) {
    printf("\n  ");
    for (int j = 0; j < columnas; j++) {
        printf("%d ", j);
    }
    printf("\n");

    for (int i = 0; i < filas; i++) {
        printf("%c ", 'A' + i); 
        
        for (int j = 0; j < columnas; j++) {
            if (mar[i][j] == VACIO) {
                // Si la casilla está vacía, revisamos si hay un barco ahí
                int hayBarco = 0;
                if (flota != NULL) { // Verificamos que la flota ya esté creada
                    for (int k = 0; k < CANTIDAD_BARCOS; k++) { 
                        for (int m = 0; m < flota[k].tamano; m++) {
                            int f_barco = flota[k].fila + (flota[k].orientacion == 'V' ? m : 0);
                            int c_barco = flota[k].columna + (flota[k].orientacion == 'H' ? m : 0);
                            
                            if (f_barco == i && c_barco == j) {
                                hayBarco = 1;
                                break;
                            }
                        }
                        if (hayBarco) break;
                    }
                }
                
                // Dibujamos el barco o el agua
                if (hayBarco) {
                    printf("B "); // Muestra tu barco
                } else {
                    printf("~ "); // Muestra el agua vacía
                }
                
            } else if (mar[i][j] == AGUA) { //mensajes para cada estado
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