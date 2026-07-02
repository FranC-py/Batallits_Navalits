# Batallits_Navalits
gcc Main/main.c Mapa/Mapa.c Barcos/Barcos.c Juego/Juego.c Online/Online.c -o batalla_naval
./batalla_naval

explicacion de porque es se compila asi:
 gcc: es el compilador, main.c: es el archivo a compilar, -o: bandera -o(de output) le dice al compilador que cree un archivo que me de una salida y ademas se llame batllaNaval
./: le dice al compilador que el archivo a ejecuctar esta en esta carpeta y batallaNaval es el nombre del archivo
IMPORTANTE SE DEBE DE EJECUTAR //gcc main.c -o batallaNaval CADA VEZ QUE SE HACE UN CAMBIO PARA "ACUTALIZAR EL PROGRAMA"



MAIN: Punto de entrada y coordinador central del programa. Gestiona el menú principal, establece el modo de juego (offline y online) y mantiene vivo el bucle de la partida. Se encarga de alternar los turnos, procesar las entradas del usuario y coordinar al resto de los módulos hasta finalizar el juego y ejecutar la limpieza de recursos.

MAPA: Administra la memoria dinámica y la representación gráfica del campo de batalla. Utiliza punteros dobles y malloc() para crear matrices de tamaño personalizado según la elección de los jugadores, dibuja la interfaz visual en la consola y libera la memoria con free() al terminar para evitar fugas (memory leaks).

BARCOS: Define la estructura lógica y el posicionamiento de la flota en el tablero. Utiliza un struct para encapsular las propiedades de cada navío (tamaño, coordenadas, impactos) y emplea un algoritmo de generación aleatoria que distribuye las naves validando los límites de la matriz y evitando superposiciones.

JUEGO: Contiene el motor lógico y las reglas de la partida. Escanea matemáticamente las coordenadas de cada disparo enemigo contra los structs de los barcos para dictaminar el resultado exacto (AGUA, TOCADO o HUNDIDO), además de llevar el registro del daño global para determinar en qué momento un jugador pierde toda su flota.

ONLINE: Implementa el modo multijugador mediante una arquitectura Cliente-Servidor local o LAN. Utiliza Sockets TCP de la librería de UNIX para conectar dos terminales, garantizando la sincronización inicial de las dimensiones del mapa y el intercambio seguro y en tiempo real de los disparos y resultados a través de la red.