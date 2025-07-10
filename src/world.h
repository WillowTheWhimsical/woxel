#ifndef WORLD_H
#define WORLD_H

#define for_world \
for (int t = 0; t < world.l; t++) { \
for (int j = 0; j < world.h; j++) { \
for (int i = 0; i < world.w; i++)

struct World {
	int* tile;
	int w, h, l;
};
extern struct World world;

void generate_world(int w, int h, int l);
void save_world(const char* path);
void load_world(const char* path);
void destroy_world();

int get_block(int x, int y, int z);
void set_block(int x, int y, int z, int block);

#endif
