#include "Block.h"

Block::Block() 
	: texture{ nullptr }, 
	  pos{ 0, 0, Map::GetTileSize(), 
	  Map::GetTileSize() }, 
	  dir{ 0, 1, 0, 0 }, 
	  angle{ 0.0 },
	  lastDir{ 0, 0, 0, 0 },
	  tileSize{ Map::GetTileSize() } {

}

//Getters and Setters
void Block::SetPos(int x, int y) { pos.x = x; pos.y = y; }
int Block::GetPosX() { return pos.x; }
int Block::GetPosY() { return pos.y; }
double Block::GetAngle() { return angle; }
const SDL_Rect& Block::GetPos() const { return pos; }

void Block::SetDir(int x, int y) { 
	lastDir.x = pos.x;
	lastDir.y = pos.y;

	dir.x = x; dir.y = y; 
}

int Block::GetDirX() { return dir.x; }
int Block::GetDirY() { return dir.y; }
void Block::SetAngle(double angle) { this->angle = angle; }

//Methods
void Block::Init(SDL_Texture* texture) {
	this->texture = texture;
}

void Block::Draw() {
	clf::Render::DrawSpriteRot(texture, src, pos, angle, SDL_FLIP_NONE);
}

void Block::Finish() {
	texture = nullptr;
}

bool Block::IsColliding(const SDL_Rect& rect) const {
	return (
		(rect.x * tileSize) + rect.w >= pos.x * tileSize &&
		(pos.x * tileSize) + pos.w >= rect.x * tileSize &&
		(rect.y * tileSize) + rect.h >= pos.y * tileSize &&
		(pos.y * tileSize) + pos.h >= rect.y * tileSize);
}