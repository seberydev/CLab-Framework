#include "Ball.h"
#include "Padding.h"

void Ball::Init(int radius, int speed, const SDL_Color& color) {
	this->radius = radius;
	this->color = color;
	this->speed = speed;
	diameter = radius * 2;
	dir = { -1, -1, 0, 0 };
	startPos = pos = { (clf::Engine::ScreenWidth() / 2 - radius), (clf::Engine::ScreenHeight() / 2 - radius) };
	
	//Evaluate (minX, minY, maxX, maxY)
	minMaxPos = { Padding::GetSize(), Padding::GetSize(), clf::Engine::ScreenWidth() - Padding::GetSize(), clf::Engine::ScreenHeight() - Padding::GetSize() };
}

void Ball::Draw() {
	clf::Render::DrawFillCircle(pos, radius, color);
}

void Ball::Move(float deltaTime, const SDL_Rect& p1, const SDL_Rect& p2) {
	pos.x += static_cast<int>(speed * deltaTime * dir.x);
	pos.y += static_cast<int>(speed * deltaTime * dir.y);

	//Check if the ball is out of bounds
	if (pos.y + diameter >= minMaxPos.h) {
		dir.y *= -1;
		pos.y = minMaxPos.h - diameter;
	}

	if (pos.y <= minMaxPos.y) {
		dir.y *= -1;
		pos.y = minMaxPos.y;
	}

	//Check Collision With Players
	if (IsColliding(p1)) {
		dir.x *= -1;
		pos.x = p1.x + p1.w;
	}

	if (IsColliding(p2)) {
		dir.x *= -1;
		pos.x = p2.x - diameter;
	}

	Reset();
}

void Ball::Reset() {
	if (pos.x + diameter >= minMaxPos.w || pos.x + diameter <= minMaxPos.x) {
		dir.x = 0;
		dir.y = 0;
		pos.x = startPos.x;
		pos.y = startPos.y;
	}
}

bool Ball::IsColliding(const SDL_Rect& rect) {
	return (
		rect.x + rect.w >= pos.x &&
		pos.x + diameter >= rect.x &&
		rect.y + rect.h >= pos.y &&
		pos.y + diameter >= rect.y);
}