#ifndef RANDOM_H
#define RANDOM_H

#include <SDL2/SDL.h>

// Initialize random number generator
void init_random(void);

// Get random float between min and max
float random_float(float min, float max);

// Get random color
SDL_Color random_color(void);

#endif // RANDOM_H 