#include <clf.h>

class Game : public clf::Engine {
public:
	Game() = default;
	virtual ~Game() = default;
public:
	SDL_Rect characterDST{ 100, 100, 64, 64 };
	SDL_Rect characterSRC{ 0, 0, 32, 32 };
	SDL_Texture* characterTexture{ nullptr };
	SDL_Texture* message{ nullptr };
	SDL_Color messageColor{ 255, 255, 255, 255 };
	SDL_Rect messageDST;
	int counter{0};
protected:
	void OnStart() override {
		characterTexture = clf::Asset::LoadPNG("assets/character.png");
		message = clf::Asset::LoadText("assets/pico.ttf", 14, "The Best Game", messageColor, 0);
		messageDST = { 20, 20, clf::Info::GetTextureWidth(message), clf::Info::GetTextureHeight(message) };
	}

	void OnInput(const Uint8* keystates) override {
		if (keystates[SDL_SCANCODE_W]) 
			characterDST.y -= 1;
		else if (keystates[SDL_SCANCODE_A]) 
			characterDST.x -= 1;
		else if (keystates[SDL_SCANCODE_S]) 
			characterDST.y += 1;
		else if(keystates[SDL_SCANCODE_D]) 
			characterDST.x += 1;

		if (keystates[SDL_SCANCODE_SPACE]) {
			++counter;
			message = clf::Asset::ChangeText(message, "assets/pico.ttf", 16, std::to_string(counter), messageColor, 1);
			messageDST = { 20, 20, clf::Info::GetTextureWidth(message), clf::Info::GetTextureHeight(message) };
		}
	}

	void OnUpdate(float deltaTime) override {
	
	}

	void OnRender() override {
		clf::Draw::Clear({ 34, 56, 20, 255 });
		clf::Draw::DrawFillRect({ 100, 100, 50, 50 }, { 255, 255, 255, 255 });
		clf::Draw::DrawSprite(characterTexture, characterSRC, characterDST);
		clf::Draw::DrawText(message, messageDST);
		clf::Draw::DrawCircle({ 400, 400 }, 20, { 255, 255, 255, 255 });
		clf::Draw::DrawTriangle({ 100, 100 }, { 150, 200 }, { 50, 200 }, { 255, 255, 255, 255 });
	}

	void OnFinish() override {
		SDL_DestroyTexture(characterTexture);
		SDL_DestroyTexture(message);
	}
};

int main(int argc, char *args[]) {
	Game game;
	game.Build("Game", 600, 600);

	return 0;
}