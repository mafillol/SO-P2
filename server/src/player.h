#pragma once
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


/** Representa el estado de una partida*/
enum status
{
  FINISH = 0,
  START = 1,
  STANDBY = 2,
};
typedef enum status Status;

typedef struct player
{
	/** Nombre de usuario del jugador*/
	char name[1024];
	/** Puntos del jugador*/
	int points;

	int8_t round_point;

	int8_t id;

	int socket;

	int8_t aim;

	bool answer;
	
} Player;

typedef struct game{
	Player** players;

	Status estado;

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