#include <CLF/CLF.h>

class Game : public clf::Engine {
public:
	Game() = default;
	virtual ~Game() = default;
protected:
	void OnStart() override {
		clf::Asset::LoadPNG( {"character", "assets/character.png", { 0, 0, 32, 32 }, { 0, 0, 32, 32 }} );
	}

	void OnInput() override {
	
	}

	void OnUpdate(float deltaTime) override {
		clf::Asset::GetAsset("character").dst.x += 1;
	}

	void OnRender() override {
		clf::Draw::Clear(clf::Color::PORTAFINO);
		clf::Draw::DrawTriangle(100, 400, 200, 200, 300, 400, clf::Color::METALLIC_BRONZE);
		clf::Draw::DrawSquare(200, 10, 50, clf::Color::METALLIC_BRONZE);
		clf::Draw::DrawRectangle(400, 400, 150, 50, clf::Color::METALLIC_BRONZE);
		clf::Draw::DrawCircle(100.0, 100.0, 20.0, clf::Color::METALLIC_BRONZE);
		clf::Draw::DrawSprite("character");
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