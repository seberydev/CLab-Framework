#include <clf.h>

class Game : public clf::Engine {
public:
	Game() = default;
	virtual ~Game() = default;
public:
	clf::Sprite character{ {0, 0, 32, 32}, { 100, 100, 64, 64} };
	SDL_Texture* texture{nullptr};
protected:
	void OnStart() override {
		texture = clf::Asset::LoadPNG("assets/character.png");
	}

	void OnInput(const Uint8* keystates) override {
		if (keystates[SDL_SCANCODE_W]) 
			character.dst.y -= 1;
		else if (keystates[SDL_SCANCODE_A]) 
			character.dst.x -= 1;
		else if (keystates[SDL_SCANCODE_S]) 
			character.dst.y += 1;
		else if(keystates[SDL_SCANCODE_D]) 
			character.dst.x += 1;
		
	}

	void OnUpdate(float deltaTime) override {
		
	}

	void OnRender() override {
		clf::Draw::Clear({ 34, 56, 20, 255 });
		clf::Draw::DrawSprite(texture, character);
	}

	void OnFinish() override {
	
	}
};

int main(int argc, char *args[]) {
	Game game;
	game.Build("Game", 600, 600);

	return 0;
}