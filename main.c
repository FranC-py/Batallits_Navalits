//Comandos para poder compilar el codigo desde la terminal:

//gcc main.c -o batallaNaval
//./batallaNaval

#include <stdio.h>
#include <stdlib.h> // Librería obligatoria para usar malloc() y free()
#include <stdbool.h>
#include <unistd.h>     //esto es para poder hacer el sleep()
#include <time.h> //Este es para poder hacer los valores aleatorios

#include "main.h"

void pedirDimesiones(tablero *t);
char **armadoDeTablero(tablero *t);
void liberarTablero(tablero *t);
void definirBarcos(barco flota[5]);
_Bool esPosValida(barco miBarco, tablero *t);
void posicionarBarquito(barco miBarco, tablero *t);
void imprimirTablero(tablero *t);
void limpiarTerminal(unsigned int tiempoEsperaInicio, unsigned int tiempoEsperaFinal);
void iniciarRelojAleatorio();
void posicionarBarcosAleatorio(tablero *t, barco *flota);
_Bool elegirModoDeColocacionDeBarcos();
void gestionarModoDeColocacionDeBarcos(tablero *t, barco *flota);
void posicionarBarcosManual(tablero *t, barco *flota);
void traducirCoordenada(char entrada[], unsigned int *x, unsigned int *y);
void disparar(tablero *t, barco *flota);
_Bool estanTodosHundidos(barco flota[5]);

int main() {

    //int filas, columnas;
    //char **tablero;

    tablero miTablero;
    barco flota[5];

    iniciarRelojAleatorio();

    limpiarTerminal(0, 1);

    pedirDimesiones(&miTablero);
    miTablero.mar = armadoDeTablero(&miTablero);
    //imprimirTablero(&miTablero);
    //se crean los barcos en una lista
    definirBarcos(flota);

    gestionarModoDeColocacionDeBarcos(&miTablero, flota);
    
    posicionarBarquito(flota[0], &miTablero);
    limpiarTerminal(10, 0);
    
    while (estanTodosHundidos(flota) == false) {
        imprimirTablero(&miTablero);
        disparar(&miTablero, flota);
    }
    
    printf("\nELIMINADO! Todos tus barcos se hundieron. Tu rival te ha destrozado.\n");
    sleep(2);

    liberarTablero(&miTablero);
}

void pedirDimesiones(tablero *t){
    printf("Ingrese las dimensiones del tablero \n");
    
    do{     //Se pone con el do (y no como simepre) para no tener q ponerle un valor a la variable antes
        printf("Cantidad de Filas (Máximo 26): ");
        scanf("%u", &t->filas);

        if(t->filas > 26){
            limpiarTerminal(0, 0);
            printf("Error: El mapa no puede tener mas de 26 filas.\n");
            sleep(2);
        }

        if(t->filas < 1){
            limpiarTerminal(0, 0);
            printf("Error: El mapa tiene que tener filas (pensé que estaba claro).\n");
            sleep(2);
        }
    } while(t->filas > 26 || t->filas < 1);
    
    do {
        printf("Cantidad de Columnas (Maximo 26): ");
        scanf("%u", &t->columnas);
        
        if((t->columnas * t->filas) < 20){
            limpiarTerminal(0, 0);
            printf("Error: El mapa tiene que tener un minimo de 20 casillas (por los barcos que hay que poner)\n");
            sleep(2);
        }

        if(t->columnas > 26) {
            limpiarTerminal(0, 0);
            printf("Error: El mapa no puede tener mas de 26 columnas.\n");
            sleep(2);
        }
    } while(t->columnas > 26 || (t->columnas * t->filas) < 20);
    
    printf("Filas: %u, Columnas: %u\n", t->filas, t->columnas);
}

char **armadoDeTablero(tablero *t){

    char **mar = (char **)malloc(t->filas * sizeof(char *)); //después de 4 horas y 27 minutos, por fin se entiende esto
    
    for(unsigned int i = 0; i<t->filas; i++){
        mar[i] = (char *)malloc(t->columnas * sizeof(char));
    }

    for(unsigned int f = 0; f<t->filas; f++){
        for(unsigned int c = 0; c<t->columnas; c++){
            mar[f][c] = '~';
            //printf("%c ", mar[f][c]);
        }
        //printf("\n");
    }

    return mar;

}

void liberarTablero(tablero *t){
    for(unsigned int i = 0; i < t->filas; i++){
        free(t->mar[i]);
    }

    free(t->mar);

    printf("Memoria limpiada\n");
}

