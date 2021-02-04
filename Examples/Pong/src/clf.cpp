#include "clf.h"

// ----------------------------------------------------------------
// - Base clf::Engine Implementation                              -
// ----------------------------------------------------------------
SDL_Renderer* clf::Engine::renderer{ nullptr };
SDL_Window* clf::Engine::window{ nullptr };

int clf::Engine::screenWidth;
int clf::Engine::screenHeight; 

int clf::Engine::ScreenWidth() { return screenWidth; }
int clf::Engine::ScreenHeight() { return screenHeight; }

SDL_Renderer* clf::Engine::GetRenderer() { return renderer; }

//The core clf::Engine methods and structure
void clf::Engine::OnStart() {  }
void clf::Engine::OnInput(const Uint8* keystates) {  }
void clf::Engine::OnUpdate(float deltaTime) {  }
void clf::Engine::OnRender() {  }
void clf::Engine::OnFinish() {  }

bool clf::Engine::Build(const char* title, int screenWidth, int screenHeight, int subsystemFlags, int windowFlags) {
	if (!Initialize(title, screenWidth, screenHeight, subsystemFlags, windowFlags))
		return false;

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	OnStart();

	while (isRunning) {
		auto startTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();

		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT)
				isRunning = false;
		}

		const Uint8* keystates = SDL_GetKeyboardState(NULL);

		OnInput(keystates);
		OnUpdate(deltaTime);
		OnRender();

		SDL_RenderPresent(renderer);

		auto endTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();

		float elapsed = static_cast<float>((endTime - startTime) * 0.001);
		deltaTime = elapsed;
		deltaTime = (deltaTime > MAX_DELTA_TIME) ? MAX_DELTA_TIME : deltaTime;
	}

	OnFinish();

	//Unloads the SDL_Mixer library
	Mix_HaltMusic();
	Mix_CloseAudio();
	while (Mix_Init(0))
		Mix_Quit();

	//Unloads all SDL libraries
	IMG_Quit();
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();

	return true;
}

//Creates the window and renderer
bool clf::Engine::Initialize(const char* title, int screenWidth, int screenHeight, int subsystemFlags, int windowFlags) {
	if (SDL_Init(subsystemFlags) < 0 || !IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) || TTF_Init() == -1 || !Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
		return false;

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		return false;

	window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screenWidth,
		screenHeight,
		windowFlags);

	if (!window)
		return false;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!renderer)
		return false;

	isRunning = true;

	return true;
}

// ----------------------------------------------------------------
// - clf::Info Implementation                                     -
// ----------------------------------------------------------------
int clf::Info::GetTextureWidth(SDL_Texture* texture) {
	int width{ 0 };
	SDL_QueryTexture(texture, NULL, NULL, &width, nullptr);

	return width;
}

int clf::Info::GetTextureHeight(SDL_Texture* texture) {
	int height{ 0 };
	SDL_QueryTexture(texture, NULL, NULL, nullptr, &height);

	return height;
}

// ----------------------------------------------------------------
// - clf::Asset Implementation                                    -
// ----------------------------------------------------------------
SDL_Texture* clf::Asset::LoadSprite(const char* filepath) {
	//Fails if the clf::Asset is not PNG or JPG
	SDL_RWops* rwop{ SDL_RWFromFile(filepath, "rb") };
	if (!(IMG_isPNG(rwop) || IMG_isJPG(rwop)))
		return nullptr;

	SDL_Surface* temp{ IMG_Load(filepath) };
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(clf::Engine::GetRenderer(), temp) };

	SDL_FreeSurface(temp);
	temp = nullptr;
	SDL_RWclose(rwop);
	rwop = nullptr;

	return texture;
}

SDL_Texture* clf::Asset::LoadText(const char* filepath, int size, const char* text, const SDL_Color& color, int outline, int style) {
	TTF_Font* font{ TTF_OpenFont(filepath, size) };
	TTF_SetFontOutline(font, outline);
	TTF_SetFontStyle(font, style);
	SDL_Surface* temp{ TTF_RenderText_Blended(font, text, color) };
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(clf::Engine::GetRenderer(), temp) };

	SDL_FreeSurface(temp);
	temp = nullptr;
	TTF_CloseFont(font);
	font = nullptr;

	return texture;
}

