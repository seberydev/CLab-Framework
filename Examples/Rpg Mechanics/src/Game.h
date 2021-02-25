#pragma once

#include "clf.h"
#include "Skeleton.h"
#include "Spawner.h"

class Game : public clf::Engine {
public:
	static int score;
protected:
	void OnStart() override;
	void OnInput(const Uint8* keystates) override;
	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnFinish() override;
private:
	clf::Utilities::Sprite bg;
	Skeleton skeleton;
	float bottomSize = 50.0f;
	clf::Utilities::Text message;
	SDL_Color black{ 0, 0, 0, 255 };
	clf::Utilities::Text scoreText;
	clf::Utilities::Text scoreNumberText;
	Spawner spawner;
	int oldScore = 0;
};