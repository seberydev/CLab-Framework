#include "Timer.h"

Timer::Timer(float delay, float startTime)
	: counter{ startTime }, delay{ delay } {

}

void Timer::SetDelay(float delay) { this->delay = delay; }
float Timer::GetDelay() const { return delay; }

bool Timer::Timing(float deltaTime) {
	counter += deltaTime;

	if (counter >= delay) {
		counter = 0.0f;
		return true;
	}

	return false;
}