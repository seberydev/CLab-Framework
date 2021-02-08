#include "Game.h"

void Game::OnStart() {
	snake.Init();
}

void Game::OnInput(const Uint8* keystates) {
	snake.SetDir(keystates);
}

void Game::OnUpdate(float deltaTime) {
	snake.Move(deltaTime);
}

void Game::OnRender() {
	clf::Render::Clear(clf::Color::POMEGRANATE);
	map.DrawGrid();
	snake.Draw();
}

void Game::OnFinish() {
	snake.Finish();
}