SDL_Texture* clf::Asset::ChangeText(SDL_Texture* texture, const char* filepath, int size, const char* text, const SDL_Color& color, int outline, int style) {
	SDL_DestroyTexture(texture);
	return LoadText(filepath, size, text, color, outline, style);
}

Mix_Music* clf::Asset::LoadMusic(const char* filepath) {
	Mix_Music* music{ Mix_LoadMUS(filepath) };
	return music;
}

Mix_Chunk* clf::Asset::LoadSound(const char* filepath) {
	Mix_Chunk* sound{ Mix_LoadWAV(filepath) };
	return sound;
}

void clf::Asset::FreeTexture(SDL_Texture* texture) {
	SDL_DestroyTexture(texture);
	texture = nullptr;
}

void clf::Asset::FreeMusic(Mix_Music* music) {
	Mix_FreeMusic(music);
	music = nullptr;
}

void clf::Asset::FreeSound(Mix_Chunk* sound) {
	Mix_FreeChunk(sound);
	sound = nullptr;
}

void clf::Asset::FreeChannel(unsigned int channel) {
	Mix_HaltChannel(static_cast<int>(channel));
}

// ----------------------------------------------------------------
// - Render Implementation                                        -
// ----------------------------------------------------------------
void clf::Render::Clear(const SDL_Color& color) {
	SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderClear(clf::Engine::GetRenderer());
}

void clf::Render::DrawLine(const SDL_Point& start, const SDL_Point& end, const SDL_Color& color) {
	SDL_RenderDrawLine(clf::Engine::GetRenderer(), start.x, start.y, end.x, end.y);
}

void clf::Render::DrawFillRect(const SDL_Rect& destination, const SDL_Color& color) {
	SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(clf::Engine::GetRenderer(), &destination);
}

void clf::Render::DrawRect(const SDL_Rect& destination, const SDL_Color& color) {
	SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(clf::Engine::GetRenderer(), &destination);
}

void clf::Render::DrawCircle(const SDL_FPoint& topLeft, double radius, const SDL_Color& color) {
	const double PI = 3.1415926535;
	double x1{ 0.0 }, y1{ 0.0 };

	for (double angle{ 0.0 }; angle < 360.0; angle += 0.1) {
		x1 = radius * cos(angle * PI / 180.0);
		y1 = radius * sin(angle * PI / 180.0);

		SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
		SDL_RenderDrawPointF(clf::Engine::GetRenderer(), static_cast<float>(topLeft.x + x1), static_cast<float>(topLeft.y + y1));
	}
}

void clf::Render::DrawFillCircle(const SDL_Point& topLeft, int radius, const SDL_Color& color) {
	int maxX{ topLeft.x + (radius * 2) - 1 }, maxY{ topLeft.y + (radius * 2) - 1 };
	int squaredRadius{ radius * radius };
	int cX{ topLeft.x + radius }, cY{ topLeft.y + radius };

	for (int y{ topLeft.y + 1 }; y <= maxY; ++y) {
		for (int x{ topLeft.x + 1 }; x <= maxX; ++x) {
			int distance = ((x - cX) * (x - cX)) + ((y - cY) * (y - cY));
			if (distance <= squaredRadius) {
				SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
				SDL_RenderDrawPoint(clf::Engine::GetRenderer(), x, y);
			}
		}
	}
}

void clf::Render::DrawTriangle(const SDL_Point& v1, const SDL_Point& v2, const SDL_Point& v3, const SDL_Color& color) {
	SDL_Point vertices[4] = { v1, v2, v3, v1 };

	SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderDrawLines(clf::Engine::GetRenderer(), vertices, 4);
}

void clf::Render::DrawSprite(SDL_Texture* texture, const SDL_Rect& source, const SDL_Rect& destination) {
	DrawSpriteRot(texture, source, destination, 0.0, nullptr, SDL_FLIP_NONE);
}

