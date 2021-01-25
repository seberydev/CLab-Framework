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
	Mix_Music* mainSound{ nullptr };
	Mix_Chunk* soundEffect{ nullptr };
protected:
	void OnStart() override {
		characterTexture = clf::Asset::LoadSprite("assets/character.png");
		message = clf::Asset::LoadText("assets/pico.ttf", 14, "The Best Game", messageColor, 0);
		messageDST = { 20, 20, clf::Info::GetTextureWidth(message), clf::Info::GetTextureHeight(message) };
		mainSound = clf::Asset::LoadMusic("assets/sound.ogg");
		soundEffect = clf::Asset::LoadSound("assets/shoot.wav");
		clf::Sound::SetMusicVolume(100);
		//clf::Sound::PlayFadeInMusic(mainSound, true, 0, 5000);

		std::cout << clf::Sound::GetChannelVolume(3) << "\n";
	}

	void OnInput(const Uint8* keystates, const SDL_Event& events, int currentEvents) override {
		if (keystates[SDL_SCANCODE_W]) 
			characterDST.y -= 1;
		else if (keystates[SDL_SCANCODE_A]) 
			characterDST.x -= 1;
		else if (keystates[SDL_SCANCODE_S]) 
			characterDST.y += 1;
		else if(keystates[SDL_SCANCODE_D]) 
			characterDST.x += 1;

		if (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_z)
			clf::Sound::PauseChannel(3);

		if (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_x)
			clf::Sound::ResumeChannel(3);

		if (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_c)
			clf::Sound::PlayChannel(3, soundEffect, false);

		if (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_v)
			clf::Sound::PlayFadeInChannel(3, soundEffect, false, 3000);

		if (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_b)
			clf::Sound::FadeOutChannel(3, 3000);

		if (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_n)
			clf::Sound::PlayChannel(3, soundEffect, true);
		
		if (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_SPACE) {
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
		clf::Asset::FreeTexture(characterTexture);
		clf::Asset::FreeTexture(message);
		clf::Asset::FreeMusic(mainSound);
		clf::Asset::FreeChannel(3);
		clf::Asset::FreeSound(soundEffect);
	}
};

int main(int argc, char *args[]) {
	Game game;
	game.Build("Game", 600, 600, SDL_INIT_EVERYTHING, SDL_WINDOW_SHOWN);

	return 0;
}