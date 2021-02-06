#pragma once

#include "clf.h"

class Ball {
public:
	Ball() = default;
	~Ball() = default;
public:
	void Init(int radius, int speed, const SDL_Color& color);
	void Draw();
	void Move(float deltaTime, const SDL_Rect& p1, const SDL_Rect& p2);
	bool Reset();
	bool IsColliding(const SDL_Rect& rect) const;
	SDL_Rect GetCenter() const;
	void SetDir(int x, int y);
	void Finish();
private:
	SDL_Point pos;
	SDL_Point startPos;
	SDL_Rect dir;
	SDL_Rect minMaxPos;
	SDL_Color color;
	int radius;
	int speed;
	int diameter;
	Mix_Chunk* sound;
	int soundChannel;
};