void clf::Render::DrawSpriteRot(SDL_Texture* texture, const SDL_Rect& source, const SDL_Rect& destination, const double angle, const SDL_Point* center, const SDL_RendererFlip& flip) {
	SDL_RenderCopyEx(clf::Engine::GetRenderer(), texture, &source, &destination, angle, center, flip);
}

void clf::Render::DrawText(SDL_Texture* texture, const SDL_Rect& destination) {
	DrawTextRot(texture, destination, 0.0, nullptr, SDL_FLIP_NONE);
}

void clf::Render::DrawTextRot(SDL_Texture* texture, const SDL_Rect& destination, const double angle, const SDL_Point* center, const SDL_RendererFlip& flip) {
	SDL_RenderCopyEx(clf::Engine::GetRenderer(), texture, nullptr, &destination, angle, center, flip);
}

// ----------------------------------------------------------------
// - Sound Implementation		                                  -
// ----------------------------------------------------------------
//Music Methods
bool clf::Sound::IsPlayingMusic() {
	return Mix_PlayingMusic();
}

int clf::Sound::GetMusicVolume() {
	return Mix_VolumeMusic(-1);
}

void clf::Sound::SetMusicVolume(unsigned int volume) {
	Mix_VolumeMusic(static_cast<int>(volume));
}

void clf::Sound::PauseMusic() {
	if (IsPlayingMusic())
		Mix_PauseMusic();
}

void clf::Sound::ResumeMusic() {
	Mix_ResumeMusic();
}

void clf::Sound::PlayMusic(Mix_Music* music, int repeat) {
	Mix_PlayMusic(music, repeat);
}

void clf::Sound::PlayFadeInMusic(Mix_Music* music, int repeat, unsigned int miliseconds) {
	Mix_FadeInMusic(music, repeat, static_cast<int>(miliseconds));
}

void clf::Sound::FadeOutMusic(unsigned int miliseconds) {
	while (!Mix_FadeOutMusic(static_cast<int>(miliseconds)) && IsPlayingMusic()) {
		SDL_Delay(100);
	}
}

void clf::Sound::ChangeMusic(Mix_Music* newMusic, int repeat) {
	Mix_HaltMusic();
	PlayMusic(newMusic, repeat);
}

void clf::Sound::ChangeFadeInMusic(Mix_Music* newMusic, int repeat, unsigned int miliseconds) {
	Mix_HaltMusic();
	PlayFadeInMusic(newMusic, repeat, miliseconds);
}

void clf::Sound::ChangeFadeOutMusic(Mix_Music* newMusic, int repeat, unsigned int miliseconds) {
	FadeOutMusic(miliseconds);
	PlayMusic(newMusic, repeat);
}

void clf::Sound::ChangeFadeOutFadeInMusic(Mix_Music* newMusic, int repeat, unsigned int inMS, unsigned int outMS) {
	FadeOutMusic(outMS);
	PlayFadeInMusic(newMusic, repeat, inMS);
}

//Channel Methods
bool clf::Sound::IsPlayingChannel(int channel) {
	return Mix_Playing(channel);
}

int clf::Sound::GetChannelVolume(int channel) {
	return Mix_Volume(channel, -1);
}

void clf::Sound::SetChannelVolume(int channel, unsigned int volume) {
	Mix_Volume(channel, static_cast<int>(volume));
}

void clf::Sound::PauseChannel(int channel) {
	Mix_Pause(channel);
}

void clf::Sound::ResumeChannel(int channel) {
	Mix_Resume(channel);
}

void clf::Sound::PlayChannel(int channel, Mix_Chunk* sound, int repeat) {
	Mix_PlayChannel(channel, sound, repeat > 0 ? repeat - 1 : repeat);
}

void clf::Sound::PlayFadeInChannel(int channel, Mix_Chunk* sound, int repeat, unsigned int miliseconds) {
	Mix_HaltChannel(static_cast<int>(channel));
	Mix_FadeInChannel(channel, sound, repeat > 0 ? repeat - 1 : repeat, miliseconds);
}

void clf::Sound::FadeOutChannel(int channel, unsigned int miliseconds) {
	Mix_FadeOutChannel(channel, static_cast<int>(miliseconds));
}