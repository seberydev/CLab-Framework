#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <assert.h>
#include <iostream>
#include <map>

namespace clf {
	// ----------------------------------------------------------------
	// - Base Engine Specification                                    -
	// ----------------------------------------------------------------
	class Engine {
	public:
		Engine() = default;
		virtual ~Engine() = default;
	public:
		static SDL_Renderer* renderer;
		bool Initialize(const std::string& title, int screen_width, int screen_height);
		void Build();
	protected:
		virtual void OnStart() = 0;
		virtual void OnInput() = 0;
		virtual void OnUpdate(float deltaTime) = 0;
		virtual void OnRender() = 0;
		virtual void OnFinish() = 0;
	private:
		const int FPS{ 60 };
		const int FRAME_TARGET{ 1000 / FPS };
		const float MAX_DELTA_TIME{ 0.05f };
		SDL_Window* window{ nullptr };
		SDL_Event event;
		bool isRunning{ false };
		float deltaTime{ 0.0f };
		int ticksLastFrame{ 0 };
		int delay{ 0 };
	private:
		void CalcDeltaTime();
	};

	// ----------------------------------------------------------------
	// - Base Engine Implementation                                   -
	// ----------------------------------------------------------------
	SDL_Renderer* Engine::renderer{nullptr};

	bool Engine::Initialize(const std::string& title, int screen_width, int screen_height) {	
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0 || !IMG_Init(IMG_INIT_PNG))
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

	void Engine::Build() {
		OnStart();

		while (isRunning) {
			CalcDeltaTime();

			while (SDL_PollEvent(&event) != 0) {
				if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
					isRunning = false;
			}

			const Uint8* keystates = SDL_GetKeyboardState(NULL);

			OnInput();
			OnUpdate(deltaTime);
			OnRender();

			SDL_RenderPresent(renderer);
		}

		OnFinish();

		IMG_Quit();
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		SDL_Quit();
	}

	void Engine::CalcDeltaTime() {
		int delay = FRAME_TARGET - (SDL_GetTicks() - ticksLastFrame);

		if (delay > 0 && delay <= FRAME_TARGET) SDL_Delay(delay);

		deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
		deltaTime = (deltaTime > MAX_DELTA_TIME) ? MAX_DELTA_TIME : deltaTime;

		ticksLastFrame = SDL_GetTicks();
	}

	// ----------------------------------------------------------------
	// - Color Palette Specification                                  -
	// ----------------------------------------------------------------
	class Color {
	public:
		Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
		~Color() = default;
	public:
		static const Color METALLIC_BRONZE,
			HOT_PINK,
			POMEGRANATE,
			MALIBU,
			PORTAFINO,
			SCREAMIN_GREEN,
			TURQUOISE,
			ROYAL_BLUE;
	public:
		Uint8 R() const;
		Uint8 G() const;
		Uint8 B() const;
		Uint8 A() const;
	private:
		Uint8 r, g, b, a;
	};

	// ----------------------------------------------------------------
	// - Color Palette Implementation                                 -
	// ----------------------------------------------------------------
	Color const Color::METALLIC_BRONZE{ 76, 57, 26, 255 },
		Color::HOT_PINK{ 255, 102, 209, 255 },
		Color::POMEGRANATE{ 239, 52, 52, 255 },
		Color::MALIBU{ 94, 150, 255, 255 },
		Color::PORTAFINO{ 255, 249, 186, 255 },
		Color::SCREAMIN_GREEN{ 87, 249, 84, 255 },
		Color::TURQUOISE{ 68, 229, 162, 255 },
		Color::ROYAL_BLUE{ 114, 66, 237, 255 };

	Color::Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
		: r{ red }, g{ green }, b{ blue }, a{ alpha } {

	}

	Uint8 Color::R() const { return r; }
	Uint8 Color::G() const { return g; }
	Uint8 Color::B() const { return b; }
	Uint8 Color::A() const { return a; }

