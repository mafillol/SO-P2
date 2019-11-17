#include "functions.h"
#include "comunication.h"

/** Imprime las cartas a partir del payload*/
void print_cards(char* long_string){
	char** m1 = matrix();
	char** m2 = matrix();

	char** words = (char**)calloc(20, sizeof(char*));

	int aux = 0;
	int count = 0;
	int row = 0;
	char** matrix;

	//mientras no guardemos todas las palabars en tarjetas
	while(count<20){
		if(count<10){
			matrix = m1;
		}
		else{
			matrix = m2;
		}
		int largo = long_string[aux];
		int pos_random = long_string[aux+largo+1];

		words[count] = (char*) calloc(largo+1, sizeof(char));
		strncpy(words[count], &long_string[aux+1], largo);

		//Mientras la palabra se salga de la carta, se corre a la izquierda
		while((largo+pos_random) > 19){
			pos_random--;
		}
		int p = 0;
		for(int i=aux+1;i<aux+largo+1;i++){
			//Guardamos la palabra en la matiz
			matrix[row][pos_random] = long_string[i];
			pos_random++;
			//Guardamos la palabra para revisarla
			words[count][p] = long_string[i];
			p++;
		}
		count++;
		aux = aux + largo + 2;
		row = row +2;
		if(count == 10){
			row = 0;
		}
	}
	
	//Si la palabra repetida queda en la misma carta, las separa
	change_repeat_word_in_matrix(words, m1, m2);

	print_matrix(m1, m2);

	destroy_matrix(m1);
	destroy_matrix(m2);

	//Liberamos la memoria de la lsita de palabras
	for(int i=0; i<20; i++){
		free(words[i]);
	}
	free(words);
}


/** Imprime dos matrices en forma de cartas*/
void print_matrix(char** m1, char** m2){

	printf("\n");
	printf("                 CARTA 1                 \t");
	printf("                 CARTA 2                 \n\n");
	for(int row=0; row<20; row++){
		for(int col=0; col<20; col++){
			printf("%c ", m1[row][col]);
		}
		printf("\t");
		for(int col=0; col<20; col++){
			printf("%c ", m2[row][col]);
		}
		printf("\n");
	}

	printf("\n");
}

/** Retorna una matriz de 20x20 con un caracter comun*/
char** matrix(){

	char** m = (char**)calloc(20, sizeof(char*));

	for(int i=0; i<20; i++){
		m[i] = (char*)calloc(21,sizeof(char)); //21 para el termino del string
		for(int j=0; j<20; j++){
			m[i][j] = '-';
		}
	}
	return m;
}

/** Destruye una matriz de 20x20*/
void destroy_matrix(char** m){
	for(int i=0; i<20; i++){
		free(m[i]);
	}
	free(m);
}

/** Cambia de posicion la palabra si es que se encuentra en la misma carta*/
void change_repeat_word_in_matrix(char** words, char** m1, char** m2){

	int fila[10] = {0,2,4,6,8,10,12,14,16,18}; 
	int pos_m1 = repeat_word_in_matrix(words, m1);

	//Si hay una palabra repetida en la primera carta
	if(pos_m1 != -1){
		int new_position = fila[rand()%10];
		//Guardamos la referencia a la otra fila de la matriz
		char* aux = m2[new_position];
		//Invertimos los valores
		m2[new_position] = m1[pos_m1];
		m1[pos_m1] = aux;
	}

	int pos_m2 = repeat_word_in_matrix(words, m2);

	//Si hay una palabra repetida en la segunda carta
	if(pos_m2 != -1){
		int new_position = fila[rand()%10];
		//Guardamos la referencia a la otra fila de la matriz
		char* aux = m1[new_position];
		//Invertimos los valores
		m1[new_position] = m2[pos_m2];
		m2[pos_m2] = aux;
	}
}

/** Retorna una de las posiciones de la palabra repetida en la matriz, o -1 si no encuentra*/
int repeat_word_in_matrix(char** words, char** matrix){

	//Para cada una de las palabras de la lista
	for(int i=0; i<20; i++){
		int count = 0;
		//Para cada una de las filas de la matriz
		for(int j=0; j<20; j++){
			//Si la palabra se encuentra en la fila de la matriz
			if (strstr(matrix[j], words[i]) != NULL){
				count ++;
			}
			//Si se ha encontrado la palabra mas de una vez
			if(count > 1){
				//Retornamos la posicion en la matriz
				return j;
			}
		}
	}
	//Si no hay palabras repetidas
	return -1;
}



/** Crea una copia del string en uppercase*/
//Obtenida del link:
//https://stackoverflow.com/questions/35181913/converting-char-to-uppercase-in-c
char* uppercase(char* word){
	char* s = (char*) calloc(strlen(word) + 1, sizeof(char));
	strcpy(s, word);
	char* tmp = s;

  while (*tmp) {
    *tmp = toupper((unsigned char) *tmp);
    tmp++;
  }
  return s;
}


