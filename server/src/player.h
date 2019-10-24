#pragma once
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


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

	uint8_t round_point;

	uint8_t id;

	int socket;

	uint8_t aim;
	
} Player;

typedef struct game{
	Player** players;
	Status estado;
	int n_players;
	uint8_t partida;
	uint8_t round;
	char* answer;
} Game;



Player* init_player(char* name,uint8_t ID);

void destroy_player(Player* p);

Game* init_game();

void destroy_game(Game* game);