#pragma once
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////
///         FUNCIONES PRIVADAS         ///
//////////////////////////////////////////

/** Retrona una matriz de 20x20 con un caracter comun*/
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