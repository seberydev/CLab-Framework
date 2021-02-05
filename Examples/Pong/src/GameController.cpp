#include "GameController.h"

bool GameController::GetGameStarted() const { return gameStarted; }

void GameController::SetGameStarted(bool gameStarted) { this->gameStarted = gameStarted; }

void GameController::Init() {
	gameStarted = false;
}