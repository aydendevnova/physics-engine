#include "renderer.h"
#include <stdio.h>

bool init_renderer(World* world) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return false;
    }
    
    world->window = SDL_CreateWindow("Physics Engine",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!world->window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }
    
    world->renderer = SDL_CreateRenderer(world->window, -1,
        SDL_RENDERER_ACCELERATED);
    if (!world->renderer) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(world->window);
        SDL_Quit();
        return false;
    }
    
    return true;
}

void cleanup_renderer(World* world) {
    SDL_DestroyRenderer(world->renderer);
    SDL_DestroyWindow(world->window);
    SDL_Quit();
}

void render_world(World* world) {
    SDL_SetRenderDrawColor(world->renderer, 0, 0, 0, 255);
    SDL_RenderClear(world->renderer);
    
    for (int i = 0; i < world->bodyCount; i++) {
        Body* body = &world->bodies[i];
        SDL_SetRenderDrawColor(world->renderer, 
            body->color.r, body->color.g, body->color.b, body->color.a);
            
        SDL_Rect rect = {
            (int)(body->x - body->radius),
            (int)(body->y - body->radius),
            (int)(body->radius * 2),
            (int)(body->radius * 2)
        };
        SDL_RenderFillRect(world->renderer, &rect);
    }
    
    SDL_RenderPresent(world->renderer);
} 