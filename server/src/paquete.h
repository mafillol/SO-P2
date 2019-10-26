#pragma once
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>


//////////////////////////////////////////
///         FUNCIONES PRIVADAS         ///
//////////////////////////////////////////

/** Funcion auxiliar para imprimir las palabras*/
void print_word(char* w);

/** Funcion auxiliar para imprimir las palabras unidas en un largo string*/
void print_words(char* long_string);

//Funcion auxiliar. Quita el salto de linea a un string
//Obtenida del link: 
//https://cboard.cprogramming.com/c-programming/70320-how-remove-newline-string.html
void strip(char* string);



//////////////////////////////////////////
///         FUNCIONES PUBLICAS         ///
//////////////////////////////////////////

/** Retorna un la lista de strings, el primero es la respuesta 
y el segundo es un string largo con las 20 palabras*/
char** get_random_cards(char* file_name);


