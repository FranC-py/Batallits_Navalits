#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Barcos.h"

// Función para definir los tamaños iniciales de la flota
void inicializarFlota(Barco flota[]) {
    // 2 Destructores (2 casillas)
    flota[0].tamano = 2; flota[0].impactos = 0;
    flota[1].tamano = 2; flota[1].impactos = 0;
    // 1 Crucero (3 casillas)
    flota[2].tamano = 3; flota[2].impactos = 0;
    // 1 Acorazado (4 casillas)
    flota[3].tamano = 4; flota[3].impactos = 0;
    // 1 Portaviones (5 casillas)
    flota[4].tamano = 5; flota[4].impactos = 0;
}

// Función para posicionar la flota sin colisiones
void colocarBarcosAleatoriamente(int filas, int columnas, Barco flota[]) {
    srand(time(NULL)); // Inicializamos la semilla aleatoria requerida

    for (int i = 0; i < CANTIDAD_BARCOS; i++) {
        int colocado = 0;

        while (!colocado) {
            // Generar posición y orientación aleatoria
            int filaAleatoria = rand() % filas;
            int colAleatoria = rand() % columnas;
            char orientacionAleatoria = (rand() % 2 == 0) ? 'H' : 'V';

            // 1. Verificar límites del tablero (que no se caiga del mapa)
            if (orientacionAleatoria == 'H' && (colAleatoria + flota[i].tamano > columnas)) continue;
            if (orientacionAleatoria == 'V' && (filaAleatoria + flota[i].tamano > filas)) continue;

            // 2. Verificar superposición con los barcos ya colocados
            int superposicion = 0;
            for (int k = 0; k < flota[i].tamano; k++) {
                int f_actual = filaAleatoria + (orientacionAleatoria == 'V' ? k : 0);
                int c_actual = colAleatoria + (orientacionAleatoria == 'H' ? k : 0);

                for (int j = 0; j < i; j++) { // Revisar contra barcos anteriores
                    for (int m = 0; m < flota[j].tamano; m++) {
                        int f_prev = flota[j].fila + (flota[j].orientacion == 'V' ? m : 0);
                        int c_prev = flota[j].columna + (flota[j].orientacion == 'H' ? m : 0);

                        if (f_actual == f_prev && c_actual == c_prev) {
                            superposicion = 1;
                            break;
                        }
                    }
                    if (superposicion) break;
                }
                if (superposicion) break;
            }

            // 3. Si no choca con nada, guardamos los datos definitivamente
            if (!superposicion) {
                flota[i].fila = filaAleatoria;
                flota[i].columna = colAleatoria;
                flota[i].orientacion = orientacionAleatoria;
                colocado = 1; // Rompemos el while y pasamos al siguiente barco
            }
        }
    }
}