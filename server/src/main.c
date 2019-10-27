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

int main(int argc, char const *argv[])
{
  for(int i=0; i<2; i++){
    char** cards = get_random_cards(file_words);
    print_words(cards[1]);
    free(cards[0]);
    free(cards[1]);
    free(cards);
    printf("\n\n\n\n");
  }
  return 0;
}

// int main(int argc, char *argv[]){
//   // Se define una IP y un puerto
//   char * IP = "127.0.1.1";
//   int PORT = 8070;
//   Game* game = NULL;  

//   // Se crea el servidor y se obtienen los sockets de ambos clientes.
//   PlayersInfo * players_info = prepare_sockets_and_get_clients(IP, PORT);

//   // Guardaremos los sockets en un arreglo e iremos alternando a quién escuchar.
//   int sockets_array[2] = {players_info->socket_c1, players_info->socket_c2};
//   int my_attention = 0;
//   while (1)
//   {
//     // Se obtiene el paquete de uno de los clientes
//     int msg_code = server_receive_id(sockets_array[my_attention]);
//     printf("Mensaje : %d\n", msg_code);
    
//      /** El cliente desea inicar una partida*/
//     if (msg_code == 1)
//     { 
//       char * client_message = server_receive_payload(sockets_array[my_attention]);
//       //Avisamos que se establecio la conexion
//       server_send_message(sockets_array[my_attention], 2, "");
//       //Preguntamos por el nickname
//       server_send_message(sockets_array[my_attention], 3, "");

//       free(client_message);
//     }

//     /** El cliente establece un nickname*/
//     else if (msg_code == 4){
//       char * client_message = server_receive_payload(sockets_array[my_attention]);
//       ID ++;
//       Player* p = init_player(client_message, ID);
//       if(!game){
//         game = init_game();
//       }
//       game->players[my_attention] = p;
//       game->n_players++;

//       char response[1];
//       sprintf(response, "%d", p->id);
//       //Enviamos el id del jugador
//       server_send_message(sockets_array[my_attention], 6, response);

//       //Si se encuentra un contrincante
//       if(game->n_players>=2){
//       //if(game->n_players>=1){
//         int adversario = (my_attention + 1) % 2;
//         char name[1024];
//         strcpy(name, game->players[adversario]->name);
//         char buffer[1];

//         //Avisamos que se encontro un contrincante
//         server_send_message(sockets_array[adversario], 5, name);

//         //Generamos las primeras cartas
//         char** cards = get_random_cards(file_words);
//         game->answer = cards[0];
//         uppercase(game->answer); //Convertimos a mayusculas por precaucion
//         game->words = cards[1];

//         //Avisamos a ambos jugadores que inicio la partida
//         for(int i=0; i<2;i++){
//           //Avisamos que inicio la partida
//           sprintf(buffer, "%d", game->partida);
//           server_send_message(sockets_array[i], 7, buffer);
//           // Enviamos las primeras palabras          
//           server_send_message(sockets_array[i], 9, game->words);
//         }
//         free(cards);
//       }
//       free(client_message);
//     }

//     /** El cliente manda una palabra respuesta*/
//     else if(msg_code == 10){
//       char * client_message = server_receive_payload(sockets_array[my_attention]);

//       int adversario = (my_attention + 1) % 2;
      
//       Player* p = game->players[my_attention];
//       //Disminuimos los intentos restantes
//       p->aim = p->aim - 1;

//       //Si le quedan intentos y aun no contesta
//       if(p->aim > 0 && !p->answer){
//         //Convertimos la respuesta a mayusculas
//         uppercase(client_message);

//         // Si la respuesta es correcta
//         if(strcmp(client_message, game->answer) == 0){
//           char response[3];
//           response[0] = 1;
//           response[1] = p->aim;

//           //Enviamos la respuesta y los intentos
//           server_send_message(sockets_array[my_attention], 11, response);

//           //Guardamos que el usuario ha respondido
//           p->answer = true;

//           //Guardamos el puntaje
//           switch(p->aim){
//             case 2:
//               p->round_point = 5;
//               p->points = p->points + 5;
//               break;
//             case 1:
//               p->round_point = 3;
//               p->points = p->points + 3;
//               break;
//             case 0:
//               p->round_point = 1;
//               p->points = p->points + 1;
//               break;
//           }
//         }
//         //Si la respuesta es incorrecta
//         else{
//           char response[3];
//           response[0] = 0;
//           response[1] = p->aim;

//           //Enviamos la respuesta y los intentos
//           server_send_message(sockets_array[my_attention], 11, response);
//           //Volvemos a enviar las palabras
//           server_send_message(sockets_array[my_attention], 9, game->words);
//         }
//       }

//       //Respuesta equivocada en todos los intentos
//       else if(p->aim == 0 && !p->answer){
//         p->answer = true;
//       }

//       //Si ambos jugadores contestaron, la ronda termina
//       if(p->answer && game->players[adversario]->answer){
//         //Avisamos el ganador es el jugador actual
//         if (p->round_point > game->players[adversario]->round_point){
//           for(int i=0; i<2; i++){
//             char buffer[2];
//             sprintf(buffer, "%d",p->id);
//             server_send_message(sockets_array[i], 12, buffer);

//           }
//         }
//         //Avisamos que el ganador es el adversario
//         else if (p->round_point < game->players[adversario]->round_point){
//           for(int i=0; i<2; i++){
//             char buffer[2];
//             sprintf(buffer, "%d",game->players[adversario]->id);
//             server_send_message(sockets_array[i], 12, buffer);
//           }
//         }
//         //Avisamos que hubo empate
//         else if (p->round_point < game->players[adversario]->round_point){
//           for(int i=0; i<2; i++){
//             server_send_message(sockets_array[i], 12, "0");
//           }
//         }

//         //Avanzamos en la ronda
//         game->round++;
//         //Se termina la partida
//         if(game->round == 5){

//         }
//         //Avanza a la siguiente ronda
//         else{
//           //Liberamos las palabras y respuesta anterior
//           free(game->answer);
//           free(game->words);

//           //Generamos un nuevo set de palabras
//           char** cards = get_random_cards(file_words);
//           game->answer = cards[0];
//           uppercase(game->answer); //Convertimos a mayusculas por precaucion
//           game->words = cards[1];

//           for (int i = 0; i < 2; ++i)
//           {
//             Player* p = game->players[i];
//             p->answer = false; //Avisamos que los jugadores no han respondido
//             p->round_point = 0; //Limpiamos el puntaje de la ronda
//             // Enviamos las nuevas palabras de la ronda        
//             server_send_message(sockets_array[i], 9, game->words);
//           }
//           free(cards);
//         }
//       }
//       free(client_message);
//       printf("Valor al terminar %d\n", p->aim);
//     }

//     /** El cliente responde si quiere seguir jugando*/
//     else if(msg_code == 16){
//       char * client_message = server_receive_payload(sockets_array[my_attention]);
//       free(client_message);
//     }

//     /** El cliente se desconecta*/
//     else if(msg_code == 17){
//       char * client_message = server_receive_payload(sockets_array[my_attention]);
//       free(client_message);
//     }

//     printf("------------------\n");
//     // Mi atención cambia al otro socket
//     my_attention = (my_attention + 1) % 2;
//   }

//   //Se borra el juego
//   if(game){
//     destroy_game(game);
//   }

//   return 0;
// }
