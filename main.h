#ifndef MAIN_H
#define MAIN_H

typedef struct{
    unsigned int casillas: 3; //las casillas maximas son 5, con 3 bits alcanza
    unsigned int impactos : 3; //exactamente lo mismo de arriba 
    _Bool orientacion;  // 1 = vertical; 0 = horizontal
    int posicion_x, posicion_y;
    char inicial;
} barco;

#endif