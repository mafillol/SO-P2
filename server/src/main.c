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

// int main(int argc, char const *argv[])
// {
//   char** cards = get_random_cards(file_words);
//   free(cards[0]);
//   free(cards[1]);
//   free(cards);
//   return 0;
// }

int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  char * IP = "127.0.1.1";
  int PORT = 8070;
  Game* game = NULL;  

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
    else if (msg_code == 4){
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
      if(game->n_players>=1){
        int adversario = (my_attention + 1) % 2;
        char* response = p->name;

        //Avisamos que se encontro un contrincante
        server_send_message(sockets_array[adversario], 5, response);

        //Generamos las primeras cartas
        char** cards = get_random_cards(file_words);
        game->answer = cards[0];
        game->words = cards[1];

        //Avisamos a ambos jugadores que inicio la partida
        for(int i=0; i<2;i++){
          //Avisamos que inicio la partida
          sprintf(response, "%d", game->partida);
          server_send_message(sockets_array[i], 7, response);
          // Enviamos las primeras palabras          
          server_send_message(sockets_array[i], 9, game->words);
        }
        free(cards);
      }
      free(client_message);
    }

    /** El cliente manda una palabra respuesta*/
    else if(msg_code == 10){
      char * client_message = server_receive_payload(sockets_array[my_attention]);

      int adversario = (my_attention + 1) % 2;
      
      Player* p = game->players[my_attention];

      //Si le quedan intentos y aun no contesta
      if(p->aim < 2 && !p->answer){
        // Si la respuesta es correcta
        if(strcmp(client_message, game->answer) == 0){
          uint8_t response[2] = {1, p->aim};
          //Enviamos la respuesta y los intentos
          server_send_message(sockets_array[my_attention], 11, response);

          //Guardamos que el usuario ha respondido
          p->answer = true;
        }
        else{
          uint8_t response[2] = {0, p->aim};
          //Enviamos la respuesta y los intentos
          server_send_message(sockets_array[my_attention], 11, response);
          //Volvemos a enviar las palabras
          server_send_message(sockets_array[my_attention], 9, game->words);
          //Aumentamos los intentos
          p->aim++;
        }
      }
      else if(p->aim == 2 && !p->answer){
        p->answer = true;
      }
      free(client_message);
    }

    /** El cliente responde si quiere seguir jugando*/
    else if(msg_code == 16){
      char * client_message = server_receive_payload(sockets_array[my_attention]);
      free(client_message);
    }

    /** El cliente se desconecta*/
    else if(msg_code == 17){
      char * client_message = server_receive_payload(sockets_array[my_attention]);
      free(client_message);
    }

    printf("------------------\n");
    // Mi atención cambia al otro socket
    my_attention = (my_attention + 1) % 2;
  }

  return 0;
}
