#include "CLF.h"

SDL_Renderer* CLF::renderer{nullptr};

CLF::CLF() 
	: window{ nullptr },
	  isRunning{ false },
	  deltaTime{ 0.0f },
	  ticksLastFrame{ 0 }{

}

//Methods
SDL_Renderer* CLF::GetRenderer() { return renderer; }

bool CLF::Initialize(const std::string &title, int screen_width, int screen_height) {
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

void CLF::Build() {
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

void CLF::CalcDeltaTime() {
	int delay = Data::FRAME_TARGET - (SDL_GetTicks() - ticksLastFrame);

	if (delay > 0 && delay <= Data::FRAME_TARGET) SDL_Delay(delay);

	deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
	deltaTime = (deltaTime > Data::MAX_DELTA_TIME) ? Data::MAX_DELTA_TIME : deltaTime;

	ticksLastFrame = SDL_GetTicks();
}

void CLF::Clear(const Color& color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(renderer);
}
