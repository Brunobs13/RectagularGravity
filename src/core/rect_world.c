#include "rect_world.h"

#include <stdio.h>
#include <string.h>

static void write_error(char *error, size_t error_size, const char *message) {
    if (error != NULL && error_size > 0) {
        snprintf(error, error_size, "%s", message);
    }
}

static int inside_plane(const RgWorld *world, const RgRect *rect) {
    if (rect->x < 1 || rect->y < 1) {
        return 0;
    }

    if (rect->x + rect->width - 1 > world->plane_width) {
        return 0;
    }

    if (rect->y + rect->height - 1 > world->plane_height) {
        return 0;
    }

    return 1;
}

static int collides(const RgRect *a, const RgRect *b) {
    if (a->x + a->width <= b->x || b->x + b->width <= a->x) {
        return 0;
    }

    if (a->y + a->height <= b->y || b->y + b->height <= a->y) {
        return 0;
    }

    return 1;
}

static int collides_with_any(const RgWorld *world, const RgRect *candidate, int ignore_index) {
    for (size_t i = 0; i < world->count; ++i) {
        if ((int)i == ignore_index) {
            continue;
        }
        if (collides(candidate, &world->rects[i])) {
            return 1;
        }
    }

    return 0;
}

static int find_rect_at_point(const RgWorld *world, int px, int py) {
    for (size_t i = 0; i < world->count; ++i) {
        const RgRect *rect = &world->rects[i];
        if (rect->x <= px && px < rect->x + rect->width && rect->y <= py && py < rect->y + rect->height) {
            return (int)i;
        }
    }

    return -1;
}

void rg_world_init(RgWorld *world, int plane_width, int plane_height) {
    if (world == NULL) {
        return;
    }

    world->plane_width = plane_width;
    world->plane_height = plane_height;
    world->count = 0;
}

void rg_world_apply_gravity(RgWorld *world) {
    if (world == NULL) {
        return;
    }

    int moved;
    do {
        moved = 0;

        for (size_t i = 0; i < world->count; ++i) {
            while (world->rects[i].y > 1) {
                world->rects[i].y -= 1;
                if (collides_with_any(world, &world->rects[i], (int)i)) {
                    world->rects[i].y += 1;
                    break;
                }
                moved = 1;
            }
        }
    } while (moved);
}

int rg_world_create(RgWorld *world, int x, int y, int width, int height, char *error, size_t error_size) {
    if (world == NULL) {
        write_error(error, error_size, "World is NULL.");
        return 0;
    }

    if (width <= 0 || height <= 0) {
        write_error(error, error_size, "Rectangle width/height must be positive.");
        return 0;
    }

    if (world->count >= RG_MAX_RECTS) {
        write_error(error, error_size, "Maximum number of rectangles reached.");
        return 0;
    }

    RgRect candidate = {.x = x, .y = y, .width = width, .height = height};
    if (!inside_plane(world, &candidate)) {
        write_error(error, error_size, "Rectangle is outside the plane bounds.");
        return 0;
    }

    if (collides_with_any(world, &candidate, -1)) {
        write_error(error, error_size, "Rectangle collides with an existing rectangle.");
        return 0;
    }

    world->rects[world->count++] = candidate;
    rg_world_apply_gravity(world);
    return 1;
}

static int move_by_delta(RgWorld *world,
                         int px,
                         int py,
                         int delta,
                         int direction,
                         char *error,
                         size_t error_size) {
    if (world == NULL) {
        write_error(error, error_size, "World is NULL.");
        return 0;
    }

    if (delta < 0) {
        write_error(error, error_size, "Delta must be >= 0.");
        return 0;
    }

    int idx = find_rect_at_point(world, px, py);
    if (idx < 0) {
        write_error(error, error_size, "No rectangle found at the given point.");
        return 0;
    }

    RgRect original = world->rects[idx];
    world->rects[idx].x += direction * delta;

    if (!inside_plane(world, &world->rects[idx])) {
        world->rects[idx] = original;
        write_error(error, error_size, "Movement would place rectangle outside bounds.");
        return 0;
    }

    if (collides_with_any(world, &world->rects[idx], idx)) {
        world->rects[idx] = original;
        write_error(error, error_size, "Movement would cause a collision.");
        return 0;
    }

    rg_world_apply_gravity(world);
    return 1;
}

int rg_world_move_right(RgWorld *world, int px, int py, int delta, char *error, size_t error_size) {
    return move_by_delta(world, px, py, delta, 1, error, error_size);
}

int rg_world_move_left(RgWorld *world, int px, int py, int delta, char *error, size_t error_size) {
    return move_by_delta(world, px, py, delta, -1, error, error_size);
}

double rg_world_instability(const RgWorld *world) {
    if (world == NULL || world->count < 2) {
        return 0.0;
    }

    double instability_total = 0.0;
    int unstable_count = 0;

    for (size_t i = 0; i < world->count; ++i) {
        const RgRect *top = &world->rects[i];
        int supported = 0;

        for (size_t j = 0; j < world->count; ++j) {
            if (i == j) {
                continue;
            }

            const RgRect *base = &world->rects[j];
            if (top->y != base->y + base->height) {
                continue;
            }

            int left = top->x > base->x ? top->x : base->x;
            int right_top = top->x + top->width;
            int right_base = base->x + base->width;
            int right = right_top < right_base ? right_top : right_base;
            if (right > left) {
                supported += (right - left);
            }
        }

        double supported_ratio = (double)supported / (double)top->width;
        if (supported_ratio > 1.0) {
            supported_ratio = 1.0;
        }

        double unsupported_ratio = 1.0 - supported_ratio;
        if (unsupported_ratio > supported_ratio) {
            instability_total += unsupported_ratio;
            unstable_count++;
        }
    }

    if (unstable_count == 0) {
        return 0.0;
    }

    return instability_total / (double)unstable_count;
}

int rg_world_render_ascii(const RgWorld *world, char *buffer, size_t buffer_size) {
    if (world == NULL || buffer == NULL || buffer_size == 0) {
        return 0;
    }

    if (world->plane_width <= 0 || world->plane_height <= 0) {
        return 0;
    }

    const size_t required = (size_t)world->plane_height * ((size_t)world->plane_width + 1) + 1;
    if (buffer_size < required) {
        return 0;
    }

    size_t offset = 0;
    char plane[256][256];

    if (world->plane_height > 256 || world->plane_width > 256) {
        return 0;
    }

    for (int y = 0; y < world->plane_height; ++y) {
        for (int x = 0; x < world->plane_width; ++x) {
            plane[y][x] = ' ';
        }
    }

    for (size_t i = 0; i < world->count; ++i) {
        const RgRect *r = &world->rects[i];
        for (int y = r->y - 1; y < r->y + r->height - 1; ++y) {
            for (int x = r->x - 1; x < r->x + r->width - 1; ++x) {
                if (x == r->x - 1 || x == r->x + r->width - 2 || y == r->y - 1 || y == r->y + r->height - 2) {
                    plane[y][x] = '#';
                }
            }
        }
    }

    for (int y = world->plane_height - 1; y >= 0; --y) {
        for (int x = 0; x < world->plane_width; ++x) {
            buffer[offset++] = plane[y][x];
        }
        buffer[offset++] = '\n';
    }

    buffer[offset] = '\0';
    return 1;
}
