#include "Game.h"

int main(int argc, char* args[]) {
	Game game;
	if (!game.Build("Pong Game - CLab Framework", 600, 400, SDL_INIT_EVERYTHING, SDL_WINDOW_SHOWN))
		return -1;

	return 0;
}