#pragma once

//SDL Includes
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
//STL Includes
#include <string>
#include <assert.h>
#include <iostream>

namespace clf {
	// ----------------------------------------------------------------
	// - Base Engine Specification                                    -
	// ----------------------------------------------------------------
	class Engine {
	public:
		Engine() = default;
		virtual ~Engine() = default;
	public:
		void Build(const std::string& title, int screen_width, int screen_height, int subsystemFlags, int windowFlags);
		static SDL_Renderer* renderer;
	protected:
		virtual void OnStart();
		virtual void OnInput(const Uint8* keystates, const SDL_Event& events, int currentEvents);
		virtual void OnUpdate(float deltaTime);
		virtual void OnRender();
		virtual void OnFinish();
	private:
		bool Initialize(const std::string& title, int screen_width, int screen_height, int subsystemFlags, int windowFlags);
		void CalcDeltaTime();
		const float MAX_DELTA_TIME{ 0.05f };
		int ticksLastFrame{ 0 };
		float deltaTime{ 0.0f };
		bool isRunning{ false };
		SDL_Window* window{ nullptr };
		SDL_Event event;
	};
	
	// ----------------------------------------------------------------
	// - Base Engine Implementation                                   -
	// ----------------------------------------------------------------
	SDL_Renderer* Engine::renderer{nullptr};

	//The core engine methods and structure
	void Engine::OnStart() {  }
	void Engine::OnInput(const Uint8* keystates, const SDL_Event& events, int currentEvents) {  }
	void Engine::OnUpdate(float deltaTime) {  }
	void Engine::OnRender() {  }
	void Engine::OnFinish() {  }

	void Engine::Build(const std::string& title, int screen_width, int screen_height, int subsystemFlags, int windowFlags) {
		assert(Initialize(title, screen_width, screen_height, subsystemFlags, windowFlags));

		OnStart();

		while (isRunning) {
			CalcDeltaTime();			

			
			while (SDL_PollEvent(&event) != 0) {
				if (event.type == SDL_QUIT)
					isRunning = false;
			}

			const Uint8* keystates = SDL_GetKeyboardState(NULL);

			OnInput(keystates, event, 3);
			OnUpdate(deltaTime);
			OnRender();

			SDL_RenderPresent(renderer);
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
	}

	//Creates the window and renderer
	bool Engine::Initialize(const std::string& title, int screen_width, int screen_height, int subsystemFlags, int windowFlags) {
		if (SDL_Init(subsystemFlags) < 0 || !IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) || TTF_Init() == -1 || !Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
			return false;

		assert(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) != -1);

		window = SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			screen_width,
			screen_height,
			windowFlags);

		if (!window)
			return false;

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		if (!renderer)
			return false;

		isRunning = true;

