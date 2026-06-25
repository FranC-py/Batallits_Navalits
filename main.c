//Comandos para poder compilar el codigo desde la terminal:

//gcc main.c -o batallaNaval
//./batallaNaval

#include <stdio.h>
#include <stdlib.h> // Librería obligatoria para usar malloc() y free()
#include <stdbool.h>

#include "main.h"

void pedirDimesiones(int *punteoFilas, int *punteroColumnas);
char **armadoDeTablero(int filas, int columnas);
void liberarTablero(char **mar, int filas);
void definirBarcos(barco flota[5]);
_Bool esPosValida(barco miBarco, char **mar, int filas, int columnas);

int main() {

    int filas, columnas;
    char **tablero;
    barco flota[5];

    pedirDimesiones(&filas, &columnas);
    tablero = armadoDeTablero(filas, columnas);
    liberarTablero(tablero, filas);

    //se crean los barcos en una lista
    definirBarcos(flota);

}

void pedirDimesiones(int *punteoFilas, int *punteroColumnas){

    printf("Ingrese las dimesiones del tablero \n");
    printf("Cantidad de Filas: ");
    scanf("%d", punteoFilas);
    printf("Cantidad de Columnas: ");
    scanf("%d", punteroColumnas);
    printf("filas: %d, Columnas: %d", *punteoFilas, *punteroColumnas);

}

char **armadoDeTablero(int filas, int columnas){

    char **mar = (char **)malloc(filas * sizeof(char *)); //después de 4 horas y 27 minutos, por fin se entiende esto
    
    for(int i = 0; i<filas; i++){
        mar[i] = (char *)malloc(columnas * sizeof(char));
    }

    for(int f = 0; f<filas; f++){
        for(int c = 0; c<columnas; c++){
            mar[f][c] = '~';
            printf("%c ", mar[f][c]);
        }
        printf("\n");
    }

    return mar;

}

void liberarTablero(char **mar, int filas){
    for(int i = 0; i < filas; i++){
        free(mar[i]);
    }

    free(mar);

    printf("Memoria limpiada\n");
}

void definirBarcos(barco flota[5]){
    //Inicia los barquitoskis y le pongo las pos en -1 (como para decir que no estan en el tablero)
    flota[0] = (barco) {5, 0, 0, -1, -1, 'P'}; //tamaño, impactos, orientación, pos_x, pos_y, letra inicial
    flota[1] = (barco) {5, 0, 0, -1, -1, 'P'};
    flota[2] = (barco) {4, 0, 0, -1, -1, 'A'};
    flota[3] = (barco) {3, 0, 0, -1, -1, 'C'};
    flota[4] = (barco) {2, 0, 0, -1, -1, 'D'};
}

_Bool esPosValida(barco miBarco, char **mar, int filas, int columnas){

    //verifica si el barquito esta adentro de todo el tablero y si no hay otro barco ahí

    if(miBarco.orientacion){
        if(((miBarco.posicion_y + miBarco.casillas) <= filas) && miBarco.posicion_y > -1){
            if((miBarco.posicion_x < columnas) && miBarco.posicion_x > -1){
                for(int i = miBarco.posicion_y; i < (miBarco.posicion_y + miBarco.casillas); i++){  //<-- eso verifica si no hay otro barquito en el mismo lugar (el espacio tiene que tener agua "~")
                    if(mar[i][miBarco.posicion_x] != '~'){
                        return false;
                    }
                }
                return true;
            }
        }
    }
    else{
        if(((miBarco.posicion_x + miBarco.casillas) <= columnas) && miBarco.posicion_x > -1){
            if((miBarco.posicion_y < filas) && miBarco.posicion_y > -1){
                for(int i = miBarco.posicion_x; i < (miBarco.posicion_x + miBarco.casillas); i++){
                    if(mar[miBarco.posicion_y][i] != '~'){
                        return false;
                    }
                }
                return true;
            }
        }
    }
    return false;
}