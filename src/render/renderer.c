#include "renderer.h"
#include <stdio.h>

bool init_renderer(World* world) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return false;
    }
    
    // Create main window
    world->window = SDL_CreateWindow("Physics Engine",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!world->window) {
        fprintf(stderr, "Main window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }
    
    // Create debug window
    world->debug_window = SDL_CreateWindow("Debug Info",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        DEBUG_WINDOW_WIDTH, DEBUG_WINDOW_HEIGHT, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!world->debug_window) {
        fprintf(stderr, "Debug window creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(world->window);
        SDL_Quit();
        return false;
    }
    
    // Create main renderer with vsync
    world->renderer = SDL_CreateRenderer(world->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!world->renderer) {
        fprintf(stderr, "Main renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(world->debug_window);
        SDL_DestroyWindow(world->window);
        SDL_Quit();
        return false;
    }
    
    // Create debug renderer with vsync
    world->debug_renderer = SDL_CreateRenderer(world->debug_window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!world->debug_renderer) {
        fprintf(stderr, "Debug renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(world->renderer);
        SDL_DestroyWindow(world->debug_window);
        SDL_DestroyWindow(world->window);
        SDL_Quit();
        return false;
    }

    // Set blend mode for both renderers
    SDL_SetRenderDrawBlendMode(world->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(world->debug_renderer, SDL_BLENDMODE_BLEND);
    
    printf("Renderer initialization successful!\n");
    printf("Main window size: %dx%d\n", WINDOW_WIDTH, WINDOW_HEIGHT);
    printf("Debug window size: %dx%d\n", DEBUG_WINDOW_WIDTH, DEBUG_WINDOW_HEIGHT);
    
    return true;
}

void cleanup_renderer(World* world) {
    SDL_DestroyRenderer(world->debug_renderer);
    SDL_DestroyRenderer(world->renderer);
    SDL_DestroyWindow(world->debug_window);
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

void render_debug_info(World* world) {
    SDL_SetRenderDrawColor(world->debug_renderer, 40, 40, 40, 255);
    SDL_RenderClear(world->debug_renderer);
    
    // Set up text position
    int x = 10;
    int y = 10;
    int line_height = 20;
    
    // Render debug info for each body
    for (int i = 0; i < world->bodyCount; i++) {
        Body* body = &world->bodies[i];
        SDL_SetRenderDrawColor(world->debug_renderer, 
            body->color.r, body->color.g, body->color.b, 255);
            
        // Draw a small rectangle to indicate the body's color
        SDL_Rect colorRect = {x, y, 10, 10};
        SDL_RenderFillRect(world->debug_renderer, &colorRect);
        
        // Draw lines to separate bodies
        SDL_SetRenderDrawColor(world->debug_renderer, 100, 100, 100, 255);
        SDL_RenderDrawLine(world->debug_renderer, 
            x, y + line_height, 
            DEBUG_WINDOW_WIDTH - 10, y + line_height);
            
        y += line_height + 5;
    }
    
    SDL_RenderPresent(world->debug_renderer);
} 