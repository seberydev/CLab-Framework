#pragma once

class GameController {
public:
	GameController() = default;
	~GameController() = default;
public:
	void Init();
	bool GetGameStarted() const;
	void SetGameStarted(bool gameStarted);
private:
	bool gameStarted;
};