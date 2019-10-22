#include "player.h"




Player* init_player(char* name,int ID){
	Player* p = malloc(sizeof(Player));
	p->name = malloc(strlen(name)*sizeof(char));
	strcpy(p->name, name);
	p->points = 0;
	p->id = ID;
	return p;
}


Game* init_game(){
	Game* g = malloc(sizeof(Game));
	g->n_players = 0;
	g->players =(Player**) malloc(2 * sizeof(Player*));
	return g;
}



void destroy_game(Game* game){
	for(int i= 0; i<game->n_players; i++){
		free(game->players[i]->name);
		free(game->players[i]);
	}
	free(game);
}