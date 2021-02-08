#pragma once

#include "clf.h"
#include "Map.h"
#include "Snake.h"

class Game : public clf::Engine {
public:
	Game() = default;
	~Game() = default;
protected:
	void OnStart() override;
	void OnInput(const Uint8* keystates) override;
	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnFinish() override;
private:
	Map map;
	Snake snake;
};
