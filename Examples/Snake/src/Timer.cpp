#include "Timer.h"

Timer::Timer(float delay, float startTime)
	: counter{ startTime }, delay{ delay } {

}

bool Timer::Timing(float deltaTime) {
	counter += deltaTime;

	if (counter >= delay) {
		counter = 0.0f;
		return true;
	}

	return false;
}