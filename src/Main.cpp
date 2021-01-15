#include <CLF/CLF.h>

class Game : public clf::Engine {
public:
	Game() = default;
	virtual ~Game() = default;
protected:
	void OnStart() override {
	
	}

	void OnInput() override {
	
	}

	void OnUpdate(float deltaTime) override {
		
	}

	void OnRender() override {
		Clear(clf::Color::PORTAFINO);
		DrawTriangle(100, 400, 200, 200, 300, 400, clf::Color::METALLIC_BRONZE);
		DrawSquare(200, 10, 50, clf::Color::METALLIC_BRONZE);
		DrawRectangle(400, 400, 150, 50, clf::Color::METALLIC_BRONZE);
		DrawCircle(100.0, 100.0, 20.0, clf::Color::METALLIC_BRONZE);
	}

	void OnFinish() override {
		
	}
};

int main(int argc, char *args[]) {
	Game game;
	if (game.Initialize("Game", 600, 600))
		game.Build();
	else
		return -1;

	return 0;
}