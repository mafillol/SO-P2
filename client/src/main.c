#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "conection.h"
#include "comunication.h"
#include "functions.h"

int ID;
char * IP;
int PORT;
bool write_file_log = false;

char * get_input(){
  char * response = calloc(40, sizeof(char));
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
  for(int i=0; i<argc; i++){
    if(strcmp(argv[i], "-l") == 0){
      write_file_log = true;
      FILE* f = fopen("log.txt", "w+");
      fclose(f);
    }
    if(strcmp(argv[i], "-i") == 0){
      IP = argv[i+1];
    }
    if(strcmp(argv[i], "-p") == 0){
      PORT = argv[i+1] - "0";
    }
  }

  IP = "127.0.1.1";
  PORT = 8070;

  // Se prepara el socket
  int server_socket = prepare_socket(IP, PORT);

  /** Se avisa al servidor que queremos conectarnos*/
  client_send_message(server_socket, 1 , "");

  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
  while (1){
    int msg_code = client_receive_id(server_socket);

    /** Establecemos conexion con el servidor*/
    if (msg_code == 2) { 
      char * message = client_receive_payload(server_socket);
      printf("Conexion establecida con servidor...\n");
      free(message);
    }

    /** Establecemos un nickname*/
    else if (msg_code == 3) { 
      char * message = client_receive_payload(server_socket);

      //Obtenemos en nick del cliente
      printf("Ingrese un nick para la partida:\t");
      char* name = get_input();

      //Enviamos el nombre al servidor
      client_send_message(server_socket, 4, name);

      free(message);
    }
    /** Se encontro oponente para la partida*/
    else if (msg_code == 5) { 
      char * message = client_receive_payload(server_socket);

      printf("Partida iniciada con contrincante: %s\n", message);

      free(message);
    }

    /**Servidor envia ID*/
    else if(msg_code == 6){
      char * message = client_receive_payload(server_socket);

      printf("Tu id personal es: %d\n", (int)message[0]);

      ID = message[0];
      free(message);
    }

    /** Partida iniciada correctamente*/
    else if(msg_code == 7){
      char * message = client_receive_payload(server_socket);

      printf("Partida %d iniciada.\n", (int)message[0]);

      free(message);
    }

    /** Se desplegan los puntajes de los jugadores*/
    else if(msg_code == 8){
      char * message = client_receive_payload(server_socket);

      printf("Tu puntaje es: %d\tEl puntaje de tu contrincante es: %d\n", (int)message[0],(int)message[1]);
      
      free(message);
    }

    /** Se envian las palabras*/
    else if(msg_code == 9){
      char * message = client_receive_payload(server_socket);

      // Imprimimos las cartas
      print_cards(message);

      // Pedimos respuesta del cliente
      printf("\nIngrese la palabra repetida o Disconnect para salir:\t");
      char* response = get_input();
      printf("Lo que sale de consola = %s\n", response);

      char* upper = uppercase(response);

      //Si se desconecta
      if(strcmp(upper, "DISCONNECT") == 0){
        client_send_message(server_socket, 17, "");
      }
      // Enviamos respuesta al servidor
      else{
        client_send_message(server_socket, 10, response);
      }

      //Liberamos la memoria
      free(upper);
      free(message);
    }

    /** Resultado de respuesta*/
    else if(msg_code == 11){
      char * message = client_receive_payload(server_socket);
      int response = message[0];
      int aim = message[1];
      int try;
      switch(aim){
        case 2:
          try = 1;
          break;
        case 1:
          try = 2;
          break;
        case 0:
          try = 3;
          break;
      }

      //Respuesta incorrecta
      if(response == 0){
        printf("Respuesta incorrecta, tienes %d intentos restantes\n", aim);
      }
      //Respuesta correcta
      else if(response == 1){
        printf("Respuesta correcta en %d intentos.\n",try);
      }
      free(message);
    }

    /** Ronda terminada*/
    else if(msg_code == 12){
      char * message = client_receive_payload(server_socket);
      int response = message[0];

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

    /** Partida terminada*/
    else if(msg_code == 13){
      char * message = client_receive_payload(server_socket);
      printf("Partida %d terminada.\n", message[0]);
      free(message);
    }

    //Ganador de la partida
    else if(msg_code == 14){
      char * message = client_receive_payload(server_socket);
      int response = message[0];

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

    /** Pregunta por un nuevo juego*/
    else if(msg_code == 15){
      char * message = client_receive_payload(server_socket);
      bool answer = false;
      while(!answer){
        printf("¿Qué desea hacer?\n   1)Iniciar nueva partida\n   2)Terminar de jugar\n");
        int option = getchar() - '0';
        getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin

        char response[1];
        //Iniciamos nueva partida
        if(option == 1){
          response[0] = 1;
          client_send_message(server_socket,16, response);
          answer = true;
        }
        //Terminamos de jugar
        else if(option == 2){
          response[0] = 0;
          client_send_message(server_socket,16, response);
          answer = true;
        }
      }
      free(message);
    }
    
    /** Servidor envía desconexion */ 
    else if(msg_code == 17){
      char * message = client_receive_payload(server_socket);
      printf("Desconectando del servidor...\n");
      ID = 0;
      free(message);
      break; 
    }

    /** Servidor manda paquete mal contruido o desconocido*/
    else if(msg_code == 20){
      char * message = client_receive_payload(server_socket);
      printf("ERROR: ID desconocido o paquete mal contruido\n");
      free(message);
    }

    /** Error inesperado del servidor*/
    else if(msg_code == 0){
      char * message = client_receive_payload(server_socket);
      printf("Ha ocurrido un error inesperado con el servidor.\n");
      printf("Terminando programa....\n");
      free(message);
      break;
    }

    printf("------------------\n");
  }

  // Se cierra el socket
  close(server_socket);
  //free(IP);

  return 0;
}
