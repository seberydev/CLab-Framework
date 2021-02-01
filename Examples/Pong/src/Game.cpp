#include "Game.h"

//Engine Methods
void Game::OnStart() {
	padding.Init(10, clf::Color::ROYAL_BLUE);
	p1.Init(padding.GetSize() + 10, clf::Color::POMEGRANATE);
	p2.Init(ScreenWidth() - padding.GetSize() - 30, clf::Color::POMEGRANATE);
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
}

void Game::OnRender() {
	clf::Render::Clear(clf::Color::PORTAFINO);
	padding.Draw();
	p1.Draw();
	p2.Draw();
}

void Game::OnFinish() {

}
