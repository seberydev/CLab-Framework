#pragma once

#include <array>
#include "clf.h"
#include "Timer.h"
#include "Block.h"

class Snake {
public:
	Snake();
	~Snake() = default;
public:
	void Init();
	void Draw();
	void SetDir(const Uint8* keystates);
	void Move(float deltaTime);
	void Finish();
private:
	double HeadAngle();
	double TailAngle();
	Timer timerToMove;
	int tileSize;
	std::array<Block, Map::size> blocks;
	size_t currentBlocks;
	SDL_Texture* head;
	SDL_Texture* body;
	SDL_Texture* tail;
};
