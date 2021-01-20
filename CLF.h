#pragma once

//SDL Includes
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
//STL Includes
#include <string>
#include <assert.h>

namespace clf {
	// ----------------------------------------------------------------
	// - Base Engine Specification                                    -
	// ----------------------------------------------------------------
	class Engine {
	public:
		Engine() = default;
		virtual ~Engine() = default;
	public:
		void Build(const std::string& title, int screen_width, int screen_height);
		static SDL_Renderer* renderer;
	protected:
		virtual void OnStart() = 0;
		virtual void OnInput(const Uint8* keystates) = 0;
		virtual void OnUpdate(float deltaTime) = 0;
		virtual void OnRender() = 0;
		virtual void OnFinish() = 0;
	private:
		bool Initialize(const std::string& title, int screen_width, int screen_height);
		void CalcDeltaTime();
		const int FPS{ 60 };
		const int FRAME_TARGET{ 1000 / FPS };
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

	void Engine::Build(const std::string& title, int screen_width, int screen_height) {
		assert(Initialize(title, screen_width, screen_height));

		OnStart();

		while (isRunning) {
			CalcDeltaTime();

			while (SDL_PollEvent(&event) != 0) {
				if (event.type == SDL_QUIT)
					isRunning = false;
			}

			const Uint8* keystates = SDL_GetKeyboardState(NULL);

			OnInput(keystates);
			OnUpdate(deltaTime);
			OnRender();

			SDL_RenderPresent(renderer);
		}

		OnFinish();

		IMG_Quit();
		TTF_Quit();
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		SDL_Quit();
	}

	bool Engine::Initialize(const std::string& title, int screen_width, int screen_height) {
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0 || !IMG_Init(IMG_INIT_PNG) || TTF_Init() == -1)
			return false;

		window = SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			screen_width,
			screen_height,
			SDL_WINDOW_SHOWN);

		if (!window)
			return false;

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		if (!renderer)
			return false;

		isRunning = true;

		return true;
	}

	void Engine::CalcDeltaTime() {
		int delay = FRAME_TARGET - (SDL_GetTicks() - ticksLastFrame);

		if (delay > 0 && delay <= FRAME_TARGET) SDL_Delay(delay);

		deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
		deltaTime = (deltaTime > MAX_DELTA_TIME) ? MAX_DELTA_TIME : deltaTime;

		ticksLastFrame = SDL_GetTicks();
	}

	// ----------------------------------------------------------------
	// - Asset Specification                                          -
	// ----------------------------------------------------------------
	class Asset {
	public:
		Asset() = delete;
		~Asset() = delete;
	public:
		static SDL_Texture* LoadPNG(const std::string& filepath);
	};

	// ----------------------------------------------------------------
	// - Asset Implementation                                         -
	// ----------------------------------------------------------------
	SDL_Texture* Asset::LoadPNG(const std::string& filepath) {
		//Fails if the asset is not png
		SDL_RWops* rwop{ SDL_RWFromFile(filepath.c_str(), "rb") };
		assert(IMG_isPNG(rwop));

		SDL_Surface* temp{ IMG_Load(filepath.c_str()) };
		SDL_Texture* texture{ SDL_CreateTextureFromSurface(clf::Engine::renderer, temp) };

		SDL_FreeSurface(temp);
		temp = nullptr;
		SDL_RWclose(rwop);
		rwop = nullptr;

		return texture;
	}

	// ----------------------------------------------------------------
	// - Sprite Specification		                                  -
	// ----------------------------------------------------------------
	struct Sprite {
	public:
		Sprite(const SDL_Rect& source, const SDL_Rect& destination);
		~Sprite() = default;
	public:
		SDL_Rect src;
		SDL_Rect dst;
	};

	// ----------------------------------------------------------------
	// - Sprite Implementation		                                  -
	// ----------------------------------------------------------------
	Sprite::Sprite(const SDL_Rect& source, const SDL_Rect& destination)
		: src{ source }, dst{ destination } {}

	// ----------------------------------------------------------------
	// - Draw Specification		                                      -
	// ----------------------------------------------------------------
	class Draw {
	public:
		Draw() = delete;
		~Draw() = delete;
	public:
		static void Clear(const SDL_Color& color);
		static void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const SDL_Color& color);
		static void DrawRect(const SDL_Rect& destination, const SDL_Color& color);
		static void DrawCircle(double x, double y, double radius, const SDL_Color& color);
		static void DrawSprite(SDL_Texture* texture, Sprite& sprite);
	};

	// ----------------------------------------------------------------
	// - Draw Implementation                                          -
	// ----------------------------------------------------------------
	void Draw::Clear(const SDL_Color& color) {
		SDL_SetRenderDrawColor(clf::Engine::renderer, color.r, color.g, color.b, color.a);
		SDL_RenderClear(clf::Engine::renderer);
	}

	void Draw::DrawRect(const SDL_Rect& destination, const SDL_Color& color) {
		SDL_SetRenderDrawColor(clf::Engine::renderer, color.r, color.g, color.b, color.a);
		SDL_RenderDrawRect(clf::Engine::renderer, &destination);
	}

	void Draw::DrawCircle(double x, double y, double radius, const SDL_Color& color) {
		const double PI = 3.1415926535;
		double x1{ 0.0 }, y1{ 0.0 };

		for (double angle{ 0.0 }; angle < 360.0; angle += 0.1) {
			x1 = radius * cos(angle * PI / 180.0);
			y1 = radius * sin(angle * PI / 180.0);

			SDL_SetRenderDrawColor(clf::Engine::renderer, color.r, color.g, color.b, color.a);
			SDL_RenderDrawPointF(clf::Engine::renderer, static_cast<float>(x + x1), static_cast<float>(y + y1));
		}
	}

	void Draw::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const SDL_Color& color) {
		SDL_Point points[4] = {
			{x1, y1},
			{x2, y2},
			{x3, y3},
			{x1, y1}
		};

		SDL_SetRenderDrawColor(clf::Engine::renderer, color.r, color.g, color.b, color.a);
		SDL_RenderDrawLines(clf::Engine::renderer, points, 4);
	}

	void Draw::DrawSprite(SDL_Texture* texture, Sprite& sprite) {
		SDL_RenderCopy(clf::Engine::renderer, texture, &sprite.src, &sprite.dst);
	}


}

