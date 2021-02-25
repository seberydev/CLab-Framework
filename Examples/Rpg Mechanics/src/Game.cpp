#include "Game.h"

int Game::score = 0;

void Game::OnStart() {
	bg.OnStart(
		0, 0, 608, 350,
		HalfScreenWidthF(), HalfScreenHeightF() - bottomSize, ScreenWidthF(), ScreenHeightF() - bottomSize,
		"assets/bg.png", SDL_FLIP_NONE, 0.0);

	message.OnStart(
		140, ScreenHeightF() - 38, 
		"assets/Oleaguid.ttf", "Hello! I'm a skeleton", 32, clf::Utilities::Color::PORTAFINO, 0, TTF_STYLE_NORMAL);

	scoreText.OnStart(
		80, 25,
		"assets/Oleaguid.ttf", "Score ", 32, clf::Utilities::Color::PORTAFINO, 0, TTF_STYLE_NORMAL);

	scoreNumberText.OnStart(
		135, 25,
		"assets/Oleaguid.ttf", "0", 32, clf::Utilities::Color::PORTAFINO, 0, TTF_STYLE_NORMAL);

	skeleton.OnStart(bg.GetDstW() / 2.0f, bg.GetDstH() / 2.0f);

	spawner.OnStart();
}

void Game::OnInput(const Uint8* keystates) {
	skeleton.OnInput(keystates);
}

void Game::OnUpdate(float deltaTime) {
	skeleton.OnUpdate(deltaTime);
	spawner.OnUpdate(deltaTime);
	spawner.CheckCollision(clf::Utilities::Info::GetRectTopLeftF(skeleton.GetPos()));

	if (oldScore != score) {
		oldScore = score;
		scoreNumberText.SetText(std::to_string(oldScore).c_str());
	}
}

void Game::OnRender() {
	clf::Render::Clear(black);
	bg.Draw();
	spawner.Draw();
	skeleton.Draw();
	message.Draw();
	scoreText.Draw();
	scoreNumberText.Draw();
}

void Game::OnFinish() {
	bg.OnFinish();
	skeleton.OnFinish();
	message.OnFinish();
	scoreText.OnFinish();
	spawner.OnFinish();
	scoreNumberText.OnFinish();
}