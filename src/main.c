#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    float x, y;
    float vx, vy;  // velocity
    float ax, ay;  // acceleration
    float mass;
    float radius;
} Body;

// Physics constants
#define GRAVITY 784.0f // 9.8 * 80; Increased for more game-like feel
#define RESTITUTION 0.8f  // Bounce energy retention (1.0 = perfect bounce)
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS_CAP 120
#define TIME_SCALE 1.0f  // Adjust this to speed up or slow down simulation

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Body* bodies;
    int bodyCount;
    bool running;
} World;

void updatePhysics(World* world, float dt) {
    // Scale the time step
    dt *= TIME_SCALE;
    
    for (int i = 0; i < world->bodyCount; i++) {
        Body* body = &world->bodies[i];
        
        // Apply gravity force
        body->ay = GRAVITY;
        
        // Velocity Verlet integration (more accurate than Euler)

        // Store current acceleration values for Velocity Verlet integration
        float old_ax = body->ax;
        float old_ay = body->ay;
        
        // Update position using Velocity Verlet formula:
        // x(t + dt) = x(t) + v(t)dt + (1/2)a(t)dt^2
        body->x += body->vx * dt + 0.5f * old_ax * dt * dt;
        body->y += body->vy * dt + 0.5f * old_ay * dt * dt;
        
        // Update velocity using average of old and new acceleration:
        // v(t + dt) = v(t) + (1/2)(a(t) + a(t + dt))dt
        // This provides better energy conservation than simple Euler integration
        body->vx += 0.5f * (old_ax + body->ax) * dt;
        body->vy += 0.5f * (old_ay + body->ay) * dt;
        
        // Boundary collisions (all sides)
        if (body->y > WINDOW_HEIGHT - body->radius) {
            body->y = WINDOW_HEIGHT - body->radius;
            body->vy *= -RESTITUTION;
        }
        if (body->y < body->radius) {
            body->y = body->radius;
            body->vy *= -RESTITUTION;
        }
        if (body->x > WINDOW_WIDTH - body->radius) {
            body->x = WINDOW_WIDTH - body->radius;
            body->vx *= -RESTITUTION;
        }
        if (body->x < body->radius) {
            body->x = body->radius;
            body->vx *= -RESTITUTION;
        }
    }
}

void render(World* world) {
    SDL_SetRenderDrawColor(world->renderer, 0, 0, 0, 255);
    SDL_RenderClear(world->renderer);
    
    SDL_SetRenderDrawColor(world->renderer, 255, 255, 255, 255);
    for (int i = 0; i < world->bodyCount; i++) {
        Body* body = &world->bodies[i];
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

// Helper function to get random float between min and max
float random_float(float min, float max) {
    return min + (rand() / (float)RAND_MAX) * (max - min);
}

int main() {
    // Seed the random number generator with current time
    srand(time(NULL));
    
    // Initialize all members of world struct to 0/NULL using compound literal
    World world = {0};
    world.running = true;
    world.bodyCount = 10;
    world.bodies = malloc(sizeof(Body) * world.bodyCount);
    
    // Initialize SDL with error checking
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }
    
    world.window = SDL_CreateWindow("Physics Engine",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!world.window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    world.renderer = SDL_CreateRenderer(world.window, -1,
        SDL_RENDERER_ACCELERATED);
    if (!world.renderer) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(world.window);
        SDL_Quit();
        return 1;
    }
    
    // Create multiple bodies with random positions and velocities
    for (int i = 0; i < world.bodyCount; i++) {
        world.bodies[i] = (Body){
            .x = random_float(50, WINDOW_WIDTH - 50),  // Keep away from edges
            .y = random_float(50, WINDOW_HEIGHT / 2),   // Start in top half
            .vx = random_float(-200, 200),             // Random horizontal velocity
            .vy = random_float(-100, 100),             // Random vertical velocity
            .ax = 0,
            .ay = 0,
            .mass = random_float(0.5f, 2.0f),         // Random mass
            .radius = random_float(10, 30)            // Random size
        };
    }
    
    Uint32 lastTime = SDL_GetTicks();
    while (world.running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                world.running = false;
        }
        
        Uint32 currentTime = SDL_GetTicks();
        float dt = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
        updatePhysics(&world, dt);
        render(&world);
        
        SDL_Delay(1000 / FPS_CAP);
    }
    
    // Cleanup
    free(world.bodies);
    SDL_DestroyRenderer(world.renderer);
    SDL_DestroyWindow(world.window);
    SDL_Quit();
    
    return 0;
}