#pragma once

#include "clf.h"

class Skeleton {
public:
	void OnStart(float x, float y);
	void OnInput(const Uint8* keystates);
	void OnUpdate(float deltaTime);
	void Draw();
	void OnFinish();
private:
	enum class AnimationID {
		IDLE_DOWN = 0,
		IDLE_UP,
		IDLE_RIGHT,
		IDLE_LEFT,
	};
private:
	void ChangeAnimID();
	void MoveAnim(float deltaTime);
	void SetOnBound();
private:
	AnimationID animID = AnimationID::IDLE_DOWN;
	clf::Utilities::Sprite skeleton;
	SDL_FRect direction;
	float speed = 80.0f;
	int animIndex = 0;
	float currentTime = 0.0f;
	float animDelay = 0.1f;
	int animTotalFrames = 5;
	float minX, maxX, minY, maxY;
};
