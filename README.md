# Proyecto 2 - Sistema Operativo

## Integrantes
Macarena Fillol (mafillol@uc.cl) - 14636859

## Instrucciones

### Servidor
Para que el servidor funcione es necesario que el archivo palabras.txt y la carpeta assets se encuentren dentro de la carpeta servidor.
De la misma forma, dentro de la carpeta src deben encontrarse los siguientes archivos:
- *conection.c* contiene la función de creación de los socket de clientes.
- *communication.c* contiene las funciones de conexión entre el servidor y el cliente. 
- *player.c* contiene todas las funciones relativas a los jugadores y el juego.
- *paquete.c* contiene las funciones auxiliares para facilitar la creación de los Payloads, además de la función para escribir los logs.

### Cliente
Para el correcto funcionamiento del cliente, dentro de la carpeta src deben encontrarse los siguientes archivos:
- *conection.c* contiene la función de creación de los socket de clientes
- *communication.c* contiene las funciones de conexión entre el servidor y el cliente 
- *functions.c* contiene las funciones auxiliares para facilitar el despliegue en pantalla de las cartas, además de la función para escribir los logs.

### Inicio
Es necesario que el servidor se encuentre corriendo para poder conectar a los clientes.
Una vez que el servidor se encuentre corriendo, **ambos socket** deben estar conectados con sus respectivos clientes para comenzar la ejecución, independientemente de si se envía el paquete *Start Connection* o no. El servidor cuenta con un time out de 2 segundos, para que la respuesta del servidor pueda ser lo mas rápida e interactiva posible.


## Supuestos
Dado que el enunciado no especifica, se asumió que el archivo de las 500 palabras a leer por el servidor se encuentran dentro del archivo palabras.txt (siguiendo así el ejemplo de la ayudantía).
Cuando un usuario decide desconectarse, pierde inmediatamente la partida.
Al terminar una partida e iniciar una nueva, los puntajes de los clientes vuelven a 0.
Cuando ocurre una caída inesperada del servidor, se avisa al cliente por consola y el programa termina.
Cuando el cliente manda un paquete mal construido o con un id incorrecto, el servidor envía el error al cliente, luego envía a ambos clientes el paquete *Disconnect*, y finalmente termina el programa.
El "MessageType ID = 0" se encuentra protegido para determinar cuando el servidor ha sido desconectado de manera repentina.
Como no se especifica en el enunciado, se asumió que para el paquete *Game Winner/Loser*, el servidor envía el *ID* del cliente ganador o un 0 en el caso de empate (exactamente como se especifica que funciona *Round Winner/Loser*).
Si el servidor envía un paquete mal construido (es decir, que es construido de manera incorrecta por el servidor, **NO** *Error Bad Package*), entonces se avisa por consola al cliente, luego el cliente envía el paquete *Disconnect* al servidor y finalmente termina su ejecución.


## Consideraciones: Paquetes mal construidos
Para los paquetes que no necesitan mandar información en el payload (como por ejemplo, *Start Connection*), si el usuario envía información, entonces es considerado un paquete mal construido.
Si el usuario ingresa un nick vacío o la palabra respuesta es vacía, se considera como un paquete mal construido.

## Otras consideraciones
Tanto el cliente como el servidor no discriminan entre mayúsculas y minúsculas.