#include "Padding.h"

int Padding::GetSize() { return size; }

int Padding::size;

void Padding::Init(int size, const SDL_Color& color) {
	this->size = size;
	this->color = color;

	up = { 0, 0, clf::Engine::ScreenWidth(), size };
	right = { clf::Engine::ScreenWidth() - size, 0, size, clf::Engine::ScreenHeight() };
	down = { 0, clf::Engine::ScreenHeight() - size, clf::Engine::ScreenWidth(), size };
	left = { 0, 0, size, clf::Engine::ScreenHeight() };
}

void Padding::Draw() {
	//Draw Middle Line
	clf::Render::DrawFillRect({ clf::Engine::ScreenWidth() / 2 - (size / 2), 0, size, clf::Engine::ScreenHeight() }, color);
	
	//Draw Padding
	clf::Render::DrawFillRect(up, color);
	clf::Render::DrawFillRect(right, color);
	clf::Render::DrawFillRect(down, color);
	clf::Render::DrawFillRect(left, color);
}