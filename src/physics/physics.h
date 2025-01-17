#ifndef PHYSICS_H
#define PHYSICS_H

#include "../core/types.h"

// Initialize a new physics body with given parameters
Body create_body(float x, float y, float vx, float vy, float mass, float radius, SDL_Color color);

// Update physics for all bodies in the world
void update_physics(World* world, float dt);

// Handle collisions between bodies (to be implemented)
void handle_collisions(World* world);

// Apply forces to a body
void apply_force(Body* body, float fx, float fy);

#endif // PHYSICS_H 