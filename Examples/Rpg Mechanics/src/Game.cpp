#include "Game.h"
#include <iostream>

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

	timeText.OnStart(
		ScreenWidthF() - 40, 25,
		"assets/Oleaguid.ttf", std::to_string(seconds).c_str(), 32, clf::Utilities::Color::PORTAFINO, 0, TTF_STYLE_NORMAL);

	maxScoreText.OnStart(
		ScreenWidthF() - 125, ScreenHeightF() - 38,
		"assets/Oleaguid.ttf", "Max Score ", 32, clf::Utilities::Color::PORTAFINO, 0, TTF_STYLE_NORMAL);

	maxScoreNumberText.OnStart(
		ScreenWidthF() - 35, ScreenHeightF() - 38,
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

	//To count seconds
	currentTime += deltaTime;

	if (currentTime >= 1.0f) {
		currentTime = 0.0f;
		--seconds;
		timeText.SetText(std::to_string(seconds).c_str());
	}

	if (seconds <= 0) {
		if (score > maxScore) {
			maxScore = score;
			maxScoreNumberText.SetText(std::to_string(maxScore).c_str());
		}

		currentTime = 0.0f;
		seconds = 60;
		timeText.SetText(std::to_string(seconds).c_str());
		score = 0;
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
	timeText.Draw();
	maxScoreText.Draw();
	maxScoreNumberText.Draw();
}

void Game::OnFinish() {
	bg.OnFinish();
	skeleton.OnFinish();
	message.OnFinish();
	scoreText.OnFinish();
	spawner.OnFinish();
	scoreNumberText.OnFinish();
	timeText.OnFinish();
	maxScoreText.OnFinish();
	maxScoreNumberText.OnFinish();
}