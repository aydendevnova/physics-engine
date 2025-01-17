#ifndef TYPES_H
#define TYPES_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// Physics constants
#define GRAVITY 784.0f
#define RESTITUTION 0.8f
#define TIME_SCALE 1.0f

// Collision constants
#define COLLISION_ITERATIONS 2  // Number of collision resolution iterations
#define MIN_SEPARATION 0.01f   // Minimum separation distance after collision

// Window constants
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS_CAP 120

typedef struct {
    float x, y;
    float vx, vy;
    float ax, ay;
    float mass;
    float radius;
    SDL_Color color;
} Body;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Body* bodies;
    int bodyCount;
    bool running;
} World;

#endif // TYPES_H 