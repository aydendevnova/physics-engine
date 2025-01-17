# 2D Physics Engine

A simple 2D physics engine implementation in C using SDL2 for rendering and Nuklear for debug UI. The engine simulates basic rigid body dynamics with collision detection and resolution. Not production ready, this is a hobby project.

## Features

- Velocity Verlet integration for motion
- Boundary collision handling
- Real-time debug visualization with inspector

## Building and Running

The project uses a bash build script that compiles all source files and links with SDL2. Currently it might only work on macOS with Homebrew. Build and run with:

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

## Future Considerations

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
