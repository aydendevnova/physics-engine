#include "core/types.h"
#include "physics/physics.h"
#include "render/renderer.h"
#include "utils/random.h"
#include <stdlib.h>

int main() {
    World world = {0};
    world.running = true;
    world.bodyCount = 15;
    world.bodies = malloc(sizeof(Body) * world.bodyCount);
    
    // Initialize systems
    init_random();
    if (!init_renderer(&world)) {
        free(world.bodies);
        return 1;
    }
    
    // Create initial bodies
    for (int i = 0; i < world.bodyCount; i++) {
        world.bodies[i] = create_body(
            random_float(50, WINDOW_WIDTH - 50),   // x
            random_float(50, WINDOW_HEIGHT / 2),   // y
            random_float(-200, 200),              // vx
            random_float(-100, 100),              // vy
            random_float(0.5f, 2.0f),             // mass
            random_float(10, 30),                 // radius
            random_color()                        // color
        );
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
        
        update_physics(&world, dt);
        render_world(&world);
        
        SDL_Delay(1000 / FPS_CAP);
    }
    
    // Cleanup
    free(world.bodies);
    cleanup_renderer(&world);
    
    return 0;
}