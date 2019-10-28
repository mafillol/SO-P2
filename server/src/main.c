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
      // Mi atención cambia al otro socket
      my_attention = (my_attention + 1) % 2;
    }

    /** El cliente establece un nickname*/
    else if (msg_code == 4){
      char * client_message = server_receive_payload(sockets_array[my_attention]);
      ID ++;
      Player* p = init_player(client_message, ID);
      if(!game){
        game = init_game();
      }

      game->players[my_attention] = p;
      game->n_players++;

      char id[1] = {p->id};

      //Enviamos el id del jugador
      server_send_message(sockets_array[my_attention], 6, id);

      //Si se encuentra un contrincante
      if(game->n_players>=2){

        //Generamos las primeras cartas
        new_words(game, file_words);

        //Avisamos a ambos jugadores que inicio la partida
        for(int i=0; i<2;i++){
          int adversario = (i + 1) % 2;     

          //Avisamos que se encontro un contrincante
          server_send_message(sockets_array[i], 5, game->players[adversario]->name);

          char partida[1] = {game->partida};
          //Avisamos que inicio la partida
          server_send_message(sockets_array[i], 7, partida);

          // Enviamos los puntajes 
          char points[2] = {p->points, game->players[adversario]->points}; 
          server_send_message(sockets_array[i], 8, points);

          // Enviamos las primeras palabras          
          server_send_message(sockets_array[i], 9, game->words);
        }
      }
      free(client_message);
      // Mi atención cambia al otro socket
      my_attention = (my_attention + 1) % 2;

    }

    /** El cliente manda una palabra respuesta*/
    else if(msg_code == 10){
      char * client_message = server_receive_payload(sockets_array[my_attention]);

      int adversario = (my_attention + 1) % 2;
      
      Player* p = game->players[my_attention];
      //Disminuimos los intentos restantes
      p->aim --;

      //Si le quedan intentos y aun no contesta
      if(p->aim > 0 && !p->answer){
        //Convertimos la respuesta a mayusculas
        uppercase(client_message);

        // Si la respuesta es correcta
        if(strcmp(client_message, game->answer) == 0){
          char response[2] = {1, p->aim};

          //Enviamos la respuesta y los intentos
          server_send_message(sockets_array[my_attention], 11, response);

          //Guardamos que el usuario ha respondido
          p->answer = true;

          //Guardamos el puntaje
          switch(p->aim){
            case 2:
              p->round_point = 5;
              p->points = p->points + 5;
              break;
            case 1:
              p->round_point = 3;
              p->points = p->points + 3;
              break;
            case 0:
              p->round_point = 1;
              p->points = p->points + 1;
              break;
          }
        }
        //Si la respuesta es incorrecta
        else{
          char response[2] = {0, p->aim};

          //Enviamos la respuesta y los intentos
          server_send_message(sockets_array[my_attention], 11, response);
          //Volvemos a enviar las palabras
          server_send_message(sockets_array[my_attention], 9, game->words);
        }
      }

      //Respuesta equivocada en todos los intentos
      else if(p->aim == 0 && !p->answer){
        p->answer = true;
      }

      //Si ambos jugadores contestaron, la ronda termina
      if(p->answer && game->players[adversario]->answer){

        char round_winner[1];
        //El ganador es el jugador actual
        if (p->round_point > game->players[adversario]->round_point){
          round_winner[0] = p->id;
        }

        //El ganador es el adversario
        else if (p->round_point < game->players[adversario]->round_point){
          round_winner[0] = game->players[adversario]->id;
        }
        //Hay empate entre los jugadores
        else if (p->round_point < game->players[adversario]->round_point){
          round_winner[0] = 0;
        }
        //Avisamos a ambos jugadores del ganador
        for(int i=0; i<2; i++){
          server_send_message(sockets_array[i], 12, round_winner);
        }

        //Avanzamos en la ronda
        game->round++;

        //Se termina la partida
        if(game->round == 5){

          char partida[1] = {game->partida};
          //Avisamos el fin de la partida
          for (int i = 0; i < 2; ++i)
          {
            server_send_message(sockets_array[i], 13, partida);
          }
          char winner[1];
          //El ganador de la partida es el jugador actual
          if(game->players[0]->points > game->players[1]->points){
            winner[0] = game->players[0]->id;
          }
          //El ganador de la partida es el adversario
          else if(game->players[0]->points < game->players[1]->points){
            winner[0] = game->players[1]->id;
          }
          //Hay un empate
          if(game->players[0]->points == game->players[1]->points){
            winner[0] = 0;
          }

          for (int i = 0; i < 2; ++i)
          {
            // Avisamos el ganador de la partida     
            server_send_message(sockets_array[i], 14, winner);
            
            //Preguntamos por un juego nuevo
            server_send_message(sockets_array[i], 15, "");
          }

        }

        //Avanza a la siguiente ronda
        else{

          //Generamos un nuevo set de palabras
          new_words(game, file_words);

          for (int i = 0; i < 2; ++i)
          {

            Player* p = game->players[i];
            p->answer = false; //Los jugadores no han respondido
            p->round_point = 0; //Limpiamos el puntaje de la ronda

            // Enviamos los puntajes 
            char points[2] = {p->points, game->players[(i + 1) %2]->points}; 
            server_send_message(sockets_array[i], 8, points);

            // Enviamos las nuevas palabras de la ronda        
            server_send_message(sockets_array[i], 9, game->words);
          }
        }
      }
      free(client_message);

      // Mi atención cambia al otro socket
      my_attention = (my_attention + 1) % 2;

    }


    /** El cliente responde si quiere seguir jugando*/
    else if(msg_code == 16){
      char * client_message = server_receive_payload(sockets_array[my_attention]);

      int response = client_message[0];

      //Si no quiere seguir jugando
      if(response == 0){
        for (int i = 0; i < 2; ++i)
        {
          //Avisamos de la desconexion        
          server_send_message(sockets_array[i], 17, "");
        }
        destroy_game(game);
      }

      Player* p = game->players[my_attention];
      //Guardamos la respuesta del usuario
      p->new_game=response;

      //Si el otro cliente tambien quiere seguir jugando
      if(game->players[(my_attention + 1) % 2]->new_game == YES){
        //Avanzamos la partida
        game->partida++;
        game->round = 1;

        //Generamos un nuevo set de palabras
        new_words(game, file_words);

        for(int i=0; i<2 ; i++){
          char partida[1] = {game->partida};
          //Avisamos que inicio la partida
          server_send_message(sockets_array[i], 7, partida);

          // Enviamos los puntajes 
          char points[2] = {p->points, game->players[(i + 1) % 2]->points}; 
          server_send_message(sockets_array[i], 8, points);

          // Enviamos las primeras palabras          
          server_send_message(sockets_array[i], 9, game->words);
        }
        //Restauramos las respuestas delos clientes para nuevo juego
        p->new_game = NOTANSWER;
        game->players[(my_attention + 1) % 2]->new_game = NOTANSWER;
      }
      free(client_message);
      // Mi atención cambia al otro socket
      my_attention = (my_attention + 1) % 2;

    }

    /** El cliente se desconecta*/
    else if(msg_code == 17){
      char * client_message = server_receive_payload(sockets_array[my_attention]);

      int adversario = (my_attention + 1) % 2;

      //El ganador es el jugador que no se desconecta
      char winner[1] = {game->players[adversario]->id};

      for (int i = 0; i < 2; ++i)
      {
        //Avisamos al ganador de la partida
        server_send_message(sockets_array[i], 14, winner);
        //Avisamos de la desconexion        
        server_send_message(sockets_array[i], 17, "");
      }

      //Eliminamos el juego
      destroy_game(game);

      free(client_message);

      // Mi atención cambia al otro socket
      my_attention = (my_attention + 1) % 2;

    }

    /** Paquete incorrecto */
    else if(msg_code != 0){
      char * client_message = server_receive_payload(sockets_array[my_attention]);

      //Avisamos al cliente que el paquete es incorrecto
      server_send_message(sockets_array[my_attention], 20, "");

      free(client_message);

      // Mi atención cambia al otro socket
      my_attention = (my_attention + 1) % 2;
    }

    else{
      // Mi atención cambia al otro socket
      my_attention = (my_attention + 1) % 2;
    }

    printf("------------------\n");
  }

  return 0;
}
