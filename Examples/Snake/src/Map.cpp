#include "Map.h"
#include "clf.h"

int Map::GetTileSize() { return tileSize; }
int Map::Width() { return totalTilesX; }
int Map::Height() { return totalTilesY; }

void Map::DrawGrid() {
	for (int y{ 0 }; y < totalTilesY; ++y) {
		for (int x{ 0 }; x < totalTilesX; ++x) {
			clf::Render::DrawRect({ tileSize * x, tileSize * y, tileSize, tileSize }, clf::Color::PORTAFINO);
		}
	}
}