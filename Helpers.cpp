#include"Helpers.h"

void SetColour(SDL_Renderer* r, SDL_Colour c) {
	SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
}