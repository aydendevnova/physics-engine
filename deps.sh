#!/bin/bash

# Check if Homebrew is installed
if ! command -v brew &> /dev/null; then
    echo "Homebrew is not installed. Please install it first:"
    echo '/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"'
    exit 1
fi

# Install SDL2
echo "Installing SDL2..."
brew install sdl2 pkg-config

# Create include directory if it doesn't exist
mkdir -p include/nuklear

# Download Nuklear headers if they don't exist
if [ ! -f include/nuklear/nuklear.h ]; then
    echo "Downloading Nuklear headers..."
    curl -o include/nuklear/nuklear.h https://raw.githubusercontent.com/Immediate-Mode-UI/Nuklear/master/nuklear.h
    curl -o include/nuklear/nuklear_sdl_renderer.h https://raw.githubusercontent.com/Immediate-Mode-UI/Nuklear/master/demo/sdl_renderer/nuklear_sdl_renderer.h
fi

# Verify SDL2 installation
if pkg-config --exists sdl2; then
    echo "SDL2 installation verified"
    echo "SDL2 Include path: $(pkg-config --cflags-only-I sdl2)"
    echo "SDL2 Library path: $(pkg-config --libs sdl2)"
else
    echo "Error: SDL2 installation could not be verified"
    exit 1
fi

echo "All dependencies installed successfully!"
echo "You can now run './build.sh' to build the project"