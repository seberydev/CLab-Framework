#include "Color.h"

Color const Color::METALLIC_BRONZE{ 76, 57, 26, 255 };
Color const Color::HOT_PINK{ 255, 102, 209, 255 };
Color const Color::POMEGRANATE{ 239, 52, 52, 255 };
Color const Color::MALIBU{ 94, 150, 255, 255 };
Color const Color::PORTAFINO{ 255, 249, 186, 255 };
Color const Color::SCREAMIN_GREEN{ 87, 249, 84, 255 };
Color const Color::TURQUOISE{ 68, 229, 162, 255 };
Color const Color::ROYAL_BLUE{ 114, 66, 237, 255 };

Color::Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) 
	: r{ red }, g{ green }, b{ blue }, a{ alpha } {
	
}
