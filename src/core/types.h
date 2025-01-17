#ifndef TYPES_H
#define TYPES_H

#include <SDL.h>
#include <stdbool.h>

// Nuklear declarations only
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "../../include/nuklear/nuklear.h"
#include "../../include/nuklear/nuklear_sdl_renderer.h"

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
#define DEBUG_WINDOW_WIDTH 400
#define DEBUG_WINDOW_HEIGHT 600
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
    SDL_Window* debug_window;
    SDL_Renderer* debug_renderer;
    struct nk_context* nk_ctx;
    struct nk_font_atlas* atlas;
    Body* bodies;
    int bodyCount;
    bool running;
} World;

#endif // TYPES_H 