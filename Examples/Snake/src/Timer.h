#pragma once

class Timer {
public:
	Timer(float delay, float startTime);
	~Timer() = default;
public:
	bool Timing(float deltaTime);
private:
	float counter;
	float delay;
};