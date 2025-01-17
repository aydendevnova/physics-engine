#include "random.h"
#include <stdlib.h>
#include <time.h>

void init_random(void) {
    srand(time(NULL));
}

float random_float(float min, float max) {
    return min + (rand() / (float)RAND_MAX) * (max - min);
}

SDL_Color random_color(void) {
    return (SDL_Color){
        .r = rand() % 256,
        .g = rand() % 256,
        .b = rand() % 256,
        .a = 255
    };
} 