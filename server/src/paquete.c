#include "paquete.h"


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
		words[i] = (char*) malloc((len+1)*sizeof(char));
		strcpy(words[i], line);
	}

	//Escogemos las 19 palabras
	for(int i=0; i<18; i++){
		char* random_word = words[rand() % 499];
		int len = strlen(random_word);

		bool save = false;
		//Mientras no guardemos la palabra escogida
		while(!save){
			//Seleccionamos una fila al azar para guardarla
			int pos_random = rand() % 20;
			if(!cards[pos_random]){
				cards[pos_random] = (char*) malloc((len+1)*sizeof(char));
				strcpy(cards[pos_random], random_word);
				save = true;
			}
		}
	}

	//Seleccionamos la respuesta
	char* answer = NULL;
	while(!answer){
		char* answer = cards[rand() % 20];
		printf("Estamos fallando\n");
	}
	//Encontramos la unica fila sin palabra
	for(int i=0; i<19; i++){
		//Guardamos la respuesta
		if(!cards[i]){
			int len = strlen(answer);
			cards[i] = (char*) malloc((len+1)*sizeof(char));
			strcpy(cards[i], answer);
			break;
		}
	}
	printf("Estamos fallando\n");

	//Cerramos el archivo
	fclose(file);

	for(int i=0; i<19;i++){
    	printf("%s\n", cards[i]);
  	}


	//Eliminamos las palabras de la lista
	for(int i=0; i<500;i++){
		free(words[i]);
	}

	//Eliminamos la lista de palabras
	free(words);
	//Retornamos las cartas
	return cards;
}

//Funcion auxiliar. Quita el salto de linea a un string
//Obtenida del link: 
//https://cboard.cprogramming.com/c-programming/70320-how-remove-newline-string.html
void strip(char* string){
  string[strcspn (string,"\n")] = '\0';
}

////////////////////////////////////////////////////////////////////////////////////////
// IMPORTANTE - REFERENCIA                                                            //
// Quita el salto de linea a un string obtenida de                                    //
// https://cboard.cprogramming.com/c-programming/70320-how-remove-newline-string.html //
////////////////////////////////////////////////////////////////////////////////////////