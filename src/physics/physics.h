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

// Apply an impulse to a body
void apply_impulse(Body* body, float ix, float iy);

// Get body at position (returns NULL if no body at position)
Body* get_body_at_position(World* world, int x, int y);

// Get closest edge point and normal for a body given a click position
void get_closest_edge_info(Body* body, int click_x, int click_y, float* edge_x, float* edge_y, float* normal_x, float* normal_y);

#endif // PHYSICS_H 