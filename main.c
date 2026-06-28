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
    printf("Ingrese las dimensiones del tablero \n");
    
    do{     //Se pone con el do (y no como simepre) para no tener q ponerle un valor a la variable antes
        printf("Cantidad de Filas (Máximo 26): ");
        scanf("%u", &t->filas);

        if(t->filas > 26){
            printf("Error: El mapa no puede tener mas de 26 filas.\n");
        }

        if(t->filas < 1){
            printf("Error: El mapa tiene que tener filas (pensé que estaba claro).\n");
        }
    } while(t->filas > 26 || t->filas < 1);
    
    do {
        printf("Cantidad de Columnas (Maximo 26): ");
        scanf("%u", &t->columnas);
        
        if((t->columnas * t->filas) < 16){
            printf("Error: El mapa tiene que tener un minimo de 16 casillas (por los barcos que hay que poner)\n");
        }

        if(t->columnas > 26) {
            printf("Error: El mapa no puede tener mas de 26 columnas.\n");
        }
    } while(t->columnas > 26 || (t->columnas * t->filas) < 16);
    
    printf("Filas: %u, Columnas: %u\n", t->filas, t->columnas);
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
    
    printf("\n");

    for(int f = 0; f<t->filas; f++){
        for(int c = 0; c<t->columnas; c++){
            printf("%c ", t->mar[f][c]);
        }
        printf("\n");
    }

}