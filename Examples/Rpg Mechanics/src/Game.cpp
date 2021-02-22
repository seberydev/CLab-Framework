#include "Game.h"
#include <iostream>

void Game::OnStart() {
	bg.OnStart(
		0, 0, 608, 350,
		HalfScreenWidthF(), HalfScreenHeightF() - bottomSize, ScreenWidthF(), ScreenHeightF() - bottomSize,
		"assets/bg.png", SDL_FLIP_NONE, 0.0);

	skeleton.OnStart(bg.GetDstW() / 2.0f, bg.GetDstH() / 2.0f);
}

void Game::OnInput(const Uint8* keystates) {
	skeleton.OnInput(keystates);
}

void Game::OnUpdate(float deltaTime) {
	skeleton.OnUpdate(deltaTime);
}

void Game::OnRender() {
	bg.Draw();
	skeleton.Draw();
}

void Game::OnFinish() {
	bg.OnFinish();
	skeleton.OnFinish();
}