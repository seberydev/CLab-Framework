#pragma once

class GameController {
public:
	GameController() = default;
	~GameController() = default;
public:
	void Init();
	bool GetGameStarted() const;
	void SetGameStarted(bool gameStarted);
	int GetGameMode() const;
	void SetGameMode(int gameMode);
private:
	bool gameStarted;
	int gameMode; // 1 for 1 player and 2 for 2 players
};