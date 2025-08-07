#include "world.h"
#include "blocks.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>

struct World world;

void generate_world(int w, int h, int l) {
	world.data = malloc(sizeof(int) * w * h * l);
	world.w = w;
	world.h = h;
	world.l = l;

	for (int i = 0; i < w * h * l; i++)
		world.data[i] = B_AIR;

	for (int j = 0; j < 5; j++) {
		for (int t = 0; t < l; t++) {
			for (int i = 0; i < w; i++) {
				set_block(i, j, t, B_STONE);
			}
		}
	}
	for (int j = 5; j < 10; j++) {
		for (int t = 0; t < l; t++) {
			for (int i = 0; i < w; i++) {
				set_block(i, j, t, B_DIRT);
			}
		}
	}
	for (int t = 0; t < l; t++) {
		for (int i = 0; i < w; i++) {
			set_block(i, 10, t, B_GRASS);
		}
	}
}

void save_world(const char* path) {
	char header[32];
	sprintf(header, "#WWF\n%dx%dx%d\n", world.w, world.h, world.l);

	FILE* file = fopen(path, "w");
	fwrite(header, sizeof(char), strlen(header), file);
	fwrite(world.data, sizeof(char), world.w * world.h * world.l, file);
	fclose(file);
}

void load_world(const char* path) {
	FILE* file = fopen(path, "r");

	if (fscanf(file, "#WWF\n%dx%dx%d\n", &world.w, &world.h, &world.l) != 3)
		puts("failed to load world");

	world.data = malloc(sizeof(char) * world.w * world.h * world.l);

	freopen(path, "r", file);

	int index, nl = 0;
	for (index = 0; nl < 2; index++)
		if (fgetc(file) == '\n') nl++;

	for (int i = 0; i < world.w * world.h * world.l; i++)
		world.data[i] = fgetc(file);

	fclose(file);
}

void destroy_world() {
	free(world.data);
}

int get_block(int x, int y, int z) {
	if (x < 0 || y < 0 || z < 0 || x >= world.w || y >= world.h || z >= world.l) return -1;
	return world.data[x + y * world.w + z * world.w * world.h];
}

void set_block(int x, int y, int z, int block) {
	if (x < 0 || y < 0 || z < 0 || x >= world.w || y >= world.h || z >= world.l) return;
	world.data[x + y * world.w + z * world.w * world.h] = block;
}
