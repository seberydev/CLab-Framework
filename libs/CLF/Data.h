#pragma once

class Data {
private:
	Data() = default;
	~Data() = default;
public:
	//Delta Time Data
	inline static const int FPS = 60;
	inline static const int FRAME_TARGET = 1000 / FPS;
	inline static const float MAX_DELTA_TIME = 0.05f;
};
