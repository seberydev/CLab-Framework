#include <clf.h>
#include <iostream>

class Game : public clf::Engine {
public:
	Game() = default;
public:
	int width{ 600 }, height{ 400 };
	SDL_Color backgroundColor{ 0, 0, 0, 255 };
	SDL_Color playerColor{ 255, 255, 255, 255 };
	SDL_Rect p1{ 20, (height / 2) - 30, 20, 60 };
	SDL_Rect p2{ width - 40, (height / 2) - 30, 20, 60 };
	int p1Dir{ 0 };
	int p2Dir{ 0 };
	int pSpeed{ 500 };
	int ballRadius{ 10 };
	SDL_Point ballPos{ (width / 2 - ballRadius), (height / 2 - ballRadius) };
	SDL_Point ballStartPos{ (width / 2 - ballRadius), (height / 2 - ballRadius) };
	SDL_Color ballColor{ 255, 255, 255, 255 };
	SDL_Rect ballDir{ -1, -1, 0, 0 };
	int ballSpeed{ 500 };
	SDL_Texture* titleText{ nullptr };
	SDL_Rect titleTextDst;
protected:
	void OnStart() override {
		titleText = clf::Asset::LoadText("assets/fonts/BoxfontRound.ttf", 24, "Pong Game", { 255, 255, 255, 255 }, 0);
		titleTextDst = { 
			(width / 2) - (clf::Info::GetTextureWidth(titleText) / 2), 20, 
			clf::Info::GetTextureWidth(titleText), 
			clf::Info::GetTextureHeight(titleText) };
	}

	void OnInput(const Uint8* keystates) override {
		//Move Players
		if (keystates[SDL_SCANCODE_W])
			p1Dir = -1;
		else if (keystates[SDL_SCANCODE_S])
			p1Dir = 1;
		else 
			p1Dir = 0;

		if (keystates[SDL_SCANCODE_UP])
			p2Dir = -1;
		else if (keystates[SDL_SCANCODE_DOWN])
			p2Dir = 1;
		else
			p2Dir = 0;

	}

	void OnUpdate(float deltaTime) override {
		p1.y += static_cast<int>(pSpeed * deltaTime * p1Dir);
		p2.y += static_cast<int>(pSpeed * deltaTime * p2Dir);

		ballPos.x += static_cast<int>(ballSpeed * deltaTime * ballDir.x);
		ballPos.y += static_cast<int>(ballSpeed * deltaTime * ballDir.y);
		BallOutOfBounds();
		ResetBall();
	}

	void OnRender() override {
		clf::Render::Clear(backgroundColor);
		clf::Render::DrawFillRect(p1, playerColor);
		clf::Render::DrawFillRect(p2, playerColor);
		clf::Render::DrawText(titleText, titleTextDst);
		clf::Render::DrawFillCircle(ballPos, ballRadius, ballColor);
	}

	void OnFinish() override {
		clf::Asset::FreeTexture(titleText);
	}

	void BallOutOfBounds() {
		if (ballPos.y + ballRadius * 2 >= height) {
			ballDir.y *= -1;
			ballPos.y = height - ballRadius * 2;
		}

		if (ballPos.y <= 0) {
			ballDir.y *= -1;
			ballPos.y = 0;
		}

		//Check Collision With Players
		if (IsCollidingWithBall(p1)) {
			ballDir.x *= -1;
			ballPos.x = p1.x + p1.w;
		}

		if (IsCollidingWithBall(p2)) {
			ballDir.x *= -1;
			ballPos.x = p2.x - ballRadius * 2;
		}
	}

	bool IsCollidingWithBall(const SDL_Rect& rect) {
		return (
			rect.x + rect.w >= ballPos.x &&
			ballPos.x + ballRadius * 2 >= rect.x &&
			rect.y + rect.h >= ballPos.y &&
			ballPos.y + ballRadius * 2 >= rect.y);
	}

	void ResetBall() {
		if (ballPos.x >= p2.x + p2.w / 2) {
			ballDir.x = 0;
			ballDir.y = 0;
			ballPos.x = ballStartPos.x;
			ballPos.y = ballStartPos.y;
		}

	}
};

int main(int argc, char* args[]) {
	Game game;
	game.Build("Pong Game - CLab Framework", 600, 400, SDL_INIT_EVERYTHING, SDL_WINDOW_SHOWN);

	return 0;
}