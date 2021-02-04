#pragma once

#include "clf.h"

class Text {
public:
	Text() = default;
	~Text();
public:
	void Init(const SDL_Color& color, const char* text, const char* filepath, int size, int style);
	void Draw();
	void SetPos(int x, int y);
	const SDL_Rect& GetPos() const;
private:
	SDL_Texture* texture;
	SDL_Color color;
	SDL_Rect pos;
	const char* text;
	const char* filepath;
	int size;
	int style;
};
