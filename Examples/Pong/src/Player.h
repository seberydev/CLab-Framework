#pragma once

#include "clf.h"

class Player {
public:
	Player() = default;
	~Player() = default;
public:
	void Init(int x, int speed, const SDL_Color& color);
	void Draw();
	void Move(float deltaTime);
	void SetDir(int dir);
	const SDL_Rect& GetPos();
private:
	SDL_Rect pos;
	SDL_Color color;
	int dir;
	int speed;
	int maxY, minY;
};
