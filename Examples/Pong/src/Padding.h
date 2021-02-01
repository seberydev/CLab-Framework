#pragma once

#include "clf.h"

class Padding {
public:
	Padding() = default;
	~Padding() = default;
public:
	void Init(int size, const SDL_Color& color);
	void Draw();
	int GetSize();
private:
	SDL_Color color;
	SDL_Rect up, right, down, left;
	int size;
};