		return true;
	}

	void Engine::CalcDeltaTime() {
		deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
		deltaTime = (deltaTime > MAX_DELTA_TIME) ? MAX_DELTA_TIME : deltaTime;

		ticksLastFrame = SDL_GetTicks();
	}

	// ----------------------------------------------------------------
	// - Info Specification                                           -
	// ----------------------------------------------------------------
	//Useful info about SDL classes
	class Info {
	private:
		Info() = default;
		~Info() = default;
	public:
		static int GetTextureWidth(SDL_Texture* texture);
		static int GetTextureHeight(SDL_Texture* texture);
	};

	// ----------------------------------------------------------------
	// - Info Implementation                                          -
	// ----------------------------------------------------------------
	int Info::GetTextureWidth(SDL_Texture* texture) {
		int width{ 0 };
		SDL_QueryTexture(texture, NULL, NULL, &width, nullptr);

		return width;
	}

	int Info::GetTextureHeight(SDL_Texture* texture) {
		int height{ 0 };
		SDL_QueryTexture(texture, NULL, NULL, nullptr, &height);

		return height;
	}

	// ----------------------------------------------------------------
	// - Asset Specification                                          -
	// ----------------------------------------------------------------
	class Asset {
	private:
		Asset() = default;
		~Asset() = default;
	public:
		static SDL_Texture* LoadSprite(const std::string& filepath);
		static SDL_Texture* LoadText(const std::string& filepath, int size, const std::string& text, const SDL_Color& color, int outline);
		static SDL_Texture* ChangeText(SDL_Texture* texture, const std::string& filepath, int size, const std::string& text, const SDL_Color& color, int outline);
		static Mix_Music* LoadMusic(const std::string& filepath);
		static Mix_Chunk* LoadSound(const std::string& filepath);
		static void FreeTexture(SDL_Texture* texture);
		static void FreeMusic(Mix_Music* music);
		static void FreeSound(Mix_Chunk* sound);
		static void FreeChannel(unsigned int channel);
	};

	// ----------------------------------------------------------------
	// - Asset Implementation                                         -
	// ----------------------------------------------------------------
	SDL_Texture* Asset::LoadSprite(const std::string& filepath) {
		//Fails if the asset is not PNG or JPG
		SDL_RWops* rwop{ SDL_RWFromFile(filepath.c_str(), "rb") };
		assert(IMG_isPNG(rwop) || IMG_isJPG(rwop));

		SDL_Surface* temp{ IMG_Load(filepath.c_str()) };
		SDL_Texture* texture{ SDL_CreateTextureFromSurface(clf::Engine::renderer, temp) };

		SDL_FreeSurface(temp);
		temp = nullptr;
		SDL_RWclose(rwop);
		rwop = nullptr;

		return texture;
	}

	SDL_Texture* Asset::LoadText(const std::string& filepath, int size, const std::string& text, const SDL_Color& color, int outline) {
		TTF_Font* font{ TTF_OpenFont(filepath.c_str(), size) };
		TTF_SetFontOutline(font, outline);
		SDL_Surface* temp{ TTF_RenderText_Blended(font, text.c_str(), color) };
		SDL_Texture* texture{ SDL_CreateTextureFromSurface(clf::Engine::renderer, temp) };
		
		assert(texture);
		
		SDL_FreeSurface(temp);
		temp = nullptr;
		TTF_CloseFont(font);
		font = nullptr;

		return texture;
	}

	SDL_Texture* Asset::ChangeText(SDL_Texture* texture, const std::string& filepath, int size, const std::string& text, const SDL_Color& color, int outline) {
		SDL_DestroyTexture(texture);
		return LoadText(filepath, size, text, color, outline);
	}

	Mix_Music* Asset::LoadMusic(const std::string& filepath) {
		Mix_Music* music{ Mix_LoadMUS(filepath.c_str()) };
		assert(music);
		return music;
	}

	Mix_Chunk* Asset::LoadSound(const std::string& filepath) {
		Mix_Chunk* sound{ Mix_LoadWAV(filepath.c_str()) };
		assert(sound);
		
		return sound;
	}

	void Asset::FreeTexture(SDL_Texture* texture) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}

	void Asset::FreeMusic(Mix_Music* music) {
		Mix_FreeMusic(music);
		music = nullptr;
	}

	void Asset::FreeSound(Mix_Chunk* sound) {
		Mix_FreeChunk(sound);
		sound = nullptr;
	}

	void Asset::FreeChannel(unsigned int channel) {
		Mix_HaltChannel(static_cast<int>(channel));
	}

	// ----------------------------------------------------------------
	// - Draw Specification		                                      -
	// ----------------------------------------------------------------
	class Draw {
	private:
		Draw() = default;
		~Draw() = default;
	public:
		static void Clear(const SDL_Color& color);
		static void DrawTriangle(const SDL_Point& v1, const SDL_Point& v2, const SDL_Point& v3, const SDL_Color& color);
		static void DrawFillRect(const SDL_Rect& destination, const SDL_Color& color);
		static void DrawRect(const SDL_Rect& destination, const SDL_Color& color);
		static void DrawCircle(const SDL_FPoint& point, double radius, const SDL_Color& color);
		static void DrawSprite(SDL_Texture* texture, const SDL_Rect& source, const SDL_Rect& destination);
		static void DrawText(SDL_Texture* texture, const SDL_Rect& destination);
	};

	// ----------------------------------------------------------------
	// - Draw Implementation                                          -
	// ----------------------------------------------------------------
	void Draw::Clear(const SDL_Color& color) {
		SDL_SetRenderDrawColor(clf::Engine::renderer, color.r, color.g, color.b, color.a);
		SDL_RenderClear(clf::Engine::renderer);
	}

	void Draw::DrawFillRect(const SDL_Rect& destination, const SDL_Color& color) {
		SDL_SetRenderDrawColor(clf::Engine::renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(clf::Engine::renderer, &destination);
	}

	void Draw::DrawRect(const SDL_Rect& destination, const SDL_Color& color) {
		SDL_SetRenderDrawColor(clf::Engine::renderer, color.r, color.g, color.b, color.a);
		SDL_RenderDrawRect(clf::Engine::renderer, &destination);
	}

	void Draw::DrawCircle(const SDL_FPoint& point, double radius, const SDL_Color& color) {
		const double PI = 3.1415926535;
		double x1{ 0.0 }, y1{ 0.0 };
		
		for (double angle{ 0.0 }; angle < 360.0; angle += 0.1) {
			x1 = radius * cos(angle * PI / 180.0);
			y1 = radius * sin(angle * PI / 180.0);

			SDL_SetRenderDrawColor(clf::Engine::renderer, color.r, color.g, color.b, color.a);
			SDL_RenderDrawPointF(clf::Engine::renderer, static_cast<float>(point.x + x1), static_cast<float>(point.y + y1));
		}
	}

	void Draw::DrawTriangle(const SDL_Point& v1, const SDL_Point& v2, const SDL_Point& v3, const SDL_Color& color) {
		SDL_Point vertices[4] = {v1, v2, v3, v1};

		SDL_SetRenderDrawColor(clf::Engine::renderer, color.r, color.g, color.b, color.a);
		SDL_RenderDrawLines(clf::Engine::renderer, vertices, 4);
	}

	void Draw::DrawSprite(SDL_Texture* texture, const SDL_Rect& source, const SDL_Rect& destination) {
		SDL_RenderCopy(clf::Engine::renderer, texture, &source, &destination);
	}

	void Draw::DrawText(SDL_Texture* texture, const SDL_Rect& destination) {
		SDL_RenderCopy(clf::Engine::renderer, texture, nullptr, &destination);
	}

	// ----------------------------------------------------------------
	// - Sound Specification		                                  -
	// ----------------------------------------------------------------
	class Sound {
	private:
		Sound() = default;
		~Sound() = default;
	public:
		//Music Methods
		static bool IsPlayingMusic(); 
		static int GetMusicVolume();
		static void SetMusicVolume(unsigned int volume); 
		static void PauseMusic(); 
		static void ResumeMusic(); 
		static void PlayMusic(Mix_Music* music, int repeat); 
		static void PlayFadeInMusic(Mix_Music* music, int repeat, unsigned int miliseconds); //
		static void FadeOutMusic(unsigned int miliseconds); 
		static void ChangeMusic(Mix_Music* newMusic, int repeat);
		static void ChangeFadeInMusic(Mix_Music* newMusic, int repeat, unsigned int miliseconds);
		static void ChangeFadeOutMusic(Mix_Music* newMusic, int repeat, unsigned int miliseconds);
		static void ChangeFadeOutFadeInMusic(Mix_Music* newMusic, int repeat, unsigned int inMS, unsigned int outMS);
		//Channel Methods (For sound effects)
		static bool IsPlayingChannel(int channel);
		static int GetChannelVolume(int channel);
		static void SetChannelVolume(int channel, unsigned int volume);
		static void PauseChannel(int channel);
		static void ResumeChannel(int channel);
		static void PlayChannel(int channel, Mix_Chunk* sound, int repeat);
		static void PlayFadeInChannel(int channel, Mix_Chunk* sound, int repeat, unsigned int miliseconds);
		static void FadeOutChannel(int channel, unsigned int miliseconds);
	};

	// ----------------------------------------------------------------
	// - Sound Implementation		                                  -
	// ----------------------------------------------------------------
	//Music Methods
	bool Sound::IsPlayingMusic() {
		return Mix_PlayingMusic();
	}

	int Sound::GetMusicVolume() {
		return Mix_VolumeMusic(-1);
	}

	void Sound::SetMusicVolume(unsigned int volume) {
		Mix_VolumeMusic(static_cast<int>(volume));
	}

	void Sound::PauseMusic() {
		if (IsPlayingMusic())
			Mix_PauseMusic();
	}

	void Sound::ResumeMusic() {
		Mix_ResumeMusic();
	}
	
	void Sound::PlayMusic(Mix_Music* music, int repeat) {
		assert(Mix_PlayMusic(music, repeat) != -1);
	}

	void Sound::PlayFadeInMusic(Mix_Music* music, int repeat, unsigned int miliseconds) {
		assert(Mix_FadeInMusic(music, repeat, static_cast<int>(miliseconds)) != -1);
	}

	void Sound::FadeOutMusic(unsigned int miliseconds) {
		while (!Mix_FadeOutMusic(static_cast<int>(miliseconds)) && IsPlayingMusic()) {
			SDL_Delay(100);
		}
	}

	void Sound::ChangeMusic(Mix_Music* newMusic, int repeat) {
		Mix_HaltMusic();
		PlayMusic(newMusic, repeat);
	}

	void Sound::ChangeFadeInMusic(Mix_Music* newMusic, int repeat, unsigned int miliseconds) {
		Mix_HaltMusic();
		PlayFadeInMusic(newMusic, repeat, miliseconds);
	}

	void Sound::ChangeFadeOutMusic(Mix_Music* newMusic, int repeat, unsigned int miliseconds) {
		FadeOutMusic(miliseconds);
		PlayMusic(newMusic, repeat);
	}

	void Sound::ChangeFadeOutFadeInMusic(Mix_Music* newMusic, int repeat, unsigned int inMS, unsigned int outMS) {
		FadeOutMusic(outMS);
		PlayFadeInMusic(newMusic, repeat, inMS);
	}

	//Channel Methods
	bool Sound::IsPlayingChannel(int channel) {
		return Mix_Playing(channel);
	}

	int Sound::GetChannelVolume(int channel) {
		return Mix_Volume(channel, -1);
	}

	void Sound::SetChannelVolume(int channel, unsigned int volume) {
		Mix_Volume(channel, static_cast<int>(volume));
	}

	void Sound::PauseChannel(int channel) {
		Mix_Pause(channel);
	}

	void Sound::ResumeChannel(int channel) {
		Mix_Resume(channel);
	}

	void Sound::PlayChannel(int channel, Mix_Chunk* sound, int repeat) {
		assert(Mix_PlayChannel(channel, sound, repeat > 0 ? repeat - 1 : repeat) != -1);
	}

	void Sound::PlayFadeInChannel(int channel, Mix_Chunk* sound, int repeat, unsigned int miliseconds) {
		Mix_HaltChannel(static_cast<int>(channel));
		assert(Mix_FadeInChannel(channel, sound, repeat > 0 ? repeat - 1 : repeat, miliseconds) != -1);
	}

	void Sound::FadeOutChannel(int channel, unsigned int miliseconds) {
		Mix_FadeOutChannel(channel, static_cast<int>(miliseconds));
	}

}

