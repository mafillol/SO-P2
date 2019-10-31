#include "paquete.h"
#include "comunication.h"

/** Retorna un la lista de strings, el primero es la respuesta 
y el segundo es un string largo con las 20 palabras*/
char** get_random_cards(char* file_name){

	//Inicializo variables auxiliares para leer el archivo
	char* line = NULL; 
	size_t size = 0;



	// Abro el archivo
	FILE* file = fopen(file_name, "r");

	//Declaramos un arreglo de palabras
	char** words = (char**) malloc(500*sizeof(char*));

	//Declaramos las palabras de las cartas
	char** cards = (char**) calloc(20,sizeof(char*));

	//Guardamos en una lista cada una de las palabras del archivo
	for(int i=0; i<500; i++){
		getline(&line, &size, file);
		strip(line);
		int len = strlen(line);
		words[i] = (char*) malloc((len+2)*sizeof(char));
		strcpy(words[i], line);
	}

	int total_size = 0; //Largo total del payload
	int count = 0;

	//Escogemos las 19 palabras
	while(count<19){
		char* random_word = words[rand() % 500];
		bool save = false;
		//Si la palabra ya fue seleccionada, escogemos otra
		if(word_in_array(random_word, cards)) continue;
		//Mientras no guardemos la palabra escogida
		while(!save){
			//Seleccionamos una fila al azar para guardarla
			int pos_random = rand() % 20;
			if(!cards[pos_random]){
				int largo; //Largo de la palabra
				char word[strlen(random_word) + 1]; //Palabra
				sscanf(random_word, "%d,%s",&largo, word);
				cards[pos_random] = (char*) calloc(largo+3,sizeof(char));

				total_size = total_size + largo + 2;

				//Columna random para la carta
				uint8_t random_col = rand() % 20;
				//Mientras la carta quede fuera de la targeta
				while(random_col + largo >19){
					//Lo movemos a la izquierda
					random_col--;
				}
				//Agregamos el largo
				cards[pos_random][0] = largo;
				//Agregamos los caracteres
				memcpy(&cards[pos_random][1], word, largo);
				//Agregamos la posicion al final de la palabra
				cards[pos_random][largo+1] = random_col;
				save = true;
			}
		}
		count ++;
	}

	char* answer = (char*) calloc(500,sizeof(char));;

	//Para todas las filas de cartas
	for(int i=0; i<20; i++){

		//Encontramos la unica sin carta
		if(!cards[i]){
			//Tomamos una carta cualquiera
			int random_card = rand() % 20;
			while(!cards[random_card]){
				random_card = rand() % 20;
			}
			int largo = cards[random_card][0]; // Largo de la palabra
			cards[i] = (char*) calloc(largo+3,sizeof(char));

			total_size = total_size + largo + 2;

			strcpy(cards[i], cards[random_card]);

			//Guardamos el string respuesta para facilitar el juego
			strncpy(answer, &cards[random_card][1], largo);

			//Columna random para la carta repetida
			uint8_t random_col = rand() % 20;

			while(random_col + largo >19){
				//Lo movemos a la izquierda
				random_col--;
			}
			cards[i][largo+1] = random_col;
			break;
		}
	}

	//Concatenamos las 20 palabras en un string largo
	char* long_string = (char*) calloc((total_size+1),sizeof(char));

	int aux = 0;
	for(int i=0; i<20;i++){
		memcpy(&long_string[aux], cards[i], cards[i][0] + 2);
		aux = aux + cards[i][0] + 2;
	}

	//Armamos la respuesta
	char** payload = (char**) calloc(2,sizeof(char*));
	payload[0] = answer;
	payload[1] = long_string;

	//Liberamos la linea de lectura del archivo
	free(line);
	//Cerramos el archivo
	fclose(file);

	//Eliminamos las palabras de la lista
	for(int i=0; i<500;i++){
		free(words[i]);
	}
	//Eliminamos la lista de palabras
	free(words);

	for(int i=0;i<20;i++){
		free(cards[i]);
	}
	free(cards);
	//Retornamos las cartas
	return payload;
}

/** Funcion auxiliar para imprimir las palabras*/
void print_word(char* w){

	int largo = w[0];

	printf("%d ", largo);
	for(int j=1;j-1<largo;j++){
		printf("%c", w[j]);
	}
	int pos_random = w[largo+1];
	printf(" %d\n", pos_random);
}

