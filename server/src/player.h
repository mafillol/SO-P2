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

typedef struct player
{
	/** Nombre de usuario del jugador*/
	char* name;
	/** Puntos del jugador*/
	int points;

	int8_t round_point;

	int8_t id;

	int socket;

	int8_t aim;

	bool answer;

	Answer_New_Game new_game;
	
} Player;

typedef struct game{
	Player** players;

	int n_players;

	int8_t partida;

	int8_t round;

	char* answer;

	char* words;

} Game;



Player* init_player(char* name,uint8_t ID);

void destroy_player(Player* p);

Game* init_game();

void destroy_game(Game* game);