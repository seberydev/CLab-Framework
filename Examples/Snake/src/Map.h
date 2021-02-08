#pragma once

class Map {
public:
	Map() = default;
	~Map() = default;
public:
	static int GetTileSize();
	static int Width();
	static int Height();
	void DrawGrid();
private:
	static constexpr int tileSize = 16;
	static constexpr int screenWidth = 608;
	static constexpr int screenHeight = 400;
	static constexpr int totalTilesX = screenWidth / tileSize;
	static constexpr int totalTilesY = screenHeight / tileSize;
public:
	static constexpr int size = totalTilesX * totalTilesY;
private:
	int map[totalTilesX * totalTilesY];
};

