#ifndef UI_H
#define UI_H

#include "../core/types.h"

// Initialize the UI system
bool init_ui(World* world);

// Cleanup UI resources
void cleanup_ui(World* world);

// Update and render the UI
void update_ui(World* world);

#endif // UI_H 