#include <clf.h>

class Game : public clf::Engine {
public:
	Game() = default;
};

int main(int argc, char* args[]) {
	Game game;
	game.Build("Pong Game - CLab Framework", 400, 400, SDL_INIT_EVERYTHING, SDL_WINDOW_SHOWN);

	return 0;
}