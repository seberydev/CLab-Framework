#pragma once

#include "clf.h"

class Object {
public:
	Object() = default;
public:
	float GetDstW() const;
	float GetDstH() const;
	void OnStart(SDL_Texture* texture);
	void Draw();
private:
	clf::Utilities::Sprite sprite;
};
