#pragma once
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


/** Representa el estado de la respuesta del jugador a una nueva partida*/
enum answer_new_game
{
  NO = 0,
  YES = 1,
  NOTANSWER = 2,
};

typedef enum answer_new_game Answer_New_Game;

/** Representa a un jugador*/
typedef struct player
{
	/** Nombre de usuario del jugador*/
	char* name;
	/** Puntos del jugador*/
	int points;
	/** Puntos de la ronda actual*/
	int8_t round_point;
	/** ID del jugador*/
	int8_t id;
	/** Intentos restantes para la ronda actual*/
	int8_t aim;
	/** El jugador ha respondido o no en la ronda actual*/
	bool answer;
	/** Respuesta del jugador a una nueva partida*/
	Answer_New_Game new_game;
	
} Player;

/** Representa a un juego entre dos jugadores*/
typedef struct game{
	/** Array de jugadores en el juego*/
	Player** players;
	/** Cantidad de jugadores*/
	int n_players;
	/** Numero de partida actual*/
	int8_t partida;
	/** Numero de ronda actual*/
	int8_t round;
	/** Respuesta a la ronda actual, en la partida actual*/
	char* answer;
	/** Palabras de la ronda actual, en la partida actual*/
	char* words;

} Game;


//////////////////////////////////////////
///         FUNCIONES PRIVADAS         ///
//////////////////////////////////////////

/** Destruye a un jugador*/
void destroy_player(Player* p);





//////////////////////////////////////////
///         FUNCIONES PUBLICAS         ///
//////////////////////////////////////////

/** Inicializa un nuevo jugador*/
Player* init_player(char* name,uint8_t ID);

/** Inicializa un nuevo juego*/
Game* init_game();

/** Destruye un juego*/
void destroy_game(Game* game);