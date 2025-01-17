#include "ui.h"
#include <stdio.h>

bool init_ui(World* world) {
    // Initialize Nuklear
    world->nk_ctx = nk_sdl_init(world->debug_window, world->debug_renderer);
    if (!world->nk_ctx) {
        fprintf(stderr, "Failed to initialize Nuklear\n");
        return false;
    }

    // Get display scale factor for HiDPI support
    int render_w, render_h, window_w, window_h;
    float scale_x, scale_y;
    SDL_GetRendererOutputSize(world->debug_renderer, &render_w, &render_h);
    SDL_GetWindowSize(world->debug_window, &window_w, &window_h);
    scale_x = (float)render_w / window_w;
    scale_y = (float)render_h / window_h;
    float scale = scale_x; // Use x scale as they should be the same
    
    // Set up font with proper scaling
    struct nk_font_atlas* atlas;
    nk_sdl_font_stash_begin(&atlas);
    // Scale base font size (13) by the display scale factor
    struct nk_font* font = nk_font_atlas_add_default(atlas, 13.0f * scale, NULL);
    nk_sdl_font_stash_end();
    
    if (font) {
        nk_style_set_font(world->nk_ctx, &font->handle);
        
        // Scale the rest of the UI
        struct nk_style* style = &world->nk_ctx->style;
        style->window.spacing = nk_vec2(4 * scale, 4 * scale);
        style->window.padding = nk_vec2(4 * scale, 4 * scale);
        style->window.group_padding = nk_vec2(4 * scale, 4 * scale);
        style->window.popup_padding = nk_vec2(4 * scale, 4 * scale);
        style->window.combo_padding = nk_vec2(4 * scale, 4 * scale);
        style->window.scrollbar_size = nk_vec2(10 * scale, 10 * scale);
    }

    return true;
}

void cleanup_ui(World* world) {
    nk_sdl_shutdown();
}

static void draw_body_properties(struct nk_context* ctx, Body* body, int index) {
    char buffer[256];
    
    // Create a collapsible section for each body
    snprintf(buffer, sizeof(buffer), "Body %d", index);
    if (nk_tree_push_id(ctx, NK_TREE_TAB, buffer, NK_MAXIMIZED, index)) {
        // Color preview
        nk_layout_row_dynamic(ctx, 20, 2);
        nk_label(ctx, "Color:", NK_TEXT_LEFT);
        struct nk_color color = {body->color.r, body->color.g, body->color.b, 255};
        struct nk_rect bounds = nk_widget_bounds(ctx);
        nk_fill_rect(nk_window_get_canvas(ctx), bounds, 0, color);

        // Position
        nk_layout_row_dynamic(ctx, 20, 2);
        snprintf(buffer, sizeof(buffer), "X: %.2f", body->x);
        nk_label(ctx, buffer, NK_TEXT_LEFT);
        snprintf(buffer, sizeof(buffer), "Y: %.2f", body->y);
        nk_label(ctx, buffer, NK_TEXT_LEFT);

        // Velocity
        nk_layout_row_dynamic(ctx, 20, 2);
        snprintf(buffer, sizeof(buffer), "Velocity X: %.2f", body->vx);
        nk_label(ctx, buffer, NK_TEXT_LEFT);
        snprintf(buffer, sizeof(buffer), "Velocity Y: %.2f", body->vy);
        nk_label(ctx, buffer, NK_TEXT_LEFT);

        // Acceleration
        nk_layout_row_dynamic(ctx, 20, 2);
        snprintf(buffer, sizeof(buffer), "Accel X: %.2f", body->ax);
        nk_label(ctx, buffer, NK_TEXT_LEFT);
        snprintf(buffer, sizeof(buffer), "Accel Y: %.2f", body->ay);
        nk_label(ctx, buffer, NK_TEXT_LEFT);

        // Physical properties
        nk_layout_row_dynamic(ctx, 20, 2);
        snprintf(buffer, sizeof(buffer), "Mass: %.2f", body->mass);
        nk_label(ctx, buffer, NK_TEXT_LEFT);
        snprintf(buffer, sizeof(buffer), "Radius: %.2f", body->radius);
        nk_label(ctx, buffer, NK_TEXT_LEFT);

        // Calculated properties
        float speed = sqrt(body->vx * body->vx + body->vy * body->vy);
        float accel = sqrt(body->ax * body->ax + body->ay * body->ay);
        float ke = 0.5f * body->mass * speed * speed;

        nk_layout_row_dynamic(ctx, 20, 2);
        snprintf(buffer, sizeof(buffer), "Speed: %.2f", speed);
        nk_label(ctx, buffer, NK_TEXT_LEFT);
        snprintf(buffer, sizeof(buffer), "Acceleration: %.2f", accel);
        nk_label(ctx, buffer, NK_TEXT_LEFT);

        nk_layout_row_dynamic(ctx, 20, 1);
        snprintf(buffer, sizeof(buffer), "Kinetic Energy: %.2f", ke);
        nk_label(ctx, buffer, NK_TEXT_LEFT);

        nk_tree_pop(ctx);
    }
}

void update_ui(World* world) {
    // Clear background
    SDL_SetRenderDrawColor(world->debug_renderer, 35, 35, 35, 255);
    SDL_RenderClear(world->debug_renderer);

    // Get actual render dimensions for HiDPI
    int render_w, render_h;
    SDL_GetRendererOutputSize(world->debug_renderer, &render_w, &render_h);

    if (nk_begin(world->nk_ctx, "Physics Debug", 
        nk_rect(0, 0, render_w, render_h),
        NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
        
        // Simulation Overview
        nk_layout_row_dynamic(world->nk_ctx, 30, 1);
        nk_label(world->nk_ctx, "Simulation Overview", NK_TEXT_CENTERED);
        
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "Total Bodies: %d", world->bodyCount);
        nk_label(world->nk_ctx, buffer, NK_TEXT_LEFT);

        // Separator
        nk_layout_row_dynamic(world->nk_ctx, 10, 1);
        nk_spacing(world->nk_ctx, 1);

        // Bodies List
        nk_layout_row_dynamic(world->nk_ctx, 30, 1);
        nk_label(world->nk_ctx, "Physics Bodies", NK_TEXT_LEFT);

        // Draw each body's properties
        for (int i = 0; i < world->bodyCount; i++) {
            draw_body_properties(world->nk_ctx, &world->bodies[i], i);
        }
    }
    nk_end(world->nk_ctx);

    // Render UI
    nk_sdl_render(NK_ANTI_ALIASING_ON);
    
    // Present renderer
    SDL_RenderPresent(world->debug_renderer);
} 