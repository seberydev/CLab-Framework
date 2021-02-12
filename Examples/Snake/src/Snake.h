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
	const SDL_Rect& GetHeadPos() const;
	void Grow();
	void IsGameOver();
	void Reset();
private:
	double HeadAngle();
	double TailAngle();
	Timer timerToMove;
	float startTimerToMoveDelay;
	int tileSize;
	std::array<Block, Map::size> blocks;
	size_t currentBodyBlocks;
	size_t startCurrentBodyBlocks;
	SDL_Texture* head;
	SDL_Texture* body;
	SDL_Texture* tail;
};
