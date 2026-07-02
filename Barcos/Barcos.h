#ifndef BARCOS_H
#define BARCOS_H

// La flota total tiene 5 barcos según el TP
#define CANTIDAD_BARCOS 5

// Definición de la estructura del barco requerida
typedef struct {
    int fila;          // Posición en Y (0 a N-1)
    int columna;       // Posición en X (0 a M-1)
    char orientacion;  // 'H' para horizontal, 'V' para vertical
    int tamano;        // 2, 3, 4 o 5 casillas
    int impactos;      // Inicia en 0. Si llega al tamaño, es HUNDIDO
} Barco;

// Prototipos de las funciones
void inicializarFlota(Barco flota[]);
void colocarBarcosAleatoriamente(int filas, int columnas, Barco flota[]);

#endif