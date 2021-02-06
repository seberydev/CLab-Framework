#pragma once

#include <random>

class Random {
public:
	Random();
	~Random() = default;
public:
	int GetRandomInt(int min, int max);
	int GetRandomIntEx(int min, int max, int exclude);
private:
	std::random_device device;
	std::mt19937 random;
};
