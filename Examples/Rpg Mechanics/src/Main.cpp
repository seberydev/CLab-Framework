#include "Game.h"

int main(int argc, char* args[]) {
	Game game;
	if (!game.Build(
		"Rpg Mechanics - CLab Framework",
		608,
		400,
		SDL_INIT_EVERYTHING,
		SDL_WINDOW_SHOWN,
		nullptr))
		return -1;



	return 0;
}