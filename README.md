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
Cuando el cliente manda un paquete mal construido o con un id incorrecto, el servidor envia el error al cliente, luego envia a ambos clientes una desconexion, y finalmente termina el programa.


## Consideraciones: Paquetes mal construidos
Para los paquetes que no necesitan mandar informacion en el payload (como por ejemplo, *Start Connection*), si el usuario envia informacion, entonces es considerado un paquete mal construido.
Si el usuario ingresa un nick vacio o la palabra respuesta es vacia, se considera como un paquete mal construido.

## Consideraciones: logs
Para el tamaño del payload y la informacion escrita en el archivo log.txt se considera que el cliente envia un '\0' al final del payload. En otras palabras, cuando el cliente envia un payload vacio, este va a tener un tamaño de 1 byte y como valor '\0':