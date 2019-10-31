#include "player.h"

/** Inicializa un nuevo jugador*/
Player* init_player(char* name,uint8_t ID){

	Player* p = malloc(sizeof(Player));

	p->name = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(p->name, name);

	p->points = 0;
	p->round_point = 0;
	p->id = ID;
	p->aim = 3;
	p->answer = false;
	p->new_game = NOTANSWER;
	return p;
}

/** Destruye a un jugador*/
void destroy_player(Player* p){
	if(p){
		free(p->name);
		p->name = NULL;
		free(p);
		p = NULL;
	}	
}

/** Inicializa un nuevo juego*/
Game* init_game(){
	Game* g = malloc(sizeof(Game));
	g->n_players = 0;
	g->partida = 1;
	g->round = 1;
	g->players =(Player**) malloc(2 * sizeof(Player*));
	return g;
}

/** Destruye un juego*/
void destroy_game(Game* game){
	for(int i= 0; i<game->n_players; i++){
		destroy_player(game->players[i]);
	}
	if(game->players){
		free(game->players);
		game->players = NULL;
	}
	if(game->answer){
		free(game->answer);
		game->answer = NULL;
	}
	if(game->words){
		free(game->words);
		game->words = NULL;
	}
	if(game){
		free(game);
		game = NULL;
	}
}