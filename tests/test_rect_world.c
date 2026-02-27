#include <assert.h>
#include <string.h>

#include "rect_world.h"

int main(void) {
    RgWorld world;
    rg_world_init(&world, 20, 10);

    char error[RG_ERROR_MAX] = {0};

    assert(rg_world_create(&world, 2, 6, 4, 2, error, sizeof(error)) == 1);
    assert(world.count == 1);
    assert(world.rects[0].y == 1);

    assert(rg_world_create(&world, 2, 9, 4, 2, error, sizeof(error)) == 1);
    assert(world.count == 2);
    assert(world.rects[1].y == 3);

    assert(rg_world_move_right(&world, 2, 1, 3, error, sizeof(error)) == 1);
    assert(world.rects[0].x == 5);

    assert(rg_world_move_left(&world, 5, 1, 10, error, sizeof(error)) == 0);
    assert(strstr(error, "outside") != NULL || strstr(error, "Movement") != NULL);

    double instability = rg_world_instability(&world);
    assert(instability >= 0.0 && instability <= 1.0);

    char render[1024] = {0};
    assert(rg_world_render_ascii(&world, render, sizeof(render)) == 1);
    assert(strstr(render, "#") != NULL);

    return 0;
}
