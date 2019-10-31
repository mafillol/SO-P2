# Proyecto 2 - Sistema Operativo

## Integrantes
Macarena Fillol (mafillol@uc.cl) - 14636859

## Instrucciones

### Servidor
Para que el servidor funcione es necesario que el archivo palabras.txt y la carpeta assets se encuentren dentro de la carpeta servidor.
De la misma forma, dentro de la carpeta src deben encontrarse los siguientes archivos:
- *conection.c* contiene la funcion de creacion de los socket de clientes.
- *communication.c* contiene las funciones de conexion entre el servidor y el cliente. 
- *player.c* contiene todas las funciones relativas a los jugadores y el juego.
- *paquete.c* contiene las funciones auxiliares para facilitar la creacion de los Payloads, ademas de la funcion para escribir los logs.

### Cliente
Para el correcto funcionamiento del cliete, dentro de la carpeta src deben encontrarse los siguientes archivos:
- *conection.c* contiene la funcion de creacion de los socket de clientes
- *communication.c* contiene las funciones de conexion entre el servidor y el cliente 
- *functions.c* contiene las funciones auxiliares para facilitar el despliegue en pantalla de las cartas, ademas de la funcion para escribir los logs.

### Inicio
Es necesario que el servidor se encuentre corriendo para poder conectar a los clientes.
Una vez que el servidor se encuentre corriendo, los clientes tienen un lapso de 5 segundos para poder conectarse a traves del socket al servidor. Pasado ese tiempo, la conexion ya no se acepta. La razon de esto es que se ha decidido agregar un timeout para que, una vez los jugadores inicien la partida, la respuesta del servidor pueda ser lo mas rapida e interactiva posible.


## Supuestos
Dado que el enunciado no especifica, se asumio que el archivo de las 500 palabras a leer por el servidor se encuentran dentro del archivo palabras.txt (siguiendo asi el ejemplo de la ayudantia).
Cuando un usuario decide desconectarse, pierde inmediatamente la partida.
Al terminar una partida e iniciar una nueva, los puntajes de los clientes vuelven a 0.
Cuando ocurre una caida inesperada del servidor, se avisa al cliente por consola y el programa termina.