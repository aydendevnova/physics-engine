#ifndef RENDERER_H
#define RENDERER_H

#include "../core/types.h"

// Initialize the renderer and window
bool init_renderer(World* world);

// Cleanup renderer resources
void cleanup_renderer(World* world);

// Render the current state of the world
void render_world(World* world);

// Render debug information
void render_debug_info(World* world);

#endif // RENDERER_H 