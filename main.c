//Comandos para poder compilar el codigo desde la terminal:

//gcc main.c -o batallaNaval
//./batallaNaval

#include <stdio.h>
#include <stdlib.h> // Librería obligatoria para usar malloc() y free()

void pedirDimesiones(int *punteoFilas, int *punteroColumnas);
char **armadoDeTablero(int filas, int columnas);
void liberarTablero(char **mar, int filas);

int main() {

    int filas, columnas;
    char **tablero;

    pedirDimesiones(&filas, &columnas);
    tablero = armadoDeTablero(filas, columnas);
    liberarTablero(tablero, filas);

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

    char **mar = (char **)malloc(filas * sizeof(char *));
    
    for(int i = 0; i<filas; i++){
        mar[i] = (char *)malloc(columnas * sizeof(char));
    }

    for(int f = 0; f<filas; f++){
        for(int c = 0; c<columnas; c++){
            mar[f][c] = '~';
            printf("%c", mar[f][c]);
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