	// ----------------------------------------------------------------
	// - Draw Specification		                                      -
	// ----------------------------------------------------------------
	struct Draw {
	public:
		static void Clear(const clf::Color& color);
		static void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Color& color);
		static void DrawSquare(int x, int y, int size, const Color& color);
		static void DrawRectangle(int x, int y, int width, int height, const Color& color);
		static void DrawCircle(double x, double y, double radius, const Color& color);
	private:
		Draw() = default;
		~Draw() = default;
	};

	// ----------------------------------------------------------------
	// - Draw Implementation		                                  -
	// ----------------------------------------------------------------
	void Draw::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Color& color) {
		SDL_Point points[4] = {
			{x1, y1},
			{x2, y2},
			{x3, y3},
			{x1, y1}
		};

		SDL_SetRenderDrawColor(clf::Engine::renderer, color.R(), color.G(), color.B(), color.A());
		SDL_RenderDrawLines(clf::Engine::renderer, points, 4);
	}

	void Draw::DrawSquare(int x, int y, int size, const Color& color) {
		SDL_SetRenderDrawColor(clf::Engine::renderer, color.R(), color.G(), color.B(), color.A());
		SDL_Rect rect{ x, y, size, size };
		SDL_RenderDrawRect(clf::Engine::renderer, &rect);
	}

	void Draw::DrawRectangle(int x, int y, int width, int height, const Color& color) {
		SDL_SetRenderDrawColor(clf::Engine::renderer, color.R(), color.G(), color.B(), color.A());
		SDL_Rect rect{ x, y, width, height };
		SDL_RenderDrawRect(clf::Engine::renderer, &rect);
	}

	void Draw::DrawCircle(double x, double y, double radius, const Color& color) {
		const double PI = 3.1415926535;
		double x1{ 0.0 }, y1{ 0.0 };

		for (double angle{ 0.0 }; angle < 360.0; angle += 0.1) {
			x1 = radius * cos(angle * PI / 180.0);
			y1 = radius * sin(angle * PI / 180.0);

			SDL_SetRenderDrawColor(clf::Engine::renderer, color.R(), color.G(), color.B(), color.A());
			SDL_RenderDrawPointF(clf::Engine::renderer, static_cast<float>(x + x1), static_cast<float>(y + y1));
		}
	}

	void Draw::Clear(const clf::Color& color) {
		SDL_SetRenderDrawColor(clf::Engine::renderer, color.R(), color.G(), color.B(), color.A());
		SDL_RenderClear(clf::Engine::renderer);
	}

	// ----------------------------------------------------------------
	// - Custom Asset Specification		                              -
	// ----------------------------------------------------------------
	class CustomAsset {
	public:
		CustomAsset(int width, int height, const std::string &filepath, const std::string &name);
		~CustomAsset() = default;
		int Width() const;
		int Height() const;
		const std::string& Filepath() const;
		const std::string& Name() const;
	private:
		int width;
		int height;
		std::string filepath;
		std::string name;
	};

	// ----------------------------------------------------------------
	// - Custom Asset Specification		                              -
	// ----------------------------------------------------------------
	CustomAsset::CustomAsset(int width, int height, const std::string& filepath, const std::string& name)
		: width{ width }, height{ height }, filepath{ filepath }, name{ name } {}

	int CustomAsset::Width() const { return width; }
	int CustomAsset::Height() const { return height; }
	const std::string& CustomAsset::Filepath() const { return filepath; }
	const std::string& CustomAsset::Name() const { return name; };

	// ----------------------------------------------------------------
	// - Asset Specification		                                  -
	// ----------------------------------------------------------------
	class Asset {
	public:
		static void LoadPNG(int width, int height, const std::string& filepath, const std::string& name);
		static void LoadPNG(const CustomAsset &customAsset);
	private:
		Asset() = default;
		~Asset() = default;
	private:
		static std::map<std::string, SDL_Texture*> textures;
		static std::map<std::string, CustomAsset> customAssets;
	};

	// ----------------------------------------------------------------
	// - Asset Class Implementation		                              -
	// ----------------------------------------------------------------
	std::map<std::string, SDL_Texture*> Asset::textures;
	std::map<std::string, CustomAsset> Asset::customAssets;

	void Asset::LoadPNG(int width, int height, const std::string& filepath, const std::string& name) {
		assert(textures.count(name) == 0);
		SDL_RWops* rwop{ SDL_RWFromFile(filepath.c_str(), "rb") };
		assert(IMG_isPNG(rwop));

		SDL_Surface* temp{ IMG_Load(filepath.c_str()) };
		customAssets.insert(std::pair<std::string, CustomAsset>(name, { width, height, filepath, name }));
		textures.insert(std::pair<std::string, SDL_Texture*>(name, SDL_CreateTextureFromSurface(clf::Engine::renderer, temp)));

		SDL_FreeSurface(temp);
		temp = nullptr;
		SDL_RWclose(rwop);
		rwop = nullptr;

		std::cout << "Texture Created!\n";
	}

	void Asset::LoadPNG(const CustomAsset& customAsset) {
		LoadPNG(customAsset.Width(), customAsset.Height(), customAsset.Filepath(), customAsset.Name());
	}

}

