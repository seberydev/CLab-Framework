#include "Spawner.h"

Spawner::Spawner() 
	: texture{ nullptr } {
	
}

void Spawner::OnStart() {
	texture = clf::Asset::LoadSprite("assets/apple.png");

	for (auto& object : objects) {
		object.OnStart(texture);
	}

	maxX = clf::Engine::ScreenWidthF() - objects[0].GetDstW() / 2.0f;
	minX = objects[0].GetDstW() / 2.0f;
	maxY = clf::Engine::ScreenHeightF() - 50.0f - objects[0].GetDstH();
	minY = objects[0].GetDstH() / 2.0f;
}

void Spawner::OnUpdate(float deltaTime) {
	currentTime += deltaTime;

	if (currentTime >= delay) {
		currentTime = 0.0f;
		// TODO: SPAWN OBJECTS
	}
}

void Spawner::Draw() {
	//Test
	objects[0].Draw();
}

void Spawner::OnFinish() {
	clf::Asset::FreeTexture(texture);
}
