#pragma once

#include "clf.h"

class Padding {
public:
	Padding() = default;
	~Padding() = default;
public:
	static int GetSize();
	void Init(int size, const SDL_Color& color);
	void Draw();
private:
	static int size;
	SDL_Color color;
	SDL_Rect up, right, down, left;
};
