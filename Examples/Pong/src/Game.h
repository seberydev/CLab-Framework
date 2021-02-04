#pragma once

#include "clf.h"
#include "Player.h"
#include "Padding.h"
#include "Ball.h"
#include "Text.h"

class Game : public clf::Engine {
public:
	Game() = default;
	~Game() = default;
private:
	Padding padding;
	Player p1;
	Player p2;
	Ball ball;
	Text titleText;
	Text p1Text;
	Text p2Text;
	Text p1ScoreText;
	Text p2ScoreText;
protected:
	void OnStart() override;
	void OnInput(const Uint8* keystates) override;
	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnFinish() override;
};
