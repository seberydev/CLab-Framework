#pragma once

#include "clf.h"

class Item {
public:
	Item();
	~Item() = default;
public:
	void Init(int x, int y);
	void Draw();
	void Finish();
	bool IsColliding(const SDL_Rect& rect) const;
	void NewPos(int x, int y);
private:
	static constexpr SDL_Rect src{ 0, 0, 40, 40 };
	SDL_Rect pos;
	SDL_Texture* texture;
	int tileSize;
};
