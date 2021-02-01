#pragma once

//SDL Libraries
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
//Utilities
#define NDEBUG
#include <assert.h>
#include <string>
#include <chrono>

namespace clf {
	// ----------------------------------------------------------------
	// - Engine Specification                                    -
	// ----------------------------------------------------------------
	class Engine {
	public:
		Engine() = default;
		virtual ~Engine() = default;
	public:
		void Build(const std::string& title, int screen_width, int screen_height, int subsystemFlags, int windowFlags);
		static int ScreenWidth();
		static int ScreenHeight();
		static SDL_Renderer* renderer;
	protected:
		virtual void OnStart();
		virtual void OnInput(const Uint8* keystates);
		virtual void OnUpdate(float deltaTime);
		virtual void OnRender();
		virtual void OnFinish();
	private:
		bool Initialize(const std::string& title, int screen_width, int screen_height, int subsystemFlags, int windowFlags);
		static SDL_Window* window;
		const float MAX_DELTA_TIME{ 0.02f };
		float deltaTime{ 0.0f };
		bool isRunning{ false };
		static int screenWidth;
		static int screenHeight;
		SDL_Event event;
	};

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
	// - Color Constants		                                  -
	// ----------------------------------------------------------------
	class Color {
	private:
		Color() = default;
		~Color() = default;
	public:
		static constexpr SDL_Color METALLIC_BRONZE{ 76, 57, 26, 255 };
		static constexpr SDL_Color HOT_PINK{ 255, 102, 209, 255 };
		static constexpr SDL_Color POMEGRANATE{ 239, 52, 52, 255 };
		static constexpr SDL_Color MALIBU{ 94, 150, 255, 255 };
		static constexpr SDL_Color PORTAFINO{ 255, 249, 186, 255 };
		static constexpr SDL_Color SCREAMIN_GREEN{ 87, 249, 84, 255 };
		static constexpr SDL_Color TURQUOISE{ 68, 229, 162, 255 };
		static constexpr SDL_Color ROYAL_BLUE{ 114, 66, 237, 255 };
	};

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
	// - Render Specification		                                  -
	// ----------------------------------------------------------------
	class Render {
	private:
		Render() = default;
		~Render() = default;
	public:
		static void Clear(const SDL_Color& color);
		static void DrawTriangle(const SDL_Point& v1, const SDL_Point& v2, const SDL_Point& v3, const SDL_Color& color);
		static void DrawFillRect(const SDL_Rect& destination, const SDL_Color& color);
		static void DrawRect(const SDL_Rect& destination, const SDL_Color& color);
		static void DrawCircle(const SDL_FPoint& topLeft, double radius, const SDL_Color& color);
		static void DrawFillCircle(const SDL_Point& topLeft, int radius, const SDL_Color& color);
		static void DrawSprite(SDL_Texture* texture, const SDL_Rect& source, const SDL_Rect& destination);
		static void DrawSpriteRot(SDL_Texture* texture, const SDL_Rect& source, const SDL_Rect& destination, const double angle, const SDL_Point* center, const SDL_RendererFlip& flip);
		static void DrawText(SDL_Texture* texture, const SDL_Rect& destination);
		static void DrawTextRot(SDL_Texture* texture, const SDL_Rect& destination, const double angle, const SDL_Point* center, const SDL_RendererFlip& flip);
	};

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
		
}


