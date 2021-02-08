#include "Snake.h"
#include "Map.h"

Snake::Snake()
	: timerToMove{ 0.5f, 0.0f },
	  tileSize{ Map::GetTileSize() },
	  currentBlocks{ 5 } {

}

void Snake::Init() {
	//Snake's Head
	blocks.at(0).Init("assets/sprites/head.png");
	blocks.at(0).SetPos((Map::Width() / 2) * tileSize, (Map::Height() / 2) * tileSize);

	//Snake's Body
	for (size_t i{ 1 }; i <= currentBlocks; ++i) {
		blocks.at(i).Init("assets/sprites/body.png");
		blocks.at(i).SetPos((Map::Width() / 2) * tileSize, ((Map::Height() / 2) * tileSize) - (tileSize * i));
	}
}

void Snake::SetDir(const Uint8* keystates) {
	//Snake's Head Direction
	if (keystates[SDL_SCANCODE_W] && blocks.at(0).GetDirY() != 1) {
		blocks.at(0).SetDir(0, -1);
	} else if (keystates[SDL_SCANCODE_S] && blocks.at(0).GetDirY() != -1) {
		blocks.at(0).SetDir(0, 1);
	} else if(keystates[SDL_SCANCODE_D] && blocks.at(0).GetDirX() != -1) {
		blocks.at(0).SetDir(1, 0);
	} else if (keystates[SDL_SCANCODE_A] && blocks.at(0).GetDirX() != 1) {
		blocks.at(0).SetDir(-1, 0);
	}
}

void Snake::Move(float deltaTime) {
	//Move the snake at specific time in seconds
	if (timerToMove.Timing(deltaTime)) {
		//Snake's Head Angle
		SetHeadAngle();

		//Snake's Body
		for (size_t i{ currentBlocks }; i >= 1; --i) {
			blocks.at(i).SetPos(blocks.at(i - 1).GetPosX(), blocks.at(i - 1).GetPosY());
		}

		//Snake's Head
		blocks.at(0).SetPos(
			blocks.at(0).GetPosX() + tileSize * blocks.at(0).GetDirX(),
			blocks.at(0).GetPosY() + tileSize * blocks.at(0).GetDirY());
	}
}

void Snake::Draw() {
	//Snake's Head
	blocks.at(0).Draw();

	//Snake's Body
	for (size_t i{ 1 }; i <= currentBlocks; ++i) {
		blocks.at(i).Draw();
	}
}

void Snake::Finish() {
	//Snake's Head
	blocks.at(0).Finish();

	//Snake's Body
	for (size_t i{ 1 }; i <= currentBlocks; ++i) {
		blocks.at(i).Finish();
	}
}

void Snake::SetHeadAngle() {
	if (blocks.at(0).GetDirX() > 0)
		blocks.at(0).SetAngle(270.0);
	else if (blocks.at(0).GetDirX() < 0)
		blocks.at(0).SetAngle(90.0);
	else if (blocks.at(0).GetDirY() > 0)
		blocks.at(0).SetAngle(0.0);
	else if (blocks.at(0).GetDirY() < 0)
		blocks.at(0).SetAngle(180.0);
}

