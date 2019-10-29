#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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


//////////////////////////////////////////
///         FUNCIONES PUBLICAS         ///
//////////////////////////////////////////

/** Imprime las cartas a partir del payload*/
void print_cards(char* long_string);

/** Crea una copia del string en uppercase*/
//Obtenida del link:
//https://stackoverflow.com/questions/35181913/converting-char-to-uppercase-in-c
char* uppercase(char* word);