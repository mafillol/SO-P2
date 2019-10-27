#include "functions.h"

/** Imprime las cartas a partir del payload*/
void print_cards(char* long_string){
	char** m1 = matrix();
	char** m2 = matrix();

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

		//Mientras la palabra se salga de la carta, se corre a la izquierda
		while(largo+pos_random >= 20){
			pos_random--;
		}

		for(int i=aux+1;i<aux+largo+1;i++){
			matrix[row][pos_random] = long_string[i];
			pos_random++;
		}
		count++;
		aux = aux + largo + 2;
		row = row +2;
		if(count == 10){
			row = 0;
		}
	}

	print_matrix(m1, m2);

	destroy_matrix(m1);
	destroy_matrix(m2);
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


/** Retrona una matriz de 20x20 con un caracter comun*/
char** matrix(){

	char** m = (char**)malloc(20*sizeof(char*));

	for(int i=0; i<20; i++){
		m[i] = (char*)malloc(20*sizeof(char));
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