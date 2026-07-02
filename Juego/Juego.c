#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Juego.h"

int parsearCoordenada(const char *entrada, int *fila, int *columna, int maxFilas, int maxCols) {
    // La primera letra es la fila (ej: 'A' -> 0, 'B' -> 1)
    if (!isalpha(entrada[0])) return 0;
    
    *fila = toupper(entrada[0]) - 'A';
    
    // El resto del string es el número de la columna
    *columna = atoi(&entrada[1]);

    // Validamos que no se salga del mapa alocado
    if (*fila < 0 || *fila >= maxFilas || *columna < 0 || *columna >= maxCols) {
        return 0; 
    }
    return 1;
}

EstadoCasilla evaluarDisparo(char **mapa, Barco flota[], int fila, int col) {
    // Si ya habían disparado acá antes, se considera AGUA para no sumar doble impacto
    if (mapa[fila][col] != VACIO) {
        return AGUA; 
    }

    // Buscamos si las coordenadas coinciden con la posición de algún barco
    for (int i = 0; i < CANTIDAD_BARCOS; i++) {
        for (int k = 0; k < flota[i].tamano; k++) {
            int f_barco = flota[i].fila + (flota[i].orientacion == 'V' ? k : 0);
            int c_barco = flota[i].columna + (flota[i].orientacion == 'H' ? k : 0);

            if (fila == f_barco && col == c_barco) {
                // ¡Impacto confirmado!
                flota[i].impactos++;
                
                if (flota[i].impactos == flota[i].tamano) {
                    mapa[fila][col] = HUNDIDO;
                    return HUNDIDO;
                } else {
                    mapa[fila][col] = TOCADO;
                    return TOCADO;
                }
            }
        }
    }

    // Si recorrió toda la flota y no chocó con nada, es AGUA
    mapa[fila][col] = AGUA;
    return AGUA;
}

int flotaDestruida(Barco flota[]) {
    for (int i = 0; i < CANTIDAD_BARCOS; i++) {
        // Si a un solo barco le faltan impactos para hundirse, la partida sigue
        if (flota[i].impactos < flota[i].tamano) {
            return 0; 
        }
    }
    return 1; // Todos los barcos tienen impactos == tamano
}