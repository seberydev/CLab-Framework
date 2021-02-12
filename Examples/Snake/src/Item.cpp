#include "Item.h"
#include "Map.h"

Item::Item() 
	: pos{ 0, 0, Map::GetTileSize(), Map::GetTileSize() }, texture{ nullptr }, tileSize{ Map::GetTileSize() } {

}

void Item::Init(int x, int y) {
	pos.x = x * tileSize;
	pos.y = y * tileSize;

	texture = clf::Asset::LoadSprite("assets/sprites/apple.png");
}

void Item::Draw() {
	clf::Render::DrawSprite(texture, src, pos);
}

void Item::Finish() {
	clf::Asset::FreeTexture(texture);
}

bool Item::IsColliding(const SDL_Rect& rect) const {
	return (
		(rect.x * tileSize) + rect.w >= pos.x * tileSize &&
		(pos.x * tileSize) + pos.w >= rect.x * tileSize &&
		(rect.y * tileSize) + rect.h >= pos.y * tileSize &&
		(pos.y * tileSize) + pos.h >= rect.y * tileSize);
}

void Item::NewPos(int x, int y) {
	pos.x = x * tileSize;
	pos.y = y * tileSize;
}