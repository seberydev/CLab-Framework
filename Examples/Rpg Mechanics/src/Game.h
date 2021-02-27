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
	clf::Utilities::Text scoreText;
	clf::Utilities::Text scoreNumberText;
	clf::Utilities::Text timeText;
	clf::Utilities::Text maxScoreText;
	clf::Utilities::Text maxScoreNumberText;
	SDL_Color black{ 0, 0, 0, 255 };
	Spawner spawner;
	int oldScore = 0;
	int seconds = 60;
	float currentTime = 0.0f;
	int maxScore = 0;
};