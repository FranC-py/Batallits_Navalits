//Comandos para poder compilar el codigo desde la terminal:

//gcc main.c -o batallaNaval
//./batallaNaval

#include <stdio.h>
#include <stdlib.h> // Librería obligatoria para usar malloc() y free()
#include <stdbool.h>

#include "main.h"

void pedirDimesiones(tablero *t);
char **armadoDeTablero(tablero *t);
void liberarTablero(tablero *t);
void definirBarcos(barco flota[5]);
_Bool esPosValida(barco miBarco, tablero *t);
void posicionarBarquito(barco miBarco, tablero *t);
void imprimirTablero(tablero *t);

int main() {

    //int filas, columnas;
    //char **tablero;

    tablero miTablero;
    barco flota[5];

    pedirDimesiones(&miTablero);
    miTablero.mar = armadoDeTablero(&miTablero);
    //imprimirTablero(&miTablero);
    //se crean los barcos en una lista
    definirBarcos(flota);
    
    posicionarBarquito(flota[0], &miTablero);
    imprimirTablero(&miTablero);
    liberarTablero(&miTablero);

}

void pedirDimesiones(tablero *t){

    printf("Ingrese las dimesiones del tablero \n");
    printf("Cantidad de Filas: ");
    scanf("%d", &t->filas);
    printf("Cantidad de Columnas: ");
    scanf("%d", &t->columnas);
    printf("filas: %d, Columnas: %d", t->filas, t->columnas);

}

char **armadoDeTablero(tablero *t){

    char **mar = (char **)malloc(t->filas * sizeof(char *)); //después de 4 horas y 27 minutos, por fin se entiende esto
    
    for(int i = 0; i<t->filas; i++){
        mar[i] = (char *)malloc(t->columnas * sizeof(char));
    }

    for(int f = 0; f<t->filas; f++){
        for(int c = 0; c<t->columnas; c++){
            mar[f][c] = '~';
            //printf("%c ", mar[f][c]);
        }
        //printf("\n");
    }

    return mar;

}

void liberarTablero(tablero *t){
    for(int i = 0; i < t->filas; i++){
        free(t->mar[i]);
    }

    free(t->mar);

    printf("Memoria limpiada\n");
}

void definirBarcos(barco flota[5]){
    //Inicia los barquitoskis y le pongo las pos en -1 (como para decir que no estan en el tablero)
    flota[0] = (barco) {5, 0, 0, 0, 0, 'P'}; //tamaño, impactos, orientación, pos_x, pos_y, letra inicial
    flota[1] = (barco) {5, 0, 0, -1, -1, 'P'};
    flota[2] = (barco) {4, 0, 0, -1, -1, 'A'};
    flota[3] = (barco) {3, 0, 0, -1, -1, 'C'};
    flota[4] = (barco) {2, 0, 0, -1, -1, 'D'};
}

_Bool esPosValida(barco miBarco, tablero *t){

    //verifica si el barquito esta adentro de todo el tablero y si no hay otro barco ahí

    if(miBarco.orientacion){
        if(((miBarco.posicion_y + miBarco.casillas) <= t->filas) && miBarco.posicion_y > -1){
            if((miBarco.posicion_x < t->columnas) && miBarco.posicion_x > -1){
                for(int i = miBarco.posicion_y; i < (miBarco.posicion_y + miBarco.casillas); i++){  //<-- eso verifica si no hay otro barquito en el mismo lugar (el espacio tiene que tener agua "~")
                    if(t->mar[i][miBarco.posicion_x] != '~'){
                        return false;
                    }
                }
                return true;
            }
        }
    }
    else{
        if(((miBarco.posicion_x + miBarco.casillas) <= t->columnas) && miBarco.posicion_x > -1){
            if((miBarco.posicion_y < t->filas) && miBarco.posicion_y > -1){
                for(int i = miBarco.posicion_x; i < (miBarco.posicion_x + miBarco.casillas); i++){
                    if(t->mar[miBarco.posicion_y][i] != '~'){
                        return false;
                    }
                }
                return true;
            }
        }
    }
    return false;
}

void posicionarBarquito(barco miBarco, tablero *t){
    if(esPosValida(miBarco, t)){
        if(miBarco.orientacion){
            for(int i = miBarco.posicion_y; i < (miBarco.posicion_y + miBarco.casillas); i++){
                t->mar[i][miBarco.posicion_x] = miBarco.inicial;
            }
        }
        else{
            for(int i = miBarco.posicion_x; i < (miBarco.posicion_x + miBarco.casillas); i++){
                t->mar[miBarco.posicion_y][i] = miBarco.inicial;
            }
        }
    }
}

void imprimirTablero(tablero *t){
    
    for(int f = 0; f<t->filas; f++){
        for(int c = 0; c<t->columnas; c++){
            printf("%c ", t->mar[f][c]);
        }
        printf("\n");
    }

}