/** Funcion auxiliar para imprimir las palabras unidas en un largo string*/
void print_words(char* long_string){
	int aux = 0;
	int count = 0;
	while(count<20){
		int largo = long_string[aux];

		printf("%d ", largo);

		for(int i=aux+1;i<aux+largo+1;i++){
			printf("%c", long_string[i]);
		}
		int pos_random = long_string[aux+largo+1];
		printf(" %d\n", pos_random);
		count++;
		aux = aux + largo + 2;
	}
}

/** Retorna true o false si la palabra se encuentra en la lista*/
bool word_in_array(char* word, char** cards){
	int largo; //Largo de la palabra
	char w[strlen(word) + 1]; //Palabra
	sscanf(word, "%d,%s",&largo, w);
	for(int i=0; i<20; i++){
		//Si hay una carta
		if(cards[i]){
			//Si la palabra se encuentra
			if(strstr(&cards[i][1], w) != NULL){
				return true;
			}
		}
	}
	//Nunca encontramos la palabra
	return false;
}

//Funcion auxiliar. Quita el salto de linea a un string
//Obtenida del link: 
//https://cboard.cprogramming.com/c-programming/70320-how-remove-newline-string.html
void strip(char* string){
  string[strcspn (string,"\n")] = '\0';
}

/** Convierte el string en uppercase*/
//Obtenida del link:
//https://stackoverflow.com/questions/35181913/converting-char-to-uppercase-in-c
void uppercase(char* word){
	char *s = word;
  while (*s) {
    *s = toupper((unsigned char) *s);
    s++;
  }
}

/** Genera un nuevo grupo de palabras para el juego a partir de las palabras en el archivo*/
void new_words(Game* game, char* file_words){
	//Eliminamos la respuesta anterior
	if(game->answer){
		free(game->answer);
		game->answer = NULL;
	}
	//Eliminamos las palabras anteriores
	if(game->words){
		free(game->words);
		game->words = NULL;
	}
	//Generamos un nuevo grupo de palabras
	char** cards = get_random_cards(file_words);
	//Guardamos la nueva respuesta para comparar
  game->answer = cards[0];
  //Convertimos a mayusculas por precaucion para comparar mas facil las respuestas
  uppercase(game->answer);
  //Guardamos las palabras nuevas
  game->words = cards[1];
  free(cards);
  cards = NULL;
}


