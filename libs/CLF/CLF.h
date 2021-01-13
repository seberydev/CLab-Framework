#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "Data.h"
#include "interfaces/Clear.h"

class CLF {
public:
	CLF();
	virtual ~CLF() = default;
public:
	static SDL_Renderer* GetRenderer();
	bool Initialize(const std::string &title, int screen_width, int screen_height);
	void Build();
protected:
	virtual void OnStart() = 0;
	virtual void OnInput() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnRender() = 0;
	virtual void OnFinish() = 0;
private:
	static SDL_Renderer* renderer;
	SDL_Window* window;
	bool isRunning;
	float deltaTime;
	SDL_Event event;
	int ticksLastFrame;
	void CalcDeltaTime();
};
