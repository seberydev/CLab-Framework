#include "Object.h"

float Object::GetDstW() const { return sprite.GetDstW(); }
float Object::GetDstH() const { return sprite.GetDstH(); }

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