// Forma de escribir time strap
// Obtenida de link: http://www.informit.com/articles/article.aspx?p=23618&seqNum=8
/** Funcion encargada de escribir en log*/
void write_log(int pkg_id, char* message, int socket){
	// Abro el archivo
	FILE* file_logs = fopen("log.txt", "a");

	struct timeval tv;
 	struct tm* ptm;
 	char time_string[40];
 	
 	// Obtain the time of day, and convert it to a tm struct
  gettimeofday (&tv, NULL);
  ptm = localtime (&tv.tv_sec);long milliseconds;

  ///Format the date and time, down to a single second
 	strftime(time_string, sizeof (time_string), "%Y-%m-%d %H:%M:%S", ptm);

 	//Obtenemmos los milisegundos de los microsegundos
 	milliseconds = tv.tv_usec / 1000;

 	// Estribimos el tiempo en el archivo
 	fprintf(file_logs, "[%s.%03ld] ", time_string, milliseconds);

 	//Cambiar como obtener largo payload
	if(pkg_id == 1){
		fprintf(file_logs, "[SERVER][PKGE IN] AskConnection received. Package: %d %d %s\n",pkg_id,real_size_payload(pkg_id, message), message);
	}
	else if(pkg_id == 2){
		fprintf(file_logs, "[SERVER][PKGE OUT] ConnectionEstablished sended to socket %d. Package: %d %d %d\n",socket+1,pkg_id,real_size_payload(pkg_id, message), (int)message[0]);
	}
	else if(pkg_id == 3){
		fprintf(file_logs, "[SERVER][PKGE OUT] AskNickname sended to socket %d. Package: %d %d %d\n",socket+1,pkg_id,real_size_payload(pkg_id, message), (int)message[0]);
	}
	else if(pkg_id == 4){
		fprintf(file_logs, "[SERVER][PKGE IN] ReturnNickname received. Package: %d %d %s \n",pkg_id, (int)strlen(message), message);
	}
	else if(pkg_id == 5){
		fprintf(file_logs, "[SERVER][PKGE OUT] OpponentFound sended to socket %d. Package: %d %d %s\n",socket+1, pkg_id, real_size_payload(pkg_id, message), message);
	}
	else if(pkg_id == 6){
		fprintf(file_logs, "[SERVER][PKGE OUT] SendIDs sended to socket %d. Package: %d %d %d\n", socket+1, pkg_id, real_size_payload(pkg_id,message), (int)message[0]);
	}
	else if(pkg_id == 7){
		fprintf(file_logs, "[SERVER][PKGE OUT] StartGame sended to socket %d. Package: %d %d %d\n", socket+1, pkg_id, real_size_payload(pkg_id,message), (int)message[0]);
	}
	else if(pkg_id == 8){
		fprintf(file_logs, "[SERVER][PKGE OUT] SendScore sended to socket %d. Package: %d %d %d %d\n", socket+1,pkg_id, real_size_payload(pkg_id,message), (int)message[0], (int)message[1]);
	}
	else if(pkg_id == 9){
		fprintf(file_logs, "[SERVER][PKGE OUT] SendCards sended to socket %d.Package: %d %d", socket+1, pkg_id, real_size_payload(pkg_id,message));
		int aux = 0;
		int count = 0;
		while(count<20){
			int largo = message[aux];
			fprintf(file_logs, " %d ",largo);
			for(int i=aux+1;i<aux+largo+1;i++){
				fprintf(file_logs, "%c", message[i]);
			}
			int pos_random = message[aux+largo+1];
			fprintf(file_logs, " %d", pos_random);
			count++;
			aux = aux + largo + 2;
	
		}
		fprintf(file_logs, "\n");
	}
	else if(pkg_id == 10){
		fprintf(file_logs, "[SERVER][PKGE IN] SendWord received from socket %d. Package: %d %d %s\n",socket+1, pkg_id, real_size_payload(pkg_id,message),message);
	}
	else if (pkg_id == 11){
		fprintf(file_logs, "[SERVER][PKGE OUT] ResponseWord sended to socket %d. Package: %d %d %d %d\n", socket+1, pkg_id, real_size_payload(pkg_id,message),(int)message[0], (int)message[1]);
	}
	else if(pkg_id == 12){
		fprintf(file_logs, "[SERVER][PKGE OUT] RoundWinnerLoser sended to socket %d. Package: %d %d %d\n", socket+1, pkg_id, real_size_payload(pkg_id,message), (int)message[0]);
	}
	else if(pkg_id == 13){
		fprintf(file_logs, "[SERVER][PKGE OUT] EndGame sended to socket %d. Package: %d %d %d\n",socket+1, pkg_id, real_size_payload(pkg_id,message), (int)message[0]);
	}
	else if(pkg_id == 14){
		fprintf(file_logs, "[SERVER][PKGE OUT] GameWinnerLoser sended to socket %d. Package: %d %d %d\n", socket+1, pkg_id, real_size_payload(pkg_id,message), (int)message[0]);
	}
	else if(pkg_id == 15){
		fprintf(file_logs, "[SERVER][PKGE OUT] AskNewGame sended to socket %d. Package: %d %d %s\n",socket+1,pkg_id,(int)strlen(message), message);
	}
	else if(pkg_id == 16){
		fprintf(file_logs, "[SERVER][PKGE IN] AnswerNewGame received from socket %d. Package: %d %d %d", socket+1, pkg_id, real_size_payload(pkg_id,message), (int)message[0]);
	}
	else if(pkg_id == 17){
		fprintf(file_logs, "[SERVER][PKGE OUT] Disconnect sended to socket %d. Package: %d %d %s\n", socket+1, pkg_id, (int)strlen(message), message);
	}
	else if(pkg_id == 20){
		fprintf(file_logs, "[SERVER][PKGE OUT] Error Bad Package sended to socket %d. Package: %d %d %s\n",socket+1, pkg_id, (int)strlen(message), message);
	}
	else{
		fprintf(file_logs, "[SERVER][PKGE IN] Error Bad Package received from socket %d. Package: %d %d %s\n",socket+1, pkg_id, (int)strlen(message), message);
	}
	fclose(file_logs); 
}

////////////////////////////////////////////////////////////////////////////////////////
// IMPORTANTE - REFERENCIA                                                            //
// Quita el salto de linea a un string obtenida de                                    //
// https://cboard.cprogramming.com/c-programming/70320-how-remove-newline-string.html //
// Funcion string a mayusculas obtenida de                                            //
// https://stackoverflow.com/questions/35181913/converting-char-to-uppercase-in-c     //
////////////////////////////////////////////////////////////////////////////////////////