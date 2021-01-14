#include <CLF/CLF.h>

class Game : public CLF {
public:
	Game() : CLF() { };
	virtual ~Game() = default;
protected:
	void OnStart() override {
	
	}

	void OnInput() override {
	
	}

	void OnUpdate(float deltaTime) override {
		
	}

	void OnRender() override {
		Clear(Color::POMEGRANATE);
	}

	void OnFinish() override {
	
	}
};

/*
	TODO:
	Draw Shapes (Outlined and Filled):
	-Triangle
	-Square
	-Rectangle
	-Circle
*/

int main(int argc, char *args[]) {
	Game game;
	if (game.Initialize("Game", 480, 320))
		game.Build();
	else
		return -1;

	return 0;
}