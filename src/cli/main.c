#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rect_world.h"

static void usage(void) {
    printf("Usage:\n");
    printf("  rectangle_gravity create x,y+w,h [create ...] [moveright x,y+p] [moveleft x,y+p]\n");
}

int main(int argc, char **argv) {
    RgWorld world;
    rg_world_init(&world, 80, 25);

    if (argc <= 1) {
        usage();
        return 1;
    }

    char error[RG_ERROR_MAX] = {0};

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "create") == 0 && i + 1 < argc) {
            int x = 0;
            int y = 0;
            int w = 0;
            int h = 0;
            if (sscanf(argv[++i], "%d,%d+%d,%d", &x, &y, &w, &h) != 4) {
                fprintf(stderr, "Invalid create argument format. Expected x,y+w,h\n");
                return 1;
            }
            if (!rg_world_create(&world, x, y, w, h, error, sizeof(error))) {
                fprintf(stderr, "create failed: %s\n", error);
                return 1;
            }
            continue;
        }

        if (strcmp(argv[i], "moveright") == 0 && i + 1 < argc) {
            int x = 0;
            int y = 0;
            int p = 0;
            if (sscanf(argv[++i], "%d,%d+%d", &x, &y, &p) != 3) {
                fprintf(stderr, "Invalid moveright argument format. Expected x,y+p\n");
                return 1;
            }
            if (!rg_world_move_right(&world, x, y, p, error, sizeof(error))) {
                fprintf(stderr, "moveright failed: %s\n", error);
                return 1;
            }
            continue;
        }

        if (strcmp(argv[i], "moveleft") == 0 && i + 1 < argc) {
            int x = 0;
            int y = 0;
            int p = 0;
            if (sscanf(argv[++i], "%d,%d+%d", &x, &y, &p) != 3) {
                fprintf(stderr, "Invalid moveleft argument format. Expected x,y+p\n");
                return 1;
            }
            if (!rg_world_move_left(&world, x, y, p, error, sizeof(error))) {
                fprintf(stderr, "moveleft failed: %s\n", error);
                return 1;
            }
            continue;
        }

        fprintf(stderr, "Unknown command: %s\n", argv[i]);
        usage();
        return 1;
    }

    char buffer[2200];
    if (!rg_world_render_ascii(&world, buffer, sizeof(buffer))) {
        fprintf(stderr, "failed to render world\n");
        return 1;
    }

    printf("%s", buffer);
    printf("Instability score: %.3f\n", rg_world_instability(&world));
    return 0;
}
