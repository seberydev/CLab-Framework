#include "Clear.h"

void Clear::Blue() {
	SDL_SetRenderDrawColor(CLF::GetRenderer(), 92, 138, 185, 255);
	SDL_RenderClear(CLF::GetRenderer());
}
