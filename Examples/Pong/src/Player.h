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
	void SetDir(const Uint8* keystates, int upKey, int downKey);
	void SetDirIA(int centerBallY);
	void Reset();
	const SDL_Rect& GetPos() const;
private:
	SDL_Rect pos;
	SDL_Rect startPos;
	SDL_Color color;
	int dir;
	int speed;
	int maxY, minY;
};
