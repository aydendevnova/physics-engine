#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build

# Get SDL2 flags from pkg-config
SDL_CFLAGS=$(pkg-config --cflags sdl2)
SDL_LIBS=$(pkg-config --libs sdl2)

# Common compiler flags
# Put our include directory first so our SDL.h is found before system ones
CFLAGS="-I./include -Wall -Wextra $SDL_CFLAGS"

# Compile source files
gcc $CFLAGS -c src/main.c -o build/main.o
gcc $CFLAGS -c src/physics/physics.c -o build/physics.o
gcc $CFLAGS -c src/render/renderer.c -o build/renderer.o
gcc $CFLAGS -c src/utils/random.c -o build/random.o
gcc $CFLAGS -c src/ui/ui.c -o build/ui.o
gcc $CFLAGS -c src/ui/nuklear_impl.c -o build/nuklear_impl.o

# Link object files
gcc build/main.o \
    build/physics.o \
    build/renderer.o \
    build/random.o \
    build/ui.o \
    build/nuklear_impl.o \
    -o build/engine \
    $SDL_LIBS \
    -lm \
    -framework OpenGL \
    -framework Cocoa

# Check if build succeeded
if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Run ./build/engine to start the application"
else
    echo "Build failed!"
fi