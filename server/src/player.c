#include "player.h"


Player* init_player(char* name,uint8_t ID){
	Player* p = malloc(sizeof(Player));
	//strcpy(p->name, name);
	p->points = 0;
	p->round_point = 0;
	p->id = ID;
	p->aim = 0;
	return p;
}

void destroy_player(Player* p){
	free(p);
}

Game* init_game(){
	Game* g = malloc(sizeof(Game));
	g->n_players = 0;
	g->partida = 1;
	g->players =(Player**) malloc(2 * sizeof(Player*));
	return g;
}

void destroy_game(Game* game){
	for(int i= 0; i<game->n_players; i++){
		destroy_player(game->players[i]);
	}
	free(game);
}