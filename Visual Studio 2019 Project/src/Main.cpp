#include <clf.h>

class Game : public clf::Engine {
public:
	Game() = default;
public:
	SDL_Rect characterDST{ 100, 100, 64, 64 };
	SDL_Rect characterSRC{ 0, 0, 32, 32 };
	SDL_Rect characterDir{ 0, 0, 0, 0 };
	SDL_Texture* characterTexture{ nullptr };
	float characterSpeed{ 520.0f };

	Mix_Chunk* sound{ nullptr };
	SDL_Texture* text{ nullptr };
protected:
	void OnStart() override {
		characterTexture = clf::Asset::LoadSprite("assets/character.png");

		sound = clf::Asset::LoadSound("assets/shoot.wav");
		clf::Sound::PlayFadeInChannel(2, sound, 2, 1000);
		text = clf::Asset::LoadText("assets/pico.ttf", 18, "Hello World", { 255, 255, 255, 255 }, 0);
	}

	void OnInput(const Uint8* keystates) override {
		if (keystates[SDL_SCANCODE_W]) 
			characterDir.y = -1;
		else if (keystates[SDL_SCANCODE_S])
			characterDir.y = 1;
		else
			characterDir.y = 0;

		if (keystates[SDL_SCANCODE_A]) 
			characterDir.x = -1;
		else if(keystates[SDL_SCANCODE_D]) 
			characterDir.x = 1;
		else
			characterDir.x = 0;
	}

	void OnUpdate(float deltaTime) override {
		characterDST.x += static_cast<int>(characterSpeed * deltaTime * characterDir.x);
		characterDST.y += static_cast<int>(characterSpeed * deltaTime * characterDir.y);
	}

	void OnRender() override {
		clf::Render::Clear({ 34, 56, 20, 255 });
		clf::Render::DrawSpriteRot(characterTexture, characterSRC, characterDST, 90.0, nullptr, SDL_FLIP_HORIZONTAL);
		clf::Render::DrawFillCircle({ 1, 1 }, 20, { 255, 255, 255, 255 });
		clf::Render::DrawText(text, { 300, 300, clf::Info::GetTextureWidth(text), clf::Info::GetTextureHeight(text) });
	}

	void OnFinish() override {
		clf::Asset::FreeTexture(characterTexture);
		clf::Asset::FreeTexture(text);
	}
};

int main(int argc, char *args[]) {
	Game game;
	game.Build("Game", 600, 600, SDL_INIT_EVERYTHING, SDL_WINDOW_SHOWN);

	return 0;
}