#pragma once

#include <SDL_stdinc.h>

struct Color {
public:
	Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
	~Color() = default;
public:
	static const Color METALLIC_BRONZE;
	static const Color HOT_PINK;
	static const Color POMEGRANATE;
	static const Color MALIBU;
	static const Color PORTAFINO;
	static const Color SCREAMIN_GREEN;
	static const Color TURQUOISE;
	static const Color ROYAL_BLUE;
public:
	Uint8 r, g, b, a;
};