// Forma de escribir time strap
// Obtenida de link: http://www.informit.com/articles/article.aspx?p=23618&seqNum=8
/** Funcion encargada de escribir en log*/
void write_log(int pkg_id, char* message, int size){
	// Abro el archivo
	FILE* file_logs = fopen("log.txt", "a");

	struct timeval tv;
 	struct tm* ptm;
 	char time_string[40];
 	
 	//Obtenemos el tiempo y lo convertimos en tm struct
	gettimeofday (&tv, NULL);
	ptm = localtime (&tv.tv_sec);long milliseconds;

	//Damos formato al tiempo en un solo string
 	strftime(time_string, sizeof (time_string), "%Y-%m-%d %H:%M:%S", ptm);

 	//Obtenemmos los milisegundos de los microsegundos
 	milliseconds = tv.tv_usec / 1000;

 	// Estribimos el tiempo en el archivo
 	fprintf(file_logs, "[%s.%03ld] ", time_string, milliseconds);

 	//Cambiar como obtener largo payload
	if(pkg_id == 1){
		fprintf(file_logs, "[CLIENT][PKGE OUT] AskConnection send to server. Package: %d %d %s\n",pkg_id,real_size_payload(pkg_id, message), message);
	}
	else if(pkg_id == 2){
		fprintf(file_logs, "[CLIENT][PKGE IN] ConnectionEstablished sended from server. Package: %d %d %s\n",pkg_id,size, message);
	}
	else if(pkg_id == 3){
		fprintf(file_logs, "[CLIENT][PKGE IN] AskNickname sended from server. Package: %d %d %s\n",pkg_id,size, message);
	}
	else if(pkg_id == 4){
		fprintf(file_logs, "[CLIENT][PKGE OUT] ReturnNickname send to server. Package: %d %d %s \n",pkg_id, (int)strlen(message), message);
	}
	else if(pkg_id == 5){
		fprintf(file_logs, "[CLIENT][PKGE IN] OpponentFound sended from server. Package: %d %d %s\n", pkg_id, size, message);
	}
	else if(pkg_id == 6){
		fprintf(file_logs, "[CLIENT][PKGE IN] SendIDs sended from server. Package: %d %d", pkg_id, size);
		for(int i=0; i<size; i++){
			fprintf(file_logs, " %d", (int)message[i]);
		}
		fprintf(file_logs, "\n");
	}
	else if(pkg_id == 7){
		fprintf(file_logs, "[CLIENT][PKGE IN] StartGame sended from server. Package: %d %d", pkg_id, size);
		for(int i=0; i<size; i++){
			fprintf(file_logs, " %d", (int)message[i]);
		}
		fprintf(file_logs, "\n");
	}
	else if(pkg_id == 8){
		fprintf(file_logs, "[CLIENT][PKGE IN] SendScore sended from server. Package: %d %d",pkg_id,size);
		for(int i=0; i<size; i++){
			fprintf(file_logs, " %d", (int)message[i]);
		}
		fprintf(file_logs, "\n");
	}
	else if(pkg_id == 9){
		fprintf(file_logs, "[CLIENT][PKGE IN] SendCards sended from server.Package: %d %d",pkg_id, size);
		int aux = 0;
		int count = 0;
		while(aux<size){
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
		fprintf(file_logs, "[CLIENT][PKGE OUT] SendWord send to server. Package: %d %d %s\n",pkg_id, real_size_payload(pkg_id,message),message);
	}
	else if (pkg_id == 11){
		fprintf(file_logs, "[CLIENT][PKGE IN] ResponseWord sended from server. Package: %d %d", pkg_id, size);
		for(int i=0; i<size; i++){
			fprintf(file_logs, " %d", (int)message[i]);
		}
		fprintf(file_logs, "\n");
	}
	else if(pkg_id == 12){
		fprintf(file_logs, "[CLIENT][PKGE IN] RoundWinnerLoser sended from server. Package: %d %d", pkg_id, size);
		for(int i=0; i<size; i++){
			fprintf(file_logs, " %d", (int)message[i]);
		}
		fprintf(file_logs, "\n");
	}
	else if(pkg_id == 13){
		fprintf(file_logs, "[CLIENT][PKGE IN] EndGame sended from server. Package: %d %d", pkg_id,size);
		for(int i=0; i<size; i++){
			fprintf(file_logs, " %d", (int)message[i]);
		}
		fprintf(file_logs, "\n");
	}
	else if(pkg_id == 14){
		fprintf(file_logs, "[CLIENT][PKGE IN] GameWinnerLoser sended from server. Package: %d %d", pkg_id, size);
		for(int i=0; i<size; i++){
			fprintf(file_logs, " %d", (int)message[i]);
		}
		fprintf(file_logs, "\n");
	}
	else if(pkg_id == 15){
		fprintf(file_logs, "[CLIENT][PKGE IN] AskNewGame sended from server. Package: %d %d %s\n", pkg_id, size, message);
	}
	else if(pkg_id == 16){
		fprintf(file_logs, "[CLIENT][PKGE OUT] AnswerNewGame send to server. Package: %d %d %d\n", pkg_id, real_size_payload(pkg_id, message), (int)message[0]);
	}
	else if(pkg_id == 17 && size == -1){
		fprintf(file_logs, "[CLIENT][PKGE OUT] Disconnect send to server. Package: %d %d %s\n", pkg_id, (int)strlen(message), message);
	}
	else if(pkg_id == 17){
		fprintf(file_logs, "[CLIENT][PKGE IN] Disconnect sended from server. Package: %d %d %s\n", pkg_id, size, message);
	}
	else if(pkg_id == 20){
		fprintf(file_logs, "[CLIENT][PKGE IN] Error Bad Package sended from server. Package: %d %d %s\n",pkg_id, size, message);
	}
	fclose(file_logs); 
}




////////////////////////////////////////////////////////////////////////////////////////
// IMPORTANTE - REFERENCIA                                                            //
// Funcion string a mayusculas obtenida de                                            //
// https://stackoverflow.com/questions/35181913/converting-char-to-uppercase-in-c     //
// Forma de escribir time strap obtenida de                                           //
// http://www.informit.com/articles/article.aspx?p=23618&seqNum=8                     //
////////////////////////////////////////////////////////////////////////////////////////