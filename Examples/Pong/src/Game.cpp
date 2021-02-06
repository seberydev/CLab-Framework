#include "Game.h"

//Engine Methods
void Game::OnStart() {
	//Start Main music
	mainMusic = clf::Asset::LoadMusic("assets/music/Mars.wav");
	clf::Sound::SetMusicVolume(20);
	clf::Sound::PlayFadeInMusic(mainMusic, -1, 3000);

	gameController.Init();

	//Initialize Texts
	const char* mainFont = "assets/fonts/BoxfontRound.ttf";

	titleText.Init(clf::Color::PORTAFINO, "Pong Game", mainFont, 32, TTF_STYLE_BOLD);
	titleText.SetPos(HalfScreenWidth() - (titleText.GetPos().w / 2), 40);

	p1Text.Init(clf::Color::PORTAFINO, "1 Player (R)", mainFont, 24, TTF_STYLE_NORMAL);
	p1Text.SetPos(HalfScreenWidth() - (p1Text.GetPos().w / 2), HalfScreenHeight() - (p1Text.GetPos().h / 2) - 20);

	p2Text.Init(clf::Color::PORTAFINO, "2 Players (T)", mainFont, 24, TTF_STYLE_NORMAL);
	p2Text.SetPos(HalfScreenWidth() - (p2Text.GetPos().w / 2), HalfScreenHeight() - (p2Text.GetPos().h / 2) + 20);

	//Initialize Padding
	padding.Init(10, clf::Color::TURQUOISE);

	//Initialize Players and Ball
	p1.Init(Padding::GetSize() + 10, 500, clf::Color::SCREAMIN_GREEN);
	p2.Init(ScreenWidth() - Padding::GetSize() - 30, 500, clf::Color::SCREAMIN_GREEN);
	ball.Init(10, 500, clf::Color::POMEGRANATE);
}

void Game::OnInput(const Uint8* keystates) {
	if (gameController.GetGameStarted()) {
		p1.SetDir(keystates, SDL_SCANCODE_W, SDL_SCANCODE_S);

		if (gameController.GetGameMode() == 2)
			p2.SetDir(keystates, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN);
		else
			p2.SetDirAI(ball.GetCenter().y);
	}

	//Start The Game For 1 Player or 2 Players
	//R for 1 Player Mode and T for 2 Players Mode
	if (!gameController.GetGameStarted() && keystates[SDL_SCANCODE_R] ||
		!gameController.GetGameStarted() && keystates[SDL_SCANCODE_T]) {
		gameController.SetGameStarted(true);
		ball.SetDir(random.GetRandomIntEx(-1, 1, 0), random.GetRandomIntEx(-1, 1, 0));
		gameController.SetGameMode(keystates[SDL_SCANCODE_R] ? 1 : 2);
	}
}

void Game::OnUpdate(float deltaTime) {
	p1.Move(deltaTime);

	if (gameController.GetGameMode() == 2)
		p2.Move(deltaTime);
	else if (ball.GetCenter().x > HalfScreenWidth() + random.GetRandomInt(20, 175))
		p2.Move(deltaTime);
		
	ball.Move(deltaTime, p1.GetPos(), p2.GetPos());
	
	if (ball.Reset()) {
		gameController.SetGameStarted(false);
		p1.Reset();
		p2.Reset();
	}
}

void Game::OnRender() {
	//Render Padding and Background Color
	clf::Render::Clear(clf::Color::ROYAL_BLUE);
	padding.Draw();
	
	//Render Players and Ball
	p1.Draw();
	p2.Draw();
	ball.Draw();

	//Render Texts
	if (!gameController.GetGameStarted()) {
		titleText.Draw();
		p1Text.Draw();
		p2Text.Draw();
	} 
}

void Game::OnFinish() {
	ball.Finish();
	//Delete textures
	titleText.Finish();
	p1Text.Finish();
	p2Text.Finish();
	//Delete Sounds
	clf::Asset::FreeMusic(mainMusic);
}
