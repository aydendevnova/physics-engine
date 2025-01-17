#include "core/types.h"
#include "physics/physics.h"
#include "render/renderer.h"
#include "ui/ui.h"
#include "utils/random.h"
#include <stdlib.h>

// Constants for impulse behavior
#define IMPULSE_STRENGTH 1000.0f

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
    if (!init_ui(&world)) {
        cleanup_renderer(&world);
        free(world.bodies);
        return 1;
    }
    
    // Store window IDs for event handling
    Uint32 main_window_id = SDL_GetWindowID(world.window);
    Uint32 debug_window_id = SDL_GetWindowID(world.debug_window);
    
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
        Uint32 currentTime = SDL_GetTicks();
        float dt = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
        // Start UI input handling
        nk_input_begin(world.nk_ctx);
        
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                world.running = false;
                continue;
            }
            else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
                if (event.window.windowID == main_window_id || 
                    event.window.windowID == debug_window_id) {
                    world.running = false;
                    continue;
                }
            }
            
            // If event is in debug window, let Nuklear handle it
            if (event.window.windowID == debug_window_id) {
                nk_sdl_handle_event(&event);
            }
            // If event is in main window, handle physics
            else if (event.window.windowID == main_window_id && 
                     event.type == SDL_MOUSEBUTTONDOWN && 
                     event.button.button == SDL_BUTTON_LEFT) {
                
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                
                // Find clicked body
                Body* clickedBody = get_body_at_position(&world, mouseX, mouseY);
                if (clickedBody) {
                    // Get edge point and normal
                    float edgeX, edgeY, normalX, normalY;
                    get_closest_edge_info(clickedBody, mouseX, mouseY, &edgeX, &edgeY, &normalX, &normalY);
                    
                    // Apply impulse in the direction of the normal
                    apply_impulse(clickedBody, normalX * IMPULSE_STRENGTH, normalY * IMPULSE_STRENGTH);
                }
            }
        }
        
        // End UI input handling
        nk_input_end(world.nk_ctx);
        
        update_physics(&world, dt);
        render_world(&world);
        update_ui(&world);
        
        SDL_Delay(1000 / FPS_CAP);
    }
    
    // Cleanup
    cleanup_ui(&world);
    cleanup_renderer(&world);
    free(world.bodies);
    
    return 0;
}