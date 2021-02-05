#include "Player.h"
#include "Padding.h"

#include <iostream>

const SDL_Rect& Player::GetPos() const { return pos; }

void Player::Init(int x, int speed, const SDL_Color& color) {
	this->color = color;
	this->speed = speed;
	startPos = pos = { x, clf::Engine::ScreenHeight() / 2 - 35, 20, 70 };
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

void Player::SetDir(const Uint8* keystates, int upKey, int downKey) {
	if (keystates[upKey])
		dir = -1;
	else if (keystates[downKey])
		dir = 1;
	else
		dir = 0;
}

void Player::SetDirIA(int centerBallY) {
	if (centerBallY < pos.y) 
		dir = -1;
	else if (centerBallY > pos.y + pos.h)
		dir = 1;
	else
		dir = 0;
}

void Player::Reset() {
	dir = 0;
	pos.x = startPos.x;
	pos.y = startPos.y;
}


