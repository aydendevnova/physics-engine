#include "physics.h"
#include <math.h>

Body create_body(float x, float y, float vx, float vy, float mass, float radius, SDL_Color color) {
    return (Body){
        .x = x,
        .y = y,
        .vx = vx,
        .vy = vy,
        .ax = 0,
        .ay = 0,
        .mass = mass,
        .radius = radius,
        .color = color
    };
}

void apply_force(Body* body, float fx, float fy) {
    body->ax += fx / body->mass;
    body->ay += fy / body->mass;
}

void handle_boundary_collision(Body* body) {
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

void handle_circle_collision(Body* a, Body* b) {
    // Calculate distance between centers
    float dx = b->x - a->x;
    float dy = b->y - a->y;
    float distance = sqrtf(dx * dx + dy * dy);
    
    // Check if circles are overlapping
    float minDist = a->radius + b->radius;
    if (distance >= minDist) return;
    
    // Normalize collision vector
    float nx = dx / distance;
    float ny = dy / distance;
    
    // Calculate relative velocity
    float rvx = b->vx - a->vx;
    float rvy = b->vy - a->vy;
    
    // Calculate relative velocity along collision normal
    float velAlongNormal = rvx * nx + rvy * ny;
    
    // If objects are moving apart, don't resolve collision
    if (velAlongNormal > 0) return;
    
    // Calculate restitution (bounce)
    float e = RESTITUTION;
    
    // Calculate impulse scalar
    float j = -(1 + e) * velAlongNormal;
    j /= 1/a->mass + 1/b->mass;
    
    // Apply impulse
    float impulsex = j * nx;
    float impulsey = j * ny;
    
    a->vx -= impulsex / a->mass;
    a->vy -= impulsey / a->mass;
    b->vx += impulsex / b->mass;
    b->vy += impulsey / b->mass;
    
    // Positional correction (to prevent sinking)
    float percent = 0.8f; // penetration resolution percentage
    float slop = 0.01f;   // penetration allowance
    float penetration = minDist - distance;
    
    if (penetration > slop) {
        float correction = (penetration * percent) / (1/a->mass + 1/b->mass);
        float cx = correction * nx;
        float cy = correction * ny;
        
        a->x -= cx / a->mass;
        a->y -= cy / a->mass;
        b->x += cx / b->mass;
        b->y += cy / b->mass;
    }
}

void handle_collisions(World* world) {
    // Check each pair of bodies for collisions
    for (int i = 0; i < world->bodyCount; i++) {
        for (int j = i + 1; j < world->bodyCount; j++) {
            handle_circle_collision(&world->bodies[i], &world->bodies[j]);
        }
    }
}

void update_physics(World* world, float dt) {
    dt *= TIME_SCALE;
    
    for (int i = 0; i < world->bodyCount; i++) {
        Body* body = &world->bodies[i];
        
        // Store current acceleration for Velocity Verlet
        float old_ax = body->ax;
        float old_ay = body->ay;
        
        // Reset acceleration
        body->ax = 0;
        body->ay = 0;
        
        // Apply gravity
        apply_force(body, 0, body->mass * GRAVITY);
        
        // Update position (Velocity Verlet)
        body->x += body->vx * dt + 0.5f * old_ax * dt * dt;
        body->y += body->vy * dt + 0.5f * old_ay * dt * dt;
        
        // Update velocity
        body->vx += 0.5f * (old_ax + body->ax) * dt;
        body->vy += 0.5f * (old_ay + body->ay) * dt;
        
        // Handle collisions with boundaries
        handle_boundary_collision(body);
    }
    
    // Handle collisions between bodies
    for (int i = 0; i < COLLISION_ITERATIONS; i++) {
        handle_collisions(world);
    }
} 