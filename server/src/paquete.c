#include "paquete.h"


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

	//Escogemos las 19 palabras
	for(int i=0; i<19; i++){
		char* random_word = words[rand() % 500];
		bool save = false;
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
	}

	char* answer = (char*) calloc(500,sizeof(char));

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
	}
	//Eliminamos las palabras anteriores
	if(game->words){
		free(game->words);
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
}

////////////////////////////////////////////////////////////////////////////////////////
// IMPORTANTE - REFERENCIA                                                            //
// Quita el salto de linea a un string obtenida de                                    //
// https://cboard.cprogramming.com/c-programming/70320-how-remove-newline-string.html //
// Funcion string a mayusculas obtenida de                                            //
// https://stackoverflow.com/questions/35181913/converting-char-to-uppercase-in-c     //
////////////////////////////////////////////////////////////////////////////////////////