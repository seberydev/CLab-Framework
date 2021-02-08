#include "Block.h"

Block::Block() 
	: texture{ nullptr }, 
	  pos{ 0, 0, Map::GetTileSize(), 
	  Map::GetTileSize() }, 
	  dir{ 0, 1, 0, 0 }, 
	  angle{ 0.0 },
	  lastDir{ 0, 0, 0, 0 } {

}

//Getters and Setters
void Block::SetPos(int x, int y) { pos.x = x; pos.y = y; }
int Block::GetPosX() { return pos.x; }
int Block::GetPosY() { return pos.y; }

void Block::SetDir(int x, int y) { 
	lastDir.x = pos.x;
	lastDir.y = pos.y;

	dir.x = x; dir.y = y; 
}

int Block::GetDirX() { return dir.x; }
int Block::GetDirY() { return dir.y; }
void Block::SetAngle(double angle) { this->angle = angle; }

//Methods
void Block::Init(const char* filepath) {
	texture = clf::Asset::LoadSprite(filepath);
}

void Block::Draw() {
	clf::Render::DrawSpriteRot(texture, src, pos, angle, SDL_FLIP_NONE);
}

void Block::Finish() {
	clf::Asset::FreeTexture(texture);
}