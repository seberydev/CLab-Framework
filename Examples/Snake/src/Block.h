#pragma once

#include "clf.h"
#include "Map.h"

class Block {
public:
	Block();
	~Block() = default;
public:
	void Init(SDL_Texture *texture);
	void Draw();
	void Finish();
	void SetPos(int x, int y);
	int GetPosX();
	int GetPosY();
	void SetDir(int x, int y);
	int GetDirX();
	int GetDirY();
	void SetAngle(double angle);
	double GetAngle();
private:
	static constexpr SDL_Rect src{ 0, 0, 40, 40 };
	SDL_Texture* texture;
	SDL_Rect pos;
	SDL_Rect dir;
	double angle;
	SDL_Rect lastDir;
};
