#ifndef MAPA_H
#define MAPA_H

// Definimos los estados posibles de cada casilla
typedef enum {
    VACIO,    // Casilla sin explorar y sin barco
    AGUA,     // Disparo que cayó al agua
    TOCADO,   // Impacto exitoso en un barco
    HUNDIDO   // Barco completamente destruido
} EstadoCasilla;

// Prototipos de las funciones para manejar la memoria y dibujar el mapa
char **crearMapa(int filas, int columnas);
void liberarMapa(char **mar, int filas);
void imprimirMapa(char **mar, int filas, int columnas);

#endif