#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <ctype.h>

//////////////////////////////////////////
///         FUNCIONES PRIVADAS         ///
//////////////////////////////////////////

/** Retorna una matriz de 20x20 con un caracter comun*/
char** matrix();
/** Destruye una matriz de 20x20*/
void destroy_matrix(char** m);

/** Imprime dos matrices en forma de cartas*/
void print_matrix(char** m1, char** m2);

/** Retorna una de las posiciones de la palabra repetida en la matriz, o -1 si no encuentra*/
int repeat_word_in_matrix(char** words, char** matrix);

/** Cambia de posicion la palabra si es que se encuentra en la misma carta*/
void change_repeat_word_in_matrix(char** words, char** m1, char** m2);


//////////////////////////////////////////
///         FUNCIONES PUBLICAS         ///
//////////////////////////////////////////

/** Imprime las cartas a partir del payload*/
void print_cards(char* long_string);

/** Crea una copia del string en uppercase*/
//Obtenida del link:
//https://stackoverflow.com/questions/35181913/converting-char-to-uppercase-in-c
char* uppercase(char* word);

// Forma de escribir time strap
// Obtenida de link: http://www.informit.com/articles/article.aspx?p=23618&seqNum=8
/** Funcion encargada de escribir en log*/
void write_log(int pkg_id, char* message, int size);