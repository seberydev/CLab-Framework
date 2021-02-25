#include "Spawner.h"
#include "Game.h"
#include <iostream>

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
		SpawnObject();
	}
}

void Spawner::Draw() {
	for (auto& object : objects) {
		if (object.IsActive())
			object.Draw();
	}
}

void Spawner::OnFinish() {
	clf::Asset::FreeTexture(texture);
}

void Spawner::SpawnObject() {
	if (currentActiveObjects < maxObjects) {
		for (auto& object : objects) {
			if (!object.IsActive()) {
				object.IsActive(true);
				object.SetDstX(static_cast<float>(random.GetRandomInt(static_cast<int>(minX) + boundPadding, static_cast<int>(maxX) - boundPadding)));
				object.SetDstY(static_cast<float>(random.GetRandomInt(static_cast<int>(minY) + boundPadding, static_cast<int>(maxY) - boundPadding)));
				++currentActiveObjects;
				break;
			}
		}
		
	}
}

void Spawner::CheckCollision(const SDL_FRect& topLeftPos) {
	float margin = 10.0f;

	for (auto& object : objects) {
		if (object.IsActive()) {
			SDL_FRect objPos = clf::Utilities::Info::GetRectTopLeftF(object.GetPos());

			if (objPos.x + objPos.w >= topLeftPos.x + margin &&
				objPos.x <= topLeftPos.x + topLeftPos.w - margin &&
				objPos.y <= topLeftPos.y + topLeftPos.h - margin &&
				objPos.y + objPos.h >= topLeftPos.y + margin) {
				++Game::score;
				object.IsActive(false);
				--currentActiveObjects;
			}
		}
			
	}
}
