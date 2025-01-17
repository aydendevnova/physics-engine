#!/bin/bash

# Get SDL2 flags from pkg-config
SDL_CFLAGS=$(pkg-config --cflags sdl2)
SDL_LIBS=$(pkg-config --libs sdl2)

# Create build directory if it doesn't exist
mkdir -p build

# Compile with all necessary flags
clang src/main.c -o build/physics_engine \
    $SDL_CFLAGS \
    $SDL_LIBS \
    -Wall -Wextra \
    -framework OpenGL \
    -framework Cocoa

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Build successful! Run with: ./build/physics_engine"
else
    echo "Build failed!"
fi