#include "Player.h"
#include "Padding.h"

const SDL_Rect& Player::GetPos() { return pos; }

void Player::Init(int x, int speed, const SDL_Color& color) {
	this->color = color;
	this->speed = speed;
	pos = { x, clf::Engine::ScreenHeight() / 2 - 35, 20, 70 };
	this->maxY = clf::Engine::ScreenHeight() - Padding::GetSize() - pos.h;
	this->minY = Padding::GetSize();
}

void Player::Draw() {
	clf::Render::DrawFillRect(pos, color);
}

void Player::Move(float deltaTime) {
	pos.y += static_cast<int>(speed * deltaTime * dir);

	if (pos.y > maxY)
		pos.y = maxY;

	if (pos.y < minY)
		pos.y = minY;
}

void Player::SetDir(int dir) {
	this->dir = dir;
}
