#include "Game.h"

void Game::OnStart() {
	snake.Init();
	item.Init(random.GetRandomInt(0, Map::Width()), random.GetRandomInt(0, Map::Height()));
}

void Game::OnInput(const Uint8* keystates) {
	snake.SetDir(keystates);
}

void Game::OnUpdate(float deltaTime) {
	snake.IsGameOver();
	if (item.IsColliding(snake.GetHeadPos())) {
		item.NewPos(random.GetRandomInt(0, Map::Width()), random.GetRandomInt(0, Map::Height()));
		snake.Grow();
	}

	snake.Move(deltaTime);
}

void Game::OnRender() {
	clf::Render::Clear(clf::Color::SCREAMIN_GREEN);
	//map.DrawGrid();
	item.Draw();
	snake.Draw();
}

void Game::OnFinish() {
	snake.Finish();
	item.Finish();
}