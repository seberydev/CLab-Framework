#include "Snake.h"
#include "Map.h"

Snake::Snake()
	: timerToMove{ 0.5f, 0.0f },
	  tileSize{ Map::GetTileSize() },
	  currentBlocks{ 2 },
	  head{ nullptr },
	  body{ nullptr },
	  tail{ nullptr } {

}

void Snake::Init() {
	//Get Textures
	head = clf::Asset::LoadSprite("assets/sprites/head.png");
	body = clf::Asset::LoadSprite("assets/sprites/body.png");
	tail = clf::Asset::LoadSprite("assets/sprites/tail.png");

	//Snake's Head
	blocks.at(0).Init(head);
	blocks.at(0).SetPos((Map::Width() / 2) * tileSize, (Map::Height() / 2) * tileSize);

	//Snake's Body
	for (size_t i{ 1 }; i <= currentBlocks; ++i) {
		blocks.at(i).Init(body);
		blocks.at(i).SetPos((Map::Width() / 2) * tileSize, ((Map::Height() / 2) * tileSize) - (tileSize * i));
	}

	//Snake's Tail
	blocks.at(currentBlocks + 1).Init(tail);
	blocks.at(currentBlocks + 1).SetPos((Map::Width() / 2) * tileSize, ((Map::Height() / 2) * tileSize) - (tileSize * (currentBlocks + 1)));
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
		//Snake's Head Angle and last Position
		double headAngle = HeadAngle();

		//Prevent to move backwards
		if (blocks.at(0).GetPosX() + tileSize * blocks.at(0).GetDirX() == blocks.at(1).GetPosX() &&
			blocks.at(0).GetPosY() + tileSize * blocks.at(0).GetDirY() == blocks.at(1).GetPosY()) {
		
			if (headAngle == 0.0 || headAngle == 180.0)
				blocks.at(0).SetDir(blocks.at(0).GetDirX(), blocks.at(0).GetDirY() * -1);
			else
				blocks.at(0).SetDir(blocks.at(0).GetDirX() * -1, blocks.at(0).GetDirY());

		} else {
			blocks.at(0).SetAngle(headAngle);
		}

		//Snake's Body and Tail
		for (size_t i{ currentBlocks + 1 }; i >= 1; --i) {
			blocks.at(i).SetPos(blocks.at(i - 1).GetPosX(), blocks.at(i - 1).GetPosY());
		}

		//Snake's Tail angle
		blocks.at(currentBlocks + 1).SetAngle(TailAngle());

		//Snake's Head
		blocks.at(0).SetPos(
			blocks.at(0).GetPosX() + tileSize * blocks.at(0).GetDirX(),
			blocks.at(0).GetPosY() + tileSize * blocks.at(0).GetDirY());
	}

}

void Snake::Draw() {
	//Snake's Head
	blocks.at(0).Draw();

	//Snake's Body and Tail
	for (size_t i{ 1 }; i <= currentBlocks + 1; ++i) {
		blocks.at(i).Draw();
	}

}

void Snake::Finish() {
	//Snake's Head
	blocks.at(0).Finish();

	//Snake's Body and Tail
	for (size_t i{ 1 }; i <= currentBlocks + 1; ++i) {
		blocks.at(i).Finish();
	}

	//Free Textures
	clf::Asset::FreeTexture(head);
	clf::Asset::FreeTexture(body);
	clf::Asset::FreeTexture(tail);
}

double Snake::HeadAngle() {
	if (blocks.at(0).GetDirX() > 0)
		return 270.0;
	else if (blocks.at(0).GetDirX() < 0)
		return 90.0;
	else if (blocks.at(0).GetDirY() > 0)
		return 0.0;
	else
		return 180.0;
}

double Snake::TailAngle() {
	int compareX = blocks.at(currentBlocks).GetPosX();
	int compareY = blocks.at(currentBlocks).GetPosY();
	int tailX = blocks.at(currentBlocks + 1).GetPosX();
	int tailY = blocks.at(currentBlocks + 1).GetPosY();

	if (compareY == tailY && compareX < tailX)
		return 90.0;
	else if (compareY == tailY && compareX > tailX)
		return 270.0;
	else if (compareY < tailY && compareX == tailX)
		return 180.0;
	else
		return 0.0;
}

