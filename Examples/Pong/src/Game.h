#pragma once

#include "clf.h"
#include "Player.h"
#include "Padding.h"

class Game : public clf::Engine {
public:
	Game() = default;
	~Game() = default;
private:
	Padding padding;
	Player p1;
	Player p2;
protected:
	void OnStart() override;
	void OnInput(const Uint8* keystates) override;
	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnFinish() override;
};
