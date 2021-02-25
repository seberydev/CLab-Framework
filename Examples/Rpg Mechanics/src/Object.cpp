#include "Object.h"

float Object::GetDstW() const { return sprite.GetDstW(); }
float Object::GetDstH() const { return sprite.GetDstH(); }
void Object::SetDstX(float x) { sprite.SetDstX(x); }
void Object::SetDstY(float y) { sprite.SetDstY(y); }
const SDL_FRect& Object::GetPos() const { return sprite.GetDst(); }
bool Object::IsActive() const { return isActive; }
void Object::IsActive(bool isActive) { this->isActive = isActive; }

void Object::OnStart(SDL_Texture* texture) {
	sprite.OnStart(
		0, 0, 16, 16,
		0, 0, 32, 32,
		nullptr,
		SDL_FLIP_NONE,
		0.0);

	sprite.SetTexture(texture);
}

void Object::Draw() {
	sprite.Draw();
}

