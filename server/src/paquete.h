#pragma once
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include "player.h"

//////////////////////////////////////////
///         FUNCIONES PRIVADAS         ///
//////////////////////////////////////////

/** Funcion auxiliar para imprimir la palabra*/
void print_word(char* w);

/** Funcion auxiliar para imprimir las palabras unidas en un largo string*/
void print_words(char* long_string);

/** Retorna un la lista de strings, el primero es la respuesta 
y el segundo es un string largo con las 20 palabras*/
char** get_random_cards(char* file_name);

/** Convierte el string en uppercase*/
//Obtenida del link:
//https://stackoverflow.com/questions/35181913/converting-char-to-uppercase-in-c
void uppercase(char* word);

//Funcion auxiliar. Quita el salto de linea a un string
//Obtenida del link: 
//https://cboard.cprogramming.com/c-programming/70320-how-remove-newline-string.html
void strip(char* string);

/** Retorna true o false si la palabra se encuentra en la lista*/
bool word_in_array(char* word, char** cards);

//////////////////////////////////////////
///         FUNCIONES PUBLICAS         ///
//////////////////////////////////////////



/** Genera un nuevo grupo de palabras para el juego a partir de las palabras en el archivo*/
void new_words(Game* game, char* file_words);

// Forma de escribir time strap
// Obtenida de link: http://www.informit.com/articles/article.aspx?p=23618&seqNum=8
/** Funcion encargada de escribir en log*/
void write_log(int pkg_id, char* message, int socket, int size);





////////////////////////////////////////////////////////////////////////////////////////
// IMPORTANTE - REFERENCIA                                                            //
// Quita el salto de linea a un string obtenida de                                    //
// https://cboard.cprogramming.com/c-programming/70320-how-remove-newline-string.html //
// Funcion string a mayusculas obtenida de                                            //
// https://stackoverflow.com/questions/35181913/converting-char-to-uppercase-in-c     //
////////////////////////////////////////////////////////////////////////////////////////