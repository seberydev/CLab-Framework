#include "Random.h"

Random::Random()
	: random{ device() } {
	
}

int Random::GetRandomInt(int min, int max) {
	std::uniform_int<int> dist(min, max);

	return dist(random);
}

int Random::GetRandomIntEx(int min, int max, int exclude) {
	std::uniform_int<int> dist(min, max);
	int r = dist(random);

	while (r == exclude)
		r = dist(random);

	return r;
}