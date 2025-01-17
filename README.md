# 2D Physics Engine

A simple 2D physics engine implementation in C using SDL2 for rendering and Nuklear for debug UI. The engine simulates basic rigid body dynamics with collision detection and resolution.

## Features

- Circle-based rigid body physics simulation
- Velocity Verlet integration for accurate motion
- Collision detection and resolution between circles
- Boundary collision handling
- Real-time debug visualization
- Properties inspector UI for all physics bodies

## Technical Details

### Physics Implementation

The core physics system uses the following components:

1. **Velocity Verlet Integration**

   - Updates positions and velocities in two steps
   - Provides more accurate results than basic Euler integration
   - Handles acceleration changes during timestep

2. **Circle Collision Detection & Resolution**

   - Checks overlapping circles using distance comparison
   - Resolves collisions with position correction
   - Applies impulse-based velocity changes
   - Uses multiple iterations for stability

3. **Force Application**
   - Gravity constant of 784.0 units/s²
   - Elastic collisions with 0.8 restitution
   - Linear force accumulation

### Visualization

The engine provides two windows:

1. Main simulation window showing the physics bodies in motion
2. Debug window with real-time physics properties:
   - Position (x,y)
   - Velocity (vx,vy)
   - Acceleration (ax,ay)
   - Mass
   - Radius
   - Speed
   - Kinetic Energy

### Key Constants

- GRAVITY: 784.0f
- RESTITUTION: 0.8f
- TIME_SCALE: 1.0f
- COLLISION_ITERATIONS: 2
- MIN_SEPARATION: 0.01f
- WINDOW_WIDTH: 800
- WINDOW_HEIGHT: 600
- DEBUG_WINDOW_WIDTH: 400
- DEBUG_WINDOW_HEIGHT: 600
- FPS_CAP: 120

## Building and Running

The project uses a bash build script that compiles all source files and links with SDL2. Build and run with:

1. Execute build script: `./build.sh`
2. Run executable: `./build/engine`

## Dependencies

- SDL2 for rendering and window management
- Nuklear for immediate mode GUI
- Standard C libraries

## Project Structure

The codebase is organized into modules:

- src/core: Core types and constants
- src/physics: Physics simulation code
- src/render: Rendering and visualization
- src/ui: Debug UI implementation
- include/nuklear: GUI framework headers
- build.sh: Build script

## Implementation Notes

The physics engine implements:

- Compound collision resolution
- Energy conservation through elastic collisions
- Real-time property inspection
- Configurable simulation parameters
- Modular architecture separating physics, rendering and UI

## Future Improvements

1. Additional collision shapes (polygons)
2. Joints and constraints
3. Sleeping bodies optimization
4. Spatial partitioning
5. Continuous collision detection
6. Angular physics (rotation)
7. Constraint-based physics
8. Particle effects
9. Advanced material properties
10. Multi-threading support
