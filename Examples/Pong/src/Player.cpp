#include "Player.h"

Player::Player() 
	: pos{ 0, 0, 20, 60 }, dir{ 0.0f }, speed{ 500.0f } {

}

void Player::Init(int x, const SDL_Color& color) {
	this->color = color;
	pos = { x, clf::Engine::ScreenHeight() / 2 - 35, 20, 70 };
}

void Player::Draw() {
	clf::Render::DrawFillRect(pos, color);
}

void Player::Move(float deltaTime) {
	pos.y += static_cast<int>(speed * deltaTime * dir);
}

void Player::SetDir(float dir) {
	this->dir = dir;
}
