#pragma once

#include "clf.h"

class Object {
public:
	Object() = default;
public:
	float GetDstW() const;
	float GetDstH() const;
	void SetDstX(float x);
	void SetDstY(float y);
	const SDL_FRect& GetPos() const;
	bool IsActive() const;
	void IsActive(bool isActive);
	void OnStart(SDL_Texture* texture);
	void Draw();
private:
	clf::Utilities::Sprite sprite;
	bool isActive = false;
};
