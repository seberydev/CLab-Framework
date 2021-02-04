#include "Game.h"

//Engine Methods
void Game::OnStart() {
	//Initialize Texts
	const char* mainFont = "assets/fonts/BoxfontRound.ttf";

	titleText.Init(clf::Color::MALIBU, "Pong Game", mainFont, 32, TTF_STYLE_BOLD);
	titleText.SetPos((ScreenWidth() / 2) - (titleText.GetPos().w / 2), 40);

	p1Text.Init(clf::Color::MALIBU, "1 Player (R)", mainFont, 24, TTF_STYLE_NORMAL);
	p1Text.SetPos((ScreenWidth() / 2) - (p1Text.GetPos().w / 2), (ScreenHeight() / 2) - (p1Text.GetPos().h / 2) - 20);

	p2Text.Init(clf::Color::MALIBU, "2 Players (T)", mainFont, 24, TTF_STYLE_NORMAL);
	p2Text.SetPos((ScreenWidth() / 2) - (p2Text.GetPos().w / 2), (ScreenHeight() / 2) - (p2Text.GetPos().h / 2) + 20);

	p1ScoreText.Init(clf::Color::MALIBU, "0", mainFont, 32, TTF_STYLE_NORMAL);
	p1ScoreText.SetPos(40, 40);

	p2ScoreText.Init(clf::Color::MALIBU, "0", mainFont, 32, TTF_STYLE_NORMAL);
	p2ScoreText.SetPos(ScreenWidth() - p2ScoreText.GetPos().w - 40, 40);

	//Initialize Padding
	padding.Init(10, clf::Color::ROYAL_BLUE);

	//Initialize Players and Ball
	p1.Init(Padding::GetSize() + 10, 500, clf::Color::POMEGRANATE);
	p2.Init(ScreenWidth() - Padding::GetSize() - 30, 500, clf::Color::POMEGRANATE);
	ball.Init(10, 500, clf::Color::METALLIC_BRONZE);
}

void Game::OnInput(const Uint8* keystates) {
	if (keystates[SDL_SCANCODE_W])
		p1.SetDir(-1);
	else if (keystates[SDL_SCANCODE_S])
		p1.SetDir(1);
	else 
		p1.SetDir(0);

	if (keystates[SDL_SCANCODE_UP])
		p2.SetDir(-1);
	else if (keystates[SDL_SCANCODE_DOWN])
		p2.SetDir(1);
	else
		p2.SetDir(0);
}

void Game::OnUpdate(float deltaTime) {
	p1.Move(deltaTime);
	p2.Move(deltaTime);
	ball.Move(deltaTime, p1.GetPos(), p2.GetPos());
}

void Game::OnRender() {
	//Render Padding and Background Color
	clf::Render::Clear(clf::Color::PORTAFINO);
	padding.Draw();
	
	//Render Players and Ball
	p1.Draw();
	p2.Draw();
	ball.Draw();

	//Render Texts
	titleText.Draw();
	p1Text.Draw();
	p2Text.Draw();
	p1ScoreText.Draw();
	p2ScoreText.Draw();
}

void Game::OnFinish() {
	
}
