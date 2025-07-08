#include "world.h"
#include "blocks.h"

#include <malloc.h>

struct World world;

void generate_world(int w, int h, int l) {
	world.tile = malloc(sizeof(int) * w * h * l);
	world.w = w;
	world.h = h;
	world.l = l;

	for (int i = 0; i < w * h * l; i++)
		world.tile[i] = B_AIR;

	for (int j = 0; j < 5; j++) {
		for (int t = 0; t < h; t++) {
			for (int i = 0; i < w; i++) {
				set_block(i, j, t, B_STONE);
			}
		}
	}
	for (int j = 5; j < 10; j++) {
		for (int t = 0; t < h; t++) {
			for (int i = 0; i < w; i++) {
				set_block(i, j, t, B_DIRT);
			}
		}
	}
	for (int t = 0; t < h; t++) {
		for (int i = 0; i < w; i++) {
			set_block(i, 10, t, B_GRASS);
		}
	}
}

void destroy_world() {
	free(world.tile);
}

int get_block(int x, int y, int z) {
	if (x < 0 || y < 0 || z < 0 || x >= world.w || y >= world.h || z >= world.l) return -1;
	return world.tile[x + y * world.w + z * world.w * world.h];
}

void set_block(int x, int y, int z, int block) {
	if (x < 0 || y < 0 || z < 0 || x >= world.w || y >= world.h || z >= world.l) return;
	world.tile[x + y * world.w + z * world.w * world.h] = block;
}
