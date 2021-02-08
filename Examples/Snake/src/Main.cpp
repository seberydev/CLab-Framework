#include "Game.h"

int main(int argc, char* args[]) {
	Game game;
	if (!game.Build(
		"Snake Game - CLab Framework",
		608,
		400,
		SDL_INIT_EVERYTHING,
		SDL_WINDOW_SHOWN,
		"assets/icon.png"))
		return -1;
	
	return 0;
}

