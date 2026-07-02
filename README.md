# Batallits_Navalits

====================================================================
TRABAJO PRÁCTICO: BATALLA NAVAL (C99/C11)
Materia: Electrónica Digital IV
Institución: Instituto Técnico Salesiano Villada
====================================================================

INTEGRANTES:
- Franco Ignacio Castillo
- [NOMBRE DE TU COMPAÑERO ACÁ]

====================================================================
1. INSTRUCCIONES DE COMPILACIÓN Y EJECUCIÓN
====================================================================
El código fuente consta de un archivo principal en C que maneja tanto 
la lógica offline como la conexión online mediante sockets TCP.

Comando de compilación (desde la terminal de Linux/WSL):
gcc main.c -o batallaNaval

Comando de ejecución:
./batallaNaval

Instrucciones para Modo Multijugador (Misma red / Localhost):
1. Abrir una terminal, ejecutar el juego y seleccionar la opción "2. Crear Partida (Host)".
2. El sistema indicará la IP local y asignará un puerto dinámico (ej: 8080).
3. Abrir una segunda terminal, ejecutar el juego y seleccionar "3. Unirse a Partida (Cliente)".
4. Ingresar la IP (usar 127.0.0.1 para pruebas en la misma PC) y el puerto dictado por el Host.

====================================================================
2. ESTRUCTURAS DE DATOS UTILIZADAS
====================================================================
Para cumplir con los requisitos de manejo de memoria y organización 
de la información, se diseñaron las siguientes estructuras:

A. struct tablero:
Contiene las dimensiones (filas y columnas) ingresadas por el usuario 
y un puntero doble (char **mar) que permite alojar la matriz del 
tablero de forma dinámica utilizando malloc() en tiempo de ejecución. 
Al finalizar la partida, la memoria se libera con free() para evitar 
memory leaks.

B. struct barco:
Representa a cada navío de la flota. Almacena:
- casillas: Tamaño total del barco.
- impactos: Cantidad de veces que fue acertado.
- orientacion: 1 para Vertical, 0 para Horizontal.
- posicion_x / posicion_y: Coordenadas de origen en el tablero.
- inicial: Letra representativa (P, A, C, D).

====================================================================
3. PROTOCOLO DE MENSAJES (MODO ONLINE)
====================================================================
Debido a las limitaciones detalladas en la sección de dificultades, 
el flujo de red implementado utiliza un protocolo simplificado basado 
en el envío y recepción de buffers de caracteres (cadenas) mediante 
las funciones send() y recv().

Flujo de la arquitectura Cliente-Servidor:
1. El Jugador A tipea una coordenada (Ej: "B4") o el comando "salir".
2. El sistema envía esa cadena literal a través del socket hacia B.
3. El proceso B recibe la cadena, la procesa mediante la función 
   traducirCoordenada(), impacta sobre su matriz dinámica e imprime 
   el resultado localmente.

====================================================================
4. DIFICULTADES ENCONTRADAS (JUSTIFICACIÓN TÉCNICA)
====================================================================
Durante el desarrollo del Punto 2 (Sockets TCP), nos enfrentamos a 
limitaciones severas de infraestructura de red relacionadas con el 
entorno de desarrollo (WSL2 - Windows Subsystem for Linux).

Aislamiento de red (WSL2 NAT):
Dado que WSL2 opera sobre una máquina virtual con su propia subred 
interna (IPs 172.31.x.x), el servidor en C levanta el puerto 
correctamente, pero Windows bloquea por defecto el tráfico entrante 
desde las direcciones del Wi-Fi físico (192.168.x.x). Conectar dos 
computadoras físicas requería realizar "Port Forwarding" en 
PowerShell y desactivar el Firewall de Windows, lo cual excede el 
alcance del lenguaje C. Por este motivo, la evaluación de red se 
realizó mediante interfaces de loopback (127.0.0.1).

I/O Multiplexado (select) y Serialización:
La complejidad de implementar un protocolo binario estricto (structs 
pasados a bytes) y la función select() para evitar los bloqueos de 
scanf() chocó con los tiempos límite de entrega y la inestabilidad 
de los descriptores de archivo en las pruebas de red. Se optó por 
garantizar la estabilidad de la lógica principal (validación de 
memoria y turnos) usando un protocolo secuencial por consola.