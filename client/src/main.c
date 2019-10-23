#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "conection.h"
#include "comunication.h"

int ID;

char * get_input(){
  char * response = malloc(20);
  int pos=0;
  while (1){
    char c = getchar();
    if (c == '\n') break;
    response[pos] = c;
    pos++;
  }
  response[pos] = '\0';
  return response;
}


int main (int argc, char *argv[]){
  //Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y puerto de este cliente da igual)
  char * IP = "127.0.1.1";
  int PORT = 8070;

  // Se prepara el socket
  int server_socket = prepare_socket(IP, PORT);
  bool start = false;
  bool end = false;

  client_send_message(server_socket,1, "");
  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
  while (1){
    
    int msg_code = client_receive_id(server_socket);

    //Establecemos coneccion con el servidor
    if (msg_code == 2) { 
      printf("Connexion establecida con servidor...\n");
    }

    //Establecemos un nickname
    if (msg_code == 3) { 
      printf("Ingrese un nick para la partida:\t");
      char* name = get_input();
      client_send_message(server_socket, 4, name);
    }
    //Se encontro oponente para la partida
    if (msg_code == 5) { 
    char * message = client_receive_payload(server_socket);
    printf("Partida iniciada con contrincante: %s\n", message);
    free(message);
    }

    //Servidor envia ID
    if(msg_code == 6){
      char * message = client_receive_payload(server_socket);
      printf("Tu id personal es: %s\n", message);
      ID = (int) strtol(message, (char **)NULL, 10);
      free(message);
    }
    //Partida iniciada correctamente
    if(msg_code == 7){
      char * message = client_receive_payload(server_socket);
      printf("Partida %s iniciada.\n", message);
      free(message);
    }

    //Se desplegan los puntajes de los jugadores
    if(msg_code == 8){
      char * message = client_receive_payload(server_socket);
      printf("Tu puntaje es: %c\tEl puntaje de tu contrincante es: %c\n", message[0],message[1]);
      free(message);
    }

    //Se envían las cartas
    if(msg_code == 9){
      // Funcion para imprimir cartas
    }

    //Resultado de respuesta
    if(msg_code == 11){
      char * message = client_receive_payload(server_socket);
      int response = message[0] - '0';
      int aim = message[1] - '0';

      //Respuesta incorrecta
      if(response == 0){
        printf("Respuesta incorrecta, tienes %d intentos restantes\n", 2-aim);
      }
      //Respuesta correcta
      else if(response == 1){
        printf("Respuesta correcta en %d intentos.\n",aim+1);
      }
      free(message);
    }

    //Ronda terminada
    if(msg_code == 12){
      char * message = client_receive_payload(server_socket);
      int response = (int) strtol(message, (char **)NULL, 10);

      //Ronda empatada
      if(response == 0){
        printf("La ronda termino en un empate.\n");
      }
      //Ronda ganada
      else if(response == ID){
        printf("Felicidades, has ganado la ronda.\n");
      }
      //Ronda perdida
      else if(response != ID){
        printf("Lo lamentamos, has perdido la ronda.\n");
      }
      free(message);

    }

    //Partida terminada
    if(msg_code == 13){
      char * message = client_receive_payload(server_socket);
      printf("Partida %s terminada.\n", message);
      free(message);
    }

    //Ganador de la partida
    if(msg_code == 14){
      char * message = client_receive_payload(server_socket);
      int response = (int) strtol(message, (char **)NULL, 10);
      //Partida ganada
      if(response == ID){
        printf("Felicidades!! Has ganado la partida.\n");
      }
      //Partida perdida
      else{
        printf("Lo sentimos, has perdido la partida\n");
      }
      free(message);
    }

    //Pregunta por un nuevo juego
    if(msg_code == 15){
      char * message = client_receive_payload(server_socket);
      bool answer = false;
      while(!answer){
        printf("¿Qué desea hacer?\n   1)Iniciar nueva partida\n   2)Terminar de jugar\n");
        int option = getchar() - '0';
        getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin

        //Iniciamos nueva partida
        if(option == 1){
          client_send_message(server_socket,16, "1");
          answer = true;
        }
        //Nos desconectamos
        else if(option == 2){
          client_send_message(server_socket,16, "0");
          client_send_message(server_socket,17, "");
          answer = true;
        }
    }
    
    //Nos desconectan 
    if(msg_code == 17){
      printf("Desconectando del servidor...\n");
      ID = 0;    
    }

    if(msg_code == 20){
      printf("ERROR: ID desconocido o paquete mal contruido\n");
    }

    printf("------------------\n");
    }
  }

  // Se cierra el socket
  close(server_socket);
  free(IP);

  return 0;
}
