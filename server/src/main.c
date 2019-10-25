#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "comunication.h"
#include "conection.h"
#include "paquete.h"
#include "player.h"

uint8_t ID = 0;
char* file_words = "palabras.txt";

char * revert(char * message){
  int len = strlen(message) + 1;
  char * response = malloc(len);
  
  for (int i = 0; i < len-1; i++)
  {
    response[i] = message[len-2-i];
  }
  response[len-1] = '\0';
  return response;
}

int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  char * IP = "127.0.1.1";
  int PORT = 8070;
  Game* game = NULL;

  char** cards = get_random_cards(file_words);

  for(int i=0; i<19;i++){
    printf("%s\n", cards[i]);
  }


  // Se crea el servidor y se obtienen los sockets de ambos clientes.
  PlayersInfo * players_info = prepare_sockets_and_get_clients(IP, PORT);

  // Guardaremos los sockets en un arreglo e iremos alternando a quién escuchar.
  int sockets_array[2] = {players_info->socket_c1, players_info->socket_c2};
  int my_attention = 0;
  while (1)
  {
    // Se obtiene el paquete de uno de los clientes
    int msg_code = server_receive_id(sockets_array[my_attention]);
    printf("Mensaje : %d\n", msg_code);
    
     /** El cliente desea inicar una partida*/
    if (msg_code == 1)
    { 
      char * client_message = server_receive_payload(sockets_array[my_attention]);

      //Avisamos que se establecio la conexion
      server_send_message(sockets_array[my_attention], 2, "");
      //Preguntamos por el nickname
      server_send_message(sockets_array[my_attention], 3, "");

      free(client_message);
    }

    /** El cliente establece un nickname*/
    if (msg_code == 4){
      char * client_message = server_receive_payload(sockets_array[my_attention]);
      ID ++;
      Player* p = init_player(client_message, ID);
      if(!game){
        game = init_game();
      }
      game->players[game->n_players] = p;
      game->n_players++;

      char response[1];
      sprintf(response, "%d", p->id);

      //Enviamos el id del jugador
      server_send_message(sockets_array[my_attention], 6, response);

      //Si se encuentra un contrincante
      if(game->n_players>=2){
        int adversario = (my_attention + 1) % 2;
        char* response = p->name;

        //Avisamos que se encontro un contrincante
        server_send_message(sockets_array[adversario], 5, response);

        //Avisamos a ambos jugadores que inicio la partida
        for(int i=0; i<2;i++){
          //Avisamos que inicio la partida
          sprintf(response, "%d", game->partida);
          server_send_message(sockets_array[i], 7, response);
        }
      }
      free(client_message);
    }

    /** El cliente manda una palabra respuesta*/
    if(msg_code == 10){
      char * client_message = server_receive_payload(sockets_array[my_attention]);
      //Si ya no tiene intentos
      Player* p = game->players[my_attention];
      if(p->aim < 3){
        printf("INCOMPLETO :(\n");
      }
      free(client_message);
    }
    printf("------------------\n");
    // Mi atención cambia al otro socket
    my_attention = (my_attention + 1) % 2;
  }

  return 0;
}
