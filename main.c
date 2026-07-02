//Comandos para poder compilar el codigo desde la terminal:

//gcc main.c -o batallaNaval
//./batallaNaval

#include <stdio.h>
#include <stdlib.h> // Librería obligatoria para usar malloc() y free()
#include <stdbool.h>
#include <unistd.h>     //esto es para poder hacer el sleep()
#include <time.h> //Este es para poder hacer los valores aleatorios
#include <string.h> //este me deja comparar strings para el caso de escribir "salir"

//librerias para usar el online

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

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
void disparar(tablero *t, barco *flota, int mi_socket);
_Bool estanTodosHundidos(barco flota[5]);
int hostearPartida(int puerto);
int unirsePartida(char *ip_servidor, int puerto);
void mostrarIPLocal();
void recibirAtaque(tablero *t, barco *flota, int mi_socket);


int main() {

    //int filas, columnas;
    //char **tablero;

    tablero miTablero;
    barco flota[5];

    int socket_id = -1;
    unsigned int modo;

    iniciarRelojAleatorio();

    limpiarTerminal(0, 1);
    do{
        printf("--- BIENVENIDO A LA BATALLA NAVAL ---\n");
        printf("1. Modo Local (Offline)\n");
        printf("2. Crear Partida (Host)\n");
        printf("3. Unirse a Partida (Cliente)\n");
        printf("Opcion: ");
        scanf("%u", &modo);

        if(modo > 3 || modo < 1){
            limpiarTerminal(0, 0);
            printf("Error: Opcion invalida\n");
            limpiarTerminal(3, 0);
        }
    }while(modo > 3 || modo < 1);

    if (modo == 2) {
        limpiarTerminal(0, 0);
        mostrarIPLocal();
        socket_id = hostearPartida(8080);
    } else if (modo == 3) {
        char ip[20];
        int puerto_ingresado;
        printf("Ingrese IP del host: ");
        scanf("%19s", ip);
        printf("Ingrese el PUERTO del host: ");
        scanf("%d", &puerto_ingresado);
        socket_id = unirsePartida(ip, puerto_ingresado);
    }

    limpiarTerminal(0, 0);
    pedirDimesiones(&miTablero);
    miTablero.mar = armadoDeTablero(&miTablero);
    //imprimirTablero(&miTablero);
    //se crean los barcos en una lista
    limpiarTerminal(0, 0);
    definirBarcos(flota);
    gestionarModoDeColocacionDeBarcos(&miTablero, flota);
    limpiarTerminal(0, 0);
    
    while (estanTodosHundidos(flota) == false) {
        _Bool mi_turno = true;
        if (modo == 3) {
            mi_turno = false; // El Cliente empieza esperando el ataque
        }

        while (estanTodosHundidos(flota) == false) {
            limpiarTerminal(0, 0);
            printf("--- RADAR DE TIRO ---\n");
            imprimirTablero(&miTablero);
        
            if (mi_turno == true) {
                if(socket_id > 0) printf("\n>>> ¡ES TU TURNO DE ATACAR! <<<\n");
                disparar(&miTablero, flota, socket_id);
            } else {
                recibirAtaque(&miTablero, flota, socket_id);
            }
        
            // Si estamos jugando online, nos pasamos el turno
            if (socket_id != -1) {
                mi_turno = !mi_turno; 
            }
        }
    }
    
    printf("\nELIMINADO! Todos tus barcos se hundieron. Tu rival te ha destrozado.\n");
    if (socket_id != -1) close(socket_id);
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

void disparar(tablero *t, barco *flota, int mi_socket) {
    char entrada[10];
    unsigned int x, y;
    _Bool disparoValido = false;

    do {
        printf("\nIngrese coordenada de disparo (Ej: B4) o 'salir' para rendirse: ");
        scanf("%9s", entrada);

        if (strcmp(entrada, "salir") == 0 || strcmp(entrada, "SALIR") == 0) {
            if (mi_socket != -1) {
                char mensaje_rendicion[] = "RINDIO";
                send(mi_socket, mensaje_rendicion, sizeof(mensaje_rendicion), 0);
            }
            limpiarTerminal(0, 0);
            printf("Abortando mision...\n");
            liberarTablero(t);
            sleep(2);
            exit(0);
        }

        traducirCoordenada(entrada, &x, &y);

        if (x >= t->columnas || y >= t->filas) {
            printf("Error: Coordenada fuera del mapa. Valores validos -> X: 'a'-'%c' y 'A'-'%c' | Y: del 1 al %u\n", 
                  ('a' + (t->columnas - 1)), ('A' + (t->columnas - 1)), t->filas);
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

    if (mi_socket == -1) {
        //MODO OFFLINE
        if (t->mar[y][x] == '~') {
            printf("\n¡AGUA!\n");
            t->mar[y][x] = 'O';
            sleep(2);
        } else {
            t->mar[y][x] = 'X';
            for (int i = 0; i < 5; i++) {
                _Bool impactoAcertado = false;
                
                if (flota[i].orientacion == 1) {
                    if ((int)x == flota[i].posicion_x && (int)y >= flota[i].posicion_y && (int)y < (flota[i].posicion_y + (int)flota[i].casillas)) {
                        impactoAcertado = true;
                    }
                } else {
                    if ((int)y == flota[i].posicion_y && (int)x >= flota[i].posicion_x && (int)x < (flota[i].posicion_x + (int)flota[i].casillas)) {
                        impactoAcertado = true;
                    }
                }

                if (impactoAcertado == true) {
                    flota[i].impactos++;
                    if (flota[i].impactos == flota[i].casillas) {
                        printf("\n¡HUNDIDO! Hundiste el barco de tamano %u.\n", flota[i].casillas);
                    } else {
                        printf("\n¡TOCADO!\n");
                    }
                    sleep(3);
                    break;
                }
            }
        }
    } else {
        // MODO ONLINE
        send(mi_socket, entrada, sizeof(entrada), 0);
        char respuesta[2];
        printf("Esperando confirmacion del enemigo...\n");
        recv(mi_socket, respuesta, sizeof(respuesta), 0);
        
        if (respuesta[0] == 'A') {
            printf("\n¡Radar: AGUA!\n");
        } else if (respuesta[0] == 'T') {
            printf("\n¡Radar: IMPACTO CONFIRMADO (Tocado)!\n");
        } else if (respuesta[0] == 'H') {
            printf("\n¡Radar: BARCO DESTRUIDO (Hundido)!\n");
        }
        sleep(3);
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

int hostearPartida(int puerto_inicial) {
    int socket_servidor, socket_cliente;
    struct sockaddr_in direccion_servidor, direccion_cliente;
    socklen_t tamano_cliente = sizeof(direccion_cliente);
    int puerto_actual = puerto_inicial;

    // 1. Se crea el socket TCP
    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_servidor == -1) {
        printf("Error: No se pudo crear el socket.\n");
        exit(1);
    }

    // --- EL SECRETO: Limpiar la memoria de la estructura ---
    memset(&direccion_servidor, 0, sizeof(direccion_servidor));

    // 2. Bucle para buscar un puerto disponible
    while (1) {
        direccion_servidor.sin_family = AF_INET;
        direccion_servidor.sin_addr.s_addr = INADDR_ANY;
        direccion_servidor.sin_port = htons(puerto_actual);

        if (bind(socket_servidor, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) == 0) {
            break; // Exito
        }
        
        puerto_actual++;
        if (puerto_actual > 65535) {
            printf("Error fatal: Sin puertos disponibles.\n");
            exit(1);
        }
    }

    listen(socket_servidor, 1);
    
    printf("\n=================================================\n");
    printf("  SALA CREADA CON EXITO\n");
    printf("  Puerto asignado: %d\n", puerto_actual);
    printf("  Esperando a que el Jugador 2 se conecte...\n");
    printf("=================================================\n");

    // Limpiamos también la del cliente por las dudas
    memset(&direccion_cliente, 0, sizeof(direccion_cliente));
    socket_cliente = accept(socket_servidor, (struct sockaddr *)&direccion_cliente, &tamano_cliente);
    
    if (socket_cliente < 0) {
        printf("Error: Fallo al aceptar la conexion.\n");
        exit(1);
    }

    printf("¡Jugador 2 conectado!\n");
    sleep(2);
    
    return socket_cliente;
}

int unirsePartida(char *ip_servidor, int puerto) {
    int socket_cliente;
    struct sockaddr_in direccion_servidor;

    socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_cliente == -1) {
        printf("Error: No se pudo crear el socket.\n");
        exit(1);
    }

    // --- EL SECRETO: Limpiar la memoria antes de conectarse ---
    memset(&direccion_servidor, 0, sizeof(direccion_servidor));

    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_port = htons(puerto);
    direccion_servidor.sin_addr.s_addr = inet_addr(ip_servidor); 

    limpiarTerminal(0,0);
    printf("Conectando con la base enemiga en IP %s y Puerto %d...\n", ip_servidor, puerto);

    if (connect(socket_cliente, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) < 0) {
        printf("Error: Fallo la conexion. Revisa que el Host este esperando.\n");
        exit(1);
    }

    printf("¡Conectado exitosamente a la partida!\n");
    sleep(2);
    
    return socket_cliente;
}

void mostrarIPLocal() {
    // se crea un socket UDP temporal
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        printf("No se pudo detectar la IP. Usa 'localhost' o 127.0.0.1 si estas en la misma PC.\n");
        return;
    }

    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("8.8.8.8"); // DNS de Google
    serv.sin_port = htons(53);

    // Simulamos una conexión para que el SO nos asigne nuestra IP de red local
    if (connect(sock, (const struct sockaddr*)&serv, sizeof(serv)) < 0) {
        printf("Sin conexion a red. Usa '127.0.0.1' para jugar en esta misma computadora.\n");
        close(sock);
        return;
    }

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    if (getsockname(sock, (struct sockaddr*)&name, &namelen) == 0) {
        printf("\n=================================================\n");
        printf("  TU IP LOCAL ES: %s\n", inet_ntoa(name.sin_addr));
        printf("  Dictar este numero al Jugador 2\n");
        printf("=================================================\n\n");
    }
    
    close(sock);
}

void recibirAtaque(tablero *t, barco *flota, int mi_socket) {
    char buffer[10];
    char respuesta[2] = "A"; 
    unsigned int x, y;

    printf("\n>>> ESPERANDO ATAQUE ENEMIGO...\n");
    
    // El programa se congela acá hasta que llegue el misil por la red
    recv(mi_socket, buffer, sizeof(buffer), 0);
    
    if (strcmp(buffer, "RINDIO") == 0) {
        limpiarTerminal(0,0);
        printf("\n¡VICTORIA! El enemigo se ha rendido.\n");
        sleep(4);
        exit(0);
    }
    
    traducirCoordenada(buffer, &x, &y);
    printf("\n¡Alarma! El enemigo disparó a la coordenada %s\n", buffer);
    sleep(2);
    
    // Evaluamos el daño en NUESTRO tablero
    if (t->mar[y][x] == VACIO) {
        printf("Reporte: ¡Cayo en el AGUA!\n");
        t->mar[y][x] = AGUA;
        respuesta[0] = 'A';
    } else if (t->mar[y][x] == AGUA || t->mar[y][x] == TOCADO) {
        printf("Reporte: Disparó donde ya había atacado.\n");
        respuesta[0] = 'A';
    } else {
        t->mar[y][x] = TOCADO;
        respuesta[0] = 'T'; // Por defecto tocado, ahora comprobamos si se hundió
        
        for (int i = 0; i < 5; i++) {
            _Bool impactoAcertado = false;
            if (flota[i].orientacion == 1) {
                if ((int)x == flota[i].posicion_x && (int)y >= flota[i].posicion_y && (int)y < (flota[i].posicion_y + (int)flota[i].casillas)) {
                    impactoAcertado = true;
                }
            } else {
                if ((int)y == flota[i].posicion_y && (int)x >= flota[i].posicion_x && (int)x < (flota[i].posicion_x + (int)flota[i].casillas)) {
                    impactoAcertado = true;
                }
            }

            if (impactoAcertado) {
                flota[i].impactos++;
                if (flota[i].impactos == flota[i].casillas) {
                    respuesta[0] = 'H';
                }
                break;
            }
        }
        
        if (respuesta[0] == 'T') printf("Reporte: ¡Nos han TOCADO un barco!\n");
        if (respuesta[0] == 'H') printf("Reporte: ¡ATENCIÓN! ¡Nos han HUNDIDO un barco!\n");
    }
    
    // Le devolvemos el resultado al radar del enemigo
    send(mi_socket, respuesta, sizeof(respuesta), 0);
    sleep(3);
}