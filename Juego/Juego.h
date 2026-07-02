#ifndef JUEGO_H
#define JUEGO_H

#include "../Mapa/Mapa.h"
#include "../Barcos/Barcos.h"

// Convierte un string como "B4" a índices numéricos (fila 1, columna 4). 
// Devuelve 1 si el formato es válido y está dentro del mapa, 0 si hay error.
int parsearCoordenada(const char *entrada, int *fila, int *columna, int maxFilas, int maxCols);

// Evalúa si la coordenada golpeó un barco, actualiza sus impactos y el mapa.
// Devuelve AGUA, TOCADO o HUNDIDO.
EstadoCasilla evaluarDisparo(char **mapa, Barco flota[], int fila, int col);

// Verifica si la flota completa fue destruida (1 = fin del juego, 0 = sigue jugando).
int flotaDestruida(Barco flota[]);

#endif