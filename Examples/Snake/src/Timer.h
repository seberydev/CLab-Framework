#pragma once

class Timer {
public:
	Timer(float delay, float startTime);
	~Timer() = default;
public:
	bool Timing(float deltaTime);
	void SetDelay(float delay);
	float GetDelay() const;
private:
	float counter;
	float delay;
};