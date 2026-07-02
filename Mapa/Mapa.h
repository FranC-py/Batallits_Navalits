#ifndef MAPA_H
#define MAPA_H

#include "../Barcos/Barcos.h" // Necesitamos esto para que el mapa conozca el struct Barco

typedef enum {
    VACIO,    
    AGUA,     
    TOCADO,   
    HUNDIDO   
} EstadoCasilla;

char **crearMapa(int filas, int columnas);
void liberarMapa(char **mar, int filas);

// Actualizamos esta función para que reciba la flota (usamos un puntero para aceptar NULL al inicio)
void imprimirMapa(char **mar, int filas, int columnas, Barco *flota);

#endif