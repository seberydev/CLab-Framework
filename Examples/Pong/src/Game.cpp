#include "Game.h"

//Engine Methods
void Game::OnStart() {
	titleText = clf::Asset::LoadText("assets/fonts/BoxfontRound.ttf", 24, "Pong Game", clf::Color::MALIBU, 0);
	titleTextDst = {
		(ScreenWidth() / 2) - (clf::Info::GetTextureWidth(titleText) / 2), 20,
		clf::Info::GetTextureWidth(titleText),
		clf::Info::GetTextureHeight(titleText) };
	padding.Init(10, clf::Color::ROYAL_BLUE);
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
	clf::Render::Clear(clf::Color::PORTAFINO);
	clf::Render::DrawText(titleText, titleTextDst);
	p1.Draw();
	p2.Draw();
	ball.Draw();
	padding.Draw();
}

void Game::OnFinish() {
	clf::Asset::FreeTexture(titleText);
}
