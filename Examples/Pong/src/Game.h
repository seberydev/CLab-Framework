#pragma once

#include "clf.h"
#include "Player.h"
#include "Padding.h"
#include "Ball.h"
#include "Text.h"
#include "GameController.h"
#include "Random.h"

class Game : public clf::Engine {
public:
	Game() = default;
	~Game() = default;
private:
	GameController gameController;
	Padding padding;
	Player p1;
	Player p2;
	Ball ball;
	Text titleText;
	Text p1Text;
	Text p2Text;
	Random random;
	Mix_Music* mainMusic;
protected:
	void OnStart() override;
	void OnInput(const Uint8* keystates) override;
	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnFinish() override;
};
