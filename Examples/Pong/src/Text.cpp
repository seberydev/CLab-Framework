#include "Text.h"

const SDL_Rect& Text::GetPos() const { return pos; }

void Text::Init(const SDL_Color& color, const char* text, const char* filepath, int size, int style) {
	this->color = color;
	this->text = text;
	this->filepath = filepath;
	this->size = size;

	texture = clf::Asset::LoadText(filepath, size, text, color, 0, style);
	pos = { 0, 0, clf::Info::GetTextureWidth(texture), clf::Info::GetTextureHeight(texture) };
}

void Text::Draw() {
	clf::Render::DrawText(texture, pos);
}

void Text::SetPos(int x, int y) {
	pos.x = x;
	pos.y = y;
}

void Text::Finish() {
	clf::Asset::FreeTexture(texture);
}