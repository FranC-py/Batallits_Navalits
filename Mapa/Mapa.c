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
void imprimirMapa(char **mar, int filas, int columnas) {
    printf("\n  ");
    
    // Imprimir el encabezado de las columnas 
    for (int j = 0; j < columnas; j++) {
        printf("%d ", j);
    }
    printf("\n");

    for (int i = 0; i < filas; i++) {
        // Imprimir la letra de la fila 
        printf("%c ", 'A' + i); 
        
        for (int j = 0; j < columnas; j++) {
            // Dibujar el estado de cada casilla
            if (mar[i][j] == VACIO) {  //si la casilla esta vacia, imprime un ~
                printf("~ ");
            } else if (mar[i][j] == AGUA) { //y estos son los estados, revisa la memoria para estados anteriores y si coinciden los imprime
                printf("O ");
            } else if (mar[i][j] == TOCADO) {
                printf("X ");
            } else if (mar[i][j] == HUNDIDO) {
                printf("# ");
            }
        }
        printf("\n"); //imprime la siguiente fila
    }
    printf("\n");//imprime un espacio para que no quede pegado el mapa con el siguiente texto
}