#ifndef RECT_WORLD_H
#define RECT_WORLD_H

#include <stddef.h>

#define RG_MAX_RECTS 256
#define RG_ERROR_MAX 256

typedef struct {
    int x;
    int y;
    int width;
    int height;
} RgRect;

typedef struct {
    int plane_width;
    int plane_height;
    RgRect rects[RG_MAX_RECTS];
    size_t count;
} RgWorld;

void rg_world_init(RgWorld *world, int plane_width, int plane_height);
int rg_world_create(RgWorld *world, int x, int y, int width, int height, char *error, size_t error_size);
int rg_world_move_right(RgWorld *world, int px, int py, int delta, char *error, size_t error_size);
int rg_world_move_left(RgWorld *world, int px, int py, int delta, char *error, size_t error_size);
void rg_world_apply_gravity(RgWorld *world);
double rg_world_instability(const RgWorld *world);
int rg_world_render_ascii(const RgWorld *world, char *buffer, size_t buffer_size);

#endif
