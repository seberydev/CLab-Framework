#pragma once

#include "clf.h"

class Player {
public:
	Player();
	~Player() = default;
public:
	void Init(int x, const SDL_Color& color);
	void Draw();
	void Move(float deltaTime);
	void SetDir(float dir);
private:
	SDL_Rect pos;
	SDL_Color color;
	float dir;
	float speed;
};
