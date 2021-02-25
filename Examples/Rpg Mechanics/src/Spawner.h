#pragma once

#include "clf.h"
#include "Object.h"
#include "Random.h"

class Spawner {
public:
	Spawner();
public:
	void OnStart();
	void OnUpdate(float deltaTime);
	void Draw();
	void OnFinish();
	void CheckCollision(const SDL_FRect& topLeftPos);
private:
	void SpawnObject();
private:
	static constexpr int maxObjects = 10;
	Object objects[maxObjects];
	int currentIndex = 0;
	int currentActiveObjects = 0;
	float currentTime = 0.0f;
	float delay = 2.0f;
	float maxX, minX, maxY, minY;
	SDL_Texture* texture;
	Random random;
	int boundPadding = 20;
};