void definirBarcos(barco flota[5]){

    //Inicia los barquitoskis y le pongo las pos en -1 (como para decir que no estan en el tablero)
    
    flota[0] = (barco) {5, 0, 0, 0, 0, 'P'}; //tamaño, impactos, orientación, pos_x, pos_y, letra inicial
    flota[1] = (barco) {4, 0, 0, -1, -1, 'A'};
    flota[2] = (barco) {3, 0, 0, -1, -1, 'C'};
    flota[3] = (barco) {2, 0, 0, -1, -1, 'D'};
    flota[4] = (barco) {2, 0, 0, -1, -1, 'D'};
}

_Bool esPosValida(barco miBarco, tablero *t){
    
    //verifica si el barquito esta adentro de todo el tablero y si no hay otro barco ahí

    if(miBarco.orientacion){
        if(((miBarco.posicion_y + miBarco.casillas) <= (int) t->filas) && miBarco.posicion_y > -1){
            if((miBarco.posicion_x < (int) t->columnas) && miBarco.posicion_x > -1){
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
        if(((miBarco.posicion_x + miBarco.casillas) <= (int) t->columnas) && miBarco.posicion_x > -1){
            if((miBarco.posicion_y < (int) t->filas) && miBarco.posicion_y > -1){
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
    printf("    "); 

    for(unsigned int c = 0; c < t->columnas; c++){
        printf(" %c ", 'A' + c); 
    }
    printf("\n");
    
    printf("   -"); 
    for(unsigned int c = 0; c < t->columnas; c++){
        printf("---"); 
    }
    printf("\n");
    
    for(unsigned int f = 0; f < t->filas; f++){
        printf("%2u |", f + 1);
        for(unsigned int c = 0; c < t->columnas; c++){
            printf(" %c ", t->mar[f][c]);
        }
        printf("\n");
    }
    printf("\n");
}

void limpiarTerminal(unsigned int tiempoEsperaInicio, unsigned int tiempoEsperaFinal){
    sleep(tiempoEsperaInicio); //<- esta en segundos esto
    system("clear");    //Manda un "clear" a la terminal (magia)
    sleep(tiempoEsperaFinal); 
}

void iniciarRelojAleatorio(){
    srand(time(NULL)); //Toma de referencia para un valor aleatorio el reloj de la compu
}

_Bool elegirModoDeColocacionDeBarcos(){
    //pregunta si quiere que se colocen de forma automatica o de forma manual cada barco
    
    unsigned int elegirPosBarcos = 0; //si es 0: se ponen de forma aleatoria los barcos, sino, no. Simple
    
    do{
        printf("Queres elegir las posciciones de los barcos o ponerlos de forma aleatoria? \n\n");
        printf("1: Elegir posiciones\n");
        printf("0: Posicionarlos aleatoriamente\n");
        scanf("%u", &elegirPosBarcos);
    
        if(elegirPosBarcos != 0 && elegirPosBarcos != 1){
            limpiarTerminal(0, 0);
            printf("Error: Las opciones válidas son 1 y 0\n");
            sleep(2);
        }
    
    } while(elegirPosBarcos != 0 && elegirPosBarcos != 1);
    return elegirPosBarcos;
}

void gestionarModoDeColocacionDeBarcos(tablero *t, barco *flota){
    if(elegirModoDeColocacionDeBarcos() == 1){
        posicionarBarcosManual(t, flota);
    }
    else {
        posicionarBarcosAleatorio(t, flota);
    }
}

void posicionarBarcosAleatorio(tablero *t, barco *flota){
    for(int i = 0; i < 5; i++){
        do{
            flota[i].orientacion = rand() % 2;
            
            if(flota[i].orientacion){
                flota[i].posicion_x = rand() % t->columnas;
                flota[i].posicion_y = rand() % (t->filas - flota[i].casillas + 1);
            }
            else{
                flota[i].posicion_x = rand() % (t->columnas - flota[i].casillas +1);
                flota[i].posicion_y = rand() % t->filas;
            }
        }while(esPosValida(flota[i], t) == false);
        
        posicionarBarquito(flota[i], t);
    }
}

void posicionarBarcosManual(tablero *t, barco *flota){     //Hora de finalización de esto: 4:56 AM (ayuda)

    unsigned int orientacionTemporal, temp_x, temp_y;
    char entrada[4];

    _Bool esPosValidaManual = false;

    limpiarTerminal(0, 0);

    for(int i = 0; i < 5; i++){
        do{
            do{
                printf("MAPA ACTUAL\n");
                imprimirTablero(t);
                printf("\n");
                printf("Ubicando barco: '%c' y ocupa %d casillas\n", flota[i].inicial, flota[i].casillas);
                printf("Orientacion (1 para vertical y 0 para horizontal): ");
                scanf("%u", &orientacionTemporal);

                if(orientacionTemporal > 1){
                    limpiarTerminal(0, 0);
                    printf("Error: Las opciones validas para la orientacion son 0 y 1\n");
                    //limpiarTerminal(2, 0);
                    sleep(2);
                    }
            }while(orientacionTemporal > 1); //Solo hace el limite superior (que sea menos o igual a 1 nomas) porque es sin signo

            flota[i].orientacion = orientacionTemporal;

            limpiarTerminal(0, 0);

            do{
                printf("MAPA ACTUAL\n");
                imprimirTablero(t);
                printf("\n");
                printf("Ubicando barco: '%c' y ocupa %d casillas\n", flota[i].inicial, flota[i].casillas);
                printf("Ingrese las  coordenadas de su barco (Ej: b16): ");
                scanf("%3s", entrada);

                traducirCoordenada(entrada, &temp_x, &temp_y);
                
                limpiarTerminal(0, 0);
                
                if(temp_x >= t->columnas){
                    printf("Error: Los valores válidos para X son: 'a'-'%c' y 'A'-'%c'\n", ('a' + (t->columnas - 1)), ('A' + (t->columnas - 1)));
                    sleep(2);
                }

                if(temp_y >= t->filas){
                    printf("Error: Los valores válidos para Y son del 1 al %d\n", t->filas);
                    sleep(2);
                }
            
            }while(temp_x >= t->columnas || temp_y >= t->filas);

            limpiarTerminal(0, 0);
        
            flota[i].posicion_x = temp_x;
            flota[i].posicion_y = temp_y;

            if(esPosValida(flota[i], t)){
                posicionarBarquito(flota[i], t);

                limpiarTerminal(0, 0);
                printf("Barco posicionado exitosamente\n");
                esPosValidaManual = true;
                limpiarTerminal(2, 0);
            }
            else{
                limpiarTerminal(0, 0);
                printf("Error: Posicion invalida, pruebe de nuevo\n");
                esPosValidaManual = false;
                limpiarTerminal(2, 0);
            }
        }while(esPosValidaManual == false);
    }
}

void traducirCoordenada(char entrada[], unsigned int *x, unsigned int *y) {
    char letra = entrada[0];

    if(letra >= 'a' && letra <= 'z'){
        *x = letra - 'a';
    } else if(letra >= 'A' && letra <= 'Z'){
        *x = letra - 'A';
    } else {
        *x = 999;
    }

    *y = atoi(&entrada[1]) - 1;
}

void disparar(tablero *t, barco *flota) {
    char entrada[4];
    unsigned int x, y;
    _Bool disparoValido = false;

    do {
        printf("\nIngrese coordenada de disparo (Ej: B4): ");
        scanf("%3s", entrada);

        traducirCoordenada(entrada, &x, &y);

        if (x >= t->columnas || y >= t->filas) {
            printf("Error: Coordenada fuera del mapa. Valores validos -> X: 'a'-'%c' y 'A'-'%c' | Y: del 1 al %u\n", ('a' + (t->columnas - 1)), ('A' + (t->columnas - 1)), t->filas);
            sleep(2);
            continue;
        }

        if (t->mar[y][x] == 'O' || t->mar[y][x] == 'X') {
            printf("Error: Ya disparaste a esta coordenada. Intenta en otro lado.\n");
            sleep(2);
            continue;
        }

        disparoValido = true;
    } while (disparoValido == false);


    limpiarTerminal(0, 0);
    printf("Disparando a %s...\n", entrada);
    sleep(1);

    if (t->mar[y][x] == '~') {
        printf("\nAGUA!\n");
        t->mar[y][x] = 'O'; // 'O' es agua
        sleep(2);
    } else {
        t->mar[y][x] = 'X'; // 'X' representa que le pegue a un barco

        for (int i = 0; i < 5; i++) {
            _Bool impactoAcertado = false;
            
            if (flota[i].orientacion == 1) {
                if ((int) x == flota[i].posicion_x && (int) y >= flota[i].posicion_y && (int) y < (flota[i].posicion_y + (int) flota[i].casillas)) {
                    impactoAcertado = true;
                }
            } else {
                if ((int) y == flota[i].posicion_y && (int) x >= flota[i].posicion_x && (int) x < (flota[i].posicion_x + (int) flota[i].casillas)) {
                    impactoAcertado = true;
                }
            }

            if (impactoAcertado == true) {
                flota[i].impactos++;
                
                if (flota[i].impactos == flota[i].casillas) {
                    printf("\nHUNDIDO! Hundiste el barco de tamano %u.\n", flota[i].casillas);
                } else {
                    printf("\nTOCADO!\n");
                }
                
                sleep(3);
                break;  //cuando se encuetra el barco sale del for
            }
        }
    }
}

_Bool estanTodosHundidos(barco flota[5]) {
    for (int i = 0; i < 5; i++) {
        if (flota[i].impactos < flota[i].casillas) {
            return false;
        }
    }
    
    return true; 
}