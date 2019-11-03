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

char* file_words = "palabras.txt";
bool write_file_log = false;

extern int size_payload;

char * IP;
int PORT;


int main(int argc, char *argv[]){

  // Se define una IP, un puerto y logs
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
      PORT = atoi(argv[i+1]);
    }
  }

  //Juego entre clientes
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

    if(msg_code == 0){
      my_attention = (my_attention + 1) % 2;
      continue;
    }

    printf("Mensaje : %d\n", msg_code);
    
     /** El cliente desea inicar una partida*/
    if (msg_code == 1)
    { 

      char * client_message = server_receive_payload(sockets_array[my_attention]);

      //Escribimos en el log file
      if(write_file_log){
        write_log(msg_code, client_message, 0, size_payload);
      }

      //Paquete mal construido
      if(size_payload != 0){
        //Avisamos que el paquete esta mal construido
        server_send_message(sockets_array[my_attention], 20, "");
        //Escribimos en el log file
        if(write_file_log){
          write_log(20, "", my_attention, 0);
        }
        //Desconectamos a los clientes
        for (int i = 0; i < 2; ++i)
        {
          //Avisamos de la desconexion        
          server_send_message(sockets_array[i], 17, "");
          //Escribimos en el log file
          if(write_file_log){
            write_log(17, "", i, 0);
          }
        }
        free(client_message);
        break; //Terminamos la ejecucion
      }

      //Avisamos que se establecio la conexion
      server_send_message(sockets_array[my_attention], 2, "");
      //Escribimos en el log file
      if(write_file_log){
        write_log(2, "", my_attention, 0);
      }
      //Preguntamos por el nickname
      server_send_message(sockets_array[my_attention], 3, "");
       //Escribimos en el log file
      if(write_file_log){
        write_log(3, "", my_attention, 0);
      }

      free(client_message);
    }

    /** El cliente establece un nickname*/
    else if (msg_code == 4){
      char * client_message = server_receive_payload(sockets_array[my_attention]);
      //Escribimos en el log file
      if(write_file_log){
        write_log(4, client_message, my_attention, size_payload);
      }

      //Paquete mal construido
      if(size_payload == 0){
        //Avisamos que el paquete esta mal construido
        server_send_message(sockets_array[my_attention], 20, "");
        //Escribimos en el log file
        if(write_file_log){
          write_log(20, "", my_attention, 0);
        }
        //Desconectamos a los clientes
        for (int i = 0; i < 2; ++i)
        {
          //Avisamos de la desconexion        
          server_send_message(sockets_array[i], 17, "");
          //Escribimos en el log file
          if(write_file_log){
            write_log(17, "", i, 0);
          }
        }
        free(client_message);
        break; //Terminamos la ejecucion
      }
      Player* p = init_player(client_message, my_attention + 1);

      if(!game){
         game = init_game();
      }
      game->players[my_attention] = p;
      game->n_players++;


      //Si se encuentra un contrincante
      if(game->n_players>=2){

        //Generamos las primeras cartas
        new_words(game, file_words);

        //Avisamos a ambos jugadores que inicio la partida
        for(int i=0; i<2;i++){
          int adversario = (i + 1) % 2;     

          //Avisamos que se encontro un contrincante
          server_send_message(sockets_array[i], 5, game->players[adversario]->name);
          //Escribimos en el log file
          if(write_file_log){
            write_log(5, game->players[adversario]->name, i, 0);
          }

          char id[2] = {game->players[i]->id, '\0'};

          //Enviamos el id del jugador
          server_send_message(sockets_array[i], 6, id);
          //Escribimos en el log file
          if(write_file_log){
            write_log(6, id, i, 0);
          }


          char partida[2] = {game->partida, '\0'};
          //Avisamos que inicio la partida
          server_send_message(sockets_array[i], 7, partida);
           //Escribimos en el log file
          if(write_file_log){
            write_log(7, partida, i, 0);
          }

          // Enviamos los puntajes 
          char points[3] = {p->points, game->players[adversario]->points, '\0'};

          server_send_message(sockets_array[i], 8, points);
          //Escribimos en el log file
          if(write_file_log){
            write_log(8, points, i, 0);
          }

          // Enviamos las primeras palabras          
          server_send_message(sockets_array[i], 9, game->words);
          //Escribimos en el log file
          if(write_file_log){
            write_log(9, game->words, i, 0);
          }
        }
      }
      free(client_message);
    }

    /** El cliente manda una palabra respuesta*/
    else if(msg_code == 10){
      char * client_message = server_receive_payload(sockets_array[my_attention]);

      //Escribimos en el log file
      if(write_file_log){
        write_log(msg_code, client_message, my_attention, size_payload);
      }

      //Paquete mal construido
      if(size_payload == 0){
        //Avisamos que el paquete esta mal construido
        server_send_message(sockets_array[my_attention], 20, "");
        //Escribimos en el log file
        if(write_file_log){
          write_log(20, "", my_attention, 0);
        }
        //Desconectamos a los clientes
        for (int i = 0; i < 2; ++i)
        {
          //Avisamos de la desconexion        
          server_send_message(sockets_array[i], 17, "");
          //Escribimos en el log file
          if(write_file_log){
            write_log(17, "", i, 0);
          }
        }
        free(client_message);
        break; //Terminamos la ejecucion
      }

      int adversario = (my_attention + 1) % 2;
      
      Player* p = game->players[my_attention];

      //Si le quedan intentos y aun no contesta
      if(p->aim > 0 && !p->answer){
        //Disminuimos los intentos restantes
        p->aim --;
        //Convertimos la respuesta a mayusculas
        uppercase(client_message);

        // Si la respuesta es correcta
        if(strcmp(client_message, game->answer) == 0){
          char response[3] = {1, p->aim, '\0'};

          //Enviamos la respuesta y los intentos
          server_send_message(sockets_array[my_attention], 11, response);
          //Escribimos en el log file
          if(write_file_log){
            write_log(11, response, my_attention, 0);
          }

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
          char response[3] = {0, p->aim, '\0'};

          //Enviamos la respuesta y los intentos
          server_send_message(sockets_array[my_attention], 11, response);

          //Escribimos en el log file
          if(write_file_log){
            write_log(11, response, my_attention, 0);
          }

          //Si ya no me quedan intentos
          if(p->aim == 0){
            p->answer = true;
          }
          //Si todavia quedan intentos
          else{
            //Volvemos a enviar las palabras
            server_send_message(sockets_array[my_attention], 9, game->words);
            //Escribimos en el log file
            if(write_file_log){
              write_log(9, game->words, my_attention, 0);
            }
          }

        }
      }

      //Si ambos jugadores contestaron, la ronda termina
      if(p->answer && game->players[adversario]->answer){

        char round_winner[2];
        round_winner[1] = '\0';
        //El ganador es el jugador actual
        if (p->round_point > game->players[adversario]->round_point){
          round_winner[0] = p->id;
        }

        //El ganador es el adversario
        else if (p->round_point < game->players[adversario]->round_point){
          round_winner[0] = game->players[adversario]->id;
        }
        //Hay empate entre los jugadores
        else if (p->round_point == game->players[adversario]->round_point){
          round_winner[0] = 0;
        }
        //Avisamos a ambos jugadores del ganador
        for(int i=0; i<2; i++){
          server_send_message(sockets_array[i], 12, round_winner);
          //Escribimos en el log file
          if(write_file_log){
            write_log(12, round_winner, i, 0);
          }
        }

        //Avanzamos en la ronda
        game->round++;

        //Se termina la partida
        if(game->round > 5){

          char partida[2] = {game->partida, '\0'};

          //Avisamos el fin de la partida
          for (int i = 0; i < 2; ++i)
          {
            server_send_message(sockets_array[i], 13, partida);
            //Escribimos en el log file
            if(write_file_log){
              write_log(13, partida, i, 0);
            }
          }

          char winner[2];
          winner[1] = '\0';
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
            Player* p = game->players[i];
            p->answer = false; //Los jugadores no han respondido
            p->round_point = 0; //Limpiamos el puntaje de la ronda
            p->aim = 3;  //Restauramos los intentos
            p->points = 0; //Limpiamos el puntaje de la partida

            // Avisamos el ganador de la partida     
            server_send_message(sockets_array[i], 14, winner);

            //Escribimos en el log file
            if(write_file_log){
              write_log(14, winner, i,0 );
            }
            
            //Preguntamos por un juego nuevo
            server_send_message(sockets_array[i], 15, "");
            //Escribimos en el log file
            if(write_file_log){
              write_log(15, "", i, 0);
            }
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
            p->aim = 3;  //Restauramos los intentos

            // Enviamos los puntajes
            char points[3] = {p->points, game->players[(i + 1) %2]->points, '\0'};

            server_send_message(sockets_array[i], 8, points);
            //Escribimos en el log file
            if(write_file_log){
              write_log(8, points, i, 0);
            }

            // Enviamos las nuevas palabras de la ronda        
            server_send_message(sockets_array[i], 9, game->words);
            //Escribimos en el log file
            if(write_file_log){
              write_log(9, game->words, i, 4);
            }
          }
        }
      }
      free(client_message);
    }


    /** El cliente responde si quiere seguir jugando*/
    else if(msg_code == 16){
      char * client_message = server_receive_payload(sockets_array[my_attention]);
      //Escribimos en el log file
      if(write_file_log){
        write_log(msg_code, client_message, my_attention, size_payload);
      }
      //Paquete mal construido
      if(size_payload != 1){
        //Avisamos que el paquete esta mal construido
        server_send_message(sockets_array[my_attention], 20, "");
        //Escribimos en el log file
        if(write_file_log){
          write_log(20, "", my_attention, 0);
        }
        //Desconectamos a los clientes
        for (int i = 0; i < 2; ++i)
        {
          //Avisamos de la desconexion        
          server_send_message(sockets_array[i], 17, "");
          //Escribimos en el log file
          if(write_file_log){
            write_log(17, "", i, 0);
          }
        }
        free(client_message);
        break; //Terminamos la ejecucion
      }
      int response = client_message[0];

      //Paquete mal contruido
      if(response!=0 && response != 1){
        // Avisamos al usuario del paquete mal contruido
        server_send_message(sockets_array[my_attention], 20, "");
        //Escribimos en el log file
        if(write_file_log){
          write_log(20, "", my_attention, 0);
        }
        //Desconectamos a los clientes
        for (int i = 0; i < 2; ++i)
        {
          //Avisamos de la desconexion        
          server_send_message(sockets_array[i], 17, "");
          //Escribimos en el log file
          if(write_file_log){
            write_log(17, "", i, 0);
          }
        }
        free(client_message);
        break; //Terminamos la ejecucion
      }

      else{
        //Si no quiere seguir jugando
        if(response == 0){
          for (int i = 0; i < 2; ++i)
          {
            //Avisamos de la desconexion        
            server_send_message(sockets_array[i], 17, "");
            //Escribimos en el log file
            if(write_file_log){
              write_log(17, "", i, 0);
            }
          }
          free(client_message);
          break;
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
            char partida[2] = {game->partida, '\0'};
            //Avisamos que inicio la partida
            server_send_message(sockets_array[i], 7, partida);
            //Escribimos en el log file
            if(write_file_log){
              write_log(7, partida, i, 0);
            }

            // Enviamos los puntajes
            char points[3] ={p->points, game->players[(i + 1) %2]->points, '\0'};

            server_send_message(sockets_array[i], 8, points);
            //Escribimos en el log file
            if(write_file_log){
              write_log(8, points, i, 0);
            }

            // Enviamos las primeras palabras          
            server_send_message(sockets_array[i], 9, game->words);
            //Escribimos en el log file
            if(write_file_log){
              write_log(9, game->words, i, 0);
            }
          }
          //Restauramos las respuestas delos clientes para nuevo juego
          p->new_game = NOTANSWER;
          game->players[(my_attention + 1) % 2]->new_game = NOTANSWER;
        }
      }

      free(client_message);
    }

    /** El cliente se desconecta*/
    else if(msg_code == 17){
      char * client_message = server_receive_payload(sockets_array[my_attention]);

      //Escribimos en el log file
      if(write_file_log){
        write_log(17, client_message, my_attention, size_payload);
      }

      //Paquete mal construido
      if(size_payload != 0){
        //Avisamos que el paquete esta mal construido
        server_send_message(sockets_array[my_attention], 20, "");
        //Escribimos en el log file
        if(write_file_log){
          write_log(20, "", my_attention, 0);
        }
        //Desconectamos a los clientes
        for (int i = 0; i < 2; ++i)
        {
          //Avisamos de la desconexion        
          server_send_message(sockets_array[i], 17, "");
          //Escribimos en el log file
          if(write_file_log){
            write_log(17, "", i, 0);
          }
        }
        free(client_message);
        break; //Terminamos la ejecucion
      }

      int adversario = (my_attention + 1) % 2;

      //El ganador es el jugador que no se desconecta
      char winner[2] = {game->players[adversario]->id, '\0'};

      for (int i = 0; i < 2; ++i)
      {
        //Avisamos al ganador de la partida
        server_send_message(sockets_array[i], 14, winner);
        //Escribimos en el log file
        if(write_file_log){
          write_log(14, winner, i, 0);
        }
        //Avisamos de la desconexion        
        server_send_message(sockets_array[i], 17, "");
        //Escribimos en el log file
        if(write_file_log){
          write_log(17, "", i, 0);
        }
      }
      //Liberamos la memoria
      free(client_message);
      break;
    }

    /** Paquete incorrecto */
    else if(msg_code != 0){
      char * client_message = server_receive_payload(sockets_array[my_attention]);
      //Escribimos en el log file
      if(write_file_log){
        write_log(msg_code, client_message, my_attention, size_payload);
      }

      //Avisamos al cliente que el paquete es incorrecto
      server_send_message(sockets_array[my_attention], 20, "");

      //Escribimos en el log file
      if(write_file_log){
        write_log(20, "", my_attention, 0);
      }
      
      //Desconectamos a los clientes
      for (int i = 0; i < 2; ++i)
      {
        //Avisamos de la desconexion        
        server_send_message(sockets_array[i], 17, "");
        //Escribimos en el log file
        if(write_file_log){
          write_log(17, "", i, 0);
        }
      }
      free(client_message);
      break; //Terminamos la ejecucion
    }
    //Escuchamos al otro cliente
    my_attention = (my_attention + 1) % 2;
    printf("------------------\n");
  }

  // Se cierra el socket
  for(int i=0; i<2; i++){
    close(sockets_array[i]);
  }
  // Liberamos la lista de jugadores
  if(players_info){
    free(players_info);
  }

  //Destruimos el juego
  if(game){
    destroy_game(game);
  }

  return 0;
}
