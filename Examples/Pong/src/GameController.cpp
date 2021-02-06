#include "GameController.h"

bool GameController::GetGameStarted() const { return gameStarted; }

int GameController::GetGameMode() const { return gameMode; }
void GameController::SetGameMode(int gameMode) { this->gameMode = gameMode; }

void GameController::SetGameStarted(bool gameStarted) { this->gameStarted = gameStarted; }

void GameController::Init() {
	gameStarted = false;
}