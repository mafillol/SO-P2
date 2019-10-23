#pragma once
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
	char* name;
	/** Puntos del jugador*/
	int points;

	int id;

	int socket;

	int aim;
	
} Player;

typedef struct game{
	Player** players;
	Status estado;
	int n_players;
	int partida;
	char* answer;
} Game;



Player* init_player(char* name,int ID);

Game* init_game();

void destroy_game(Game* game);